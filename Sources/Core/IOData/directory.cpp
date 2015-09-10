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
#include "API/Core/IOData/directory.h"
#include "API/Core/IOData/directory_scanner.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/System/system.h"

#ifdef __MINGW32__
#define _WIN32_IE 0x0500
#endif

#ifndef WIN32
#include <unistd.h>
#include <cstdio>
#include <pwd.h>

#ifndef PATH_MAX
#define PATH_MAX	256	// TODO: Fixme - Paths should not have a limit
#endif

#ifndef MAX_PATH
#define MAX_PATH PATH_MAX
#endif
#else
#include <shlobj.h>

#ifndef MAX_PATH
#define MAX_PATH _MAX_PATH
#endif

#ifdef _MSC_VER
#pragma comment(lib, "shell32.lib")
#endif
#endif

#include <sys/stat.h>
#include <sys/types.h>

namespace clan
{

	bool Directory::create(const std::string &dir_name, bool recursive)
	{
		if (dir_name.empty())
			return false;

		std::string full_path = PathHelp::add_trailing_slash(dir_name);

		bool result = true;
		if (recursive)
		{
			for (std::string::size_type pos = full_path.find_first_of("\\/"); pos != std::string::npos; pos = full_path.find_first_of("\\/", pos + 1))
			{
				std::string path = full_path.substr(0, pos);
#ifdef WIN32
				result = CreateDirectory(StringHelp::utf8_to_ucs2(path).c_str(), NULL) != 0;
#else
				result = mkdir(path.c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) == 0;
#endif
			}
		}
		else
		{
#ifdef WIN32
			result = CreateDirectory(StringHelp::utf8_to_ucs2(full_path).c_str(), NULL) != 0;
#else
			result = mkdir(full_path.c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) == 0;
#endif
		}

		return result;
	}

	bool Directory::remove(const std::string &dir_name, bool delete_files, bool delete_sub_directories)
	{
		if (dir_name.empty())
			return false;

		std::string full_path = PathHelp::add_trailing_slash(dir_name);

		if (delete_files || delete_sub_directories)
		{
			DirectoryScanner scanner;

			if (!scanner.scan(full_path))
				return false;

			bool scan_successful = scanner.next();
			while (scan_successful)
			{
				// if found subdirectory, try remove it, also checking for "." and "..", because they are unremovable
				if (scanner.is_directory() && delete_sub_directories && scanner.get_name() != "." && scanner.get_name() != "..")
				{
					// FIXME: directory scanner locks directory, so it can't be removed, this is workaround
					std::string sub_dir_path = scanner.get_pathname();
					scan_successful = scanner.next();

					// delete files in subdirectory
					if (!Directory::remove(sub_dir_path, delete_files, delete_sub_directories))
						return false;
				}
				else if (!scanner.is_directory() && delete_files)
				{
#ifdef WIN32
					if (DeleteFile(StringHelp::utf8_to_ucs2(scanner.get_pathname()).c_str()) == 0)
						return false;
#else
					if (::remove(scanner.get_pathname().c_str()) != 0)
						return false;
#endif
					scan_successful = scanner.next();
				}
				else
				{
					scan_successful = scanner.next();
				}
			}
		}

		// finally remove the directory (or subdirectory if in recursion)
#ifdef WIN32
		return RemoveDirectory(StringHelp::utf8_to_ucs2(full_path).c_str()) != 0;
#else
		return rmdir(full_path.c_str()) == 0;
#endif
	}

	bool Directory::rename(const std::string &old_name, const std::string &new_name)
	{
#ifdef WIN32
		return MoveFile(StringHelp::utf8_to_ucs2(old_name).c_str(), StringHelp::utf8_to_ucs2(new_name).c_str()) != 0;
#else
		return ::rename(old_name.c_str(), new_name.c_str()) == 0;
#endif
	}

	bool Directory::set_current(const std::string &dir_name)
	{
#ifdef WIN32
		return SetCurrentDirectory(StringHelp::utf8_to_ucs2(dir_name).c_str()) != 0;
#else
		return chdir(dir_name.c_str()) == 0;
#endif
	}

	std::string Directory::get_current()
	{
#ifdef WIN32
		DWORD len = GetCurrentDirectory(0, NULL);
		if (len == 0)
			throw Exception("GetCurrentDirectory failed!");

		std::vector<TCHAR> path(len);
		if (GetCurrentDirectory(len, &path[0]) == 0)
			throw Exception("GetCurrentDirectory failed!");
		return StringHelp::ucs2_to_utf8(&path[0]);
#else
		char cwd_buffer[MAX_PATH];
		if (getcwd(cwd_buffer, MAX_PATH) == nullptr)
			throw Exception("Working dir is more than legal length !");
		return cwd_buffer;
#endif
	}

#ifndef __APPLE__
	std::string Directory::get_appdata(const std::string &company_name, const std::string &application_name, const std::string &version, bool create_dirs_if_missing)
	{
		std::string configuration_path;

#if defined(WIN32)
		TCHAR app_data[MAX_PATH];
		if (FAILED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_DEFAULT, app_data)))
			throw Exception("SHGetFolderPath failed!");
		configuration_path = PathHelp::add_trailing_slash(StringHelp::ucs2_to_utf8(app_data));
#else
		struct passwd *pwd = getpwuid(getuid());
		if (pwd == nullptr || pwd->pw_dir == nullptr)
			throw Exception("getpwuid failed!");
		configuration_path = PathHelp::add_trailing_slash(pwd->pw_dir) + ".";
#endif

		if (!company_name.empty())
			configuration_path += PathHelp::add_trailing_slash(company_name);
		if (!application_name.empty())
			configuration_path += PathHelp::add_trailing_slash(application_name);
		if (!version.empty())
			configuration_path += PathHelp::add_trailing_slash(version);

		if (create_dirs_if_missing)
			create(configuration_path, true);

		return configuration_path;
	}

	std::string Directory::get_local_appdata(const std::string &company_name, const std::string &application_name, const std::string &version, bool create_dirs_if_missing)
	{
		std::string configuration_path;

#if defined(WIN32)
		TCHAR app_data[MAX_PATH];
		if (FAILED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_DEFAULT, app_data)))
			throw Exception("SHGetFolderPath failed!");
		configuration_path = PathHelp::add_trailing_slash(StringHelp::ucs2_to_utf8(app_data));
#elif defined(__APPLE__)
		NSArray *paths = NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES);
		NSString *libraryDirectory = [paths objectAtIndex:0];
		const char *s = [libraryDirectory UTF8String];
		throw Exception("Congratulations, you got the task to implement Directory::get_local_appdata on this platform.");
#else
		return get_appdata(company_name, application_name, version, create_dirs_if_missing);
#endif

		if (!company_name.empty())
			configuration_path += PathHelp::add_trailing_slash(company_name);
		if (!application_name.empty())
			configuration_path += PathHelp::add_trailing_slash(application_name);
		if (!version.empty())
			configuration_path += PathHelp::add_trailing_slash(version);

		if (create_dirs_if_missing)
			create(configuration_path, true);

		return configuration_path;
	}

	std::string Directory::get_resourcedata(const std::string &application_name, const std::string &data_dir_name)
	{
#if defined(WIN32)
		std::string exe_path = System::get_exe_path();
		if (!data_dir_name.empty())
			exe_path += data_dir_name + '\\';
		return exe_path;
#elif defined(__APPLE__)
		
		CFBundleRef bundle = CFBundleGetMainBundle();
		CFURLRef url = CFBundleCopyResourcesDirectoryURL(bundle);
		CFStringRef str = CFURLCopyPath(url);
		
		char buffer[4096];
		Boolean result = CFStringGetCString(str, buffer, 4096, kCFStringEncodingUTF8);
		if (!result)
		{
			CFRelease(str);
			CFRelease(url);
			throw Exception("Bundle resource path is too long!");
		}
		
		std::string resource_path = PathHelp::combine(buffer, data_dir_name);
		
		CFRelease(str);
		CFRelease(url);
		
		return resource_path;
#else
		std::string exe_path = System::get_exe_path();
		if (exe_path.length() > 1)
		{
			std::string::size_type pos = exe_path.rfind('/', exe_path.length() - 2);
			if (pos != std::string::npos)
				return exe_path.substr(0, pos + 1) + "share/" + application_name + '/';
		}

		if (!data_dir_name.empty())
			exe_path += data_dir_name + '/';
		return exe_path;
#endif
	}
#endif
}
