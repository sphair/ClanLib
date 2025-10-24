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

/// \addtogroup clanCore_Text clanCore Text
/// \{


#ifndef _cl_header_string_reference_
#define _cl_header_string_reference_

#if defined(_MSC_VER)
#pragma once
#endif

#include "../api_core.h"
#include "string_data.h"

/// \brief String reference template.
///
/// Reference to string data.  Extends the basic CL_StringData template with container functionality required when converting to or from C strings.
/// \xmlonly !group=Core/Text! !header=core.h! \endxmlonly
template<typename CharType, typename StdString>
class CL_API_CORE CL_StringReference : public CL_StringData<CharType, CL_StringReference<CharType, StdString>, StdString>
{
public:

#ifndef WIN32
	// Fix for GCC bug for not finding CL_StringDataTypes::size_type and so on .... grrrrrrr!
	// TODO: Remove this fix when GCC is a good compiler. As from 13 December 2007, SVN GCC does not work
	// See: http:/** gcc.gnu.org/bugzilla/show_bug.cgi?id=34453*/

	typedef unsigned int size_type;
	typedef CharType char_type;
	typedef CharType *iterator;
	typedef const CharType *const_iterator;

	// These could and maybe should be replaced with "this->function_name()"
	iterator begin() { return CL_StringData<CharType, CL_StringReference<CharType, StdString>, StdString>::begin(); }
	iterator end() { return CL_StringData<CharType, CL_StringReference<CharType, StdString>, StdString>::end(); }
	size_type size() const { return CL_StringData<CharType, CL_StringReference<CharType, StdString>, StdString>::size(); }
	size_type length() const { return CL_StringData<CharType, CL_StringReference<CharType, StdString>, StdString>::length(); }
	bool empty() const { return CL_StringData<CharType, CL_StringReference<CharType, StdString>, StdString>::empty(); }
	const CharType *data() const { return CL_StringData<CharType, CL_StringReference<CharType, StdString>, StdString>::data(); }
	CharType *data() { return CL_StringData<CharType, CL_StringReference<CharType, StdString>, StdString>::data(); }

#endif

	CL_StringReference();
	CL_StringReference(const StdString &source);
	CL_StringReference(const CL_StringReference &source);
	CL_StringReference(const CL_StringData<CharType, CL_StringReference<CharType, StdString>, StdString> &source);
	CL_StringReference(const char *c_str);
	CL_StringReference(const char *c_str, size_type length, bool null_terminated);
	CL_StringReference(const wchar_t *wc_str);
	CL_StringReference(const wchar_t *wc_str, size_type length, bool null_terminated);
	~CL_StringReference();

	const CharType *c_str() const;
	void set_length(size_type length);

	CL_StringReference<CharType, StdString> &operator =(const CL_StringReference<CharType, StdString> &source);
	CL_StringReference<CharType, StdString> &operator =(const CL_StringData<CharType, CL_StringReference<CharType, StdString>, StdString> &source);
	CL_StringReference<CharType, StdString> &operator =(const char *c_str);
	CL_StringReference<CharType, StdString> &operator =(const wchar_t *c_str);

protected:
	void clear() const;
	void create_temp(const CharType *data, size_type length) const;

	mutable bool null_terminated;
	mutable bool temporary;
};

#endif
/// \}
