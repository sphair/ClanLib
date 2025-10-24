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
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/logger.h"
#include "API/Core/System/exception.h"
#ifndef WIN32
#include <wchar.h>
#include <wctype.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#else
#include <cstring>
#include <tchar.h>
#endif

// This function or variable may be unsafe. Consider using xxxx instead.
// To disable deprecation, use _CRT_SECURE_NO_DEPRECATE. See online help for details.
#ifdef WIN32
#pragma warning(disable: 4996)
#endif
// We disable this warning because the usage patterns we use here are safe,
// and although we could switch to newer more 'safe' versions, this may break
// compilation in earlier versions of Visual Studio. -- mbn 13. sep 2006


#ifndef WIN32
#ifndef HAVE_WCSCASECMP
int	cl_wcscasecmp(const wchar_t *, const wchar_t *);
#define wcscasecmp cl_wcscasecmp
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_StringHelp Operations:

std::vector<CL_String> CL_StringHelp::split_text(
	const CL_StringRef &text,
	const CL_StringRef &split_string,
	bool skip_empty)
{
	std::vector<CL_String> result;
	CL_String::size_type end_pos = 0, begin_pos = 0;
	while (true)
	{
		end_pos = text.find(split_string, begin_pos);
		if (end_pos == CL_String::npos)
		{
			if (begin_pos != text.length())
				result.push_back(text.substr(begin_pos));
			break;
		}
		else
		{
			if (!skip_empty || begin_pos != end_pos)
				result.push_back(text.substr(begin_pos, end_pos-begin_pos));
			begin_pos = end_pos + split_string.length();
		}
	}
	return result;
}

CL_String CL_StringHelp::trim(const CL_StringRef &text)
{
	CL_String::size_type first_char = text.find_first_not_of(" \r\n\t");
	CL_String::size_type last_char = text.find_last_not_of(" \r\n\t");
	if (first_char == CL_String::npos)
		return CL_String();
	if (last_char == CL_String::npos)
		return CL_String();
	return text.substr(first_char, last_char - first_char + 1);
}

int CL_StringHelp::compare(const CL_StringRef8 &a, const CL_StringRef8 &b, bool case_insensitive)
{
#ifdef WIN32
	if (case_insensitive)
		return stricmp(a.c_str(), b.c_str());
	else
		return strcmp(a.c_str(), b.c_str());
#else
	if (case_insensitive)
		return strcasecmp(a.c_str(), b.c_str());
	else
		return strcmp(a.c_str(), b.c_str());
#endif
}

int CL_StringHelp::compare(const CL_StringRef16 &a, const CL_StringRef16 &b, bool case_insensitive)
{
#ifdef WIN32
	if (case_insensitive)
		return wcsicmp(a.c_str(), b.c_str());
	else
		return wcscmp(a.c_str(), b.c_str());
#else
	if (case_insensitive)
		return wcscasecmp(a.c_str(), b.c_str());
	else
		return wcscmp(a.c_str(), b.c_str());
#endif
}

CL_String CL_StringHelp::text_to_upper(const CL_StringRef &s)
{
	return local8_to_upper(s);
}
	
CL_String8 CL_StringHelp::wchar_to_utf8(wchar_t value)
{
	char text[8];

	if ( (value < 0x80) && (value > 0) )
	{
		text[0] = (char) value;
		text[1] = 0;
	}
	else if( value < 0x800 )
	{
		text[0] = (char) (  0xc0 | ( value >> 6 ));
		text[1] = (char) (  0x80 | ( value & 0x3f ));
		text[2] = 0;
	}
	else if( value < 0x10000 )
	{
		text[0] = (char) (  0xe0 | ( value >> 12 ));
		text[1] = (char) (  0x80 | ( (value >> 6) & 0x3f ));
		text[2] = (char) (  0x80 | ( value & 0x3f ));
		text[3] = 0;
	
	}
	else if( value < 0x200000 )
	{
#ifndef WIN32
		text[0] = (char) (  0xf0 | ( value >> 18 ));
#endif
		text[1] = (char) (  0x80 | ( (value >> 12) & 0x3f ));
		text[2] = (char) (  0x80 | ( (value >> 6) & 0x3f ));
		text[3] = (char) (  0x80 | ( value & 0x3f ));
		text[4] = 0;

	}
	else if( value < 0x4000000 )
	{
#ifndef WIN32
		text[0] = (char) (  0xf8 | ( value >> 24 ));
		text[1] = (char) (  0x80 | ( (value >> 18) & 0x3f ));
#endif
		text[2] = (char) (  0x80 | ( (value >> 12) & 0x3f ));
		text[3] = (char) (  0x80 | ( (value >> 6) & 0x3f ));
		text[4] = (char) (  0x80 | ( value & 0x3f ));
		text[5] = 0;

	}
	else if( value < 0x80000000 )
	{
#ifndef WIN32
		text[0] = (char) (  0xfc | ( value >> 30 ));
		text[1] = (char) (  0x80 | ( (value >> 24) & 0x3f ));
		text[2] = (char) (  0x80 | ( (value >> 18) & 0x3f ));
#endif
		text[3] = (char) (  0x80 | ( (value >> 12) & 0x3f ));
		text[4] = (char) (  0x80 | ( (value >> 6) & 0x3f ));
		text[5] = (char) (  0x80 | ( value & 0x3f ));
		text[6] = 0;
	}
	else
	{
		text[0] = 0;	// Invalid wchar value
	}
	return text;
}

CL_String8 CL_StringHelp::local8_to_upper(const CL_StringRef8 &s)
{
	CL_String8 result = s;
	CL_String8::size_type index, size;
	size = result.length();
	for (index = 0; index < size; index++)
	{
		result[index] = (unsigned char) toupper((unsigned char) result[index]);
	}
	return result;
}

CL_String16 CL_StringHelp::ucs2_to_upper(const CL_StringRef16 &s)
{
	CL_String16 result = s;
	CL_String16::size_type index, size;
	size = result.length();
	for (index = 0; index < size; index++)
	{
		result[index] = towupper(result[index]);
	}
	return result;
}
	
CL_String CL_StringHelp::text_to_lower(const CL_StringRef &s)
{
	return local8_to_lower(s);
}
	
CL_String8 CL_StringHelp::local8_to_lower(const CL_StringRef8 &s)
{
	CL_String8 result = s;
	CL_String8::size_type index, size;
	size = result.length();
	for (index = 0; index < size; index++)
	{
		result[index] = (unsigned char) tolower((unsigned char) result[index]);
	}
	return result;
}
	
CL_String16 CL_StringHelp::ucs2_to_lower(const CL_StringRef16 &s)
{
	CL_String16 result = s;
	CL_String16::size_type index, size;
	size = result.length();
	for (index = 0; index < size; index++)
	{
		result[index] = towlower(result[index]);
	}
	return result;
}

CL_String CL_StringHelp::float_to_text(float value, int num_decimal_places)
{
	return float_to_local8(value, num_decimal_places);
}

CL_String8 CL_StringHelp::float_to_local8(float value, int num_decimals)
{
	char buf[64];
	memset(buf, 0, 64);
#ifdef WIN32
	_snprintf(buf, 63, ("%." + CL_StringHelp::int_to_local8(num_decimals) + "f").c_str(), value);
#else
	snprintf(buf, 63, ("%." + CL_StringHelp::int_to_local8(num_decimals) + "f").c_str(), value);
#endif
	return CL_String8(buf);
}
	
CL_String16 CL_StringHelp::float_to_ucs2(float value, int num_decimals)
{
#ifdef WIN32
	WCHAR buf[64];
	memset(buf, 0, 64 * sizeof(WCHAR));
	swprintf(buf, ("%." + CL_StringHelp::int_to_ucs2(num_decimals) + "f").c_str(), value);
	return CL_String16(buf);
#else
	wchar_t buf[64];
	memset(buf, 0, 64 * sizeof(wchar_t));
	swprintf(buf, 63, ("%." + CL_StringHelp::int_to_ucs2(num_decimals) + "f").c_str(), value);
	return CL_String16(buf);
#endif
}

float CL_StringHelp::text_to_float(const CL_StringRef &value)
{
	return local8_to_float(value);
}
	
float CL_StringHelp::local8_to_float(const CL_StringRef8 &value)
{
	float result = 0.0;
	sscanf(value.c_str(), "%f", &result);
	return result;
}
	
float CL_StringHelp::ucs2_to_float(const CL_StringRef16 &value)
{
	float result = 0.0;
	swscanf(value.c_str(), L"%f", &result);
	return result;
}

CL_String CL_StringHelp::double_to_text(double value, int num_decimals)
{
	return double_to_local8(value, num_decimals);
}

CL_String8 CL_StringHelp::double_to_local8(double value, int num_decimals)
{
	char buf[64];
	memset(buf, 0, 64);
#ifdef WIN32
	_snprintf(buf, 63, ("%." + CL_StringHelp::int_to_local8(num_decimals) + "f").c_str(), value);
#else
	snprintf(buf, 63, ("%." + CL_StringHelp::int_to_local8(num_decimals) + "f").c_str(), value);
#endif
	return CL_String8(buf);
}
	
CL_String16 CL_StringHelp::double_to_ucs2(double value, int num_decimals)
{
#ifdef WIN32
	WCHAR buf[64];
	memset(buf, 0, 64 * sizeof(WCHAR));
	swprintf(buf, ("%." + CL_StringHelp::int_to_ucs2(num_decimals) + "f").c_str(), value);
	return CL_String16(buf);
#else
	wchar_t buf[64];
	memset(buf, 0, 64 * sizeof(wchar_t));
	swprintf(buf, 63, ("%." + CL_StringHelp::int_to_ucs2(num_decimals) + "f").c_str(), value);
	return CL_String16(buf);
#endif
}
	
double CL_StringHelp::text_to_double(const CL_StringRef &value)
{
	return local8_to_double(value);
}
	
double CL_StringHelp::local8_to_double(const CL_StringRef8 &value)
{
	double result = 0.0;
	sscanf(value.c_str(), "%lf", &result);
	return result;
}
	
double CL_StringHelp::ucs2_to_double(const CL_StringRef16 &value)
{
	double result = 0.0;
	swscanf(value.c_str(), L"%lf", &result);
	return result;
}

CL_String CL_StringHelp::int_to_text(int value)
{
	return int_to_local8(value);
}

CL_String8 CL_StringHelp::int_to_local8(int value)
{
	char buf[32];
	memset(buf, 0, 32);
#ifdef WIN32
	_snprintf(buf, 31, "%d", value);
#else
	snprintf(buf, 31, "%d", value);
#endif
	return CL_String8(buf);
}
	
CL_String16 CL_StringHelp::int_to_ucs2(int value)
{
#ifdef WIN32
	WCHAR buf[32];
	memset(buf, 0, 32 * sizeof(WCHAR));
	swprintf(buf, L"%d", value);
	return CL_String16(buf);
#else
	wchar_t buf[32];
	memset(buf, 0, 32 * sizeof(wchar_t));
	swprintf(buf, 31, L"%d", value);
	return CL_String16(buf);
#endif
}

int CL_StringHelp::text_to_int(const CL_StringRef &value, int base)
{
	return local8_to_int(value, base);
}
	
int CL_StringHelp::local8_to_int(const CL_StringRef8 &value, int base)
{
	if (base == 10)
	{
		return atoi(value.c_str());
	}
	else if (base == 16)
	{
		int result = 0;
		CL_String8::size_type i, length;
		length = value.length();
		for (i = 0; i < length; i++)
		{
			if (value[i] >= '0' && value[i] <= '9')
			{
				result = (result << 4) + (value[i] - '0');
			}
			else if (value[i] >= 'a' && value[i] <= 'f')
			{
				result = (result << 4) + 10 + (value[i] - 'a');
			}
			else if (value[i] >= 'A' && value[i] <= 'F')
			{
				result = (result << 4) + 10 + (value[i] - 'A');
			}
			else
			{
				break;
			}
		}
		return result;
	}
	else
	{
		throw CL_Exception("Unsupported base passed for local8_to_int");
	}
}
	
int CL_StringHelp::ucs2_to_int(const CL_StringRef16 &value, int base)
{
	if (base == 10)
	{
#ifdef WIN32
		return _wtoi(value.c_str());
#else
		throw CL_Exception("ucs2_to_int not implemented on unix yet");
		return 0;
#endif
	}
	else if (base == 16)
	{
		int result = 0;
		CL_String16::size_type i, length;
		length = value.length();
		for (i = 0; i < length; i++)
		{
			if (value[i] >= L'0' && value[i] <= L'9')
			{
				result = (result << 4) + (value[i] - L'0');
			}
			else if (value[i] >= L'a' && value[i] <= L'f')
			{
				result = (result << 4) + 10 + (value[i] - L'a');
			}
			else if (value[i] >= L'A' && value[i] <= L'F')
			{
				result = (result << 4) + 10 + (value[i] - L'A');
			}
			else
			{
				break;
			}
		}
		return result;
	}
	else
	{
		throw CL_Exception("Unsupported base passed for ucs2_to_int");
	}
}

CL_String CL_StringHelp::uint_to_text(unsigned int value)
{
	return uint_to_local8(value);
}

CL_String8 CL_StringHelp::uint_to_local8(unsigned int value)
{
	char buf[32];
	memset(buf, 0, 32);
#ifdef WIN32
	_snprintf(buf, 31, "%u", value);
#else
	snprintf(buf, 31, "%u", value);
#endif
	return CL_String8(buf);
}
	
CL_String16 CL_StringHelp::uint_to_ucs2(unsigned int value)
{
#ifdef WIN32
	WCHAR buf[32];
	memset(buf, 0, 32 * sizeof(WCHAR));
	swprintf(buf, L"%u", value);
	return CL_String16(buf);
#else
	wchar_t buf[32];
	memset(buf, 0, 32 * sizeof(wchar_t));
	swprintf(buf, 31, L"%u", value);
	return CL_String16(buf);
#endif
}

unsigned int CL_StringHelp::text_to_uint(const CL_StringRef &value, int base)
{
	return local8_to_uint(value, base);
}
	
unsigned int CL_StringHelp::local8_to_uint(const CL_StringRef8 &value, int base)
{
	if (base == 10)
	{
		return (unsigned int) atoi(value.c_str());
	}
	else if (base == 16)
	{
		int result = 0;
		CL_String8::size_type i, length;
		length = value.length();
		for (i = 0; i < length; i++)
		{
			if (value[i] >= '0' && value[i] <= '9')
			{
				result = (result << 4) + (value[i] - '0');
			}
			else if (value[i] >= 'a' && value[i] <= 'f')
			{
				result = (result << 4) + 10 + (value[i] - 'a');
			}
			else if (value[i] >= 'A' && value[i] <= 'F')
			{
				result = (result << 4) + 10 + (value[i] - 'A');
			}
			else
			{
				break;
			}
		}
		return result;
	}
	else
	{
		throw CL_Exception("Unsupported base passed for local8_to_int");
	}
}
	
unsigned int CL_StringHelp::ucs2_to_uint(const CL_StringRef16 &value, int base)
{
	if (base == 10)
	{
#ifdef WIN32
		return (unsigned int) _wtoi(value.c_str());
#else
		throw CL_Exception("ucs2_to_uint not implemented on unix yet");
		return 0;
#endif
	}
	else if (base == 16)
	{
		unsigned int result = 0;
		CL_String16::size_type i, length;
		length = value.length();
		for (i = 0; i < length; i++)
		{
			if (value[i] >= L'0' && value[i] <= L'9')
			{
				result = (result << 4) + (value[i] - L'0');
			}
			else if (value[i] >= L'a' && value[i] <= L'f')
			{
				result = (result << 4) + 10 + (value[i] - L'a');
			}
			else if (value[i] >= L'A' && value[i] <= L'F')
			{
				result = (result << 4) + 10 + (value[i] - L'A');
			}
			else
			{
				break;
			}
		}
		return result;
	}
	else
	{
		throw CL_Exception("Unsupported base passed for ucs2_to_int");
	}
}

CL_String CL_StringHelp::bool_to_text(bool value)
{
	return bool_to_local8(value);
}

CL_String8 CL_StringHelp::bool_to_local8(bool value)
{
	return value ? "true" : "false";
}

CL_String16 CL_StringHelp::bool_to_ucs2(bool value)
{
	return value ? L"true" : L"false";
}

bool CL_StringHelp::text_to_bool(const CL_StringRef &value)
{
	return local8_to_bool(value);
}

bool CL_StringHelp::local8_to_bool(const CL_StringRef8 &value)
{
	if (CL_StringHelp::compare(value, "true", true) == 0)
		return true;
	else if (CL_StringHelp::compare(value, "yes", true) == 0)
		return true;
	else if (value == "1")
		return true;
	else
		return false;
}

bool CL_StringHelp::ucs2_to_bool(const CL_StringRef16 &value)
{
	if (CL_StringHelp::compare(value, L"true", true) == 0)
		return true;
	else if (CL_StringHelp::compare(value, L"yes", true) == 0)
		return true;
	else if (value == L"1")
		return true;
	else
		return false;
}

CL_String8 CL_StringHelp::text_to_local8(const CL_StringRef &text)
{
	return text;
}

CL_String8 CL_StringHelp::text_to_utf8(const CL_StringRef &text)
{
	return text;
}

CL_String8 CL_StringHelp::ucs2_to_latin1(const CL_StringRef16 &ucs2)
{
	CL_String8::size_type i, length = ucs2.length();
	CL_String8 latin1(length, ' ');
	for (i=0; i<length; i++)
		latin1[i] = (char) ucs2[i];
	return latin1;
}

CL_String8 CL_StringHelp::ucs2_to_latin9(const CL_StringRef16 &ucs2)
{
	CL_String8::size_type i, length = ucs2.length();
	CL_String8 latin1(length, ' ');
	for (i=0; i<length; i++)
		latin1[i] = (ucs2[i] != 0x20ac) ? ucs2[i] : 0xa4;
	return latin1;
}

CL_String8 CL_StringHelp::ucs2_to_local8(const CL_StringRef16 &ucs2)
{
	return ucs2_to_latin9(ucs2);
}

CL_String8 CL_StringHelp::ucs2_to_utf8(const CL_StringRef16 &ucs2)
{
	// Calculate length:

	CL_String16::size_type length_ucs2 = ucs2.length();
	CL_String8::size_type length_utf8 = 0;
	CL_String16::size_type pos;
	for (pos = 0; pos < length_ucs2; pos++)
	{
		if (ucs2[pos] < 0x0080)
			length_utf8++;
		else if (ucs2[pos] < 0x0800)
			length_utf8 += 2;
		else
			length_utf8 += 3;
	}
	
	// Perform conversion:
	
	CL_String8 utf8(length_utf8, ' ');
	CL_String8::size_type pos_utf8 = 0;
	for (pos = 0; pos < length_ucs2; pos++)
	{
		if (ucs2[pos] < 0x0080)
		{
			utf8[pos_utf8++] = (char) ucs2[pos];
		}
		else if (ucs2[pos] < 0x0800)
		{
			utf8[pos_utf8++] = 0xc0 + (ucs2[pos] >> 6);
			utf8[pos_utf8++] = 0x80 + (ucs2[pos] & 0x3f);
		}
		else
		{
			utf8[pos_utf8++] = 0xe0 + (ucs2[pos] >> 12);
			utf8[pos_utf8++] = 0x80 + ((ucs2[pos] >> 6) & 0x3f);
			utf8[pos_utf8++] = 0x80 + (ucs2[pos] & 0x3f);
		}
	}

	return utf8;
}

CL_String16 CL_StringHelp::latin1_to_ucs2(const CL_StringRef8 &latin1)
{
	CL_String16::size_type i, length = latin1.length();
	CL_String16 ucs2(length, ' ');
	for (i=0; i<length; i++)
		ucs2[i] = latin1[i];
	return ucs2;
}

CL_String16 CL_StringHelp::latin9_to_ucs2(const CL_StringRef8 &latin9)
{
	CL_String16::size_type i, length = latin9.length();
	CL_String16 ucs2(length, ' ');
	for (i=0; i<length; i++)
		ucs2[i] = ((unsigned char) latin9[i] != 0xa4) ? latin9[i] : 0x20ac;
	return ucs2;
}

CL_String CL_StringHelp::local8_to_text(const CL_StringRef8 &local8)
{
	return local8;
}

CL_String CL_StringHelp::ucs2_to_text(const CL_StringRef16 &ucs2)
{
	return ucs2_to_utf8(ucs2);
}

CL_String16 CL_StringHelp::local8_to_ucs2(const CL_StringRef8 &local8)
{
	return latin9_to_ucs2(local8);
}

CL_String16 CL_StringHelp::utf8_to_ucs2(const CL_StringRef8 &utf8)
{
	// Calculate length:

	CL_String16::size_type length_ucs2 = 0;
	CL_String8::size_type length_utf8 = utf8.length();
	CL_String8::size_type pos = 0;
	while (pos < length_utf8)
	{
		unsigned char c = utf8[pos++];
		int trailing_bytes = trailing_bytes_for_utf8[c];
		length_ucs2++;
		pos += trailing_bytes;
	}
	if (pos != length_utf8)
	{
		// error in utf8 string
		length_ucs2--;
	}
	if (pos <= 0)
		return CL_String16();

	// Perform conversion:
	
	CL_String16 ucs2(length_ucs2, L'?');
	pos = 0;
	CL_String16::size_type ucs2_pos = 0;
	while (pos < length_utf8 && ucs2_pos < length_ucs2)
	{
		unsigned char c = utf8[pos++];
		int trailing_bytes = trailing_bytes_for_utf8[c];
		unsigned int ucs4 = (c & bitmask_leadbyte_for_utf8[trailing_bytes]);
		for (int i=0; i<trailing_bytes; i++)
		{
			c = (unsigned char) utf8[pos+i];
			if (c < 0xc0)
			{
				ucs4 = (ucs4 << 6) + (c & 0x3f);
			}
			else
			{
				// error in utf8 string
				ucs4 = L'?';
				break;
			}
		}
		if (ucs4 > 0 && ucs4 <= 0xffff)
			ucs2[ucs2_pos] = ucs4;
		else
			ucs2[ucs2_pos] = L'?';

		ucs2_pos++;
		pos += trailing_bytes;
	}
	return ucs2;
}

CL_String CL_StringHelp::utf8_to_text(const CL_StringRef8 &utf8)
{
	return utf8;
}

CL_String8 CL_StringHelp::text_to_cp437(const CL_StringRef &text)
{
	return ucs2_to_cp437(local8_to_ucs2(text));
}

CL_String CL_StringHelp::cp437_to_text(const CL_StringRef8 &cp437)
{
	return ucs2_to_utf8(cp437_to_ucs2(cp437));
}

CL_String16 CL_StringHelp::cp437_to_ucs2(const CL_StringRef8 &cp437)
{
	static CL_String16::char_type cp437_charset[] =
	{
		0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007,
		0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E, 0x000F,
		0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017,
		0x0018, 0x0019, 0x001A, 0x001B, 0x001C, 0x001D, 0x001E, 0x001F,
		0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027,
		0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F,
		0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037,
		0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F,
		0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047,
		0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F,
		0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057,
		0x0058, 0x0059, 0x005A, 0x005B, 0x005C, 0x005D, 0x005E, 0x005F,
		0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067,
		0x0068, 0x0069, 0x006A, 0x006B, 0x006C, 0x006D, 0x006E, 0x006F,
		0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077,
		0x0078, 0x0079, 0x007A, 0x007B, 0x007C, 0x007D, 0x007E, 0x007F,
		0x00C7, 0x00FC, 0x00E9, 0x00E2, 0x00E4, 0x00E0, 0x00E5, 0x00E7,
		0x00EA, 0x00EB, 0x00E8, 0x00EF, 0x00EE, 0x00EC, 0x00C4, 0x00C5,
		0x00C9, 0x00E6, 0x00C6, 0x00F4, 0x00F6, 0x00F2, 0x00FB, 0x00F9,
		0x00FF, 0x00D6, 0x00DC, 0x00A2, 0x00A3, 0x00A5, 0x20A7, 0x0192,
		0x00E1, 0x00ED, 0x00F3, 0x00FA, 0x00F1, 0x00D1, 0x00AA, 0x00BA,
		0x00BF, 0x2310, 0x00AC, 0x00BD, 0x00BC, 0x00A1, 0x00AB, 0x00BB,
		0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556,
		0x2555, 0x2563, 0x2551, 0x2557, 0x255D, 0x255C, 0x255B, 0x2510,
		0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x255E, 0x255F,
		0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x2567,
		0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256B,
		0x256A, 0x2518, 0x250C, 0x2588, 0x2584, 0x258C, 0x2590, 0x2580,
		0x03B1, 0x00DF, 0x0393, 0x03C0, 0x03A3, 0x03C3, 0x00B5, 0x03C4,
		0x03A6, 0x0398, 0x03A9, 0x03B4, 0x221E, 0x03C6, 0x03B5, 0x2229,
		0x2261, 0x00B1, 0x2265, 0x2264, 0x2320, 0x2321, 0x00F7, 0x2248,
		0x00B0, 0x2219, 0x00B7, 0x221A, 0x207F, 0x00B2, 0x25A0, 0x00A0
	};

	CL_String16 str16;
	str16.resize(cp437.length());
	const unsigned char *input = (const unsigned char *) cp437.data();
	CL_String16::char_type *output = str16.data();
	CL_String16::size_type index, size;
	size = cp437.length();
	for (index = 0; index < size; index++)
		output[index] = cp437_charset[input[index]];
	return str16;
}

CL_String8 CL_StringHelp::ucs2_to_cp437(const CL_StringRef16 &text)
{
	CL_String8 str8;
	str8.resize(text.length());
	unsigned char *output = (unsigned char *) str8.data();
	const CL_String16::char_type *input = text.data();
	CL_String16::size_type index, size;
	size = text.length();
	for (index = 0; index < size; index++)
	{
		if (input[index] >= 0x00 && input[index] <= 0x7f)
		{
			str8[index] = text[index];
			continue;
		}

		if (input[index] == 0x00C7) { output[index] = 0x80; continue; }
		if (input[index] == 0x00FC) { output[index] = 0x81; continue; }
		if (input[index] == 0x00E9) { output[index] = 0x82; continue; }
		if (input[index] == 0x00E2) { output[index] = 0x83; continue; }
		if (input[index] == 0x00E4) { output[index] = 0x84; continue; }
		if (input[index] == 0x00E0) { output[index] = 0x85; continue; }
		if (input[index] == 0x00E5) { output[index] = 0x86; continue; }
		if (input[index] == 0x00E7) { output[index] = 0x87; continue; }
		if (input[index] == 0x00EA) { output[index] = 0x88; continue; }
		if (input[index] == 0x00EB) { output[index] = 0x89; continue; }
		if (input[index] == 0x00E8) { output[index] = 0x8A; continue; }
		if (input[index] == 0x00EF) { output[index] = 0x8B; continue; }
		if (input[index] == 0x00EE) { output[index] = 0x8C; continue; }
		if (input[index] == 0x00EC) { output[index] = 0x8D; continue; }
		if (input[index] == 0x00C4) { output[index] = 0x8E; continue; }
		if (input[index] == 0x00C5) { output[index] = 0x8F; continue; }
		if (input[index] == 0x00C9) { output[index] = 0x90; continue; }
		if (input[index] == 0x00E6) { output[index] = 0x91; continue; }
		if (input[index] == 0x00C6) { output[index] = 0x92; continue; }
		if (input[index] == 0x00F4) { output[index] = 0x93; continue; }
		if (input[index] == 0x00F6) { output[index] = 0x94; continue; }
		if (input[index] == 0x00F2) { output[index] = 0x95; continue; }
		if (input[index] == 0x00FB) { output[index] = 0x96; continue; }
		if (input[index] == 0x00F9) { output[index] = 0x97; continue; }
		if (input[index] == 0x00FF) { output[index] = 0x98; continue; }
		if (input[index] == 0x00D6) { output[index] = 0x99; continue; }
		if (input[index] == 0x00DC) { output[index] = 0x9A; continue; }
		if (input[index] == 0x00A2) { output[index] = 0x9B; continue; }
		if (input[index] == 0x00A3) { output[index] = 0x9C; continue; }
		if (input[index] == 0x00A5) { output[index] = 0x9D; continue; }
		if (input[index] == 0x20A7) { output[index] = 0x9E; continue; }
		if (input[index] == 0x0192) { output[index] = 0x9F; continue; }
		if (input[index] == 0x00E1) { output[index] = 0xA0; continue; }
		if (input[index] == 0x00ED) { output[index] = 0xA1; continue; }
		if (input[index] == 0x00F3) { output[index] = 0xA2; continue; }
		if (input[index] == 0x00FA) { output[index] = 0xA3; continue; }
		if (input[index] == 0x00F1) { output[index] = 0xA4; continue; }
		if (input[index] == 0x00D1) { output[index] = 0xA5; continue; }
		if (input[index] == 0x00AA) { output[index] = 0xA6; continue; }
		if (input[index] == 0x00BA) { output[index] = 0xA7; continue; }
		if (input[index] == 0x00BF) { output[index] = 0xA8; continue; }
		if (input[index] == 0x2310) { output[index] = 0xA9; continue; }
		if (input[index] == 0x00AC) { output[index] = 0xAA; continue; }
		if (input[index] == 0x00BD) { output[index] = 0xAB; continue; }
		if (input[index] == 0x00BC) { output[index] = 0xAC; continue; }
		if (input[index] == 0x00A1) { output[index] = 0xAD; continue; }
		if (input[index] == 0x00AB) { output[index] = 0xAE; continue; }
		if (input[index] == 0x00BB) { output[index] = 0xAF; continue; }
		if (input[index] == 0x2591) { output[index] = 0xB0; continue; }
		if (input[index] == 0x2592) { output[index] = 0xB1; continue; }
		if (input[index] == 0x2593) { output[index] = 0xB2; continue; }
		if (input[index] == 0x2502) { output[index] = 0xB3; continue; }
		if (input[index] == 0x2524) { output[index] = 0xB4; continue; }
		if (input[index] == 0x2561) { output[index] = 0xB5; continue; }
		if (input[index] == 0x2562) { output[index] = 0xB6; continue; }
		if (input[index] == 0x2556) { output[index] = 0xB7; continue; }
		if (input[index] == 0x2555) { output[index] = 0xB8; continue; }
		if (input[index] == 0x2563) { output[index] = 0xB9; continue; }
		if (input[index] == 0x2551) { output[index] = 0xBA; continue; }
		if (input[index] == 0x2557) { output[index] = 0xBB; continue; }
		if (input[index] == 0x255D) { output[index] = 0xBC; continue; }
		if (input[index] == 0x255C) { output[index] = 0xBD; continue; }
		if (input[index] == 0x255B) { output[index] = 0xBE; continue; }
		if (input[index] == 0x2510) { output[index] = 0xBF; continue; }
		if (input[index] == 0x2514) { output[index] = 0xC0; continue; }
		if (input[index] == 0x2534) { output[index] = 0xC1; continue; }
		if (input[index] == 0x252C) { output[index] = 0xC2; continue; }
		if (input[index] == 0x251C) { output[index] = 0xC3; continue; }
		if (input[index] == 0x2500) { output[index] = 0xC4; continue; }
		if (input[index] == 0x253C) { output[index] = 0xC5; continue; }
		if (input[index] == 0x255E) { output[index] = 0xC6; continue; }
		if (input[index] == 0x255F) { output[index] = 0xC7; continue; }
		if (input[index] == 0x255A) { output[index] = 0xC8; continue; }
		if (input[index] == 0x2554) { output[index] = 0xC9; continue; }
		if (input[index] == 0x2569) { output[index] = 0xCA; continue; }
		if (input[index] == 0x2566) { output[index] = 0xCB; continue; }
		if (input[index] == 0x2560) { output[index] = 0xCC; continue; }
		if (input[index] == 0x2550) { output[index] = 0xCD; continue; }
		if (input[index] == 0x256C) { output[index] = 0xCE; continue; }
		if (input[index] == 0x2567) { output[index] = 0xCF; continue; }
		if (input[index] == 0x2568) { output[index] = 0xD0; continue; }
		if (input[index] == 0x2564) { output[index] = 0xD1; continue; }
		if (input[index] == 0x2565) { output[index] = 0xD2; continue; }
		if (input[index] == 0x2559) { output[index] = 0xD3; continue; }
		if (input[index] == 0x2558) { output[index] = 0xD4; continue; }
		if (input[index] == 0x2552) { output[index] = 0xD5; continue; }
		if (input[index] == 0x2553) { output[index] = 0xD6; continue; }
		if (input[index] == 0x256B) { output[index] = 0xD7; continue; }
		if (input[index] == 0x256A) { output[index] = 0xD8; continue; }
		if (input[index] == 0x2518) { output[index] = 0xD9; continue; }
		if (input[index] == 0x250C) { output[index] = 0xDA; continue; }
		if (input[index] == 0x2588) { output[index] = 0xDB; continue; }
		if (input[index] == 0x2584) { output[index] = 0xDC; continue; }
		if (input[index] == 0x258C) { output[index] = 0xDD; continue; }
		if (input[index] == 0x2590) { output[index] = 0xDE; continue; }
		if (input[index] == 0x2580) { output[index] = 0xDF; continue; }
		if (input[index] == 0x03B1) { output[index] = 0xE0; continue; }
		if (input[index] == 0x00DF) { output[index] = 0xE1; continue; }
		if (input[index] == 0x0393) { output[index] = 0xE2; continue; }
		if (input[index] == 0x03C0) { output[index] = 0xE3; continue; }
		if (input[index] == 0x03A3) { output[index] = 0xE4; continue; }
		if (input[index] == 0x03C3) { output[index] = 0xE5; continue; }
		if (input[index] == 0x00B5) { output[index] = 0xE6; continue; }
		if (input[index] == 0x03C4) { output[index] = 0xE7; continue; }
		if (input[index] == 0x03A6) { output[index] = 0xE8; continue; }
		if (input[index] == 0x0398) { output[index] = 0xE9; continue; }
		if (input[index] == 0x03A9) { output[index] = 0xEA; continue; }
		if (input[index] == 0x03B4) { output[index] = 0xEB; continue; }
		if (input[index] == 0x221E) { output[index] = 0xEC; continue; }
		if (input[index] == 0x03C6) { output[index] = 0xED; continue; }
		if (input[index] == 0x03B5) { output[index] = 0xEE; continue; }
		if (input[index] == 0x2229) { output[index] = 0xEF; continue; }
		if (input[index] == 0x2261) { output[index] = 0xF0; continue; }
		if (input[index] == 0x00B1) { output[index] = 0xF1; continue; }
		if (input[index] == 0x2265) { output[index] = 0xF2; continue; }
		if (input[index] == 0x2264) { output[index] = 0xF3; continue; }
		if (input[index] == 0x2320) { output[index] = 0xF4; continue; }
		if (input[index] == 0x2321) { output[index] = 0xF5; continue; }
		if (input[index] == 0x00F7) { output[index] = 0xF6; continue; }
		if (input[index] == 0x2248) { output[index] = 0xF7; continue; }
		if (input[index] == 0x00B0) { output[index] = 0xF8; continue; }
		if (input[index] == 0x2219) { output[index] = 0xF9; continue; }
		if (input[index] == 0x00B7) { output[index] = 0xFA; continue; }
		if (input[index] == 0x221A) { output[index] = 0xFB; continue; }
		if (input[index] == 0x207F) { output[index] = 0xFC; continue; }
		if (input[index] == 0x00B2) { output[index] = 0xFD; continue; }
		if (input[index] == 0x25A0) { output[index] = 0xFE; continue; }
		if (input[index] == 0x00A0) { output[index] = 0xFF; continue; }
		output[index] = '_';
	}
	return str8;
}

CL_StringHelp::BOMType CL_StringHelp::detect_bom(const void *data, CL_String::size_type length)
{
	const unsigned char utf32_be[] = { 0x00, 0x00, 0xfe, 0xff };
	const unsigned char utf32_le[] = { 0xff, 0xfe, 0x00, 0x00 };
	const unsigned char utf16_be[] = { 0xfe, 0xff };
	const unsigned char utf16_le[] = { 0xff, 0xfe };
	const unsigned char utf8[] = { 0xef, 0xbb, 0xbf };

	if (length >= 3 && memcmp(data, utf8, 3) == 0)
		return bom_utf8;
	else if (length >= 2 && memcmp(data, utf16_le, 2) == 0)
		return bom_utf16_le;
	else if (length >= 2 && memcmp(data, utf16_be, 2) == 0)
		return bom_utf16_be;
	else if (length >= 4 && memcmp(data, utf32_le, 4) == 0)
		return bom_utf32_le;
	else if (length >= 4 && memcmp(data, utf32_be, 4) == 0)
		return bom_utf32_be;
	else
		return bom_none;
}

/////////////////////////////////////////////////////////////////////////////
// CL_StringHelp Implementation:

const char CL_StringHelp::trailing_bytes_for_utf8[256] =
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5
};

const unsigned char CL_StringHelp::bitmask_leadbyte_for_utf8[6] =
{
	0x7f,
	0x1f,
	0x0f,
	0x07,
	0x03,
	0x01
};

#ifndef WIN32
#ifndef HAVE_WCSCASECMP
int
cl_wcscasecmp(const wchar_t *s1, const wchar_t *s2)
{
	const wchar_t *us1 = (const wchar_t *)s1;
	const wchar_t *us2 = (const wchar_t *)s2;

	while (towlower(*us1) == towlower(*us2++))
		if (*us1++ == '\0')
			return (0);
	return (towlower(*us1) - towlower(*--us2));
}
#endif
#endif


