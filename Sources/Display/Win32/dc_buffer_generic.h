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

#ifndef header_dc_buffer_generic
#define header_dc_buffer_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>
#include "API/Display/pixel_buffer.h"
#include "Display/pixel_buffer_generic.h"

class CL_DeviceContextBuffer_Generic : public CL_PixelBuffer_Generic
{
//! Construction:
public:
	//: Construct a pixelbuffer using the pixel format of the passed device context.
	CL_DeviceContextBuffer_Generic(HDC reference_dc, int width, int height, bool monocrome_alpha);

	~CL_DeviceContextBuffer_Generic();

//! Attributes:
public:
	HDC get_dc() { return memory_dc; }

//! Operations:
public:
	//: Returns a pointer to the beginning of the pixel buffer. Pointer
	//: is only valid inside a lock/unlock session.
	virtual void *get_data();

	//: Reference counted lock function.
	virtual void perform_lock();

	//: Reference counted unlock function.
	virtual void perform_unlock();

//! Implementation:
private:
	HDC ref_dc;

	HDC memory_dc;

	BITMAPINFO bitmap_info;

	HBITMAP bitmap, orig_bitmap;

	unsigned char *data;

	bool first_lock;
};

#endif
