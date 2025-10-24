/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
#include "opengl_pixel_buffer_provider.h"
#include "opengl_graphic_context_provider.h"
#include "API/GL/opengl_wrap.h"
#include "API/Display/Render/shared_gc_data.h"

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLPixelBufferProvider Construction:

CL_OpenGLPixelBufferProvider::CL_OpenGLPixelBufferProvider(CL_OpenGLGraphicContextProvider *gc_provider)
: buffer(gc_provider)
{
}

CL_OpenGLPixelBufferProvider::~CL_OpenGLPixelBufferProvider()
{
}

void CL_OpenGLPixelBufferProvider::create(const void *data, int new_width, int new_height, CL_PixelBufferDirection direction, CL_TextureFormat new_format, CL_BufferUsage usage)
{
	width = new_width;
	height = new_height;

	if (direction == cl_data_from_gpu)
	{
		selected_binding = CL_PIXEL_PACK_BUFFER_BINDING;
		selected_target = CL_PIXEL_PACK_BUFFER;
	}
	else
	{
		selected_binding = CL_PIXEL_UNPACK_BUFFER_BINDING;
		selected_target = CL_PIXEL_UNPACK_BUFFER;
	}

	int size = CL_OpenGL::get_textureformat_bits(new_format);
	size = (size + 7) / 8;	// To bytes
	bytes_per_pixel = size;
	size = size * width * height;

	buffer.create(data, size, usage, selected_binding, selected_target);
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLPixelBufferProvider Operations:
void CL_OpenGLPixelBufferProvider::upload_data(const CL_Rect &dest_rect, const void *data)
{
	// Handle the simple base
	if ( (dest_rect.left == 0) && (dest_rect.get_width() == width) )
	{
		int offset = dest_rect.top * width * bytes_per_pixel;
		int size = dest_rect.get_height() * width * bytes_per_pixel;
		buffer.upload_data(offset, data, size);
	}
	else
	{
		// Need to upload in blocks here
		throw CL_Exception("CL_OpenGLPixelBufferProvider::upload_data() Implement me for this situation");
	}
}
