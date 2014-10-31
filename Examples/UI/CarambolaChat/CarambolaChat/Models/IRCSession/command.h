
#pragma once

class IRCSession;
class IRCEntity;

class Command
{
public:
	static void execute(IRCSession *session, const IRCEntity &filter, std::string command_line);

private:
	static void execute_time(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args);
	static void execute_ping(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args);
	static void execute_clientinfo(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args);
	static void execute_userinfo(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args);
	static void execute_version(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args);
	static void execute_finger(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args);
	static void execute_me(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args);
	static void execute_kill(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args);
	static void execute_whowas(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args);
	static void execute_whois(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args);
	static void execute_who(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args);
	static void execute_notice_ctcp(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args);
	static void execute_notice(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args);
	static void execute_ctcp(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args);
	static void execute_privmsg(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args);
	static void execute_info(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args);
	static void execute_admin(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args);
	static void execute_trace(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args);
	static void execute_connect(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args);
	static void execute_send_time(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args);
	static void execute_links(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args);
	static void execute_stats(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args);
	static void execute_send_version(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args);
	static void execute_kick(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args);
	static void execute_invite(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args);
	static void execute_list(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args);
	static void execute_names(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args);
	static void execute_topic(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args);
	static void execute_mode(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args);
	static void execute_part(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args);
	static void execute_join(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args);
	static void execute_quit(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args);
	static void execute_nick(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args);
	static void execute_oper(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args);
	static void execute_dcc(IRCSession *session, const IRCEntity &filter, std::vector<std::string> &args);

	struct CommandFunc
	{
		const char *name;
		void (*func)(IRCSession *, const IRCEntity &, std::vector<std::string> &);
	};
};
