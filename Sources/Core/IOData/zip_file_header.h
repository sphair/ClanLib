/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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

#ifndef header_zip_file_header
#define header_zip_file_header

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/Core/IOData/datatypes.h"

class CL_InputSource;
class CL_OutputSource;

class CL_Zip_FileHeader
{
//! Construction:
public:
	CL_Zip_FileHeader();
	
	~CL_Zip_FileHeader();
	
//! Attributes:
public:
	cl_int32 signature; // 0x02014b50
	
	cl_int16 version_made_by;
	
	cl_int16 version_needed_to_extract;
	
	cl_int16 general_purpose_bit_flag;
	
	cl_int16 compression_method;
	
	cl_int16 last_mod_file_time;
	
	cl_int16 last_mod_file_date;
	
	cl_int32 crc32;
	
	cl_int32 compressed_size;
	
	cl_int32 uncompressed_size;
	
	cl_int16 file_name_length;
	
	cl_int16 extra_field_length;
	
	cl_int16 file_comment_length;
	
	cl_int16 disk_number_start;
	
	cl_int16 internal_file_attributes;
	
	cl_int32 external_file_attributes;
	
	cl_int32 relative_offset_of_local_header;
	
	std::string filename;
	
	std::string extra_field;
	
	std::string file_comment;

	std::string local_filename; //used only when zipping
	
//! Operations:
public:
	void load(CL_InputSource *input);
	
	void save(CL_OutputSource *output);

//! Implementation:
private:
};

#endif
