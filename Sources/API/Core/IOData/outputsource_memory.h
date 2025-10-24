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

#ifndef header_outputsource_memory
#define header_outputsource_memory

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

#include "outputsource.h"

//: Outputsource writing to memory.
//- !group=Core/IO Data!
//- !header=core.h!
//- <p>CL_OutputSource_Memory provides a way to write to memory in a way that is abstracted from 
//- the user. CL_OutputSource_Memory should not be instantiated directly, but 
//- rather CL_OutputSourceProvider should create the appropriate output source.</p>

class CL_API_CORE CL_OutputSource_Memory : public CL_OutputSource
{
//! Construction:
public:
	//: Constructs a memory output source.
	CL_OutputSource_Memory();

	//: Output Source Memory destructor.
	virtual ~CL_OutputSource_Memory();

//! Attributes:
public:
	//: Returns current position in output source.
	//return: Current position in source.
	virtual int tell() const;

	//: Returns the size of the output source.
	//return: Size of the output source.
	virtual int size() const;

	//: Get the pointer to the internal data array in the memory source.
	//- <p>Used to extract the data after usage.</p>
	//return: String containing the data written to the output source.
	std::string get_data() const;

//! Operations:
public:
	//: Writes larger amounts of data (no endian and 64 bit conversion).
	//param data: Points to the array from which to write.
	//param size: Number of bytes to write.
	//return: Num bytes actually written.
	virtual int write(const void *data, int size);
	
	//: Opens the output source. By default, it is open.
	virtual void open();
	
	//: Closes the output source.
	virtual void close();

	//: Make a copy of the current outputsource, standing at the same position.
	//return: The copy of the output source.
	virtual CL_OutputSource *clone();

//! Implementation:
private:
	std::string data;
	unsigned int pos;
};

#endif
