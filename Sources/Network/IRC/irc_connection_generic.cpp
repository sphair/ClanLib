/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
**    (if your name is missing here, please add it)
*/

#include "irc_connection_generic.h"
#include "API/Core/System/event_listener.h"
#include "API/Core/System/error.h"
#include "API/Core/System/log.h"

/////////////////////////////////////////////////////////////////////////////
// CL_IRCConnection_Generic construction:

CL_IRCConnection_Generic::CL_IRCConnection_Generic(const std::string &server, const std::string &port) :
	sock(CL_Socket::tcp), server(server), port(port), signal_error(false), ref_count(0)
{
	thread = CL_Thread(this);
	thread.start();
}

CL_IRCConnection_Generic::~CL_IRCConnection_Generic()
{
	thread.wait();
}

/////////////////////////////////////////////////////////////////////////////
// CL_IRCConnection_Generic attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_IRCConnection_Generic operations:

void CL_IRCConnection_Generic::add_ref()
{
	ref_count++;
}

void CL_IRCConnection_Generic::release_ref()
{
	ref_count--;
	if (ref_count == 0) delete this;
}

void CL_IRCConnection_Generic::send_data(const std::string &data)
{
	CL_MutexSection mutex_lock(&mutex);
	send_queue.push(data);
	send_trigger.set_flag();
}

/////////////////////////////////////////////////////////////////////////////
// CL_IRCConnection_Generic implementation:

void CL_IRCConnection_Generic::run()
{
	try
	{
		sock.connect(CL_IPAddress(server, port));

		std::string last_line;

		char buffer[512];
		while (true)
		{
			CL_EventListener events;
			events.add_trigger(sock.get_read_trigger());
			events.add_trigger(&quit_trigger);
			events.add_trigger(&send_trigger);
			events.wait();

			if (quit_trigger.get_flag())
				break;
				
			if (send_trigger.get_flag())
			{
				CL_MutexSection mutex_lock(&mutex);
				if (!send_queue.empty())
				{
					sock.send(send_queue.front());
					send_queue.pop();
				}
				if (!send_queue.empty())
					send_trigger.set_flag();
				else
					send_trigger.reset();
			}

			if (sock.get_read_trigger()->get_flag())
			{
				int received = sock.recv(buffer, 512);
				if (received == 0) break;
		
				int start = 0;
				for (int i=0; i<received; i++)
				{
					if (buffer[i] == '\n')
					{
						last_line.append(buffer+start, i-start+1);
						start = i+1;

		 				CL_MutexSection mutex_lock(&mutex);
						received_queue.push(last_line);
				
						last_line = std::string();
					}
				}
				last_line.append(buffer+start, received-start);
			}
		}
	}
	catch (CL_Error error)
	{
		signal_error = true;
		error_message = error.message;
	}
}
	
void CL_IRCConnection_Generic::keep_alive()
{
	CL_MutexSection mutex_lock(&mutex);
	while (!received_queue.empty())
	{
		std::string line = received_queue.front();
		received_queue.pop();
		
		// Parse line:
		std::string prefix, command;
		std::vector<std::string> args;
		line = line.substr(0, line.length()-2); // cut off CR-LF
		line.append(" "); // whitespace at end. Makes parsing easier.

		// Find low level message escapes:
		std::string::size_type pos = 0;
		pos = 0;
		while (pos != std::string::npos)
		{
			pos = line.find('\020', pos);
			if (pos == std::string::npos || pos+1 == std::string::npos) break;
			switch (line[pos+1])
			{
			case '\020':
				line.replace( pos, 2, 1, '\020' );
				break;

			case '0':
				line.replace( pos, 2, 1, '\0' );
				break;

			case 'n':
				line.replace( pos, 2, 1, '\n' );
				break;

			case 'r':
				line.replace( pos, 2, 1, '\r' );
				break;
			};
			pos++;
		}

		// Split line prefix and command:
		pos = 0;
		std::string::size_type endpos;
		if (line[0] == ':') // prefix
		{
			pos = line.find(" ", 1)+1;
			prefix = line.substr(1, pos-2);
		}
		
		endpos = line.find(" ", pos);
		command = line.substr(pos, endpos-pos);
		pos = endpos+1;
		
		while (pos != line.length())
		{
			if (line[pos] == ':')
			{
				args.push_back(line.substr(pos+1, line.length()-pos-2));
				break;
			}
		
			endpos = line.find(" ", pos);
			args.push_back(line.substr(pos, endpos-pos));
			pos = endpos+1;
		}
		
		// Handle command:
		int numeric_value = atoi(command.c_str());
		if (numeric_value != 0)
		{
			sig_numeric_reply(prefix, numeric_value, args);
			
			if (numeric_value == RPL_NAMREPLY)
			{
				const std::string &self = args[0];
				const std::string &channel = args[2];
				const std::string &users = args[3];
				std::vector<std::string> nicks;

				std::string::size_type pos = 0;
				while (pos < users.length())
				{
					std::string::size_type new_pos = users.find(" ", pos);
					if (new_pos == users.npos) new_pos = users.length();
					nicks.push_back(users.substr(pos, new_pos-pos));
					pos = new_pos+1;
				}
				
				sig_name_reply(self, channel, nicks);
			}
			
			continue;
		}
		
		sig_command_received(prefix, command, args);

		int num_args = args.size();
		if (command == "NICK" && num_args == 1) sig_nick(prefix, args[0]);
		else if (command == "JOIN" && num_args == 1) sig_join(prefix, args[0]);
		else if (command == "PART" && num_args >= 1)
		{
			if (num_args == 1) // No part reason
			{
				std::string empty;
				sig_part(prefix, args[0], empty);
			}
			else // User made comment about why it left
			{
				sig_part(prefix, args[0], args[1]);
			}
		}
		else if (command == "MODE")
		{
			std::vector<std::string> params;
			for (int i=2; i<num_args; i++) params.push_back(args[i]);
			sig_mode(prefix, args[0], args[1], params);
		}
		else if (command == "TOPIC" && num_args == 2) sig_topic(prefix, args[0], args[1]);
		else if (command == "INVITE" && num_args == 2) sig_invite(prefix, args[0], args[1]);
		else if (command == "KICK" && num_args == 2) sig_kick(prefix, args[0], args[1], "");
		else if (command == "KICK" && num_args == 3) sig_kick(prefix, args[0], args[1], args[2]);
		else if (command == "PRIVMSG" && num_args == 2)
		{
			if (args[1][0] == '\001') // ctcp
			{
				std::string line = args[1].substr(1, args[1].length()-2);

				// Find ctcp level message escapes:
				std::string::size_type pos = 0;
				pos = 0;
				while (pos != std::string::npos)
				{
					pos = line.find('\\', pos);
					if (pos == std::string::npos || pos+1 == std::string::npos) break;
					switch (line[pos+1])
					{
					case '\\':
						line.replace( pos, 2, 1, '\\' );
						break;

					case 'a':
						line.replace( pos, 2, 1, '\001' );
						break;
					};
					pos++;
				}

				std::string command, data;
				std::string::size_type data_pos = line.find(' ');
				if (data_pos == std::string::npos)
				{
					command = line;
				}
				else
				{
					command = line.substr(0, data_pos);
					data = line.substr(data_pos+1);
				}

				sig_privmsg_ctcp(prefix, args[0], command, data);
			}
			else
			{
				sig_privmsg(prefix, args[0], args[1]);
			}
		}
		else if (command == "NOTICE" && num_args == 2)
		{
			if (args[1][0] == '\001') // ctcp
			{
				std::string line = args[1].substr(1, args[1].length()-2);

				// Find ctcp level message escapes:
				std::string::size_type pos = 0;
				pos = 0;
				while (pos != std::string::npos)
				{
					pos = line.find('\\', pos);
					if (pos == std::string::npos || pos+1 == std::string::npos) break;
					switch (line[pos+1])
					{
					case '\\':
						line.replace( pos, 2, 1, '\\' );
						break;

					case 'a':
						line.replace( pos, 2, 1, '\001' );
						break;
					};
					pos++;
				}

				std::string command, data;
				std::string::size_type data_pos = line.find(' ');
				if (data_pos == std::string::npos)
				{
					command = line;
				}
				else
				{
					command = line.substr(0, data_pos);
					data = line.substr(data_pos+1);
				}

				sig_notice_ctcp(prefix, args[0], command, data);
			}
			else
			{
				sig_notice(prefix, args[0], args[1]);
			}
		}
		else if (command == "PING" && num_args == 1) sig_ping(args[0], "");
		else if (command == "PING" && num_args == 2) sig_ping(args[0], args[1]);
		else sig_unknown_command_received(prefix, command, args);
	}
	
	if (signal_error)
	{
		sig_socket_error(error_message);
		signal_error = false;
	}
}
