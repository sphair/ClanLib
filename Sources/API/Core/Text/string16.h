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
**    Harry Storbacka
*/

/// \addtogroup clanCore_Text clanCore Text
/// \{

#pragma once

#include "../api_core.h"
#include "string_data16.h"

class CL_MemoryPool;

/// \brief String container template.
///
///  Basic string container.  Extends StringData with operations that work on string data.
/// \xmlonly !group=Core/Text! !header=core.h! \endxmlonly
class CL_API_CORE CL_String16 : public CL_StringData16
{
public:
	CL_String16();

	/// \brief Constructs a StringContainer
	///
	/// \param source = Std String
	CL_String16(const std::wstring &source);

	/// \brief Constructs a StringContainer
	///
	/// \param source = String Container
	CL_String16(const CL_String16 &source);

	/// \brief Constructs a StringContainer
	CL_String16(const CL_StringData16 &source);

	/// \brief Constructs a StringContainer
	///
	/// \param c_str = char
	CL_String16(const char *c_str);

	/// \brief Constructs a StringContainer
	///
	/// \param c_str = char
	/// \param length = size_type
	CL_String16(const char *c_str, size_type length);

	/// \brief Constructs a StringContainer
	///
	/// \param wc_str = wchar_t
	CL_String16(const wchar_t *wc_str);

	/// \brief Constructs a StringContainer
	///
	/// \param wc_str = wchar_t
	/// \param length = size_type
	CL_String16(const wchar_t *wc_str, size_type length);

	/// \brief Constructs a StringContainer
	///
	/// \param n = size_type
	/// \param c = Char Type
	CL_String16(size_type n, wchar_t c);
	~CL_String16();

	/// \brief std::wstring
	///
	/// \return operator
	operator CL_StringRef16() const;

	const wchar_t *c_str() const;

	/// \brief Max size
	///
	/// \return size_type
	size_type max_size() const { return (size_type) -1; }

	/// \brief Capacity
	///
	/// \return size_type
	size_type capacity() const { return data_capacity; }

//	void swap(CL_String16 &other);
//	static swap(CL_String16 &s1, CL_String16 &s2);

	/// \brief Clear
	void clear();

	/// \brief Reserve
	///
	/// \param size = size_type
	void reserve(size_type size);

	/// \brief Resize
	///
	/// \param n = size_type
	void resize(size_type n);

	/// \brief Resize
	///
	/// \param n = size_type
	/// \param c = Char Type
	void resize(size_type n, wchar_t c);

//	template <class InputIterator>
//	CL_String16 &assign(InputIterator first, InputIterator last);
	CL_String16 &assign(const CL_StringData16 &s);

	/// \brief Assign
	///
	/// \param s = String Data
	/// \param pos = size_type
	/// \param n = size_type
	///
	/// \return String Container
	CL_String16 &assign(const CL_StringData16 &s, size_type pos, size_type n);

	/// \brief Assign
	///
	/// \param s = Char Type
	/// \param n = size_type
	///
	/// \return String Container
	CL_String16 &assign(const wchar_t *s, size_type n);

	/// \brief Assign
	///
	/// \param s = Char Type
	///
	/// \return String Container
	CL_String16 &assign(const wchar_t *s);

	/// \brief Assign
	///
	/// \param n = size_type
	/// \param c = Char Type
	///
	/// \return String Container
	CL_String16 &assign(size_type n, wchar_t c);

	/// \brief Insert
	///
	/// \param pos = iterator
	/// \param item = Char Type
	///
	/// \return iterator
	iterator insert(iterator pos, const wchar_t &item)
	{
		size_type insert_pos = (size_type) (pos - begin());
		size_type n = length();
		if (insert_pos > n)
			insert_pos = n;
		resize(n + 1);
		wchar_t *d = data();
		for (size_type i = n+1; i > insert_pos; i--)
			d[i] = d[i-1];
		d[insert_pos] = item;

		return begin() + insert_pos;
	}

	/// \brief Insert
	///
	/// \param pos = iterator
	/// \param num_copies = size_type
	/// \param item = Char Type
	void insert(iterator pos, size_type num_copies, const wchar_t &item);

	/// \brief Insert
	///
	/// \param pos = size_type
	/// \param s = Text
	///
	/// \return String Container
	CL_String16 &insert(size_type pos, const CL_StringData16 &s);

	/// \brief Insert
	///
	/// \param pos = size_type
	/// \param s = String Data
	/// \param pos1 = size_type
	/// \param length = size_type
	///
	/// \return String Container
	CL_String16 &insert(size_type pos, const CL_StringData16 &s, size_type pos1, size_type length);

	/// \brief Insert
	///
	/// \param pos = size_type
	/// \param s = Char Type
	///
	/// \return String Container
	CL_String16 &insert(size_type pos, const wchar_t *s);

	/// \brief Insert
	///
	/// \param pos = size_type
	/// \param s = Char Type
	/// \param s_length = size_type
	///
	/// \return String Container
	CL_String16 &insert(size_type pos, const wchar_t *s, size_type s_length);

	/// \brief Insert
	///
	/// \param pos = size_type
	/// \param n = size_type
	/// \param c = Char Type
	///
	/// \return String Container
	CL_String16 &insert(size_type pos, size_type n, wchar_t c);

	/// \brief Append
	///
	/// \param s = Text
	///
	/// \return String Container
	CL_String16 &append(const CL_StringData16 &s);

	/// \brief Append
	///
	/// \param s = Text
	/// \param pos = size_type
	/// \param n = size_type
	///
	/// \return String Container
	CL_String16 &append(const CL_StringData16 &s, size_type pos, size_type n);

	CL_String16 &append(const char *s);

	/// \brief Append
	///
	/// \param s = char
	/// \param n = size_type
	///
	/// \return String Container
	CL_String16 &append(const char *s, size_type n);

	/// \brief Append
	///
	/// \param s = wchar_t
	///
	/// \return String Container
	CL_String16 &append(const wchar_t *s);

	/// \brief Append
	///
	/// \param s = wchar_t
	/// \param n = size_type
	///
	/// \return String Container
	CL_String16 &append(const wchar_t *s, size_type n);

	/// \brief Append
	///
	/// \param n = size_type
	/// \param c = Char Type
	///
	/// \return String Container
	CL_String16 &append(size_type n, wchar_t c);

	/// \brief Push back
	///
	/// \param c = Char Type
	void push_back(wchar_t c);

	/// \brief Erase
	///
	/// \param p = iterator
	///
	/// \return iterator
	iterator erase(iterator p)
	{
		iterator e = end();
		if (p == e)
			return e;
		return erase(p, p+1);
	}

	/// \brief Erase
	///
	/// \param first = iterator
	/// \param last = iterator
	///
	/// \return iterator
	iterator erase(iterator first, iterator last)
	{
		size_type pos = (size_type) (first - begin());
		iterator e = end();
		iterator p1 = first;
		iterator p2 = last;
		while (p2 != e)
		{
			*p1 = *p2;
			p1++;
			p2++;
		}
		size_type new_size = (size_type) (p1 - begin());
		resize(new_size);
		return begin() + pos;
	}

	/// \brief Erase
	///
	/// \param pos = size_type
	/// \param n = size_type
	///
	/// \return String Container
	CL_String16 &erase(size_type pos = 0, size_type n = CL_StringData16::npos);

	/// \brief Replace
	///
	/// \param pos = size_type
	/// \param n = size_type
	/// \param s = Text
	///
	/// \return String Container
	CL_String16 &replace(size_type pos, size_type n, const CL_StringData16 &s);

	/// \brief Replace
	///
	/// \param pos = size_type
	/// \param n = size_type
	/// \param s = Text
	/// \param pos1 = size_type
	/// \param n1 = size_type
	///
	/// \return String Container
	CL_String16 &replace(size_type pos, size_type n, const CL_StringData16 &s, size_type pos1, size_type n1);

	/// \brief Replace
	///
	/// \param pos = size_type
	/// \param n = size_type
	/// \param s = Char Type
	/// \param n1 = size_type
	///
	/// \return String Container
	CL_String16 &replace(size_type pos, size_type n, const wchar_t *s, size_type n1);

	/// \brief Replace
	///
	/// \param pos = size_type
	/// \param n = size_type
	/// \param s = Char Type
	///
	/// \return String Container
	CL_String16 &replace(size_type pos, size_type n, const wchar_t *s);

	/// \brief Replace
	///
	/// \param pos = size_type
	/// \param n = size_type
	/// \param n1 = size_type
	/// \param c = Char Type
	///
	/// \return String Container
	CL_String16 &replace(size_type pos, size_type n, size_type n1, wchar_t c);

	/// \brief Replace
	///
	/// \param first = iterator
	/// \param last = iterator
	/// \param s = Text
	///
	/// \return String Container
	CL_String16 &replace(iterator first, iterator last, const CL_StringData16 &s);

	/// \brief Replace
	///
	/// \param first = iterator
	/// \param last = iterator
	/// \param s = Char Type
	/// \param n = size_type
	///
	/// \return String Container
	CL_String16 &replace(iterator first, iterator last, const wchar_t *s, size_type n);

	/// \brief Replace
	///
	/// \param first = iterator
	/// \param last = iterator
	/// \param s = Char Type
	///
	/// \return String Container
	CL_String16 &replace(iterator first, iterator last, const wchar_t *s);

	/// \brief Replace
	///
	/// \param first = iterator
	/// \param last = iterator
	/// \param n = size_type
	/// \param c = Char Type
	///
	/// \return String Container
	CL_String16 &replace(iterator first, iterator last, size_type n, wchar_t c);

	/// \brief Copy
	///
	/// \param buf = Char Type
	/// \param n = size_type
	/// \param pos = size_type
	///
	/// \return size_type
	size_type copy(wchar_t *buf, size_type n, size_type pos = 0) const;

	CL_String16 &operator =(const CL_String16 &source);
	CL_String16 &operator =(const CL_StringData16 &source);
	CL_String16 &operator =(const char *c_str);
	CL_String16 &operator =(const wchar_t *c_str);
	CL_String16 &operator +=(const CL_StringData16 &s);
	CL_String16 &operator +=(const char *c_str);
	CL_String16 &operator +=(const wchar_t *c_str);
	CL_String16 &operator +=(wchar_t c);

private:
	/// \brief Init
	void init();

	CL_MemoryPool *pool;
	size_type data_capacity;
	enum { local_string_length = 63 };
	wchar_t local_string[local_string_length + 1];
};

CL_API_CORE CL_String16 operator+(const CL_StringData16 &s1, const CL_StringData16 &s2);
CL_API_CORE CL_String16 operator+(const char *s1, const CL_StringData16 &s2);
CL_API_CORE CL_String16 operator+(const wchar_t *s1, const CL_StringData16 &s2);
CL_API_CORE CL_String16 operator+(const CL_StringData16 &s1, const char *s2);
CL_API_CORE CL_String16 operator+(const CL_StringData16 &s1, const wchar_t *s2);
CL_API_CORE CL_String16 operator+(wchar_t c, const CL_StringData16 &s2);
CL_API_CORE CL_String16 operator+(const CL_StringData16 &s1, wchar_t c);

/// \}
