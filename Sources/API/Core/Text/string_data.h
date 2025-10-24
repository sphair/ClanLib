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
**    Harry Storbacka
*/

/// \addtogroup clanCore_Text clanCore Text
/// \{


#ifndef _cl_header_string_data_
#define _cl_header_string_data_

#if defined(_MSC_VER)
#pragma once
#endif

#include "../api_core.h"

/// \brief Holds size_type and npos for CL_StringData template.
///
/// This class is required to workaround compiler issues.
/// \xmlonly !group=Core/Text! !header=core.h! !hide! \endxmlonly
class CL_API_CORE CL_StringDataTypes
{
public:
	typedef unsigned int size_type;
	static const size_type npos; // = 0xffffffff;
};

/// \brief String data template.
///
/// <p>String data detached from any container functionality.</p>
/// \xmlonly !group=Core/Text! !header=core.h! \endxmlonly
template<typename CharType, typename ReferenceClass, typename StdString>
class CL_API_CORE CL_StringData : public CL_StringDataTypes
{
public:
	CL_StringData();
	CL_StringData(const CharType *ptr, size_type length);

	typedef CharType char_type;
	typedef CharType *iterator;
	typedef const CharType *const_iterator;

	iterator begin() { return (iterator) data_ptr; }
	iterator end() { return begin() + data_length; }
	const_iterator begin() const { return (const_iterator) data_ptr; }
	const_iterator end() const { return begin() + data_length; }

//	reverse_iterator rbegin();
//	reverse_iterator rend();
//	const_reverse_iterator rbegin() const;
//	const_reverse_iterator rend() const;

	const CharType &operator[](size_type n) const { return *(data_ptr + n); }
	CharType &operator[](size_type n) { return *(data_ptr + n); }

	const CharType *data() const { return data_ptr; }
	CharType *data() { return data_ptr; }

	operator StdString() const;
	operator ReferenceClass() const;

	size_type size() const { return data_length; }
	size_type length() const { return data_length; }
	bool empty() const { return data_length == 0; }

	size_type find(const CL_StringData &s, size_type pos = 0) const;
	size_type find(const CharType *s, size_type pos, size_type n) const;
	size_type find(const CharType *s, size_type pos = 0) const;
	size_type find(CharType c, size_type pos = 0) const;
	size_type rfind(const CL_StringData &s, size_type pos = npos) const;
	size_type rfind(const CharType *s, size_type pos, size_type n) const;
	size_type rfind(const CharType *s, size_type pos = npos) const;
	size_type rfind(CharType c, size_type pos = npos) const;
	size_type find_first_of(const CL_StringData &s, size_type pos = 0) const;
	size_type find_first_of(const CharType *s, size_type pos, size_type n) const;
	size_type find_first_of(const CharType *s, size_type pos = 0) const;
	size_type find_first_of(CharType c, size_type pos = 0) const;
	size_type find_first_not_of(const CL_StringData &s, size_type pos = 0) const;
	size_type find_first_not_of(const CharType *s, size_type pos, size_type n) const;
	size_type find_first_not_of(const CharType *s, size_type pos = 0) const;
	size_type find_first_not_of(CharType c, size_type pos = 0) const;
	size_type find_last_of(const CL_StringData &s, size_type pos = npos) const;
	size_type find_last_of(const CharType *s, size_type pos, size_type n) const;
	size_type find_last_of(const CharType *s, size_type pos = npos) const;
	size_type find_last_of(CharType c, size_type pos = npos) const;
	size_type find_last_not_of(const CL_StringData &s, size_type pos = npos) const;
	size_type find_last_not_of(const CharType *s, size_type pos, size_type n) const;
	size_type find_last_not_of(const CharType *s, size_type pos = npos) const;
	size_type find_last_not_of(CharType c, size_type pos = npos) const;

	ReferenceClass substr(size_type pos = 0, size_type n = npos) const;

	int compare(const CL_StringData &s) const;
	int compare(size_type pos, size_type n, const CL_StringData &s) const;
	int compare(size_type pos, size_type n, const CL_StringData &s, size_type pos1, size_type n1) const;
	int compare(const CharType *s) const;
	int compare(size_type pos, size_type n, const CharType *s, size_type len = npos) const;

protected:
	mutable CharType *data_ptr;
	mutable size_type data_length;
};

template <typename CharType, typename ReferenceClass, typename StdString>
CL_API_CORE bool operator==(const CL_StringData<CharType, ReferenceClass, StdString> &s1, const CL_StringData<CharType, ReferenceClass, StdString> &s2);

template <typename CharType, typename ReferenceClass, typename StdString>
CL_API_CORE bool operator==(const CharType *s1, const CL_StringData<CharType, ReferenceClass, StdString> &s2);

template <typename CharType, typename ReferenceClass, typename StdString>
CL_API_CORE bool operator==(const CL_StringData<CharType, ReferenceClass, StdString> &s1, const CharType *s2);

template <typename CharType, typename ReferenceClass, typename StdString>
CL_API_CORE bool operator!=(const CL_StringData<CharType, ReferenceClass, StdString> &s1, const CL_StringData<CharType, ReferenceClass, StdString> &s2);

template <typename CharType, typename ReferenceClass, typename StdString>
CL_API_CORE bool operator!=(const CharType *s1, const CL_StringData<CharType, ReferenceClass, StdString> &s2);

template <typename CharType, typename ReferenceClass, typename StdString>
CL_API_CORE bool operator!=(const CL_StringData<CharType, ReferenceClass, StdString> &s1, const CharType *s2);

template <typename CharType, typename ReferenceClass, typename StdString>
CL_API_CORE bool operator<(const CL_StringData<CharType, ReferenceClass, StdString> &s1, const CL_StringData<CharType, ReferenceClass, StdString> &s2);

template <typename CharType, typename ReferenceClass, typename StdString>
CL_API_CORE bool operator<(const CharType *s1, const CL_StringData<CharType, ReferenceClass, StdString> &s2);

template <typename CharType, typename ReferenceClass, typename StdString>
CL_API_CORE bool operator<(const CL_StringData<CharType, ReferenceClass, StdString> &s1, const CharType *s2);

template <typename CharType, typename ReferenceClass, typename StdString>
CL_API_CORE bool operator>(const CL_StringData<CharType, ReferenceClass, StdString> &s1, const CL_StringData<CharType, ReferenceClass, StdString> &s2);

template <typename CharType, typename ReferenceClass, typename StdString>
CL_API_CORE bool operator>(const CharType *s1, const CL_StringData<CharType, ReferenceClass, StdString> &s2);

template <typename CharType, typename ReferenceClass, typename StdString>
CL_API_CORE bool operator>(const CL_StringData<CharType, ReferenceClass, StdString> &s1, const CharType *s2);

#endif
/// \}
