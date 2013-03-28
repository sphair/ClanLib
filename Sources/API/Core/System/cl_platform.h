/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
**    Mark Page
**    (if your name is missing here, please add it)
*/

/// \addtogroup clanCore_System clanCore System
/// \{

#pragma once

#include <cstring>

#ifdef WIN32
// GCC automatically sets __SSE2__
#ifndef CL_DISABLE_SSE2
#define __SSE__
#define __SSE2__
#endif
#endif

namespace clan
{

#if defined(WIN32) && (defined(__MINGW32__) == 0)
typedef __int8             byte8;
typedef unsigned __int8    ubyte8;
typedef __int16            byte16;
typedef unsigned __int16   ubyte16;
typedef __int32            byte32;
typedef unsigned __int32   ubyte32;
typedef __int64            byte64;
typedef unsigned __int64   ubyte64;
#else
typedef char               byte8;
typedef unsigned char      ubyte8;
typedef short              byte16;
typedef unsigned short     ubyte16;
typedef int                byte32;
typedef unsigned int       ubyte32;
typedef long long          byte64;
typedef unsigned long long ubyte64;
#endif

const float  PI_F =		   3.14159274101257f;
const double PI_D =	 3.141592653589793115998;
const float  PI =		   3.14159274101257f;
}

/// \}
