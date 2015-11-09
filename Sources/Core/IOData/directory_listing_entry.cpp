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

#include "Core/precomp.h"
#include "API/Core/IOData/directory_listing_entry.h"

namespace clan
{
	class DirectoryListingEntry_Impl
	{
	public:
		std::string get_filename()
		{
			return filename;
		}

		bool is_directory()
		{
			return directory;
		}

		bool is_hidden()
		{
			return hidden;
		}

		bool is_writable()
		{
			return writable;
		}

		bool is_readable()
		{
			return readable;
		}

		void set_filename(const std::string &value)
		{
			filename = value;
		}

		void set_directory(bool value)
		{
			directory = value;
		}

		void set_hidden(bool value)
		{
			hidden = value;
		}

		void set_writable(bool value)
		{
			writable = value;
		}

		void set_readable(bool value)
		{
			readable = value;
		}

	private:
		std::string filename;
		bool directory;
		bool hidden;
		bool writable;
		bool readable;
	};

	DirectoryListingEntry::DirectoryListingEntry()
		: impl(std::make_shared<DirectoryListingEntry_Impl>())
	{
	}

	DirectoryListingEntry::~DirectoryListingEntry()
	{

	}

	std::string DirectoryListingEntry::get_filename()
	{
		return impl->get_filename();
	}

	bool DirectoryListingEntry::is_directory()
	{
		return impl->is_directory();
	}

	bool DirectoryListingEntry::is_hidden()
	{
		return impl->is_hidden();
	}

	bool DirectoryListingEntry::is_writable()
	{
		return impl->is_writable();
	}

	bool DirectoryListingEntry::is_readable()
	{
		return impl->is_readable();
	}

	void DirectoryListingEntry::set_filename(const std::string &filename)
	{
		impl->set_filename(filename);
	}

	void DirectoryListingEntry::set_directory(bool value)
	{
		impl->set_directory(value);
	}

	void DirectoryListingEntry::set_hidden(bool value)
	{
		impl->set_hidden(value);
	}

	void DirectoryListingEntry::set_writable(bool value)
	{
		impl->set_writable(value);
	}

	void DirectoryListingEntry::set_readable(bool value)
	{
		impl->set_readable(value);
	}
}
