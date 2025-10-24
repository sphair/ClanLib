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
*/

#include "GDI/precomp.h"
#include "gdi_frame_buffer_provider.h"
#include "gdi_render_buffer_provider.h"
#include "gdi_texture_provider.h"

/////////////////////////////////////////////////////////////////////////////
// CL_GDIFrameBufferProvider Construction:

CL_GDIFrameBufferProvider::CL_GDIFrameBufferProvider()
: colorbuffer0_type(type_none)
{
}

CL_GDIFrameBufferProvider::~CL_GDIFrameBufferProvider()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_GDIFrameBufferProvider Attributes:

CL_Size CL_GDIFrameBufferProvider::get_attachment_size(int buffer_id) const
{
	switch (colorbuffer0_type)
	{
	case type_render:
		return dynamic_cast<CL_GDIRenderBufferProvider*>(colorbuffer0_render.get_provider())->buffer.get_size();
	case type_texture:
		return colorbuffer0_texture.get_size();
	default:
		return CL_Size();
	}
}

const std::vector<int> &CL_GDIFrameBufferProvider::get_attachment_indexes() const
{
 	attachment_indexes.clear();

	switch (colorbuffer0_type)
	{
	case type_render:
	case type_texture:
		{
			attachment_indexes.push_back(0);
			return attachment_indexes;
		}
	default:
		return attachment_indexes;
	}
}

CL_PixelBuffer CL_GDIFrameBufferProvider::get_colorbuffer0() const
{
	switch (colorbuffer0_type)
	{
	case type_render:
		return dynamic_cast<CL_GDIRenderBufferProvider*>(colorbuffer0_render.get_provider())->buffer;
	case type_texture:
		return dynamic_cast<CL_GDITextureProvider*>(colorbuffer0_texture.get_provider())->get_image();
	default:
		return CL_PixelBuffer();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_GDIFrameBufferProvider Operations:

void CL_GDIFrameBufferProvider::destroy()
{
	delete this;
}

void CL_GDIFrameBufferProvider::attach_color_buffer(int color_buffer, const CL_RenderBuffer &render_buffer)
{
	if (color_buffer == 0)
	{
		colorbuffer0_type = type_render;
		colorbuffer0_render = render_buffer;
		colorbuffer0_texture = CL_Texture();
		sig_changed_event.invoke();
	}
}

void CL_GDIFrameBufferProvider::attach_color_buffer(int color_buffer, const CL_Texture &texture, int level, int zoffset)
{
	if (color_buffer == 0)
	{
		colorbuffer0_type = type_texture;
		colorbuffer0_render = CL_RenderBuffer();
		colorbuffer0_texture = texture;
		sig_changed_event.invoke();
	}
}

void CL_GDIFrameBufferProvider::attach_color_buffer(int color_buffer, const CL_Texture &texture, CL_TextureSubtype subtype, int level, int zoffset)
{
	if (color_buffer == 0)
	{
		colorbuffer0_type = type_texture;
		colorbuffer0_render = CL_RenderBuffer();
		colorbuffer0_texture = texture;
		sig_changed_event.invoke();
	}
}

void CL_GDIFrameBufferProvider::detach_color_buffer(int color_buffer, const CL_Texture &texture, int level, int zoffset)
{
	if (color_buffer == 0)
	{
		colorbuffer0_type = type_none;
		colorbuffer0_render = CL_RenderBuffer();
		colorbuffer0_texture = CL_Texture();
		sig_changed_event.invoke();
	}
}

void CL_GDIFrameBufferProvider::attach_stencil_buffer(const CL_RenderBuffer &render_buffer)
{
}

void CL_GDIFrameBufferProvider::detach_stencil_buffer()
{
}

void CL_GDIFrameBufferProvider::attach_depth_buffer(const CL_RenderBuffer &render_buffer)
{
}

void CL_GDIFrameBufferProvider::attach_depth_buffer(const CL_Texture &texture, CL_TextureSubtype subtype, int level, int zoffset)
{
}

void CL_GDIFrameBufferProvider::detach_depth_buffer()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_GDIFrameBufferProvider Implementation:
