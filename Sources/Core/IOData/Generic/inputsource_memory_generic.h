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

#ifndef header_inputsource_memory_generic
#define header_inputsource_memory_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/Core/IOData/inputsource.h"
#include "API/Core/IOData/inputsource_provider.h"

class CL_OutputSource_MemoryGeneric;

class CL_InputSource_MemoryGeneric : public CL_InputSource
//: Interface to read data from a memory_generic source.
//- <p>CL_InputSource_MemoryGeneric is used to read data from a memory_generic source.</p>
//also: CL_InputSourceProvider - Interface to open input sources with.
{
public:
	CL_InputSource_MemoryGeneric(void *data, int size, bool delete_data = false);

	//: MemoryGeneric copy constructor.
	//- <p>Makes a seperate copy of the memory in MG.</p>
	//also: MemoryGeneric::clone.
	//param MG: Pointer to the MemoryGeneric object from which to copy.
	CL_InputSource_MemoryGeneric(const CL_InputSource_MemoryGeneric *MG);

	virtual ~CL_InputSource_MemoryGeneric();

	//: Reads larger amounts of data (no endian and 64 bit conversion).
	//param data: Points to an array where the read data is stored.
	//param size: Number of bytes to read.
	//return: Num bytes actually read.
	virtual int read(void *data, int size);
	
	//: Opens the input source. By default, it is open.
	virtual void open();
	
	//: Closes the input source.
	virtual void close();

	//: Make a copy of the current InputSource, standing at the same position.
	//return: The clone of the input source.
	virtual CL_InputSource *clone() const;

	//: Returns current position in input source.
	//return: Current position in input source.
	virtual int tell() const;

	//: Seeks to the specified position in the input source.
	//param pos: Position relative to 'seek_type'.
	//param seek_type: Defines what the 'pos' is relative to. Can be either seek_set, seek_cur og seek_end.
	virtual void seek(int pos, SeekEnum seek_type);

	//: Returns the size of the input source
	//return: Size of the input source.
	virtual int size() const;

	//: Pushes the current input source position. The position can be restored again with pop_position.
	virtual void push_position();
	
	//: Pops a previous pushed input source position (returns to the position).
	virtual void pop_position();

	//: Purges the input buffer of data without deleting the buffer
	virtual void purge();

private:
	unsigned char *data;
	unsigned int pos, length;
	bool delete_data;
};

class CL_InputSourceProvider_Memory : public CL_InputSourceProvider
{
public:
	CL_InputSourceProvider_Memory(unsigned char *data, unsigned int size, bool delete_data);
	CL_InputSourceProvider_Memory(CL_InputSource_MemoryGeneric *MG);
	CL_InputSourceProvider_Memory(CL_OutputSource_MemoryGeneric *MG);

	virtual CL_InputSource *open_source(const std::string &handle);
	virtual CL_InputSourceProvider *clone();

private:
	unsigned char *data;
	unsigned int size;
	bool delete_data;
};

#endif

