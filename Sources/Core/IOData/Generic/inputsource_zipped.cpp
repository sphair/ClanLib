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
#include "API/Core/IOData/inputsource_zipped.h"

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource_Zipped_Generic

class CL_InputSource_Zipped_Generic
{
public:
	CL_InputSource *input;
	bool delete_input;
};

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource_Zipped Construction:

CL_InputSource_Zipped::CL_InputSource_Zipped(
	CL_InputSource *input,
	bool delete_input)
:
	impl(new CL_InputSource_Zipped_Generic)
{
	impl->input = input;
	impl->delete_input = delete_input;
}

CL_InputSource_Zipped::~CL_InputSource_Zipped()
{
	if (impl->delete_input) delete impl->input;
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource_Zipped Attributes:

int CL_InputSource_Zipped::tell() const
{
	return impl->input->tell();
}

int CL_InputSource_Zipped::size() const
{
	return impl->input->size();
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource_Zipped Operations:

int CL_InputSource_Zipped::read(void *data, int size)
{
	return impl->input->read(data, size);
}

void CL_InputSource_Zipped::open()
{
	impl->input->open();
}

void CL_InputSource_Zipped::close()
{
	impl->input->close();
}

CL_InputSource *CL_InputSource_Zipped::clone() const
{
	return new CL_InputSource_Zipped(impl->input->clone(), true);
}

void CL_InputSource_Zipped::seek(int pos, SeekEnum seek_type)
{
	impl->input->seek(pos, seek_type);
}

void CL_InputSource_Zipped::push_position()
{
	impl->input->push_position();
}

void CL_InputSource_Zipped::pop_position()
{
	impl->input->pop_position();
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputSource_Zipped Implementation:
