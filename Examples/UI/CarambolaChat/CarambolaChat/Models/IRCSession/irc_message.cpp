
#include "precomp.h"
#include "irc_message.h"

IRCMessage::IRCMessage(const IRCMessage &src)
: prefix(src.prefix), command(src.command), params(src.params)
{
}

IRCMessage::IRCMessage(const IRCRawString &prefix, const IRCRawString &command, const std::vector<IRCRawString> &params)
: prefix(prefix), command(command), params(params)
{
}

IRCMessage::Type IRCMessage::get_type() const
{
	if (clan::StringHelp::compare(command, "PRIVMSG", true) == 0 && get_param_count() >= 2) return type_privmsg;
	else if (!command.empty() && command[0] >= '0' && command[0] <= '9') return type_numeric_reply;
	else if (clan::StringHelp::compare(command, "NOTICE", true) == 0 && get_param_count() >= 2) return type_notice;
	else if (clan::StringHelp::compare(command, "PING", true) == 0 && get_param_count() >= 1) return type_ping;
	else if (clan::StringHelp::compare(command, "NICK", true) == 0 && get_param_count() >= 1) return type_nick;
	else if (clan::StringHelp::compare(command, "JOIN", true) == 0 && get_param_count() >= 1) return type_join;
	else if (clan::StringHelp::compare(command, "PART", true) == 0 && get_param_count() >= 1) return type_part;
	else if (clan::StringHelp::compare(command, "KICK", true) == 0 && get_param_count() >= 2) return type_kick;
	else if (clan::StringHelp::compare(command, "QUIT", true) == 0) return type_quit;
	else if (clan::StringHelp::compare(command, "MODE", true) == 0 && get_param_count() >= 1)
	{
		if (IRCEntity::from_raw(get_param(0)).is_channel())
			return type_channel_mode;
		else
			return type_nick_mode;
	}
	else if (clan::StringHelp::compare(command, "TOPIC", true) == 0 && get_param_count() >= 2) return type_topic;
	else return type_unknown;
}

IRCEntity IRCMessage::get_prefix() const
{
	return IRCEntity::from_raw(prefix);
}

IRCRawString IRCMessage::get_command() const
{
	return command;
}

IRCRawString IRCMessage::get_param(size_t index) const
{
	return params[index];
}

size_t IRCMessage::get_param_count() const
{
	return params.size();
}

IRCRawString IRCMessage::create_line(const IRCRawString &prefix, const IRCRawString &command, const std::vector<IRCRawString> &params)
{
	IRCRawString line;
	if (!prefix.empty())
	{
		line.append(":");
		line.append(prefix);
		line.append(" ");
	}
	
	line.append(command);
	
	int size = params.size();
	for (int i=0; i<size;i++)
	{
		line.append(" ");
		if (i == size-1) line.append(":");
		line.append(params[i]);
	}
	if (size == 0)
		line.append(":");

	// Add low level escapes:
	IRCRawString::size_type pos;

	pos = 0;
	while (pos != IRCRawString::npos)
	{
		pos = line.find('\020', pos);
		if (pos == IRCRawString::npos) break;
		line.replace( pos, 1, "\020\020" );
		pos++;
	}

	pos = 0;
	while (pos != IRCRawString::npos)
	{
		pos = line.find('\0', pos);
		if (pos == IRCRawString::npos) break;
		line.replace( pos, 1, "\020""0" );
		pos++;
	}

	pos = 0;
	while (pos != IRCRawString::npos)
	{
		pos = line.find('\r', pos);
		if (pos == IRCRawString::npos) break;
		line.replace( pos, 1, "\020r" );
		pos++;
	}

	pos = 0;
	while (pos != IRCRawString::npos)
	{
		pos = line.find('\n', pos);
		if (pos == IRCRawString::npos) break;
		line.replace( pos, 1, "\020n" );
		pos++;
	}

	line.append("\r\n");
	return line;
}

IRCMessage IRCMessage::parse_line(IRCRawString line)
{
	// Parse line:
	IRCRawString prefix, command;
	std::vector<IRCRawString> args;
	line = line.substr(0, line.length()-2); // cut off CR-LF
	line.append(" "); // whitespace at end. Makes parsing easier.

	// Find low level message escapes:
	IRCRawString::size_type pos = 0;
	pos = 0;
	while (pos != IRCRawString::npos)
	{
		pos = line.find('\020', pos);
		if (pos == IRCRawString::npos || pos+1 == IRCRawString::npos) break;
		switch (line[pos+1])
		{
		case '\020':
			line.replace( pos, 2, 1, '\020' );
			break;

		case '0':
			line.replace( pos, 2, 1, '\0' );
			break;

		case 'n':
			line.replace( pos, 2, 1, '\n' );
			break;

		case 'r':
			line.replace( pos, 2, 1, '\r' );
			break;
		};
		pos++;
	}

	// Split line prefix and command:
	pos = 0;
	IRCRawString::size_type endpos;
	if (line[0] == ':') // prefix
	{
		pos = line.find(" ", 1)+1;
		prefix = line.substr(1, pos-2);
	}
	
	endpos = line.find(" ", pos);
	command = line.substr(pos, endpos-pos);
	pos = endpos+1;
	
	while (pos != line.length())
	{
		if (line[pos] == ':')
		{
			args.push_back(line.substr(pos+1, line.length()-pos-2));
			break;
		}
	
		endpos = line.find(" ", pos);
		args.push_back(line.substr(pos, endpos-pos));
		pos = endpos+1;
	}

	return IRCMessage(prefix, command, args);
}
