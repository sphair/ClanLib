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


#ifndef _cl_header_temp_string_container_
#define _cl_header_temp_string_container_

#if defined(_MSC_VER)
#pragma once
#endif

#include "../api_core.h"
#include "string_container.h"

/// \brief Temporary string container template.
///
/// Special version of the basic string container which uses a fast temporary memory pool for allocations.
/// \xmlonly !group=Core/Text! !header=core.h! \endxmlonly
template<typename CharType, typename ReferenceClass, typename StdString>
class CL_API_CORE CL_TempStringContainer : public CL_StringContainer<CharType, ReferenceClass, StdString>
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
	iterator begin() { return CL_StringContainer<CharType, ReferenceClass, StdString>::begin(); }
	iterator end() { return CL_StringContainer<CharType, ReferenceClass, StdString>::end(); }
	size_type size() const { return CL_StringContainer<CharType, ReferenceClass, StdString>::size(); }
	size_type length() const { return CL_StringContainer<CharType, ReferenceClass, StdString>::length(); }
	bool empty() const { return CL_StringContainer<CharType, ReferenceClass, StdString>::empty(); }
	const CharType *data() const { return CL_StringContainer<CharType, ReferenceClass, StdString>::data(); }
	CharType *data() { return CL_StringContainer<CharType, ReferenceClass, StdString>::data(); }

#endif

	CL_TempStringContainer();
	CL_TempStringContainer(const CL_TempStringContainer &source);
	CL_TempStringContainer(const CL_StringData<CharType, ReferenceClass, StdString> &source);
	CL_TempStringContainer(const char *c_str);
	CL_TempStringContainer(const char *c_str, size_type length);
	CL_TempStringContainer(const wchar_t *wc_str);
	CL_TempStringContainer(const wchar_t *wc_str, size_type length);
	CL_TempStringContainer(size_type n, CharType c);

	operator ReferenceClass() const;
};

template<typename CharType, typename ReferenceClass, typename StdString>
CL_API_CORE CL_TempStringContainer<CharType, ReferenceClass, StdString> operator+(const CL_StringData<CharType, ReferenceClass, StdString> &s1, const CL_StringData<CharType, ReferenceClass, StdString> &s2);

template<typename CharType, typename ReferenceClass, typename StdString>
CL_API_CORE CL_TempStringContainer<CharType, ReferenceClass, StdString> operator+(const char *s1, const CL_StringData<CharType, ReferenceClass, StdString> &s2);

template<typename CharType, typename ReferenceClass, typename StdString>
CL_API_CORE CL_TempStringContainer<CharType, ReferenceClass, StdString> operator+(const wchar_t *s1, const CL_StringData<CharType, ReferenceClass, StdString> &s2);

template<typename CharType, typename ReferenceClass, typename StdString>
CL_API_CORE CL_TempStringContainer<CharType, ReferenceClass, StdString> operator+(const CL_StringData<CharType, ReferenceClass, StdString> &s1, const char *s2);

template<typename CharType, typename ReferenceClass, typename StdString>
CL_API_CORE CL_TempStringContainer<CharType, ReferenceClass, StdString> operator+(const CL_StringData<CharType, ReferenceClass, StdString> &s1, const wchar_t *s2);

template<typename CharType, typename ReferenceClass, typename StdString>
CL_API_CORE CL_TempStringContainer<CharType, ReferenceClass, StdString> operator+(CharType c, const CL_StringData<CharType, ReferenceClass, StdString> &s2);

template<typename CharType, typename ReferenceClass, typename StdString>
CL_API_CORE CL_TempStringContainer<CharType, ReferenceClass, StdString> operator+(const CL_StringData<CharType, ReferenceClass, StdString> &s1, CharType c);

#endif
/// \}
