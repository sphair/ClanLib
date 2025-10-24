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

//! clanCore="I/O Data"
//! header=core.h

#ifndef header_inputsource_provider_file
#define header_inputsource_provider_file

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

#include "inputsource_provider.h"
#include <string>

class CL_InputSource;

//: The Input Source Provider File Class    
//- !group=Core/IO Data!
//- !header=core.h!
//- <p>CL_InputSourceProvider_File is a subclass of CL_InputSourceProvider.
//- It provides a way to treat normal files as input sources and to read and write them 
//- in a endian transparent manner.</p>

class CL_API_CORE CL_InputSourceProvider_File : public CL_InputSourceProvider
{
//! Construction:
public:
	//: Construtcs a file input source provider.
	//param path: File to create a source for.
	CL_InputSourceProvider_File(const std::string &path);

	//: Open input source using file as its source.
	//param filename: Filename to open.
	//return: InputSource for the file.
	virtual CL_InputSource *open_source(const std::string &filename);

	//: Get full path to source:
	virtual std::string get_pathname(const std::string &filename);

	//: Returns a new inputsource provider object that uses a new path relative to current one.
	virtual CL_InputSourceProvider *create_relative(const std::string &path);

	//: Clone the provider.
	//- <p>Creates a new instance of the class with the same internal data.
	//- This is equivaled to <i>new CL_InputSourceProvider_File(get_path())</i>.</p>
	//return: New instance of the current provider.
	virtual CL_InputSourceProvider *clone();

//! Implementation:
private:
	std::string provider_path;
};

#endif
