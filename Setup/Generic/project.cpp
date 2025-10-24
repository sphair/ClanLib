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
*/

// disable debug info truncation warnings caused by STL
#if defined(_MSC_VER)
#pragma warning(disable: 4786)
#endif

#ifdef WIN32
#include <windows.h>
#endif

#include "project.h"

/////////////////////////////////////////////////////////////////////////////
// Project files exclude list:

static const char *exclude_from_build[] =
{
	".",
	"..",
	"CVS",
	".svn",
	"Be",
	"X11",
	"GGI",
	"Svgalib",
	"FBDev",
	"DirectFB",
	"TTY",
	"Unix",
	"Makefile.am",
	"oss.cpp",
	"cdaudio_linux.cpp",
	"DirectSound",
	"GLX",
	"blit_gl_dynamic.cpp", // not working yet (dv)
	"setupnetwork_unix.cpp",
	"Mac",
	"MacOS",
	"MacOSX",
	"Quartz",
	"OSX",
	"AGL",
	NULL
};

/////////////////////////////////////////////////////////////////////////////
// Project construction:

Project::Project()
{
}

Project::Project(
	const std::string &name,
	const std::string &libname,
	const std::string &headername,
	const std::list<std::string> &libs_list_shared,
	const std::list<std::string> &libs_list_release,
	const std::list<std::string> &libs_list_debug,
	const std::list<std::string> &defines_list,
	const std::list<std::string> &ignore_list)
:
	name(name),
	libname(libname),
	headername(headername),
	libs_shared(libs_list_shared),
	libs_release(libs_list_release),
	libs_debug(libs_list_debug),
	defines_list(defines_list)
{
#ifdef _MSC_VER
	std::string lib_main_header;
	lib_main_header = std::string("Sources\\API\\") + headername;
 	files.push_back(lib_main_header);

	generate_dir(std::string("Sources\\API\\")+name, ignore_list);
#endif
	generate_dir(std::string("Sources\\")+name, ignore_list);
}

/////////////////////////////////////////////////////////////////////////////
// Project attributes:

/////////////////////////////////////////////////////////////////////////////
// Project operations:

/////////////////////////////////////////////////////////////////////////////
// Project implementation:

void Project::generate_dir(
	const std::string &dir,
	const std::list<std::string> &ignore_list)
{
	std::string path = dir;
	if (path[path.length()-1] != '\\') path += '\\';

#ifdef __MINGW32__
	for(int i=0;i<path.length();i++)
	    if(path[i]=='\\')
			path[i]='/';
#endif

	WIN32_FIND_DATAA	data;
	HANDLE handle = FindFirstFileA(std::string(path + "*.*").c_str(), &data);
	if (handle == INVALID_HANDLE_VALUE) return;

	do
	{
		bool skip = false;

		if (strncmp(data.cFileName, ".#", 2) == 0) continue; // don't add CVS revision backups.
		if (strchr(data.cFileName, '~') != NULL) continue;  //Don't get those emacs/bcc backup files

#ifdef __BORLANDC__
//		if (strstr(data.cFileName, ".cpp") == NULL) continue;
		if (strstr(data.cFileName, ".s") != NULL) continue;
		if (strstr(data.cFileName, ".h") != NULL) continue;
		if (strstr(data.cFileName, ".nasm") != NULL) continue;
		if (strstr(data.cFileName, ".asm") != NULL) continue;
		if (strstr(data.cFileName, ".txt") != NULL) continue;
		if (strstr(data.cFileName, "README") != NULL) continue;
		if (strstr(data.cFileName, "Makefile") != NULL) continue;
		if (strstr(data.cFileName, ".pkg") != NULL) continue;
		if (strstr(data.cFileName, ".m4") != NULL) continue;
		//Only snatch .cpp files
#endif

		for (int i=0; exclude_from_build[i] != NULL; i++)
			if (stricmp(data.cFileName, exclude_from_build[i]) == 0) skip = true;

		std::list<std::string>::const_iterator it;
		for (it = ignore_list.begin(); it != ignore_list.end(); it++)
		{
			if (stricmp(data.cFileName, it->c_str()) == 0)
			skip = true;
		}

		if (skip)
    		continue;

		if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
		{
			generate_dir(path + data.cFileName, ignore_list);
		}
		else
		{
#ifdef __BORLANDC__
			std::string tmp = path;
			if(tmp[tmp.length()-1] == '\\')
				tmp.erase((tmp.end()-1));
			if(paths.empty())
				paths.push_back(tmp);
			else
				if(!(paths.back() == tmp))
					paths.push_back(tmp);
			char *ext = strstr(data.cFileName, ".cpp");
			if(ext == NULL)
				continue;
			*(ext++) = 0;
			*(ext++) = 0;
			*(ext++) = 0;
			*(ext++) = 0;           
			files.push_back(data.cFileName);
			file_paths.push_back(path  +  data.cFileName);
#else
			files.push_back(path + data.cFileName);
#endif
		}

	} while (FindNextFileA(handle, &data));
}

