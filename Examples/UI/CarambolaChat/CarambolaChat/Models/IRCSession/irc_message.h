
#pragma once

#include "irc_entity.h"
#include "irc_text.h"

class IRCMessage
{
public:
	IRCMessage(const IRCMessage &src);
	IRCMessage(const IRCRawString &prefix, const IRCRawString &command, const std::vector<IRCRawString> &params);

	static IRCRawString create_line(const IRCRawString &prefix, const IRCRawString &command, const std::vector<IRCRawString> &params);
	static IRCMessage parse_line(IRCRawString line);

	enum Type
	{
		type_numeric_reply,
		type_nick,
		type_join,
		type_part,
		type_kick,
		type_channel_mode,
		type_nick_mode,
		type_topic,
		type_privmsg,
		type_notice,
		type_ping,
		type_quit,
		type_unknown
	};

	Type get_type() const;
	IRCEntity get_prefix() const;
	IRCRawString get_command() const;
	IRCRawString get_param(size_t index) const;
	size_t get_param_count() const;
	const std::vector<IRCRawString> &get_params() const { return params; }

private:
	IRCRawString prefix; // <servername> | <nick> [ '!' <user> ] [ '@' <host> ]
	IRCRawString command;
	std::vector<IRCRawString> params;
};

class IRCNumericReply : public IRCMessage
{
public:
	IRCNumericReply(const IRCMessage &src) : IRCMessage(src) { }

	int get_numeric() const { return clan::StringHelp::local8_to_int(get_command()); }
};

class IRCNickMessage : public IRCMessage
{
public:
	IRCNickMessage(const IRCMessage &src) : IRCMessage(src) { }

	IRCNick get_old_nick() const { return get_prefix(); }
	IRCNick get_new_nick() const { return IRCNick::from_raw(get_param(0)); }
};

class IRCJoinMessage : public IRCMessage
{
public:
	IRCJoinMessage(const IRCMessage &src) : IRCMessage(src) { }

	IRCNick get_user() const { return get_prefix(); }
	IRCChannel get_channel() const { return IRCChannel::from_raw(get_param(0)); }
};

class IRCPartMessage : public IRCMessage
{
public:
	IRCPartMessage(const IRCMessage &src) : IRCMessage(src) { }

	IRCNick get_user() const { return get_prefix(); }
	IRCChannel get_channel(size_t index) const { return IRCChannel::from_raw(get_param(index)); }
	size_t get_channel_count() const { return get_param_count(); }
};

class IRCKickMessage : public IRCMessage
{
public:
	IRCKickMessage(const IRCMessage &src) : IRCMessage(src) { }

	IRCNick get_user() const { return get_prefix(); }
	IRCChannel get_channel() const { return IRCChannel::from_raw(get_param(0)); }
	IRCNick get_target() const { return IRCNick::from_raw(get_param(1)); }
	bool has_comment() const { return get_param_count() > 2; }
	IRCText get_comment() const { return IRCText::from_raw(get_param(2)); }
};

class IRCQuitMessage : public IRCMessage
{
public:
	IRCQuitMessage(const IRCMessage &src) : IRCMessage(src) { }

	IRCNick get_user() const { return get_prefix(); }
	IRCText get_text() const { return IRCText::from_raw(get_param(0)); }
	bool has_text() const { return get_param_count() > 0; }
};

class IRCChannelModeMessage : public IRCMessage
{
public:
	IRCChannelModeMessage(const IRCMessage &src) : IRCMessage(src) { }

	IRCNick get_executing_user() const { return get_prefix(); }
	IRCChannel get_channel() const { return IRCChannel::from_raw(get_param(0)); }
	IRCRawString get_mode_command() const { return get_param(1); }
	IRCRawString get_mode_parameter(size_t index) const { return get_param(2+index); }
	size_t get_mode_parameter_count() const { return get_param_count()-2; }
};

class IRCNickModeMessage : public IRCMessage
{
public:
	IRCNickModeMessage(const IRCMessage &src) : IRCMessage(src) { }

	IRCNick get_executing_user() const { return get_prefix(); }
	IRCChannel get_target_user() const { return IRCChannel::from_raw(get_param(0)); }
	IRCRawString get_mode_command() const { return get_param(1); }
};

class IRCTopicMessage : public IRCMessage
{
public:
	IRCTopicMessage(const IRCMessage &src) : IRCMessage(src) { }

	IRCNick get_user() const { return get_prefix(); }
	IRCChannel get_channel() const { return IRCChannel::from_raw(get_param(0)); }
	IRCText get_topic() const { return IRCText::from_raw(get_param(1)); }
};

class IRCPrivateMessage : public IRCMessage
{
public:
	IRCPrivateMessage(const IRCMessage &src) : IRCMessage(src) { }

	IRCEntity get_sender() const { return get_prefix(); }
	IRCEntity get_target() const { return IRCEntity::from_raw(get_param(0)); }
	IRCText get_text() const { return IRCText::from_raw(get_param(1)); }
};

class IRCNoticeMessage : public IRCMessage
{
public:
	IRCNoticeMessage(const IRCMessage &src) : IRCMessage(src) { }

	IRCEntity get_sender() const { return get_prefix(); }
	IRCEntity get_target() const { return IRCEntity::from_raw(get_param(0)); }
	IRCText get_text() const { return IRCText::from_raw(get_param(1)); }
};

class IRCPingMessage : public IRCMessage
{
public:
	IRCPingMessage(const IRCMessage &src) : IRCMessage(src) { }

	IRCRawString get_daemon1() const { return get_param(0); }
	IRCRawString get_daemon2() const { return get_param(1); }
	bool has_daemon2() const { return get_param_count() > 1; }
};
