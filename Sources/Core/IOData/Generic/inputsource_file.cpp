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

#ifdef WIN32
#include <direct.h>
#else
#ifndef getcwd
#include <unistd.h>
#endif
#endif
#include <cstring>
#include <API/Core/IOData/inputsource_file.h>

CL_InputSource_File::CL_InputSource_File(const std::string &_filename)
{
	filename = _filename;
	filehandle = NULL;
	open();
}

CL_InputSource_File::CL_InputSource_File(const CL_InputSource_File *source)
{
	filename = source->filename;
	filehandle = NULL;

	open();
	fseek(filehandle, ftell(source->filehandle), SEEK_SET);
}

CL_InputSource_File::~CL_InputSource_File()
{
	close();
}

std::string CL_InputSource_File::translate_path(const std::string &path)
{
	int len = 0;

	// try to figure out if path is absolute.
	if (path.length() > 0 && (path[0] == '\\' || path[0] == '/' || (path.length() > 1 && path[1] == ':')))
	{
		// path is absolute
		return path;
	}
	else
	{

		//note, I moved BigZaphod's mac path hack to Application/MacOS/clanapp.cpp -mrfun may 18 2006
		
		char cwd[1026];
		if (getcwd(cwd, 1024) == NULL) throw CL_Error("Working dir is more than 1024 characters!");
		len = strlen(cwd);
		if (cwd[len-1] != '/' && cwd[len-1] != '\\') strcat(cwd, "/");
		
		return std::string(cwd) + std::string(path);
	}
}

int CL_InputSource_File::read(void *data, int size)
{
	return fread(data, 1, size, filehandle);
}

void CL_InputSource_File::open()
{
	if (filehandle != NULL) return;

#ifndef WIN32 // hate win32 non posix conform
	if (filename[0] == '!')
	{
		filehandle = popen(std::string(filename, 1).c_str(), "rb");
		if (filehandle == NULL)
		{
			throw CL_Error("Could not open pipe: " + std::string(filename,1));
		}
		filesize = 99999999;
	}
	else
#endif
	{
		filename = translate_path(filename);
		filehandle = fopen(filename.c_str(), "rb");
		if (filehandle == NULL)
		{
			throw CL_Error("Could not open file: " + filename);
		}
		fseek(filehandle, 0, SEEK_END);
		filesize = ftell(filehandle);
		fseek(filehandle, 0, SEEK_SET);
	}
//	cl_assert(filehandle != NULL);
}

void CL_InputSource_File::close()
{
	if (filehandle == NULL) return;
	fclose(filehandle);

	filehandle = NULL;
}

CL_InputSource *CL_InputSource_File::clone() const
{
	return new CL_InputSource_File(this);
}

int CL_InputSource_File::tell() const
{
	return ftell(filehandle);
}

void CL_InputSource_File::seek(int pos, SeekEnum seek_type)
{
	switch (seek_type)
	{
		case seek_cur:
			fseek(filehandle, pos, SEEK_CUR);
			break;

		case seek_set:
			fseek(filehandle, pos, SEEK_SET);
			break;

		case seek_end:
			fseek(filehandle, pos, SEEK_END);
			break;
	}
}

int CL_InputSource_File::size() const
{
	return filesize;
}

void CL_InputSource_File::push_position()
{
	int a = ftell(filehandle);

	stack.push(a);
}

void CL_InputSource_File::pop_position()
{
	int a = stack.top();
	stack.pop();

	fseek(filehandle, a, SEEK_SET);
}
