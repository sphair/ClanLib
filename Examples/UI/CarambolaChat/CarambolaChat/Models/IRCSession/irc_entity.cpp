
#include "precomp.h"
#include "irc_entity.h"

IRCEntity::IRCEntity()
{
}

IRCEntity IRCEntity::from_text(const std::string &name)
{
	IRCEntity entity;
	entity.label = clan::StringHelp::text_to_utf8(name);
	return entity;
}

IRCEntity IRCEntity::from_raw(const IRCRawString &raw_label)
{
	IRCEntity entity;
	entity.label = raw_label;
	return entity;
}

bool IRCEntity::operator ==(const IRCEntity &other) const
{
	if (is_channel() != other.is_channel())
		return false;
	else
		return clan::StringHelp::compare(get_name(), other.get_name(), true) == 0;
}

std::string IRCEntity::get_name() const
{
	IRCRawString::size_type pos = label.find('!');
	std::string labelshort;
	if (pos == IRCRawString::npos)
		labelshort = label;
	else
		labelshort = label.substr(0, pos);

	if (!labelshort.empty())
	{
		switch (labelshort[0])
		{
		case '#':
		case '@':
		case '+':
			return clan::StringHelp::utf8_to_text(labelshort.substr(1));
		default:
			return clan::StringHelp::utf8_to_text(labelshort);
		}
	}
	else
	{
		return clan::StringHelp::utf8_to_text(labelshort);
	}
}

IRCRawString IRCEntity::get_label() const
{
	IRCRawString::size_type pos = label.find('!');
	std::string labelshort;
	if (pos == IRCRawString::npos)
		labelshort = label;
	else
		labelshort = label.substr(0, pos);
	return labelshort;
}

bool IRCEntity::is_channel() const
{
	return !label.empty() && label[0] == '#';
}

bool IRCEntity::is_operator() const
{
	return !label.empty() && label[0] == '@';
}

bool IRCEntity::is_voiced() const
{
	return !label.empty() && label[0] == '+';
}

bool IRCEntity::empty() const
{
	return label.empty();
}
