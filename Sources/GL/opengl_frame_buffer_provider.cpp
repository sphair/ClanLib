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
*/

#include "GL/precomp.h"
#include "API/Display/Render/render_buffer.h"
#include "API/Display/TargetProviders/render_buffer_provider.h"
#include "API/Core/Text/string_format.h"
#include "API/Display/Render/texture.h"
#include "API/GL/opengl_wrap.h"
#include "API/GL/opengl.h"
#include "API/Display/Render/shared_gc_data.h"
#include "opengl_frame_buffer_provider.h"
#include "opengl_render_buffer_provider.h"
#include "opengl_texture_provider.h"

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLFrameBufferProvider Construction:

CL_OpenGLFrameBufferProvider::CL_OpenGLFrameBufferProvider(CL_OpenGLGraphicContextProvider *gc_provider)
: CL_FrameBufferProvider(), gc_provider(gc_provider), count_color_attachments(0)
{
	bind_target = cl_framebuffer_draw;
	handle = 0; 
	CL_SharedGCData::add_disposable(this);

	CL_OpenGL::set_active(gc_provider);

	if(clGenFramebuffers == 0)
		throw CL_Exception("Framebuffer not supported on your graphics card.");

	clGenFramebuffers(1, &handle);

	CL_FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
	// Default to no draw buffers  (the user may only want a depth buffer)
	clDrawBuffer(CL_NONE);
	clReadBuffer(CL_NONE);

}

CL_OpenGLFrameBufferProvider::~CL_OpenGLFrameBufferProvider()
{
	dispose();
	CL_SharedGCData::remove_disposable(this);
}

void CL_OpenGLFrameBufferProvider::on_dispose()
{
	if (handle)
	{
		if (CL_OpenGL::set_active())
		{
			clDeleteFramebuffers(1, &handle);
		}
	}

	// Detach all textures and renderbuffers
	for (int cnt=0; cnt < num_attachment_offsets; cnt++)
	{
		if (!attached_textures[cnt].is_null())
			attached_textures[cnt] = CL_Texture();

		if (!attached_renderbuffers[cnt].is_null())
			attached_renderbuffers[cnt] = CL_RenderBuffer();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLFrameBufferProvider Attributes:

CLuint CL_OpenGLFrameBufferProvider::get_handle()
{
	return handle;
}

CL_Size CL_OpenGLFrameBufferProvider::get_size() const
{
	bool size_set = false;
	CL_Size size;

	// Find the minimum renderbuffer size
	for (int cnt=0; cnt < num_attachment_offsets; cnt++)
	{
		if (!attached_textures[cnt].is_null())
		{
			CL_Size texture_size = attached_textures[cnt].get_size();
			if (size_set)
			{
				if (texture_size.width < size.width)
					size.width = texture_size.width;
				if (texture_size.height < size.height)
					size.height = texture_size.height;
			}
			else
			{
				size = texture_size;
				size_set = true;
			}
		}

		if (!attached_renderbuffers[cnt].is_null())
		{
			CL_Size texture_size = attached_renderbuffers[cnt].get_size();
			if (size_set)
			{
				if (texture_size.width < size.width)
					size.width = texture_size.width;
				if (texture_size.height < size.height)
					size.height = texture_size.height;
			}
			else
			{
				size = texture_size;
				size_set = true;
			}
		}
	}

	return size;
}

CL_FrameBufferBindTarget CL_OpenGLFrameBufferProvider::get_bind_target() const
{
	return bind_target;
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLFrameBufferProvider Operations:


void CL_OpenGLFrameBufferProvider::destroy()
{
	delete this;
}

void CL_OpenGLFrameBufferProvider::attach_color_buffer(int attachment_index, const CL_RenderBuffer &render_buffer)
{
	CL_FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
	bool replaced_object = attach_object(CL_COLOR_ATTACHMENT0+attachment_index, render_buffer);

	if (!replaced_object)
	{
		if (!count_color_attachments)
		{
			clDrawBuffer(CL_COLOR_ATTACHMENT0);
			clReadBuffer(CL_COLOR_ATTACHMENT0);
		}
		count_color_attachments++;
	}
}

void CL_OpenGLFrameBufferProvider::detach_color_buffer(int attachment_index, const CL_RenderBuffer &render_buffer)
{
	CL_FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
	detach_object(CL_COLOR_ATTACHMENT0+attachment_index, render_buffer);

	count_color_attachments--;
	if (!count_color_attachments)
	{
		clDrawBuffer(CL_NONE);
		clReadBuffer(CL_NONE);
	}
}

void CL_OpenGLFrameBufferProvider::attach_color_buffer(int attachment_index, const CL_Texture &texture, int level, int zoffset)
{
	CL_FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
	bool replaced_object = attach_object(CL_COLOR_ATTACHMENT0+attachment_index, texture, level, zoffset, 0);

	if (!replaced_object)
	{
		if (!count_color_attachments)
		{
			clDrawBuffer(CL_COLOR_ATTACHMENT0);
			clReadBuffer(CL_COLOR_ATTACHMENT0);
		}
		count_color_attachments++;
	}
}

void CL_OpenGLFrameBufferProvider::attach_color_buffer(int attachment_index, const CL_Texture &texture, CL_TextureSubtype subtype, int level, int zoffset)
{
	CL_FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
	bool replaced_object = attach_object(CL_COLOR_ATTACHMENT0+attachment_index, texture, level, zoffset, decode_texture_subtype(subtype));

	if (!replaced_object)
	{
		if (!count_color_attachments)
		{
			clDrawBuffer(CL_COLOR_ATTACHMENT0);
			clReadBuffer(CL_COLOR_ATTACHMENT0);
		}
		count_color_attachments++;
	}
}

void CL_OpenGLFrameBufferProvider::detach_color_buffer(int attachment_index, const CL_Texture &texture, int level, int zoffset)
{
	CL_FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
	detach_object(CL_COLOR_ATTACHMENT0+attachment_index, texture);

	count_color_attachments--;
	if (!count_color_attachments)
	{
		clDrawBuffer(CL_NONE);
		clReadBuffer(CL_NONE);
	}
}

void CL_OpenGLFrameBufferProvider::attach_stencil_buffer(const CL_RenderBuffer &render_buffer)
{
	CL_FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
	attach_object(CL_STENCIL_ATTACHMENT, render_buffer);
}

void CL_OpenGLFrameBufferProvider::detach_stencil_buffer(const CL_RenderBuffer &render_buffer)
{
	CL_FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
	detach_object(CL_STENCIL_ATTACHMENT, render_buffer);
}

void CL_OpenGLFrameBufferProvider::attach_stencil_buffer(const CL_Texture &texture, int level, int zoffset)
{
	CL_FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
	attach_object(CL_STENCIL_ATTACHMENT, texture, level, zoffset, 0);
}

void CL_OpenGLFrameBufferProvider::attach_stencil_buffer(const CL_Texture &texture, CL_TextureSubtype subtype, int level, int zoffset)
{
	CL_FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
	attach_object(CL_STENCIL_ATTACHMENT, texture, level, zoffset, decode_texture_subtype(subtype));
}

void CL_OpenGLFrameBufferProvider::detach_stencil_buffer(const CL_Texture &texture, int level, int zoffset)
{
	CL_FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
	detach_object(CL_STENCIL_ATTACHMENT, texture);
}

void CL_OpenGLFrameBufferProvider::attach_depth_buffer(const CL_RenderBuffer &render_buffer)
{
	CL_FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
	attach_object(CL_DEPTH_ATTACHMENT, render_buffer);
}

void CL_OpenGLFrameBufferProvider::detach_depth_buffer(const CL_RenderBuffer &render_buffer)
{
	CL_FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
	detach_object(CL_DEPTH_ATTACHMENT, render_buffer);
}

void CL_OpenGLFrameBufferProvider::attach_depth_buffer(const CL_Texture &texture, int level, int zoffset)
{
	CL_FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
	attach_object(CL_DEPTH_ATTACHMENT, texture, level, zoffset, 0);
}

void CL_OpenGLFrameBufferProvider::attach_depth_buffer(const CL_Texture &texture, CL_TextureSubtype subtype, int level, int zoffset)
{
	CL_FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
	attach_object(CL_DEPTH_ATTACHMENT, texture, level, zoffset, decode_texture_subtype(subtype));
}

void CL_OpenGLFrameBufferProvider::detach_depth_buffer(const CL_Texture &texture, int level, int zoffset)
{
	CL_FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
	detach_object(CL_DEPTH_ATTACHMENT, texture);
}

void CL_OpenGLFrameBufferProvider::attach_depth_stencil_buffer(const CL_RenderBuffer &render_buffer)
{
	CL_FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
	attach_object(CL_DEPTH_STENCIL_ATTACHMENT, render_buffer);
}

void CL_OpenGLFrameBufferProvider::detach_depth_stencil_buffer(const CL_RenderBuffer &render_buffer)
{
	CL_FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
	detach_object(CL_DEPTH_STENCIL_ATTACHMENT, render_buffer);
}

void CL_OpenGLFrameBufferProvider::attach_depth_stencil_buffer(const CL_Texture &texture, int level, int zoffset)
{
	CL_FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
	attach_object(CL_DEPTH_STENCIL_ATTACHMENT, texture, level, zoffset, 0);
}

void CL_OpenGLFrameBufferProvider::attach_depth_stencil_buffer(const CL_Texture &texture, CL_TextureSubtype subtype, int level, int zoffset)
{
	CL_FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
	attach_object(CL_DEPTH_STENCIL_ATTACHMENT, texture, level, zoffset, decode_texture_subtype(subtype));
}

void CL_OpenGLFrameBufferProvider::detach_depth_stencil_buffer(const CL_Texture &texture, int level, int zoffset)
{
	CL_FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
	detach_object(CL_DEPTH_STENCIL_ATTACHMENT, texture);
}

void CL_OpenGLFrameBufferProvider::set_bind_target( CL_FrameBufferBindTarget target )
{
	bind_target = target;
}


void CL_OpenGLFrameBufferProvider::check_framebuffer_complete()
{
	CL_FrameBufferStateTracker tracker(bind_target, handle, gc_provider);

	int error_code = clCheckFramebufferStatus(CL_FRAMEBUFFER);
	if (error_code != CL_FRAMEBUFFER_COMPLETE)
		throw CL_Exception(cl_format("FrameBuffer is : %1", get_error_message(error_code)));

}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLFrameBufferProvider Implementation:

CL_String CL_OpenGLFrameBufferProvider::get_error_message(int error_code)
{
	switch (error_code)
	{
	case CL_FRAMEBUFFER_COMPLETE:
		return "FRAMEBUFFER_COMPLETE";
	case CL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		return "FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
	case CL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		return "FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
	case CL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		return "FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
	case CL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		return "FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
	case CL_FRAMEBUFFER_UNSUPPORTED:
		return "FRAMEBUFFER_UNSUPPORTED";
	default:
		return cl_format("Error code: %1", error_code);
	}
}

CL_FrameBufferStateTracker::CL_FrameBufferStateTracker(CL_FrameBufferBindTarget target, CLuint handle, CL_OpenGLGraphicContextProvider *gc_provider)
: bind_target(target), last_bound(0), handle_and_bound_equal(false)
{
	CL_OpenGL::set_active(gc_provider);
	if (bind_target == cl_framebuffer_draw)
	{
		clGetIntegerv(CL_DRAW_FRAMEBUFFER_BINDING, &last_bound);
		if (last_bound == handle)
		{
			handle_and_bound_equal = true;
		}
		else
		{
			clBindFramebuffer(CL_DRAW_FRAMEBUFFER, handle);
		}
	}
	else
	{
		clGetIntegerv(CL_READ_FRAMEBUFFER_BINDING, &last_bound);
		if (last_bound == handle)
		{
			handle_and_bound_equal = true;
		}
		else
		{
			clBindFramebuffer(CL_READ_FRAMEBUFFER, handle);
		}
	}
}

CL_FrameBufferStateTracker::~CL_FrameBufferStateTracker()
{
	if (!handle_and_bound_equal)
	{
		if (bind_target == cl_framebuffer_draw)
			clBindFramebuffer(CL_DRAW_FRAMEBUFFER, last_bound);
		else
			clBindFramebuffer(CL_READ_FRAMEBUFFER, last_bound);
	}
}

int CL_OpenGLFrameBufferProvider::decode_internal_attachment_offset(CLenum opengl_attachment)
{
	int internal_attachment_offset = 0;

	if (opengl_attachment == CL_STENCIL_ATTACHMENT)
	{
		internal_attachment_offset = stencil_attachment_offset;
	}
	else if (opengl_attachment == CL_DEPTH_ATTACHMENT)
	{
		internal_attachment_offset = depth_attachment_offset;
	}
	else if (opengl_attachment == CL_DEPTH_STENCIL_ATTACHMENT)
	{
		internal_attachment_offset = depth_stencil_attachment_offset;
	}
	else if ( (opengl_attachment >= CL_COLOR_ATTACHMENT0) && (opengl_attachment < (CL_COLOR_ATTACHMENT0+max_color_attachments)) )
	{
		internal_attachment_offset = color_attachment_offset + (opengl_attachment - CL_COLOR_ATTACHMENT0);
	}
	else
	{
		throw CL_Exception("Specified attachment index value is currently not supported");
	}
	return internal_attachment_offset;
}


bool CL_OpenGLFrameBufferProvider::attach_object(CLenum opengl_attachment, const CL_RenderBuffer &render_buffer)
{
	int internal_attachment_offset = decode_internal_attachment_offset(opengl_attachment);

	// Check for object replacement
	bool is_replaced_object = false;
	if (!attached_renderbuffers[internal_attachment_offset].is_null())
	{
		is_replaced_object = true;
		attached_renderbuffers[internal_attachment_offset] = CL_RenderBuffer();
	}
	if (!attached_textures[internal_attachment_offset].is_null())
	{
		is_replaced_object = true;
		attached_textures[internal_attachment_offset] = CL_Texture();
	}

	// Store the renderbuffer
	attached_renderbuffers[internal_attachment_offset] = render_buffer;

	// Bind the renderbuffer
	CL_OpenGLRenderBufferProvider *gl_render_buffer = static_cast<CL_OpenGLRenderBufferProvider *>(render_buffer.get_provider());
	if (!gl_render_buffer)
			throw CL_Exception("Invalid render buffer");

	CL_FrameBufferStateTracker tracker_draw(bind_target, handle, gc_provider);

	CLenum target = CL_DRAW_FRAMEBUFFER;
	if (bind_target == cl_framebuffer_read)
		target = CL_READ_FRAMEBUFFER;

	CLuint render_buffer_handle = gl_render_buffer->get_handle();

	clFramebufferRenderbuffer(target, opengl_attachment, CL_RENDERBUFFER, render_buffer_handle);
	return is_replaced_object;
}

bool CL_OpenGLFrameBufferProvider::attach_object(CLenum opengl_attachment, const CL_Texture &texture, int level, int zoffset, CLuint texture_target)
{
	int internal_attachment_offset = decode_internal_attachment_offset(opengl_attachment);

	// Check for object replacement
	bool is_replaced_object = false;
	if (!attached_renderbuffers[internal_attachment_offset].is_null())
	{
		is_replaced_object = true;
		attached_renderbuffers[internal_attachment_offset] = CL_RenderBuffer();
	}
	if (!attached_textures[internal_attachment_offset].is_null())
	{
		is_replaced_object = true;
		attached_textures[internal_attachment_offset] = CL_Texture();
	}

	// Store the texture
	attached_textures[internal_attachment_offset] = texture;

	// Bind the renderbuffer
	CL_OpenGLTextureProvider *gl_texture_provider = dynamic_cast<CL_OpenGLTextureProvider*>(texture.get_provider());
	if (!gl_texture_provider)
			throw CL_Exception("Invalid texture");

	CLuint texture_type = gl_texture_provider->get_texture_type();
	CLuint texture_handle = gl_texture_provider->get_handle();

	CLenum target = CL_DRAW_FRAMEBUFFER;
	if (bind_target == cl_framebuffer_read)
		target = CL_READ_FRAMEBUFFER;

	if (!texture_target)
		texture_target = texture_type;

	if (texture_type == CL_TEXTURE_1D)
	{
		clFramebufferTexture1D(target, opengl_attachment, texture_target, texture_handle, level);
	}
	else if (texture_type == CL_TEXTURE_2D)
	{
		clFramebufferTexture2D(target, opengl_attachment, texture_target, texture_handle, level);
	}
	else if (texture_type == CL_TEXTURE_3D)
	{
		clFramebufferTexture3D(target, opengl_attachment, texture_target, texture_handle, level, zoffset);
	}
	return is_replaced_object;
}

void CL_OpenGLFrameBufferProvider::detach_object(CLenum opengl_attachment, const CL_RenderBuffer &render_buffer)
{
	int internal_attachment_offset = decode_internal_attachment_offset(opengl_attachment);

	CLenum target = CL_DRAW_FRAMEBUFFER;
	if (bind_target == cl_framebuffer_read)
		target = CL_READ_FRAMEBUFFER;

	clFramebufferRenderbuffer(target, opengl_attachment, CL_RENDERBUFFER, 0);

	attached_renderbuffers[internal_attachment_offset] = CL_RenderBuffer();
}

void CL_OpenGLFrameBufferProvider::detach_object(CLenum opengl_attachment, const CL_Texture &texture)
{
	int internal_attachment_offset = decode_internal_attachment_offset(opengl_attachment);

	CLenum target = CL_DRAW_FRAMEBUFFER;
	if (bind_target == cl_framebuffer_read)
		target = CL_READ_FRAMEBUFFER;

	CL_OpenGLTextureProvider *gl_texture_provider = dynamic_cast<CL_OpenGLTextureProvider*>(texture.get_provider());

	if (!gl_texture_provider)
			throw CL_Exception("Invalid texture");

	CLuint texture_type = gl_texture_provider->get_texture_type();
	CLuint texture_handle = 0;

	if (texture_type == CL_TEXTURE_1D)
	{
		clFramebufferTexture1D(target, opengl_attachment, texture_type, texture_handle, 0);
	}
	else if (texture_type == CL_TEXTURE_2D)
	{
		clFramebufferTexture2D(target, opengl_attachment, texture_type, texture_handle, 0);
	}
	else if (texture_type == CL_TEXTURE_3D)
	{
		clFramebufferTexture3D(target, opengl_attachment, texture_type, texture_handle, 0, 0);
	}

	attached_textures[internal_attachment_offset] = CL_Texture();
}

CLuint CL_OpenGLFrameBufferProvider::decode_texture_subtype(CL_TextureSubtype subtype)
{
	CLuint texture_target;
	switch (subtype)
	{
		case cl_subtype_cube_map_positive_x:
			texture_target = CL_TEXTURE_CUBE_MAP_POSITIVE_X;
			break;
		case cl_subtype_cube_map_negative_x:
			texture_target = CL_TEXTURE_CUBE_MAP_NEGATIVE_X;
			break;
		case cl_subtype_cube_map_positive_y:
			texture_target = CL_TEXTURE_CUBE_MAP_POSITIVE_Y;
			break;
		case cl_subtype_cube_map_negative_y:
			texture_target = CL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
			break;
		case cl_subtype_cube_map_positive_z:
			texture_target = CL_TEXTURE_CUBE_MAP_POSITIVE_Z;
			break;
		case cl_subtype_cube_map_negative_z:
			texture_target = CL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
			break;
		default:
			texture_target = 0;
	}
	return texture_target;
}

