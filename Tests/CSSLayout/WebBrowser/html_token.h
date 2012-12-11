
#pragma once

class HTMLAttribute
{
public:
	std::string name;
	std::string value;
};

class HTMLToken
{
public:
	enum Type
	{
		type_null,
		type_dtd,
		type_tag_begin,
		type_tag_end,
		type_tag_single,
		type_text,
		type_script_tag,
		type_style_tag,
		type_comment,
		type_invalid
	};
	HTMLToken() : type(type_null) { }
	
	Type type;
	std::string name;
	std::string value;
	std::vector<HTMLAttribute> attributes;
};
