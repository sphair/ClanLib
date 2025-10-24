/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

#pragma once

#include "../api_core.h"
#include <string>

class CL_StringRef16;

/// \brief String data class for 16 bit data
///
/// \xmlonly !group=Core/Text! !header=core.h! \endxmlonly
class CL_API_CORE CL_StringData16
{
public:
	typedef unsigned int size_type;
	static const size_type npos; // = 0xffffffff;
	typedef wchar_t char_type;
	typedef wchar_t *iterator;
	typedef const wchar_t *const_iterator;

	CL_StringData16();

	/// \brief Constructs a StringData8
	///
	/// \param ptr = Char Type
	/// \param length = size_type
	CL_StringData16(const wchar_t *ptr, size_type length);

	/// \brief Begin
	///
	/// \return iterator
	iterator begin() { return (iterator) data_ptr; }

	/// \brief End
	///
	/// \return iterator
	iterator end() { return begin() + data_length; }

	/// \brief Begin
	///
	/// \return const_iterator
	const_iterator begin() const { return (const_iterator) data_ptr; }

	/// \brief End
	///
	/// \return const_iterator
	const_iterator end() const { return begin() + data_length; }

//	reverse_iterator rbegin();
//	reverse_iterator rend();
//	const_reverse_iterator rbegin() const;
//	const_reverse_iterator rend() const;

	const wchar_t &operator[](size_type n) const { return *(data_ptr + n); }
	wchar_t &operator[](size_type n) { return *(data_ptr + n); }

	const wchar_t *data() const { return data_ptr; }

	/// \brief Data
	///
	/// \return Char Type
	wchar_t *data() { return data_ptr; }

	/// \brief std::wstring
	///
	/// \return operator
	operator std::wstring() const;

	/// \brief Convert object to a string reference
	///
	/// \return operator
	operator CL_StringRef16() const;

	/// \brief Size
	///
	/// \return size_type
	size_type size() const { return data_length; }

	/// \brief Length
	///
	/// \return size_type
	size_type length() const { return data_length; }

	/// \brief Empty
	///
	/// \return bool
	bool empty() const { return data_length == 0; }

	/// \brief Find
	///
	/// \param s = String Data
	/// \param pos = size_type
	///
	/// \return size_type
	size_type find(const CL_StringData16 &s, size_type pos = 0) const;

	/// \brief Find
	///
	/// \param s = Char Type
	/// \param pos = size_type
	/// \param n = size_type
	///
	/// \return size_type
	size_type find(const wchar_t *s, size_type pos, size_type n) const;

	/// \brief Find
	///
	/// \param s = Char Type
	/// \param pos = size_type
	///
	/// \return size_type
	size_type find(const wchar_t *s, size_type pos = 0) const;

	/// \brief Find
	///
	/// \param c = Char Type
	/// \param pos = size_type
	///
	/// \return size_type
	size_type find(wchar_t c, size_type pos = 0) const;

	/// \brief Rfind
	///
	/// \param s = String Data
	/// \param pos = size_type
	///
	/// \return size_type
	size_type rfind(const CL_StringData16 &s, size_type pos = npos) const;

	/// \brief Rfind
	///
	/// \param s = Char Type
	/// \param pos = size_type
	/// \param n = size_type
	///
	/// \return size_type
	size_type rfind(const wchar_t *s, size_type pos, size_type n) const;

	/// \brief Rfind
	///
	/// \param s = Char Type
	/// \param pos = size_type
	///
	/// \return size_type
	size_type rfind(const wchar_t *s, size_type pos = npos) const;

	/// \brief Rfind
	///
	/// \param c = Char Type
	/// \param pos = size_type
	///
	/// \return size_type
	size_type rfind(wchar_t c, size_type pos = npos) const;

	/// \brief Find first of
	///
	/// \param s = String Data
	/// \param pos = size_type
	///
	/// \return size_type
	size_type find_first_of(const CL_StringData16 &s, size_type pos = 0) const;

	/// \brief Find first of
	///
	/// \param s = Char Type
	/// \param pos = size_type
	/// \param n = size_type
	///
	/// \return size_type
	size_type find_first_of(const wchar_t *s, size_type pos, size_type n) const;

	/// \brief Find first of
	///
	/// \param s = Char Type
	/// \param pos = size_type
	///
	/// \return size_type
	size_type find_first_of(const wchar_t *s, size_type pos = 0) const;

	/// \brief Find first of
	///
	/// \param c = Char Type
	/// \param pos = size_type
	///
	/// \return size_type
	size_type find_first_of(wchar_t c, size_type pos = 0) const;

	/// \brief Find first not of
	///
	/// \param s = String Data
	/// \param pos = size_type
	///
	/// \return size_type
	size_type find_first_not_of(const CL_StringData16 &s, size_type pos = 0) const;

	/// \brief Find first not of
	///
	/// \param s = Char Type
	/// \param pos = size_type
	/// \param n = size_type
	///
	/// \return size_type
	size_type find_first_not_of(const wchar_t *s, size_type pos, size_type n) const;

	/// \brief Find first not of
	///
	/// \param s = Char Type
	/// \param pos = size_type
	///
	/// \return size_type
	size_type find_first_not_of(const wchar_t *s, size_type pos = 0) const;

	/// \brief Find first not of
	///
	/// \param c = Char Type
	/// \param pos = size_type
	///
	/// \return size_type
	size_type find_first_not_of(wchar_t c, size_type pos = 0) const;

	/// \brief Find last of
	///
	/// \param s = String Data
	/// \param pos = size_type
	///
	/// \return size_type
	size_type find_last_of(const CL_StringData16 &s, size_type pos = npos) const;

	/// \brief Find last of
	///
	/// \param s = Char Type
	/// \param pos = size_type
	/// \param n = size_type
	///
	/// \return size_type
	size_type find_last_of(const wchar_t *s, size_type pos, size_type n) const;

	/// \brief Find last of
	///
	/// \param s = Char Type
	/// \param pos = size_type
	///
	/// \return size_type
	size_type find_last_of(const wchar_t *s, size_type pos = npos) const;

	/// \brief Find last of
	///
	/// \param c = Char Type
	/// \param pos = size_type
	///
	/// \return size_type
	size_type find_last_of(wchar_t c, size_type pos = npos) const;

	/// \brief Find last not of
	///
	/// \param s = String Data
	/// \param pos = size_type
	///
	/// \return size_type
	size_type find_last_not_of(const CL_StringData16 &s, size_type pos = npos) const;

	/// \brief Find last not of
	///
	/// \param s = Char Type
	/// \param pos = size_type
	/// \param n = size_type
	///
	/// \return size_type
	size_type find_last_not_of(const wchar_t *s, size_type pos, size_type n) const;

	/// \brief Find last not of
	///
	/// \param s = Char Type
	/// \param pos = size_type
	///
	/// \return size_type
	size_type find_last_not_of(const wchar_t *s, size_type pos = npos) const;

	/// \brief Find last not of
	///
	/// \param c = Char Type
	/// \param pos = size_type
	///
	/// \return size_type
	size_type find_last_not_of(wchar_t c, size_type pos = npos) const;

	/// \brief Substr
	///
	/// \param pos = size_type
	/// \param n = size_type
	///
	/// \return Reference Class
	CL_StringRef16 substr(size_type pos = 0, size_type n = npos) const;

	/// \brief Compare
	///
	/// \param s = String Data
	///
	/// \return int
	int compare(const CL_StringData16 &s) const;

	/// \brief Compare
	///
	/// \param pos = size_type
	/// \param n = size_type
	/// \param s = String Data
	///
	/// \return int
	int compare(size_type pos, size_type n, const CL_StringData16 &s) const;

	/// \brief Compare
	///
	/// \param pos = size_type
	/// \param n = size_type
	/// \param s = String Data
	/// \param pos1 = size_type
	/// \param n1 = size_type
	///
	/// \return int
	int compare(size_type pos, size_type n, const CL_StringData16 &s, size_type pos1, size_type n1) const;

	/// \brief Compare
	///
	/// \param s = Char Type
	///
	/// \return int
	int compare(const wchar_t *s) const;

	/// \brief Compare
	///
	/// \param pos = size_type
	/// \param n = size_type
	/// \param s = Char Type
	/// \param len = size_type
	///
	/// \return int
	int compare(size_type pos, size_type n, const wchar_t *s, size_type len = npos) const;

protected:
	mutable wchar_t *data_ptr;
	mutable size_type data_length;
};

CL_API_CORE bool operator==(const CL_StringData16 &s1, const CL_StringData16 &s2);
CL_API_CORE bool operator==(const wchar_t *s1, const CL_StringData16 &s2);
CL_API_CORE bool operator==(const CL_StringData16 &s1, const wchar_t *s2);
CL_API_CORE bool operator!=(const CL_StringData16 &s1, const CL_StringData16 &s2);
CL_API_CORE bool operator!=(const wchar_t *s1, const CL_StringData16 &s2);
CL_API_CORE bool operator!=(const CL_StringData16 &s1, const wchar_t *s2);
CL_API_CORE bool operator<(const CL_StringData16 &s1, const CL_StringData16 &s2);
CL_API_CORE bool operator<(const wchar_t *s1, const CL_StringData16 &s2);
CL_API_CORE bool operator<(const CL_StringData16 &s1, const wchar_t *s2);
CL_API_CORE bool operator>(const CL_StringData16 &s1, const CL_StringData16 &s2);
CL_API_CORE bool operator>(const wchar_t *s1, const CL_StringData16 &s2);
CL_API_CORE bool operator>(const CL_StringData16 &s1, const wchar_t *s2);

/// \}
