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
#include "zip_file_header.h"
#include "API/Core/IOData/inputsource.h"
#include "API/Core/IOData/outputsource.h"
#include "zip_compression_method.h"
#include "zip_flags.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Zip_FileHeader construction: 

CL_Zip_FileHeader::CL_Zip_FileHeader()
{
	signature = 0x02014b50;
	version_needed_to_extract = 20;
	version_made_by = 0;
	general_purpose_bit_flag = CL_ZIP_CRC32_IN_FILE_DESCRIPTOR; //turn on bit 3 to indicate our CRC and filesizes won't
	//be valid until we add it after the actual file
	compression_method = zip_compress_deflate;
	last_mod_file_time = 0;
	last_mod_file_date = 0;

	disk_number_start = 0;
	crc32 = 0;
	compressed_size = 0;
	uncompressed_size = 0;
	internal_file_attributes = 0;
	external_file_attributes = 0;

}
	
CL_Zip_FileHeader::~CL_Zip_FileHeader()
{
}
	
/////////////////////////////////////////////////////////////////////////////
// CL_Zip_FileHeader attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_Zip_FileHeader operations:

void CL_Zip_FileHeader::load(CL_InputSource *input)
{
	signature = input->read_int32();
	if (signature != 0x02014b50)
	{
		throw CL_Error("Incorrect File Header signature");
	}
	version_made_by = input->read_int16();
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
	file_comment_length = input->read_int16();
	disk_number_start = input->read_int16();
	internal_file_attributes = input->read_int16();
	external_file_attributes = input->read_int32();
	relative_offset_of_local_header = input->read_int32();
	filename.resize(file_name_length);

	char *str1 = new char[file_name_length];
	char *str2 = new char[extra_field_length];
	char *str3 = new char[file_comment_length];
	try
	{
		input->read(str1, file_name_length);
		input->read(str2, extra_field_length);
		input->read(str3, file_comment_length);
		filename = std::string(str1, file_name_length);
		extra_field = std::string(str2, extra_field_length);
		file_comment = std::string(str3, file_comment_length);

		delete[] str1;
		delete[] str2;
		delete[] str3;
	}
	catch (...)
	{
		delete[] str1;
		delete[] str2;
		delete[] str3;
		throw;
	}
}

void CL_Zip_FileHeader::save(CL_OutputSource *output)
{
	
	output->write_int32(signature);
	output->write_int16(version_made_by);
	output->write_int16(version_needed_to_extract);
	output->write_int16(general_purpose_bit_flag);
	output->write_int16(compression_method);
	output->write_int16(last_mod_file_time);
	output->write_int16(last_mod_file_date);
	output->write_int32(crc32);
	output->write_int32(compressed_size);
	output->write_int32(uncompressed_size);
	output->write_int16(filename.size());
	output->write_int16(extra_field.size());
	output->write_int16(file_comment.size());
	output->write_int16(disk_number_start);
	output->write_int16(internal_file_attributes);
	output->write_int32(external_file_attributes);
	output->write_int32(relative_offset_of_local_header);
	output->write(filename.data(), filename.size());
	output->write(extra_field.data(), extra_field.size());
	output->write(file_comment.data(), file_comment.size());

}

/////////////////////////////////////////////////////////////////////////////
// CL_Zip_FileHeader implementation:
