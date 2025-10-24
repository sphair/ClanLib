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
#include "zip_end_of_central_directory_record.h"
#include "API/Core/IOData/inputsource.h"
#include "API/Core/IOData/outputsource.h"
#include "API/Core/IOData/cl_endian.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Zip_EndOfCentralDirectoryRecord construction:

CL_Zip_EndOfCentralDirectoryRecord::CL_Zip_EndOfCentralDirectoryRecord()
{	
	signature = 0x06054b50;
	SWAP_IF_BIG(signature);
}
	
CL_Zip_EndOfCentralDirectoryRecord::~CL_Zip_EndOfCentralDirectoryRecord()
{
}
	
/////////////////////////////////////////////////////////////////////////////
// CL_Zip_EndOfCentralDirectoryRecord attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_Zip_EndOfCentralDirectoryRecord operations:

void CL_Zip_EndOfCentralDirectoryRecord::load(CL_InputSource *input)
{
	signature = input->read_int32();
	if (signature != 0x06054b50)
	{
		throw CL_Error("Incorrect End of Central Directory Record signature");
	}
	
	number_of_this_disk = input->read_int16();
	number_of_disk_with_start_of_central_directory = input->read_int16();
	number_of_entries_on_this_disk = input->read_int16();
	number_of_entries_in_central_directory = input->read_int16();
	size_of_central_directory = input->read_int32();
	offset_to_start_of_central_directory = input->read_int32();
	file_comment_length = input->read_int16();

	char *str = new char[file_comment_length];
	try
	{
		input->read(str, file_comment_length);
		file_comment = std::string(str, file_comment_length);

		delete[] str;
	}
	catch (...)
	{
		delete[] str;
		throw;
	}
}
	
void CL_Zip_EndOfCentralDirectoryRecord::save(CL_OutputSource *output)
{
	output->write_int32(signature);
	output->write_int16(number_of_this_disk);
	output->write_int16(number_of_disk_with_start_of_central_directory);
	output->write_int16(number_of_entries_on_this_disk);
	output->write_int16(number_of_entries_in_central_directory);
	output->write_int32(size_of_central_directory);
	output->write_int32(offset_to_start_of_central_directory);
	output->write_int16(file_comment.length());
	output->write(file_comment.data(), file_comment.length());
}

/////////////////////////////////////////////////////////////////////////////
// CL_Zip_EndOfCentralDirectoryRecord implementation:
