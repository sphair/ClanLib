
#include "precomp.h"
#ifndef WIN32
#include <arpa/inet.h> // htonl
#endif
#include "chat.h"
#include "chatline.h"
#include "ircconnection.h"

/////////////////////////////////////////////////////////////////////////////
// Chat construction:

Chat::Chat(IRCConnection *connection, const std::string &filter, CL_Component *parent) :
	CL_Component(parent),
	connection(connection),
	filter(filter),
	font("InputBox/font", get_style_manager()->get_resources())
{
	scroll = new CL_ScrollBar(0, 0, false, this);
	scroll->set_width(16);
	scroll->set_tracking(true);
//	scroll->set_fixed_length_mode(true);
//	scroll->set_slider_length(1);

	slots.connect(sig_resize(), this, &Chat::on_resize);
	slots.connect(sig_paint(), this, &Chat::on_paint);
	slots.connect(scroll->sig_value_changed(), this, &Chat::on_scroll_value_changed);
	slots.connect(connection->sig_command_received(), this, &Chat::on_connection_command_received);
	slots.connect(connection->sig_unknown_command_received(), this, &Chat::on_connection_unknown_command_received);
	slots.connect(connection->sig_numeric_reply(), this, &Chat::on_connection_numeric_reply);
	slots.connect(connection->sig_name_reply(), this, &Chat::on_connection_name_reply);
	slots.connect(connection->sig_nick(), this, &Chat::on_connection_nick);
	slots.connect(connection->sig_join(), this, &Chat::on_connection_join);
	slots.connect(connection->sig_part(), this, &Chat::on_connection_part);
	slots.connect(connection->sig_mode(), this, &Chat::on_connection_mode);
	slots.connect(connection->sig_topic(), this, &Chat::on_connection_topic);
	slots.connect(connection->sig_invite(), this, &Chat::on_connection_invite);
	slots.connect(connection->sig_kick(), this, &Chat::on_connection_kick);
	slots.connect(connection->sig_privmsg(), this, &Chat::on_connection_privmsg);
	slots.connect(connection->sig_notice(), this, &Chat::on_connection_notice);
	slots.connect(connection->sig_privmsg_ctcp(), this, &Chat::on_connection_privmsg_ctcp);
	slots.connect(connection->sig_notice_ctcp(), this, &Chat::on_connection_notice_ctcp);
	slots.connect(connection->sig_ping(), this, &Chat::on_connection_ping);
	slots.connect(connection->sig_typepacket, this, &Chat::on_typepacket);

	color_text = CL_Color(0,0,0);
	color_channel = CL_Color(0,255,0);
	color_system = CL_Color(0,0,255);
}

Chat::~Chat()
{
	std::list<ChatLine *>::iterator it;
	for (it = lines.begin(); it != lines.end(); ++it) delete *it;
}

/////////////////////////////////////////////////////////////////////////////
// Chat attributes:


/////////////////////////////////////////////////////////////////////////////
// Chat operations:

void Chat::add_line(const std::string &text, const CL_Color &color)
{
	add_line("", text, color);
}

void Chat::add_line(const std::string &nick, const std::string &text, const CL_Color &color)
{
	ChatLine *line = new ChatLine;
	line->nick = nick;
	line->text = text;
	line->color = color;
	lines.push_back(line);

	if (lines.size() >= 1)
	{
		scroll->set_max_value(scroll->get_max_value()+1);
		if (scroll->get_value() == scroll->get_max_value()-1) scroll->set_value(scroll->get_max_value());
	
		//signal that we want to be repainted
		update();
	}

}

/////////////////////////////////////////////////////////////////////////////
// Chat implementation:

void Chat::on_resize(int old_width, int old_height)
{
	scroll->set_position(CL_Rect(get_width()-scroll->get_width(), 0, get_width(), get_height()));
}

void Chat::on_paint()
{
	CL_Gradient gradient(
		CL_Color::aliceblue, CL_Color::lightskyblue, CL_Color::aqua, CL_Color::deepskyblue);
	CL_Display::fill_rect(CL_Rect(0, 0, get_width()-scroll->get_width(), get_height()), gradient);

	int border = 5;
	int y = get_height() - border;

	int indent = 120;

	std::map<std::string, std::string>::iterator it_typetext;
	for (it_typetext = typetext.begin(); it_typetext != typetext.end(); ++it_typetext)
	{
		std::string nick = it_typetext->first;
		std::string text = it_typetext->second;

		// Skip empty lines:
		if (text.empty()) continue;

		font.set_color(
			color_system.get_red()/255.0f,
			color_system.get_green()/255.0f,
			color_system.get_blue()/255.0f,
			color_system.get_alpha()/255.0f);

		CL_Size nick_size = font.get_size("** "+nick+": ");
		if (nick_size.width < indent) nick_size.width = indent;

		CL_Size bounding_size = font.get_size(text, CL_Size(get_width()-nick_size.width-2*border, 0));
		y -= bounding_size.height + 2;

		if (!nick.empty()) font.draw(border, y, "** "+nick+": ");
		font.draw(CL_Rect(CL_Point(border+nick_size.width, y), bounding_size), text);
	}

	y -= 5;

	int skip_lines = scroll->get_max_value()-scroll->get_value();

	std::list<ChatLine *>::reverse_iterator it;
	for (it = lines.rbegin(); it != lines.rend(); ++it)
	{
		if (skip_lines > 0)
		{
			skip_lines--;
			continue;
		}

		ChatLine *line = *it;

		font.set_color(
			line->color.get_red()/255.0f,
			line->color.get_green()/255.0f,
			line->color.get_blue()/255.0f,
			line->color.get_alpha()/255.0f);

		CL_Size nick_size = font.get_size("<"+line->nick+"> ");
		if (nick_size.width < indent) nick_size.width = indent;

		CL_Size bounding_size = font.get_size(line->text, CL_Size(get_width()-nick_size.width-2*border, 0));
		y -= bounding_size.height + 2;

		if (!line->nick.empty()) font.draw(border, y, "<"+line->nick+">");
		font.draw(CL_Rect(CL_Point(border+nick_size.width, y), bounding_size), line->text);
	}
}

void Chat::on_scroll_value_changed(int value)
{
}

void Chat::on_connection_command_received(const std::string &prefix, const std::string &command, const std::vector<std::string> &params)
{
}

void Chat::on_connection_unknown_command_received(const std::string &prefix, const std::string &command, const std::vector<std::string> &params)
{
	if (filter.empty())
	{
		add_line(CL_IRCConnection::extract_nick(prefix), CL_String::format("Unknown command %1 received.", command), color_system);
	}
}

void Chat::on_connection_numeric_reply(const std::string &prefix, int command, const std::vector<std::string> &params)
{
	if (filter.empty())
	{
		std::string param_line;
		for (unsigned int i=0; i<params.size(); i++) param_line += params[i] + " ";
		add_line(CL_IRCConnection::extract_nick(prefix), CL_String::format("[%1] %2", CL_String::from_int(command), param_line), color_system);
	}
}

void Chat::on_connection_name_reply(const std::string &self, const std::string &channel, const std::vector<std::string> &users)
{
}

void Chat::on_connection_nick(const std::string &old_nick, const std::string &new_nick)
{
	add_line(CL_String::format("%1 is now known as %2", CL_IRCConnection::extract_nick(old_nick), CL_IRCConnection::extract_nick(new_nick)), color_channel);
}

void Chat::on_connection_join(const std::string &nick, const std::string &channel)
{
	if (channel == filter)
	{
		add_line(CL_String::format("%1 joined %2", CL_IRCConnection::extract_nick(nick), channel), color_channel);
	}
}

void Chat::on_connection_part(const std::string &nick, const std::string &channel, const std::string &reason)
{
	if (channel == filter)
	{
		if (reason.empty())
		{
			add_line(CL_String::format("%1 left %2", CL_IRCConnection::extract_nick(nick), channel), color_channel);
		}
		else
		{
			add_line(CL_String::format("%1 left %2 (%3)", CL_IRCConnection::extract_nick(nick), channel, reason), color_channel);
		}
	}
}

void Chat::on_connection_mode(const std::string &prefix, const std::string &receiver, const std::string &mode, const std::vector<std::string> &params)
{
	add_line(CL_String::format("%1 sets mode %3 on %2", CL_IRCConnection::extract_nick(prefix), CL_IRCConnection::extract_nick(receiver), mode), color_channel);
}

void Chat::on_connection_topic(const std::string &prefix, const std::string &channel, const std::string &topic)
{
	if (channel == filter)
	{
		add_line(CL_String::format("Topic on %2 is '%3', set by %1", CL_IRCConnection::extract_nick(prefix), channel, topic), color_channel);
	}
}

void Chat::on_connection_invite(const std::string &prefix, const std::string &nick, const std::string &channel)
{
	add_line(CL_String::format("%1 invites %2 to %3", CL_IRCConnection::extract_nick(prefix), CL_IRCConnection::extract_nick(nick), channel), color_channel);
}

void Chat::on_connection_kick(const std::string &prefix, const std::string &channel, const std::string &user, const std::string &comment)
{
	if (channel == filter || filter.empty())
	{
		add_line(CL_String::format("%1 kicked %3 from %2 (%4)", CL_IRCConnection::extract_nick(prefix), channel, CL_IRCConnection::extract_nick(user), comment), color_channel);
	}
}

void Chat::on_connection_privmsg(const std::string &prefix, const std::string &receiver, const std::string &text)
{
	if (receiver == filter)
	{
		add_line(CL_IRCConnection::extract_nick(prefix), text, color_text);
	}
}

void Chat::on_connection_notice(const std::string &prefix, const std::string &receiver, const std::string &text)
{
	if (receiver == filter || receiver.substr(0, 1) != "#")
	{
		add_line(CL_IRCConnection::extract_nick(prefix), text, color_text);
	}
}

void Chat::on_connection_privmsg_ctcp(const std::string &prefix, const std::string &receiver, const std::string &command, const std::string &data)
{
	std::string nick = prefix.substr(0,prefix.find("!"));

	if (command == "ACTION")
	{
		if (filter == receiver)
		{
			add_line(CL_String::format("*%1 %2", CL_IRCConnection::extract_nick(nick), data), color_text);
		}
		return;
	}

	// Only process ctcp for status window.
	// (actually should do this elsewhere (ie ircconnection), but its easiest to keep it here for now)
	if (!filter.empty()) return;

	if (command == "DCC")
	{
		add_line(CL_String::format("%1 DCC line: %2", CL_IRCConnection::extract_nick(nick), data), color_text);

		std::vector<std::string> tokens = CL_String::tokenize(data, " ");
		if (tokens.size() >= 4 && tokens[0] == "SEND")
		{
			std::string filename = "dcc_received_file"; // tokens[1];
			unsigned int address = CL_String::to_int(tokens[2]);
			std::string ip_address = CL_String::format(
				"%1.%2.%3.%4",
				CL_String::from_int((address>>24)&0xff),
				CL_String::from_int((address>>16)&0xff),
				CL_String::from_int((address>>8)&0xff),
				CL_String::from_int(address&0xff));
			std::string port = tokens[3];
			int size = 0;
			if (tokens.size() >= 5) size = CL_String::to_int(tokens[4]);

			try
			{
				CL_Socket sock(CL_Socket::tcp);
				
				sock.connect(CL_IPAddress(ip_address, port));

				CL_OutputSource_File outputfile(filename);
				char buffer[64*1024];
				while (true)
				{
					int data_read = sock.input.read(buffer, 64*1024);
					if (data_read == 0) break;
					outputfile.write(buffer, data_read);
					sock.output.write_int32(htonl(data_read));
				}
			}
			catch (CL_Error err)
			{
				add_line(CL_String::format("%1 DCC failed: %2", CL_IRCConnection::extract_nick(nick), err.message), color_text);
			}
		}
		else if (tokens.size() == 4 && tokens[0] == "CHAT" && tokens[1] == "chat")
		{
			// Todo: connect and establish dedicated chat connection.
		}
	}
	else if (command == "FINGER")
	{
		if (data.empty())
		{
			connection->send_privmsg_ctcp(nick, "FINGER", ":ClanLib's Example IRC Client");
		}
		else
		{
			add_line(CL_String::format("%1 FINGER REPLY: %2", CL_IRCConnection::extract_nick(nick), data.substr(1)), color_text);
		}
	}
	else if (command == "VERSION")
	{
		if (data.empty())
		{
#ifdef WIN32
			connection->send_privmsg_ctcp(nick, "VERSION", "CTalk:0.1:Windows");
#else
			connection->send_privmsg_ctcp(nick, "VERSION", "CTalk:0.1:Unix");
#endif
		}
		else
		{
			add_line(CL_String::format("%1 VERSION REPLY: %2", CL_IRCConnection::extract_nick(nick), data), color_text);
		}
	}
	else if (command == "USERINFO")
	{
		if (data.empty())
		{
			connection->send_privmsg_ctcp(nick, "USERINFO", ":A ClanLib User");
		}
		else
		{
			add_line(CL_String::format("%1 USERINFO REPLY: %2", CL_IRCConnection::extract_nick(nick), data.substr(1)), color_text);
		}
	}
	else if (command == "CLIENTINFO")
	{
		if (data.empty())
		{
			connection->send_privmsg_ctcp(nick, "CLIENTINFO", "ACTION DCC FINGER VERSION USERINFO CLIENTINFO PING TIME");
		}
		else
		{
			add_line(CL_String::format("%1 CLIENTINFO REPLY: %2", CL_IRCConnection::extract_nick(nick), data.substr(1)), color_text);
		}
	}
	else if (command == "PING")
	{
		connection->send_notice_ctcp(nick, "PING", data);
	}
	else if (command == "TIME")
	{
		if (data.empty())
		{
			connection->send_privmsg_ctcp(nick, "TIME", "I havn't got the slightest clue what the time is!");
		}
		else
		{
			add_line(CL_String::format("%1 TIME REPLY: %2", CL_IRCConnection::extract_nick(nick), data.substr(1)), color_text);
		}
	}
	else
	{
		add_line(CL_String::format("%1 CTCP %2: %3", CL_IRCConnection::extract_nick(nick), command, data), color_text);
	}
}

void Chat::on_connection_notice_ctcp(const std::string &prefix, const std::string &receiver, const std::string &command, const std::string &data)
{
	if (command == "PING")
	{
		add_line(CL_String::format("%1 PING: %2 ms", CL_IRCConnection::extract_nick(prefix), CL_String::from_int(CL_System::get_time()-CL_String::to_int(data))), color_text);
	}
	else if (filter.empty())
	{
		on_connection_privmsg_ctcp(prefix, receiver, command, data);
	}
}

void Chat::on_connection_ping(const std::string &daemon1, const std::string &daemon2)
{
	add_line("Ping...uh pong!? - we gotta clone mirc's most important feature ;)", color_system);
}

void Chat::on_typepacket(const std::string &from, const std::string &receiver, const std::string &text)
{
	if (receiver == filter)
	{
		typetext[from] = text;
	}
}
