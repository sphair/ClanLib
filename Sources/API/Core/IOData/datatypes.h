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
**    (if your name is missing here, please add it)
*/

/// \addtogroup clanCore_I_O_Data clanCore I/O Data
/// \{


#pragma once


#if _MSC_VER > 1000
#pragma once
#endif

#if defined(WIN32) && (defined(__MINGW32__) == 0)
typedef __int8             cl_int8;
typedef unsigned __int8    cl_uint8;
typedef __int16            cl_int16;
typedef unsigned __int16   cl_uint16;
typedef __int32            cl_int32;
typedef unsigned __int32   cl_uint32;
typedef __int64            cl_int64;
typedef unsigned __int64   cl_uint64;
#else
typedef char               cl_int8;
typedef unsigned char      cl_uint8;
typedef short              cl_int16;
typedef unsigned short     cl_uint16;
typedef int                cl_int32;
typedef unsigned int       cl_uint32;
typedef long long          cl_int64;
typedef unsigned long long cl_uint64;
#endif


/// \}
