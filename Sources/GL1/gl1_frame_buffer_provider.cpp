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

#include "GL1/precomp.h"
#include "API/Display/Render/render_buffer.h"
#include "API/Display/TargetProviders/render_buffer_provider.h"
#include "API/Core/Text/string_format.h"
#include "API/Display/Render/texture.h"
#include "API/GL1/opengl1_wrap.h"
#include "API/GL1/opengl1.h"
#include "API/Display/Render/shared_gc_data.h"
#include "gl1_frame_buffer_provider.h"
#include "gl1_render_buffer_provider.h"
#include "gl1_texture_provider.h"

/////////////////////////////////////////////////////////////////////////////
// CL_GL1FrameBufferProvider Construction:

CL_GL1FrameBufferProvider::CL_GL1FrameBufferProvider(CL_GL1GraphicContextProvider *gc_provider)
: CL_FrameBufferProvider(), gc_provider(gc_provider)
{
	CL_SharedGCData::add_disposable(this);
}

CL_GL1FrameBufferProvider::~CL_GL1FrameBufferProvider()
{
	dispose();
	CL_SharedGCData::remove_disposable(this);
}

void CL_GL1FrameBufferProvider::on_dispose()
{
	std::vector<CL_GraphicContextProvider*> &opengl_contexts = CL_SharedGCData::get_gc_providers();
	if (!opengl_contexts.empty())
	{
		CL_GL1GraphicContextProvider *gc_provider = dynamic_cast<CL_GL1GraphicContextProvider*>(opengl_contexts[0]);
		if (gc_provider)
		{
			stop();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_GL1FrameBufferProvider Attributes:

CL_Size CL_GL1FrameBufferProvider::get_size() const
{
	if (selected_surface.is_null())
	{
		return CL_Size();
	}
	return selected_surface.get_size();
}

CL_FrameBufferBindTarget CL_GL1FrameBufferProvider::get_bind_target() const
{
	return cl_framebuffer_draw;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GL1FrameBufferProvider Operations:

void CL_GL1FrameBufferProvider::destroy()
{
	delete this;
}

void CL_GL1FrameBufferProvider::set_active() const
{
	pbuffer.set_active();
	pbuffer_changed = true;
}

void CL_GL1FrameBufferProvider::start()
{
	if (pbuffer.is_null())
	{
		throw CL_Exception("GL1 Framebuffer does not contain a texture");
	}
}

void CL_GL1FrameBufferProvider::stop()
{
	sync_texture();
}

void CL_GL1FrameBufferProvider::attach_color_buffer(int color_buffer, const CL_RenderBuffer &render_buffer)
{
	throw CL_Exception("GL1 RenderBuffers are not supported");
}

void CL_GL1FrameBufferProvider::detach_color_buffer(int color_buffer, const CL_RenderBuffer &render_buffer)
{
}

void CL_GL1FrameBufferProvider::attach_color_buffer(int color_buffer, const CL_Texture &texture, int level, int zoffset)
{
	if(!pbuffer.is_null())
		sync_texture();

	if (texture.is_null())
	{
		return;
	}

	CL_GL1TextureProvider *texture_provider = dynamic_cast<CL_GL1TextureProvider *> (texture.get_provider());
	if (texture_provider == NULL)
	{
		throw CL_Exception("Selected texture is not a GL1 texture");
	}

	CL_Size surface_size = texture_provider->get_surface_size();

	// Find existing pbuffer
	CL_WeakPtr<CL_Texture_Impl> texture_impl(texture.get_impl());
	std::map< CL_WeakPtr<CL_Texture_Impl>, CL_PBuffer_GL1>::iterator texture_it = texture_pbuffer_map.find(texture_impl);
	if (texture_it == texture_pbuffer_map.end())
	{
		// Not found, create a new entry
		pbuffer = gc_provider->create_pbuffer(surface_size);
		texture_pbuffer_map[texture_impl] = pbuffer;
	}
	else
	{
		// Used cached pbuffer
		pbuffer = texture_it->second;
	}

	selected_texture_provider = texture_provider;
	selected_surface = texture;

	sync_pbuffer();

	// Purge the cache from unused pbuffers. Maybe we could reuse them instead? But for this to occur, the user would be recreating CL_Textures anyway (which is slow). This added complexity is not required (at the moment)
	for (texture_it = texture_pbuffer_map.begin(); texture_it != texture_pbuffer_map.end();)
	{
		if (texture_it->first.is_null())
		{
			// This "texture_it = texture_pbuffer_map.erase(texture_it);" works on visual studio,
			// but MSDN says This return type does not conform to the C++ standard. So do it a different way, so GCC does not complain
			texture_pbuffer_map.erase(texture_it);
			texture_it = texture_pbuffer_map.begin();
		}
		else
		{
			++texture_it;
		}
	}
}

void CL_GL1FrameBufferProvider::attach_color_buffer(int color_buffer, const CL_Texture &texture, CL_TextureSubtype subtype, int level, int zoffset)
{
	throw CL_Exception("Implement me");
}

void CL_GL1FrameBufferProvider::detach_color_buffer(int color_buffer, const CL_Texture &texture, int level, int zoffset)
{
	detach_all();
}

void CL_GL1FrameBufferProvider::attach_stencil_buffer(const CL_RenderBuffer &render_buffer) {}
void CL_GL1FrameBufferProvider::detach_stencil_buffer(const CL_RenderBuffer &render_buffer) {}
void CL_GL1FrameBufferProvider::attach_stencil_buffer(const CL_Texture &texture, int level, int zoffset) {}
void CL_GL1FrameBufferProvider::attach_stencil_buffer(const CL_Texture &texture, CL_TextureSubtype subtype, int level, int zoffset) {}
void CL_GL1FrameBufferProvider::detach_stencil_buffer(const CL_Texture &texture, int level, int zoffset) {}

void CL_GL1FrameBufferProvider::attach_depth_buffer(const CL_RenderBuffer &render_buffer) {}
void CL_GL1FrameBufferProvider::detach_depth_buffer(const CL_RenderBuffer &render_buffer) {}
void CL_GL1FrameBufferProvider::attach_depth_buffer(const CL_Texture &texture, int level, int zoffset) {}
void CL_GL1FrameBufferProvider::attach_depth_buffer(const CL_Texture &texture, CL_TextureSubtype subtype, int level, int zoffset) {}
void CL_GL1FrameBufferProvider::detach_depth_buffer(const CL_Texture &texture, int level, int zoffset) {}

void CL_GL1FrameBufferProvider::attach_depth_stencil_buffer(const CL_RenderBuffer &render_buffer) {}
void CL_GL1FrameBufferProvider::detach_depth_stencil_buffer(const CL_RenderBuffer &render_buffer) {}
void CL_GL1FrameBufferProvider::attach_depth_stencil_buffer(const CL_Texture &texture, int level, int zoffset) {}
void CL_GL1FrameBufferProvider::attach_depth_stencil_buffer(const CL_Texture &texture, CL_TextureSubtype subtype, int level, int zoffset) {}
void CL_GL1FrameBufferProvider::detach_depth_stencil_buffer(const CL_Texture &texture, int level, int zoffset) {}

void CL_GL1FrameBufferProvider::set_bind_target(CL_FrameBufferBindTarget target)
{
	throw CL_Exception("Implement Me");
}

/////////////////////////////////////////////////////////////////////////////
// CL_GL1FrameBufferProvider Implementation:

void CL_GL1FrameBufferProvider::sync_pbuffer()
{
	set_active();

	CL_Size surface_size = selected_texture_provider->get_surface_size();
	CL_Size texture_size = selected_texture_provider->get_texture_size();

	CLint old_viewport[4], old_matrix_mode;
	CLfloat old_matrix_projection[16], old_matrix_modelview[16];
	cl1GetIntegerv(CL_VIEWPORT, old_viewport);
	cl1GetIntegerv(CL_MATRIX_MODE, &old_matrix_mode);
	cl1GetFloatv(CL_PROJECTION_MATRIX, old_matrix_projection);
	cl1GetFloatv(CL_MODELVIEW_MATRIX, old_matrix_modelview);
	CLboolean blending = cl1IsEnabled(CL_BLEND);
	cl1Disable(CL_BLEND);

	cl1Viewport(0, 0, surface_size.width, surface_size.height);
	cl1MatrixMode(CL_PROJECTION);
	cl1LoadIdentity();
	cl1MultMatrixf(CL_Mat4f::ortho_2d(0.0f, (float)surface_size.width, 0.0f, (float)surface_size.height));
	cl1MatrixMode(CL_MODELVIEW);
	cl1LoadIdentity();

	// Copy texture to pbuffer:

	cl1ActiveTexture( CL_TEXTURE0 + 0 );
	cl1Enable(CL_TEXTURE_2D);
	cl1BindTexture(CL_TEXTURE_2D, selected_texture_provider->get_handle());

	if (cl1ActiveTexture != 0)
			cl1ActiveTexture( CL_TEXTURE0 + 0 );

	cl1DisableClientState(CL_TEXTURE_COORD_ARRAY);

	cl1MatrixMode(CL_TEXTURE);
	CLfloat scale_matrix[16];
	memset(scale_matrix, 0, sizeof(CLfloat)*16);
	scale_matrix[0] = 1.0f/float(texture_size.width);
	scale_matrix[5] = 1.0f/float(texture_size.height);
	scale_matrix[10] = 1.0f;
	scale_matrix[15] = 1.0f;
	cl1LoadMatrixf(scale_matrix);
	cl1MatrixMode(CL_MODELVIEW);

	cl1Begin(CL_QUADS);
	cl1Color3f(1.0f, 1.0f, 1.0f);
	cl1TexCoord2f(float(surface_size.width), 0.0f);
	cl1Vertex2f(float(surface_size.width), 0.0f);
	cl1TexCoord2f(float(surface_size.width), float(surface_size.height));
	cl1Vertex2f(float(surface_size.width), float(surface_size.height));
	cl1TexCoord2f(0.0f, float(surface_size.height));
	cl1Vertex2f(0.0f, float(surface_size.height));
	cl1TexCoord2f(0.0f, 0.0f);
	cl1Vertex2f(0.0f, 0.0f);
	cl1End();
	cl1Disable(CL_TEXTURE_2D);
	cl1BindTexture(CL_TEXTURE_2D, 0);

	cl1MatrixMode(CL_TEXTURE);
	cl1LoadIdentity();
	cl1MatrixMode(CL_MODELVIEW);

	if (blending)
		cl1Enable(CL_BLEND);
	cl1Viewport(old_viewport[0], old_viewport[1], old_viewport[2], old_viewport[3]);
	cl1MatrixMode(CL_PROJECTION);
	cl1LoadMatrixf(old_matrix_projection);
	cl1MatrixMode(CL_MODELVIEW);
	cl1LoadMatrixf(old_matrix_modelview);
	cl1MatrixMode(old_matrix_mode);

#ifdef __APPLE__
	cl1Flush();
#endif

	pbuffer_changed = true;
}

void CL_GL1FrameBufferProvider::sync_texture()
{
	if (pbuffer_changed && (!pbuffer.is_null()))
	{
		set_active();

		CL_Size surface_size = selected_texture_provider->get_surface_size();

		CLint old_viewport[4], old_matrix_mode;
		CLfloat old_matrix_projection[16], old_matrix_modelview[16];
		cl1GetIntegerv(CL_VIEWPORT, old_viewport);
		cl1GetIntegerv(CL_MATRIX_MODE, &old_matrix_mode);
		cl1GetFloatv(CL_PROJECTION_MATRIX, old_matrix_projection);
		cl1GetFloatv(CL_MODELVIEW_MATRIX, old_matrix_modelview);
		CLboolean blending = cl1IsEnabled(CL_BLEND);
		cl1Disable(CL_BLEND);

		cl1Viewport(0, 0, surface_size.width, surface_size.height);
		cl1MatrixMode(CL_PROJECTION);
		cl1LoadIdentity();
		cl1MultMatrixf(CL_Mat4f::ortho_2d(0.0f, (float)surface_size.width, 0.0f, (float)surface_size.height));
		cl1MatrixMode(CL_MODELVIEW);
		cl1LoadIdentity();

		if (cl1ActiveTexture != 0)
			cl1ActiveTexture( CL_TEXTURE0 );

		cl1PixelZoom(1.0f, 1.0f);

		// Copy pbuffer to texture:
		cl1Enable(CL_TEXTURE_2D);
		cl1BindTexture(CL_TEXTURE_2D,  selected_texture_provider->get_handle());
		cl1CopyTexSubImage2D(
			CL_TEXTURE_2D,
			0,
			0, 0,
			0, 0,
			surface_size.width, surface_size.height);

		cl1Disable(CL_TEXTURE_2D);
		cl1BindTexture(CL_TEXTURE_2D, 0);

		if (blending)
			cl1Enable(CL_BLEND);
		cl1Viewport(old_viewport[0], old_viewport[1], old_viewport[2], old_viewport[3]);
		cl1MatrixMode(CL_PROJECTION);
		cl1LoadMatrixf(old_matrix_projection);
		cl1MatrixMode(CL_MODELVIEW);
		cl1LoadMatrixf(old_matrix_modelview);
		cl1MatrixMode(old_matrix_mode);

#ifdef __APPLE__
		cl1Flush();
#endif
	}
	pbuffer_changed = false;
}

void CL_GL1FrameBufferProvider::detach_all()
{
	stop();
	pbuffer = CL_PBuffer_GL1();
	selected_surface = CL_Texture();
}
