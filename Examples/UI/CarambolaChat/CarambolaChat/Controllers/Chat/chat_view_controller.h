
#pragma once

#include "Models/IRCSession/irc_entity.h"
#include "Models/IRCSession/irc_text.h"
#include "Views/Chat/chat_view.h"

class WorkspaceViewController;
class IRCSession;
class IRCEntity;
class ChatUrl;
class ChatView;
class ChatLine;
class UserListView;

class ChatViewController : public clan::ViewController
{
public:
	ChatViewController(IRCSession *session, const IRCEntity &filter);
	~ChatViewController();

	const IRCEntity &get_filter() const { return filter; }
	IRCSession *get_session() const { return session; }

	//void add_private_text(const IRCNick &nick, const IRCText &text);

private:
	void create_layout();
	bool is_active_view();

	std::vector<std::string> split_input_text(const std::string &text);

	void add_line(const IRCText &text, const clan::Colorf &text_color, bool unimportant);
	void add_line(const IRCNick &sender, const IRCText &text, const clan::Colorf &text_color, const clan::Colorf &nick_color);
	void add_action_line(const IRCNick &nick, const IRCText &text);
	void add_notice_line(const IRCNick &nick, const IRCText &text);
	void add_error_line(const IRCText &text);
	void add_line_text(ChatLine &line, const std::string &text, const clan::Colorf &color);

	void on_channel_topic_updated(const IRCChannel &channel);
	void on_channel_names_updated(const IRCChannel &channel);
	void on_nick_changed(const IRCNick &old_nick, const IRCNick &new_nick);
	void on_parted(const IRCChannel &channel);
	void on_user_joined(const IRCChannel &channel, const IRCNick &nick);
	void on_user_parted(const IRCChannel &channel, const IRCNick &nick, const IRCText &text);
	void on_user_kicked(const IRCNick &nick, const IRCChannel &channel, const IRCNick &target, const IRCText &text);
	void on_user_quit(const IRCNick &nick, const IRCText &text);

	void on_text(const IRCEntity &room, const IRCNick &nick, const IRCText &text);
	void on_notice(const IRCEntity &room, const IRCNick &nick, const IRCText &text);
	void on_action(const IRCEntity &room, const IRCNick &nick, const IRCText &text);
	void on_error_text(const IRCText &text);
	void on_system_text(const IRCText &text);

	void on_channel_mode_change(const IRCNick &executing_nick, const IRCChannel &channel, const std::vector<IRCRawString> &command);
	void on_nick_mode_change(const IRCNick &executing_nick, const IRCNick &target_nick, const std::vector<IRCRawString> &command);

	void irc_session_destroyed(IRCSession *session);
	void irc_channel_parted(const IRCChannel &channel);

	void on_url_clicked(int object_id);
	void on_inputbox_return_pressed();

	IRCSession *session = 0;
	IRCEntity filter;

	std::vector<std::string> chat_input_history;
	unsigned int chat_input_history_index = 0;

	std::shared_ptr<ChatView> chat_log;
	std::shared_ptr<UserListView> user_list;
	std::shared_ptr<clan::TextFieldView> input_text;

	clan::SlotContainer slots;
/*
	static std::string get_view_caption(IRCSession *session, const IRCEntity &filter);

	void on_resize();
	void on_visibility_change(bool new_visibility);

	void on_inputbox_filter_message(CL_GUIMessage &message);
	void on_inputbox_tab_pressed();
	void on_inputbox_up_pressed();
	void on_inputbox_down_pressed();

	void on_userlist_key_pressed(CL_InputEvent event);
	void on_userlist_contextmenu(CL_Point pos);
	void on_userlist_slap();
	void on_userlist_open_conversation();
	void on_userlist_open_dcc_conversation();
    void on_userlist_whois();

	CL_ListViewItem find_user_item(const IRCNick &nick);
	int add_listview_icon(const std::string &filename);
	void sort_userlist();

	void on_toolbar_item_clicked(CL_ToolBarItem item);
	void on_toolbar_disconnect_clicked();
	void on_toolbar_set_topic_clicked();
	void on_toolbar_leave_clicked();
	void on_toolbar_moderate_clicked();
	void on_toolbar_join_clicked();

	CL_GUIThemePart part_background;
	ChannelTopic *channel_topic = 0;
	Chat *chat = 0;
	TextFieldView *inputbox = 0;
	CL_ListView *userlist = 0;
	CL_SlotContainer slots;

	const static int inputbox_min_height = 22;
	int inputbox_height = inputbox_min_height;

	std::vector<ChatUrl> chat_urls;
	int next_chat_url_id = 1;

	int icon_normal_index, icon_operator_index, icon_voiced_index;
	int next_icon_index = 1;

	CL_RegExp regexp_url1 = "(https?://.*?)([ \\r\\n\\t\"'>)]|$)";

	CL_Image icon_action;
	CL_Image icon_notice;
	CL_Image icon_error;
	CL_Image icon_topic;

	struct ListViewItemSortPosition
	{
		std::string text;
		CL_ListViewItem item;

		bool operator<(const ListViewItemSortPosition &other) const
		{
			return std::stringHelp::compare(text, other.text, true) < 0;
		}
	};
	CL_PopupMenu userlist_popup_menu;

	enum ToolbarButtonID
	{
		id_disconnect,
		id_set_topic,
		id_leave,
		id_moderate,
		id_join
	};
*/
};
