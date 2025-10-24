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

#include "Display/display_precomp.h"
#include "pixel_buffer_memory.h"

/////////////////////////////////////////////////////////////////////////////
// CL_PixelBuffer_Memory construction:

CL_PixelBuffer_Memory::CL_PixelBuffer_Memory(
	int width, int height, int pitch, const CL_PixelFormat &format, void *data_ptr)
{
	CL_PixelBuffer_Generic::width = width;
	CL_PixelBuffer_Generic::height = height;
	CL_PixelBuffer_Generic::pitch = pitch;
	CL_PixelBuffer_Generic::format = format;

	if (data_ptr)
	{
		delete_data = false;
		data = (unsigned char *) data_ptr;
	}
	else
	{
		delete_data = true;
		data = new unsigned char[pitch*height];
	}
}

CL_PixelBuffer_Memory::CL_PixelBuffer_Memory(
	int width, int height, int pitch, const CL_PixelFormat &format, const CL_Palette &palette, void *data_ptr)
{
	CL_PixelBuffer_Generic::width = width;
	CL_PixelBuffer_Generic::height = height;
	CL_PixelBuffer_Generic::pitch = pitch;
	CL_PixelBuffer_Generic::format = format;
	CL_PixelBuffer_Generic::palette = palette;

	if (data_ptr)
	{
		delete_data = false;
		data = (unsigned char *) data_ptr;
	}
	else
	{
		delete_data = true;
		data = new unsigned char[pitch*height];
	}
}

CL_PixelBuffer_Memory::~CL_PixelBuffer_Memory()
{
	if (delete_data) delete[] data;
}

/////////////////////////////////////////////////////////////////////////////
// CL_PixelBuffer_Memory operations:

void *CL_PixelBuffer_Memory::get_data()
{
	return data;
}

void CL_PixelBuffer_Memory::perform_lock()
{
}

void CL_PixelBuffer_Memory::perform_unlock()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_PixelBuffer_Memory implementation:

