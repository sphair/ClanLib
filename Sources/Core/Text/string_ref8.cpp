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
#include "API/Core/Text/string_ref8.h"
#include "API/Core/Text/string_types.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/System/memory_pool.h"

#ifndef WIN32
#include <string.h>
#endif

CL_StringRef8::CL_StringRef8()
: null_terminated(false), temporary(false)
{
}

CL_StringRef8::CL_StringRef8(const std::string &source)
: null_terminated(false), temporary(false)
{
	this->data_length = source.length();
	this->data_ptr = (char *) source.data();
}

CL_StringRef8::CL_StringRef8(const CL_StringRef8 &source)
: null_terminated(false), temporary(false)
{
	this->data_ptr = (char *) source.data();
	this->data_length = source.length();
}

CL_StringRef8::CL_StringRef8(const CL_StringData8 &source)
: null_terminated(false), temporary(false)
{
	this->data_ptr = (char *) source.data();
	this->data_length = source.length();
}

CL_StringRef8::CL_StringRef8(const char *c_str)
: null_terminated(false), temporary(false)
{
	this->data_ptr = (char *) c_str;
	this->data_length = (size_type) strlen(c_str);
}

CL_StringRef8::CL_StringRef8(const char *c_str, size_type length, bool null_terminated)
: null_terminated(null_terminated), temporary(false)
{
	this->data_ptr = (char *) c_str;
	this->data_length = length;
}

CL_StringRef8::CL_StringRef8(const wchar_t *wc_str)
: null_terminated(false), temporary(false)
{
	CL_String8 temp = CL_StringHelp::ucs2_to_local8(wc_str);
	create_temp(temp.data(), temp.length());
}

CL_StringRef8::CL_StringRef8(const wchar_t *wc_str, size_type length, bool null_terminated)
: null_terminated(null_terminated), temporary(false)
{
	CL_String8 temp = CL_StringHelp::ucs2_to_local8(CL_StringRef16(wc_str, length, null_terminated));
	create_temp(temp.data(), temp.length());
}

CL_StringRef8::~CL_StringRef8()
{
	clear();
}

const char *CL_StringRef8::c_str() const
{
	if (this->data_ptr == 0 && this->data_length == 0)
	{
		static const char empty_string[] = { 0 };
		return empty_string;
	}
	if (!null_terminated)
		create_temp(this->data_ptr, this->data_length);
	return data();
}

void CL_StringRef8::set_length(size_type length)
{
	this->data_length = length;
}

CL_StringRef8 &CL_StringRef8::operator =(const CL_StringRef8 &source)
{
	if (&source == this)
		return *this;
	clear();
	this->data_ptr = (char *) source.data();
	this->data_length = source.length();
	return *this;
}

CL_StringRef8 &CL_StringRef8::operator =(const CL_StringData8 &source)
{
	if (&source == this)
		return *this;
	clear();
	this->data_ptr = (char *) source.data();
	this->data_length = source.length();
	return *this;
}

CL_StringRef8 &CL_StringRef8::operator =(const char *c_str)
{
	clear();
	this->data_ptr = (char *) c_str;
	this->data_length = (size_type) strlen(c_str);
	return *this;
}

CL_StringRef8 &CL_StringRef8::operator =(const wchar_t *c_str)
{
	CL_String8 temp = CL_StringHelp::ucs2_to_local8(c_str);
	create_temp(temp.data(), temp.length());
	return *this;
}

void CL_StringRef8::clear() const
{
	if (temporary)
		CL_MemoryPool::get_temp_pool()->free(this->data_ptr);
	temporary = false;
	null_terminated = false;
	this->data_ptr = 0;
	this->data_length = 0;
}

void CL_StringRef8::create_temp(const char *data, size_type length) const
{
	clear();
	this->data_ptr = (char *) CL_MemoryPool::get_temp_pool()->alloc(sizeof(char) * (length + 1));
	this->data_length = length;
	memcpy(this->data_ptr, data, sizeof(char) * this->data_length);
	this->data_ptr[this->data_length] = 0;
	temporary = true;
	null_terminated = true;
}
