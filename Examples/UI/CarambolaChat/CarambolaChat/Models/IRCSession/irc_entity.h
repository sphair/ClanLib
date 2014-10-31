
#pragma once

#include "irc_raw_string.h"

class IRCEntity
{
public:
	IRCEntity();

	IRCRawString to_raw() const { return get_label(); }
	static IRCEntity from_text(const std::string &name);
	static IRCEntity from_raw(const IRCRawString &raw_label);

	bool operator ==(const IRCEntity &other) const;

	std::string get_name() const;
	IRCRawString get_label() const;

	bool is_channel() const;
	bool is_operator() const;
	bool is_voiced() const;

	bool empty() const;

private:
	IRCRawString label;
};

typedef IRCEntity IRCNick;
typedef IRCEntity IRCChannel;
typedef IRCEntity IRCServer;
