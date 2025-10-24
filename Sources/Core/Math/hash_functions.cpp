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
**    Harry Storbacka
**    Magnus Norddahl
*/

#include "Core/precomp.h"
#include "API/Core/Math/hash_functions.h"
#include "API/Core/System/databuffer.h"
#include <zlib.h>

/////////////////////////////////////////////////////////////////////////////
// CL_HashFunctions Construction:

/////////////////////////////////////////////////////////////////////////////
// CL_HashFunctions Attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_HashFunctions Operations:

cl_uint32 CL_HashFunctions::crc32( const void *data, int size, cl_uint32 running_crc/*=0*/ )
{
	cl_uint32 crc = running_crc;
	if (crc == 0)
		crc = ::crc32(0L, Z_NULL, 0);

	return ::crc32(running_crc, (const Bytef*)data, size);;
}

cl_uint32 CL_HashFunctions::adler32( const void *data, int size, cl_uint32 running_adler32/*=0*/ )
{
	cl_uint32 adler = running_adler32;
	if (adler == 0)
		adler = ::adler32(0L, Z_NULL, 0);

	return ::adler32(adler, (const Bytef*)data, size);
}


CL_String8 CL_HashFunctions::sha1(const void *data, int size, bool uppercase)
{
	CL_SHA1 sha1;
	sha1.add(data, size);
	sha1.calculate();
	return sha1.get_hash(uppercase);
}

CL_String8 CL_HashFunctions::sha1(const CL_StringRef8 &data, bool uppercase)
{
	return sha1(data.data(), data.length(), uppercase);
}

CL_String8 CL_HashFunctions::sha1(const CL_DataBuffer &data, bool uppercase)
{
	return sha1(data.get_data(), data.get_size(), uppercase);
}

void CL_HashFunctions::sha1(const void *data, int size, unsigned char out_hash[20])
{
	CL_SHA1 sha1;
	sha1.add(data, size);
	sha1.calculate();
	sha1.get_hash(out_hash);
}

void CL_HashFunctions::sha1(const CL_DataBuffer &data, unsigned char out_hash[20])
{
	sha1(data.get_data(), data.get_size(), out_hash);
}

void CL_HashFunctions::sha1(const CL_StringRef8 &data, unsigned char out_hash[20])
{
	sha1(data.data(), data.length(), out_hash);
}


/////////////////////////////////////////////////////////////////////////////
// CL_HashFunctions Implementation:

