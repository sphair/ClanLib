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
**    (if your name is missing here, please add it)
*/

#include "Core/precomp.h"
#include "API/Core/IOData/directory.h"
#include "API/Core/IOData/directory_scanner.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"

#if defined(WIN32)
#include <shlobj.h>
#if defined(_MSC_VER)
#pragma comment(lib, "shell32.lib")
#endif
#endif

#ifndef WIN32
#include <unistd.h>
#include <cstdio>

#ifndef PATH_MAX
#define PATH_MAX	256	// TODO: Fixme - Paths should not have a limit
#endif

#ifndef MAX_PATH
#define MAX_PATH PATH_MAX
#endif
#else
#include <direct.h>
#ifndef chdir
#define _chdir chdir
#endif
#ifndef MAX_PATH
#define _MAX_PATH MAX_PATH
#endif
#include <tchar.h>
#endif

#ifdef __BORLANDC__
#include <dir.h>
#endif

#include <sys/stat.h>
#include <sys/types.h>

#include <cstdlib>

/////////////////////////////////////////////////////////////////////////////
// Operations

bool CL_Directory::create(const CL_String &dir_name)
{
	if (dir_name.empty())
		return false;

	// this will be a full path
	CL_String full_path; // calculate the full path

	#ifdef WIN32
		DWORD buff_len = ::GetFullPathName(CL_StringHelp::utf8_to_ucs2(dir_name).c_str(), 0, 0, 0);

		if (buff_len == 0)
			// can't calculate, return bad status
			return false;
		else
		{
			std::vector<TCHAR> buffer_vector;
			buffer_vector.resize(buff_len + 1);
			TCHAR *buffer = &(buffer_vector[0]);
			TCHAR *buffer_ptr_to_filename = 0;
			// Obtaining full path
			buff_len = ::GetFullPathName(CL_StringHelp::utf8_to_ucs2(dir_name).c_str(), buff_len, buffer, &buffer_ptr_to_filename);
			if (buff_len == 0)
				// can't obtain full path, return bad status
				return false;
			else
				// ok, save it
				full_path = buffer;
		}
	#else
		// TODO: add here Linux version of GetFullPathName
		full_path = dir_name;
	#endif

#ifdef WIN32
		return ::CreateDirectory(CL_StringHelp::utf8_to_ucs2(full_path).c_str(), NULL) != 0;
#else
		return ::mkdir(full_path.c_str(), 0755) == 0;
#endif
}

bool CL_Directory::remove(const CL_String &dir_name, bool delete_files, bool delete_sub_directories)
{
	if (dir_name.empty())
		return false;

	// this will be a full path
	CL_String full_path;

	// calculate the full path
	#ifdef WIN32
		DWORD buff_len = ::GetFullPathName(CL_StringHelp::utf8_to_ucs2(dir_name).c_str(), 0, 0, 0);

		if (buff_len == 0)
			// can't calculate, return bad status
			return false;
		else
		{
			std::vector<TCHAR> buffer_vector;
			buffer_vector.resize(buff_len + 1);
			TCHAR *buffer = &(buffer_vector[0]);
			TCHAR *buffer_ptr_to_filename = 0;
			// Obtaining full path
			buff_len = ::GetFullPathName(CL_StringHelp::utf8_to_ucs2(dir_name).c_str(), buff_len, buffer, &buffer_ptr_to_filename);
			if (buff_len == 0)
				// can't obtaing full path, return bad status
				return false;
			else
				// ok, save it
				full_path = buffer;
		}
	#else
		// TODO: add here Linux version of GetFullPathName
		full_path = dir_name;
	#endif

	// This scope needed for deleting directiory at end of function,
	// because scanner lock current dir :(
	{
		CL_DirectoryScanner scanner;

		if (!scanner.scan(full_path))
			// can't even start scaning
			return false;

		// FIXME: probably bug in directory_scanner, it return ""
		// for first file :(
		if (scanner.next())
		while(true)
		{
			// If found sub_directory, try remove it,
			// also checking for "." and "..", because they are unremovable
			if (scanner.is_directory() && delete_sub_directories &&
				scanner.get_name() != "." && scanner.get_name() != "..")
			{
				// FIXME: directory_scanner lock directory, so it can't be
				// removed, this is workaround
				CL_String sub_dir_path = scanner.get_pathname();
				bool scann_successfull = scanner.next();

				// delete files in sub_directory
				if (!CL_Directory::remove(sub_dir_path.c_str(),
							  delete_files,
							  delete_sub_directories))
					return false;

				if (!scann_successfull)
					break;
				else
					continue;
			}
			else
			{
				// Check for deleting file (or whatever is not directory),
				// if this is allowed
				if (delete_files && !scanner.is_directory())
				{
					// delete a file
					#ifdef WIN32
						if (::DeleteFile(CL_StringHelp::utf8_to_ucs2(scanner.get_pathname()).c_str()) == 0)
							return false;
					#else
						if (::remove(scanner.get_pathname().c_str()) != 0)
							return false;
					#endif
					if (!scanner.next())
						break;
				}
				// This is for "." and ".."
				else
				{
					if (!scanner.next())
						break;
				}
			}
		}
	}
	// Finaly remove the directory (or sub_directory if in recursion)
	#ifdef WIN32
		return ::RemoveDirectory(CL_StringHelp::utf8_to_ucs2(full_path).c_str()) != 0;
	#else
		return ::rmdir(full_path.c_str()) == 0;
	#endif
}

bool CL_Directory::set_current(const CL_String &dir_name)
{
#ifdef WIN32
	return SetCurrentDirectory(CL_StringHelp::utf8_to_ucs2(dir_name).c_str()) == TRUE;
#else
	return chdir(dir_name.c_str()) == 0;
#endif
}

CL_String CL_Directory::get_current()
{
#ifdef WIN32
	TCHAR cwd_buffer[MAX_PATH];
#ifdef _CRT_INSECURE_DEPRECATE
	if (_tgetcwd(cwd_buffer, MAX_PATH) == NULL)
		throw CL_Exception("Working dir is more than legal length !");
#else
	if (_tgetcwd(cwd_buffer, MAX_PATH) == NULL)
		throw CL_Exception("Working dir is more than legal length !");
#endif
#else
	char cwd_buffer[MAX_PATH];
	if (getcwd(cwd_buffer, MAX_PATH) == NULL)
		throw CL_Exception("Working dir is more than legal length !");
#endif
	return cwd_buffer;
}

CL_String CL_Directory::get_appdata(const CL_StringRef &company_name, const CL_StringRef &application_name, const CL_StringRef &version, bool create_dirs_if_missing)
{
#if defined(WIN32)
	TCHAR app_data[MAX_PATH];
	if (FAILED(SHGetFolderPath(0, CSIDL_APPDATA, 0, SHGFP_TYPE_DEFAULT, app_data)))
		throw CL_Exception("SHGetFolderPath failed!");
	CL_String configuration_path = cl_format("%1\\%2\\%3\\%4\\", app_data, company_name, application_name, version);
	if (create_dirs_if_missing)
	{
		CL_String::size_type prevPos = 0;
		while (true)
		{
			CL_String::size_type pos = configuration_path.find_first_of("\\/", prevPos);
			if (pos == CL_String::npos)
				break;

			CL_StringRef folder = configuration_path.substr(0, pos);
			CreateDirectory(CL_StringHelp::utf8_to_ucs2(folder).c_str(), 0);

			prevPos = pos + 1;
		}
	}
	return configuration_path;
#elif defined(__APPLE__)
	throw CL_Exception("Congratulations, you got the task to implement CL_Directory::get_appdata on this platform.");
#else
	const char *home_dir = getenv("HOME");
	if (home_dir == NULL)
		throw CL_Exception("Cannot object $HOME environment variable");

	if (!create_dirs_if_missing)
	{
		return cl_format("%1/.%2/%3/%4/", home_dir, company_name, application_name, version);
	}

	struct stat stFileInfo;
	CL_String name( cl_format("%1/.%2", home_dir, company_name) );

	if (stat(name.c_str(), &stFileInfo))
	{
		if (::mkdir(name.c_str(), 0755))
			throw CL_Exception(cl_format("Cannot create %1 directory", name));	
	}

	name = cl_format("%1/%2", name, application_name);
	if (stat(name.c_str(), &stFileInfo))
	{
		if (::mkdir(name.c_str(), 0755))
			throw CL_Exception(cl_format("Cannot create %1 directory", name));	
	}

	name = cl_format("%1/%2", name, version);
	if (stat(name.c_str(), &stFileInfo))
	{
		if (::mkdir(name.c_str(), 0755))
			throw CL_Exception(cl_format("Cannot create %1 directory", name));	
	}

	name = cl_format("%1/", name);
	return name;
#endif
}

CL_String CL_Directory::get_local_appdata(const CL_StringRef &company_name, const CL_StringRef &application_name, const CL_StringRef &version, bool create_dirs_if_missing)
{
#if defined(WIN32)
	TCHAR app_data[MAX_PATH];
	if (FAILED(SHGetFolderPath(0, CSIDL_LOCAL_APPDATA, 0, SHGFP_TYPE_DEFAULT, app_data)))
		throw CL_Exception("SHGetFolderPath failed!");
	CL_String configuration_path = cl_format("%1\\%2\\%3\\%4\\", app_data, company_name, application_name, version);
	if (create_dirs_if_missing)
	{
		CL_String::size_type prevPos = 0;
		while (true)
		{
			CL_String::size_type pos = configuration_path.find_first_of("\\/", prevPos);
			if (pos == CL_String::npos)
				break;

			CL_StringRef folder = configuration_path.substr(0, pos);
			CreateDirectory(CL_StringHelp::utf8_to_ucs2(folder).c_str(), 0);

			prevPos = pos + 1;
		}
	}
	return configuration_path;
#elif defined(__APPLE__)
	throw CL_Exception("Congratulations, you got the task to implement CL_Directory::get_local_appdata on this platform.");
#else
	return get_appdata(company_name, application_name, version, create_dirs_if_missing);
#endif
}

CL_String CL_Directory::get_resourcedata(const CL_StringRef &application_name)
{
#if defined(WIN32)
	TCHAR exe_filename[MAX_PATH];
	DWORD result = GetModuleFileName(0, exe_filename, MAX_PATH);
	if (result == 0 || result == MAX_PATH)
		throw CL_Exception("GetModuleFileName failed!");
	TCHAR drive[MAX_PATH], dir[MAX_PATH], filename[MAX_PATH], extension[MAX_PATH];
	#ifdef _CRT_INSECURE_DEPRECATE
		_tsplitpath_s(exe_filename, drive, MAX_PATH, dir, MAX_PATH, filename, MAX_PATH, extension, MAX_PATH);
	#else
		_tsplitpath(exe_filename, drive, dir, filename, extension);
	#endif
	return cl_format("%1%2Resources\\", drive, dir);
#elif defined(__APPLE__)
	throw CL_Exception("Congratulations, you got the task to implement CL_Directory::get_resourcedata on this platform.");
#else
	//TODO: 
	/// In Linux, this function will return the directory "../share/application_name/"
	///    relative to the executable, so if it is located in "/usr/bin" it will return
	///    "/usr/share/application_name/"
	/// (Assuming that is correct!)

	return "Resources/";
#endif
}

