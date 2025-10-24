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

#ifndef header_zip_64_end_of_central_directory_record
#define header_zip_64_end_of_central_directory_record

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/Core/IOData/datatypes.h"

class CL_InputSource;
class CL_OutputSource;

class CL_Zip_64EndOfCentralDirectoryRecord
{
//! Construction:
public:
	CL_Zip_64EndOfCentralDirectoryRecord();
	
	~CL_Zip_64EndOfCentralDirectoryRecord();
	
//! Attributes:
public:
	cl_int32 signature; // 0x06064b50
	
	cl_int64 size_of_record;
	
	cl_int16 version_made_by;
	
	cl_int16 version_needed_to_extract;
	
	cl_int32 number_of_this_disk;
	
	cl_int32 number_of_disk_with_central_directory_start;
	
	cl_int64 number_of_entries_on_this_disk;
	
	cl_int64 number_of_entries_in_central_directory;
	
	cl_int64 size_of_central_directory;
	
	cl_int64 offset_to_start_of_central_directory;
	
	std::string extensible_data_sector;
	
//! Operations:
public:
	void load(CL_InputSource *input);
	
	void save(CL_OutputSource *output);

//! Implementation:
private:
};

#endif
