/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
#include "API/Core/IOData/virtual_directory_listing.h"
#include "API/Core/IOData/virtual_file_source.h"
#include "API/Core/IOData/virtual_directory_listing_entry.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// VirtualDirectoryListing_Impl Class:

class VirtualDirectoryListing_Impl
{
/// \name Construction
/// \{
public:
	VirtualDirectoryListing_Impl(VirtualFileSource *provider, const std::string &path) : index(0) 
	{
		bool init_ok = provider->initialize_directory_listing(path);
		if (!init_ok)
		{
			throw Exception("Cannot scan directory to initialize VirtualDirectoryListing");
		}

		bool next;
		do 
		{
			VirtualDirectoryListingEntry vdir_entry;
			next = provider->next_file(vdir_entry);
			if(next)
				list_entries.push_back(vdir_entry);
		} while (next);
	}

	~VirtualDirectoryListing_Impl()
	{
	}

/// \}
/// \name Attributes
/// \{
public:
	std::string get_filename()
	{
		return current_entry.get_filename();	
	}

	bool is_hidden()
	{
		return current_entry.is_hidden();	
	}

	bool is_readable()
	{
		return current_entry.is_readable();	
	}

	bool is_writable()
	{
		return current_entry.is_writable();	
	}

	bool is_directory()
	{
		return current_entry.is_directory();	
	}

/// \}
/// \name Operations
/// \{
public:
	bool next()
	{
		if (index < list_entries.size())
		{
			current_entry = list_entries[index];
			index++;
			return true;
		}
		return false;
	}

/// \}
/// \name Implementation
/// \{
private:
	VirtualDirectoryListingEntry current_entry;

	std::vector<VirtualDirectoryListingEntry> list_entries;

	unsigned int index;
/// \}
};

/////////////////////////////////////////////////////////////////////////////
// VirtualDirectoryListing Construction:

VirtualDirectoryListing::VirtualDirectoryListing(VirtualFileSource *provider, const std::string &path)
: impl(new VirtualDirectoryListing_Impl(provider, path))
{
}

VirtualDirectoryListing::VirtualDirectoryListing()
{
	// NULL instance
}


VirtualDirectoryListing::~VirtualDirectoryListing()
{
}

/////////////////////////////////////////////////////////////////////////////
// VirtualDirectoryListing Attributes:

void VirtualDirectoryListing::throw_if_null() const
{
	if (!impl)
		throw Exception("VirtualDirectoryListing is null");
}

std::string VirtualDirectoryListing::get_filename()
{
	return impl->get_filename();
}

bool VirtualDirectoryListing::is_directory()
{
	return impl->is_directory();
}

bool VirtualDirectoryListing::is_hidden()
{
	return impl->is_hidden();
}

bool VirtualDirectoryListing::is_writable()
{
	return impl->is_writable();
}

bool VirtualDirectoryListing::is_readable()
{
	return impl->is_readable();
}

/////////////////////////////////////////////////////////////////////////////
// VirtualDirectoryListing Operations:

bool VirtualDirectoryListing::next()
{
	return impl->next();
}

/////////////////////////////////////////////////////////////////////////////
// VirtualDirectoryListing Implementation:

}
