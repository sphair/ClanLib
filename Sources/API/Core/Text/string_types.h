/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

#pragma once

#include <string>
#include <functional>
#include "string_ref8.h"
#include "string_ref16.h"
#include "string8.h"
#include "string16.h"

typedef CL_StringRef8 CL_StringRef;
typedef CL_String8 CL_String;

#if defined(__APPLE__) || (defined(_MSC_VER) && _MSC_VER < 1600)
	namespace std
	{
		namespace tr1
		{
			template<>
			class hash<CL_String> : hash<const CL_String::char_type*>
			{
			public:
					size_t operator()(const CL_String& keyval) const
					{
							return hash<const CL_String::char_type*>::operator()(keyval.c_str());
					}
			};
		}
	}
#else
	namespace std
	{
		template<>
		class hash<CL_String> : hash<const CL_String::char_type*>
		{
		public:
			size_t operator()(const CL_String& keyval) const
			{
				return hash<const CL_String::char_type*>::operator()(keyval.c_str());
			}
		};
	}
#endif

/// \}
