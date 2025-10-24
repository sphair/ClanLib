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
: CL_FrameBufferProvider(), gc_provider(gc_provider)
{
	handles[0] = 0;
	handles[1] = 0;
	CL_SharedGCData::add_disposable(this);

	CL_OpenGL::set_active(gc_provider);

	if(clGenFramebuffers == 0)
		throw CL_Exception("Framebuffer not supported on your graphics card.");

	clGenFramebuffers(2, handles);
}

CL_OpenGLFrameBufferProvider::~CL_OpenGLFrameBufferProvider()
{
	dispose();
	CL_SharedGCData::remove_disposable(this);
}

void CL_OpenGLFrameBufferProvider::on_dispose()
{
	if (handles[0])
	{
		if (CL_OpenGL::set_active())
		{
			clDeleteFramebuffers(2, handles);
		}
	}
}
/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLFrameBufferProvider Attributes:

CLuint CL_OpenGLFrameBufferProvider::get_draw_handle()
{
	return handles[0];
}

CLuint CL_OpenGLFrameBufferProvider::get_read_handle()
{
	return handles[1];
}

CL_Size CL_OpenGLFrameBufferProvider::get_attachment_size(int buffer_id) const
{
	return sizes[buffer_id];
}

const std::vector<int> &CL_OpenGLFrameBufferProvider::get_attachment_indexes() const
{
	return attachment_indexes;
}

/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLFrameBufferProvider Operations:


void CL_OpenGLFrameBufferProvider::destroy()
{
	delete this;
}

void CL_OpenGLFrameBufferProvider::attach_color_buffer(int color_buffer, const CL_RenderBuffer &render_buffer)
{
	CL_FrameBufferStateTracker tracker_draw(CL_FrameBufferStateTracker::type_draw, handles[0], gc_provider);
	CL_FrameBufferStateTracker tracker_read(CL_FrameBufferStateTracker::type_read, handles[1], gc_provider);

	CL_OpenGLRenderBufferProvider *ogl_provider = dynamic_cast<CL_OpenGLRenderBufferProvider*>(render_buffer.get_provider());
	CLuint render_buffer_handle = ogl_provider->get_handle();
	clFramebufferRenderbuffer(CL_FRAMEBUFFER, CL_COLOR_ATTACHMENT0+color_buffer, CL_RENDERBUFFER, render_buffer_handle);
	clFramebufferRenderbuffer(CL_READ_FRAMEBUFFER, CL_COLOR_ATTACHMENT0+color_buffer, CL_RENDERBUFFER, render_buffer_handle);

	attachment_indexes.push_back(color_buffer);

	int error_code = clCheckFramebufferStatus(CL_FRAMEBUFFER);
	if (error_code != CL_FRAMEBUFFER_COMPLETE)
		throw CL_Exception(cl_format(cl_text("FrameBuffer is not FRAMEBUFFER_COMPLETE after attaching color buffer: %1"), get_error_message(error_code)));
}

void CL_OpenGLFrameBufferProvider::attach_color_buffer(int color_buffer, const CL_Texture &texture, int level, int zoffset)
{
	CL_FrameBufferStateTracker tracker_draw(CL_FrameBufferStateTracker::type_draw, handles[0], gc_provider);
	CL_FrameBufferStateTracker tracker_read(CL_FrameBufferStateTracker::type_read, handles[1], gc_provider);

	CL_OpenGLTextureProvider *provider = dynamic_cast<CL_OpenGLTextureProvider*>(texture.get_provider());
	CLuint texture_handle = provider->get_handle();
	CLuint texture_type = provider->get_texture_type();

	if (texture_type == CL_TEXTURE_1D)
	{
		clFramebufferTexture1D(CL_FRAMEBUFFER, CL_COLOR_ATTACHMENT0+color_buffer, texture_type, texture_handle, level);
		clFramebufferTexture1D(CL_READ_FRAMEBUFFER, CL_COLOR_ATTACHMENT0+color_buffer, texture_type, texture_handle, level);
	}
	else if (texture_type == CL_TEXTURE_2D)
	{
		clFramebufferTexture2D(CL_FRAMEBUFFER, CL_COLOR_ATTACHMENT0+color_buffer, texture_type, texture_handle, level);
		clFramebufferTexture2D(CL_READ_FRAMEBUFFER, CL_COLOR_ATTACHMENT0+color_buffer, texture_type, texture_handle, level);
	}
	else if (texture_type == CL_TEXTURE_3D)
	{
		clFramebufferTexture3D(CL_FRAMEBUFFER, CL_COLOR_ATTACHMENT0+color_buffer, texture_type, texture_handle, level, zoffset);
		clFramebufferTexture3D(CL_READ_FRAMEBUFFER, CL_COLOR_ATTACHMENT0+color_buffer, texture_type, texture_handle, level, zoffset);
	}

	// TODO: seems there's no way to detach if "texture_type = CL_RENDERBUFFER" - HS 14.10.2008

	sizes[color_buffer] = texture.get_size();
	attachment_indexes.push_back(color_buffer);

	int error_code = clCheckFramebufferStatus(CL_FRAMEBUFFER);
	if (error_code != CL_FRAMEBUFFER_COMPLETE)
		throw CL_Exception(cl_format(cl_text("FrameBuffer is not FRAMEBUFFER_COMPLETE after attaching color buffer: %1 (%2)"), get_error_message(error_code), error_code));
}

void CL_OpenGLFrameBufferProvider::attach_color_buffer(int color_buffer, const CL_Texture &texture, CL_TextureSubtype subtype, int level, int zoffset)
{
	CL_FrameBufferStateTracker tracker_draw(CL_FrameBufferStateTracker::type_draw, handles[0], gc_provider);
	CL_FrameBufferStateTracker tracker_read(CL_FrameBufferStateTracker::type_read, handles[1], gc_provider);
	throw CL_Exception(cl_text("CL_OpenGLFrameBufferProvider::attach_color_buffer not implemented for texture subtypes"));
}

void CL_OpenGLFrameBufferProvider::detach_color_buffer(int color_buffer, const CL_Texture &texture, int level, int zoffset)
{
	CL_FrameBufferStateTracker tracker_draw(CL_FrameBufferStateTracker::type_draw, handles[0], gc_provider);
	CL_FrameBufferStateTracker tracker_read(CL_FrameBufferStateTracker::type_read, handles[1], gc_provider);

	CL_OpenGLTextureProvider *provider = dynamic_cast<CL_OpenGLTextureProvider*>(texture.get_provider());
	CLuint texture_handle = 0; // provider->get_handle();
	CLuint texture_type = provider->get_texture_type();


	if (texture_type == CL_TEXTURE_1D)
	{
		clFramebufferTexture1D(CL_FRAMEBUFFER, CL_COLOR_ATTACHMENT0+color_buffer, texture_type, texture_handle, level);
		clFramebufferTexture1D(CL_READ_FRAMEBUFFER, CL_COLOR_ATTACHMENT0+color_buffer, texture_type, texture_handle, level);
	}
	else if (texture_type == CL_TEXTURE_2D)
	{
		clFramebufferTexture2D(CL_FRAMEBUFFER, CL_COLOR_ATTACHMENT0+color_buffer, texture_type, texture_handle, level);
		clFramebufferTexture2D(CL_READ_FRAMEBUFFER, CL_COLOR_ATTACHMENT0+color_buffer, texture_type, texture_handle, level);
	}
	else if (texture_type == CL_TEXTURE_3D)
	{
		clFramebufferTexture3D(CL_FRAMEBUFFER, CL_COLOR_ATTACHMENT0+color_buffer, texture_type, texture_handle, level, zoffset);
		clFramebufferTexture3D(CL_READ_FRAMEBUFFER, CL_COLOR_ATTACHMENT0+color_buffer, texture_type, texture_handle, level, zoffset);
	}

	for (std::vector<int>::iterator it = attachment_indexes.begin(); it != attachment_indexes.end(); ++it)
	{
		if ((*it) == color_buffer)
		{
			it = attachment_indexes.erase(it);
			break;
		}
	}
}

void CL_OpenGLFrameBufferProvider::attach_stencil_buffer(const CL_RenderBuffer &render_buffer)
{
	CL_FrameBufferStateTracker tracker_draw(CL_FrameBufferStateTracker::type_draw, handles[0], gc_provider);
	CL_FrameBufferStateTracker tracker_read(CL_FrameBufferStateTracker::type_read, handles[1], gc_provider);

	CL_OpenGLRenderBufferProvider *gl_render_buffer = static_cast<CL_OpenGLRenderBufferProvider *>(render_buffer.get_provider());
	clFramebufferRenderbuffer(CL_FRAMEBUFFER, CL_DEPTH_ATTACHMENT, CL_RENDERBUFFER, gl_render_buffer->get_handle());
	clFramebufferRenderbuffer(CL_READ_FRAMEBUFFER, CL_DEPTH_ATTACHMENT, CL_RENDERBUFFER, gl_render_buffer->get_handle());

	int error_code = clCheckFramebufferStatus(CL_FRAMEBUFFER);
	if (error_code != CL_FRAMEBUFFER_COMPLETE)
		throw CL_Exception(cl_format(cl_text("FrameBuffer is not FRAMEBUFFER_COMPLETE after attaching stencil buffer: %1"), get_error_message(error_code)));
}

void CL_OpenGLFrameBufferProvider::detach_stencil_buffer()
{
	CL_FrameBufferStateTracker tracker_draw(CL_FrameBufferStateTracker::type_draw, handles[0], gc_provider);
	CL_FrameBufferStateTracker tracker_read(CL_FrameBufferStateTracker::type_read, handles[1], gc_provider);

	clFramebufferRenderbuffer(CL_FRAMEBUFFER, CL_DEPTH_ATTACHMENT, CL_RENDERBUFFER, 0);
	clFramebufferRenderbuffer(CL_READ_FRAMEBUFFER, CL_DEPTH_ATTACHMENT, CL_RENDERBUFFER, 0);

	int error_code = clCheckFramebufferStatus(CL_FRAMEBUFFER);
	if (error_code != CL_FRAMEBUFFER_COMPLETE)
		throw CL_Exception(cl_format(cl_text("FrameBuffer is not FRAMEBUFFER_COMPLETE after detaching stencil buffer: %1"), get_error_message(error_code)));
}

void CL_OpenGLFrameBufferProvider::attach_depth_buffer(const CL_RenderBuffer &render_buffer)
{
	CL_FrameBufferStateTracker tracker_draw(CL_FrameBufferStateTracker::type_draw, handles[0], gc_provider);
	CL_FrameBufferStateTracker tracker_read(CL_FrameBufferStateTracker::type_read, handles[1], gc_provider);

	CL_OpenGLRenderBufferProvider *gl_render_buffer = static_cast<CL_OpenGLRenderBufferProvider *>(render_buffer.get_provider());
	clFramebufferRenderbuffer(CL_FRAMEBUFFER, CL_DEPTH_ATTACHMENT, CL_RENDERBUFFER, gl_render_buffer->get_handle());
	clFramebufferRenderbuffer(CL_READ_FRAMEBUFFER, CL_DEPTH_ATTACHMENT, CL_RENDERBUFFER, gl_render_buffer->get_handle());

	int error_code = clCheckFramebufferStatus(CL_FRAMEBUFFER);
	if (error_code != CL_FRAMEBUFFER_COMPLETE)
		throw CL_Exception(cl_format(cl_text("FrameBuffer is not FRAMEBUFFER_COMPLETE after attaching depth buffer: %1"), get_error_message(error_code)));
}

void CL_OpenGLFrameBufferProvider::attach_depth_buffer(const CL_Texture &texture, CL_TextureSubtype subtype, int level , int zoffset )
{
	//TODO: Fixme: Why is subtype not used?

	CL_FrameBufferStateTracker tracker_draw(CL_FrameBufferStateTracker::type_draw, handles[0], gc_provider);
	CL_FrameBufferStateTracker tracker_read(CL_FrameBufferStateTracker::type_read, handles[0], gc_provider);

	CL_OpenGLTextureProvider *provider = dynamic_cast<CL_OpenGLTextureProvider*>(texture.get_provider());
	CLuint texture_handle = provider->get_handle();
	CLuint texture_type = provider->get_texture_type();

	if (texture_type == CL_TEXTURE_1D)
	{
		clFramebufferTexture1D(CL_FRAMEBUFFER, CL_DEPTH_ATTACHMENT, texture_type, texture_handle, level);
		clFramebufferTexture1D(CL_READ_FRAMEBUFFER, CL_DEPTH_ATTACHMENT, texture_type, texture_handle, level);
	}
	if (texture_type == CL_TEXTURE_2D)
	{
		clFramebufferTexture2D(CL_FRAMEBUFFER, CL_DEPTH_ATTACHMENT, texture_type, texture_handle, level);
		clFramebufferTexture2D(CL_READ_FRAMEBUFFER, CL_DEPTH_ATTACHMENT, texture_type, texture_handle, level);
	}
	if (texture_type == CL_TEXTURE_3D)
	{
		clFramebufferTexture3D(CL_FRAMEBUFFER, CL_DEPTH_ATTACHMENT, texture_type, texture_handle, level, zoffset);
		clFramebufferTexture3D(CL_READ_FRAMEBUFFER, CL_DEPTH_ATTACHMENT, texture_type, texture_handle, level, zoffset);
	}

	int error_code = clCheckFramebufferStatus(CL_FRAMEBUFFER);
	if (error_code != CL_FRAMEBUFFER_COMPLETE)
		throw CL_Exception(cl_format(cl_text("FrameBuffer is not FRAMEBUFFER_COMPLETE after attaching depth buffer: %1"), get_error_message(error_code)));
}

void CL_OpenGLFrameBufferProvider::detach_depth_buffer()
{
	CL_FrameBufferStateTracker tracker_draw(CL_FrameBufferStateTracker::type_draw, handles[0], gc_provider);
	CL_FrameBufferStateTracker tracker_read(CL_FrameBufferStateTracker::type_read, handles[1], gc_provider);

	clFramebufferRenderbuffer(CL_FRAMEBUFFER, CL_DEPTH_ATTACHMENT, CL_RENDERBUFFER, 0);
	clFramebufferRenderbuffer(CL_READ_FRAMEBUFFER, CL_DEPTH_ATTACHMENT, CL_RENDERBUFFER, 0);
	clFramebufferTexture2D(CL_FRAMEBUFFER, CL_DEPTH_ATTACHMENT,  CL_TEXTURE_2D, 0, 0);
	clFramebufferTexture2D(CL_READ_FRAMEBUFFER, CL_DEPTH_ATTACHMENT,  CL_TEXTURE_2D, 0, 0);

	int error_code = clCheckFramebufferStatus(CL_FRAMEBUFFER);
	if (error_code != CL_FRAMEBUFFER_COMPLETE)
		throw CL_Exception(cl_format(cl_text("FrameBuffer is not FRAMEBUFFER_COMPLETE after detaching depth buffer: %1"), get_error_message(error_code)));

}


/////////////////////////////////////////////////////////////////////////////
// CL_OpenGLFrameBufferProvider Implementation:

CL_String CL_OpenGLFrameBufferProvider::get_error_message(int error_code)
{
	switch (error_code)
	{
	case CL_FRAMEBUFFER_COMPLETE:
		return cl_text("FRAMEBUFFER_COMPLETE");
	case CL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		return cl_text("FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
	case CL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		return cl_text("FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
	case CL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
		return cl_text("FRAMEBUFFER_INCOMPLETE_DIMENSIONS");
	case CL_FRAMEBUFFER_INCOMPLETE_FORMATS:
		return cl_text("FRAMEBUFFER_INCOMPLETE_FORMATS");
	case CL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		return cl_text("FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
	case CL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		return cl_text("FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
	case CL_FRAMEBUFFER_UNSUPPORTED:
		return cl_text("FRAMEBUFFER_UNSUPPORTED");
	default:
		return cl_format(cl_text("Error code: %1"), error_code);
	}
}

CL_FrameBufferStateTracker::CL_FrameBufferStateTracker(Type buffer_type, CLuint handle, CL_OpenGLGraphicContextProvider *gc_provider)
: buffer_type(buffer_type), last_bound(0), handle_and_bound_equal(false)
{
	CL_OpenGL::set_active(gc_provider);
	if (buffer_type == type_draw)
	{
		clGetIntegerv(CL_FRAMEBUFFER_BINDING, &last_bound);
		if (last_bound == handle)
		{
			handle_and_bound_equal = true;
		}
		else
		{
			clBindFramebuffer(CL_FRAMEBUFFER, handle);
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
		if (buffer_type == type_draw)
			clBindFramebuffer(CL_FRAMEBUFFER, last_bound);
		else
			clBindFramebuffer(CL_READ_FRAMEBUFFER, last_bound);
	}
}
