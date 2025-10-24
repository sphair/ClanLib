/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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


/////////////////////////////////////////////////////////////////////////////
// CL_VirtualDirectoryListing_Impl Class:

class CL_VirtualDirectoryListing_Impl
{
/// \name Construction
/// \{
public:
	CL_VirtualDirectoryListing_Impl(CL_VirtualFileSource *provider, const CL_String &path) : index(0) 
	{
		bool init_ok = provider->initialize_directory_listing(path);
		if (!init_ok)
		{
			throw CL_Exception("Cannot scan directory to initialize CL_VirtualDirectoryListing");
		}

		bool next = true;
		while (next)
		{
			CL_VirtualDirectoryListingEntry vdir_entry;
			next = provider->next_file(vdir_entry);
			list_entries.push_back(vdir_entry);
		}

	}

	~CL_VirtualDirectoryListing_Impl()
	{
	}


/// \}
/// \name Attributes
/// \{
public:
	CL_String get_filename()
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
	CL_VirtualDirectoryListingEntry current_entry;

	std::vector<CL_VirtualDirectoryListingEntry> list_entries;

	unsigned int index;
/// \}
};

/////////////////////////////////////////////////////////////////////////////
// CL_VirtualDirectoryListing Construction:

CL_VirtualDirectoryListing::CL_VirtualDirectoryListing(CL_VirtualFileSource *provider, const CL_String &path)
: impl(new CL_VirtualDirectoryListing_Impl(provider, path))
{
}

CL_VirtualDirectoryListing::~CL_VirtualDirectoryListing()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_VirtualDirectoryListing Attributes:

CL_String CL_VirtualDirectoryListing::get_filename()
{
	return impl->get_filename();
}

bool CL_VirtualDirectoryListing::is_directory()
{
	return impl->is_directory();
}

bool CL_VirtualDirectoryListing::is_hidden()
{
	return impl->is_hidden();
}

bool CL_VirtualDirectoryListing::is_writable()
{
	return impl->is_writable();
}

bool CL_VirtualDirectoryListing::is_readable()
{
	return impl->is_readable();
}

/////////////////////////////////////////////////////////////////////////////
// CL_VirtualDirectoryListing Operations:

bool CL_VirtualDirectoryListing::next()
{
	return impl->next();
}

/////////////////////////////////////////////////////////////////////////////
// CL_VirtualDirectoryListing Implementation:
