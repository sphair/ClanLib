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

/// \addtogroup clanRegExp_Regular_Expressions clanRegExp Regular Expressions
/// \{

#pragma once

#include "../Core/System/sharedptr.h"
#include "api_regexp.h"
#include "regexp_match.h"

class CL_RegExp_Impl;

/// \brief Regular Expression.
///
/// \xmlonly !group=RegExp/Regular Expressions! !header=regexp.h! \endxmlonly
class CL_API_REGEXP CL_RegExp
{
/// \name Construction
/// \{

public:

	/// \brief Constructs a RegExp
	///
	/// \param expression = char
	/// \param compile_flags = value
	/// \param study = bool
	CL_RegExp(const char *expression, int compile_flags = 0, bool study = false);

	/// \brief Constructs a RegExp
	///
	/// \param expression = String Ref8
	/// \param compile_flags = value
	/// \param study = bool
	CL_RegExp(const CL_StringRef8 &expression, int compile_flags = 0, bool study = false);

	~CL_RegExp();

/// \}
/// \name Attributes
/// \{

public:
	enum CompileFlags
	{
		compile_anchored         = 1,
		compile_auto_callout     = 2,
		compile_caseless         = 4,
		compile_dollar_endonly   = 8,
		compile_dot_all          = 16,
		compile_extended         = 32,
		compile_extra            = 64,
		compile_multi_line       = 128,
		compile_no_auto_capture  = 256,
		compile_ungreedy         = 512,
		compile_utf8             = 1024,
		compile_no_utf8_check    = 2048
	};

	enum SearchFlags
	{
		search_anchored      = 1,
		search_not_bol       = 2,
		search_not_eol       = 4,
		search_not_empty     = 8,
		search_no_utf8_check = 16,
		search_partial       = 32
	};

	/// \brief Get string number
	///
	/// \param name = char
	///
	/// \return int
	int get_string_number(const char *name) const;

	/// \brief Get string number
	///
	/// \param name = String Ref8
	///
	/// \return int
	int get_string_number(const CL_StringRef8 &name) const;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Match regular expression against a given subject string.
	CL_RegExpMatch search(
		const char *subject,
		int length,
		int start_offset = 0,
		int search_flags = 0) const;

	CL_RegExpMatch search(
		const CL_StringRef8 &subject,
		int start_offset = 0,
		int search_flags = 0) const;

	void search(
		const char *subject,
		int length,
		int start_offset,
		int search_flags,
		CL_RegExpMatch &result) const;

	void search(
		const CL_StringRef8 &subject,
		int start_offset,
		int search_flags,
		CL_RegExpMatch &result) const;

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_RegExp_Impl> impl;
/// \}
};

/// \}
