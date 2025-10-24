
#ifndef file_chatview
#define file_chatview

#include "view.h"
#include "chat.h"

class IRCConnection;

class ChatView : public View
{
//! Construction:
public:
	ChatView(IRCConnection *connection, const std::string &filter, MainFrame *mainframe);

	~ChatView();

//! Attributes:
public:
	const std::string &get_filter() const { return chat->get_filter(); }

//! Operations:
public:

//! Implementation:
private:
	void on_resize(int old_width, int old_height);

	void on_paint();

	void on_inputbox_return_pressed();

	void on_inputbox_changed(const std::string &text);

	void on_userlist_activated(int item_id);

	void on_userlist_key_up(const CL_InputEvent &event);

	void on_userlist_mouse_up(const CL_InputEvent &event);

	void on_userlist_contextmenu();

	void on_connection_numeric_reply(const std::string &prefix, int command, const std::vector<std::string> &params);

	void on_connection_nick(const std::string &_old_nick, const std::string &_new_nick);

	void on_connection_join(const std::string &nick, const std::string &channel);

	void on_connection_part(const std::string &nick, const std::string &channel, const std::string &reason);

	Chat *chat;
	
	CL_InputBox *inputbox;

	CL_ListBox *userlist;

	IRCConnection *connection;

	CL_SlotContainer slots;
};

#endif
