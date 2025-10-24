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

#ifndef header_inputsource
#define header_inputsource

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
#include "datatypes.h"

//: Interface to read data.
//- !group=Core/IO Data!
//- !header=core.h!
//- <p> CL_InputSource is used to read data from different types of input sources.
//- In most cases, this is regular files and ClanLib zlib compressed datafiles. </p>
//- 
//- <p>To access some system specific input source types, such as datafiles and
//- regular ones, use the static create functions in the CL_InputSourceProvider
//- class.</p>
//also CL_InputSourceProvider: Interface to open input sources with.
class CL_API_CORE CL_InputSource
{
//! Enums:
public:
	//: Enum used by seek()
	enum SeekEnum
	{
		seek_set,	 // from beginning of index
		seek_cur,	 // from current position
		seek_end	 // from end of index
	};

//! Construction:
public:
	//: Input Source Constructor
	CL_InputSource() { set_system_mode(); }

	//: Input Source Destructor
	virtual ~CL_InputSource() { return; }

//! Attributes:
public:
	//: Returns current position in input source.
	//return: Current position in input source.
	virtual int tell() const = 0;

	//: Returns the size of the input source
	//return: Size of the input source.
	virtual int size() const = 0;

	//: Returns true if the input source is in little endian mode.
	virtual bool is_little_endian() { return little_endian_mode; }

//! Operations:
public:
	//: Changes input data endianess to the local systems mode.
	virtual void set_system_mode();

	//: Changes input data endianess to big endian mode.
	virtual void set_big_endian_mode();

	//: Changes input data endianess to little endian mode. This is the default setting.
	virtual void set_little_endian_mode();
	
	//: Reads a signed 64 bit integer from input source.
	//return: The integer read.
	virtual cl_int64 read_int64();

	//: Reads an unsigned 64 bit integer from input source.
	//return: The integer read.
	virtual cl_uint64 read_uint64();
	
	//: Reads a signed 32 bit integer from input source.
	//return: The integer read.
	virtual cl_int32 read_int32();

	//: Reads an unsigned 32 bit integer from input source.
	//return: The integer read.
	virtual cl_uint32 read_uint32();
	
	//: Reads a signed 16 bit integer from input source.
	//return: The integer read.
	virtual cl_int16 read_int16();

	//: Reads an unsigned 16 bit integer from input source.
	//return: The integer read.
	virtual cl_uint16 read_uint16();
	
	//: Reads a signed 8 bit integer from input source.
	//return: The integer read.
	virtual cl_int8 read_int8();

	//: Reads an unsigned 8 bit integer from input source.
	//return: The integer read.
	virtual cl_uint8 read_uint8();

	//: Reads a signed 16 bit integer (short) from input source.
	//return: The short read.
	virtual short read_short16();

	//: Reads an unsigned 16 bit integer (short) from input source.
	//return: The unsigned short read.
	virtual unsigned short read_ushort16();

	//: Reads a signed 8 bit integer (char) from input source.
	//return: The char read.
	virtual char read_char8();

	//: Reads an unsigned 8 bit integer (char) from input source.
	//return: The unsigned char read.
	virtual unsigned char read_uchar8();

	//: Reads an ieee format floating point from input source.
	//return: The float read.
	virtual float read_float32();

	//: Reads an ieee format double from input source.
	//return: The double read.
	virtual double read_float64();

	//: Reads a bool from input source.
	//return: The bool read.
	virtual bool read_bool8();

	//: Reads larger amounts of data (no endian and 64 bit conversion).
	//param data: Points to an array where the read data is stored.
	//param size: Number of bytes to read.
	//return: Num bytes actually read.
	virtual int read(void *data, int size) = 0;
	
	//: Opens the input source. By default, it is open.
	virtual void open() = 0;
	
	//: Closes the input source.
	virtual void close() = 0;

	//: Make a copy of the current inputsource, standing at the same position.
	//return: The copy of the input source.
	virtual CL_InputSource *clone() const = 0;

	//: Reads a string from the input source.
	//- <p>The binary format expected in the input source is first an uint32 telling the length of the
	//- string, and then the string itself.</p>
	//return: The string read.
	virtual std::string read_string();

	//: Seeks to the specified position in the input source.
	//param pos: Position relative to 'seek_type'.
	//param seek_type: Defines what the 'pos' is relative to. Can be either seek_set, seek_cur og seek_end.
	virtual void seek(int pos, SeekEnum seek_type) = 0;

	//: Pushes the current input source position.
	//: The position can be restored again with pop_position.
	virtual void push_position() = 0;
	
	//: Pops a previous pushed input source position (returns to the position).
	virtual void pop_position() = 0;

//! Implementation:
protected:
	bool little_endian_mode;
};

#endif
