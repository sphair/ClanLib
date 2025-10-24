
#ifndef header_clanbot
#define header_clanbot

#include <ClanLib/application.h>
#include <ClanLib/core.h>
#include <ClanLib/network.h>

class ClanBotConnection : public CL_ClanApplication
{
public:
	int main(int argc, char **argv);

public:
	void on_log(const std::string &id, int severity, const std::string &message);

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
	
	std::string server;
	
	CL_IRCConnection *connection;
	
	bool connected;
	
	CL_SlotContainer slots;
};

#endif
