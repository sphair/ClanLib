
#pragma once

#include <list>

class View;
class IRCConnection;
class ChatLine;

class Chat : public CL_GUIComponent
{
//! Construction:
public:
	Chat(IRCConnection *connection, const CL_String &filter, CL_GUIComponent *parent);
	~Chat();

//! Attributes:
public:
	CL_Colorf color_text, color_channel, color_system;

	const CL_String &get_filter() const { return filter; }

//! Operations:
public:
	void add_line(const CL_String &text, const CL_Colorf &color = CL_Colorf::black);
	void add_line(const CL_String &nick, const CL_String &text, const CL_Colorf &color = CL_Colorf::black);

//! Implementation:
private:
	void on_resize();
	void on_render(CL_GraphicContext &gc, const CL_Rect &clip_rect);
	void on_scroll();
	void on_connection_command_received(const CL_String &prefix, const CL_String &command, const std::vector<CL_String> &params);
	void on_connection_unknown_command_received(const CL_String &prefix, const CL_String &command, const std::vector<CL_String> &params);
	void on_connection_numeric_reply(const CL_String &prefix, int command, const std::vector<CL_String> &params);
	void on_connection_name_reply(const CL_String &self, const CL_String &channel, const std::vector<CL_String> &users);
	void on_connection_nick(const CL_String &old_nick, const CL_String &new_nick);
	void on_connection_join(const CL_String &nick, const CL_String &channel);
	void on_connection_part(const CL_String &nick, const CL_String &channel, const CL_String &reason);
	void on_connection_mode(const CL_String &prefix, const CL_String &receiver, const CL_String &mode, const std::vector<CL_String> &params);
	void on_connection_topic(const CL_String &prefix, const CL_String &channel, const CL_String &topic);
	void on_connection_invite(const CL_String &prefix, const CL_String &nick, const CL_String &channel);
	void on_connection_kick(const CL_String &prefix, const CL_String &channel, const CL_String &user, const CL_String &comment);
	void on_connection_privmsg(const CL_String &prefix, const CL_String &receiver, const CL_String &text);
	void on_connection_notice(const CL_String &prefix, const CL_String &receiver, const CL_String &text);
	void on_connection_privmsg_ctcp(const CL_String &prefix, const CL_String &receiver, const CL_String &command, const CL_String &data);
	void on_connection_notice_ctcp(const CL_String &prefix, const CL_String &receiver, const CL_String &command, const CL_String &data);
	void on_connection_ping(const CL_String &daemon1, const CL_String &daemon2);
	void on_typepacket(const CL_String &from, const CL_String &receiver, const CL_String &text);
	static CL_FontDescription get_font_description();

	CL_ScrollBar *scroll;
	IRCConnection *connection;
	CL_String filter;
	std::list<ChatLine *> lines;
	std::map<CL_String, CL_String> typetext;
	CL_Font font;
	CL_SlotContainer slots;
};
