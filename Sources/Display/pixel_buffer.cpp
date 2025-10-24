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
#include "API/Display/pixel_buffer.h"
#include "API/Core/IOData/datatypes.h"
#include "pixel_buffer_generic.h"
#include "pixel_buffer_memory.h"
#include "API/Core/System/error.h"

/////////////////////////////////////////////////////////////////////////////
// CL_PixelBuffer construction:

CL_PixelBuffer::CL_PixelBuffer(CL_PixelBuffer_Generic *impl)
: impl(impl)
{
	if (impl) impl->add_ref();
}

CL_PixelBuffer::CL_PixelBuffer(int width, int height, int pitch, const CL_PixelFormat &format, void *data)
: impl(new CL_PixelBuffer_Memory(width, height, pitch, format, data))
{
	impl->add_ref();
}

CL_PixelBuffer::CL_PixelBuffer(int width, int height, int pitch, const CL_PixelFormat &format, const CL_Palette &palette, void *data)
: impl(new CL_PixelBuffer_Memory(width, height, pitch, format, palette, data))
{
	impl->add_ref();
}

CL_PixelBuffer::CL_PixelBuffer()
: impl(0)
{
}

CL_PixelBuffer::CL_PixelBuffer(const CL_PixelBuffer &copy)
: impl(copy.impl)
{
	if (impl) impl->add_ref();
}

CL_PixelBuffer::~CL_PixelBuffer()
{
	if (impl) impl->release_ref();
}

/////////////////////////////////////////////////////////////////////////////
// CL_PixelBuffer attributes:

const CL_PixelFormat &CL_PixelBuffer::get_format() const
{
	return impl->format;
}

const CL_Palette &CL_PixelBuffer::get_palette() const
{
	return impl->palette;
}

int CL_PixelBuffer::get_width() const
{
	return impl->width;
}

int CL_PixelBuffer::get_height() const
{
	return impl->height;
}

unsigned int CL_PixelBuffer::get_pitch() const
{
	return impl->pitch;
}

void *CL_PixelBuffer::get_data()
{
	return impl->get_data();
}

CL_Color CL_PixelBuffer::get_pixel(int x, int y)
{
	CL_Color color;

	lock();
	
	cl_uint8* buf = static_cast<cl_uint8*>(impl->get_data());
	CL_PixelFormat format = get_format();

	if (format.get_type() == pixelformat_index)
	{
		color = impl->palette[int(buf[y*impl->pitch + x])];
	}
	else if (format.get_type() == pixelformat_rgba)
	{
		int depth = format.get_depth ();

		cl_uint8 *pos = &buf[y * impl->pitch + x * ((depth + 7)/8)];

		if (8 == depth)
		{
			cl_uint8 value = *pos;

			color = CL_Color ((value & format.get_red_mask ()) >> format.get_mask_shift (format.get_red_mask ()),
							  (value & format.get_green_mask ()) >> format.get_mask_shift (format.get_green_mask ()),
							  (value & format.get_blue_mask ()) >> format.get_mask_shift (format.get_blue_mask ()),
							  (value & format.get_alpha_mask ()) >> format.get_mask_shift (format.get_alpha_mask ()));
		}
		else if (16 == depth)
		{
			cl_uint16 value = *((cl_uint16*)pos);

			color = CL_Color ((value & format.get_red_mask ()) >> format.get_mask_shift (format.get_red_mask ()),
							  (value & format.get_green_mask ()) >> format.get_mask_shift (format.get_green_mask ()),
							  (value & format.get_blue_mask ()) >> format.get_mask_shift (format.get_blue_mask ()),
							  (value & format.get_alpha_mask ()) >> format.get_mask_shift (format.get_alpha_mask ()));
		}
		else if (24 == depth)
		{
			cl_uint32 value = ((*pos) << 16) +
				              ((*(pos + 1)) << 8) +
							  *(pos + 2);

			color = CL_Color ((value & format.get_red_mask ()) >> format.get_mask_shift (format.get_red_mask ()),
							  (value & format.get_green_mask ()) >> format.get_mask_shift (format.get_green_mask ()),
							  (value & format.get_blue_mask ()) >> format.get_mask_shift (format.get_blue_mask ()),
							  (value & format.get_alpha_mask ()) >> format.get_mask_shift (format.get_alpha_mask ()));
		}
		else if (32 == depth)
		{
			cl_uint32 value = *((cl_uint32*)pos);

			color = CL_Color ((value & format.get_red_mask ()) >> format.get_mask_shift (format.get_red_mask ()),
							  (value & format.get_green_mask ()) >> format.get_mask_shift (format.get_green_mask ()),
							  (value & format.get_blue_mask ()) >> format.get_mask_shift (format.get_blue_mask ()),
							  (value & format.get_alpha_mask ()) >> format.get_mask_shift (format.get_alpha_mask ()));
		}
	}
	unlock();

	return color;
}

/////////////////////////////////////////////////////////////////////////////
// CL_PixelBuffer operations:

CL_PixelBuffer &CL_PixelBuffer::operator =(const CL_PixelBuffer &copy)
{
	if (impl) impl->release_ref();
	impl = copy.impl;
	if (impl) impl->add_ref();
	return *this;
}

CL_PixelBuffer::operator bool() const
{
  return (impl != 0);
}

void CL_PixelBuffer::lock()
{
	impl->lock_count++;
	if (impl->lock_count == 1) impl->perform_lock();
}

void CL_PixelBuffer::unlock()
{
	impl->lock_count--;
	if (impl->lock_count == 0) impl->perform_unlock();
}

void CL_PixelBuffer::convert(CL_PixelBuffer target)
{
	target.lock();

	convert(
		target.get_data(),
		target.get_format(),
		target.get_pitch(),
		CL_Rect(
			0, 0,
			target.get_width(), target.get_height()));

	target.unlock();
}

void CL_PixelBuffer::convert(
	void *buffer,
	const CL_PixelFormat &format,
	int dest_pitch,
	const CL_Rect &dest_rect,
	const CL_Rect &src_rect)
{
	lock();

	bool null_src = false;
	if (src_rect.left == 0 && src_rect.top == 0 && src_rect.right == 0 && src_rect.bottom == 0)
	{
		null_src = true;
	}

	if (format.get_type() == pixelformat_index)
	{
		throw CL_Error("Converting to indexed pixelformats not supported.");
	}

	if (null_src == false && dest_rect.get_size() != src_rect.get_size())
	{
		throw CL_Error("Source and destination rects must have same size. Scaled converting not supported.");
	}

	char *src_data = (char *) get_data();
	char *dest_data = (char *) buffer;
	if (null_src == false) src_data += src_rect.top*get_pitch() + src_rect.left*((get_format().get_depth()+7)/8);
	dest_data += dest_rect.top*dest_pitch + dest_rect.left*((format.get_depth()+7)/8);

	if (get_format().get_type() == pixelformat_index)
	{
		CL_PixelBuffer_Generic::convert_pal(
			src_data,
			get_format(),
			get_pitch(),
			get_palette(),
			dest_data,
			format,
			dest_pitch,
			dest_rect.get_size());
	}
	else
	{
		CL_PixelBuffer_Generic::convert(
			src_data,
			get_format(),
			get_pitch(),
			dest_data,
			format,
			dest_pitch,
			dest_rect.get_size());
	}

	unlock();
}

void CL_PixelBuffer::convert_line(void *buffer, const CL_PixelFormat &format, int y)
{
	convert(
		buffer,
		format,
		(format.get_depth()+7)/8*get_width(),
		CL_Rect(0, 0, get_width(), 1),
		CL_Rect(0, y, get_width(), 1));
}

CL_PixelBuffer CL_PixelBuffer::to_format(const CL_PixelFormat &format)
{
	lock();
	CL_PixelBuffer result(get_width(), get_height(), get_width()*((format.get_depth()+7)/8), format);
	convert(result);
	unlock();
	return result;
}

void CL_PixelBuffer::set_colorkey(bool enabled, unsigned int colorkey)
{
	impl->format.enable_colorkey(enabled);
	impl->format.set_colorkey(colorkey);
}

void CL_PixelBuffer::draw_pixel(int x, int y, const CL_Color &color)
{
	lock();

	cl_uint8* buf = static_cast<cl_uint8*>(impl->get_data());
	CL_PixelFormat format = get_format();

	if (format.get_type() == pixelformat_index)
	{
		throw CL_Error("Direct settings of CL_Colors pixels in paletted mode is not supported.");
	}
	else if (format.get_type() == pixelformat_rgba)
	{
		int depth = format.get_depth ();
		int bytes_per_pixel = (depth + 7)/8;
		cl_uint8 *pos = &buf[y * impl->pitch + x * bytes_per_pixel];

		int red_shift = format.get_mask_shift(format.get_red_mask()),
			green_shift = format.get_mask_shift(format.get_green_mask()),
			blue_shift = format.get_mask_shift(format.get_blue_mask()),
			alpha_shift = format.get_mask_shift(format.get_alpha_mask());

		switch (bytes_per_pixel)
		{
		case 4:
		case 3:
			{
				cl_uint32 c = (color.get_red() << red_shift) | (color.get_green() << green_shift) | (color.get_blue() << blue_shift) | (color.get_alpha() << alpha_shift);
				memcpy (pos, &c, bytes_per_pixel);
			}
			break;

		case 2:
			{
				cl_uint16 c = (color.get_red() << red_shift) | (color.get_green() << green_shift) | (color.get_blue() << blue_shift) | (color.get_alpha() << alpha_shift);
				memcpy (pos, &c, bytes_per_pixel);
			}
			break;

		case 1:
			{
				cl_uint8 c = (color.get_red() << red_shift) | (color.get_green() << green_shift) | (color.get_blue() << blue_shift) | (color.get_alpha() << alpha_shift);
				*pos = c;
			}
			break;

		default:
			throw CL_Error("Unsuported pixel format depth for draw_pixel.");
		}

	}
	unlock();
}

/////////////////////////////////////////////////////////////////////////////
// CL_PixelBuffer implementation:
