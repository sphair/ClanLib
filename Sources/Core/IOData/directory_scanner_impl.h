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
*/

#pragma once

namespace clan
{
	class DirectoryScanner_Impl
	{
	public:
		virtual ~DirectoryScanner_Impl() { }

		virtual bool scan(const std::string& pathname) = 0;
		virtual bool scan(const std::string& pathname, const std::string& pattern) = 0;

		/// \brief Returns the path of the directory being scanned.
		virtual std::string get_directory_path() = 0;

		/// \brief Returns the name of the current found file.
		virtual std::string get_name() = 0;

		/// \brief Returns the size of the current found file.
		virtual int get_size() = 0;

		/// \brief Returns the name of the current found file, including the directory path.
		virtual std::string get_pathname() = 0;

		/// \brief Returns true if filename is a directory.
		virtual bool is_directory() = 0;

		/// \brief Returns true if filename is hidden.
		virtual bool is_hidden() = 0;

		/// \brief Returns true if file is readable by current user.
		virtual bool is_readable() = 0;

		/// \brief Returns true if file is writable by current user.
		virtual bool is_writable() = 0;

		/// \brief Find next file in directory scan. Returns false if no more files was found.
		virtual bool next() = 0;
	};
}
