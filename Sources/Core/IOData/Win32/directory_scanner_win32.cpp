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

#include "Core/precomp.h"
#include "directory_scanner_win32.h"
#include "API/Core/Text/string_help.h"

namespace clan
{
	DirectoryScanner_Win32::DirectoryScanner_Win32()
		: handle(INVALID_HANDLE_VALUE), first_next(true)
	{
	}

	DirectoryScanner_Win32::~DirectoryScanner_Win32()
	{
		if (handle != INVALID_HANDLE_VALUE) FindClose(handle);
	}

	bool DirectoryScanner_Win32::scan(const std::string &pathname)
	{
		return scan(pathname, "*.*");
	}

	bool DirectoryScanner_Win32::scan(const std::string &pathname, const std::string& pattern)
	{
		if (handle != INVALID_HANDLE_VALUE)
		{
			FindClose(handle);
			handle = INVALID_HANDLE_VALUE;
		}
		/*
			// Find the full path of the directory we are about to search:
			int size_buffer = GetFullPathName(pathname, 0, 0, 0);
			LPWSTR buffer = new WCHAR[size_buffer];
			LPWSTR filepart = 0;
			GetFullPathNameW(pathname, size_buffer, buffer, &filepart);
			full_pathname = path_with_ending_slash(buffer);
			delete[] buffer;
			*/
		directory_path = path_with_ending_slash(pathname);

		// Start our search:
		std::string filename = directory_path + pattern;
		handle = FindFirstFile(StringHelp::utf8_to_ucs2(filename).c_str(), &fileinfo);
		first_next = true;
		return (handle != INVALID_HANDLE_VALUE);
	}

	std::string DirectoryScanner_Win32::get_directory_path()
	{
		return directory_path;
	}

	int DirectoryScanner_Win32::get_size()
	{
		return fileinfo.nFileSizeLow;
	}

	std::string DirectoryScanner_Win32::get_name()
	{
		if (first_next) return std::string();
		return StringHelp::ucs2_to_utf8(fileinfo.cFileName);
	}

	std::string DirectoryScanner_Win32::get_pathname()
	{
		if (first_next) return std::string();
		return directory_path + StringHelp::ucs2_to_utf8(fileinfo.cFileName);
	}

	bool DirectoryScanner_Win32::is_directory()
	{
		if (first_next) return false;
		return (fileinfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY;
	}

	bool DirectoryScanner_Win32::is_hidden()
	{
		if (first_next) return false;
		return (fileinfo.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) == FILE_ATTRIBUTE_HIDDEN;
	}

	bool DirectoryScanner_Win32::is_readable()
	{
		if (first_next) return false;

		HANDLE file = CreateFile(
			StringHelp::utf8_to_ucs2(get_pathname()).c_str(),
			GENERIC_READ,
			FILE_SHARE_READ,
			0,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING,
			0);

		if (file != INVALID_HANDLE_VALUE) CloseHandle(file);
		return file != INVALID_HANDLE_VALUE;
	}

	bool DirectoryScanner_Win32::is_writable()
	{
		if (first_next) return false;

		HANDLE file = CreateFile(
			StringHelp::utf8_to_ucs2(get_pathname()).c_str(),
			GENERIC_READ,
			FILE_SHARE_READ,
			0,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING,
			0);

		if (file != INVALID_HANDLE_VALUE) CloseHandle(file);
		return file != INVALID_HANDLE_VALUE;
	}

	bool DirectoryScanner_Win32::next()
	{
		if (handle == INVALID_HANDLE_VALUE) return false;
		if (first_next)
		{
			first_next = false;
			return true;
		}

		return FindNextFile(handle, &fileinfo) == TRUE;
	}

	std::string DirectoryScanner_Win32::path_with_ending_slash(const std::string &path)
	{
		int len = path.length();
		if (len == 0) return ".\\";
		if (path[len - 1] == '/' || path[len - 1] == '\\') return path;
		return path + "\\";
	}
}
