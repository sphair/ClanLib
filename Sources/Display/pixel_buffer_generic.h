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

#ifndef header_pixel_buffer_generic
#define header_pixel_buffer_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/Display/pixel_format.h"
#include "API/Display/palette.h"

//: Pixel data implementation interface.
class CL_PixelBuffer_Generic
{
//! Construction:
public:
	CL_PixelBuffer_Generic() : lock_count(0), ref_count(0) { return; }

	virtual ~CL_PixelBuffer_Generic() { return; }

//! Attributes:
public:
	//: Buffer pixel format description.
	CL_PixelFormat format;

	//: Palette, if 8 bpp pixel format.
	CL_Palette palette;

	//: Buffer scanline pitch.
	unsigned int pitch;

	//: Buffer width.
	int width;

	//: Buffer height.
	int height;

	//: Lock reference count.
	int lock_count;

//! Operations:
public:
	//: Returns a pointer to the beginning of the pixel buffer. Pointer
	//: is only valid inside a lock/unlock session.
	virtual void *get_data()=0;

	//: Reference counted lock function. Allocate pixel buffer in this function. 
	virtual void perform_lock()=0;

	//: Reference counted unlock function. Free pixel buffer in this function.
	virtual void perform_unlock()=0;

	int add_ref() { return ++ref_count; }

	int release_ref()
	{
		ref_count--;
		if (ref_count == 0) { delete this; return 0; }
		return ref_count;
	}
	
	//: Converts between two RGBA formats.
	static void convert(
		void* in_buffer,
		const CL_PixelFormat& in_pf,
		int in_pitch,
		void* out_buffer,
		const CL_PixelFormat& out_pf,
		int out_pitch,
		const CL_Size& size);

	//: Converts from palette format to RGBA.
	static void convert_pal(
		void* in_buffer,
		const CL_PixelFormat& in_pf,
		int in_pitch,
		const CL_Palette& in_pal,
		void* out_buffer,
		const CL_PixelFormat& out_pf,
		int out_pitch,
		const CL_Size& size);

//! Implementation:
private:
	int ref_count;
};

#endif
