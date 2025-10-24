
#include "precomp.h"
#ifndef WIN32
#include <arpa/inet.h> // htonl
#endif
#include "chat.h"
#include "chatline.h"
#include "ircconnection.h"

/////////////////////////////////////////////////////////////////////////////
// Chat construction:

Chat::Chat(IRCConnection *connection, const CL_String &filter, CL_GUIComponent *parent)
: CL_GUIComponent(parent), connection(connection), filter(filter), font(get_gc(), get_font_description())
{
	scroll = new CL_ScrollBar(this);
	scroll->set_vertical();
	scroll->set_geometry(CL_Rect(get_width() - 16, 0, get_width(), get_height()));

	func_render().set(this, &Chat::on_render);
	func_resized().set(this, &Chat::on_resize);
	scroll->func_scroll().set(this, &Chat::on_scroll);
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

	color_text = CL_Colorf(0.0f,0.0f,0.0f);
	color_channel = CL_Colorf(0.0f,1.0f,0.0f);
	color_system = CL_Colorf(0.0f,0.0f,1.0f);
}

CL_FontDescription Chat::get_font_description()
{
	CL_FontDescription desc;
	desc.set_typeface_name("Courier New");
	desc.set_height(-13);
	return desc;
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

void Chat::add_line(const CL_String &text, const CL_Colorf &color)
{
	add_line("", text, color);
}

void Chat::add_line(const CL_String &nick, const CL_String &text, const CL_Colorf &color)
{
	ChatLine *line = new ChatLine;
	line->nick = nick;
	line->text = text;
	line->color = color;
	lines.push_back(line);

	if (lines.size() >= 1)
	{
		scroll->set_max(lines.size());
		// if (scroll->get_position() == scroll->get_max()-2)
			scroll->set_position(scroll->get_max()-1);
	}
	invalidate_rect();
}

/////////////////////////////////////////////////////////////////////////////
// Chat implementation:

void Chat::on_resize()
{
	scroll->set_geometry(CL_Rect(get_width()-scroll->get_width(), 0, get_width(), get_height()));
}

void Chat::on_render(CL_GraphicContext &gc, const CL_Rect &clip_rect)
{
	int border = 5;
	int y = get_height() - border;
	CL_Rect client_area(0, 0, get_width(), get_height() - border);

	int indent = 120;

	std::map<CL_String, CL_String>::iterator it_typetext;
	for (it_typetext = typetext.begin(); it_typetext != typetext.end(); ++it_typetext)
	{
		CL_String nick = it_typetext->first;
		CL_String text = it_typetext->second;

		// Skip empty lines:
		if (text.empty())
			continue;

		CL_Size nick_size = font.get_text_size(gc, "** "+nick+": ");
		if (nick_size.width < indent) nick_size.width = indent;

		CL_Size bounding_size = font.get_text_size(gc, text);//, CL_Size(get_width()-nick_size.width-2*border, 0));
		y -= bounding_size.height + 2;

		if (!nick.empty()) font.draw_text(gc, border, y, "** "+nick+": ", color_system);
		//font.draw_text(gc, CL_Rect(CL_Point(border+nick_size.width, y), bounding_size), text, color_system);
		font.draw_text(gc, border+nick_size.width, y, text, color_system);
	}

	// y -= 5;

	int skip_lines = scroll->get_max()-(scroll->get_position()+1);

	std::list<ChatLine *>::reverse_iterator it;
	for (it = lines.rbegin(); it != lines.rend(); ++it)
	{
		if (y < 0)
			break;

		if (skip_lines > 0)
		{
			skip_lines--;
			continue;
		}

		ChatLine *line = *it;
		if (line->layout_width != client_area.get_width())
		{
			line->layout.clear();
			if (!line->nick.empty())
				line->layout.add_text("<"+line->nick+"> ", font, line->color);
			line->layout.add_text(line->text, font, line->color);
			line->layout.layout(gc, client_area.get_width()-border-scroll->get_width());
			line->layout_width = client_area.get_width();
		}

		CL_Size nick_size = font.get_text_size(gc, "<"+line->nick+"> ");
		if (nick_size.width < indent) nick_size.width = indent;

		y -= line->layout.get_size().height;

		line->layout.set_position(CL_Point(border, y));
		line->layout.draw_layout(gc);
	}

	CL_FontMetrics font_metrics = font.get_font_metrics(gc);
	int page_step = cl_max(1, get_height()/static_cast<int>(font_metrics.get_height() + font_metrics.get_external_leading()));
	if (scroll->get_page_step() != page_step)
		scroll->set_ranges(0, lines.empty() ? 1 : lines.size(), 1, page_step);
}

void Chat::on_scroll()
{
	invalidate_rect();
}

void Chat::on_connection_command_received(const CL_String &prefix, const CL_String &command, const std::vector<CL_String> &params)
{
}

void Chat::on_connection_unknown_command_received(const CL_String &prefix, const CL_String &command, const std::vector<CL_String> &params)
{
	if (filter.empty() && command != "QUIT")
	{
		add_line(CL_IRCConnection::extract_nick(prefix), cl_format("Unknown command %1 received.", command), color_system);
	}
}

void Chat::on_connection_numeric_reply(const CL_String &prefix, int command, const std::vector<CL_String> &params)
{
	if (filter.empty())
	{
		CL_String param_line;
		for (unsigned int i=0; i<params.size(); i++) param_line += params[i] + " ";
		add_line(CL_IRCConnection::extract_nick(prefix), cl_format("[%1] %2", CL_StringHelp::int_to_text(command), param_line), color_system);
	}
}

void Chat::on_connection_name_reply(const CL_String &self, const CL_String &channel, const std::vector<CL_String> &users)
{
}

void Chat::on_connection_nick(const CL_String &old_nick, const CL_String &new_nick)
{
	add_line(cl_format("%1 is now known as %2", CL_IRCConnection::extract_nick(old_nick), CL_IRCConnection::extract_nick(new_nick)), color_channel);
}

void Chat::on_connection_join(const CL_String &nick, const CL_String &channel)
{
	if (channel == filter)
	{
		add_line(cl_format("%1 joined %2", CL_IRCConnection::extract_nick(nick), channel), color_channel);
	}
}

void Chat::on_connection_part(const CL_String &nick, const CL_String &channel, const CL_String &reason)
{
	if (channel == filter)
	{
		if (reason.empty())
		{
			add_line(cl_format("%1 left %2", CL_IRCConnection::extract_nick(nick), channel), color_channel);
		}
		else
		{
			add_line(cl_format("%1 left %2 (%3)", CL_IRCConnection::extract_nick(nick), channel, reason), color_channel);
		}
	}
}

void Chat::on_connection_mode(const CL_String &prefix, const CL_String &receiver, const CL_String &mode, const std::vector<CL_String> &params)
{
	add_line(cl_format("%1 sets mode %3 on %2", CL_IRCConnection::extract_nick(prefix), CL_IRCConnection::extract_nick(receiver), mode), color_channel);
}

void Chat::on_connection_topic(const CL_String &prefix, const CL_String &channel, const CL_String &topic)
{
	if (channel == filter)
	{
		add_line(cl_format("Topic on %2 is '%3', set by %1", CL_IRCConnection::extract_nick(prefix), channel, topic), color_channel);
	}
}

void Chat::on_connection_invite(const CL_String &prefix, const CL_String &nick, const CL_String &channel)
{
	add_line(cl_format("%1 invites %2 to %3", CL_IRCConnection::extract_nick(prefix), CL_IRCConnection::extract_nick(nick), channel), color_channel);
}

void Chat::on_connection_kick(const CL_String &prefix, const CL_String &channel, const CL_String &user, const CL_String &comment)
{
	if (channel == filter || filter.empty())
	{
		add_line(cl_format("%1 kicked %3 from %2 (%4)", CL_IRCConnection::extract_nick(prefix), channel, CL_IRCConnection::extract_nick(user), comment), color_channel);
	}
}

void Chat::on_connection_privmsg(const CL_String &prefix, const CL_String &receiver, const CL_String &text)
{
	if (receiver == filter)
	{
		add_line(CL_IRCConnection::extract_nick(prefix), text, color_text);
	}
}

void Chat::on_connection_notice(const CL_String &prefix, const CL_String &receiver, const CL_String &text)
{
	if (receiver == filter || receiver.substr(0, 1) != "#")
	{
		add_line(CL_IRCConnection::extract_nick(prefix), text, color_text);
	}
}

void Chat::on_connection_privmsg_ctcp(const CL_String &prefix, const CL_String &receiver, const CL_String &command, const CL_String &data)
{
	CL_String nick = prefix.substr(0,prefix.find("!"));

	if (command == "ACTION")
	{
		if (filter == receiver)
		{
			add_line(cl_format("* %1 %2", CL_IRCConnection::extract_nick(nick), data), color_text);
		}
		return;
	}

	// Only process ctcp for status window.
	// (actually should do this elsewhere (ie ircconnection), but its easiest to keep it here for now)
	if (!filter.empty()) return;

	if (command == "DCC")
	{
		add_line(cl_format("%1 DCC line: %2", CL_IRCConnection::extract_nick(nick), data), color_text);

		std::vector<CL_TempString> tokens = CL_StringHelp::split_text(data, " ");
		if (tokens.size() >= 4 && tokens[0] == "SEND")
		{
			CL_String filename = "dcc_received_file"; // tokens[1];
			unsigned int address = CL_StringHelp::text_to_int(tokens[2]);
			CL_String ip_address = cl_format(
				"%1.%2.%3.%4",
				CL_StringHelp::int_to_text((address>>24)&0xff),
				CL_StringHelp::int_to_text((address>>16)&0xff),
				CL_StringHelp::int_to_text((address>>8)&0xff),
				CL_StringHelp::int_to_text(address&0xff));
			CL_String port = tokens[3];
			int size = 0;
			if (tokens.size() >= 5) size = CL_StringHelp::text_to_int(tokens[4]);

			try
			{
				CL_TCPConnection sock;
				sock.connect(CL_SocketName(ip_address, port));

				CL_File outputfile(filename, CL_File::create_always);
				char buffer[64*1024];
				while (true)
				{
					int data_read = sock.read(buffer, 64*1024);
					if (data_read == 0) break;
					outputfile.write(buffer, data_read);
					sock.write_int32(htonl(data_read));
				}
			}
			catch (CL_Exception& exception)
			{
				add_line(cl_format("%1 DCC failed: %2", CL_IRCConnection::extract_nick(nick), exception.message), color_text);
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
			add_line(cl_format("%1 FINGER REPLY: %2", CL_IRCConnection::extract_nick(nick), data.substr(1)), color_text);
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
			add_line(cl_format("%1 VERSION REPLY: %2", CL_IRCConnection::extract_nick(nick), data), color_text);
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
			add_line(cl_format("%1 USERINFO REPLY: %2", CL_IRCConnection::extract_nick(nick), data.substr(1)), color_text);
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
			add_line(cl_format("%1 CLIENTINFO REPLY: %2", CL_IRCConnection::extract_nick(nick), data.substr(1)), color_text);
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
			add_line(cl_format("%1 TIME REPLY: %2", CL_IRCConnection::extract_nick(nick), data.substr(1)), color_text);
		}
	}
	else
	{
		add_line(cl_format("%1 CTCP %2: %3", CL_IRCConnection::extract_nick(nick), command, data), color_text);
	}
}

void Chat::on_connection_notice_ctcp(const CL_String &prefix, const CL_String &receiver, const CL_String &command, const CL_String &data)
{
	if (command == "PING")
	{
		add_line(cl_format("%1 PING: %2 ms", CL_IRCConnection::extract_nick(prefix), CL_StringHelp::int_to_text(CL_System::get_time()-CL_StringHelp::text_to_int(data))), color_text);
	}
	else if (filter.empty())
	{
		on_connection_privmsg_ctcp(prefix, receiver, command, data);
	}
}

void Chat::on_connection_ping(const CL_String &daemon1, const CL_String &daemon2)
{
}

void Chat::on_typepacket(const CL_String &from, const CL_String &receiver, const CL_String &text)
{
	if (receiver == filter)
	{
		typetext[from] = text;
	}
}
