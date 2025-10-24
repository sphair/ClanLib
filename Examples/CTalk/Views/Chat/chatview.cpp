
#include "precomp.h"
#include "chatview.h"
#include "chat.h"
#include "chaturl.h"
#include "Document/irc_session.h"
#include "Document/irc_text.h"
#include "Document/command.h"
#include "Framework/mainframe.h"

ChatView::ChatView(IRCSession *session, const IRCEntity &filter, CL_GUIComponent *parent, MainFrame *mainframe)
: View(parent, mainframe, get_view_caption(filter)), channel_topic(0), chat(0), inputbox(0), userlist(0), 
  chat_input_history_index(0), next_chat_url_id(1),
  session(session), filter(filter), next_icon_index(1), regexp_url1("(http://.*?)([ \\r\\n\\t\"'>)]|$)")
{
	set_type_name("chatview");
	//set_class_name("black");

	part_background = CL_GUIThemePart(this);

	channel_topic = new ChannelTopic(this);
	chat = new Chat(this);
	inputbox = new CL_LineEdit(this);
	inputbox->set_class_name("chatinput");
	userlist = new CL_ListView(this);
	userlist->set_class_name("chatusers");
	inputbox->set_focus();

	CL_GraphicContext gc = get_gc();
	icon_action = CL_Image(gc, "Resources/chat_icon_action.png");
	icon_notice = CL_Image(gc, "Resources/chat_icon_notice.png");
	icon_error = CL_Image(gc, "Resources/chat_icon_error.png");

	icon_normal_index = add_listview_icon("Resources/user.png");
	icon_operator_index = add_listview_icon("Resources/user_operator.png");
	icon_voiced_index = add_listview_icon("Resources/user_voice.png");

	userlist->get_header()->append(userlist->get_header()->create_column("nick", "Name"));

	// Only show userlist for channels:
	if (!get_filter().is_channel())
	{
		channel_topic->set_visible(false);
		userlist->set_visible(false);
	}

	func_resized().set(this, &ChatView::on_resize);
	inputbox->func_unhandled_event().set(this, &ChatView::on_inputbox_unhandled_input);

	userlist->func_key_pressed().set(this, &ChatView::on_userlist_key_pressed);
	userlist->func_mouse_right_up().set(this, &ChatView::on_userlist_contextmenu);

	slots.connect(session->cb_channel_topic_updated, this, &ChatView::on_channel_topic_updated);
	slots.connect(session->cb_channel_names_updated, this, &ChatView::on_channel_names_updated);
	slots.connect(session->cb_nick_changed, this, &ChatView::on_nick_changed);
	slots.connect(session->cb_parted, this, &ChatView::on_parted);
	slots.connect(session->cb_user_joined, this, &ChatView::on_user_joined);
	slots.connect(session->cb_user_parted, this, &ChatView::on_user_parted);
	slots.connect(session->cb_user_quit, this, &ChatView::on_user_quit);

	slots.connect(session->cb_channel_text, this, &ChatView::on_channel_text);
	slots.connect(session->cb_channel_notice, this, &ChatView::on_channel_notice);
	slots.connect(session->cb_channel_action, this, &ChatView::on_channel_action);
	slots.connect(session->cb_private_text, this, &ChatView::on_private_text);
	slots.connect(session->cb_private_notice, this, &ChatView::on_private_notice);
	slots.connect(session->cb_private_action, this, &ChatView::on_private_action);
	slots.connect(session->cb_system_text, this, &ChatView::on_system_text);
	slots.connect(session->cb_error_text, this, &ChatView::on_error_text);
	slots.connect(session->cb_channel_mode_change, this, &ChatView::on_channel_mode_change);
	slots.connect(session->cb_nick_mode_change, this, &ChatView::on_nick_mode_change);

	slots.connect(chat->cb_url_clicked, this, &ChatView::on_url_clicked);

	on_resize();
}

CL_String ChatView::get_view_caption(const IRCEntity &filter)
{
	if (filter.empty())
		return "Status";
	else if (filter.is_channel())
		return "#"+CL_StringHelp::text_to_lower(filter.get_name());
	else
		return filter.get_name();
}

int ChatView::add_listview_icon(const CL_String &filename)
{
	CL_GraphicContext gc = get_gc();
	CL_Sprite sprite(gc, filename);
	CL_ListViewIcon icon;
	icon.set_sprite(sprite, listview_mode_details);
	CL_ListViewIconList icon_list = userlist->get_icon_list();
	icon_list.set_icon(next_icon_index, icon);
	return next_icon_index++;
}

void ChatView::add_private_text(const IRCNick &nick, const IRCText &text)
{
	if (get_filter() == nick)
	{
		add_line(nick, text, chat->get_color_text(), chat->get_color_nick_others());
		get_mainframe()->flag_activity(this);
	}
}

ChatView::~ChatView()
{
	if (filter.is_channel())
	{
		session->part(filter);
	}
	else if (filter.empty())
	{
		// What should we do if the Status window is closed?
		session->disconnect_graceful();
	}
}

void ChatView::on_resize()
{
	CL_Rect contentbox = part_background.get_content_box(get_size());

	int topic_height = channel_topic->get_topic_height(contentbox.get_width());
	int inputbox_height = 20;
	int userlist_width = 150;

	if (!get_filter().is_channel())
	{
		topic_height = 0;
		userlist_width = 0;
	}

	channel_topic->set_geometry(CL_Rect(contentbox.left, contentbox.top, contentbox.right, contentbox.top+topic_height));
	chat->set_geometry(CL_Rect(contentbox.left, contentbox.top+topic_height, contentbox.right-userlist_width, contentbox.bottom-inputbox_height));
	userlist->set_geometry(CL_Rect(contentbox.right-userlist_width, contentbox.top+topic_height, contentbox.right, contentbox.bottom-inputbox_height));
	inputbox->set_geometry(CL_Rect(contentbox.left, contentbox.bottom-inputbox_height, contentbox.right, contentbox.bottom));
}

bool ChatView::on_inputbox_unhandled_input(CL_InputEvent input_event)
{
	if (input_event.id == CL_KEY_ENTER && input_event.type == CL_InputEvent::released) // should be CL_InputEvent::pressed, but lineedit is bugged?
	{
		on_inputbox_return_pressed();
		return true;
	}
	else if (input_event.id == CL_KEY_TAB && input_event.type == CL_InputEvent::released) // should be CL_InputEvent::pressed, but lineedit is bugged?
	{
		on_inputbox_tab_pressed();
		return true;
	}
	else if (input_event.id == CL_KEY_UP && input_event.type == CL_InputEvent::pressed)
	{
		on_inputbox_up_pressed();
		return true;
	}
	else if (input_event.id == CL_KEY_DOWN && input_event.type == CL_InputEvent::pressed)
	{
		on_inputbox_down_pressed();
		return true;
	}
	else if (input_event.id == CL_KEY_PRIOR && input_event.type == CL_InputEvent::pressed)
	{
		chat->scroll_page_up();
		return true;
	}
	else if (input_event.id == CL_KEY_NEXT && input_event.type == CL_InputEvent::pressed)
	{
		chat->scroll_page_down();
		return true;
	}
	else
	{
		return false;
	}
}

void ChatView::on_inputbox_up_pressed()
{
	if(chat_input_history_index > 0)
	{
		chat_input_history_index--;
		inputbox->set_text(chat_input_history[chat_input_history_index]);
	}
}

void ChatView::on_inputbox_down_pressed()
{
	if( chat_input_history.size() > 0 && chat_input_history_index < chat_input_history.size() - 1)
	{
		chat_input_history_index++;
		inputbox->set_text(chat_input_history[chat_input_history_index]);
	}
	else
	{
		inputbox->set_text(CL_String());
	}
}

void ChatView::on_inputbox_tab_pressed()
{
	if (get_filter().is_channel())
	{
		int cursor_pos = inputbox->get_cursor_pos();
		if(cursor_pos > 0)
		{
			CL_String line = inputbox->get_text();
			CL_String::size_type pos = line.rfind(' ', cursor_pos);

			if(pos == CL_String::npos)
				pos = 0;
			else
				pos = pos + 1;
		
			CL_StringRef search_nick = line.substr(pos, cursor_pos - pos);
			int search_length = search_nick.length();

			IRCJoinedChannel status = session->get_channel_status(get_filter());
			for (size_t i = 0; i < status.users.size(); i++)
			{
				CL_String user_name = status.users[i].get_name();
				if(CL_StringHelp::compare(user_name.substr(0, search_length), search_nick, true) == 0)
				{
					CL_StringRef prefix = line.substr(0, pos);
					CL_StringRef postfix = line.substr(cursor_pos);

					inputbox->set_text(prefix + user_name + postfix);
					return;
				}
			}
		}
	}
}

void ChatView::on_inputbox_return_pressed()
{
	CL_String chat_line = inputbox->get_text();
	if(!chat_line.empty())
	{
		chat_input_history.push_back(chat_line);
		chat_input_history_index = chat_input_history.size();

		std::vector<CL_String> lines = split_input_text(chat_line);
		for (size_t i = 0; i < lines.size(); i++)
		{
			CL_String &command_line = lines[i];
			if (!command_line.empty())
			{
				if (command_line[0] != '/')
				{
					session->send_text(get_filter(), IRCText::from_text(command_line));
					add_line(session->get_nick(), IRCText::from_text(command_line), chat->get_color_text(), chat->get_color_nick());
					get_mainframe()->flag_activity(this);
				}
				else
				{
					try
					{
						session->execute_command(get_filter(), command_line);
					}
					catch (CL_Exception &e)
					{
						add_error_line(IRCText::from_text(e.message));
						get_mainframe()->flag_activity(this);
					}
				}
			}
		}

		inputbox->set_text("");
	}
}

std::vector<CL_String> ChatView::split_input_text(const CL_String &text)
{
	std::vector<CL_String> lines;
	CL_String::size_type pos = 0;
	while (pos < text.length())
	{
		CL_String::size_type start = pos;
		CL_String::size_type end = text.find('\n', start);
		if (end == CL_String::npos)
			break;
		pos = end+1;

		if (end > 0 && text[end-1] == '\r')
			end--;

		if (end > start)
			lines.push_back(text.substr(start, end-start));
	}
	if (pos < text.length())
		lines.push_back(text.substr(pos));
	return lines;
}

void ChatView::on_url_clicked(int object_id)
{
	for(size_t i = 0; i < chat_urls.size(); ++i)
	{
		if(chat_urls[i].object_id == object_id)
		{
			#ifdef WIN32
			ShellExecute(NULL, cl_text("open"), chat_urls[i].url.c_str(), NULL, NULL, SW_SHOWNORMAL);
			#endif
		}
	}
}

void ChatView::on_userlist_key_pressed(CL_InputEvent event)
{
	if (event.id == CL_KEY_APPS)
	{
		on_userlist_contextmenu(CL_Point(-1, -1));
	}
}

void ChatView::on_userlist_contextmenu(CL_Point pos)
{
	CL_PopupMenu menu;
	menu.insert_item(L"Slap!", cmd_slap);
	menu.insert_item(L"Open Conversation", cmd_open_conversation);
	menu.func_item_selected().set(this, &ChatView::on_userlist_menu_command);
	menu.exec(userlist, pos);
}

void ChatView::on_userlist_menu_command(CL_PopupMenuItem item)
{
	CL_ListViewItem selected_item = userlist->get_selected_item();
	if (!selected_item.is_null())
	{
		switch (item.get_id())
		{
		case cmd_slap:
			Command::execute(session, filter, cl_format("/me slaps %1 around a bit with a large carambola fruit!", selected_item.get_column("nick").get_text()));
			break;
		case cmd_open_conversation:
			get_mainframe()->open_conversation(IRCNick::from_text(selected_item.get_column("nick").get_text()), session);
			break;
		default:
			break;
		}
	}
}

void ChatView::on_channel_text(const IRCChannel &channel, const IRCNick &nick, const IRCText &text)
{
	if (get_filter() == channel)
	{
		add_line(nick, text, chat->get_color_text(), chat->get_color_nick_others());
		get_mainframe()->flag_activity(this);
	}
}

void ChatView::on_channel_notice(const IRCChannel &channel, const IRCNick &nick, const IRCText &text)
{
	if (get_filter() == channel)
	{
		add_notice_line(nick, text);
		get_mainframe()->flag_activity(this);
	}
}

void ChatView::on_channel_action(const IRCChannel &channel, const IRCNick &nick, const IRCText &text)
{
	if (get_filter() == channel)
	{
		add_action_line(nick, text);
		get_mainframe()->flag_activity(this);
	}
}

void ChatView::on_private_text(const IRCNick &nick, const IRCText &text)
{
	add_private_text(nick, text);
	get_mainframe()->flag_activity(this);
}

void ChatView::on_private_notice(const IRCNick &nick, const IRCText &text)
{
	if (is_active_view())
	{
		add_notice_line(nick, text);
		get_mainframe()->flag_activity(this);
	}
}

void ChatView::on_private_action(const IRCNick &nick, const IRCText &text)
{
	if (get_filter() == nick)
	{
		add_action_line(nick, text);
		get_mainframe()->flag_activity(this);
	}
}

void ChatView::on_error_text(const IRCText &text)
{
	if (is_active_view())
	{
		add_error_line(text);
		get_mainframe()->flag_activity(this);
	}
}

void ChatView::on_system_text(const IRCText &text)
{
	if (get_filter().empty())
	{
		add_line(text, chat->get_color_system());
	}
}

void ChatView::on_channel_topic_updated(const IRCChannel &channel)
{
	if (get_filter() == channel)
	{
		IRCJoinedChannel status = session->get_channel_status(channel);
		channel_topic->set_topic("Unknown time"/*status.topic_time*/, status.topic, status.topic_author);
		on_resize();
	}
}

void ChatView::on_channel_names_updated(const IRCChannel &channel)
{
	if (get_filter() == channel)
	{
		IRCJoinedChannel status = session->get_channel_status(channel);

		userlist->clear();
		for (size_t i = 0; i < status.users.size(); i++)
		{
			CL_ListViewItem item = userlist->create_item();
			item.set_column_text("nick", status.users[i].get_name());

			if (status.users[i].is_operator())
				item.set_icon(icon_operator_index);
			else if (status.users[i].is_voiced())
				item.set_icon(icon_voiced_index);
			else
				item.set_icon(icon_normal_index);

			userlist->get_document_item().append_child(item);
		}
		// userlist->sort();
	}
}

void ChatView::on_nick_changed(const IRCNick &old_nick, const IRCNick &new_nick)
{
	CL_ListViewItem cur = find_user_item(old_nick);
	if (!cur.is_null())
	{
		cur.set_column_text("nick", new_nick.get_name());
		// userlist->sort();

		add_line(IRCText::from_text(cl_format(L"%1 is now known as %2", old_nick.get_name(), new_nick.get_name())), chat->get_color_channel());
	}
}

void ChatView::on_parted(const IRCChannel &channel)
{
	if (get_filter() == channel)
	{
		add_line(IRCText::from_text(L"You left this channel"), chat->get_color_channel());
	}
}

void ChatView::on_user_joined(const IRCChannel &channel, const IRCNick &nick)
{
	if (get_filter() == channel)
	{
		CL_ListViewItem item = userlist->create_item();
		item.set_column_text("nick", nick.get_name());

		if (nick.is_operator())
			item.set_icon(icon_operator_index);
		else if (nick.is_voiced())
			item.set_icon(icon_voiced_index);
		else
			item.set_icon(icon_normal_index);

		userlist->get_document_item().append_child(item);
		// userlist->sort();

		add_line(IRCText::from_text(cl_format("%1 joined the channel", nick.get_name())), chat->get_color_channel());
	}
}

void ChatView::on_user_parted(const IRCChannel &channel, const IRCNick &nick, const IRCText &text)
{
	if (get_filter() == channel)
	{
		CL_ListViewItem cur = find_user_item(nick);
		if (!cur.is_null())
			cur.remove();

		if (text.empty())
			add_line(IRCText::from_text(cl_format("%1 left the channel", nick.get_name())), chat->get_color_channel());
		else
			add_line(IRCText::from_text(cl_format("%1 left the channel (%2)", nick.get_name(), text.get_text())), chat->get_color_channel());
	}
}

void ChatView::on_user_quit(const IRCNick &nick, const IRCText &text)
{
	CL_ListViewItem cur = find_user_item(nick);
	if (!cur.is_null())
	{
		cur.remove();

		if (text.empty())
			add_line(IRCText::from_text(cl_format("%1 quit", nick.get_name())), chat->get_color_channel());
		else
			add_line(IRCText::from_text(cl_format("%1 quit (%2)", nick.get_name(), text.get_text())), chat->get_color_channel());
	}
}

CL_ListViewItem ChatView::find_user_item(const IRCNick &nick)
{
	CL_ListViewItem cur = userlist->get_document_item().get_first_child();
	while (!cur.is_null())
	{
		if (CL_StringHelp::compare(cur.get_column("nick").get_text(), nick.get_name(), true) == 0)
			return cur;

		cur = cur.get_next_sibling();
	}
	return CL_ListViewItem();
}

void ChatView::on_channel_mode_change(const IRCNick &executing_nick, const IRCChannel &channel, const std::vector<IRCRawString> &command)
{
	if (get_filter() == channel)
	{
		CL_String text;
		for (size_t i = 0; i < command.size(); i++)
		{
			if (!text.empty())
				text += " ";
			text += IRCText::from_raw(command[i]).get_text();
		}
		add_line(IRCText::from_text(cl_format(L"%1 sets mode %2", executing_nick.get_name(), text)), chat->get_color_channel());

		if (command.size() > 1)
		{
			IRCNick nick = IRCNick::from_raw(command[1]);
			CL_ListViewItem item = find_user_item(nick);
			if (!item.is_null())
			{
				if (command[0] == "+o")
					item.set_icon(icon_operator_index);
				else if (command[0] == "+v")
					item.set_icon(icon_voiced_index);
				else if (command[0] == "-o" || command[0] == "-v")
					item.set_icon(icon_normal_index);
			}
		}
	}
}

void ChatView::on_nick_mode_change(const IRCNick &executing_nick, const IRCNick &target_nick, const std::vector<IRCRawString> &command)
{
	if (is_active_view())
	{
		CL_String text;
		for (size_t i = 0; i < command.size(); i++)
		{
			if (!text.empty())
				text += " ";
			text += IRCText::from_raw(command[i]).get_text();
		}
		add_line(IRCText::from_text(cl_format(L"%1 sets mode %2 on %3", executing_nick.get_name(), text, target_nick.get_name())), chat->get_color_system());
	}
}

void ChatView::add_action_line(const IRCNick &nick, const IRCText &text)
{
	CL_SpanLayout layout;
	layout.add_image(icon_action);
	add_layout_text(layout, L" " + nick.get_name() + L" " + text.get_text(), chat->get_color_channel());
	chat->add_line(layout);
}

void ChatView::add_notice_line(const IRCNick &nick, const IRCText &text)
{
	CL_SpanLayout layout;
	layout.add_image(icon_notice);
	add_layout_text(layout, L" " + text.get_text(), chat->get_color_channel());
	chat->add_line(nick.get_name(), chat->get_color_nick_others(), layout);
}

void ChatView::add_error_line(const IRCText &text)
{
	CL_SpanLayout layout;
	layout.add_image(icon_error);
	add_layout_text(layout, L" " + text.get_text(), chat->get_color_system());
	chat->add_line(layout);
}

void ChatView::add_line(const IRCText &text, const CL_Colorf &text_color)
{
	CL_SpanLayout layout;
	add_layout_text(layout, text.get_text(), text_color);
	chat->add_line(layout);
}

void ChatView::add_line(const IRCNick &sender, const IRCText &text, const CL_Colorf &text_color, const CL_Colorf &nick_color)
{
	CL_SpanLayout layout;
	add_layout_text(layout, text.get_text(), text_color);
	chat->add_line(sender.get_name(), nick_color, layout);
}

void ChatView::add_layout_text(CL_SpanLayout &layout, const CL_String &text, const CL_Colorf &color)
{
	CL_TempString8 text8 = CL_StringHelp::text_to_utf8(text);
	int start_offset = 0;
	while (true)
	{
		CL_RegExpMatch result = regexp_url1.search(text8, start_offset);
		if (result.is_match())
		{
			int cur_offset = result.get_capture_pos(1);
			if (cur_offset != start_offset)
				layout.add_text(CL_StringHelp::utf8_to_text(text8.substr(start_offset, cur_offset-start_offset)), chat->font, color);

			ChatUrl url;
			url.object_id = next_chat_url_id++;
			url.url = CL_StringHelp::utf8_to_text(result.get_capture(text8, 1));
			chat_urls.push_back(url);

			layout.add_text(url.url, chat->font_url, chat->get_color_url(), url.object_id);

			start_offset = cur_offset + result.get_capture_length(1);
		}
		else
		{
			layout.add_text(CL_StringHelp::utf8_to_text(text8.substr(start_offset)), chat->font, color);
			break;
		}
	}
}

bool ChatView::is_active_view()
{
	View *view = get_mainframe()->get_active_view();
	if (get_mainframe()->get_active_view() == this)
	{
		return true;
	}
	else // Redirect to Status view (empty filter) if user is viewing another network
	{
		ChatView *chatview = dynamic_cast<ChatView*>(view);
		if (chatview && chatview->get_session() != get_session() && filter.empty())
			return true;
		else
			return false;
	}
}
