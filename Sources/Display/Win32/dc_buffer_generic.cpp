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
#include "dc_buffer_generic.h"

// Define DIBSECTION to share pixelbuffer data with GDI. Currently fucks up colors tho.
// #define USE_DIBSECTION

/////////////////////////////////////////////////////////////////////////////
// CL_DeviceContextBuffer_Generic construction:

CL_DeviceContextBuffer_Generic::CL_DeviceContextBuffer_Generic(HDC reference_dc, int bitmap_width, int bitmap_height, bool monocrome_alpha)
{
	ref_dc = reference_dc;

	// Find the pixelformat properties for the reference DC:
	unsigned int red_mask   = 0xff0000;
	unsigned int green_mask = 0x00ff00;
	unsigned int blue_mask  = 0x0000ff;

	// Setup pixelbuffer format info:
	if (monocrome_alpha)
	{
		format.set_red_mask(red_mask);
		format.set_green_mask(red_mask);
		format.set_blue_mask(red_mask);
		format.set_alpha_mask(blue_mask);
		format.set_depth(32);
	}
	else
	{
		format.set_red_mask(red_mask);
		format.set_green_mask(green_mask);
		format.set_blue_mask(blue_mask);
		format.set_alpha_mask(0);
		format.set_depth(32);
	}
	width = bitmap_width;
	height = bitmap_height;
	pitch = width*4;

	// Create DC and bitmap for pixelbuffer:
	memory_dc = CreateCompatibleDC(ref_dc);

	memset(&bitmap_info, 0, sizeof(BITMAPINFO));
	bitmap_info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmap_info.bmiHeader.biWidth = width;
	bitmap_info.bmiHeader.biHeight = -height;
	bitmap_info.bmiHeader.biPlanes = 1;
	bitmap_info.bmiHeader.biBitCount = 32;
	bitmap_info.bmiHeader.biCompression = BI_BITFIELDS;
	bitmap_info.bmiHeader.biSizeImage = pitch*height;
	bitmap_info.bmiColors[0].rgbRed = red_mask;
	bitmap_info.bmiColors[0].rgbGreen = green_mask;
	bitmap_info.bmiColors[0].rgbBlue = blue_mask;
#ifdef USE_DIBSECTION
	bitmap = CreateDIBSection(ref_dc, &bitmap_info, DIB_RGB_COLORS, (LPVOID *) &data, 0, 0);
#else
	bitmap = CreateCompatibleBitmap(ref_dc, width, height);
	first_lock = true;
	data = 0;
#endif

	orig_bitmap = (HBITMAP) SelectObject(memory_dc, bitmap);
}

CL_DeviceContextBuffer_Generic::~CL_DeviceContextBuffer_Generic()
{
#ifndef USE_DIBSECTION
	delete[] data;
#endif

	SelectObject(memory_dc, orig_bitmap);
	DeleteObject(bitmap);
	DeleteDC(memory_dc);
}

/////////////////////////////////////////////////////////////////////////////
// CL_DeviceContextBuffer_Generic operations:

void *CL_DeviceContextBuffer_Generic::get_data()
{
	return data;
}

void CL_DeviceContextBuffer_Generic::perform_lock()
{
#ifdef USE_DIBSECTION
	// We call GdiFlush() here to ensure that any operations on the device context actually finished when
	// application start accessing the data.
	GdiFlush();
#else
	if (first_lock)
	{
		data = new unsigned char[pitch*height];

		/*int scanlines =*/ GetDIBits(memory_dc, bitmap, 0, height, data, &bitmap_info, DIB_RGB_COLORS);
		first_lock = false;
	}
#endif
}

void CL_DeviceContextBuffer_Generic::perform_unlock()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_DeviceContextBuffer_Generic implementation:
