
#include "precomp.h"
#include "chatview.h"
#include "chat.h"
#include "ircconnection.h"

/////////////////////////////////////////////////////////////////////////////
// ChatView construction:

ChatView::ChatView(IRCConnection *connection, const CL_String &filter, CL_GUIComponent *parent, MainFrame *mainframe)
: View(parent, mainframe, filter.empty() ? "Status" : filter), chat(0), inputbox(0), userlist(0), connection(connection)
{
	chat = new Chat(connection, filter, this);
	inputbox = new CL_LineEdit(this);
	userlist = new CL_ListView(this);
	inputbox->set_focus();

	userlist->get_header()->append(userlist->get_header()->create_column("nick", "Name"));

	// Only show userlist for channels:
	if (!is_channel())
		userlist->set_visible(false);

	func_resized().set(this, &ChatView::on_resize);
	inputbox->func_unhandled_event().set(this, &ChatView::on_inputbox_unhandled_input);
/*
	slots.connect(inputbox->sig_changed(), this, &ChatView::on_inputbox_changed);
	slots.connect(userlist->sig_activated(), this, &ChatView::on_userlist_activated);
	slots.connect(userlist->sig_key_up(), this, &ChatView::on_userlist_key_up);
	slots.connect(userlist->sig_mouse_up(), this, &ChatView::on_userlist_mouse_up);
*/
	slots.connect(connection->sig_unknown_command_received(), this, &ChatView::on_connection_unknown_command_received);
	slots.connect(connection->sig_numeric_reply(), this, &ChatView::on_connection_numeric_reply);
	slots.connect(connection->sig_nick(), this, &ChatView::on_connection_nick);
	slots.connect(connection->sig_join(), this, &ChatView::on_connection_join);
	slots.connect(connection->sig_part(), this, &ChatView::on_connection_part);

	on_resize();
}

ChatView::~ChatView()
{
}

/////////////////////////////////////////////////////////////////////////////
// ChatView attributes:

bool ChatView::is_channel() const
{
	return get_filter().substr(0, 1) == "#";
}

/////////////////////////////////////////////////////////////////////////////
// ChatView operations:


/////////////////////////////////////////////////////////////////////////////
// ChatView implementation:

void ChatView::on_resize()
{
	int inputbox_height = 16;
	int userlist_width = 150;

	if (!is_channel())
		userlist_width = 0;

	chat->set_geometry(CL_Rect(0, 0, get_width()-userlist_width, get_height()-inputbox_height));
	userlist->set_geometry(CL_Rect(get_width()-userlist_width, 0, get_width(), get_height()-inputbox_height));
	inputbox->set_geometry(CL_Rect(0, get_height()-inputbox_height, get_width(), get_height()));
}

bool ChatView::on_inputbox_unhandled_input(CL_InputEvent input_event)
{
	if (input_event.id == CL_KEY_ENTER)
	{
		on_inputbox_return_pressed();
		return true;
	}
	else
	{
		return false;
	}
}

void ChatView::on_inputbox_return_pressed()
{
	CL_String command_line = inputbox->get_text();
	if (command_line.empty()) return;

	if (command_line[0] != '/') // not a command
	{
		connection->send_privmsg(chat->get_filter(), command_line);
		chat->add_line(connection->get_nick(), command_line, chat->color_text);
	}
	else // its a command
	{
		std::vector<CL_TempString> args = CL_StringHelp::split_text(command_line, " ");

		if (args[0] == "/nick")
		{
			if (args.size() == 2)
			{
				connection->send_nick(args[1]);
			}
			else
			{
				chat->add_line("syntax: /nick <new nick>", chat->color_system);
			}
		}
		else if (args[0] == "/oper")
		{
			// void send_oper(const CL_String &user, const CL_String &password);
		}
		else if (args[0] == "/quit")
		{
			if (args.size() == 1)
			{
				connection->send_quit("ClanLib - the biggest overkill library ever written for an IRC client!");
			}
			else if (args.size() >= 2)
			{
				CL_String reason;
				for (unsigned int i=1; i<args.size(); i++)
				{
					if (i > 1) reason += " ";
					reason += args[i];
				}

				connection->send_quit(reason);
			}

			exit_with_code(1);
		}
		else if (args[0] == "/join")
		{
			if (args.size() == 2)
			{
				connection->send_join(args[1]);
			}
			else if (args.size() == 3)
			{
				connection->send_join(args[1], args[2]);
			}
			else
			{
				chat->add_line("syntax: /join <channel> [key]", chat->color_system);
			}
		}
		else if (args[0] == "/part")
		{
			if (args.size() == 2)
			{
				connection->send_part(args[1]);
			}
			else if (args.size() >= 3)
			{
				CL_String reason;
				for (unsigned int i=2; i<args.size(); i++)
				{
					if (i > 2) reason += " ";
					reason += args[i];
				}

				connection->send_part(args[1], reason);
			}
			else
			{
				chat->add_line("syntax: /part <channel> [reason]", chat->color_system);
			}
		}
		else if (args[0] == "/mode")
		{
			if (args.size() >= 3)
			{
				std::vector<CL_String> remaining_args;
				for (unsigned int i=3; i<args.size(); i++) remaining_args.push_back(args[i]);
				connection->send_mode(args[1], args[2], remaining_args);
			}
			else
			{
				chat->add_line("syntax: /mode <channel or nick> <mode> [args]", chat->color_system);
			}
		}
		else if (args[0] == "/topic")
		{
			if (args.size() == 2)
			{
				connection->send_topic(args[1]);
			}
			else if (args.size() > 2)
			{
				CL_String new_topic;
				for (unsigned int i=2; i<args.size(); i++)
				{
					if (i > 2) new_topic += " ";
					new_topic += args[i];
				}
				connection->send_topic(args[1], new_topic);
			}
			else
			{
				chat->add_line("syntax: /topic <channel> [new topic]", chat->color_system);
			}
		}
		else if (args[0] == "/names")
		{
			if (args.size() == 2)
			{
				connection->send_names(args[1]);
			}
			else
			{
				chat->add_line("syntax: /names <channel>", chat->color_system);
			}
		}
		else if (args[0] == "/list")
		{
			if (args.size() == 1)
			{
				connection->send_list();
			}
			else if (args.size() == 2)
			{
				connection->send_list(args[1]);
			}
			else if (args.size() == 3)
			{
				connection->send_list(args[1], args[2]);
			}
			else
			{
				chat->add_line("syntax: /list [channel] [server]", chat->color_system);
			}
		}
		else if (args[0] == "/invite")
		{
			if (args.size() == 3)
			{
				connection->send_invite(args[1], args[2]);
			}
			else
			{
				chat->add_line("syntax: /invite <nick> <channel>", chat->color_system);
			}
		}
		else if (args[0] == "/kick")
		{
			if (args.size() >= 3)
			{
				CL_String comment;
				for (unsigned int i=2; i<args.size(); i++)
				{
					if (i > 2) comment += " ";
					comment += args[i];
				}
				connection->send_kick(args[1], args[2], comment);
			}
			else
			{
				chat->add_line("syntax: /kick <channel> <user> [comment]", chat->color_system);
			}
		}
		else if (args[0] == "/send_version")
		{
			if (args.size() == 1)
			{
				connection->send_version();
			}
			else if (args.size() == 2)
			{
				connection->send_version(args[1]);
			}
			else
			{
				chat->add_line("syntax: /send_version [server]", chat->color_system);
			}
		}
		else if (args[0] == "/stats")
		{
			if (args.size() == 1)
			{
				connection->send_stats();
			}
			else if (args.size() == 2)
			{
				connection->send_stats(args[1]);
			}
			else if (args.size() == 3)
			{
				connection->send_stats(args[1], args[2]);
			}
			else
			{
				chat->add_line("syntax: /stats [query] [server]", chat->color_system);
			}
		}
		else if (args[0] == "/links")
		{
			if (args.size() == 1)
			{
				connection->send_links();
			}
			else if (args.size() == 2)
			{
				connection->send_links(args[1]);
			}
			else if (args.size() == 3)
			{
				connection->send_links(args[1], args[2]);
			}
			else
			{
				chat->add_line("syntax: /links [remote server] [server mask]", chat->color_system);
			}
		}
		else if (args[0] == "/send_time")
		{
			if (args.size() == 1)
			{
				connection->send_time();
			}
			else if (args.size() == 2)
			{
				connection->send_time(args[1]);
			}
			else
			{
				chat->add_line("syntax: /send_time [server]", chat->color_system);
			}
		}
		else if (args[0] == "/connect")
		{
			if (args.size() == 2)
			{
				connection->send_connect(args[1]);
			}
			else if (args.size() == 3)
			{
				connection->send_connect(args[1], CL_StringHelp::text_to_int(args[2]));
			}
			else if (args.size() == 4)
			{
				connection->send_connect(args[1], CL_StringHelp::text_to_int(args[2]), args[3]);
			}
			else
			{
				chat->add_line("syntax: /connect <target server> [port] [remote server]", chat->color_system);
			}
		}
		else if (args[0] == "/trace")
		{
			if (args.size() == 1)
			{
				connection->send_trace();
			}
			else if (args.size() == 2)
			{
				connection->send_trace(args[1]);
			}
			else
			{
				chat->add_line("syntax: /trace [server]", chat->color_system);
			}
		}
		else if (args[0] == "/admin")
		{
			if (args.size() == 1)
			{
				connection->send_admin();
			}
			else if (args.size() == 2)
			{
				connection->send_admin(args[1]);
			}
			else
			{
				chat->add_line("syntax: /admin [server]", chat->color_system);
			}
		}
		else if (args[0] == "/info")
		{
			if (args.size() == 1)
			{
				connection->send_info();
			}
			else if (args.size() == 2)
			{
				connection->send_info(args[1]);
			}
			else
			{
				chat->add_line("syntax: /info [server]", chat->color_system);
			}
		}
		else if (args[0] == "/privmsg")
		{
			if (args.size() >= 2)
			{
				CL_String text;
				for (unsigned int i=2; i<args.size(); i++)
				{
					if (i > 2) text += " ";
					text += args[i];
				}
				connection->send_privmsg(args[1], text);
			}
			else
			{
				chat->add_line("syntax: /privmsg <nick or channel> <text>", chat->color_system);
			}
		}
		else if (args[0] == "/ctcp" || args[0] == "/privmsg_ctcp")
		{
			if (args.size() >= 3)
			{
				CL_String data;
				for (unsigned int i=3; i<args.size(); i++)
				{
					if (i > 3) data += " ";
					data += args[i];
				}
				connection->send_privmsg_ctcp(args[1], args[2], data);
			}
			else
			{
				chat->add_line("syntax: /ctcp <nick or channel> <command> [data]", chat->color_system);
			}
		}
		else if (args[0] == "/notice")
		{
			if (args.size() >= 2)
			{
				CL_String text;
				for (unsigned int i=2; i<args.size(); i++)
				{
					if (i > 2) text += " ";
					text += args[i];
				}
				connection->send_notice(args[1], text);
			}
			else
			{
				chat->add_line("syntax: /notice <nick or channel> <text>", chat->color_system);
			}
		}
		else if (args[0] == "/notice_ctcp")
		{
			if (args.size() >= 3)
			{
				CL_String data;
				for (unsigned int i=3; i<args.size(); i++)
				{
					if (i > 3) data += " ";
					data += args[i];
				}
				connection->send_notice_ctcp(args[1], args[2], data);
			}
			else
			{
				chat->add_line("syntax: /notice_ctcp <nick or channel> <command> [data]", chat->color_system);
			}
		}
		else if (args[0] == "/who")
		{
			if (args.size() == 2)
			{
				connection->send_who(args[1]);
			}
			else if (args.size() == 3)
			{
				connection->send_who(args[1], args[2] == "true");
			}
			else
			{
				chat->add_line("syntax: /whois <name> [oper = false]", chat->color_system);
			}
		}
		else if (args[0] == "/whois")
		{
			if (args.size() == 2)
			{
				connection->send_whois(args[1]);
			}
			else if (args.size() == 3)
			{
				connection->send_whois(args[1], args[2]);
			}
			else
			{
				chat->add_line("syntax: /whois <nickmask>", chat->color_system);
				chat->add_line("syntax: /whois <server> <nickmask>", chat->color_system);
			}
		}
		else if (args[0] == "/whowas")
		{
			if (args.size() == 2)
			{
				connection->send_whowas(args[1]);
			}
			else if (args.size() == 3)
			{
				connection->send_whowas(args[1], CL_StringHelp::text_to_int(args[2]));
			}
			else if (args.size() == 4)
			{
				connection->send_whowas(args[1], CL_StringHelp::text_to_int(args[2]), args[3]);
			}
			else
			{
				chat->add_line("syntax: /whowas <nick> [count] [server]", chat->color_system);
			}
		}
		else if (args[0] == "/kill")
		{
			if (args.size() >= 3)
			{
				CL_String text;
				for (unsigned int i=2; i<args.size(); i++)
				{
					if (i > 2) text += " ";
					text += args[i];
				}
				connection->send_kill(args[1], text);
			}
			else
			{
				chat->add_line("syntax: /kill <nick> <comment>", chat->color_system);
			}
		}
		else if (args[0] == "/me" || args[0] == "/action")
		{
			CL_String text;
			for (CL_String::size_type i=1; i<args.size(); i++)
			{
				if (i > 1) text += " ";
				text += args[i];
			}

			connection->send_privmsg_ctcp(chat->get_filter(), "ACTION", text);
			chat->add_line(cl_format("*%1 %2", connection->get_nick(), text), chat->color_text);
		}
		else if (args[0] == "/dcc")
		{
		}
		else if (args[0] == "/finger")
		{
			if (args.size() == 2)
			{
				connection->send_privmsg_ctcp(args[1], "FINGER", CL_String());
			}
			else
			{
				chat->add_line("syntax: /finger <channel or user>", chat->color_system);
			}
		}
		else if (args[0] == "/version")
		{
			if (args.size() == 2)
			{
				connection->send_privmsg_ctcp(args[1], "VERSION", CL_String());
			}
			else
			{
				chat->add_line("syntax: /version <channel or user>", chat->color_system);
			}
		}
		else if (args[0] == "/userinfo")
		{
			if (args.size() == 2)
			{
				connection->send_privmsg_ctcp(args[1], "USERINFO", CL_String());
			}
			else
			{
				chat->add_line("syntax: /userinfo <channel or user>", chat->color_system);
			}
		}
		else if (args[0] == "/clientinfo")
		{
			if (args.size() == 2)
			{
				connection->send_privmsg_ctcp(args[1], "CLIENTINFO", CL_String());
			}
			else
			{
				chat->add_line("syntax: /clientinfo <channel or user>", chat->color_system);
			}
		}
		else if (args[0] == "/ping")
		{
			if (args.size() == 2)
			{
				connection->send_privmsg_ctcp(args[1], "PING", CL_StringHelp::int_to_text(CL_System::get_time()));
			}
			else
			{
				chat->add_line("syntax: /ping <channel or user>", chat->color_system);
			}
		}
		else if (args[0] == "/time")
		{
			if (args.size() == 2)
			{
				connection->send_privmsg_ctcp(args[1], "TIME", CL_String());
			}
			else
			{
				chat->add_line("syntax: /time <channel or user>", chat->color_system);
			}
		}
	}

	inputbox->set_text("");
#if defined(netsession_implemented)
	connection->send_type_packet(chat->get_filter(), "");
#endif
}

#if defined(netsession_implemented)
void ChatView::on_inputbox_changed(const CL_String &text)
{
	connection->send_type_packet(chat->get_filter(), text);
}
#endif

void ChatView::on_userlist_activated(int item_id)
{
	// todo: spawn private conversation with user
}

void ChatView::on_userlist_key_up(const CL_InputEvent &event)
{
	if (event.id == CL_KEY_APPS)
	{
		on_userlist_contextmenu();
	}
}

void ChatView::on_userlist_mouse_up(const CL_InputEvent &event)
{
	if (event.id == CL_MOUSE_RIGHT)
	{
		on_userlist_contextmenu();
	}
}

void ChatView::on_userlist_contextmenu()
{
	// todo: show popup menu
}

void ChatView::on_connection_numeric_reply(
	const CL_String &prefix,
	int command,
	const std::vector<CL_String> &params)
{
	if (command == RPL_NAMREPLY && params.size() >= 4)
	{
		CL_String channel = params[2];
		CL_String nicks = params[3];

		if (channel == chat->get_filter())
		{
			std::vector<CL_TempString> nick_list = CL_StringHelp::split_text(nicks, " ");
			for (unsigned int i=0; i<nick_list.size(); i++)
			{
				CL_String nick = nick_list[i];
				CL_ListViewItem item = userlist->create_item();
				item.set_column_text("nick", nick);
				userlist->get_document_item().append_child(item);
			}

			// userlist->sort();
		}
	}
	else if (command == RPL_ENDOFNAMES)
	{
		// hmm do we need to do anything here?
	}
}

void ChatView::on_connection_nick(const CL_String &_old_nick, const CL_String &_new_nick)
{
	CL_String old_nick = CL_IRCConnection::extract_nick(_old_nick);
	CL_String new_nick = CL_IRCConnection::extract_nick(_new_nick);

	CL_ListViewItem cur = userlist->get_document_item().get_first_child();
	while (!cur.is_null())
	{
		if (cur.get_column("nick").get_text() == old_nick)
		{
			cur.set_column_text("nick", new_nick);
			// userlist->sort();
			break;
		}

		cur = cur.get_next_sibling();
	}
}

void ChatView::on_connection_join(const CL_String &nick, const CL_String &channel)
{
	if (channel == chat->get_filter())
	{
		// If its us ourselves joining a channel, clear the userlist
		// (we will be expecting a set of RPL commands about the users)
		if (CL_IRCConnection::extract_nick(nick) == connection->get_nick())
		{
			userlist->clear();
		}
		else
		{
			// Check if we are already in userlist:
			CL_String old_nick = CL_IRCConnection::extract_nick(nick);
			CL_ListViewItem cur = userlist->get_document_item().get_first_child();
			while (!cur.is_null())
			{
				if (cur.get_column("nick").get_text() == old_nick)
					break;
				cur = cur.get_next_sibling();
			}

			CL_ListViewItem item = userlist->create_item();
			item.set_column_text("nick", CL_IRCConnection::extract_nick(nick));
			userlist->get_document_item().append_child(item);
			// userlist->sort();
		}
	}
}

void ChatView::on_connection_part(const CL_String &nick, const CL_String &channel, const CL_String &reason)
{
	if (channel == chat->get_filter())
	{
		CL_String old_nick = CL_IRCConnection::extract_nick(nick);
		CL_ListViewItem cur = userlist->get_document_item().get_first_child();
		while (!cur.is_null())
		{
			if (cur.get_column("nick").get_text() == old_nick)
			{
				cur.remove();
				break;
			}
			cur = cur.get_next_sibling();
		}
	}
}

void ChatView::on_connection_unknown_command_received(const CL_String &prefix, const CL_String &command, const std::vector<CL_String> &params)
{
	if (command == "QUIT")
	{
		CL_String nick = prefix;
		CL_String reason;
		if (params.size() > 0)
			reason = params[0];

		CL_String old_nick = CL_IRCConnection::extract_nick(nick);
		CL_ListViewItem cur = userlist->get_document_item().get_first_child();
		while (!cur.is_null())
		{
			if (cur.get_column("nick").get_text() == old_nick)
			{
				cur.remove();
				if (!reason.empty())
					chat->add_line(cl_format("%1 quit (%2)", CL_IRCConnection::extract_nick(nick), reason), chat->color_system);
				else
					chat->add_line(cl_format("%1 quit", CL_IRCConnection::extract_nick(nick)), chat->color_system);
				break;
			}
			cur = cur.get_next_sibling();
		}
	}
}
