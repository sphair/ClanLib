/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

#include "../api_network.h"

/// \brief CL_NetGameEventValue
///
/// \xmlonly !group=Network/NetGame! !header=network.h! \endxmlonly
class CL_API_NETWORK CL_NetGameEventValue
{
public:
	enum Type
	{
		null,
		integer,
		uinteger,
		string,
		boolean,
		number,
		complex
	};

	CL_NetGameEventValue();

	/// \brief Constructs a NetGameEventValue
	///
	/// \param value = value
	CL_NetGameEventValue(int value);

	/// \brief Constructs a NetGameEventValue
	///
	/// \param value = value
	CL_NetGameEventValue(unsigned int value);

	/// \brief Constructs a NetGameEventValue
	///
	/// \param value = value
	CL_NetGameEventValue(float value);

	/// \brief Constructs a NetGameEventValue
	///
	/// \param value = String
	CL_NetGameEventValue(const CL_String &value);

	/// \brief Constructs a NetGameEventValue
	///
	/// \param value = String Ref
	CL_NetGameEventValue(const CL_StringRef &value);

	/// \brief Constructs a NetGameEventValue
	///
	/// \param str = char
	CL_NetGameEventValue(const char *str);

	/// \brief Constructs a NetGameEventValue
	///
	/// \param str = wchar_t
	CL_NetGameEventValue(const wchar_t *str);
	explicit CL_NetGameEventValue(bool value);

	/// \brief Constructs a NetGameEventValue
	///
	/// \param type = Type
	CL_NetGameEventValue(Type type);

	/// \brief Get Type
	///
	/// \return type
	Type get_type() const;

	/// \brief Is Null
	///
	/// \return true = null
	bool is_null() const;

	/// \brief Is Uinteger
	///
	/// \return true = uinteger
	bool is_uinteger() const;

	/// \brief Is Integer
	///
	/// \return true = integer
	bool is_integer() const;

	/// \brief Is Number
	///
	/// \return true = number
	bool is_number() const;

	/// \brief Is String
	///
	/// \return true = string
	bool is_string() const;

	/// \brief Is Boolean
	///
	/// \return true = boolean
	bool is_boolean() const;

	/// \brief Is Complex
	///
	/// \return true = complex
	bool is_complex() const;

	unsigned int get_member_count() const;
	const CL_NetGameEventValue &get_member(unsigned int index) const;

	/// \brief Add member
	///
	/// \param value = Net Game Event Value
	void add_member(const CL_NetGameEventValue &value);

	/// \brief Set member
	///
	/// \param index = value
	/// \param value = Net Game Event Value
	void set_member(unsigned int index, const CL_NetGameEventValue &value);

	unsigned int to_uinteger() const;

	/// \brief To integer
	///
	/// \return int
	int to_integer() const;

	/// \brief To number
	///
	/// \return float
	float to_number() const;

	/// \brief To string
	///
	/// \return String
	CL_String to_string() const;

	/// \brief To boolean
	///
	/// \return bool
	bool to_boolean() const;

	inline operator unsigned int() const { return to_uinteger(); }
	inline operator int() const { return to_integer(); }
	inline operator float() const { return to_number(); }
	inline operator CL_String() const { return to_string(); }
	inline operator bool() const { return to_boolean(); }

private:

	/// \brief Throw if not complex
	void throw_if_not_complex() const;

	Type type;
	union
	{
		int value_int;
		unsigned int value_uint;
		float value_float;
		bool value_bool;
	};
	CL_String value_string;
	std::vector<CL_NetGameEventValue> value_complex;
};

/// \}

