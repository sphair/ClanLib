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
#include <unistd.h>
#endif

#include <API/Core/IOData/inputsource_provider_file.h>
#include <API/Core/IOData/inputsource_file.h>

CL_InputSourceProvider_File::CL_InputSourceProvider_File(const std::string &path)
{
	provider_path = CL_InputSource_File::translate_path(path);

	// make sure the resulting path that we create ends with a '/'
	// this is *also* explicitly needed for everything that tries to load
	// something with inputprovider(".") ==> when "creating" surfaces without using
	// the resource system

	int len = provider_path.length();
	if (provider_path[len-1] != '/' && provider_path[len-1] != '\\') 
		provider_path += '/';
}

CL_InputSourceProvider *CL_InputSourceProvider::create_file_provider(const std::string &path)
{
	return new CL_InputSourceProvider_File(path);
}

CL_InputSource *CL_InputSourceProvider_File::open_source(const std::string &filename)
{
	return new CL_InputSource_File(get_pathname(filename).c_str());
}

std::string CL_InputSourceProvider_File::get_pathname(const std::string &filename)
{        
	std::string filepath;

	// if path is absolute, do not prepend provider path:
	if (filename[0] == '\\' || filename[0] == '/' || filename[1] == ':')
		filepath = filename;
	else
		filepath = provider_path + filename;

	return filepath;
}

CL_InputSourceProvider *CL_InputSourceProvider_File::create_relative(const std::string &path)
{
	return new CL_InputSourceProvider_File(provider_path + path);
}

CL_InputSourceProvider *CL_InputSourceProvider_File::clone()
{
	return new CL_InputSourceProvider_File(provider_path);
}
