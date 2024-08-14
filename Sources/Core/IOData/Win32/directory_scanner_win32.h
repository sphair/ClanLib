/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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

#include "../directory_scanner_impl.h"

namespace clan
{
	class DirectoryScanner_Win32 : public DirectoryScanner_Impl
	{
	public:
		/// \brief Construct initial directory scanner.
		DirectoryScanner_Win32();

		~DirectoryScanner_Win32() override;

		/// \brief Scan for files in a directory.
		bool scan(const std::string &pathname) override;

		/// \brief Scan for files matching a pattern.
		bool scan(const std::string &pathname, const std::string &pattern) override;

		/// \brief Returns the path of the directory being scanned.
		std::string get_directory_path() override;

		/// \brief Returns the size of the current found file.
		int get_size() override;

		/// \brief Returns the name of the current found file.
		std::string get_name() override;

		/// \brief Returns the name of the current found file, including the directory path.
		std::string get_pathname() override;

		/// \brief Returns true if filename is a directory.
		bool is_directory() override;

		/// \brief Returns true if filename is hidden.
		bool is_hidden() override;

		/// \brief Returns true if filename is readable.
		bool is_readable() override;

		/// \brief Returns true if filename is writable.
		bool is_writable() override;

		/// \brief Find next file in directory scan. Returns false if no more files was found.
		bool next() override;

	private:
		std::string path_with_ending_slash(const std::string &path);

		WIN32_FIND_DATA fileinfo;
		HANDLE handle;
		bool first_next;
		std::string directory_path;
	};
}
