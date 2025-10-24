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

#include "Core/precomp.h"
#include "API/Core/Text/string_data8.h"
#include "API/Core/Text/string_ref8.h"

const CL_StringData8::size_type CL_StringData8::npos = 0xffffffff;

CL_StringData8::CL_StringData8()
: data_ptr(0), data_length(0)
{
}

CL_StringData8::CL_StringData8(const char *ptr, size_type length)
: data_ptr((char *) ptr), data_length(length)
{
}

CL_StringData8::operator std::string() const
{
	return std::string(data(), length());
}

CL_StringData8::operator CL_StringRef8() const
{
	return CL_StringRef8(data(), length(), false);
}

CL_StringData8::size_type CL_StringData8::find(const CL_StringData8 &s, size_type pos) const
{
	return find(s.data(), pos, s.length());
}

CL_StringData8::size_type CL_StringData8::find(const char *s, size_type pos, size_type n) const
{
	size_type len = data_length;
	if (pos == npos || pos == len || n == 0)
		return npos;
	const char *d = data_ptr;
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

CL_StringData8::size_type CL_StringData8::find(const char *s, size_type pos) const
{
	size_type len = 0;
	while (s[len] != 0)
		len++;
	return find(s, pos, len);
}

CL_StringData8::size_type CL_StringData8::find(char c, size_type pos) const
{
	size_type len = data_length;
	if (pos == npos || pos == len)
		return npos;
	const char *d = (const char *) data_ptr;
	for (size_type i = pos; i < len; i++)
		if (d[i] == c)
			return i;
	return npos;
}

CL_StringData8::size_type CL_StringData8::rfind(const CL_StringData8 &s, size_type pos) const
{
	return rfind(s.data(), pos, s.length());
}

CL_StringData8::size_type CL_StringData8::rfind(const char *s, size_type pos, size_type n) const
{
	size_type len = data_length;
	if (pos == npos)
		pos = len;
	if (len == 0 || n == 0)
		return npos;
	const char *d = data_ptr;
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

CL_StringData8::size_type CL_StringData8::rfind(const char *s, size_type pos) const
{
	size_type len = 0;
	while (s[len] != 0)
		len++;
	return rfind(s, pos, len);
}

CL_StringData8::size_type CL_StringData8::rfind(char c, size_type pos) const
{
	size_type len = data_length;
	if (pos == npos)
		pos = len;
	if (len == 0)
		return npos;
	if (pos >= len)
		pos = len - 1;
	const char *d = data_ptr;
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

CL_StringData8::size_type CL_StringData8::find_first_of(const CL_StringData8 &s, size_type pos) const
{
	return find_first_of(s.data(), pos, s.length());
}

CL_StringData8::size_type CL_StringData8::find_first_of(const char *s, size_type pos, size_type n) const
{
	size_type len = data_length;
	if (pos == npos || pos == len)
		return npos;
	const char *d = data_ptr;
	for (size_type i = pos; i < len; i++)
	{
		for (size_type j = 0; j < n; j++)
			if (d[i] == s[j])
				return i;
	}
	return npos;
}

CL_StringData8::size_type CL_StringData8::find_first_of(const char *s, size_type pos) const
{
	size_type len = 0;
	while (s[len] != 0)
		len++;
	return find_first_of(s, pos, len);
}

CL_StringData8::size_type CL_StringData8::find_first_of(char c, size_type pos) const
{
	size_type len = data_length;
	if (pos == npos || pos == len)
		return npos;
	const char *d = (const char *) data_ptr;
	for (size_type i = pos; i < len; i++)
		if (d[i] == c)
			return i;
	return npos;
}

CL_StringData8::size_type CL_StringData8::find_first_not_of(const CL_StringData8 &s, size_type pos) const
{
	return find_first_not_of(s.data(), pos, s.length());
}

CL_StringData8::size_type CL_StringData8::find_first_not_of(const char *s, size_type pos, size_type n) const
{
	size_type len = data_length;
	if (pos == npos || pos == len)
		return npos;
	const char *d = data_ptr;
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

CL_StringData8::size_type CL_StringData8::find_first_not_of(const char *s, size_type pos) const
{
	size_type len = 0;
	while (s[len] != 0)
		len++;
	return find_first_not_of(s, pos, len);
}

CL_StringData8::size_type CL_StringData8::find_first_not_of(char c, size_type pos) const
{
	size_type len = data_length;
	if (pos == npos || pos == len)
		return npos;
	const char *d = data_ptr;
	for (size_type i = pos; i < len; i++)
		if (d[i] != c)
			return i;
	return npos;
}

CL_StringData8::size_type CL_StringData8::find_last_of(const CL_StringData8 &s, size_type pos) const
{
	return find_last_of(s.data(), pos, s.length());
}

CL_StringData8::size_type CL_StringData8::find_last_of(const char *s, size_type pos, size_type n) const
{
	size_type len = data_length;
	if (pos > len)
		pos = len;
	const char *d = data_ptr;
	for (size_type i = pos; i != 0; i--)
	{
		for (size_type j = 0; j < n; j++)
			if (d[i-1] == s[j])
				return i-1;
	}

	return npos;
}

CL_StringData8::size_type CL_StringData8::find_last_of(const char *s, size_type pos) const
{
	size_type len = 0;
	while (s[len] != 0)
		len++;
	return find_last_of(s, pos, len);
}

CL_StringData8::size_type CL_StringData8::find_last_of(char c, size_type pos) const
{
	size_type len = data_length;
	if (pos > len)
		pos = len;
	const char *d = data_ptr;
	for (size_type i = pos; i != 0; i--)
		if (d[i-1] == c)
			return i-1;
	return npos;
}

CL_StringData8::size_type CL_StringData8::find_last_not_of(const CL_StringData8 &s, size_type pos) const
{
	return find_last_not_of(s.data(), pos, s.length());
}

CL_StringData8::size_type CL_StringData8::find_last_not_of(const char *s, size_type pos, size_type n) const
{
	size_type len = data_length;
	if (pos > len)
		pos = len;
	const char *d = data_ptr;
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

CL_StringData8::size_type CL_StringData8::find_last_not_of(const char *s, size_type pos) const
{
	size_type len = 0;
	while (s[len] != 0)
		len++;
	return find_last_not_of(s, pos, len);
}

CL_StringData8::size_type CL_StringData8::find_last_not_of(char c, size_type pos) const
{
	size_type len = data_length;
	if (pos > len)
		pos = len;
	const char *d = data_ptr;
	for (size_type i = pos; i != 0; i--)
	{
		if (d[i-1] != c)
			return i-1;
	}
	return npos;
}

CL_StringRef8 CL_StringData8::substr(size_type pos, size_type n) const
{
	const char *d = data_ptr;
	if (pos == npos || pos > data_length)
		return CL_StringRef8(d + data_length, 0, false);
	if (n == npos || n > data_length-pos)
		n = data_length - pos;
	return CL_StringRef8(d + pos, n, false);
}

int CL_StringData8::compare(const CL_StringData8 &s) const
{
	const char *d1 = data_ptr;
	const char *d2 = s.data_ptr;
	size_type l1 = data_length;
	size_type l2 = s.data_length;
	size_type l = (l1 < l2) ? l1 : l2;
	for (size_type i = 0; i < l; i++)
		if (d1[i] != d2[i])
			return (int) (((unsigned int) d1[i]) - ((unsigned int) d2[i]));
	return l1-l2;
}

int CL_StringData8::compare(size_type pos, size_type n, const CL_StringData8 &s) const
{
	const char *d1 = data_ptr;
	const char *d2 = s.data_ptr;
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

int CL_StringData8::compare(size_type pos, size_type n, const CL_StringData8 &s, size_type pos1, size_type n1) const
{
	const char *d1 = data_ptr;
	const char *d2 = s.data_ptr;
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

int CL_StringData8::compare(const char *s) const
{
	const char *d1 = data_ptr;
	const char *d2 = s;
	size_type l1 = data_length;
	size_type l2 = 0;
	while (s[l2] != 0) { l2++; }

	size_type l = (l1 < l2) ? l1 : l2;
	for (size_type i = 0; i < l; i++)
		if (d1[i] != d2[i])
			return (int) (((unsigned int) d1[i]) - ((unsigned int) d2[i]));
	return l1-l2;
}

int CL_StringData8::compare(size_type pos, size_type n, const char *s, size_type len) const
{
	const char *d1 = data_ptr;
	const char *d2 = s;
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

bool operator==(const CL_StringData8 &s1, const CL_StringData8 &s2)
{
	return s1.compare(s2) == 0;
}

bool operator==(const char *s1, const CL_StringData8 &s2)
{
	return s2.compare(s1) == 0;
}

bool operator==(const CL_StringData8 &s1, const char *s2)
{
	return s1.compare(s2) == 0;
}

bool operator!=(const CL_StringData8 &s1, const CL_StringData8 &s2)
{
	return s1.compare(s2) != 0;
}

bool operator!=(const char *s1, const CL_StringData8 &s2)
{
	return s2.compare(s1) != 0;
}

bool operator!=(const CL_StringData8 &s1, const char *s2)
{
	return s1.compare(s2) != 0;
}

bool operator<(const CL_StringData8 &s1, const CL_StringData8 &s2)
{
	return s1.compare(s2) < 0;
}

bool operator<(const char *s1, const CL_StringData8 &s2)
{
	return s2.compare(s1) > 0;
}

bool operator<(const CL_StringData8 &s1, const char *s2)
{
	return s1.compare(s2) < 0;
}

bool operator>(const CL_StringData8 &s1, const CL_StringData8 &s2)
{
	return s1.compare(s2) > 0;
}

bool operator>(const char *s1, const CL_StringData8 &s2)
{
	return s2.compare(s1) < 0;
}

bool operator>(const CL_StringData8 &s1, const char *s2)
{
	return s1.compare(s2) > 0;
}
