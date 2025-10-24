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
#include "API/Core/Text/string_reference.h"
#include "API/Core/Text/string_types.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/System/memory_pool.h"

#ifndef WIN32
#include <string.h>
#endif

template<>
CL_StringReference<wchar_t, std::wstring>::CL_StringReference(const wchar_t *wc_str)
: null_terminated(false), temporary(false)
{
	this->data_ptr = (wchar_t *) wc_str;
	this->data_length = (size_type) wcslen(wc_str);
}

template<>
CL_StringReference<wchar_t, std::wstring>::CL_StringReference(const wchar_t *wc_str, size_type length, bool null_terminated)
: null_terminated(null_terminated), temporary(false)
{
	this->data_ptr = (wchar_t *) wc_str;
	this->data_length = length;
}

template<typename CharType, typename StdString>
CL_StringReference<CharType, StdString>::CL_StringReference()
: null_terminated(false), temporary(false)
{
}

template<typename CharType, typename StdString>
CL_StringReference<CharType, StdString>::CL_StringReference(const StdString &source)
: null_terminated(false), temporary(true)
{
	this->data_length = source.length();
	this->data_ptr = (CharType *) source.data();
}

template<typename CharType, typename StdString>
CL_StringReference<CharType, StdString>::CL_StringReference(const CL_StringReference &source)
: null_terminated(false), temporary(false)
{
	this->data_ptr = (CharType *) source.data();
	this->data_length = source.length();
}

template<typename CharType, typename StdString>
CL_StringReference<CharType, StdString>::CL_StringReference(const CL_StringData<CharType, CL_StringReference<CharType, StdString>, StdString> &source)
: null_terminated(false), temporary(false)
{
	this->data_ptr = (CharType *) source.data();
	this->data_length = source.length();
}

template<>
CL_StringReference<char, std::string>::CL_StringReference(const char *c_str)
: null_terminated(false), temporary(false)
{
	this->data_ptr = (char *) c_str;
	this->data_length = (size_type) strlen(c_str);
}

template<>
CL_StringReference<char, std::string>::CL_StringReference(const char *c_str, size_type length, bool null_terminated)
: null_terminated(null_terminated), temporary(false)
{
	this->data_ptr = (char *) c_str;
	this->data_length = length;
}

template<>
CL_StringReference<char, std::string>::CL_StringReference(const wchar_t *wc_str)
: null_terminated(false), temporary(false)
{
	CL_TempString8 temp = CL_StringHelp::ucs2_to_local8(wc_str);
	create_temp(temp.data(), temp.length());
}

template<>
CL_StringReference<char, std::string>::CL_StringReference(const wchar_t *wc_str, size_type length, bool null_terminated)
: null_terminated(null_terminated), temporary(false)
{
	CL_TempString8 temp = CL_StringHelp::ucs2_to_local8(CL_StringRef16(wc_str, length, null_terminated));
	create_temp(temp.data(), temp.length());
}

template<>
CL_StringReference<wchar_t, std::wstring>::CL_StringReference(const char *c_str)
: null_terminated(false), temporary(false)
{
	CL_TempString16 temp = CL_StringHelp::local8_to_ucs2(c_str);
	create_temp(temp.data(), temp.length());
}

template<>
CL_StringReference<wchar_t, std::wstring>::CL_StringReference(const char *c_str, size_type length, bool null_terminated)
: null_terminated(null_terminated), temporary(false)
{
	CL_TempString16 temp = CL_StringHelp::local8_to_ucs2(CL_StringRef8(c_str, length, null_terminated));
	create_temp(temp.data(), temp.length());
}

template<typename CharType, typename StdString>
CL_StringReference<CharType, StdString>::~CL_StringReference()
{
	clear();
}

template<typename CharType, typename StdString>
const CharType *CL_StringReference<CharType, StdString>::c_str() const
{
	if (this->data_ptr == 0 && this->data_length == 0)
	{
		static const CharType empty_string[] = { 0 };
		return empty_string;
	}
	if (!null_terminated)
		create_temp(this->data_ptr, this->data_length);
	return data();
}

template<typename CharType, typename StdString>
void CL_StringReference<CharType, StdString>::set_length(size_type length)
{
	this->data_length = length;
}

template<typename CharType, typename StdString>
CL_StringReference<CharType, StdString> &CL_StringReference<CharType, StdString>::operator =(const CL_StringReference<CharType, StdString> &source)
{
	if (&source == this)
		return *this;
	clear();
	this->data_ptr = (CharType *) source.data();
	this->data_length = source.length();
	return *this;
}

template<typename CharType, typename StdString>
CL_StringReference<CharType, StdString> &CL_StringReference<CharType, StdString>::operator =(const CL_StringData<CharType, CL_StringReference<CharType, StdString>, StdString> &source)
{
	if (&source == this)
		return *this;
	clear();
	this->data_ptr = (CharType *) source.data();
	this->data_length = source.length();
	return *this;
}

template<>
CL_StringReference<char, std::string> &CL_StringReference<char, std::string>::operator =(const char *c_str)
{
	clear();
	this->data_ptr = (char *) c_str;
	this->data_length = (size_type) strlen(c_str);
	return *this;
}

template<>
CL_StringReference<char, std::string> &CL_StringReference<char, std::string>::operator =(const wchar_t *c_str)
{
	CL_TempString8 temp = CL_StringHelp::ucs2_to_local8(c_str);
	create_temp(temp.data(), temp.length());
	return *this;
}

template<>
CL_StringReference<wchar_t, std::wstring> &CL_StringReference<wchar_t, std::wstring>::operator =(const char *c_str)
{
	CL_TempString16 temp = CL_StringHelp::local8_to_ucs2(c_str);
	create_temp(temp.data(), temp.length());
	return *this;
}

template<>
CL_StringReference<wchar_t, std::wstring> &CL_StringReference<wchar_t, std::wstring>::operator =(const wchar_t *c_str)
{
	clear();
	this->data_ptr = (wchar_t *) c_str;
	this->data_length = (size_type) wcslen(c_str);
	return *this;
}

template<typename CharType, typename StdString>
void CL_StringReference<CharType, StdString>::clear() const
{
	if (temporary)
		CL_MemoryPool::get_temp_pool()->free(this->data_ptr);
	temporary = false;
	null_terminated = false;
	this->data_ptr = 0;
	this->data_length = 0;
}

template<typename CharType, typename StdString>
void CL_StringReference<CharType, StdString>::create_temp(const CharType *data, size_type length) const
{
	clear();
	this->data_ptr = (CharType *) CL_MemoryPool::get_temp_pool()->alloc(sizeof(CharType) * (length + 1));
	this->data_length = length;
	memcpy(this->data_ptr, data, sizeof(CharType) * this->data_length);
	this->data_ptr[this->data_length] = 0;
	temporary = true;
	null_terminated = true;
}

// Explicit instantiate the versions we use:
template class CL_StringReference<char, std::string>;
template class CL_StringReference<wchar_t, std::wstring>;
