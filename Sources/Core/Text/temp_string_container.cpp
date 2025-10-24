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
#include "API/Core/Text/temp_string_container.h"
#include "API/Core/Text/string_types.h"
#include "API/Core/System/memory_pool.h"

template<typename CharType, typename ReferenceClass, typename StdString>
CL_TempStringContainer<CharType, ReferenceClass, StdString>::CL_TempStringContainer()
{
	this->pool = CL_MemoryPool::get_temp_pool();
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_TempStringContainer<CharType, ReferenceClass, StdString>::CL_TempStringContainer(const CL_TempStringContainer &source)
{
	this->pool = CL_MemoryPool::get_temp_pool();
	append(source);
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_TempStringContainer<CharType, ReferenceClass, StdString>::CL_TempStringContainer(const CL_StringData<CharType, ReferenceClass, StdString> &source)
{
	this->pool = CL_MemoryPool::get_temp_pool();
	append(source);
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_TempStringContainer<CharType, ReferenceClass, StdString>::CL_TempStringContainer(const char *c_str)
{
	this->pool = CL_MemoryPool::get_temp_pool();
	this->append(c_str);
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_TempStringContainer<CharType, ReferenceClass, StdString>::CL_TempStringContainer(const char *c_str, size_type length)
{
	this->pool = CL_MemoryPool::get_temp_pool();
	this->append(c_str, length);
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_TempStringContainer<CharType, ReferenceClass, StdString>::CL_TempStringContainer(const wchar_t *wc_str)
{
	this->pool = CL_MemoryPool::get_temp_pool();
	this->append(wc_str);
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_TempStringContainer<CharType, ReferenceClass, StdString>::CL_TempStringContainer(const wchar_t *wc_str, size_type length)
{
	this->pool = CL_MemoryPool::get_temp_pool();
	this->append(wc_str, length);
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_TempStringContainer<CharType, ReferenceClass, StdString>::CL_TempStringContainer(size_type n, CharType c)
{
	this->pool = CL_MemoryPool::get_temp_pool();
	this->append(n, c);
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_TempStringContainer<CharType, ReferenceClass, StdString>::operator ReferenceClass() const
{
	return ReferenceClass(data(), length(), false);
}

/////////////////////////////////////////////////////////////////////////////

template<typename CharType, typename ReferenceClass, typename StdString>
CL_TempStringContainer<CharType, ReferenceClass, StdString> operator+(const CL_StringData<CharType, ReferenceClass, StdString> &s1, const CL_StringData<CharType, ReferenceClass, StdString> &s2)
{
	CL_TempStringContainer<CharType, ReferenceClass, StdString> result;
	result.reserve(s1.length() + s2.length());
	result.append(s1);
	result.append(s2);
	return result;
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_TempStringContainer<CharType, ReferenceClass, StdString> operator+(const char *s1, const CL_StringData<CharType, ReferenceClass, StdString> &s2)
{
	CL_TempStringContainer<CharType, ReferenceClass, StdString> result;
	ReferenceClass ref_s1(s1);
	result.reserve(ref_s1.length() + s2.length());
	result.append(ref_s1);
	result.append(s2);
	return result;
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_TempStringContainer<CharType, ReferenceClass, StdString> operator+(const wchar_t *s1, const CL_StringData<CharType, ReferenceClass, StdString> &s2)
{
	CL_TempStringContainer<CharType, ReferenceClass, StdString> result;
	ReferenceClass ref_s1(s1);
	result.reserve(ref_s1.length() + s2.length());
	result.append(ref_s1);
	result.append(s2);
	return result;
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_TempStringContainer<CharType, ReferenceClass, StdString> operator+(const CL_StringData<CharType, ReferenceClass, StdString> &s1, const char *s2)
{
	CL_TempStringContainer<CharType, ReferenceClass, StdString> result;
	ReferenceClass ref_s2(s2);
	result.reserve(ref_s2.length() + s1.length());
	result.append(s1);
	result.append(ref_s2);
	return result;
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_TempStringContainer<CharType, ReferenceClass, StdString> operator+(const CL_StringData<CharType, ReferenceClass, StdString> &s1, const wchar_t *s2)
{
	CL_TempStringContainer<CharType, ReferenceClass, StdString> result;
	ReferenceClass ref_s2(s2);
	result.reserve(ref_s2.length() + s1.length());
	result.append(s1);
	result.append(ref_s2);
	return result;
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_TempStringContainer<CharType, ReferenceClass, StdString> operator+(CharType c, const CL_StringData<CharType, ReferenceClass, StdString> &s2)
{
	CL_TempStringContainer<CharType, ReferenceClass, StdString> result;
	result.reserve(1 + s2.length());
	result.push_back(c);
	result.append(s2);
	return result;
}

template<typename CharType, typename ReferenceClass, typename StdString>
CL_TempStringContainer<CharType, ReferenceClass, StdString> operator+(const CL_StringData<CharType, ReferenceClass, StdString> &s1, CharType c)
{
	CL_TempStringContainer<CharType, ReferenceClass, StdString> result;
	result.reserve(1 + s1.length());
	result.append(s1);
	result.push_back(c);
	return result;
}

// Explicit instantiate the versions we use:

template class CL_TempStringContainer<char, CL_StringRef8, std::string>;
template CL_TempStringContainer<char, CL_StringRef8, std::string> operator+(const CL_StringData<char, CL_StringRef8, std::string> &s1, const CL_StringData<char, CL_StringRef8, std::string> &s2);
template CL_TempStringContainer<char, CL_StringRef8, std::string> operator+(const char *s1, const CL_StringData<char, CL_StringRef8, std::string> &s2);
template CL_TempStringContainer<char, CL_StringRef8, std::string> operator+(const wchar_t *s1, const CL_StringData<char, CL_StringRef8, std::string> &s2);
template CL_TempStringContainer<char, CL_StringRef8, std::string> operator+(const CL_StringData<char, CL_StringRef8, std::string> &s1, const char *s2);
template CL_TempStringContainer<char, CL_StringRef8, std::string> operator+(const CL_StringData<char, CL_StringRef8, std::string> &s1, const wchar_t *s2);
template CL_TempStringContainer<char, CL_StringRef8, std::string> operator+(char c, const CL_StringData<char, CL_StringRef8, std::string> &s2);
template CL_TempStringContainer<char, CL_StringRef8, std::string> operator+(const CL_StringData<char, CL_StringRef8, std::string> &s1, char c);

template class CL_TempStringContainer<wchar_t, CL_StringRef16, std::wstring>;
template CL_TempStringContainer<wchar_t, CL_StringRef16, std::wstring> operator+(const CL_StringData<wchar_t, CL_StringRef16, std::wstring> &s1, const CL_StringData<wchar_t, CL_StringRef16, std::wstring> &s2);
template CL_TempStringContainer<wchar_t, CL_StringRef16, std::wstring> operator+(const char *s1, const CL_StringData<wchar_t, CL_StringRef16, std::wstring> &s2);
template CL_TempStringContainer<wchar_t, CL_StringRef16, std::wstring> operator+(const wchar_t *s1, const CL_StringData<wchar_t, CL_StringRef16, std::wstring> &s2);
template CL_TempStringContainer<wchar_t, CL_StringRef16, std::wstring> operator+(const CL_StringData<wchar_t, CL_StringRef16, std::wstring> &s1, const char *s2);
template CL_TempStringContainer<wchar_t, CL_StringRef16, std::wstring> operator+(const CL_StringData<wchar_t, CL_StringRef16, std::wstring> &s1, const wchar_t *s2);
template CL_TempStringContainer<wchar_t, CL_StringRef16, std::wstring> operator+(wchar_t c, const CL_StringData<wchar_t, CL_StringRef16, std::wstring> &s2);
template CL_TempStringContainer<wchar_t, CL_StringRef16, std::wstring> operator+(const CL_StringData<wchar_t, CL_StringRef16, std::wstring> &s1, wchar_t c);
