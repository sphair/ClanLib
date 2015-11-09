/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
*/

#include "Network/precomp.h"
#include "API/Network/NetGame/event_value.h"
#include "API/Core/Text/string_help.h"

namespace clan
{
	NetGameEventValue::NetGameEventValue()
		: type(null), value_int(0)
	{
	}

	NetGameEventValue::NetGameEventValue(int value)
		: type(integer), value_int(value)
	{
	}

	NetGameEventValue::NetGameEventValue(unsigned int value)
		: type(uinteger), value_uint(value)
	{
	}

	NetGameEventValue::NetGameEventValue(char value)
		: type(character), value_char(value)
	{
	}

	NetGameEventValue::NetGameEventValue(unsigned char value)
		: type(ucharacter), value_uchar(value)
	{
	}

	NetGameEventValue::NetGameEventValue(float value)
		: type(number), value_float(value)
	{
	}

	NetGameEventValue::NetGameEventValue(const std::string &value)
		: type(string), value_string(value)
	{
	}

	NetGameEventValue::NetGameEventValue(const char *value)
		: type(string), value_string(value)
	{
	}

	NetGameEventValue::NetGameEventValue(const wchar_t *value)
		: type(string)
	{
		value_string = StringHelp::ucs2_to_utf8(value);
	}

	NetGameEventValue::NetGameEventValue(bool value)
		: type(boolean), value_bool(value)
	{
	}

	NetGameEventValue::NetGameEventValue(const DataBuffer &value)
		: type(binary), value_binary(value)
	{
	}

	NetGameEventValue::NetGameEventValue(Type type)
		: type(type), value_int(0)
	{
	}

	NetGameEventValue::Type NetGameEventValue::get_type() const
	{
		return type;
	}

	bool NetGameEventValue::is_null() const
	{
		return type == null;
	}

	bool NetGameEventValue::is_uinteger() const
	{
		return type == uinteger;
	}

	bool NetGameEventValue::is_integer() const
	{
		return type == integer;
	}

	bool NetGameEventValue::is_ucharacter() const
	{
		return type == ucharacter;
	}

	bool NetGameEventValue::is_character() const
	{
		return type == character;
	}

	bool NetGameEventValue::is_number() const
	{
		return type == number;
	}

	bool NetGameEventValue::is_string() const
	{
		return type == string;
	}

	bool NetGameEventValue::is_boolean() const
	{
		return type == boolean;
	}

	bool NetGameEventValue::is_binary() const
	{
		return type == binary;
	}

	bool NetGameEventValue::is_complex() const
	{
		return type == complex;
	}

	unsigned int NetGameEventValue::get_member_count() const
	{
		throw_if_not_complex();
		return value_complex.size();
	}

	const NetGameEventValue &NetGameEventValue::get_member(unsigned int index) const
	{
		throw_if_not_complex();
		return value_complex.at(index);
	}

	void NetGameEventValue::add_member(const NetGameEventValue &value)
	{
		throw_if_not_complex();
		value_complex.push_back(value);
	}

	void NetGameEventValue::set_member(unsigned int index, const NetGameEventValue &value)
	{
		throw_if_not_complex();
		value_complex.at(index) = value;
	}

	void NetGameEventValue::throw_if_not_complex() const
	{
		if (type != complex)
			throw Exception("NetGameEventValue is not a complex type");
	}

	unsigned int NetGameEventValue::get_uinteger() const
	{
		if (is_uinteger())
			return value_uint;
		else
			throw Exception("NetGameEventValue is not an unsigned integer");
	}

	int NetGameEventValue::get_integer() const
	{
		if (is_integer())
			return value_int;
		else
			throw Exception("NetGameEventValue is not an integer");
	}

	unsigned int NetGameEventValue::get_ucharacter() const
	{
		if (is_ucharacter())
			return value_uchar;
		else
			throw Exception("NetGameEventValue is not an unsigned character");
	}

	int NetGameEventValue::get_character() const
	{
		if (is_character())
			return value_char;
		else
			throw Exception("NetGameEventValue is not a character");
	}

	float NetGameEventValue::get_number() const
	{
		if (is_number())
			return value_float;
		else
			throw Exception("NetGameEventValue is not a floating point number");
	}

	std::string NetGameEventValue::get_string() const
	{
		if (is_string())
			return value_string;
		else
			throw Exception("NetGameEventValue is not a string");
	}

	bool NetGameEventValue::get_boolean() const
	{
		if (is_boolean())
			return value_bool;
		else
			throw Exception("NetGameEventValue is not a boolean");
	}

	DataBuffer NetGameEventValue::get_binary() const
	{
		if (is_binary())
			return value_binary;
		else
			throw Exception("NetGameEventValue is not a binary");
	}

	std::string NetGameEventValue::to_string(const NetGameEventValue &v)
	{
		switch (v.get_type())
		{
		case NetGameEventValue::null:
			return "null";
		case NetGameEventValue::integer:
			return StringHelp::int_to_text(v.get_integer());
		case NetGameEventValue::uinteger:
			return StringHelp::uint_to_text(v.get_uinteger());
		case NetGameEventValue::character:
			return StringHelp::int_to_text(static_cast<int>(v.get_character()));
		case NetGameEventValue::ucharacter:
			return StringHelp::uint_to_text(static_cast<unsigned int>(v.get_ucharacter()));
		case NetGameEventValue::string:
			return "\"" + v.get_string() + "\"";
		case NetGameEventValue::boolean:
			return v.get_boolean() ? "true" : "false";
		case NetGameEventValue::number:
			return StringHelp::float_to_text(v.get_number());
		case NetGameEventValue::complex:
		{
			std::string str;
			str += "[";
			for (unsigned int j = 0; j < v.get_member_count(); j++)
			{
				if (j > 0)
					str += ",";
				str += to_string(v.get_member(j));
			}
			str += "]";
			return str;
		}
		default:
			return "??" + StringHelp::int_to_text(v.get_type());
		}
	}
}
