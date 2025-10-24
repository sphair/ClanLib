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
#include "API/Core/System/cl_assert.h"
#include "API/Core/IOData/directory_scanner.h"
#include "directory_scanner_generic.h"

#ifdef WIN32
#include "../Win32/directory_scanner_win32.h"
#else
#include "../Unix/directory_scanner_unix.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_DirectoryScanner construction:

CL_DirectoryScanner::CL_DirectoryScanner()
{
#ifdef WIN32
	impl = new CL_DirectoryScanner_Win32();
#else
	impl = new CL_DirectoryScanner_Unix();
#endif
}

CL_DirectoryScanner::CL_DirectoryScanner(const CL_DirectoryScanner &copy) : impl(copy.impl)
{
}

CL_DirectoryScanner::~CL_DirectoryScanner()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_DirectoryScanner attributes:

bool CL_DirectoryScanner::scan(const std::string& pathname) 
{
	return impl->scan (pathname);
}

bool CL_DirectoryScanner::scan(const std::string& pathname, const std::string& pattern)
{
	return impl->scan (pathname, pattern);
}

std::string CL_DirectoryScanner::get_directory_path()
{
	return impl->get_directory_path();
}

int CL_DirectoryScanner::get_size()
{
	return impl->get_size();
}

std::string CL_DirectoryScanner::get_name()
{
	return impl->get_name();
}
	
std::string CL_DirectoryScanner::get_pathname()
{
	return impl->get_pathname();
}
	
bool CL_DirectoryScanner::is_directory()
{
	return impl->is_directory();
}

bool CL_DirectoryScanner::is_hidden()
{
	return impl->is_hidden();
}     

bool CL_DirectoryScanner::is_readable()
{
	return impl->is_readable();
}

 bool CL_DirectoryScanner::is_writable()
{
	return impl->is_writable();
}
/////////////////////////////////////////////////////////////////////////////
// CL_DirectoryScanner operations:

bool CL_DirectoryScanner::next() 
{ 
	return impl->next(); 
}
