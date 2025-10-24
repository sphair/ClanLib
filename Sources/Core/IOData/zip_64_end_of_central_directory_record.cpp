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
#include "zip_64_end_of_central_directory_record.h"
#include "API/Core/IOData/inputsource.h"
#include "API/Core/IOData/outputsource.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Zip_64EndOfCentralDirectoryRecord construction:

CL_Zip_64EndOfCentralDirectoryRecord::CL_Zip_64EndOfCentralDirectoryRecord()
{
	signature = 0x06064b50;
}
	
CL_Zip_64EndOfCentralDirectoryRecord::~CL_Zip_64EndOfCentralDirectoryRecord()
{
}
	
/////////////////////////////////////////////////////////////////////////////
// CL_Zip_64EndOfCentralDirectoryRecord attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_Zip_64EndOfCentralDirectoryRecord operations:

void CL_Zip_64EndOfCentralDirectoryRecord::load(CL_InputSource *input)
{
	signature = input->read_int32();
	if (signature != 0x06064b50)
	{
		throw CL_Error("Incorrect Zip64 End of Central Directory Record signature");
	}
	
	size_of_record = input->read_int64();
	version_made_by = input->read_int16();
	version_needed_to_extract = input->read_int16();
	number_of_this_disk = input->read_int32();
	number_of_disk_with_central_directory_start = input->read_int32();
	number_of_entries_on_this_disk = input->read_int64();
	number_of_entries_in_central_directory = input->read_int64();
	size_of_central_directory = input->read_int64();
	offset_to_start_of_central_directory = input->read_int64();
	
	// todo: read extensible data sector
}
	
void CL_Zip_64EndOfCentralDirectoryRecord::save(CL_OutputSource *output)
{
	output->write_int32(signature);
	output->write_int64(size_of_record);
	output->write_int16(version_made_by);
	output->write_int16(version_needed_to_extract);
	output->write_int32(number_of_this_disk);
	output->write_int32(number_of_disk_with_central_directory_start);
	output->write_int64(number_of_entries_on_this_disk);
	output->write_int64(number_of_entries_in_central_directory);
	output->write_int64(size_of_central_directory);
	output->write_int64(offset_to_start_of_central_directory);
	output->write(extensible_data_sector.data(), extensible_data_sector.size());
}

/////////////////////////////////////////////////////////////////////////////
// CL_Zip_64EndOfCentralDirectoryRecord implementation:
