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
**    Harry Storbacka
*/

#pragma once

#include <memory>

namespace clan
{
	class DirectoryListingEntry_Impl;

	/// \brief Virtual File System (VFS) directory listing entry class.
	class DirectoryListingEntry
	{
	public:
		DirectoryListingEntry();
		virtual ~DirectoryListingEntry();

		/// \brief Returns the file name of the item in the listing.
		std::string get_filename();

		/// \brief Returns true if item is a directory.
		bool is_directory();

		/// \brief Returns true if item is hidden.
		bool is_hidden();

		/// \brief Returns true if item is writable.
		bool is_writable();

		/// \brief Returns true if item is readable.
		bool is_readable();

		void set_filename(const std::string &);
		void set_directory(bool);
		void set_hidden(bool);
		void set_writable(bool);
		void set_readable(bool);

	private:
		std::shared_ptr<DirectoryListingEntry_Impl> impl;
	};
}
