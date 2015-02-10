
#include "precomp.h"
#include "chat_view_controller.h"
#include "Views/Chat/chat_view.h"
#include "Views/Chat/chat_line.h"
#include "Views/UserList/user_list_view.h"
#include "chat_url.h"
#include "Models/IRCSession/irc_session.h"
#include "Models/IRCSession/irc_text.h"
#include "Models/IRCSession/command.h"
#include "Models/app_model.h"

using namespace clan;

ChatViewController::ChatViewController(IRCSession *session, const IRCEntity &filter) : session(session), filter(filter)
{
	create_layout();

	slots.connect(AppModel::instance()->cb_irc_session_destroyed, this, &ChatViewController::irc_session_destroyed);
	if (session)
		slots.connect(session->cb_parted, this, &ChatViewController::irc_channel_parted);

	// Only show userlist for channels:
	if (!get_filter().is_channel())
	{
		//channel_topic->set_hidden();
		user_list->set_hidden();
	}

	slots.connect(session->cb_channel_topic_updated, this, &ChatViewController::on_channel_topic_updated);
	slots.connect(session->cb_channel_names_updated, this, &ChatViewController::on_channel_names_updated);
	slots.connect(session->cb_nick_changed, this, &ChatViewController::on_nick_changed);
	slots.connect(session->cb_parted, this, &ChatViewController::on_parted);
	slots.connect(session->cb_user_joined, this, &ChatViewController::on_user_joined);
	slots.connect(session->cb_user_parted, this, &ChatViewController::on_user_parted);
	slots.connect(session->cb_user_kicked, this, &ChatViewController::on_user_kicked);
	slots.connect(session->cb_user_quit, this, &ChatViewController::on_user_quit);

	slots.connect(session->cb_text, this, &ChatViewController::on_text);
	slots.connect(session->cb_notice, this, &ChatViewController::on_notice);
	slots.connect(session->cb_action, this, &ChatViewController::on_action);
	slots.connect(session->cb_system_text, this, &ChatViewController::on_system_text);
	slots.connect(session->cb_error_text, this, &ChatViewController::on_error_text);
	slots.connect(session->cb_channel_mode_change, this, &ChatViewController::on_channel_mode_change);
	slots.connect(session->cb_nick_mode_change, this, &ChatViewController::on_nick_mode_change);

	slots.connect(chat_log->cb_url_clicked, this, &ChatViewController::on_url_clicked);
	slots.connect(input_text->sig_enter_pressed(), this, &ChatViewController::on_inputbox_return_pressed);

	/*
	set_type_name("chatview");
	//set_class_name("black");

	part_background = CL_GUIThemePart(this);

	channel_topic = new ChannelTopic(this);
	chat = new Chat(this);
	inputbox = new TextFieldView(this);
	inputbox->set_class_name("chatinput");
	userlist = new CL_ListView(this);
	userlist->set_class_name("chatusers");
	inputbox->set_select_all_on_focus_gain(false);
	inputbox->set_focus();

	CL_GraphicContext gc = get_gc();
	icon_action = CL_Image(gc, "Resources/Icons/chat_icon_action.png");
	icon_notice = CL_Image(gc, "Resources/Icons/chat_icon_notice.png");
	icon_error = CL_Image(gc, "Resources/Icons/chat_icon_error.png");
	icon_topic = CL_Image(gc, "Resources/Icons/chat_icon_topic.png");

	icon_normal_index = add_listview_icon("Resources/Icons/user.png");
	icon_operator_index = add_listview_icon("Resources/Icons/user_operator.png");
	icon_voiced_index = add_listview_icon("Resources/Icons/user_voice.png");

	userlist->get_header()->append(userlist->get_header()->create_column("nick", "Name"));

	get_toolbar()->insert_item(CL_Sprite(get_gc(), "Resources/Icons/icon-delete.png"), 0, "Disconnect", id_disconnect);
	if (get_filter().is_channel())
	{
		get_toolbar()->insert_item(CL_Sprite(get_gc(), "Resources/Icons/icon-picture.png"), 0, "Set topic", id_set_topic);
		get_toolbar()->insert_item(CL_Sprite(get_gc(), "Resources/Icons/icon-delete.png"), 0, "Leave channel", id_leave);
		get_toolbar()->insert_item(CL_Sprite(get_gc(), "Resources/Icons/icon-picture.png"), 0, "Moderate", id_moderate);
	}
	get_toolbar()->insert_item(CL_Sprite(get_gc(), "Resources/Icons/icon-add.png"), 0, "Join channel", id_set_topic);
	get_toolbar()->func_item_clicked().set(this, &ChatViewController::on_toolbar_item_clicked);

	func_resized().set(this, &ChatViewController::on_resize);
	func_visibility_change().set(this, &ChatViewController::on_visibility_change);
	inputbox->func_filter_message().set(this, &ChatViewController::on_inputbox_filter_message);

	userlist->func_key_pressed().set(this, &ChatViewController::on_userlist_key_pressed);
	userlist->func_mouse_right_up().set(this, &ChatViewController::on_userlist_contextmenu);

	on_resize();
	*/
}

ChatViewController::~ChatViewController()
{
}

void ChatViewController::create_layout()
{
	auto chat_users_group = std::make_shared<View>();
	chat_users_group->box_style.set_layout_hbox();
	chat_users_group->box_style.set_flex(1.0f, 1.0f);

	chat_log = std::make_shared<ChatView>();
	chat_log->box_style.set_flex(1.0f, 1.0f);
	chat_users_group->add_subview(chat_log);

	user_list = std::make_shared<UserListView>();
	user_list->box_style.set_width(200.0f);
	user_list->box_style.set_flex(0.0f, 0.0f);
	chat_users_group->add_subview(user_list);

	auto input_bar = std::make_shared<View>();
	input_bar->box_style.set_flex(0.0f, 0.0f);
	input_bar->box_style.set_background_gradient_to_bottom(Colorf(235, 243, 252), Colorf(219, 234, 249));
	input_bar->box_style.set_border(Colorf::gray60, 0.0f, 1.0f, 0.0f, 0.0f);
	input_bar->box_style.set_padding(5.0f);
	input_bar->box_style.set_layout_hbox();

	input_text = std::make_shared<TextFieldView>();
	input_text->box_style.set_border(Colorf::gray60, 1.0f);
	input_text->box_style.set_border_radius(2.0f);
	input_text->box_style.set_background(Colorf::white);
	input_text->box_style.set_padding(5.0f, 2.0f);
	input_text->text_style().set_font("Source Sans Pro", 12.0f, 20.0f);
	input_bar->add_subview(input_text);

	view->box_style.set_layout_vbox();
	view->add_subview(chat_users_group);
	view->add_subview(input_bar);
}

bool ChatViewController::is_active_view()
{
	/*
	View *view = get_mainframe()->get_active_view();
	if (get_mainframe()->get_active_view() == this)
	{
		return true;
	}
	else // Redirect to Status view (empty filter) if user is viewing another network
	{
		ChatViewController *chatview = dynamic_cast<ChatViewController*>(view);
		if (chatview && chatview->get_session() != get_session() && filter.empty())
			return true;
		else
			return false;
	}
	*/
	return true;
}

void ChatViewController::add_action_line(const IRCNick &nick, const IRCText &text)
{
	//get_mainframe()->flag_activity(this, nick.get_name(), text.get_text(), false);
	ChatLine line;
	//line.add_image(icon_action);
	add_line_text(line, " " + nick.get_name() + " " + text.get_text(), chat_log->get_color_channel());
	chat_log->add_line(line);
}

void ChatViewController::add_notice_line(const IRCNick &nick, const IRCText &text)
{
	//get_mainframe()->flag_activity(this, nick.get_name(), text.get_text(), false);
	ChatLine line(nick.get_name(), chat_log->get_color_nick_others());
	//line.add_image(icon_notice);
	add_line_text(line, " " + text.get_text(), chat_log->get_color_channel());
	chat_log->add_line(line);
}

void ChatViewController::add_error_line(const IRCText &text)
{
	//get_mainframe()->flag_activity(this, std::string(), text.get_text(), false);
	ChatLine line;
	//line.add_image(icon_error);
	add_line_text(line, " " + text.get_text(), chat_log->get_color_system());
	chat_log->add_line(line);
}

void ChatViewController::add_line(const IRCText &text, const Colorf &text_color, bool unimportant)
{
	//get_mainframe()->flag_activity(this, std::string(), text.get_text(), unimportant);
	ChatLine line;
	add_line_text(line, text.get_text(), text_color);
	chat_log->add_line(line);
}

void ChatViewController::add_line(const IRCNick &sender, const IRCText &text, const Colorf &text_color, const Colorf &nick_color)
{
	//get_mainframe()->flag_activity(this, sender.get_name(), text.get_text(), false);
	ChatLine line(sender.get_name(), nick_color);
	add_line_text(line, text.get_text(), text_color);
	chat_log->add_line(line);
}

void ChatViewController::add_line_text(ChatLine &line, const std::string &text, const Colorf &color)
{
	auto urls_begin = std::sregex_iterator(text.begin(), text.end(), regexp_url1);
	auto urls_end = std::sregex_iterator();

	TextStyle style;
	style.set_font("Source Sans Pro", 14.0f, 20.0f);
	style.set_color(color);

	TextStyle url_style;
	url_style.set_font("Source Sans Pro", 14.0f, 20.0f);
	url_style.set_weight_bold();
	url_style.set_color(chat_log->get_color_url());

	size_t pos = 0;
	for (auto it = urls_begin; it != urls_end; ++it)
	{
		auto match = *it;
		if (pos != match.position())
		{
			line.add_text(style, text.substr(pos, match.position() - pos));
		}

		ChatUrl url;
		url.object_id = next_chat_url_id++;
		url.url = match.str();
		chat_urls.push_back(url);

		line.add_text(url_style, match.str(), url.object_id);

		pos = match.position() + match.length();
	}

	if (pos != text.length())
		line.add_text(style, text.substr(pos));
}

void ChatViewController::irc_session_destroyed(IRCSession *destroyed_session)
{
	if (session == destroyed_session)
	{
		remove_from_parent_controller();
	}
}

void ChatViewController::irc_channel_parted(const IRCChannel &channel)
{
	if (channel == filter)
	{
		remove_from_parent_controller();
	}
}

void ChatViewController::on_text(const IRCChannel &room, const IRCNick &nick, const IRCText &text)
{
	if (get_filter() == room)
	{
		increment_message_count();
		add_line(nick, text, chat_log->get_color_text(), chat_log->get_color_nick_others());
	}
}

void ChatViewController::on_notice(const IRCChannel &room, const IRCNick &nick, const IRCText &text)
{
	if ((room.is_channel() && get_filter() == room) ||
		(!room.is_channel() && is_active_view()))
	{
		increment_message_count();
		add_notice_line(nick, text);
	}
}

void ChatViewController::on_action(const IRCChannel &room, const IRCNick &nick, const IRCText &text)
{
	if (get_filter() == room)
	{
		increment_message_count();
		add_action_line(nick, text);
	}
}

void ChatViewController::on_error_text(const IRCText &text)
{
	if (is_active_view())
	{
		add_error_line(text);
	}
}

void ChatViewController::on_system_text(const IRCText &text)
{
	if (get_filter().empty())
	{
		add_line(text, chat_log->get_color_system(), false);
	}
}

void ChatViewController::on_channel_topic_updated(const IRCChannel &channel)
{
	/*
	if (get_filter() == channel)
	{
		IRCJoinedChannel status = session->get_channel_status(channel);
		channel_topic->set_topic("Unknown time", status.topic, status.topic_author);
		on_resize();

		get_mainframe()->flag_activity(this, status.topic_author.get_name(), status.topic.get_text(), false);
		CL_SpanLayout layout;
		layout.add_image(icon_topic);
		add_layout_text(layout, " " + status.topic.get_text(), chat->get_color_channel());
		add_layout_text(layout, " set by ", CL_Colorf::gray);
		add_layout_text(layout, status.topic_author.get_name(), chat->get_color_channel());
		chat->add_line(layout);
	}
	*/
}

void ChatViewController::on_channel_names_updated(const IRCChannel &channel)
{
	if (get_filter() == channel)
	{
		IRCJoinedChannel status = session->get_channel_status(channel);

		user_list->clear();

		for (size_t i = 0; i < status.users.size(); i++)
		{
			auto &nick = status.users[i];

			std::string icon = icon_normal;
			if (nick.is_operator())
				icon = icon_operator;
			else if (nick.is_voiced())
				icon = icon_voiced;

			user_list->update_user(nick.get_label(), nick.get_name(), icon);
		}

		user_list->sort();
	}
}

void ChatViewController::on_nick_changed(const IRCNick &old_nick, const IRCNick &new_nick)
{
	if (user_list->has_user(old_nick.get_label()))
	{
		user_list->rename_user(old_nick.get_label(), new_nick.get_label(), new_nick.get_name());
		user_list->sort();

		add_line(IRCText::from_text(string_format("%1 is now known as %2", old_nick.get_name(), new_nick.get_name())), chat_log->get_color_channel(), true);
	}
}

void ChatViewController::on_parted(const IRCChannel &channel)
{
	if (get_filter() == channel)
	{
		add_line(IRCText::from_text("You left this channel"), chat_log->get_color_channel(), false);
	}
}

void ChatViewController::on_user_joined(const IRCChannel &channel, const IRCNick &nick)
{
	if (get_filter() == channel)
	{
		std::string icon = icon_normal;
		if (nick.is_operator())
			icon = icon_operator;
		else if (nick.is_voiced())
			icon = icon_voiced;

		user_list->update_user(nick.get_label(), nick.get_name(), icon);
		user_list->sort();

		add_line(IRCText::from_text(string_format("%1 joined the channel", nick.get_name())), chat_log->get_color_channel(), true);
	}
}

void ChatViewController::on_user_parted(const IRCChannel &channel, const IRCNick &nick, const IRCText &text)
{
	if (get_filter() == channel)
	{
		user_list->remove_user(nick.get_label());

		if (text.empty())
			add_line(IRCText::from_text(string_format("%1 left the channel", nick.get_name())), chat_log->get_color_channel(), true);
		else
			add_line(IRCText::from_text(string_format("%1 left the channel (%2)", nick.get_name(), text.get_text())), chat_log->get_color_channel(), true);
	}
}

void ChatViewController::on_user_kicked(const IRCNick &nick, const IRCChannel &channel, const IRCNick &target, const IRCText &text)
{
	if (get_filter() == channel)
	{
		user_list->remove_user(nick.get_label());

		if (text.empty())
			add_line(IRCText::from_text(string_format("%1 kicked %2 from the channel", nick.get_name(), target.get_name())), chat_log->get_color_channel(), false);
		else
			add_line(IRCText::from_text(string_format("%1 kicked %2 from the channel (%3)", nick.get_name(), target.get_name(), text.get_text())), chat_log->get_color_channel(), false);
	}
}

void ChatViewController::on_user_quit(const IRCNick &nick, const IRCText &text)
{
	if (user_list->has_user(nick.get_label()))
	{
		user_list->remove_user(nick.get_label());

		if (text.empty())
			add_line(IRCText::from_text(string_format("%1 quit", nick.get_name())), chat_log->get_color_channel(), true);
		else
			add_line(IRCText::from_text(string_format("%1 quit (%2)", nick.get_name(), text.get_text())), chat_log->get_color_channel(), true);
	}
}

void ChatViewController::on_channel_mode_change(const IRCNick &executing_nick, const IRCChannel &channel, const std::vector<IRCRawString> &command)
{
	if (get_filter() == channel)
	{
		std::string text;
		for (size_t i = 0; i < command.size(); i++)
		{
			if (!text.empty())
				text += " ";
			text += IRCText::from_raw(command[i]).get_text();
		}
		add_line(IRCText::from_text(string_format("%1 sets mode %2", executing_nick.get_name(), text)), chat_log->get_color_channel(), true);

		if (command.size() > 1)
		{
			IRCNick nick = IRCNick::from_raw(command[1]);

			std::string icon = icon_normal;
			if (command[0] == "+o")
				icon = icon_operator;
			else if (command[0] == "+v")
				icon = icon_voiced;

			user_list->update_user(nick.get_label(), nick.get_name(), icon);
		}
	}
}

void ChatViewController::on_nick_mode_change(const IRCNick &executing_nick, const IRCNick &target_nick, const std::vector<IRCRawString> &command)
{
	if (is_active_view())
	{
		std::string text;
		for (size_t i = 0; i < command.size(); i++)
		{
			if (!text.empty())
				text += " ";
			text += IRCText::from_raw(command[i]).get_text();
		}
		add_line(IRCText::from_text(string_format("%1 sets mode %2 on %3", executing_nick.get_name(), text, target_nick.get_name())), chat_log->get_color_system(), true);
	}
}

void ChatViewController::on_url_clicked(int object_id)
{
	for (size_t i = 0; i < chat_urls.size(); ++i)
	{
		if (chat_urls[i].object_id == object_id)
		{
#ifdef WIN32
			ShellExecute(NULL, L"open", clan::StringHelp::utf8_to_ucs2(chat_urls[i].url).c_str(), NULL, NULL, SW_SHOWNORMAL);
#endif
		}
	}
}

void ChatViewController::on_inputbox_return_pressed()
{
	std::string chat_line = input_text->text();
	if (!chat_line.empty())
	{
		chat_input_history.push_back(chat_line);
		chat_input_history_index = chat_input_history.size();

		std::vector<std::string> lines = split_input_text(chat_line);
		for (size_t i = 0; i < lines.size(); i++)
		{
			std::string &command_line = lines[i];
			if (!command_line.empty())
			{
				if (command_line[0] != '/')
				{
					session->send_text(get_filter(), IRCText::from_text(command_line));
					add_line(session->get_nick(), IRCText::from_text(command_line), chat_log->get_color_text(), chat_log->get_color_nick());
				}
				else
				{
					try
					{
						session->execute_command(get_filter(), command_line);
					}
					catch (Exception &e)
					{
						add_error_line(IRCText::from_text(e.message));
					}
				}
			}
		}

		input_text->set_text("");
	}
}

std::vector<std::string> ChatViewController::split_input_text(const std::string &text)
{
	std::vector<std::string> lines;
	std::string::size_type pos = 0;
	while (pos < text.length())
	{
		std::string::size_type start = pos;
		std::string::size_type end = text.find('\n', start);
		if (end == std::string::npos)
			break;
		pos = end + 1;

		if (end > 0 && text[end - 1] == '\r')
			end--;

		if (end > start)
			lines.push_back(text.substr(start, end - start));
	}
	if (pos < text.length())
		lines.push_back(text.substr(pos));
	return lines;
}

/*
std::string ChatViewController::get_view_caption(IRCSession *session, const IRCEntity &filter)
{
	if (filter.empty())
		return session->get_connection_name();// "Status";
	else if (filter.is_channel())
		return "#"+clan::StringHelp::text_to_lower(filter.get_name());
	else
		return filter.get_name();
}

int ChatViewController::add_listview_icon(const std::string &filename)
{
	CL_GraphicContext gc = get_gc();
	CL_Sprite sprite(gc, filename);
	CL_ListViewIcon icon;
	icon.set_sprite(sprite, listview_mode_details);
	CL_ListViewIconList icon_list = userlist->get_icon_list();
	icon_list.set_icon(next_icon_index, icon);
	return next_icon_index++;
}

ChatViewController::~ChatViewController()
{
	if (filter.is_channel())
	{
		session->part(filter);
	}
	else if (filter.empty())
	{
		// What should we do if the Status window is closed?
		session->disconnect_graceful("");
	}
}

void ChatViewController::on_resize()
{
	CL_Rect contentbox = part_background.get_content_box(get_size());

	int topic_height = channel_topic->get_topic_height(contentbox.get_width());
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

void ChatViewController::on_inputbox_filter_message(CL_GUIMessage &message)
{
	if (message.get_type() == CL_GUIMessage_Input::get_type_name())
	{
		CL_InputEvent input_event = CL_GUIMessage_Input(message).get_event();
		if (input_event.id == CL_KEY_ENTER)
		{
			if (input_event.type == CL_InputEvent::pressed)
				on_inputbox_return_pressed();
			message.set_consumed();
		}
		else if (input_event.id == CL_KEY_TAB && !input_event.ctrl && !input_event.alt)
		{
			if (input_event.type == CL_InputEvent::pressed)
				on_inputbox_tab_pressed();
			message.set_consumed();
		}
		else if (input_event.id == CL_KEY_UP)
		{
			if (input_event.type == CL_InputEvent::pressed)
				on_inputbox_up_pressed();
			message.set_consumed();
		}
		else if (input_event.id == CL_KEY_DOWN)
		{
			if (input_event.type == CL_InputEvent::pressed)
				on_inputbox_down_pressed();
			message.set_consumed();
		}
		else if (input_event.id == CL_KEY_PRIOR)
		{
			if (input_event.type == CL_InputEvent::pressed)
				chat->scroll_page_up();
			message.set_consumed();
		}
		else if (input_event.id == CL_KEY_NEXT)
		{
			if (input_event.type == CL_InputEvent::pressed)
				chat->scroll_page_down();
			message.set_consumed();
		}
	}

	int new_inputbox_height = cl_clamp(inputbox->get_total_height(), inputbox_min_height, inputbox_min_height * 4);
	if (inputbox_height != new_inputbox_height)
	{
		inputbox_height = new_inputbox_height;
		on_resize();
	}
}

void ChatViewController::on_inputbox_up_pressed()
{
	if(chat_input_history_index > 0)
	{
		chat_input_history_index--;
		inputbox->set_text(chat_input_history[chat_input_history_index]);
	}
}

void ChatViewController::on_inputbox_down_pressed()
{
	if( chat_input_history.size() > 0 && chat_input_history_index < chat_input_history.size() - 1)
	{
		chat_input_history_index++;
		inputbox->set_text(chat_input_history[chat_input_history_index]);
	}
	else
	{
		inputbox->set_text(std::string());
	}
}

void ChatViewController::on_inputbox_tab_pressed()
{
	if (get_filter().is_channel())
	{
		int cursor_pos = inputbox->get_cursor_pos();
		if(cursor_pos > 0)
		{
			std::string line = inputbox->get_text();
			std::string::size_type pos = line.rfind(' ', cursor_pos);

			if(pos == std::string::npos)
				pos = 0;
			else
				pos = pos + 1;
		
			std::stringRef search_nick = line.substr(pos, cursor_pos - pos);
			int search_length = search_nick.length();

			IRCJoinedChannel status = session->get_channel_status(get_filter());
			for (size_t i = 0; i < status.users.size(); i++)
			{
				std::string user_name = status.users[i].get_name();
				if(clan::StringHelp::compare(user_name.substr(0, search_length), search_nick, true) == 0)
				{
					std::stringRef prefix = line.substr(0, pos);
					std::stringRef postfix = line.substr(cursor_pos);

					inputbox->set_text(prefix + user_name + postfix);
					inputbox->set_cursor_pos(prefix.length() + user_name.length());
					return;
				}
			}
		}
	}
}

void ChatViewController::on_userlist_key_pressed(CL_InputEvent event)
{
	if (event.id == CL_KEY_APPS)
	{
		on_userlist_contextmenu(CL_Point(-1, -1));
	}
}

void ChatViewController::on_userlist_contextmenu(CL_Point pos)
{
	userlist_popup_menu = CL_PopupMenu();
	userlist_popup_menu.insert_item("Slap!").func_clicked().set(this, &ChatViewController::on_userlist_slap);
	userlist_popup_menu.insert_item("Open Conversation").func_clicked().set(this, &ChatViewController::on_userlist_open_conversation);
	userlist_popup_menu.insert_item("Open DCC Conversation").func_clicked().set(this, &ChatViewController::on_userlist_open_dcc_conversation);
    userlist_popup_menu.insert_item("Whois").func_clicked().set(this, &ChatViewController::on_userlist_whois);
	userlist_popup_menu.start(userlist, userlist->component_to_screen_coords(pos));
}

void ChatViewController::on_userlist_slap()
{
	CL_ListViewItem selected_item = userlist->get_selected_item();
	if (!selected_item.is_null())
		Command::execute(session, filter, string_format("/me slaps %1 around a bit with a large carambola fruit!", selected_item.get_column("nick").get_text()));
}

void ChatViewController::on_userlist_open_conversation()
{
	CL_ListViewItem selected_item = userlist->get_selected_item();
	if (!selected_item.is_null())
		get_mainframe()->open_conversation(IRCNick::from_text(selected_item.get_column("nick").get_text()), session);
}

void ChatViewController::on_userlist_open_dcc_conversation()
{
	CL_ListViewItem selected_item = userlist->get_selected_item();
	if (!selected_item.is_null())
		get_mainframe()->open_dcc_conversation(IRCNick::from_text(selected_item.get_column("nick").get_text()), session);
}

void ChatViewController::on_userlist_whois()
{
    CL_ListViewItem selected_item = userlist->get_selected_item();
    if (!selected_item.is_null())
        Command::execute(session, filter, string_format("/whois %1", selected_item.get_column("nick").get_text()));
}

void ChatViewController::add_private_text(const IRCNick &nick, const IRCText &text)
{
	if (get_filter() == nick)
	{
		add_line(nick, text, chat->get_color_text(), chat->get_color_nick_others());
	}
}

void ChatViewController::sort_userlist()
{
//	std::vector<ListViewItemSortPosition> positions;
//	CL_ListViewItem root = userlist->get_document_item();
//	for (CL_ListViewItem cur = root.get_first_child(); !cur.is_null(); cur = cur.get_next_sibling())
//	{
//		ListViewItemSortPosition p;
//		p.item = cur;
//		p.text = cur.get_column("nick").get_text();
//		positions.push_back(p);
//	}
//
//	std::stable_sort(positions.begin(), positions.end());
//
//	root.remove_children();
//	for (size_t i = 0; i < positions.size(); i++)
//		root.append_child(positions[i].item);
}

CL_ListViewItem ChatViewController::find_user_item(const IRCNick &nick)
{
	CL_ListViewItem cur = userlist->get_document_item().get_first_child();
	while (!cur.is_null())
	{
		if (clan::StringHelp::compare(cur.get_column("nick").get_text(), nick.get_name(), true) == 0)
			return cur;

		cur = cur.get_next_sibling();
	}
	return CL_ListViewItem();
}

void ChatViewController::on_visibility_change(bool new_visibility)
{
	if (new_visibility)
		inputbox->set_focus();
}

void ChatViewController::on_toolbar_item_clicked(CL_ToolBarItem item)
{
	switch (item.get_id())
	{
	case id_disconnect:
		on_toolbar_disconnect_clicked();
		break;
	case id_set_topic:
		on_toolbar_set_topic_clicked();
		break;
	case id_leave:
		on_toolbar_leave_clicked();
		break;
	case id_moderate:
		on_toolbar_moderate_clicked();
		break;
	case id_join:
		on_toolbar_join_clicked();
		break;
	default:
		break;
	}
}

void ChatViewController::on_toolbar_set_topic_clicked()
{
	DlgTopic dlg(this);
	dlg.exec();
}

void ChatViewController::on_toolbar_disconnect_clicked()
{
	session->execute_command(filter, "/quit");
}

void ChatViewController::on_toolbar_leave_clicked()
{

}

void ChatViewController::on_toolbar_moderate_clicked()
{

}

void ChatViewController::on_toolbar_join_clicked()
{

}
*/
