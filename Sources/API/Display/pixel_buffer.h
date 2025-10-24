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

//! clanDisplay="Display 2D"
//! header=display.h

#ifndef header_pixel_buffer
#define header_pixel_buffer

#ifdef CL_API_DLL
#ifdef CL_DISPLAY_EXPORT
#define CL_API_DISPLAY __declspec(dllexport)
#else
#define CL_API_DISPLAY __declspec(dllimport)
#endif
#else
#define CL_API_DISPLAY
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "../Core/Math/rect.h"

class CL_PixelFormat;
class CL_Palette;
class CL_PixelBuffer_Generic;
class CL_GraphicContext;
class CL_Color;

//: Pixel data access.
//- !group=Display/Display 2D!
//- !header=display.h!
class CL_API_DISPLAY CL_PixelBuffer
{
//! Construction:
public:
	//: Constructs a pixel buffer.
	//param width: Width of pixel buffer, in pixels.
	//param height: Height of pixel buffer, in pixels.
	//param pitch: Bytes per line in pixel buffer.
	//param format: Pixel format of pixel buffer.
	//param palette: Palette used in pixel buffer.
	//param data: Data pointer to pixel data. If null, will construct a memory pixel buffer with the given dimensions.
	//- <p>If the data pointer is not null, it will internally just point at the memory location specified by data.
	//- In other words, CL_PixelBuffer do not copy the data, and it does not delete the data pointer when destroyed itself.</p>
	CL_PixelBuffer(int width, int height, int pitch, const CL_PixelFormat &format, void *data = 0);

	CL_PixelBuffer(int width, int height, int pitch, const CL_PixelFormat &format, const CL_Palette &palette, void *data = 0);

	CL_PixelBuffer(const CL_PixelBuffer &copy);

	CL_PixelBuffer();

	virtual ~CL_PixelBuffer();

//! Attributes:
public:
	//: Returns the pixel format of the pixel buffer.
	const CL_PixelFormat &get_format() const;

	//: Returns the palette of the pixel buffer.
	const CL_Palette &get_palette() const;

	//: Returns the buffer width.
	int get_width() const;

	//: Returns the buffer height.
	int get_height() const;

	//: Returns the pitch (bytes per scanline).
	unsigned int get_pitch() const;

	//: Returns a pointer to the beginning of the pixel buffer. Pointer
	//: is only valid inside a lock/unlock session.
	void *get_data();
	
	//: Returns the pixel at coordinates x and y.
	//: Only valid within a lock/unlock session.
	CL_Color get_pixel(int x, int y);

//! Operations:
public:
	//: Copy assignment operator.
	CL_PixelBuffer &operator =(const CL_PixelBuffer &copy);

	//: Return true if the CL_PixelBuffer is valid and useable
	operator bool () const;

	//: Locks the pixel buffer, making calls to get_data() valid.
	void lock();

	//: Unlocks the pixelbuffer.
	void unlock();

	//: Convert pixel buffer to the pixel format of the target buffer, storing
	//: the result in the target buffer.
	//param target: Target pixel buffer.
	//param buffer: Buffer getting stored pixels in the specified format and pitch.
	//param format: Pixel format of target buffer.
	//param dest_pitch: Bytes per line of target buffer.
	//param dest_rect: Destination rectangle for conversion.
	//param src_rect: Source rectangle for conversion.
	void convert(CL_PixelBuffer target);

	void convert(void *buffer, const CL_PixelFormat &format, int dest_pitch, const CL_Rect &dest_rect, const CL_Rect &src_rect=CL_Rect(0,0,0,0));

	//: Convert one line of pixel data to target buffer.
	//param buffer: Buffer getting stored pixels in the specified format
	//param format: Pixel format of target buffer.
	//param y: Specifies which line to convert when converting one single line.
	void convert_line(void *buffer, const CL_PixelFormat &format, int y);

	//: Converts current buffer to a new pixel format and returns the result.
	CL_PixelBuffer to_format(const CL_PixelFormat &format);

	//: Sets a new colorkey without converting the buffer.
	//param enabled: Enable or disable the colorkey.
	//param colorkey: New colorkey to use.
	void set_colorkey(bool enabled, unsigned int colorkey);
	
	//: Draw a pixel at (x, y) using the specified color.
	//: Only valid within a lock/unlock session.
	void draw_pixel(int x, int y, const CL_Color &color);

//! Implementation:
public:
	CL_PixelBuffer(CL_PixelBuffer_Generic *impl);

protected:
	//: Pixel data implementation.
	CL_PixelBuffer_Generic *impl;
};

#endif
