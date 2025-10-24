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

/// \addtogroup clanCore_Math clanCore Math
/// \{

#pragma once

#include "../api_core.h"
#include "../IOData/datatypes.h"
#include "../Text/string_types.h"
#include "sha1.h"

/// \brief A Collection of checksum functions.
///
/// \xmlonly !group=Core/Math! !header=core.h! \endxmlonly
class CL_API_CORE CL_HashFunctions
{
/// \name Construction
/// \{

public:

/// \}
/// \name Attributes
/// \{

public:

/// \}
/// \name Operations
/// \{

public:
	/// \brief Calculate a CRC32 checksum on the data. 
	static cl_uint32 crc32(const void *data, int size, cl_uint32 running_crc=0);

	/// \brief Calculate a CRC32 checksum on the data. 
	static cl_uint32 adler32(const void *data, int size, cl_uint32 running_adler32=0);

	/// \brief Generate SHA-1 hash from data.
	static CL_String8 sha1(const void *data, int size, bool uppercase = false);

	/// \brief Sha1
	///
	/// \param data = String Ref8
	/// \param uppercase = bool
	///
	/// \return String8
	static CL_String8 sha1(const CL_StringRef8 &data, bool uppercase = false);

	/// \brief Sha1
	///
	/// \param data = Data Buffer
	/// \param uppercase = bool
	///
	/// \return String8
	static CL_String8 sha1(const CL_DataBuffer &data, bool uppercase = false);

	/// \brief Sha1
	///
	/// \param data = void
	/// \param size = value
	/// \param out_hash = char
	static void sha1(const void *data, int size, unsigned char out_hash[20]);

	/// \brief Sha1
	///
	/// \param data = String Ref8
	/// \param out_hash = char
	static void sha1(const CL_StringRef8 &data, unsigned char out_hash[20]);

	/// \brief Sha1
	///
	/// \param data = Data Buffer
	/// \param out_hash = char
	static void sha1(const CL_DataBuffer &data, unsigned char out_hash[20]);

/// \}
/// \name Implementation
/// \{

private:
/// \}
};

/// \}
