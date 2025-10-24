/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
*/

#include "Network/precomp.h"
#include "irc_connection_generic.h"
#include "API/Network/Socket/tcp_connection.h"
#include "API/Network/Socket/socket_name.h"
#include "API/Core/System/event.h"
#include "API/Core/System/exception.h"

#ifndef WIN32
#include "stdlib.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_IRCConnection_Generic construction:

CL_IRCConnection_Generic::CL_IRCConnection_Generic(const CL_String &server, const CL_String &port)
: server(server), port(port), signal_error(false), ref_count(0)
{
	thread.start(this, &CL_IRCConnection_Generic::thread_main);
}

CL_IRCConnection_Generic::~CL_IRCConnection_Generic()
{
	thread.join();
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

void CL_IRCConnection_Generic::send_data(const CL_String8 &data)
{
	CL_MutexSection mutex_lock(&mutex);
	send_queue.push(data);
	event_send.set();
}

void CL_IRCConnection_Generic::process_data()
{
	CL_MutexSection mutex_lock(&mutex);
	while (!received_queue.empty())
	{
		CL_String8 line = received_queue.front();
		received_queue.pop();
		
		// Parse line:
		CL_String8 prefix, command;
		std::vector<CL_String8> args;
		line = line.substr(0, line.length()-2); // cut off CR-LF
		line.append(" "); // whitespace at end. Makes parsing easier.

		// Find low level message escapes:
		CL_String8::size_type pos = 0;
		pos = 0;
		while (pos != CL_String8::npos)
		{
			pos = line.find('\020', pos);
			if (pos == CL_String8::npos || pos+1 == CL_String8::npos) break;
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
		CL_String8::size_type endpos;
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
			sig_numeric_reply.invoke(prefix, numeric_value, args);
			
			if (numeric_value == RPL_NAMREPLY)
			{
				const CL_String8 &self = args[0];
				const CL_String8 &channel = args[2];
				const CL_String8 &users = args[3];
				std::vector<CL_String8> nicks;

				CL_String8::size_type pos = 0;
				while (pos < users.length())
				{
					CL_String8::size_type new_pos = users.find(" ", pos);
					if (new_pos == users.npos) new_pos = users.length();
					nicks.push_back(users.substr(pos, new_pos-pos));
					pos = new_pos+1;
				}
				
				sig_name_reply.invoke(self, channel, nicks);
			}
			
			continue;
		}
		
		sig_command_received.invoke(prefix, command, args);

		int num_args = args.size();
		if (command == "NICK" && num_args == 1) sig_nick.invoke(prefix, args[0]);
		else if (command == "JOIN" && num_args == 1) sig_join.invoke(prefix, args[0]);
		else if (command == "PART" && num_args >= 1)
		{
			if (num_args == 1) // No part reason
			{
				CL_String8 empty;
				sig_part.invoke(prefix, args[0], empty);
			}
			else // User made comment about why it left
			{
				sig_part.invoke(prefix, args[0], args[1]);
			}
		}
		else if (command == "MODE")
		{
			std::vector<CL_String8> params;
			for (int i=2; i<num_args; i++) params.push_back(args[i]);
			sig_mode.invoke(prefix, args[0], args[1], params);
		}
		else if (command == "TOPIC" && num_args == 2) sig_topic.invoke(prefix, args[0], args[1]);
		else if (command == "INVITE" && num_args == 2) sig_invite.invoke(prefix, args[0], args[1]);
		else if (command == "KICK" && num_args == 2) sig_kick.invoke(prefix, args[0], args[1], "");
		else if (command == "KICK" && num_args == 3) sig_kick.invoke(prefix, args[0], args[1], args[2]);
		else if (command == "PRIVMSG" && num_args == 2)
		{
			if (args[1][0] == '\001') // ctcp
			{
				CL_String8 line = args[1].substr(1, args[1].length()-2);

				// Find ctcp level message escapes:
				CL_String8::size_type pos = 0;
				pos = 0;
				while (pos != CL_String8::npos)
				{
					pos = line.find('\\', pos);
					if (pos == CL_String8::npos || pos+1 == CL_String8::npos) break;
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

				CL_String8 command, data;
				CL_String8::size_type data_pos = line.find(' ');
				if (data_pos == CL_String8::npos)
				{
					command = line;
				}
				else
				{
					command = line.substr(0, data_pos);
					data = line.substr(data_pos+1);
				}

				sig_privmsg_ctcp.invoke(prefix, args[0], command, data);
			}
			else
			{
				sig_privmsg.invoke(prefix, args[0], args[1]);
			}
		}
		else if (command == "NOTICE" && num_args == 2)
		{
			if (args[1][0] == '\001') // ctcp
			{
				CL_String8 line = args[1].substr(1, args[1].length()-2);

				// Find ctcp level message escapes:
				CL_String8::size_type pos = 0;
				pos = 0;
				while (pos != CL_String8::npos)
				{
					pos = line.find('\\', pos);
					if (pos == CL_String8::npos || pos+1 == CL_String8::npos) break;
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

				CL_String8 command, data;
				CL_String8::size_type data_pos = line.find(' ');
				if (data_pos == CL_String8::npos)
				{
					command = line;
				}
				else
				{
					command = line.substr(0, data_pos);
					data = line.substr(data_pos+1);
				}

				sig_notice_ctcp.invoke(prefix, args[0], command, data);
			}
			else
			{
				sig_notice.invoke(prefix, args[0], args[1]);
			}
		}
		else if (command == "PING" && num_args == 1) sig_ping.invoke(args[0], "");
		else if (command == "PING" && num_args == 2) sig_ping.invoke(args[0], args[1]);
		else sig_unknown_command_received.invoke(prefix, command, args);
	}
	
	if (signal_error)
	{
		sig_socket_error.invoke(error_message);
		signal_error = false;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_IRCConnection_Generic implementation:

void CL_IRCConnection_Generic::thread_main()
{
	try
	{
		bool result = connection.connect(CL_SocketName(server, port));
		if (result == false)
			throw CL_Exception(cl_text("Connection failed!"));

		CL_String8 last_line;

		char buffer[512];
		while (true)
		{
			CL_Event event_read = connection.get_read_event();
			int wait_result = CL_Event::wait(event_read, event_quit, event_send);
			if (wait_result < 0)
				throw CL_Exception(cl_text("Unexpected return value from CL_Event::wait!"));

			if (wait_result == 1) // event_quit
				break;
				
			if (wait_result == 2) // event_send
			{
				CL_MutexSection mutex_lock(&mutex);
				if (!send_queue.empty())
				{
					connection.send(send_queue.front().data(), send_queue.front().size());
					send_queue.pop();
				}
				if (!send_queue.empty())
					event_send.set();
				else
					event_send.reset();
			}

			if (wait_result == 0) // connection.get_read_event()
			{
				int received = connection.receive(buffer, 512, false);
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
				
						last_line = CL_String8();
					}
				}
				last_line.append(buffer+start, received-start);
			}
		}
	}
	catch (CL_Exception error)
	{
		signal_error = true;
		error_message = error.message;
	}
}
