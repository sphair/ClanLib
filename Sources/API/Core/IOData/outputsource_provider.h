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

#ifndef header_outputsource_provider
#define header_outputsource_provider

#if _MSC_VER > 1000
#pragma once
#endif

#include <string>

class CL_OutputSource;

//: Class that provides output targets.
//- !group=Core/IO Data!
//- !header=core.h!
//- <p>CL_OutputSourceProvider is used to open output sources. This can be from a
//- datafile where the outputsource is an index - or it a directory where the
//- files are the outputsources.</p>
class CL_OutputSourceProvider
{
public:
//! Overrideables:
	//: Output Source Provider destructor.
	virtual ~CL_OutputSourceProvider() { return; }

	//: Opens an outputsource using the passed handle.
	//param handle: handle/index/filename/identier to the output source requested.
	//return: The output source opened.
	virtual CL_OutputSource *open_source(const std::string &handle) = 0;

	//: Returns a copy of the current provider.
	//return: The copy of the outputsource provider.
	virtual CL_OutputSourceProvider *clone() = 0;
};

#endif
