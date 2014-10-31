
#pragma once

class XMPPAttribute
{
public:
	XMPPAttribute(const CL_String &name, const CL_String &value) : name(name), value(value) { }
	CL_String name;
	CL_String value;
};

class XMPPToken
{
public:
	XMPPToken() : type(type_null) { }

	enum Type
	{
		type_null,
		type_begin,
		type_end,
		type_single,
		type_text
	};
	Type type;
	CL_String name;
	std::vector<XMPPAttribute> attributes;
	CL_String value;
};
