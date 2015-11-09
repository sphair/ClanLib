/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
**    Harry Storbacka
*/

#include "Core/precomp.h"
#include "file_system_provider_zip.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Core/IOData/directory_listing_entry.h"
#include "API/Core/Text/string_format.h"

namespace clan
{
	int FileSystemProvider_Zip::zip_source_unique_id = 0;

	FileSystemProvider_Zip::FileSystemProvider_Zip(const ZipArchive &zip_archive)
		: zip_archive(zip_archive), index(0)
	{
		zip_source_unique_id++;
	}

	FileSystemProvider_Zip::~FileSystemProvider_Zip()
	{
	}

	std::string FileSystemProvider_Zip::get_path() const
	{
		return path;
	}

	std::string FileSystemProvider_Zip::get_identifier() const
	{
		// Ideally we should identify from the archive source (checksum etc)
		// I'm sure someone will implement it if it is required
		return path + string_format("/%1", zip_source_unique_id);
	}

	IODevice FileSystemProvider_Zip::open_file(const std::string &filename,
		File::OpenMode mode,
		unsigned int access,
		unsigned int share,
		unsigned int flags)
	{
		return zip_archive.open_file(filename);
	}

	bool FileSystemProvider_Zip::initialize_directory_listing(const std::string &path)
	{
		file_list = zip_archive.get_file_list(path);
		directory_list_path = path;
		index = 0;

		return true;	// Empty directories should be valid
	}

	bool FileSystemProvider_Zip::next_file(DirectoryListingEntry &entry)
	{
		if (file_list.empty())
			return false;

		if (index > file_list.size() - 1)
			return false;

		std::string filename = file_list[index].get_archive_filename();
		if (filename[0] == '/')
			filename = filename.substr(1, std::string::npos);

		entry.set_filename(filename);
		entry.set_readable(true);
		entry.set_directory(file_list[index].is_directory());
		entry.set_hidden(false); // todo
		entry.set_writable(false); // todo
		index++;

		return true;
	}
}
