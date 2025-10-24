/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
**    Mark Page
*/

#include "Display/precomp.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Core/IOData/datatypes.h"
#include "pixel_buffer_impl.h"
#include "API/Core/System/exception.h"
#include "API/Core/IOData/virtual_file_system.h"
#include "API/Core/IOData/virtual_directory.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/Resources/resource.h"
#include "API/Display/ImageProviders/provider_factory.h"
#include "API/Display/TargetProviders/pixel_buffer_provider.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "API/Display/Image/texture_format.h"


/////////////////////////////////////////////////////////////////////////////
// CL_PixelBuffer construction:

CL_PixelBuffer::CL_PixelBuffer()
{
}

CL_PixelBuffer::CL_PixelBuffer(int width, int height, CL_TextureFormat sized_format, const void *data, bool only_reference_data)
: impl(new CL_PixelBuffer_Impl(width, height, sized_format, data, only_reference_data))
{
}

CL_PixelBuffer::CL_PixelBuffer(int width, int height, CL_TextureFormat sized_format, const CL_Palette &palette, const void *data)
: impl(new CL_PixelBuffer_Impl(width, height, sized_format, palette, data))
{
}

CL_PixelBuffer::CL_PixelBuffer(CL_GraphicContext &gc, int width, int height, CL_PixelBufferDirection direction, CL_TextureFormat sized_format, const void *data, CL_BufferUsage usage)
: impl(new CL_PixelBuffer_Impl(gc, width, height, direction, sized_format, data, usage))
{
}

CL_PixelBuffer::CL_PixelBuffer(CL_GraphicContext &gc, const CL_PixelBuffer &pbuff, CL_PixelBufferDirection direction, CL_BufferUsage usage)
: impl(new CL_PixelBuffer_Impl(gc, pbuff.get_width(), pbuff.get_height(), direction, pbuff.get_format(), pbuff.get_data(), usage))
{
}

CL_PixelBuffer::CL_PixelBuffer(const CL_StringRef &fullname)
{
	CL_String path = CL_PathHelp::get_fullpath(fullname, CL_PathHelp::path_type_file);
	CL_String filename = CL_PathHelp::get_filename(fullname, CL_PathHelp::path_type_file);
	CL_VirtualFileSystem vfs(path);
	CL_VirtualDirectory dir = vfs.get_root_directory();
	*this = CL_PixelBuffer(filename, dir);
}

CL_PixelBuffer::CL_PixelBuffer(const CL_StringRef &filename, const CL_VirtualDirectory &dir)
{
	*this = CL_ImageProviderFactory::load(filename, dir, "");
}

CL_PixelBuffer::CL_PixelBuffer(CL_IODevice &file, const CL_String &image_type )
{
	*this = CL_ImageProviderFactory::load(file, image_type);
}

CL_PixelBuffer::~CL_PixelBuffer()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_PixelBuffer attributes:

void CL_PixelBuffer::throw_if_null() const
{
	if (impl.is_null())
		throw CL_Exception("CL_PixelBuffer is null");
}

const CL_Palette *CL_PixelBuffer::get_palette() const
{
	if (impl->provider)
		throw CL_Exception("GPU Provider currently does not support this function");

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
	return impl->get_pitch();
}

void *CL_PixelBuffer::get_data()
{
	if (impl->provider)
		return impl->provider->get_data();

	return impl->get_data();
}

const void *CL_PixelBuffer::get_data() const
{
	if (impl->provider)
		return impl->provider->get_data();

	return impl->get_data();
}

bool CL_PixelBuffer::has_colorkey() const
{
	return impl->colorkey_enabled;
}

unsigned int CL_PixelBuffer::get_colorkey() const
{
	return impl->colorkey;
}

int CL_PixelBuffer::get_bytes_per_pixel() const
{
	return impl->get_bytes_per_pixel();
}

unsigned int CL_PixelBuffer::get_red_mask() const
{
	return impl->get_red_mask();
}

unsigned int CL_PixelBuffer::get_green_mask() const
{
	return impl->get_green_mask();
}

unsigned int CL_PixelBuffer::get_blue_mask() const
{
	return impl->get_blue_mask();
}

unsigned int CL_PixelBuffer::get_alpha_mask() const
{
	return impl->get_alpha_mask();
}

CL_TextureFormat CL_PixelBuffer::get_format() const
{
	return impl->get_format();
}

CL_PixelBufferProvider *CL_PixelBuffer::get_provider() const
{
	return impl->provider;
}

CL_Colorf CL_PixelBuffer::get_pixel(int x, int y)
{
	return impl->get_pixel(x,y);
}

/////////////////////////////////////////////////////////////////////////////
// CL_PixelBuffer operations:

void CL_PixelBuffer::lock(CL_BufferAccess access)
{
	if (impl->provider)
		impl->provider->lock(access);
}

void CL_PixelBuffer::unlock()
{
	if (impl->provider)
		impl->provider->unlock();
}

void CL_PixelBuffer::upload_data(const CL_Rect &dest_rect, const void *data)
{
	if (impl->provider)
	{
		impl->provider->upload_data(dest_rect, data);
	}
	else
	{
		// Handle the simple base
		if ( (dest_rect.left == 0) && (dest_rect.get_width() == impl->width) )
		{
			int size = dest_rect.get_height() * impl->width * impl->get_bytes_per_pixel();
			memcpy(impl->data, data, size); 
		}
		else
		{
			// Need to upload in blocks here
			throw CL_Exception("CL_PixelBuffer::upload_data() Implement me for this situation");
		}
	}
}

CL_PixelBuffer CL_PixelBuffer::copy() const
{
	if (impl->provider)
		throw CL_Exception("GPU Provider currently does not support this function");

	CL_PixelBuffer pbuf(get_width(), get_height(), get_format(), *get_palette(), 0);
	void *dst_data = (void *)pbuf.get_data();
	void *src_data = (void *)get_data();
	memcpy(dst_data, src_data, get_height()*get_pitch());
	return pbuf;
}

CL_PixelBuffer CL_PixelBuffer::copy(const CL_Rect &rect) const
{
	if (impl->provider)
		throw CL_Exception("GPU Provider currently does not support this function");

	if (rect.left < 0 || rect.top < 0 || rect.right > impl->width, rect.bottom > impl->height)
		throw CL_Exception("Rectangle passed to CL_PixelBuffer::copy() out of bounds");

	int new_width = rect.get_width();
	int new_height = rect.get_height();

	CL_PixelBuffer pbuf(new_width, new_height, get_format(), *get_palette(), 0);
	cl_uint8 *dst_data = (cl_uint8 *)pbuf.get_data();
	cl_uint8 *src_data = (cl_uint8 *)get_data();

	int byte_width = new_width * get_bytes_per_pixel();
	int src_pitch = get_pitch();
	int dest_pitch = pbuf.get_pitch();
	for (int y_cnt = 0; y_cnt < new_height; y_cnt++)
	{
		memcpy(dst_data, src_data, byte_width);
		dst_data += dest_pitch;
		src_data += src_pitch;
	}
	return pbuf;
}

CL_PixelBuffer::operator bool() const
{
	return !impl.is_null();
}

void CL_PixelBuffer::convert(CL_PixelBuffer &target) const
{
	convert(target, CL_Rect(0, 0, target.get_width(), target.get_height()), CL_Rect(0, 0, get_width(), get_height()));
}

void CL_PixelBuffer::convert(CL_PixelBuffer &target, const CL_Rect &dest_rect, const CL_Rect &src_rect) const
{
	if (impl->provider)
		throw CL_Exception("GPU Provider currently does not support this function");

	if (target.get_format() == cl_color_index)
	{
		throw CL_Exception("Converting to indexed pixelformats not supported.");
	}

	CL_Rect new_src_rect = src_rect;
	if (new_src_rect == CL_Rect() )
	{
		new_src_rect.right = get_width();
		new_src_rect.bottom = get_height();
	}

	if ( dest_rect.get_size() != src_rect.get_size())
	{
		throw CL_Exception("Source and destination rects must have same size. Scaled converting not supported.");
	}

	impl->convert(target, dest_rect, src_rect);
}

CL_PixelBuffer CL_PixelBuffer::to_format(CL_TextureFormat sized_format) const
{
	if (impl->provider)
		throw CL_Exception("GPU Provider currently does not support this function");

	CL_PixelBuffer result(get_width(), get_height(),sized_format);
	convert(result);
	return result;
}

void CL_PixelBuffer::set_colorkey(bool new_enabled, unsigned int new_colorkey)
{
	impl->colorkey_enabled = new_enabled;
	impl->colorkey = new_colorkey;
}

void CL_PixelBuffer::flip_vertical()
{
	if (impl->provider)
		throw CL_Exception("GPU Provider currently does not support this function");

	if ( (impl->width == 0) || (impl->height <= 1) )
		return;

	unsigned int pitch = get_pitch();
	std::vector<unsigned char> line_buffer;
	line_buffer.resize(pitch);

	int num_lines = (impl->height - 1) / 2;

	int start_offset = 0;
	int end_offset = (impl->height - 1) * pitch;

	for (int cnt=0; cnt < num_lines; cnt++, start_offset+=pitch, end_offset-=pitch)
	{
		memcpy(&line_buffer[0], &impl->data[start_offset], pitch);
		memcpy(&impl->data[start_offset], &impl->data[end_offset], pitch);
		memcpy(&impl->data[end_offset], &line_buffer[0], pitch);
	}
}

void CL_PixelBuffer::premultiply_alpha()
{
	if (impl->provider)
		throw CL_Exception("GPU Provider currently does not support this function");

	if (get_format() != cl_rgba8)
	{
		throw CL_Exception("The image is not rgba8");
	}

	int w = get_width();
	int h = get_height();
	cl_uint32 *p = (cl_uint32 *) get_data();
	for (int y = 0; y < h; y++)
	{
		int index = y * w;
		cl_uint32 *line = p + index;
		for (int x = 0; x < w; x++)
		{
			cl_uint32 r = ((line[x] >> 24) & 0xff);
			cl_uint32 g = ((line[x] >> 16) & 0xff);
			cl_uint32 b = ((line[x] >> 8) & 0xff);
			cl_uint32 a = (line[x] & 0xff);

			r = r * a / 255;
			g = g * a / 255;
			b = b * a / 255;

			line[x] = (r << 24) + (g << 16) + (b << 8) + a;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CL_PixelBuffer implementation:
