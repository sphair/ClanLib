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

#include "Core/precomp.h"
#include "zip_local_file_header.h"
#include "API/Core/IOData/inputsource.h"
#include "API/Core/IOData/outputsource.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Zip_LocalFileHeader construction:

CL_Zip_LocalFileHeader::CL_Zip_LocalFileHeader()
{
	signature = 0x04034b50;
}
	
CL_Zip_LocalFileHeader::~CL_Zip_LocalFileHeader()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_Zip_LocalFileHeader attributes:

	
/////////////////////////////////////////////////////////////////////////////
// CL_Zip_LocalFileHeader operations:

void CL_Zip_LocalFileHeader::load(CL_InputSource *input)
{
	signature = input->read_int32();
	if (signature != 0x04034b50)
	{
		throw CL_Error("Incorrect Local File Header signature");
	}
	version_needed_to_extract = input->read_int16();
	general_purpose_bit_flag = input->read_int16();
	compression_method = input->read_int16();
	last_mod_file_time = input->read_int16();
	last_mod_file_date = input->read_int16();
	crc32 = input->read_int32();
	compressed_size = input->read_int32();
	uncompressed_size = input->read_int32();
	file_name_length = input->read_int16();
	extra_field_length = input->read_int16();
	char *str1 = new char[file_name_length];
	char *str2 = new char[extra_field_length];
	try
	{
		input->read(str1, file_name_length);
		input->read(str2, extra_field_length);
		filename = std::string(str1, file_name_length);
		extra_field = std::string(str2, extra_field_length);

		delete[] str1;
		delete[] str2;
	}
	catch (...)
	{
		delete[] str1;
		delete[] str2;
		throw;
	}
}
	
void CL_Zip_LocalFileHeader::save(CL_OutputSource *output)
{
}
	
/////////////////////////////////////////////////////////////////////////////
// CL_Zip_LocalFileHeader implementation:
