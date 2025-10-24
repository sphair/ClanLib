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

#ifndef header_inputsource_provider
#define header_inputsource_provider

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

#include <string>
class CL_InputSource;

//: Class that provides input sources.
//- !group=Core/IO Data!
//- !header=core.h!
//- <p>CL_InputSourceProvider is used to open inputsources. This can be from a
//- datafile where the inputsource is an index - or it a directory where the
//- files are the inputsources.</p>
class CL_API_CORE CL_InputSourceProvider
{
public:
//! Construction:
	//: Input Source Provider destructor.
	virtual ~CL_InputSourceProvider() { return; }

	//: Creates a file provider.
	//- <p>Input sources opened are considered as files.</p>
	//param path: Path where the files are located.
	//return: The file input provider created.
	static CL_InputSourceProvider *create_file_provider(const std::string &path);

	//: Creates a datafile provider.
	//- <p>Input sources are opened as indexes in the specified datafile.</p>
	//param filename: Name of the datafile to be used.
	//return: The datafile input provider created.
	static CL_InputSourceProvider *create_datafile_provider(const std::string &filename);

//! Overrideables:
	//: Opens an inputsource using the passed handle.
	//param handle: handle/index/filename/identier to the input source requested.
	//return: The opened input source.
	virtual CL_InputSource *open_source(const std::string &handle) = 0;

	//: Returns a copy of the current provider.
	//return: The copy of the inputsource provider.
	virtual CL_InputSourceProvider *clone() = 0;

	//: Get full path to source:
	virtual std::string get_pathname(const std::string &filename) = 0;

	//: Returns a new inputsource provider object that uses a new path relative to current one.
	virtual CL_InputSourceProvider *create_relative(const std::string &path) = 0;
};

#endif
