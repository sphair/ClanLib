
#pragma once

class XMPPEntity
{
public:
	XMPPEntity() { }
	XMPPEntity(const CL_String &name, const CL_String &jid) : name(name), jid(jid) { }

	bool empty() const { return jid.empty(); }
	CL_String get_jid_user() const;

	bool operator ==(const XMPPEntity &other) const;

	CL_String name;
	CL_String jid;
};

inline bool XMPPEntity::operator ==(const XMPPEntity &other) const
{
	return get_jid_user() == other.get_jid_user();
}

inline CL_String XMPPEntity::get_jid_user() const
{
	CL_String::size_type p = jid.find('/');
	return (p == CL_String::npos) ? jid : jid.substr(0, p);
}
