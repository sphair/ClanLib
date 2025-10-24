
#ifndef file_chat
#define file_chat

#include <list>

class View;
class IRCConnection;
class ChatLine;

class Chat : public CL_Component
{
//! Construction:
public:
	Chat(IRCConnection *connection, const std::string &filter, CL_Component *parent);

	~Chat();

//! Attributes:
public:
	CL_Color color_text, color_channel, color_system;

	const std::string &get_filter() const { return filter; }

//! Operations:
public:
	void add_line(const std::string &text, const CL_Color &color = CL_Color::black);

	void add_line(const std::string &nick, const std::string &text, const CL_Color &color = CL_Color::black);

//! Implementation:
private:
	void on_resize(int old_width, int old_height);

	void on_paint();

	void on_scroll_value_changed(int value);

	void on_connection_command_received(const std::string &prefix, const std::string &command, const std::vector<std::string> &params);

	void on_connection_unknown_command_received(const std::string &prefix, const std::string &command, const std::vector<std::string> &params);

	void on_connection_numeric_reply(const std::string &prefix, int command, const std::vector<std::string> &params);

	void on_connection_name_reply(const std::string &self, const std::string &channel, const std::vector<std::string> &users);

	void on_connection_nick(const std::string &old_nick, const std::string &new_nick);

	void on_connection_join(const std::string &nick, const std::string &channel);

	void on_connection_part(const std::string &nick, const std::string &channel, const std::string &reason);

	void on_connection_mode(const std::string &prefix, const std::string &receiver, const std::string &mode, const std::vector<std::string> &params);

	void on_connection_topic(const std::string &prefix, const std::string &channel, const std::string &topic);

	void on_connection_invite(const std::string &prefix, const std::string &nick, const std::string &channel);

	void on_connection_kick(const std::string &prefix, const std::string &channel, const std::string &user, const std::string &comment);

	void on_connection_privmsg(const std::string &prefix, const std::string &receiver, const std::string &text);

	void on_connection_notice(const std::string &prefix, const std::string &receiver, const std::string &text);

	void on_connection_privmsg_ctcp(const std::string &prefix, const std::string &receiver, const std::string &command, const std::string &data);

	void on_connection_notice_ctcp(const std::string &prefix, const std::string &receiver, const std::string &command, const std::string &data);

	void on_connection_ping(const std::string &daemon1, const std::string &daemon2);

	void on_typepacket(const std::string &from, const std::string &receiver, const std::string &text);

	CL_ScrollBar *scroll;

	IRCConnection *connection;

	std::string filter;

	std::list<ChatLine *> lines;

	std::map<std::string, std::string> typetext;

	CL_Font font;

	CL_SlotContainer slots;
};

#endif
