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

/// \addtogroup clanCore_Text clanCore Text
/// \{

#pragma once

#include "../api_core.h"
#include "string_data8.h"

/// \brief String reference template.
///
/// Reference to string data.  Extends the basic CL_StringData8 with container functionality required when converting to or from C strings.
/// \xmlonly !group=Core/Text! !header=core.h! \endxmlonly
class CL_API_CORE CL_StringRef8 : public CL_StringData8
{
public:
	CL_StringRef8();

	/// \brief Constructs a StringReference
	///
	/// \param source = Std String
	CL_StringRef8(const std::string &source);

	/// \brief Constructs a StringReference
	///
	/// \param source = String Reference
	CL_StringRef8(const CL_StringRef8 &source);

	/// \brief Constructs a StringReference
	CL_StringRef8(const CL_StringData8 &source);

	/// \brief Constructs a StringReference
	///
	/// \param c_str = char
	CL_StringRef8(const char *c_str);

	/// \brief Constructs a StringReference
	///
	/// \param c_str = char
	/// \param length = size_type
	/// \param null_terminated = bool
	CL_StringRef8(const char *c_str, size_type length, bool null_terminated);

	/// \brief Constructs a StringReference
	///
	/// \param wc_str = wchar_t
	CL_StringRef8(const wchar_t *wc_str);

	/// \brief Constructs a StringReference
	///
	/// \param wc_str = wchar_t
	/// \param length = size_type
	/// \param null_terminated = bool
	CL_StringRef8(const wchar_t *wc_str, size_type length, bool null_terminated);
	~CL_StringRef8();

	const char *c_str() const;

	/// \brief Set length
	///
	/// \param length = size_type
	void set_length(size_type length);

	CL_StringRef8 &operator =(const CL_StringRef8 &source);
	CL_StringRef8 &operator =(const CL_StringData8 &source);
	CL_StringRef8 &operator =(const char *c_str);
	CL_StringRef8 &operator =(const wchar_t *c_str);

protected:
	/// \brief Clear
	void clear() const;

	/// \brief Create temp
	///
	/// \param data = Char Type
	/// \param length = size_type
	void create_temp(const char *data, size_type length) const;

	mutable bool null_terminated;
	mutable bool temporary;
};

/// \}
