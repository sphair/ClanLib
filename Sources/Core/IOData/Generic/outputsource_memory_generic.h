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

#ifndef header_outputsource_memory_generic
#define header_outputsource_memory_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/Core/IOData/outputsource.h"

//: Outputsource writing to memory_generic.
class CL_OutputSource_MemoryGeneric : public CL_OutputSource
{
public:
	CL_OutputSource_MemoryGeneric(unsigned int blocksize = 4*1024);
	virtual ~CL_OutputSource_MemoryGeneric();
	
	//: Get the pointer to the internal data array in the memory_generic source.
	//: Used to extract the data after usage.
	//return: Pointer to the data written.
	void *get_data() const;

	//: Writes larger amounts of data (no endian and 64 bit conversion):
	//param data: Points to the array written.
	//param size: Number of bytes to write.
	//return: Num bytes actually written.
	virtual int write(const void *data, int size);
	
	//: Opens the output source. By default, it is open.
	virtual void open();
	
	//: Closes the output source.
	virtual void close();

	//: Make a copy of the current OutputSource, standing at the same position.
	//return: The copy of the output source.
	virtual CL_OutputSource *clone();

	//: Returns current position in output source.
	//return: Current position in source.
	virtual int tell() const;

	//: Returns the size of the output source
	//return: Current size of output source.
	virtual int size() const;

	//: Cleans out the databuffer without deleting it.
	virtual void purge();

private:
	unsigned int m_blocksize;
	unsigned char *m_data;
	unsigned int m_size, m_pos;
};

#endif
