/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
**    Harry Storbacka
*/

#include "Display/precomp.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Core/IOData/datatypes.h"
#include "pixel_buffer_impl.h"
#include "API/Core/System/exception.h"

/////////////////////////////////////////////////////////////////////////////
// CL_PixelBufferRef construction:

CL_PixelBufferRef::CL_PixelBufferRef()
: palette(0), width(0), height(0), pitch(0), data(0)
{
}

CL_PixelBufferRef::CL_PixelBufferRef(const CL_PixelBuffer &buffer)
: format(buffer.get_format()), palette((CL_Palette *) buffer.get_palette()),
  width(buffer.get_width()), height(buffer.get_height()),
  pitch(buffer.get_pitch()), data((void *) buffer.get_data())
{
}

CL_PixelBufferRef::CL_PixelBufferRef(int width, int height, int pitch, const CL_PixelFormat &format, const void *data)
: format(format), palette(0), width(width), height(height),
  pitch(pitch), data((void *) data)
{
}

CL_PixelBufferRef::CL_PixelBufferRef(int width, int height, int pitch, const CL_PixelFormat &format, const CL_Palette *palette, const void *data)
: format(format), palette((CL_Palette *) palette), width(width), height(height),
  pitch(pitch), data((void *) data)
{
}

CL_PixelBufferRef CL_PixelBufferRef::get_subimage(const CL_Rect &rect) const
{
	if (rect.left < 0 || rect.top < 0 || rect.right > width || rect.bottom > height)
		throw CL_Exception(cl_text("Rectangle passed to CL_PixelBuffer::get_subimage out of bounds"));

	const unsigned char *subdata = (const unsigned char *) data;
	subdata += rect.top * pitch;
	subdata += rect.left * ((format.get_depth() + 7) / 8);
	return CL_PixelBufferRef(rect.get_width(), rect.get_height(), get_pitch(), get_format(), get_palette(), subdata);
}

void CL_PixelBufferRef::convert(CL_PixelBuffer target) const
{
	convert(
		target.get_data(),
		target.get_format(),
		target.get_pitch(),
		CL_Rect(
			0, 0,
			target.get_width(), target.get_height()));
}

void CL_PixelBufferRef::convert(
	void *buffer,
	const CL_PixelFormat &format,
	int dest_pitch,
	const CL_Rect &dest_rect,
	const CL_Rect &src_rect) const
{
	bool null_src = false;
	if (src_rect.left == 0 && src_rect.top == 0 && src_rect.right == 0 && src_rect.bottom == 0)
	{
		null_src = true;
	}

	if (format.get_type() == pixelformat_index)
	{
		throw CL_Exception(cl_text("Converting to indexed pixelformats not supported."));
	}

	if (null_src == false && dest_rect.get_size() != src_rect.get_size())
	{
		throw CL_Exception(cl_text("Source and destination rects must have same size. Scaled converting not supported."));
	}

	char *src_data = (char *) get_data();
	char *dest_data = (char *) buffer;
	if (null_src == false) src_data += src_rect.top*get_pitch() + src_rect.left*((get_format().get_depth()+7)/8);
	dest_data += dest_rect.top*dest_pitch + dest_rect.left*((format.get_depth()+7)/8);

	if (get_format().get_type() == pixelformat_index)
	{
		CL_PixelBuffer_Impl::convert_pal(
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
		CL_PixelBuffer_Impl::convert(
			src_data,
			get_format(),
			get_pitch(),
			dest_data,
			format,
			dest_pitch,
			dest_rect.get_size());
	}
}

void CL_PixelBufferRef::convert_line(void *buffer, const CL_PixelFormat &format, int y) const
{
	convert(
		buffer,
		format,
		(format.get_depth()+7)/8*get_width(),
		CL_Rect(0, 0, get_width(), 1),
		CL_Rect(0, y, get_width(), 1));
}

CL_PixelBuffer CL_PixelBufferRef::to_format(const CL_PixelFormat &format) const
{
	CL_PixelBuffer result(get_width(), get_height(), get_width()*((format.get_depth()+7)/8), format);
	convert(result);
	return result;
}

CL_PixelBuffer CL_PixelBufferRef::copy() const
{
	CL_PixelBuffer pbuf(get_width(), get_height(), get_width()*((format.get_depth()+7)/8), get_format(), *get_palette(), 0);
	cl_uint8 *dst_data = (cl_uint8*)pbuf.get_data();
	cl_uint8 *src_data = (cl_uint8*)get_data();

	for (int y=0; y<get_height(); y++)
	{
		memcpy((void*)dst_data, (void*)src_data, get_width()*((format.get_depth()+7)/8));
		dst_data += pbuf.get_pitch();
		src_data += get_pitch();
	}

	return pbuf;
}

/////////////////////////////////////////////////////////////////////////////
// CL_PixelBuffer construction:

CL_PixelBuffer::CL_PixelBuffer()
{
}

CL_PixelBuffer::CL_PixelBuffer(int width, int height, int pitch, const CL_PixelFormat &format, const void *data)
: impl(new CL_PixelBuffer_Impl(width, height, pitch, format, data))
{
}

CL_PixelBuffer::CL_PixelBuffer(int width, int height, int pitch, const CL_PixelFormat &format, const CL_Palette &palette, const void *data)
: impl(new CL_PixelBuffer_Impl(width, height, pitch, format, palette, data))
{
}

CL_PixelBuffer::CL_PixelBuffer(const CL_PixelBufferRef &other)
: impl(new CL_PixelBuffer_Impl(other.get_width(), other.get_height(), other.get_pitch(), other.get_format(), other.get_data()))
{
	if (other.get_palette())
		impl->palette = *other.get_palette();
}

CL_PixelBuffer::CL_PixelBuffer(const CL_PixelBuffer &copy)
: impl(copy.impl)
{
}

CL_PixelBuffer::~CL_PixelBuffer()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_PixelBuffer attributes:

bool CL_PixelBuffer::is_null() const
{
	return impl.is_null();
}

const CL_PixelFormat &CL_PixelBuffer::get_format() const
{
	return impl->format;
}

const CL_Palette *CL_PixelBuffer::get_palette() const
{
	return &impl->palette;
}

int CL_PixelBuffer::get_width() const
{
	return impl->width;
}

int CL_PixelBuffer::get_height() const
{
	return impl->height;
}

CL_Size CL_PixelBuffer::get_size() const
{
	return CL_Size(impl->width, impl->height);
}

unsigned int CL_PixelBuffer::get_pitch() const
{
	return impl->pitch;
}

void *CL_PixelBuffer::get_data()
{
	return impl->get_data();
}

const void *CL_PixelBuffer::get_data() const
{
	return impl->get_data();
}

CL_Color CL_PixelBuffer::get_pixel(int x, int y)
{
	CL_Color color;

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

	return color;
}

CL_PixelBufferRef CL_PixelBuffer::get_subimage(const CL_Rect &rect) const
{
	if (rect.left < 0 || rect.top < 0 || rect.right > impl->width, rect.bottom > impl->height)
		throw CL_Exception(cl_text("Rectangle passed to CL_PixelBuffer::get_subimage out of bounds"));

	const unsigned char *subdata = (const unsigned char *) impl->data;
	subdata += rect.top * impl->pitch;
	subdata += rect.left * ((impl->format.get_depth() + 7) / 8);
	return CL_PixelBufferRef(rect.get_width(), rect.get_height(), get_pitch(), get_format(), get_palette(), subdata);
}

/////////////////////////////////////////////////////////////////////////////
// CL_PixelBuffer operations:

CL_PixelBuffer CL_PixelBuffer::copy() const
{
	CL_PixelBuffer pbuf(get_width(), get_height(), get_pitch(), get_format(), *get_palette(), 0);
	void *dst_data = (void *)pbuf.get_data();
	void *src_data = (void *)get_data();
	memcpy(dst_data, src_data, get_height()*get_pitch());
	return pbuf;
}

CL_PixelBuffer &CL_PixelBuffer::operator =(const CL_PixelBuffer &copy)
{
	impl = copy.impl;
	return *this;
}

CL_PixelBuffer::operator bool() const
{
	return !impl.is_null();
}

void CL_PixelBuffer::convert(CL_PixelBuffer target) const
{
	convert(
		target.get_data(),
		target.get_format(),
		target.get_pitch(),
		CL_Rect(
			0, 0,
			target.get_width(), target.get_height()));
}

void CL_PixelBuffer::convert(
	void *buffer,
	const CL_PixelFormat &format,
	int dest_pitch,
	const CL_Rect &dest_rect,
	const CL_Rect &src_rect) const
{
	bool null_src = false;
	if (src_rect.left == 0 && src_rect.top == 0 && src_rect.right == 0 && src_rect.bottom == 0)
	{
		null_src = true;
	}

	if (format.get_type() == pixelformat_index)
	{
		throw CL_Exception(cl_text("Converting to indexed pixelformats not supported."));
	}

	if (null_src == false && dest_rect.get_size() != src_rect.get_size())
	{
		throw CL_Exception(cl_text("Source and destination rects must have same size. Scaled converting not supported."));
	}

	char *src_data = (char *) get_data();
	char *dest_data = (char *) buffer;
	if (null_src == false) src_data += src_rect.top*get_pitch() + src_rect.left*((get_format().get_depth()+7)/8);
	dest_data += dest_rect.top*dest_pitch + dest_rect.left*((format.get_depth()+7)/8);

	if (get_format().get_type() == pixelformat_index)
	{
		CL_PixelBuffer_Impl::convert_pal(
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
		CL_PixelBuffer_Impl::convert(
			src_data,
			get_format(),
			get_pitch(),
			dest_data,
			format,
			dest_pitch,
			dest_rect.get_size());
	}
}

void CL_PixelBuffer::convert_line(void *buffer, const CL_PixelFormat &format, int y) const
{
	convert(
		buffer,
		format,
		(format.get_depth()+7)/8*get_width(),
		CL_Rect(0, 0, get_width(), 1),
		CL_Rect(0, y, get_width(), 1));
}

CL_PixelBuffer CL_PixelBuffer::to_format(const CL_PixelFormat &format) const
{
	CL_PixelBuffer result(get_width(), get_height(), get_width()*((format.get_depth()+7)/8), format);
	convert(result);
	return result;
}

void CL_PixelBuffer::set_colorkey(bool enabled, unsigned int colorkey)
{
	impl->format.enable_colorkey(enabled);
	impl->format.set_colorkey(colorkey);
}

void CL_PixelBuffer::draw_pixel(int x, int y, const CL_Colorf &color)
{
	//TODO: Implement me!
}

/////////////////////////////////////////////////////////////////////////////
// CL_PixelBuffer implementation:
