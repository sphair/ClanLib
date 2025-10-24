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
#include "zip_64_end_of_central_directory_locator.h"
#include "API/Core/IOData/inputsource.h"
#include "API/Core/IOData/outputsource.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Zip_64EndOfCentralDirectoryLocator construction:

CL_Zip_64EndOfCentralDirectoryLocator::CL_Zip_64EndOfCentralDirectoryLocator()
{
	signature = 0x07064b50;
}
	
CL_Zip_64EndOfCentralDirectoryLocator::~CL_Zip_64EndOfCentralDirectoryLocator()
{
}

/////////////////////////////////////////////////////////////////////////////	
// CL_Zip_64EndOfCentralDirectoryLocator attributes:


/////////////////////////////////////////////////////////////////////////////
// CL_Zip_64EndOfCentralDirectoryLocator operations:

void CL_Zip_64EndOfCentralDirectoryLocator::load(CL_InputSource *input)
{
	signature = input->read_int32();
	if (signature != 0x07064b50)
	{
		throw CL_Error("Incorrect Zip64 End of central directory locator signature!");
	}
	
	number_of_disk_with_zip64_end_of_central_directory = input->read_int32();
	relative_offset_of_zip64_end_of_central_directory = input->read_int64();
	total_number_of_disks = input->read_int32();
}
	
void CL_Zip_64EndOfCentralDirectoryLocator::save(CL_OutputSource *output)
{
	output->write_int32(signature);
	output->write_int32(number_of_disk_with_zip64_end_of_central_directory);
	output->write_int64(relative_offset_of_zip64_end_of_central_directory);
	output->write_int32(total_number_of_disks);
}

/////////////////////////////////////////////////////////////////////////////
// CL_Zip_64EndOfCentralDirectoryLocator implementation:

