
#pragma once

enum XMPPPresenceShowType
{
	show_offline,
	show_away,
	show_chat,
	show_dnd,
	show_extended_away
};

class XMPPRosterItem
{
public:
	XMPPRosterItem(const CL_String &jid, const CL_String &subscription_type, const CL_String &name);

	CL_String jid;
	CL_String subscription_type;
	CL_String name;
	XMPPPresenceShowType show_type;
	CL_String show_text;
};

inline XMPPRosterItem::XMPPRosterItem(const CL_String &jid, const CL_String &subscription_type, const CL_String &name)
: jid(jid), subscription_type(subscription_type), name(name), show_type(show_offline)
{
}
