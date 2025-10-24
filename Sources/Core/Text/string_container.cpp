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

#include "Core/precomp.h"
#include "API/Core/Text/string_container.h"
#include "API/Core/Text/string_types.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/System/memory_pool.h"

#ifndef WIN32
#include <cstring>
#endif

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringContainer<CharType, ReferenceClass, StdString>::CL_StringContainer()
: pool(0), data_capacity(local_string_length)
{
	init();
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringContainer<CharType, ReferenceClass, StdString>::CL_StringContainer(const StdString &source)
: pool(0), data_capacity(local_string_length)
{
	init();
	append(source.data(), source.length());
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringContainer<CharType, ReferenceClass, StdString>::CL_StringContainer(const CL_StringContainer &source)
: pool(0), data_capacity(local_string_length)
{
	init();
	append(source);
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringContainer<CharType, ReferenceClass, StdString>::CL_StringContainer(const CL_StringData<CharType, ReferenceClass, StdString> &source)
: pool(0), data_capacity(local_string_length)
{
	init();
	append(source);
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringContainer<CharType, ReferenceClass, StdString>::CL_StringContainer(const char *c_str)
: pool(0), data_capacity(local_string_length)
{
	init();
	append(c_str);
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringContainer<CharType, ReferenceClass, StdString>::CL_StringContainer(const char *c_str, size_type length)
: pool(0), data_capacity(local_string_length)
{
	init();
	append(c_str, length);
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringContainer<CharType, ReferenceClass, StdString>::CL_StringContainer(const wchar_t *wc_str)
: pool(0), data_capacity(local_string_length)
{
	init();
	append(wc_str);
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringContainer<CharType, ReferenceClass, StdString>::CL_StringContainer(const wchar_t *wc_str, size_type length)
: pool(0), data_capacity(local_string_length)
{
	init();
	append(wc_str, length);
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringContainer<CharType, ReferenceClass, StdString>::CL_StringContainer(size_type n, CharType c)
: pool(0), data_capacity(local_string_length)
{
	init();
	append(n, c);
}

template<typename CharType, typename ReferenceClass, typename StdString>
void CL_StringContainer<CharType, ReferenceClass, StdString>::init()
{
	local_string[0] = 0;
	this->data_ptr = local_string;
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringContainer<CharType, ReferenceClass, StdString>::~CL_StringContainer()
{
	if (data_capacity > local_string_length)
	{
		if (pool)
			pool->free(this->data_ptr);
		else
			delete[] this->data_ptr;
	}
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringContainer<CharType, ReferenceClass, StdString>::operator ReferenceClass() const
{
	return ReferenceClass(c_str(), length(), true);
}

template<typename CharType, typename ReferenceClass, typename StdString>
const CharType *CL_StringContainer<CharType, ReferenceClass, StdString>::c_str() const
{
	return data();
}

template<typename CharType, typename ReferenceClass, typename StdString>
void CL_StringContainer<CharType, ReferenceClass, StdString>::clear()
{
	resize(0);
}

template<typename CharType, typename ReferenceClass, typename StdString>
void CL_StringContainer<CharType, ReferenceClass, StdString>::reserve(size_type size)
{
	if (data_capacity >= size)
		return;

	CharType *old_data = this->data_ptr;

	if (pool)
		this->data_ptr = (CharType *) pool->alloc((size + 1) * sizeof(CharType));
	else
		this->data_ptr = new CharType[size+1];

	memcpy(this->data_ptr, old_data, this->data_length * sizeof(CharType));
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

template<typename CharType, typename ReferenceClass, typename StdString>
void CL_StringContainer<CharType, ReferenceClass, StdString>::resize(size_type n)
{
	if (n > data_capacity)
		reserve(n);
	this->data_length = n;
	this->data_ptr[this->data_length] = 0;
}

template<typename CharType, typename ReferenceClass, typename StdString>
void CL_StringContainer<CharType, ReferenceClass, StdString>::resize(size_type n, CharType c)
{
	size_type last_length = this->data_length;
	resize(n);
	for (size_type i = last_length; i < n; i++)
		this->data_ptr[i] = c;
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringContainer<CharType, ReferenceClass, StdString> &CL_StringContainer<CharType, ReferenceClass, StdString>::assign(const CL_StringData<CharType, ReferenceClass, StdString> &s)
{
	size_type length = s.length();
	if (this->data_capacity >= length)
	{
		// Allow assignment to self if new string is smaller than allocated buffer
		memmove(this->data_ptr, s.data(), sizeof(CharType) * length);
		resize(length);
	}
	else
	{
		resize(length);
		memcpy(this->data_ptr, s.data(), sizeof(CharType) * length);
	}
	return *this;
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringContainer<CharType, ReferenceClass, StdString> &CL_StringContainer<CharType, ReferenceClass, StdString>::assign(const CL_StringData<CharType, ReferenceClass, StdString> &s, size_type pos, size_type n)
{
	assign(s.substr(pos, n));
	return *this;
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringContainer<CharType, ReferenceClass, StdString> &CL_StringContainer<CharType, ReferenceClass, StdString>::assign(const CharType *s, size_type n)
{
	if (this->data_capacity >= n)
	{
		memmove(this->data_ptr, s, sizeof(CharType) * n);
		resize(n);
	}
	else
	{
		resize(n);
		memcpy(this->data_ptr, s, sizeof(CharType) * this->data_length);
	}
	return *this;
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringContainer<CharType, ReferenceClass, StdString> &CL_StringContainer<CharType, ReferenceClass, StdString>::assign(const CharType *s)
{
	size_type length = ReferenceClass(s).length();
	if (this->data_capacity >= length)
	{
		memmove(this->data_ptr, s, sizeof(CharType) * length);
		resize(length);
	}
	else
	{
		resize(length);
		memcpy(this->data_ptr, s, sizeof(CharType) * length);
	}
	return *this;
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringContainer<CharType, ReferenceClass, StdString> &CL_StringContainer<CharType, ReferenceClass, StdString>::assign(size_type n, CharType c)
{
	resize(n);
	for (size_type i = 0; i < n; i++)
		this->data_ptr[i] = c;
	return *this;
}

template<typename CharType, typename ReferenceClass, typename StdString>
void CL_StringContainer<CharType, ReferenceClass, StdString>::insert(iterator pos, size_type num_copies, const CharType &item)
{
	if (num_copies == 0)
		return;
	size_type insert_pos = (size_type) (pos - begin());
	size_type n = length();
	if (insert_pos > n)
		insert_pos = n;
	resize(n + num_copies);
	CharType *d = data();
	for (size_type j = 0; j < num_copies; j++)
		d[insert_pos + j] = item;
	for (size_type i = insert_pos + num_copies; i < n + num_copies - 1; i++)
		d[i] = d[i+1];
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringContainer<CharType, ReferenceClass, StdString> &CL_StringContainer<CharType, ReferenceClass, StdString>::insert(size_type pos, const CL_StringData<CharType, ReferenceClass, StdString> &s)
{
	return insert(pos, s.data(), s.length());
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringContainer<CharType, ReferenceClass, StdString> &CL_StringContainer<CharType, ReferenceClass, StdString>::insert(size_type pos, const CL_StringData<CharType, ReferenceClass, StdString> &s, size_type pos1, size_type length)
{
	return insert(pos, s.data() + pos1, length);
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringContainer<CharType, ReferenceClass, StdString> &CL_StringContainer<CharType, ReferenceClass, StdString>::insert(size_type pos, const CharType *s)
{
	return insert(pos, s, ReferenceClass(s).length());
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringContainer<CharType, ReferenceClass, StdString> &CL_StringContainer<CharType, ReferenceClass, StdString>::insert(size_type pos, const CharType *s, size_type s_length)
{
	size_type old_len = length();
	if (pos == CL_StringDataTypes::npos || pos > old_len)
		pos = old_len;
	resize(old_len + s_length);
	CharType *d = data();
	size_type i;
	for (i = length(); i > pos + s_length; i--)
		d[i-1] = d[i-1-s_length];
	for (i = pos; i < pos + s_length; i++)
		d[i] = s[i-pos];
	return *this;
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringContainer<CharType, ReferenceClass, StdString> &CL_StringContainer<CharType, ReferenceClass, StdString>::insert(size_type pos, size_type n, CharType c)
{
	size_type old_len = length();
	if (pos == CL_StringDataTypes::npos || pos > old_len)
		pos = old_len;
	resize(old_len + n);
	CharType *d = data();
	size_type i;
	for (i = length(); i > pos + n; i--)
		d[i-1] = d[i-1-n];
	for (i = pos; i < pos + n; i++)
		d[i] = c;
	return *this;
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringContainer<CharType, ReferenceClass, StdString> &CL_StringContainer<CharType, ReferenceClass, StdString>::append(const CL_StringData<CharType, ReferenceClass, StdString> &s)
{
	return insert(length(), s);
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringContainer<CharType, ReferenceClass, StdString> &CL_StringContainer<CharType, ReferenceClass, StdString>::append(const CL_StringData<CharType, ReferenceClass, StdString> &s, size_type pos, size_type n)
{
	return insert(length(), s, pos, n);
}

template<>
CL_StringContainer<char, CL_StringRef8, std::string> &CL_StringContainer<char, CL_StringRef8, std::string>::append(const char *s)
{
	return insert(length(), s);
}

template<>
CL_StringContainer<char, CL_StringRef8, std::string> &CL_StringContainer<char, CL_StringRef8, std::string>::append(const char *s, size_type n)
{
	return insert(length(), s, n);
}

template<>
CL_StringContainer<char, CL_StringRef8, std::string> &CL_StringContainer<char, CL_StringRef8, std::string>::append(const wchar_t *s)
{
	return append(CL_StringHelp::ucs2_to_local8(s));
}

template<>
CL_StringContainer<char, CL_StringRef8, std::string> &CL_StringContainer<char, CL_StringRef8, std::string>::append(const wchar_t *s, size_type n)
{
	return append(CL_StringHelp::ucs2_to_local8(CL_StringRef16(s, n, false)));
}

template<>
CL_StringContainer<wchar_t, CL_StringRef16, std::wstring> &CL_StringContainer<wchar_t, CL_StringRef16, std::wstring>::append(const char *s)
{
	return append(CL_StringHelp::local8_to_ucs2(s));
}

template<>
CL_StringContainer<wchar_t, CL_StringRef16, std::wstring> &CL_StringContainer<wchar_t, CL_StringRef16, std::wstring>::append(const char *s, size_type n)
{
	return append(CL_StringHelp::local8_to_ucs2(CL_StringRef8(s, n, false)));
}

template<>
CL_StringContainer<wchar_t, CL_StringRef16, std::wstring> &CL_StringContainer<wchar_t, CL_StringRef16, std::wstring>::append(const wchar_t *s)
{
	return insert(length(), s);
}

template<>
CL_StringContainer<wchar_t, CL_StringRef16, std::wstring> &CL_StringContainer<wchar_t, CL_StringRef16, std::wstring>::append(const wchar_t *s, size_type n)
{
	return insert(length(), s, n);
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringContainer<CharType, ReferenceClass, StdString> &CL_StringContainer<CharType, ReferenceClass, StdString>::append(size_type n, CharType c)
{
	return insert(length(), n, c);
}

template<typename CharType, typename ReferenceClass, typename StdString>
void CL_StringContainer<CharType, ReferenceClass, StdString>::push_back(CharType c)
{
	resize(length()+1, c);
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringContainer<CharType, ReferenceClass, StdString> &CL_StringContainer<CharType, ReferenceClass, StdString>::erase(size_type pos, size_type n)
{
	if (pos > length())
		return *this;
	size_type left = length() - pos;
	if (n == CL_StringDataTypes::npos || n > left)
		n = left;
	erase(begin() + pos, begin() + pos + n);
	return *this;
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringContainer<CharType, ReferenceClass, StdString> &CL_StringContainer<CharType, ReferenceClass, StdString>::replace(size_type pos, size_type n, const CL_StringData<CharType, ReferenceClass, StdString> &s)
{
	return replace(pos, n, s.data(), s.length());
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringContainer<CharType, ReferenceClass, StdString> &CL_StringContainer<CharType, ReferenceClass, StdString>::replace(size_type pos, size_type n, const CL_StringData<CharType, ReferenceClass, StdString> &s, size_type pos1, size_type n1)
{
	if (pos1 == CL_StringDataTypes::npos || pos1 > s.length())
		return *this;
	if (pos1 + n1 > s.length())
		n1 = s.length() - pos1;
	return replace(pos, n, s.data() + pos1, n1);
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringContainer<CharType, ReferenceClass, StdString> &CL_StringContainer<CharType, ReferenceClass, StdString>::replace(size_type pos, size_type n, const CharType *s, size_type n1)
{
	if (pos == CL_StringDataTypes::npos || pos > length())
		pos = length();
	if (n == CL_StringDataTypes::npos || pos + n > length())
		n = length() - pos;
	if (n1 == n)
	{
		memcpy(data() + pos, s, n * sizeof(CharType));
	}
	else if (n1 > n)
	{
		size_type rest_of_string = length() - pos - n;
		resize(length() + n1 - n);
		memmove(data() + pos + n1, data() + pos + n, rest_of_string * sizeof(CharType));
		memcpy(data() + pos, s, n1 * sizeof(CharType));
	}
	else
	{
		size_type rest_of_string = length() - pos - n;
		memcpy(data() + pos, s, n1 * sizeof(CharType));
		memcpy(data() + pos + n1, data() + pos + n, rest_of_string * sizeof(CharType));
		resize(length() + n1 - n);
	}
	return *this;
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringContainer<CharType, ReferenceClass, StdString> &CL_StringContainer<CharType, ReferenceClass, StdString>::replace(size_type pos, size_type n, const CharType *s)
{
	return replace(pos, n, s, ReferenceClass(s).length());
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringContainer<CharType, ReferenceClass, StdString> &CL_StringContainer<CharType, ReferenceClass, StdString>::replace(size_type pos, size_type n, size_type n1, CharType c)
{
	if (pos == CL_StringDataTypes::npos || pos > length())
		pos = length();
	if (n == CL_StringDataTypes::npos || pos + n > length())
		n = length() - pos;
	if (n1 == n)
	{
		CharType *d = data() + pos;
		for (size_type i = 0; i < n1; i++)
			d[i] = c;
	}
	else if (n1 > n)
	{
		size_type rest_of_string = length() - pos - n;
		resize(length() + n1 - n);
		memmove(data() + pos + n1, data() + pos + n, rest_of_string * sizeof(CharType));
		CharType *d = data() + pos;
		for (size_type i = 0; i < n1; i++)
			d[i] = c;
	}
	else
	{
		size_type rest_of_string = length() - pos - n;
		CharType *d = data() + pos;
		for (size_type i = 0; i < n1; i++)
			d[i] = c;
		memcpy(data() + pos + n1, data() + pos + n, rest_of_string * sizeof(CharType));
		resize(length() + n1 - n);
	}
	return *this;
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringContainer<CharType, ReferenceClass, StdString> &CL_StringContainer<CharType, ReferenceClass, StdString>::replace(iterator first, iterator last, const CL_StringData<CharType, ReferenceClass, StdString> &s)
{
	return replace((size_type) (first - begin()), (size_type) (last-first), s);
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringContainer<CharType, ReferenceClass, StdString> &CL_StringContainer<CharType, ReferenceClass, StdString>::replace(iterator first, iterator last, const CharType *s, size_type n)
{
	return replace((size_type) (first - begin()), (size_type) (last-first), s, n);
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringContainer<CharType, ReferenceClass, StdString> &CL_StringContainer<CharType, ReferenceClass, StdString>::replace(iterator first, iterator last, const CharType *s)
{
	return replace((size_type) (first - begin()), (size_type) (last-first), s);
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringContainer<CharType, ReferenceClass, StdString> &CL_StringContainer<CharType, ReferenceClass, StdString>::replace(iterator first, iterator last, size_type n, CharType c)
{
	return replace((size_type) (first - begin()), (size_type) (last-first), n, c);
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringDataTypes::size_type CL_StringContainer<CharType, ReferenceClass, StdString>::copy(CharType *buf, size_type n, size_type pos) const
{
	if (pos == CL_StringDataTypes::npos || pos > length())
		return 0;
	if (pos + n > length())
		n = length() - pos;
	memcpy(buf, data(), n * sizeof(CharType));
	return n;
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringContainer<CharType, ReferenceClass, StdString> &CL_StringContainer<CharType, ReferenceClass, StdString>::operator =(const CL_StringContainer &source)
{
	return assign(source);
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringContainer<CharType, ReferenceClass, StdString> &CL_StringContainer<CharType, ReferenceClass, StdString>::operator =(const CL_StringData<CharType, ReferenceClass, StdString> &source)
{
	return assign(source);
}
/*
template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringContainer<CharType, ReferenceClass, StdString> &CL_StringContainer<CharType, ReferenceClass, StdString>::operator =(const CharType *str)
{
	return assign(str);
}
*/

template<>
CL_StringContainer<char, CL_StringRef8, std::string> &CL_StringContainer<char, CL_StringRef8, std::string>::operator =(const char *c_str)
{
	clear();
	return append(c_str);
}

template<>
CL_StringContainer<char, CL_StringRef8, std::string> &CL_StringContainer<char, CL_StringRef8, std::string>::operator =(const wchar_t *c_str)
{
	clear();
	return append(c_str);
}

template<>
CL_StringContainer<wchar_t, CL_StringRef16, std::wstring> &CL_StringContainer<wchar_t, CL_StringRef16, std::wstring>::operator =(const char *c_str)
{
	clear();
	return append(c_str);
}

template<>
CL_StringContainer<wchar_t, CL_StringRef16, std::wstring> &CL_StringContainer<wchar_t, CL_StringRef16, std::wstring>::operator =(const wchar_t *c_str)
{
	clear();
	return append(c_str);
}



template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringContainer<CharType, ReferenceClass, StdString> &CL_StringContainer<CharType, ReferenceClass, StdString>::operator +=(const CL_StringData<CharType, ReferenceClass, StdString> &s)
{
	return append(s);
}

template<>
CL_StringContainer<char, CL_StringRef8, std::string> &CL_StringContainer<char, CL_StringRef8, std::string>::operator +=(const char *c_str)
{
	return append(c_str);
}

template<>
CL_StringContainer<char, CL_StringRef8, std::string> &CL_StringContainer<char, CL_StringRef8, std::string>::operator +=(const wchar_t *c_str)
{
	return append(c_str);
}

template<>
CL_StringContainer<wchar_t, CL_StringRef16, std::wstring> &CL_StringContainer<wchar_t, CL_StringRef16, std::wstring>::operator +=(const char *c_str)
{
	return append(c_str);
}

template<>
CL_StringContainer<wchar_t, CL_StringRef16, std::wstring> &CL_StringContainer<wchar_t, CL_StringRef16, std::wstring>::operator +=(const wchar_t *c_str)
{
	return append(c_str);
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringContainer<CharType, ReferenceClass, StdString> &CL_StringContainer<CharType, ReferenceClass, StdString>::operator +=(CharType c)
{
	return append(1, c);
}

// Explicit instantiate the versions we use:
template class CL_StringContainer<char, CL_StringRef8, std::string>;
template class CL_StringContainer<wchar_t, CL_StringRef16, std::wstring>;
