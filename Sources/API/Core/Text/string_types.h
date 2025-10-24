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

/// \addtogroup clanCore_Text clanCore Text
/// \{


#ifndef _cl_header_string_types_
#define _cl_header_string_types_

#if defined(_MSC_VER)
#pragma once
#endif

#include <string>
#include "string_data.h"
#include "string_reference.h"
#include "string_container.h"
#include "temp_string_container.h"

typedef CL_StringReference<char, std::string> CL_StringRef8;
typedef CL_StringReference<wchar_t, std::wstring> CL_StringRef16;
typedef CL_StringContainer<char, CL_StringRef8, std::string> CL_String8;
typedef CL_StringContainer<wchar_t, CL_StringRef16, std::wstring> CL_String16;
typedef CL_TempStringContainer<char, CL_StringRef8, std::string> CL_TempString8;
typedef CL_TempStringContainer<wchar_t, CL_StringRef16, std::wstring> CL_TempString16;

#if defined(UNICODE)
typedef CL_StringRef16 CL_StringRef;
typedef CL_String16 CL_String;
typedef CL_TempString16 CL_TempString;
#define cl_text(a) L##a
#else
typedef CL_StringRef8 CL_StringRef;
typedef CL_String8 CL_String;
typedef CL_TempString8 CL_TempString;
#define cl_text(a) a
#endif

#endif
/// \}
