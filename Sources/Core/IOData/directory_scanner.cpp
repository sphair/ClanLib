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

#include "Core/precomp.h"
#include "API/Core/IOData/directory_scanner.h"
#include "directory_scanner_impl.h"

#ifdef WIN32
#include "Win32/directory_scanner_win32.h"
#else
#include "Unix/directory_scanner_unix.h"
#endif

#ifndef WIN32
#include <string.h>
#endif

namespace clan
{
	DirectoryScanner::DirectoryScanner()
	{
#ifdef WIN32
		impl = std::make_shared<DirectoryScanner_Win32>();
#else
		impl = std::make_shared<DirectoryScanner_Unix>();
#endif
	}

	DirectoryScanner::~DirectoryScanner()
	{
	}

	bool DirectoryScanner::scan(const std::string& pathname)
	{
		return impl->scan(pathname);
	}

	bool DirectoryScanner::scan(const std::string& pathname, const std::string& pattern)
	{
		return impl->scan(pathname, pattern);
	}

	std::string DirectoryScanner::get_directory_path()
	{
		return impl->get_directory_path();
	}

	int DirectoryScanner::get_size()
	{
		return impl->get_size();
	}

	std::string DirectoryScanner::get_name()
	{
		return impl->get_name();
	}

	std::string DirectoryScanner::get_pathname()
	{
		return impl->get_pathname();
	}

	bool DirectoryScanner::is_directory()
	{
		return impl->is_directory();
	}

	bool DirectoryScanner::is_hidden()
	{
		return impl->is_hidden();
	}

	bool DirectoryScanner::is_readable()
	{
		return impl->is_readable();
	}

	bool DirectoryScanner::is_writable()
	{
		return impl->is_writable();
	}

	bool DirectoryScanner::next()
	{
		return impl->next();
	}
}
