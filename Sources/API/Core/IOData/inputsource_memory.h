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

#ifndef header_inputsource_memory
#define header_inputsource_memory

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

#include "inputsource.h"

//: Interface to read data from a memory source.
//- !group=Core/IO Data!
//- !header=core.h!
//- <p>CL_InputSource_Memory is used to read data from a memory source.
//- CL_InputSourceProvider - Interface to open input sources with.</p>
class CL_API_CORE CL_InputSource_Memory : public CL_InputSource
{
//! Construction:
public:
	//: Input Source Memory constructor.
	CL_InputSource_Memory(const std::string &data);

	CL_InputSource_Memory(void *data, int size, bool copy_data = false);

	//: Input Source Memory destructor.
	virtual ~CL_InputSource_Memory();

//! Attributes:
public:
	//: Returns current position in input source.
	//return: Current position in input source.
	virtual int tell() const;

	//: Returns the size of the input source
	//return: Size of the input source.
	virtual int size() const;

	//: Get the pointer to the internal data array in the memory source.
	//return: String containing the internal data array's data.
	std::string get_data() const;

//! Operations:
public:
	//: Reads larger amounts of data (no endian and 64 bit conversion).
	//param data: Points to an array where the read data is stored.
	//param size: Number of bytes to read.
	//return: Num bytes actually read.
	virtual int read(void *data, int size);
	
	//: Opens the input source. By default, it is open.
	virtual void open();
	
	//: Closes the input source.
	virtual void close();

	//: Make a copy of the current inputsource, standing at the same position.
	//return: The copy of the input source.
	virtual CL_InputSource *clone() const;

	//: Seeks to the specified position in the input source.
	//param pos: Position relative to 'seek_type'.
	//param seek_type: Defines what the 'pos' is relative to. Can be either seek_set, seek_cur og seek_end.
	virtual void seek(int pos, SeekEnum seek_type);

	//: Pushes the current input source position.
	//- <p>The position can be restored again with pop_position.</p>
	virtual void push_position();
	
	//: Pops a previous pushed input source position (returns to the position).
	virtual void pop_position();

//! Implementation:
private:
	unsigned char *data;
	int data_size;
	int pos;
	std::string str_data;
};

#endif
