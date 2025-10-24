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
#include "API/Core/IOData/zip_archive.h"
#include "API/Core/IOData/inputsource_file.h"
#include "API/Core/IOData/inputsource_memory.h"
#include "API/Core/System/clanstring.h"
#include "zip_archive_generic.h"
#include "zip_file_header.h"
#include "zip_64_end_of_central_directory_record.h"
#include "zip_64_end_of_central_directory_locator.h"
#include "zip_end_of_central_directory_record.h"
#include "zip_file_entry_generic.h"
#include "inputsource_zip_fileentry.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Zip_Archive construction:

CL_Zip_Archive::CL_Zip_Archive() : impl(new CL_Zip_Archive_Generic)
{
}
	
CL_Zip_Archive::CL_Zip_Archive(const std::string &filename) : impl(new CL_Zip_Archive_Generic)
{
	impl->filename = filename;

	// Load zip file structures:

	CL_InputSource *input = new CL_InputSource_File(filename);
	
	// indicate the file is little-endian
	input->set_little_endian_mode();
	
	// Find end of central directory record:

	int size_file = input->size();
	
	char buffer[32*1024];
	if (size_file > 32*1024) input->seek(-32*1024, CL_InputSource::seek_end);
	int size_buffer = input->read(buffer, 32*1024);
	
	int end_record_pos = -1;
	for (int pos = size_buffer-4; pos >= 0; pos--)
	{
		if( buffer[pos] == 0x50 && buffer[pos+1] == 0x4b && buffer[pos+2] == 0x05  && buffer[pos+3] == 0x06 )
		{
			end_record_pos = size_file-size_buffer+pos;
			input->tell();
			break;
		}
	}
	if (end_record_pos == -1)
	{
		delete input;
		throw CL_Error("This appear not to be a zip file");
	}
	
	// Load end of central directory record:
	
	CL_Zip_EndOfCentralDirectoryRecord end_of_directory;
	input->seek(end_record_pos, CL_InputSource::seek_set);
	end_of_directory.load(input);
	
	// Look for zip64 central directory locator:
	
	bool zip64 = false;
	CL_Zip_64EndOfCentralDirectoryLocator zip64_locator;
	CL_Zip_64EndOfCentralDirectoryRecord zip64_end_of_directory;
	
	int end64_locator = end_record_pos-20;
	input->seek(end64_locator, CL_InputSource::seek_set);
	if (input->read_uint32() == 0x07064b50)
	{
		// Load zip64 structures:
		
		input->seek(end64_locator, CL_InputSource::seek_set);
		zip64_locator.load(input);
		
		input->seek(end64_locator+zip64_locator.relative_offset_of_zip64_end_of_central_directory, CL_InputSource::seek_set);
		zip64_end_of_directory.load(input);
		
		zip64 = true;
	}
	
	// Load central directory records:
	
	if (zip64) input->seek(zip64_end_of_directory.offset_to_start_of_central_directory, CL_InputSource::seek_set);
	else input->seek(end_of_directory.offset_to_start_of_central_directory, CL_InputSource::seek_set);
	
	cl_int64 num_entries = end_of_directory.number_of_entries_in_central_directory;
	if (zip64) num_entries = zip64_end_of_directory.number_of_entries_in_central_directory;

	for (int i=0; i<num_entries; i++)
	{
		CL_Zip_FileEntry entry;
		entry.impl->record.load(input);
		impl->files.push_back(entry);
	}

	delete input;
}

CL_Zip_Archive::CL_Zip_Archive(const CL_Zip_Archive &copy) : impl(copy.impl)
{
}
	
CL_Zip_Archive::~CL_Zip_Archive()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_Zip_Archive attributes:

std::vector<CL_Zip_FileEntry> &CL_Zip_Archive::get_file_list()
{
	return impl->files;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Zip_Archive operations:

CL_InputSource *CL_Zip_Archive::open_source(const std::string &filename)
{
	int size = impl->files.size();
	for (int i=0; i<size; i++)
	{
		CL_Zip_FileEntry &entry = impl->files[i];
		if (entry.impl->record.filename == filename)
		{
			switch (entry.impl->type)
			{
			case CL_Zip_FileEntry_Generic::type_file:
				return new CL_InputSource_Zip_FileEntry(impl->filename, entry);

			case CL_Zip_FileEntry_Generic::type_removed:
				break;

			case CL_Zip_FileEntry_Generic::type_added_memory:
				return new CL_InputSource_Memory(entry.impl->data);

			case CL_Zip_FileEntry_Generic::type_added_file:
				return new CL_InputSource_File(entry.impl->filename);
			}
			throw CL_Error(CL_String::format("Found but hates %1", filename));
		}
	}
	throw CL_Error(CL_String::format("Unable to find zip index %1", filename));
	return 0;
}

CL_InputSourceProvider *CL_Zip_Archive::clone()
{
	return new CL_Zip_Archive(*this);
}

std::string CL_Zip_Archive::get_pathname(const std::string &filename)
{
	return filename;
}

CL_InputSourceProvider *CL_Zip_Archive::create_relative(const std::string &path)
{
	// This is wrong. Should seperate the inputsource provider from CL_Zip_Archive,
	// but don't really bother doing it right now.
	// -- mbn may 10, 2003
	return clone();
}

CL_OutputSource *CL_Zip_Archive::create_file(const std::string &filename, bool compress)
{
	return 0;
}

void CL_Zip_Archive::add_file(const std::string &filename, bool compress)
{
}

void CL_Zip_Archive::save()
{
}
	
void CL_Zip_Archive::save(const std::string &filename)
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_Zip_Archive implementation:
