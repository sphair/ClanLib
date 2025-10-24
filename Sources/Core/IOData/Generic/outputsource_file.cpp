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

#include "Core/precomp.h"
#include "API/Core/IOData/outputsource_file.h"
#include "API/Core/System/cl_assert.h"
//#include "API/Core/System/error.h"

CL_OutputSource_File::CL_OutputSource_File(const std::string &_filename)
{
	filename = _filename;
	pos = 0;
	file = NULL;
	open();
}

CL_OutputSource_File::CL_OutputSource_File()
{
	pos = 0;
	file = NULL;
}

CL_OutputSource_File::~CL_OutputSource_File()
{
	close();
}

int CL_OutputSource_File::write(const void *data, int size)
{
	cl_assert(file != NULL);
	fwrite(data, size, 1, file);
	pos += size;
	return size;
}

void CL_OutputSource_File::open()
{
	if(file != NULL)
		return;

	file = fopen(filename.c_str(), "w+b");
	if(file == NULL)
		throw CL_Error("could not create file");
}

void CL_OutputSource_File::close()
{
	if(file == NULL)
		return;

	fclose(file);
	file = NULL;
}

CL_OutputSource *CL_OutputSource_File::clone()
{
	cl_assert(false); // not implemented yet.
	return NULL;
}

int CL_OutputSource_File::tell() const
{
	return pos;
}

int CL_OutputSource_File::size() const
{
	return pos;
}
