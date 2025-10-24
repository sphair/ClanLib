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

#include "Core/precomp.h"
#include "API/Core/Text/string8.h"
#include "API/Core/Text/string_types.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/System/memory_pool.h"

#ifndef WIN32
#include <cstring>
#endif

CL_String8::CL_String8()
: pool(0), data_capacity(local_string_length)
{
	init();
}

CL_String8::CL_String8(const std::string &source)
: pool(0), data_capacity(local_string_length)
{
	init();
	append(source.data(), source.length());
}

CL_String8::CL_String8(const CL_String8 &source)
: pool(0), data_capacity(local_string_length)
{
	init();
	append(source);
}

CL_String8::CL_String8(const CL_StringData8 &source)
: pool(0), data_capacity(local_string_length)
{
	init();
	append(source);
}

CL_String8::CL_String8(const char *c_str)
: pool(0), data_capacity(local_string_length)
{
	init();
	append(c_str);
}

CL_String8::CL_String8(const char *c_str, size_type length)
: pool(0), data_capacity(local_string_length)
{
	init();
	append(c_str, length);
}

CL_String8::CL_String8(const wchar_t *wc_str)
: pool(0), data_capacity(local_string_length)
{
	init();
	append(wc_str);
}

CL_String8::CL_String8(const wchar_t *wc_str, size_type length)
: pool(0), data_capacity(local_string_length)
{
	init();
	append(wc_str, length);
}

CL_String8::CL_String8(size_type n, char c)
: pool(0), data_capacity(local_string_length)
{
	init();
	append(n, c);
}

void CL_String8::init()
{
	local_string[0] = 0;
	this->data_ptr = local_string;
}

CL_String8::~CL_String8()
{
	if (data_capacity > local_string_length)
	{
		if (pool)
			pool->free(this->data_ptr);
		else
			delete[] this->data_ptr;
	}
}

CL_String8::operator CL_StringRef8() const
{
	return CL_StringRef8(c_str(), length(), true);
}

const char *CL_String8::c_str() const
{
	return data();
}

void CL_String8::clear()
{
	resize(0);
}

void CL_String8::reserve(size_type size)
{
	if (data_capacity >= size)
		return;

	char *old_data = this->data_ptr;

	if (pool)
		this->data_ptr = (char *) pool->alloc((size + 1) * sizeof(char));
	else
		this->data_ptr = new char[size+1];

	memcpy(this->data_ptr, old_data, this->data_length * sizeof(char));
	this->data_ptr[this->data_length] = 0;
	data_capacity = size;

	if (old_data != local_string)
	{
		if (pool)
			pool->free(old_data);
		else
			delete[] old_data;
	}
}

void CL_String8::resize(size_type n)
{
	if (n > data_capacity)
		reserve(n);
	this->data_length = n;
	this->data_ptr[this->data_length] = 0;
}

void CL_String8::resize(size_type n, char c)
{
	size_type last_length = this->data_length;
	resize(n);
	for (size_type i = last_length; i < n; i++)
		this->data_ptr[i] = c;
}

CL_String8 &CL_String8::assign(const CL_StringData8 &s)
{
	size_type length = s.length();
	if (this->data_capacity >= length)
	{
		// Allow assignment to self if new string is smaller than allocated buffer
		memmove(this->data_ptr, s.data(), sizeof(char) * length);
		resize(length);
	}
	else
	{
		resize(length);
		memcpy(this->data_ptr, s.data(), sizeof(char) * length);
	}
	return *this;
}

CL_String8 &CL_String8::assign(const CL_StringData8 &s, size_type pos, size_type n)
{
	assign(s.substr(pos, n));
	return *this;
}

CL_String8 &CL_String8::assign(const char *s, size_type n)
{
	if (this->data_capacity >= n)
	{
		memmove(this->data_ptr, s, sizeof(char) * n);
		resize(n);
	}
	else
	{
		resize(n);
		memcpy(this->data_ptr, s, sizeof(char) * this->data_length);
	}
	return *this;
}

CL_String8 &CL_String8::assign(const char *s)
{
	size_type length = CL_StringRef8(s).length();
	if (this->data_capacity >= length)
	{
		memmove(this->data_ptr, s, sizeof(char) * length);
		resize(length);
	}
	else
	{
		resize(length);
		memcpy(this->data_ptr, s, sizeof(char) * length);
	}
	return *this;
}

CL_String8 &CL_String8::assign(size_type n, char c)
{
	resize(n);
	for (size_type i = 0; i < n; i++)
		this->data_ptr[i] = c;
	return *this;
}

void CL_String8::insert(iterator pos, size_type num_copies, const char &item)
{
	if (num_copies == 0)
		return;
	size_type insert_pos = (size_type) (pos - begin());
	size_type n = length();
	if (insert_pos > n)
		insert_pos = n;
	resize(n + num_copies);
	char *d = data();
	for (size_type j = 0; j < num_copies; j++)
		d[insert_pos + j] = item;
	for (size_type i = insert_pos + num_copies; i < n + num_copies - 1; i++)
		d[i] = d[i+1];
}

CL_String8 &CL_String8::insert(size_type pos, const CL_StringData8 &s)
{
	return insert(pos, s.data(), s.length());
}

CL_String8 &CL_String8::insert(size_type pos, const CL_StringData8 &s, size_type pos1, size_type length)
{
	return insert(pos, s.data() + pos1, length);
}

CL_String8 &CL_String8::insert(size_type pos, const char *s)
{
	return insert(pos, s, CL_StringRef8(s).length());
}

CL_String8 &CL_String8::insert(size_type pos, const char *s, size_type s_length)
{
	size_type old_len = length();
	if (pos == CL_StringData8::npos || pos > old_len)
		pos = old_len;
	resize(old_len + s_length);
	char *d = data();
	size_type i;
	for (i = length(); i > pos + s_length; i--)
		d[i-1] = d[i-1-s_length];
	for (i = pos; i < pos + s_length; i++)
		d[i] = s[i-pos];
	return *this;
}

CL_String8 &CL_String8::insert(size_type pos, size_type n, char c)
{
	size_type old_len = length();
	if (pos == CL_StringData8::npos || pos > old_len)
		pos = old_len;
	resize(old_len + n);
	char *d = data();
	size_type i;
	for (i = length(); i > pos + n; i--)
		d[i-1] = d[i-1-n];
	for (i = pos; i < pos + n; i++)
		d[i] = c;
	return *this;
}

CL_String8 &CL_String8::append(const CL_StringData8 &s)
{
	return insert(length(), s);
}

CL_String8 &CL_String8::append(const CL_StringData8 &s, size_type pos, size_type n)
{
	return insert(length(), s, pos, n);
}

CL_String8 &CL_String8::append(const char *s)
{
	return insert(length(), s);
}

CL_String8 &CL_String8::append(const char *s, size_type n)
{
	return insert(length(), s, n);
}

CL_String8 &CL_String8::append(const wchar_t *s)
{
	return append(CL_StringHelp::ucs2_to_local8(s));
}

CL_String8 &CL_String8::append(const wchar_t *s, size_type n)
{
	return append(CL_StringHelp::ucs2_to_local8(CL_StringRef16(s, n, false)));
}

CL_String8 &CL_String8::append(size_type n, char c)
{
	return insert(length(), n, c);
}

void CL_String8::push_back(char c)
{
	resize(length()+1, c);
}

CL_String8 &CL_String8::erase(size_type pos, size_type n)
{
	if (pos > length())
		return *this;
	size_type left = length() - pos;
	if (n == CL_StringData8::npos || n > left)
		n = left;
	erase(begin() + pos, begin() + pos + n);
	return *this;
}

CL_String8 &CL_String8::replace(size_type pos, size_type n, const CL_StringData8 &s)
{
	return replace(pos, n, s.data(), s.length());
}

CL_String8 &CL_String8::replace(size_type pos, size_type n, const CL_StringData8 &s, size_type pos1, size_type n1)
{
	if (pos1 == CL_StringData8::npos || pos1 > s.length())
		return *this;
	if (pos1 + n1 > s.length())
		n1 = s.length() - pos1;
	return replace(pos, n, s.data() + pos1, n1);
}

CL_String8 &CL_String8::replace(size_type pos, size_type n, const char *s, size_type n1)
{
	if (pos == CL_StringData8::npos || pos > length())
		pos = length();
	if (n == CL_StringData8::npos || pos + n > length())
		n = length() - pos;
	if (n1 == n)
	{
		memcpy(data() + pos, s, n * sizeof(char));
	}
	else if (n1 > n)
	{
		size_type rest_of_string = length() - pos - n;
		resize(length() + n1 - n);
		memmove(data() + pos + n1, data() + pos + n, rest_of_string * sizeof(char));
		memcpy(data() + pos, s, n1 * sizeof(char));
	}
	else
	{
		size_type rest_of_string = length() - pos - n;
		memcpy(data() + pos, s, n1 * sizeof(char));
		memcpy(data() + pos + n1, data() + pos + n, rest_of_string * sizeof(char));
		resize(length() + n1 - n);
	}
	return *this;
}

CL_String8 &CL_String8::replace(size_type pos, size_type n, const char *s)
{
	return replace(pos, n, s, CL_StringRef8(s).length());
}

CL_String8 &CL_String8::replace(size_type pos, size_type n, size_type n1, char c)
{
	if (pos == CL_StringData8::npos || pos > length())
		pos = length();
	if (n == CL_StringData8::npos || pos + n > length())
		n = length() - pos;
	if (n1 == n)
	{
		char *d = data() + pos;
		for (size_type i = 0; i < n1; i++)
			d[i] = c;
	}
	else if (n1 > n)
	{
		size_type rest_of_string = length() - pos - n;
		resize(length() + n1 - n);
		memmove(data() + pos + n1, data() + pos + n, rest_of_string * sizeof(char));
		char *d = data() + pos;
		for (size_type i = 0; i < n1; i++)
			d[i] = c;
	}
	else
	{
		size_type rest_of_string = length() - pos - n;
		char *d = data() + pos;
		for (size_type i = 0; i < n1; i++)
			d[i] = c;
		memcpy(data() + pos + n1, data() + pos + n, rest_of_string * sizeof(char));
		resize(length() + n1 - n);
	}
	return *this;
}

CL_String8 &CL_String8::replace(iterator first, iterator last, const CL_StringData8 &s)
{
	return replace((size_type) (first - begin()), (size_type) (last-first), s);
}

CL_String8 &CL_String8::replace(iterator first, iterator last, const char *s, size_type n)
{
	return replace((size_type) (first - begin()), (size_type) (last-first), s, n);
}

CL_String8 &CL_String8::replace(iterator first, iterator last, const char *s)
{
	return replace((size_type) (first - begin()), (size_type) (last-first), s);
}

CL_String8 &CL_String8::replace(iterator first, iterator last, size_type n, char c)
{
	return replace((size_type) (first - begin()), (size_type) (last-first), n, c);
}

CL_StringData8::size_type CL_String8::copy(char *buf, size_type n, size_type pos) const
{
	if (pos == CL_StringData8::npos || pos > length())
		return 0;
	if (pos + n > length())
		n = length() - pos;
	memcpy(buf, data(), n * sizeof(char));
	return n;
}

CL_String8 &CL_String8::operator =(const CL_String8 &source)
{
	return assign(source);
}

CL_String8 &CL_String8::operator =(const CL_StringData8 &source)
{
	return assign(source);
}

CL_String8 &CL_String8::operator =(const char *c_str)
{
	clear();
	return append(c_str);
}

CL_String8 &CL_String8::operator =(const wchar_t *c_str)
{
	clear();
	return append(c_str);
}

CL_String8 &CL_String8::operator +=(const CL_StringData8 &s)
{
	return append(s);
}

CL_String8 &CL_String8::operator +=(const char *c_str)
{
	return append(c_str);
}

CL_String8 &CL_String8::operator +=(const wchar_t *c_str)
{
	return append(c_str);
}

CL_String8 &CL_String8::operator +=(char c)
{
	return append(1, c);
}

/////////////////////////////////////////////////////////////////////////////

CL_String8 operator+(const CL_StringData8 &s1, const CL_StringData8 &s2)
{
	CL_String8 result;
	result.reserve(s1.length() + s2.length());
	result.append(s1);
	result.append(s2);
	return result;
}

CL_String8 operator+(const char *s1, const CL_StringData8 &s2)
{
	CL_String8 result;
	CL_StringRef8 ref_s1(s1);
	result.reserve(ref_s1.length() + s2.length());
	result.append(ref_s1);
	result.append(s2);
	return result;
}

CL_String8 operator+(const wchar_t *s1, const CL_StringData8 &s2)
{
	CL_String8 result;
	CL_StringRef8 ref_s1(s1);
	result.reserve(ref_s1.length() + s2.length());
	result.append(ref_s1);
	result.append(s2);
	return result;
}

CL_String8 operator+(const CL_StringData8 &s1, const char *s2)
{
	CL_String8 result;
	CL_StringRef8 ref_s2(s2);
	result.reserve(ref_s2.length() + s1.length());
	result.append(s1);
	result.append(ref_s2);
	return result;
}

CL_String8 operator+(const CL_StringData8 &s1, const wchar_t *s2)
{
	CL_String8 result;
	CL_StringRef8 ref_s2(s2);
	result.reserve(ref_s2.length() + s1.length());
	result.append(s1);
	result.append(ref_s2);
	return result;
}

CL_String8 operator+(char c, const CL_StringData8 &s2)
{
	CL_String8 result;
	result.reserve(1 + s2.length());
	result.push_back(c);
	result.append(s2);
	return result;
}

CL_String8 operator+(const CL_StringData8 &s1, char c)
{
	CL_String8 result;
	result.reserve(1 + s1.length());
	result.append(s1);
	result.push_back(c);
	return result;
}
