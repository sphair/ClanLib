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
#include "string_data8.h"

class CL_MemoryPool;

/// \brief String container template.
///
///  Basic string container.  Extends StringData with operations that work on string data.
/// \xmlonly !group=Core/Text! !header=core.h! \endxmlonly
class CL_API_CORE CL_String8 : public CL_StringData8
{
public:
	CL_String8();

	/// \brief Constructs a StringContainer
	///
	/// \param source = Std String
	CL_String8(const std::string &source);

	/// \brief Constructs a StringContainer
	///
	/// \param source = String Container
	CL_String8(const CL_String8 &source);

	/// \brief Constructs a StringContainer
	CL_String8(const CL_StringData8 &source);

	/// \brief Constructs a StringContainer
	///
	/// \param c_str = char
	CL_String8(const char *c_str);

	/// \brief Constructs a StringContainer
	///
	/// \param c_str = char
	/// \param length = size_type
	CL_String8(const char *c_str, size_type length);

	/// \brief Constructs a StringContainer
	///
	/// \param wc_str = wchar_t
	CL_String8(const wchar_t *wc_str);

	/// \brief Constructs a StringContainer
	///
	/// \param wc_str = wchar_t
	/// \param length = size_type
	CL_String8(const wchar_t *wc_str, size_type length);

	/// \brief Constructs a StringContainer
	///
	/// \param n = size_type
	/// \param c = Char Type
	CL_String8(size_type n, char c);
	~CL_String8();

	/// \brief std::string
	///
	/// \return operator
	operator CL_StringRef8() const;

	const char *c_str() const;

	/// \brief Max size
	///
	/// \return size_type
	size_type max_size() const { return (size_type) -1; }

	/// \brief Capacity
	///
	/// \return size_type
	size_type capacity() const { return data_capacity; }

//	void swap(CL_String8 &other);
//	static swap(CL_String8 &s1, CL_String8 &s2);

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
	void resize(size_type n, char c);

//	template <class InputIterator>
//	CL_String8 &assign(InputIterator first, InputIterator last);
	CL_String8 &assign(const CL_StringData8 &s);

	/// \brief Assign
	///
	/// \param s = String Data
	/// \param pos = size_type
	/// \param n = size_type
	///
	/// \return String Container
	CL_String8 &assign(const CL_StringData8 &s, size_type pos, size_type n);

	/// \brief Assign
	///
	/// \param s = String data
	/// \param n = size_type
	///
	/// \return String Container
	CL_String8 &assign(const char *s, size_type n);

	/// \brief Assign
	///
	/// \param s = Char Type
	///
	/// \return String Container
	CL_String8 &assign(const char *s);

	/// \brief Assign
	///
	/// \param n = size_type
	/// \param c = Char Type
	///
	/// \return String Container
	CL_String8 &assign(size_type n, char c);

	/// \brief Insert
	///
	/// \param pos = iterator
	/// \param item = Char Type
	///
	/// \return iterator
	iterator insert(iterator pos, const char &item)
	{
		size_type insert_pos = (size_type) (pos - begin());
		size_type n = length();
		if (insert_pos > n)
			insert_pos = n;
		resize(n + 1);
		char *d = data();
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
	void insert(iterator pos, size_type num_copies, const char &item);

	/// \brief Insert
	///
	/// \param pos = size_type
	/// \param s = String Data
	///
	/// \return String Container
	CL_String8 &insert(size_type pos, const CL_StringData8 &s);

	/// \brief Insert
	///
	/// \param pos = size_type
	/// \param s = String Data
	/// \param pos1 = size_type
	/// \param length = size_type
	///
	/// \return String Container
	CL_String8 &insert(size_type pos, const CL_StringData8 &s, size_type pos1, size_type length);

	/// \brief Insert
	///
	/// \param pos = size_type
	/// \param s = Char Type
	///
	/// \return String Container
	CL_String8 &insert(size_type pos, const char *s);

	/// \brief Insert
	///
	/// \param pos = size_type
	/// \param s = Char Type
	/// \param s_length = size_type
	///
	/// \return String Container
	CL_String8 &insert(size_type pos, const char *s, size_type s_length);

	/// \brief Insert
	///
	/// \param pos = size_type
	/// \param n = size_type
	/// \param c = Char Type
	///
	/// \return String Container
	CL_String8 &insert(size_type pos, size_type n, char c);

	/// \brief Append
	///
	/// \param s = String Data
	///
	/// \return String Container
	CL_String8 &append(const CL_StringData8 &s);

	/// \brief Append
	///
	/// \param s = String Data
	/// \param pos = size_type
	/// \param n = size_type
	///
	/// \return String Container
	CL_String8 &append(const CL_StringData8 &s, size_type pos, size_type n);

	CL_String8 &append(const char *s);

	/// \brief Append
	///
	/// \param s = char
	/// \param n = size_type
	///
	/// \return String Container
	CL_String8 &append(const char *s, size_type n);

	/// \brief Append
	///
	/// \param s = wchar_t
	///
	/// \return String Container
	CL_String8 &append(const wchar_t *s);

	/// \brief Append
	///
	/// \param s = wchar_t
	/// \param n = size_type
	///
	/// \return String Container
	CL_String8 &append(const wchar_t *s, size_type n);

	/// \brief Append
	///
	/// \param n = size_type
	/// \param c = Char Type
	///
	/// \return String Container
	CL_String8 &append(size_type n, char c);

	/// \brief Push back
	///
	/// \param c = Char Type
	void push_back(char c);

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
	CL_String8 &erase(size_type pos = 0, size_type n = CL_StringData8::npos);

	/// \brief Replace
	///
	/// \param pos = size_type
	/// \param n = size_type
	/// \param s = String Data
	///
	/// \return String Container
	CL_String8 &replace(size_type pos, size_type n, const CL_StringData8 &s);

	/// \brief Replace
	///
	/// \param pos = size_type
	/// \param n = size_type
	/// \param s = String Data
	/// \param pos1 = size_type
	/// \param n1 = size_type
	///
	/// \return String Container
	CL_String8 &replace(size_type pos, size_type n, const CL_StringData8 &s, size_type pos1, size_type n1);

	/// \brief Replace
	///
	/// \param pos = size_type
	/// \param n = size_type
	/// \param s = Char Type
	/// \param n1 = size_type
	///
	/// \return String Container
	CL_String8 &replace(size_type pos, size_type n, const char *s, size_type n1);

	/// \brief Replace
	///
	/// \param pos = size_type
	/// \param n = size_type
	/// \param s = Char Type
	///
	/// \return String Container
	CL_String8 &replace(size_type pos, size_type n, const char *s);

	/// \brief Replace
	///
	/// \param pos = size_type
	/// \param n = size_type
	/// \param n1 = size_type
	/// \param c = Char Type
	///
	/// \return String Container
	CL_String8 &replace(size_type pos, size_type n, size_type n1, char c);

	/// \brief Replace
	///
	/// \param first = iterator
	/// \param last = iterator
	/// \param s = String Data
	///
	/// \return String Container
	CL_String8 &replace(iterator first, iterator last, const CL_StringData8 &s);

	/// \brief Replace
	///
	/// \param first = iterator
	/// \param last = iterator
	/// \param s = Char Type
	/// \param n = size_type
	///
	/// \return String Container
	CL_String8 &replace(iterator first, iterator last, const char *s, size_type n);

	/// \brief Replace
	///
	/// \param first = iterator
	/// \param last = iterator
	/// \param s = Char Type
	///
	/// \return String Container
	CL_String8 &replace(iterator first, iterator last, const char *s);

	/// \brief Replace
	///
	/// \param first = iterator
	/// \param last = iterator
	/// \param n = size_type
	/// \param c = Char Type
	///
	/// \return String Container
	CL_String8 &replace(iterator first, iterator last, size_type n, char c);

	/// \brief Copy
	///
	/// \param buf = Char Type
	/// \param n = size_type
	/// \param pos = size_type
	///
	/// \return size_type
	size_type copy(char *buf, size_type n, size_type pos = 0) const;

	CL_String8 &operator =(const CL_String8 &source);
	CL_String8 &operator =(const CL_StringData8 &source);
	CL_String8 &operator =(const char *c_str);
	CL_String8 &operator =(const wchar_t *c_str);
	CL_String8 &operator +=(const CL_StringData8 &s);
	CL_String8 &operator +=(const char *c_str);
	CL_String8 &operator +=(const wchar_t *c_str);
	CL_String8 &operator +=(char c);

private:
	/// \brief Init
	void init();

	CL_MemoryPool *pool;
	size_type data_capacity;
	enum { local_string_length = 63 };
	char local_string[local_string_length + 1];
};

CL_API_CORE CL_String8 operator+(const CL_StringData8 &s1, const CL_StringData8 &s2);
CL_API_CORE CL_String8 operator+(const char *s1, const CL_StringData8 &s2);
CL_API_CORE CL_String8 operator+(const wchar_t *s1, const CL_StringData8 &s2);
CL_API_CORE CL_String8 operator+(const CL_StringData8 &s1, const char *s2);
CL_API_CORE CL_String8 operator+(const CL_StringData8 &s1, const wchar_t *s2);
CL_API_CORE CL_String8 operator+(char c, const CL_StringData8 &s2);
CL_API_CORE CL_String8 operator+(const CL_StringData8 &s1, char c);

/// \}
