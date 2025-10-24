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
#include "API/Core/IOData/virtual_directory_listing_entry.h"

/////////////////////////////////////////////////////////////////////////////
// CL_VirtualDirectoryListingEntry_Impl Class:

class CL_VirtualDirectoryListingEntry_Impl
{
//! Construction:
public:
	CL_VirtualDirectoryListingEntry_Impl()
	{
	}

	~CL_VirtualDirectoryListingEntry_Impl()
	{
	}

//! Attributes:
public:
	CL_String get_filename()
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

//! Operations:
public:
	void set_filename(const CL_String &value)
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


//! Implementation:
private:
	CL_String filename;
	bool directory;
	bool hidden;
	bool writable;
	bool readable;
};

/////////////////////////////////////////////////////////////////////////////
// CL_VirtualDirectoryListingEntry Construction:

CL_VirtualDirectoryListingEntry::CL_VirtualDirectoryListingEntry()
: impl(new CL_VirtualDirectoryListingEntry_Impl)
{
}

CL_VirtualDirectoryListingEntry::~CL_VirtualDirectoryListingEntry()
{
	
}

/////////////////////////////////////////////////////////////////////////////
// CL_VirtualDirectoryListingEntry Attributes:

CL_String CL_VirtualDirectoryListingEntry::get_filename()
{
	return impl->get_filename();
}

bool CL_VirtualDirectoryListingEntry::is_directory()
{
	return impl->is_directory();
}

bool CL_VirtualDirectoryListingEntry::is_hidden()
{
	return impl->is_hidden();
}

bool CL_VirtualDirectoryListingEntry::is_writable()
{
	return impl->is_writable();
}

bool CL_VirtualDirectoryListingEntry::is_readable()
{
	return impl->is_readable();
}

/////////////////////////////////////////////////////////////////////////////
// CL_VirtualDirectoryListingEntry Operations:

void CL_VirtualDirectoryListingEntry::set_filename(const CL_String &filename)
{
	impl->set_filename(filename);
}

void CL_VirtualDirectoryListingEntry::set_directory(bool value)
{
	impl->set_directory(value);
}

void CL_VirtualDirectoryListingEntry::set_hidden(bool value)
{
	impl->set_hidden(value);
}

void CL_VirtualDirectoryListingEntry::set_writable(bool value)
{
	impl->set_writable(value);
}

void CL_VirtualDirectoryListingEntry::set_readable(bool value)
{
	impl->set_readable(value);
}

/////////////////////////////////////////////////////////////////////////////
// CL_VirtualDirectoryListingEntry Implementation:
