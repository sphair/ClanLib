/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
**    Mark Page
*/

#include "GL/precomp.h"
#include "gl3_pixel_buffer_provider.h"
#include "gl3_graphic_context_provider.h"
#include "API/GL/opengl_wrap.h"
#include "API/Display/Render/shared_gc_data.h"

namespace clan
{
	GL3PixelBufferProvider::GL3PixelBufferProvider()
	{
	}

	GL3PixelBufferProvider::~GL3PixelBufferProvider()
	{
	}

	void GL3PixelBufferProvider::create(const void *data, const Size &new_size, PixelBufferDirection direction, TextureFormat new_format, BufferUsage usage)
	{
		data_locked = false;
		texture_format = new_format;
		size = new_size;
		if (direction == data_from_gpu)
		{
			selected_binding = GL_PIXEL_PACK_BUFFER_BINDING;
			selected_target = GL_PIXEL_PACK_BUFFER;
		}
		else
		{
			selected_binding = GL_PIXEL_UNPACK_BUFFER_BINDING;
			selected_target = GL_PIXEL_UNPACK_BUFFER;
		}

		int total_size = PixelBuffer::get_bytes_per_pixel(new_format);
		bytes_per_pixel = total_size;
		pitch = bytes_per_pixel * size.width;
		total_size = pitch * size.height;

		buffer.create(data, total_size, usage, selected_binding, selected_target);
	}

	void GL3PixelBufferProvider::upload_data(GraphicContext &gc, const Rect &dest_rect, const void *data)
	{
		// Handle the simple base
		if ((dest_rect.left == 0) && (dest_rect.get_width() == size.width))
		{
			int offset = dest_rect.top * pitch;
			int total_size = dest_rect.get_height() * pitch;
			buffer.upload_data(gc, offset, data, total_size);
		}
		else
		{
			// Need to upload in blocks here
			throw Exception("GL3PixelBufferProvider::upload_data() Implement me for this situation");
		}
	}

	void *GL3PixelBufferProvider::get_data()
	{
		if (!data_locked)
			throw Exception("lock() not called before get_data()");

		return buffer.get_data();
	}
}
