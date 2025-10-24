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

#ifndef header_pixel_buffer_memory
#define header_pixel_buffer_memory

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/Display/pixel_buffer.h"
#include "pixel_buffer_generic.h"

//: Memory pixel data implementation.
class CL_PixelBuffer_Memory : public CL_PixelBuffer_Generic
{
//! Construction:
public:
	CL_PixelBuffer_Memory(int width, int height, int pitch, const CL_PixelFormat &format, void *data = 0);

	CL_PixelBuffer_Memory(int width, int height, int pitch, const CL_PixelFormat &format, const CL_Palette &palette, void *data = 0);

	virtual ~CL_PixelBuffer_Memory();

//! Operations:
public:
	//: Returns a pointer to the beginning of the pixel buffer. Pointer
	//: is only valid inside a lock/unlock session.
	virtual void *get_data();

	//: Reference counted lock function. Allocate pixel buffer in this function. 
	virtual void perform_lock();

	//: Reference counted unlock function. Free pixel buffer in this function.
	virtual void perform_unlock();

//! Implementation:
private:
	unsigned char *data;

	bool delete_data;
};

#endif
