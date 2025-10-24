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

#ifndef header_inputsource_file
#define header_inputsource_file

#if _MSC_VER > 1000
#pragma once
#endif

#include "inputsource.h"

#include <string>
#include <stack>
#include <stdio.h>

//: Inputsource reading from file.
//- !group=Core/IO Data!
//- !header=core.h!
class CL_InputSource_File : public CL_InputSource
{
//! Construction:
public:
	//: Input Souce File constructor.
	CL_InputSource_File(const std::string &filename);

	CL_InputSource_File(const CL_InputSource_File *source);

	//: Input Souce File destructor
	virtual ~CL_InputSource_File();

//! Attributes:
public:
	//: Returns current position in input source.
	//return: Current position in input source.
	virtual int tell() const;

	//: Returns the size of the input source
	//return: Size of the input source.
	virtual int size() const;

//! Operations:
public:
	//: Reads larger amounts of data (no endian and 64 bit conversion):
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
	
	//: Gets the actual path after relative path translation.
	static std::string translate_path(const std::string &path);

//! Implementation:
private:
	std::stack<int> stack;
	std::string filename;
	FILE *filehandle;
	int filesize;
};

#endif
