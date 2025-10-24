
#pragma once

#include "view.h"
#include "chat.h"

class IRCConnection;

class ChatView : public View
{
//! Construction:
public:
	ChatView(IRCConnection *connection, const CL_String &filter, CL_GUIComponent *parent, MainFrame *mainframe);
	~ChatView();

//! Attributes:
public:
	const CL_String &get_filter() const { return chat->get_filter(); }
	bool is_channel() const;

//! Operations:
public:

//! Implementation:
private:
	void on_resize();
	void on_render(CL_GraphicContext &gc, const CL_Rect &clip_rect);

	bool on_inputbox_unhandled_input(CL_InputEvent input_event);
	void on_inputbox_return_pressed();
#if defined(netsession_implemented)
	void on_inputbox_changed(const CL_String &text);
#endif

	void on_userlist_activated(int item_id);
	void on_userlist_key_up(const CL_InputEvent &event);
	void on_userlist_mouse_up(const CL_InputEvent &event);
	void on_userlist_contextmenu();

	void on_connection_numeric_reply(const CL_String &prefix, int command, const std::vector<CL_String> &params);
	void on_connection_nick(const CL_String &_old_nick, const CL_String &_new_nick);
	void on_connection_join(const CL_String &nick, const CL_String &channel);
	void on_connection_part(const CL_String &nick, const CL_String &channel, const CL_String &reason);
	void on_connection_unknown_command_received(const CL_String &prefix, const CL_String &command, const std::vector<CL_String> &params);

	Chat *chat;
	CL_LineEdit *inputbox;
	CL_ListView *userlist;
	IRCConnection *connection;
	CL_SlotContainer slots;
};
