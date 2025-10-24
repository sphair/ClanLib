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
**    Mark Page
*/

#include "SDL/precomp.h"
#include "sdl_frame_buffer_provider.h"
#include "sdl_render_buffer_provider.h"
#include "sdl_texture_provider.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SDLFrameBufferProvider Construction:

CL_SDLFrameBufferProvider::CL_SDLFrameBufferProvider()
: colorbuffer0_type(type_none)
{
}

CL_SDLFrameBufferProvider::~CL_SDLFrameBufferProvider()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_SDLFrameBufferProvider Attributes:

CL_Size CL_SDLFrameBufferProvider::get_attachment_size(int buffer_id) const
{
	CL_SDLRenderBufferProvider *render_ptr;
	CL_SDLTextureProvider *texture_ptr;
	SDL_Surface *sptr = NULL;

	switch (colorbuffer0_type)
	{
	case type_render:
		render_ptr = dynamic_cast<CL_SDLRenderBufferProvider*>(colorbuffer0_render.get_provider());
		if (render_ptr)
			sptr = render_ptr->get_surface();

	case type_texture:
		texture_ptr = dynamic_cast<CL_SDLTextureProvider*>(colorbuffer0_texture.get_provider());
		if (texture_ptr)
			sptr = texture_ptr->get_surface();
	default:
		break;
	}

	CL_Size size;
	if (sptr)
	{
		size.width = sptr->w;
		size.height = sptr->h;
	}
	return size;

}

std::vector<int> CL_SDLFrameBufferProvider::get_attachment_indexes() const
{
	switch (colorbuffer0_type)
	{
	case type_render:
	case type_texture:
		{
			std::vector<int> v;
			v.push_back(0);
			return v;
		}
	default:
		return std::vector<int>();
	}
}

SDL_Surface *CL_SDLFrameBufferProvider::get_colorbuffer0() const
{
	CL_SDLRenderBufferProvider *render_ptr;
	CL_SDLTextureProvider *texture_ptr;

	switch (colorbuffer0_type)
	{
	case type_render:
		render_ptr = dynamic_cast<CL_SDLRenderBufferProvider*>(colorbuffer0_render.get_provider());
		if (!render_ptr)
			return NULL;
		return render_ptr->get_surface();

	case type_texture:
		texture_ptr = dynamic_cast<CL_SDLTextureProvider*>(colorbuffer0_texture.get_provider());
		if (!texture_ptr)
			return NULL;
		return texture_ptr->get_surface();
	default:
		return NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_SDLFrameBufferProvider Operations:

void CL_SDLFrameBufferProvider::destroy()
{
	delete this;
}

void CL_SDLFrameBufferProvider::attach_color_buffer(int color_buffer, const CL_RenderBuffer &render_buffer)
{
	if (color_buffer == 0)
	{
		colorbuffer0_type = type_render;
		colorbuffer0_render = render_buffer;
		colorbuffer0_texture = CL_Texture();
		sig_changed_event.invoke();
	}
}

void CL_SDLFrameBufferProvider::attach_color_buffer(int color_buffer, const CL_Texture &texture, int level, int zoffset)
{
	if (color_buffer == 0)
	{
		colorbuffer0_type = type_texture;
		colorbuffer0_render = CL_RenderBuffer();
		colorbuffer0_texture = texture;
		sig_changed_event.invoke();
	}
}

void CL_SDLFrameBufferProvider::attach_color_buffer(int color_buffer, const CL_Texture &texture, CL_TextureSubtype subtype, int level, int zoffset)
{
	if (color_buffer == 0)
	{
		colorbuffer0_type = type_texture;
		colorbuffer0_render = CL_RenderBuffer();
		colorbuffer0_texture = texture;
		sig_changed_event.invoke();
	}
}

void CL_SDLFrameBufferProvider::detach_color_buffer(int color_buffer, const CL_Texture &texture, int level, int zoffset)
{
	if (color_buffer == 0)
	{
		colorbuffer0_type = type_none;
		colorbuffer0_render = CL_RenderBuffer();
		colorbuffer0_texture = CL_Texture();
		sig_changed_event.invoke();
	}
}

void CL_SDLFrameBufferProvider::attach_stencil_buffer(const CL_RenderBuffer &render_buffer)
{
}

void CL_SDLFrameBufferProvider::detach_stencil_buffer()
{
}

void CL_SDLFrameBufferProvider::attach_depth_buffer(const CL_RenderBuffer &render_buffer)
{
}

void CL_SDLFrameBufferProvider::attach_depth_buffer(const CL_Texture &texture, CL_TextureSubtype subtype, int level, int zoffset)
{
}

void CL_SDLFrameBufferProvider::detach_depth_buffer()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_SDLFrameBufferProvider Implementation:
