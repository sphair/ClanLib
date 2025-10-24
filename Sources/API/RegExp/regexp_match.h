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

/// \addtogroup clanRegExp_Regular_Expressions clanRegExp Regular Expressions
/// \{


#pragma once


#include "../Core/System/sharedptr.h"
#include "api_regexp.h"

class CL_RegExpMatch_Impl;

/// \brief Regular Expression match.
///
/// \xmlonly !group=RegExp/Regular Expressions! !header=regexp.h! \endxmlonly
class CL_API_REGEXP CL_RegExpMatch
{
/// \name Construction
/// \{

public:
	CL_RegExpMatch();

	CL_RegExpMatch(const CL_RegExpMatch &other);

	~CL_RegExpMatch();


/// \}
/// \name Attributes
/// \{

public:
	const int *get_vector() const;

	int *get_vector();

	int get_vector_size() const;

	CL_StringRef8 get_capture(const char *subject, int capture) const;

	CL_StringRef8 get_capture(const CL_StringRef8 &subject, int capture) const;

	int get_capture_pos(int capture) const;

	int get_capture_length(int capture) const;

	int get_capture_end(int capture) const;

	int get_captures_count() const;

	bool is_partial() const;

	bool is_match() const;


/// \}
/// \name Operations
/// \{

public:
	CL_RegExpMatch &operator =(const CL_RegExpMatch &other);

	void set_vector_size(int size);

	void set_captures_count(int count);

	void set_partial_match(bool partial);


/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_RegExpMatch_Impl> impl;
/// \}
};


/// \}
