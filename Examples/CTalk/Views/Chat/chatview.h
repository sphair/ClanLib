
#pragma once

#include "Framework/view.h"
#include "Document/irc_entity.h"
#include "Document/irc_text.h"
#include "chat.h"
#include "channel_topic.h"
#include "text_edit.h"

typedef CL_LineEdit ChatLineEdit;
//typedef TextEdit ChatLineEdit;

class IRCSession;
class IRCEntity;
class ChatUrl;

class ChatView : public View
{
public:
	ChatView(IRCSession *session, const IRCEntity &filter, CL_GUIComponent *parent, MainFrame *mainframe);
	~ChatView();

	const IRCEntity &get_filter() const { return filter; }
	IRCSession *get_session() const { return session; }

	void add_private_text(const IRCNick &nick, const IRCText &text);

private:
	static CL_String get_view_caption(IRCSession *session, const IRCEntity &filter);

	void on_resize();
	void on_render(CL_GraphicContext &gc, const CL_Rect &clip_rect);
	void on_visibility_change(bool new_visibility);

	void on_inputbox_filter_message(CL_GUIMessage &message);
	void on_inputbox_tab_pressed();
	void on_inputbox_return_pressed();
	void on_inputbox_up_pressed();
	void on_inputbox_down_pressed();

	void on_url_clicked(int object_id);

	void on_userlist_key_pressed(CL_InputEvent event);
	void on_userlist_contextmenu(CL_Point pos);
	void on_userlist_slap();
	void on_userlist_open_conversation();

	void on_channel_topic_updated(const IRCChannel &channel);

	void on_channel_names_updated(const IRCChannel &channel);
	void on_nick_changed(const IRCNick &old_nick, const IRCNick &new_nick);
	void on_parted(const IRCChannel &channel);
	void on_user_joined(const IRCChannel &channel, const IRCNick &nick);
	void on_user_parted(const IRCChannel &channel, const IRCNick &nick, const IRCText &text);
	void on_user_quit(const IRCNick &nick, const IRCText &text);

	void on_text(const IRCEntity &room, const IRCNick &nick, const IRCText &text);
	void on_notice(const IRCEntity &room, const IRCNick &nick, const IRCText &text);
	void on_action(const IRCEntity &room, const IRCNick &nick, const IRCText &text);
	void on_error_text(const IRCText &text);
	void on_system_text(const IRCText &text);

	void on_channel_mode_change(const IRCNick &executing_nick, const IRCChannel &channel, const std::vector<IRCRawString> &command);
	void on_nick_mode_change(const IRCNick &executing_nick, const IRCNick &target_nick, const std::vector<IRCRawString> &command);

	std::vector<CL_String> split_input_text(const CL_String &text);

	CL_ListViewItem find_user_item(const IRCNick &nick);
	int add_listview_icon(const CL_String &filename);
	void sort_userlist();

	void add_line(const IRCText &text, const CL_Colorf &text_color);
	void add_line(const IRCNick &sender, const IRCText &text, const CL_Colorf &text_color, const CL_Colorf &nick_color);

	void add_layout_text(CL_SpanLayout &layout, const CL_String &text, const CL_Colorf &color);

	void add_action_line(const IRCNick &nick, const IRCText &text);
	void add_notice_line(const IRCNick &nick, const IRCText &text);
	void add_error_line(const IRCText &text);

	bool is_active_view();

	CL_GUIThemePart part_background;
	ChannelTopic *channel_topic;
	Chat *chat;
	ChatLineEdit *inputbox;
	CL_ListView *userlist;
	IRCSession *session;
	IRCEntity filter;
	CL_SlotContainer slots;

	std::vector<CL_String> chat_input_history;
	unsigned int chat_input_history_index;

	std::vector<ChatUrl> chat_urls;
	int next_chat_url_id;

	int icon_normal_index, icon_operator_index, icon_voiced_index;
	int next_icon_index;

	CL_RegExp regexp_url1;

	CL_Image icon_action;
	CL_Image icon_notice;
	CL_Image icon_error;
	CL_Image icon_topic;

	struct ListViewItemSortPosition
	{
		CL_String text;
		CL_ListViewItem item;

		bool operator<(const ListViewItemSortPosition &other) const
		{
			return CL_StringHelp::compare(text, other.text, true) < 0;
		}
	};
	CL_PopupMenu userlist_popup_menu;

};
