/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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

/// \addtogroup clanNetwork_NetGame clanNetwork NetGame
/// \{

#pragma once

/// \brief CL_NetGameEventValue
///
/// \xmlonly !group=Network/NetGame! !header=network.h! \endxmlonly
class CL_NetGameEventValue
{
public:
	enum Type
	{
		null,
		integer,
		string,
		boolean,
		number
	};

	CL_NetGameEventValue();
	CL_NetGameEventValue(int value);
	CL_NetGameEventValue(float value);
	CL_NetGameEventValue(const CL_String &value);
	CL_NetGameEventValue(const CL_StringRef &value);
	CL_NetGameEventValue(const char *str);
	CL_NetGameEventValue(const wchar_t *str);
	explicit CL_NetGameEventValue(bool value);
	CL_NetGameEventValue(Type type, const CL_String &value);

	Type get_type() const;
	const CL_String &get_value() const { return value; }

	bool is_null() const;
	bool is_integer() const;
	bool is_number() const;
	bool is_string() const;
	bool is_boolean() const;

	int to_integer() const;
	float to_number() const;
	CL_String to_string() const;
	bool to_boolean() const;

	inline operator int() const { return to_integer(); }
	inline operator float() const { return to_number(); }
	inline operator CL_String() const { return to_string(); }
	inline operator bool() const { return to_boolean(); }

private:
	Type type;
	CL_String value;
};

/// \}

