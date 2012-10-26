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

namespace clan
{

class IODevice;

class Zip64EndOfCentralDirectoryRecord
{
/// \name Construction
/// \{

public:
	Zip64EndOfCentralDirectoryRecord();

	~Zip64EndOfCentralDirectoryRecord();


/// \}
/// \name Attributes
/// \{

public:
	byte32 signature; // 0x06064b50

	byte64 size_of_record;

	byte16 version_made_by;

	byte16 version_needed_to_extract;

	byte32 number_of_this_disk;

	byte32 number_of_disk_with_central_directory_start;

	byte64 number_of_entries_on_this_disk;

	byte64 number_of_entries_in_central_directory;

	byte64 size_of_central_directory;

	byte64 offset_to_start_of_central_directory;

	std::string extensible_data_sector;


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
