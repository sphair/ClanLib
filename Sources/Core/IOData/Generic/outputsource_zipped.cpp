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
#include "API/Core/IOData/outputsource_zipped.h"

/////////////////////////////////////////////////////////////////////////////
// CL_OutputSource_Zipped_Generic

class CL_OutputSource_Zipped_Generic
{
public:
	CL_OutputSource *output;
	bool delete_output;
	int pos;
	int length;
};

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_OutputSource_Zipped::CL_OutputSource_Zipped(
	CL_OutputSource *output,
	bool delete_output)
:
	impl(new CL_OutputSource_Zipped_Generic)
{
	impl->output = output;
	impl->delete_output = delete_output;
	impl->pos = 0;
	impl->length = 0;
}

CL_OutputSource_Zipped::CL_OutputSource_Zipped(
	const CL_OutputSource_Zipped &copy)
:
	impl(new CL_OutputSource_Zipped_Generic)
{
	impl->output = copy.impl->output->clone();
	impl->delete_output = true;
	impl->pos = copy.impl->pos;
	impl->length = copy.impl->length;
}

CL_OutputSource_Zipped::~CL_OutputSource_Zipped()
{
	if (impl->delete_output) delete impl->output;
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

int CL_OutputSource_Zipped::tell() const
{
	return impl->pos;
}

int CL_OutputSource_Zipped::size() const
{
	return impl->length;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

int CL_OutputSource_Zipped::write(const void *data, int size)
{
	int written = impl->output->write(data, size);
	impl->pos += written;
	impl->length += written;
	return written;
}

void CL_OutputSource_Zipped::open()
{
	impl->output->open();
	impl->pos = 0;
	impl->length = 0;
}

void CL_OutputSource_Zipped::close()
{
	impl->output->close();
}

CL_OutputSource *CL_OutputSource_Zipped::clone()
{
	return new CL_OutputSource_Zipped(*this);
}

/////////////////////////////////////////////////////////////////////////////
// Implementation:
