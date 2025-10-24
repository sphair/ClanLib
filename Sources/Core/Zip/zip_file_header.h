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

#pragma once

#include "API/Core/System/cl_platform.h"
#include "API/Core/System/databuffer.h"

class CL_IODevice;

class CL_ZipFileHeader
{
/// \name Construction
/// \{

public:
	CL_ZipFileHeader();

	~CL_ZipFileHeader();


/// \}
/// \name Attributes
/// \{

public:
	cl_int signature; // 0x02014b50

	cl_short version_made_by;

	cl_short version_needed_to_extract;

	cl_short general_purpose_bit_flag;

	cl_short compression_method;

	cl_short last_mod_file_time;

	cl_short last_mod_file_date;

	cl_uint crc32;

	cl_int compressed_size;

	cl_int uncompressed_size;

	cl_short file_name_length;

	cl_short extra_field_length;

	cl_short file_comment_length;

	cl_short disk_number_start;

	cl_short internal_file_attributes;

	cl_int external_file_attributes;

	cl_int relative_offset_of_local_header;

	CL_String filename;

	CL_DataBuffer extra_field;

	CL_String file_comment;


/// \}
/// \name Operations
/// \{

public:
	void load(CL_IODevice &input);

	void save(CL_IODevice &output);


/// \}
/// \name Implementation
/// \{

private:
/// \}
};


