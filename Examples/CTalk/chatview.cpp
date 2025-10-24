
#include "precomp.h"
#include "chatview.h"
#include "chat.h"
#include "ircconnection.h"

/////////////////////////////////////////////////////////////////////////////
// ChatView construction:

ChatView::ChatView(IRCConnection *connection, const std::string &filter, MainFrame *mainframe) :
	View(mainframe, filter.empty() ? "Status" : filter), chat(0), inputbox(0), userlist(0), connection(connection)
{
	chat = new Chat(connection, filter, this);
	inputbox = new CL_InputBox(this);
	userlist = new CL_ListBox(this);
	inputbox->set_focus();

	// Only show userlist for channels:
	if (filter.substr(0, 1) != "#") userlist->show(false);

	slots.connect(sig_resize(), this, &ChatView::on_resize);
	slots.connect(sig_paint(), this, &ChatView::on_paint);
	slots.connect(inputbox->sig_return_pressed(), this, &ChatView::on_inputbox_return_pressed);
	slots.connect(inputbox->sig_changed(), this, &ChatView::on_inputbox_changed);
	slots.connect(userlist->sig_activated(), this, &ChatView::on_userlist_activated);
	slots.connect(userlist->sig_key_up(), this, &ChatView::on_userlist_key_up);
	slots.connect(userlist->sig_mouse_up(), this, &ChatView::on_userlist_mouse_up);
	slots.connect(connection->sig_numeric_reply(), this, &ChatView::on_connection_numeric_reply);
	slots.connect(connection->sig_nick(), this, &ChatView::on_connection_nick);
	slots.connect(connection->sig_join(), this, &ChatView::on_connection_join);
	slots.connect(connection->sig_part(), this, &ChatView::on_connection_part);
}

ChatView::~ChatView()
{
}

/////////////////////////////////////////////////////////////////////////////
// ChatView attributes:


/////////////////////////////////////////////////////////////////////////////
// ChatView operations:


/////////////////////////////////////////////////////////////////////////////
// ChatView implementation:

void ChatView::on_resize(int old_width, int old_height)
{
	int inputbox_height = 16;
	int userlist_width = 150;

	if (userlist->is_visible() == false) userlist_width = 0;

	chat->set_position(CL_Rect(0, 0, get_width()-userlist_width, get_height()-inputbox_height));
	userlist->set_position(CL_Rect(get_width()-userlist_width, 0, get_width(), get_height()-inputbox_height));
	inputbox->set_position(CL_Rect(0, get_height()-inputbox_height, get_width(), get_height()));
}

void ChatView::on_paint()
{
}

void ChatView::on_inputbox_return_pressed()
{
	std::string command_line = inputbox->get_text();
	if (command_line.empty()) return;

	if (command_line[0] != '/') // not a command
	{
		connection->send_privmsg(chat->get_filter(), command_line);
		chat->add_line(connection->get_nick(), command_line, chat->color_text);
	}
	else // its a command
	{
		std::vector<std::string> args = CL_String::tokenize(command_line, " ");

		if (args[0] == "/nick")
		{
			if (args.size() > 1)
			{
				connection->send_nick(args[1]); 
			}
		}
		else if (args[0] == "/oper")
		{
			// void send_oper(const std::string &user, const std::string &password);
		}
		else if (args[0] == "/quit")
		{
			if (args.size() == 1)
			{
				connection->send_quit("ClanLib - the biggest overkill library ever written for an IRC client!");
			}
			else if (args.size() >= 2)
			{
				std::string reason;
				for (unsigned int i=1; i<args.size(); i++)
				{
					if (i > 1) reason += " ";
					reason += args[i];
				}

				connection->send_quit(reason);
			}

			quit();
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
				std::string reason;
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
				std::vector<std::string> remaining_args;
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
				std::string new_topic;
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
				std::string comment;
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
				connection->send_connect(args[1], CL_String::to_int(args[2]));
			}
			else if (args.size() == 4)
			{
				connection->send_connect(args[1], CL_String::to_int(args[2]), args[3]);
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
				connection->send_admin("");
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
				std::string text;
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
				std::string data;
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
				std::string text;
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
				std::string data;
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
				connection->send_whowas(args[1], CL_String::to_int(args[2]));
			}
			else if (args.size() == 4)
			{
				connection->send_whowas(args[1], CL_String::to_int(args[2]), args[3]);
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
				std::string text;
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
			std::string text;
			for (std::string::size_type i=1; i<args.size(); i++)
			{
				if (i > 1) text += " ";
				text += args[i];
			}

			connection->send_privmsg_ctcp(chat->get_filter(), "ACTION", text);
			chat->add_line(std::string("*CTalk ")+text, chat->color_text);
		}
		else if (args[0] == "/dcc")
		{
		}
		else if (args[0] == "/finger")
		{
			if (args.size() == 2)
			{
				connection->send_privmsg_ctcp(args[1], "FINGER", std::string());
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
				connection->send_privmsg_ctcp(args[1], "VERSION", std::string());
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
				connection->send_privmsg_ctcp(args[1], "USERINFO", std::string());
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
				connection->send_privmsg_ctcp(args[1], "CLIENTINFO", std::string());
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
				connection->send_privmsg_ctcp(args[1], "PING", CL_String::from_int(CL_System::get_time()));
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
				connection->send_privmsg_ctcp(args[1], "TIME", std::string());
			}
			else
			{
				chat->add_line("syntax: /time <channel or user>", chat->color_system);
			}
		}
	}

	inputbox->set_text("");
	connection->send_type_packet(chat->get_filter(), "");
}

void ChatView::on_inputbox_changed(const std::string &text)
{
	//why was this sending a packet on every keystroke?  No need.  --mrfun Sep 22 2006
	//connection->send_type_packet(chat->get_filter(), text);
}

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
	const std::string &prefix,
	int command,
	const std::vector<std::string> &params)
{
	if (command == RPL_NAMREPLY && params.size() >= 4)
	{
		std::string channel = params[2];
		std::string nicks = params[3];

		if (channel != chat->get_filter()) return;

		std::vector<std::string> nick_list = CL_String::tokenize(nicks, " ");
		for (unsigned int i=0; i<nick_list.size(); i++)
		{
			std::string nick = nick_list[i];
			userlist->insert_item(nick);
		}

		userlist->sort();
	}
	else if (command == RPL_ENDOFNAMES)
	{
		// hmm do we need to do anything here?
	}
}

void ChatView::on_connection_nick(const std::string &_old_nick, const std::string &_new_nick)
{
	std::string old_nick = CL_IRCConnection::extract_nick(_old_nick);
	std::string new_nick = CL_IRCConnection::extract_nick(_new_nick);

	std::vector<CL_ListItem *> &items = userlist->get_items();
	for (unsigned int i=0; i<items.size(); i++)
	{
		if (items[i]->str == old_nick)
		{
			userlist->remove_item(i);
			userlist->insert_item(new_nick);
			userlist->sort();
			return;
		}
	}
}

void ChatView::on_connection_join(const std::string &nick, const std::string &channel)
{
	if (channel == chat->get_filter())
	{
		// If its us ourselves joining a channel, clear the userlist
		// (we will be expecting a set of RPL commands about the users)
		if (CL_IRCConnection::extract_nick(nick) == connection->get_nick())
		{
			userlist->clear();
			return;
		}

		// Check if we are already in userlist:
		std::vector<CL_ListItem *> &items = userlist->get_items();
		for (unsigned int i=0; i<items.size(); i++)
		{
			std::string old_nick = CL_IRCConnection::extract_nick(nick);

			if (items[i]->str == old_nick)
			{
				return;
			}
		}

		userlist->insert_item(CL_IRCConnection::extract_nick(nick));
		userlist->sort();
	}
}

void ChatView::on_connection_part(const std::string &nick, const std::string &channel, const std::string &reason)
{
	if (channel == chat->get_filter())
	{
		std::vector<CL_ListItem *> &items = userlist->get_items();
		for (unsigned int i=0; i<items.size(); i++)
		{
			std::string old_nick = CL_IRCConnection::extract_nick(nick);

			if (items[i]->str == old_nick)
			{
				userlist->remove_item(i);
				return;
			}
		}
	}
}
