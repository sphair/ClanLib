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

#include "Core/precomp.h"
#include "API/Core/Text/string_data.h"
#include "API/Core/Text/string_types.h"

const CL_StringDataTypes::size_type CL_StringDataTypes::npos = 0xffffffff;

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringData<CharType, ReferenceClass, StdString>::CL_StringData()
: data_ptr(0), data_length(0)
{
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringData<CharType, ReferenceClass, StdString>::CL_StringData(const CharType *ptr, size_type length)
: data_ptr((CharType *) ptr), data_length(length)
{
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringData<CharType, ReferenceClass, StdString>::operator StdString() const
{
	return StdString(data(), length());
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringData<CharType, ReferenceClass, StdString>::operator ReferenceClass() const
{
	return ReferenceClass(data(), length(), false);
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringDataTypes::size_type CL_StringData<CharType, ReferenceClass, StdString>::find(const CL_StringData &s, size_type pos) const
{
	return find(s.data(), pos, s.length());
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringDataTypes::size_type CL_StringData<CharType, ReferenceClass, StdString>::find(const CharType *s, size_type pos, size_type n) const
{
	size_type len = data_length;
	if (pos == npos || pos == len || n == 0)
		return npos;
	const CharType *d = data_ptr;
	size_type cur_char = 0;
	size_type cur_start = 0;
	for (size_type i = pos; i < len; i++)
	{
		if (cur_char == 0)
		{
			if (d[i] == s[cur_char])
			{
				cur_start = i;
				cur_char++;
				if (cur_char == n)
					return cur_start;
			}
		}
		else
		{
			if (d[i] != s[cur_char])
			{
				cur_char = 0;
				i = cur_start;
			}
			else
			{
				cur_char++;
				if (cur_char == n)
					return cur_start;
			}
		}
	}
	return npos;
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringDataTypes::size_type CL_StringData<CharType, ReferenceClass, StdString>::find(const CharType *s, size_type pos) const
{
	size_type len = 0;
	while (s[len] != 0)
		len++;
	return find(s, pos, len);
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringDataTypes::size_type CL_StringData<CharType, ReferenceClass, StdString>::find(CharType c, size_type pos) const
{
	size_type len = data_length;
	if (pos == npos || pos == len)
		return npos;
	const CharType *d = (const CharType *) data_ptr;
	for (size_type i = pos; i < len; i++)
		if (d[i] == c)
			return i;
	return npos;
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringDataTypes::size_type CL_StringData<CharType, ReferenceClass, StdString>::rfind(const CL_StringData &s, size_type pos) const
{
	return rfind(s.data(), pos, s.length());
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringDataTypes::size_type CL_StringData<CharType, ReferenceClass, StdString>::rfind(const CharType *s, size_type pos, size_type n) const
{
	size_type len = data_length;
	if (pos == npos)
		pos = len;
	if (len == 0 || n == 0)
		return npos;
	const CharType *d = data_ptr;
	size_type cur_char = n;
	size_type cur_start = 0;
	while (true)
	{
		if (cur_char == n)
		{
			if (d[pos] == s[cur_char-1])
			{
				cur_start = pos;
				cur_char--;
				if (cur_char == 0)
					return pos;
			}
		}
		else
		{
			if (d[pos] != s[cur_char-1])
			{
				cur_char = n;
				pos = cur_start;
			}
			else
			{
				cur_char--;
				if (cur_char == 0)
					return pos;
			}
		}
		if (pos == 0)
			break;
		pos--;
	}
	return npos;
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringDataTypes::size_type CL_StringData<CharType, ReferenceClass, StdString>::rfind(const CharType *s, size_type pos) const
{
	size_type len = 0;
	while (s[len] != 0)
		len++;
	return rfind(s, pos, len);
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringDataTypes::size_type CL_StringData<CharType, ReferenceClass, StdString>::rfind(CharType c, size_type pos) const
{
	size_type len = data_length;
	if (pos == npos)
		pos = len;
	if (len == 0)
		return npos;
	if (pos >= len)
		pos = len - 1;
	const CharType *d = data_ptr;
	while (true)
	{
		if (d[pos] == c)
			return pos;
		if (pos == 0)
			break;
		pos--;
	}
	return npos;
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringDataTypes::size_type CL_StringData<CharType, ReferenceClass, StdString>::find_first_of(const CL_StringData &s, size_type pos) const
{
	return find_first_of(s.data(), pos, s.length());
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringDataTypes::size_type CL_StringData<CharType, ReferenceClass, StdString>::find_first_of(const CharType *s, size_type pos, size_type n) const
{
	size_type len = data_length;
	if (pos == npos || pos == len)
		return npos;
	const CharType *d = data_ptr;
	for (size_type i = pos; i < len; i++)
	{
		for (size_type j = 0; j < n; j++)
			if (d[i] == s[j])
				return i;
	}
	return npos;
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringDataTypes::size_type CL_StringData<CharType, ReferenceClass, StdString>::find_first_of(const CharType *s, size_type pos) const
{
	size_type len = 0;
	while (s[len] != 0)
		len++;
	return find_first_of(s, pos, len);
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringDataTypes::size_type CL_StringData<CharType, ReferenceClass, StdString>::find_first_of(CharType c, size_type pos) const
{
	size_type len = data_length;
	if (pos == npos || pos == len)
		return npos;
	const CharType *d = (const CharType *) data_ptr;
	for (size_type i = pos; i < len; i++)
		if (d[i] == c)
			return i;
	return npos;
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringDataTypes::size_type CL_StringData<CharType, ReferenceClass, StdString>::find_first_not_of(const CL_StringData &s, size_type pos) const
{
	return find_first_not_of(s.data(), pos, s.length());
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringDataTypes::size_type CL_StringData<CharType, ReferenceClass, StdString>::find_first_not_of(const CharType *s, size_type pos, size_type n) const
{
	size_type len = data_length;
	if (pos == npos || pos == len)
		return npos;
	const CharType *d = data_ptr;
	for (size_type i = pos; i < len; i++)
	{
		bool found = false;
		for (size_type j = 0; j < n; j++)
			if (d[i] == s[j])
				found = true;
		if (!found)
			return i;
	}
	return npos;
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringDataTypes::size_type CL_StringData<CharType, ReferenceClass, StdString>::find_first_not_of(const CharType *s, size_type pos) const
{
	size_type len = 0;
	while (s[len] != 0)
		len++;
	return find_first_not_of(s, pos, len);
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringDataTypes::size_type CL_StringData<CharType, ReferenceClass, StdString>::find_first_not_of(CharType c, size_type pos) const
{
	size_type len = data_length;
	if (pos == npos || pos == len)
		return npos;
	const CharType *d = data_ptr;
	for (size_type i = pos; i < len; i++)
		if (d[i] != c)
			return i;
	return npos;
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringDataTypes::size_type CL_StringData<CharType, ReferenceClass, StdString>::find_last_of(const CL_StringData &s, size_type pos) const
{
	return find_last_of(s.data(), pos, s.length());
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringDataTypes::size_type CL_StringData<CharType, ReferenceClass, StdString>::find_last_of(const CharType *s, size_type pos, size_type n) const
{
	size_type len = data_length;
	if (pos > len)
		pos = len;
	const CharType *d = data_ptr;
	for (size_type i = pos; i != 0; i--)
	{
		for (size_type j = 0; j < n; j++)
			if (d[i-1] == s[j])
				return i-1;
	}

	return npos;
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringDataTypes::size_type CL_StringData<CharType, ReferenceClass, StdString>::find_last_of(const CharType *s, size_type pos) const
{
	size_type len = 0;
	while (s[len] != 0)
		len++;
	return find_last_of(s, pos, len);
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringDataTypes::size_type CL_StringData<CharType, ReferenceClass, StdString>::find_last_of(CharType c, size_type pos) const
{
	size_type len = data_length;
	if (pos > len)
		pos = len;
	const CharType *d = data_ptr;
	for (size_type i = pos; i != 0; i--)
		if (d[i-1] == c)
			return i-1;
	return npos;
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringDataTypes::size_type CL_StringData<CharType, ReferenceClass, StdString>::find_last_not_of(const CL_StringData &s, size_type pos) const
{
	return find_last_not_of(s.data(), pos, s.length());
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringDataTypes::size_type CL_StringData<CharType, ReferenceClass, StdString>::find_last_not_of(const CharType *s, size_type pos, size_type n) const
{
	size_type len = data_length;
	if (pos > len)
		pos = len;
	const CharType *d = data_ptr;
	for (size_type i = pos; i != 0; i--)
	{
		bool found = false;
		for (size_type j = 0; j < n; j++)
			if (d[i-1] == s[j])
				found = true;
		if (!found)
			return i-1;
	}
	return npos;
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringDataTypes::size_type CL_StringData<CharType, ReferenceClass, StdString>::find_last_not_of(const CharType *s, size_type pos) const
{
	size_type len = 0;
	while (s[len] != 0)
		len++;
	return find_last_not_of(s, pos, len);
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_StringDataTypes::size_type CL_StringData<CharType, ReferenceClass, StdString>::find_last_not_of(CharType c, size_type pos) const
{
	size_type len = data_length;
	if (pos > len)
		pos = len;
	const CharType *d = data_ptr;
	for (size_type i = pos; i != 0; i--)
	{
		if (d[i-1] != c)
			return i-1;
	}
	return npos;
}

template<typename CharType, typename ReferenceClass, typename StdString>
ReferenceClass CL_StringData<CharType, ReferenceClass, StdString>::substr(size_type pos, size_type n) const
{
	const CharType *d = data_ptr;
	if (pos == npos || pos > data_length)
		return ReferenceClass(d + data_length, 0, false);
	if (n == npos || pos + n > data_length)
		n = data_length - pos;
	return ReferenceClass(d + pos, n, false);
}

template<typename CharType, typename ReferenceClass, typename StdString>
int CL_StringData<CharType, ReferenceClass, StdString>::compare(const CL_StringData &s) const
{
	const CharType *d1 = data_ptr;
	const CharType *d2 = s.data_ptr;
	size_type l1 = data_length;
	size_type l2 = s.data_length;
	size_type l = (l1 < l2) ? l1 : l2;
	for (size_type i = 0; i < l; i++)
		if (d1[i] != d2[i])
			return (int) (((unsigned int) d1[i]) - ((unsigned int) d2[i]));
	return l1-l2;
}

template<typename CharType, typename ReferenceClass, typename StdString>
int CL_StringData<CharType, ReferenceClass, StdString>::compare(size_type pos, size_type n, const CL_StringData &s) const
{
	const CharType *d1 = data_ptr;
	const CharType *d2 = s.data_ptr;
	size_type l1 = data_length;
	size_type l2 = s.data_length;

	if (pos > l1)
		pos = l1;
	d1 += pos;
	l1 -= pos;
	if (n < l1)
		l1 = n;

	size_type l = (l1 < l2) ? l1 : l2;
	for (size_type i = 0; i < l; i++)
		if (d1[i] != d2[i])
			return (int) (((unsigned int) d1[i]) - ((unsigned int) d2[i]));
	return l1-l2;
}

template<typename CharType, typename ReferenceClass, typename StdString>
int CL_StringData<CharType, ReferenceClass, StdString>::compare(size_type pos, size_type n, const CL_StringData &s, size_type pos1, size_type n1) const
{
	const CharType *d1 = data_ptr;
	const CharType *d2 = s.data_ptr;
	size_type l1 = data_length;
	size_type l2 = s.data_length;

	if (pos > l1)
		pos = l1;
	d1 += pos;
	l1 -= pos;
	if (n < l1)
		l1 = n;

	if (pos1 > l2)
		pos1 = l2;
	d2 += pos1;
	l2 -= pos1;
	if (n1 < l2)
		l2 = n1;

	size_type l = (l1 < l2) ? l1 : l2;
	for (size_type i = 0; i < l; i++)
		if (d1[i] != d2[i])
			return (int) (((unsigned int) d1[i]) - ((unsigned int) d2[i]));
	return l1-l2;
}

template<typename CharType, typename ReferenceClass, typename StdString>
int CL_StringData<CharType, ReferenceClass, StdString>::compare(const CharType *s) const
{
	const CharType *d1 = data_ptr;
	const CharType *d2 = s;
	size_type l1 = data_length;
	size_type l2 = 0;
	while (s[l2] != 0) { l2++; }

	size_type l = (l1 < l2) ? l1 : l2;
	for (size_type i = 0; i < l; i++)
		if (d1[i] != d2[i])
			return (int) (((unsigned int) d1[i]) - ((unsigned int) d2[i]));
	return l1-l2;
}

template<typename CharType, typename ReferenceClass, typename StdString>
int CL_StringData<CharType, ReferenceClass, StdString>::compare(size_type pos, size_type n, const CharType *s, size_type len) const
{
	const CharType *d1 = data_ptr;
	const CharType *d2 = s;
	size_type l1 = data_length;
	size_type l2 = 0;
	while (s[l2] != 0) { l2++; }
	if (len != npos && l2 > len)
		l2 = len;

	if (pos > l1)
		pos = l1;
	d1 += pos;
	l1 -= pos;
	if (n != npos && n < l1)
		l1 = n;

	size_type l = (l1 < l2) ? l1 : l2;
	for (size_type i = 0; i < l; i++)
		if (d1[i] != d2[i])
			return (int) (((unsigned int) d1[i]) - ((unsigned int) d2[i]));
	return l1-l2;
}

/////////////////////////////////////////////////////////////////////////////

template <typename CharType, typename ReferenceClass, typename StdString>
bool operator==(const CL_StringData<CharType, ReferenceClass, StdString> &s1, const CL_StringData<CharType, ReferenceClass, StdString> &s2)
{
	return s1.compare(s2) == 0;
}

template <typename CharType, typename ReferenceClass, typename StdString>
bool operator==(const CharType *s1, const CL_StringData<CharType, ReferenceClass, StdString> &s2)
{
	return s2.compare(s1) == 0;
}

template <typename CharType, typename ReferenceClass, typename StdString>
bool operator==(const CL_StringData<CharType, ReferenceClass, StdString> &s1, const CharType *s2)
{
	return s1.compare(s2) == 0;
}

template <typename CharType, typename ReferenceClass, typename StdString>
bool operator!=(const CL_StringData<CharType, ReferenceClass, StdString> &s1, const CL_StringData<CharType, ReferenceClass, StdString> &s2)
{
	return s1.compare(s2) != 0;
}

template <typename CharType, typename ReferenceClass, typename StdString>
bool operator!=(const CharType *s1, const CL_StringData<CharType, ReferenceClass, StdString> &s2)
{
	return s2.compare(s1) != 0;
}

template <typename CharType, typename ReferenceClass, typename StdString>
bool operator!=(const CL_StringData<CharType, ReferenceClass, StdString> &s1, const CharType *s2)
{
	return s1.compare(s2) != 0;
}

template <typename CharType, typename ReferenceClass, typename StdString>
bool operator<(const CL_StringData<CharType, ReferenceClass, StdString> &s1, const CL_StringData<CharType, ReferenceClass, StdString> &s2)
{
	return s1.compare(s2) < 0;
}

template <typename CharType, typename ReferenceClass, typename StdString>
bool operator<(const CharType *s1, const CL_StringData<CharType, ReferenceClass, StdString> &s2)
{
	return s2.compare(s1) > 0;
}

template <typename CharType, typename ReferenceClass, typename StdString>
bool operator<(const CL_StringData<CharType, ReferenceClass, StdString> &s1, const CharType *s2)
{
	return s1.compare(s2) < 0;
}

template <typename CharType, typename ReferenceClass, typename StdString>
bool operator>(const CL_StringData<CharType, ReferenceClass, StdString> &s1, const CL_StringData<CharType, ReferenceClass, StdString> &s2)
{
	return s1.compare(s2) > 0;
}

template <typename CharType, typename ReferenceClass, typename StdString>
bool operator>(const CharType *s1, const CL_StringData<CharType, ReferenceClass, StdString> &s2)
{
	return s2.compare(s1) < 0;
}

template <typename CharType, typename ReferenceClass, typename StdString>
bool operator>(const CL_StringData<CharType, ReferenceClass, StdString> &s1, const CharType *s2)
{
	return s1.compare(s2) > 0;
}


// Explicit instantiate the versions we use:

template class CL_StringData<char, CL_StringRef8, std::string>;
template bool operator==(const CL_StringData<char, CL_StringRef8, std::string> &s1, const CL_StringData<char, CL_StringRef8, std::string> &s2);
template bool operator==(const char *s1, const CL_StringData<char, CL_StringRef8, std::string> &s2);
template bool operator==(const CL_StringData<char, CL_StringRef8, std::string> &s1, const char *s2);
template bool operator!=(const CL_StringData<char, CL_StringRef8, std::string> &s1, const CL_StringData<char, CL_StringRef8, std::string> &s2);
template bool operator!=(const char *s1, const CL_StringData<char, CL_StringRef8, std::string> &s2);
template bool operator!=(const CL_StringData<char, CL_StringRef8, std::string> &s1, const char *s2);
template bool operator<(const CL_StringData<char, CL_StringRef8, std::string> &s1, const CL_StringData<char, CL_StringRef8, std::string> &s2);
template bool operator<(const char *s1, const CL_StringData<char, CL_StringRef8, std::string> &s2);
template bool operator<(const CL_StringData<char, CL_StringRef8, std::string> &s1, const char *s2);
template bool operator>(const CL_StringData<char, CL_StringRef8, std::string> &s1, const CL_StringData<char, CL_StringRef8, std::string> &s2);
template bool operator>(const char *s1, const CL_StringData<char, CL_StringRef8, std::string> &s2);
template bool operator>(const CL_StringData<char, CL_StringRef8, std::string> &s1, const char *s2);

template class CL_StringData<wchar_t, CL_StringRef16, std::wstring>;
template bool operator==(const CL_StringData<wchar_t, CL_StringRef16, std::wstring> &s1, const CL_StringData<wchar_t, CL_StringRef16, std::wstring> &s2);
template bool operator==(const wchar_t *s1, const CL_StringData<wchar_t, CL_StringRef16, std::wstring> &s2);
template bool operator==(const CL_StringData<wchar_t, CL_StringRef16, std::wstring> &s1, const wchar_t *s2);
template bool operator!=(const CL_StringData<wchar_t, CL_StringRef16, std::wstring> &s1, const CL_StringData<wchar_t, CL_StringRef16, std::wstring> &s2);
template bool operator!=(const wchar_t *s1, const CL_StringData<wchar_t, CL_StringRef16, std::wstring> &s2);
template bool operator!=(const CL_StringData<wchar_t, CL_StringRef16, std::wstring> &s1, const wchar_t *s2);
template bool operator<(const CL_StringData<wchar_t, CL_StringRef16, std::wstring> &s1, const CL_StringData<wchar_t, CL_StringRef16, std::wstring> &s2);
template bool operator<(const wchar_t *s1, const CL_StringData<wchar_t, CL_StringRef16, std::wstring> &s2);
template bool operator<(const CL_StringData<wchar_t, CL_StringRef16, std::wstring> &s1, const wchar_t *s2);
template bool operator>(const CL_StringData<wchar_t, CL_StringRef16, std::wstring> &s1, const CL_StringData<wchar_t, CL_StringRef16, std::wstring> &s2);
template bool operator>(const wchar_t *s1, const CL_StringData<wchar_t, CL_StringRef16, std::wstring> &s2);
template bool operator>(const CL_StringData<wchar_t, CL_StringRef16, std::wstring> &s1, const wchar_t *s2);
