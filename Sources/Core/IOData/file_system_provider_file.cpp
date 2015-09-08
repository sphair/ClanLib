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
#include "file_system_provider_file.h"
#include "API/Core/IOData/file.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/IOData/directory_listing_entry.h"

namespace clan
{
	FileSystemProvider_File::FileSystemProvider_File(const std::string &path)
		: path(PathHelp::add_trailing_slash(path, PathHelp::path_type_file))
	{
	}

	FileSystemProvider_File::~FileSystemProvider_File()
	{
	}

	std::string FileSystemProvider_File::get_path() const
	{
		return path;
	}

	std::string FileSystemProvider_File::get_identifier() const
	{
		return path;
	}

	IODevice FileSystemProvider_File::open_file(const std::string &filename,
		File::OpenMode mode,
		unsigned int access,
		unsigned int share,
		unsigned int flags)
	{
		return File(path + filename, mode, access, share, flags);
	}

	bool FileSystemProvider_File::initialize_directory_listing(const std::string &additionalpath)
	{
		return dir_scanner.scan(PathHelp::combine(this->path, additionalpath));
	}

	bool FileSystemProvider_File::next_file(DirectoryListingEntry &entry)
	{
		bool next = dir_scanner.next();

		if (next)
		{
			entry.set_directory(dir_scanner.is_directory());
			entry.set_filename(dir_scanner.get_name());
			entry.set_hidden(dir_scanner.is_hidden());
			entry.set_readable(dir_scanner.is_readable());
			entry.set_writable(dir_scanner.is_writable());
		}

		return next;
	}
}
