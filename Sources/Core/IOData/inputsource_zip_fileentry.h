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

#ifndef header_inputsource_zip_fileentry
#define header_inputsource_zip_fileentry

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/Core/IOData/inputsource.h"
#include "API/Core/IOData/zip_file_entry.h"
#include "zip_local_file_header.h"
#include <stack>
#include <zlib.h>

class CL_InputSource_Zip_FileEntry : public CL_InputSource
{
//! Construction:
public:
	CL_InputSource_Zip_FileEntry(const std::string &filename, const CL_Zip_FileEntry &entry);

	CL_InputSource_Zip_FileEntry(const CL_InputSource_Zip_FileEntry &copy);

	virtual ~CL_InputSource_Zip_FileEntry();

//! Attributes:
public:
	virtual int tell() const;

	virtual int size() const;

//! Operations:
public:
	virtual int read(void *data, int size);

	virtual void open();

	virtual void close();

	virtual CL_InputSource *clone() const;

	virtual void seek(int pos, SeekEnum seek_type);

	virtual void push_position();

	virtual void pop_position();

//! Implementation:
private:
	CL_InputSource *inputsource;

	CL_Zip_FileEntry file_entry;

	CL_Zip_LocalFileHeader file_header;

	cl_int64 pos, compressed_pos;

	z_stream zs;

	std::stack<cl_int64> position_stack;

	bool source_open;

	char zbuffer[16*1024];
};

#endif
