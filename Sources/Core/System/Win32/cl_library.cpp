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
#include "API/Core/System/cl_library.h"

std::map<std::string, CL_LibraryFactoryMaker *, std::less<std::string> > CL_LibraryManager::libraries;

CL_LibraryLoader::CL_LibraryLoader(const std::string& file) : filename(file), loaded(false), handle(0), last_error(0)
{
	if(filename.rfind(".dll",filename.size()) == -1)
	{
		filename.append(".dll");
	}
}

CL_LibraryLoader::~CL_LibraryLoader()
{
	// Don't forget to unload the library if needed
	if((loaded == true) && (handle != 0))
		unload();
}

std::string CL_LibraryLoader::name() const
{
	return filename;
}

bool CL_LibraryLoader::is_loaded() const
{
	return loaded;
}

void *CL_LibraryLoader::resolve(const std::string& symbol)
{
	// Just in case the handle has been corrupted
	if(handle == 0)
		return 0;

	// Lookup the symbol name
	void *adr = (void *) GetProcAddress(handle,symbol.c_str());

	if(adr == 0)
	{
		if(last_error != 0)
			delete last_error;
		last_error = new CL_Error(std::string("Couldn't find " + symbol + " within " + filename));
		return 0;
	}
	
	return adr;
}

bool CL_LibraryLoader::load()
{
	handle = LoadLibrary(filename.c_str());
	if(handle == 0)
	{
		if(last_error != 0)
			delete last_error;
		last_error = new CL_Error(std::string("Couldn't load " + filename));
		loaded = false;
	}
	else
	{
		loaded = true;
	}

	return loaded;
}

bool CL_LibraryLoader::unload()
{
	if(last_error != 0)
	{
		delete last_error;
		last_error = 0;
	}

	// Well if the handle is already NULL, don't need to unload it again
	if(handle == 0)
		return true;
	
	if(FreeLibrary(handle) == 0)
	{
		last_error = new CL_Error(std::string("Couldn't unload " + filename));
		return false;
	}

	handle = 0;
	loaded = false;

	return true;
}

CL_Error *CL_LibraryLoader::get_last_error() const
{
	if(last_error == 0)
		return new CL_Error("No CL_Library has been set");
	return last_error;
}

CL_LibraryFactoryMaker_t CL_LibraryManager::get(const std::string& lib_name)
{
	std::map<std::string, CL_LibraryFactoryMaker *, std::less<std::string> >::iterator libs_itr;
	CL_LibraryFactoryMaker_t lib;
	bool found = true;

	if((libs_itr = libraries.find(lib_name)) != libraries.end())
	{
		lib = (*libs_itr).second;
	}
	else
	{
		found = false;
	}

	if(found == false)
		return 0;

	return lib;
}

void CL_LibraryManager::add(const std::string& lib_name,CL_LibraryFactoryMaker *lib)
{
	libraries[lib_name] = lib;
}

void CL_LibraryManager::remove(const std::string& lib_name)
{
	if(libraries.find(lib_name) != libraries.end())
		libraries.erase(lib_name);
}

bool CL_LibraryManager::has(const std::string& lib_name)
{
	if(libraries.find(lib_name) != libraries.end())
		return true;
	return false;
}
