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
*/

#pragma once

#include "API/Core/System/cl_platform.h"
#include "API/Core/System/databuffer.h"

namespace clan
{

class IODevice;

class ZipFileHeader
{
/// \name Construction
/// \{

public:
	ZipFileHeader();

	~ZipFileHeader();


/// \}
/// \name Attributes
/// \{

public:
	byte32 signature; // 0x02014b50

	byte16 version_made_by;

	byte16 version_needed_to_extract;

	byte16 general_purpose_bit_flag;

	byte16 compression_method;

	byte16 last_mod_file_time;

	byte16 last_mod_file_date;

	ubyte32 crc32;

	byte32 compressed_size;

	byte32 uncompressed_size;

	byte16 file_name_length;

	byte16 extra_field_length;

	byte16 file_comment_length;

	byte16 disk_number_start;

	byte16 internal_file_attributes;

	byte32 external_file_attributes;

	byte32 relative_offset_of_local_header;

	std::string filename;

	DataBuffer extra_field;

	std::string file_comment;


/// \}
/// \name Operations
/// \{

public:
	void load(IODevice &input);

	void save(IODevice &output);


/// \}
/// \name Implementation
/// \{

private:
/// \}
};

}
