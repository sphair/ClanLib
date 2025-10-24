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

//! clanCore="Library"
//! header=core.h

#ifndef header_cl_library
#define header_cl_library

#ifdef CL_API_DLL
#ifdef CL_CORE_EXPORT
#define CL_API_CORE __declspec(dllexport)
#else
#define CL_API_CORE __declspec(dllimport)
#endif
#else
#define CL_API_CORE
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include <map>
#include <string>
#include <functional>

#ifdef WIN32
#include "windows.h"
#endif

#include "error.h"

//: CL_LibraryFactory class
//- !group=Core/System!
//- !header=core.h!
//- <p>Convenient abstract class that your own plugin-classes should inherit in order to be managed with CL_LibraryManager.</p>
class CL_API_CORE CL_LibraryFactory
{
public:
	virtual ~CL_LibraryFactory() = 0;
};

inline CL_LibraryFactory::~CL_LibraryFactory() {}

//: CL_LibraryLoader class
//- !group=Core/System!
//- !header=core.h!
//- <p>This class is a wrapper to dynmanically load code from a dynamic library.</p>            
class CL_API_CORE CL_LibraryLoader
{
//! Construction:
public:
	//: Constructor that does nothing
	//param filename: The filename parameter is the path to the dynamic library to be loaded. You can avoid the the exyension and the constructor will automatically ends the name with either a '.so' under Linux or a '.dll' under Windows
	CL_LibraryLoader(const std::string& filename);
	
	//: The destructor will unload the library if you didn't call yourself unoload() method
	virtual ~CL_LibraryLoader();

//! Attributes:
private:
	//: Name of the opened library (with the extension)
	std::string filename;

	//: State of the library
	bool loaded;

	//: Pointer to the dynamic loaded library
#ifdef WIN32
	HINSTANCE handle;
#else
	void *handle;
#endif

	//: Last error message emitted if an error happened
	CL_Error *last_error;

public:
	//: Return the dynamic library filename
	//return: Return the dynamic library filename
	std::string name() const;

	//: Resolve a symbol name within the library
	//param symbol: Name of the symbol you're looking for within the library
	//return: The address of the symbol if found or 0
	void *resolve(const std::string& symbol);

	//: Return the state of the library
	//return: true if the library is loaded otherwise false
	bool is_loaded() const;

	//: Load the library 
	//return: true if succeeded, false otherwise. You might consider to look at the last error message in the latter case
	bool load();

	//: Unload the library
	//return: true if succeeded, false otherwise. You might consider to look at the last error message in the latter case
	bool unload();

	//: Get the last error message
	//return: The last error message
	CL_Error *get_last_error() const;
};

typedef CL_LibraryFactory *CL_LibraryFactoryMaker();
typedef CL_LibraryFactory *(*CL_LibraryFactoryMaker_t)();

//: CL_LibraryManager class
//- !group=Core/System!
//- !header=core.h!
//- <p>This a convenient class to manage all your loaded classes.</p>
class CL_API_CORE CL_LibraryManager
{
private:
	//: This map stores all makers founded within a dynamic library. 
	//- <p>C modules are easily parsed when looking for a symbol by underlaying librairies (like dlsym() for Linux or GetProcAddress() under Windows), but when it comes to parse C++ modules, these underlaying librairies fail as C++ compilators must decorate symbols name within module (this is due to C++ behaviour). Thus in order to dynamically load classes you must use a trick. Your class must inherit from an abstract class (the interface) and you must provide a C-style function that will be a maker. This function will only create an instance of the class you're looking for. Therefore, when you load a class, you first must retreive the maker function and call it. You'll get then an instance of the class that you can use as usual. This map stores maker function that have been resolved with CL_Library::resolve() method.</p>
	static std::map<std::string, CL_LibraryFactoryMaker *, std::less<std::string> > libraries;

public:
	virtual ~CL_LibraryManager() = 0;

//! Operations:
	//: Retreive a maker function
	//param maker_name: The name of the maker function you're looking within the map
	//return: The maker function
	static CL_LibraryFactoryMaker_t get(const std::string& maker_name);

	//: Add a new maker function to the map
	//param maker_name: The name of the maker function
	//param maker: Maker function to add to the map
	static void add(const std::string& maker_name,CL_LibraryFactoryMaker *maker);

	//: Remove a maker function from the map
	//param maker_name: Maker name function
	static void remove(const std::string&);

	//: Convenient method to check if a given maker function is already in the map
	//param maker_name: Maker name function
	//return: true if found false otherwise
	static bool has(const std::string& maker_name);
};

inline CL_LibraryManager::~CL_LibraryManager() {}

#endif
