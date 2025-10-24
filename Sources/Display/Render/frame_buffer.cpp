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

#include "Display/precomp.h"
#include "API/Display/Render/frame_buffer.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/Display/TargetProviders/frame_buffer_provider.h"

class CL_FrameBuffer_Impl
{
public:
	CL_FrameBuffer_Impl() : provider(0)
	{
	}

	~CL_FrameBuffer_Impl()
	{
		if (provider)
			provider->destroy();
	}

	CL_FrameBufferProvider *provider;
};


/////////////////////////////////////////////////////////////////////////////
// CL_FrameBuffer Construction:

CL_FrameBuffer::CL_FrameBuffer()
{
}

CL_FrameBuffer::CL_FrameBuffer(CL_GraphicContext &context)
: impl(new CL_FrameBuffer_Impl)
{
	CL_GraphicContextProvider *gc_provider = context.get_provider();
	impl->provider = gc_provider->alloc_frame_buffer();
}

/////////////////////////////////////////////////////////////////////////////
// CL_FrameBuffer Attributes:

void CL_FrameBuffer::throw_if_null() const
{
	if (impl.is_null())
		throw CL_Exception("CL_FrameBuffer is null");
}

CL_FrameBufferProvider *CL_FrameBuffer::get_provider() const
{
	return impl->provider;
}

CL_Size CL_FrameBuffer::get_size() const
{
	return impl->provider->get_size();
}

CL_FrameBufferBindTarget CL_FrameBuffer::get_bind_target() const
{
	return impl->provider->get_bind_target();
}

/////////////////////////////////////////////////////////////////////////////
// CL_FrameBuffer Operations:

void CL_FrameBuffer::attach_color_buffer(int attachment_index, const CL_RenderBuffer &render_buffer)
{
	impl->provider->attach_color_buffer(attachment_index, render_buffer);
}

void CL_FrameBuffer::detach_color_buffer( int attachment_index, const CL_RenderBuffer &render_buffer )
{
	impl->provider->detach_color_buffer(attachment_index, render_buffer);
}

void CL_FrameBuffer::attach_color_buffer(int attachment_index, const CL_Texture &texture, int level, int zoffset)
{
	impl->provider->attach_color_buffer(attachment_index, texture, level, zoffset);
}

void CL_FrameBuffer::attach_color_buffer(int attachment_index, const CL_Texture &texture, CL_TextureSubtype subtype, int level, int zoffset)
{
	impl->provider->attach_color_buffer(attachment_index, texture, subtype, level, zoffset);
}

void CL_FrameBuffer::detach_color_buffer(int attachment_index, const CL_Texture &texture, int level, int zoffset)
{
	impl->provider->detach_color_buffer(attachment_index, texture, level, zoffset);
}

void CL_FrameBuffer::attach_stencil_buffer(const CL_RenderBuffer &render_buffer)
{
	impl->provider->attach_stencil_buffer(render_buffer);
}

void CL_FrameBuffer::detach_stencil_buffer( const CL_RenderBuffer &render_buffer )
{
	impl->provider->detach_stencil_buffer(render_buffer);
}

void CL_FrameBuffer::attach_stencil_buffer(const CL_Texture &texture, int level, int zoffset)
{
	impl->provider->attach_stencil_buffer(texture, level, zoffset);
}

void CL_FrameBuffer::attach_stencil_buffer(const CL_Texture &texture, CL_TextureSubtype subtype, int level, int zoffset)
{
	impl->provider->attach_stencil_buffer(texture, subtype, level, zoffset);
}

void CL_FrameBuffer::detach_stencil_buffer(const CL_Texture &texture, int level, int zoffset)
{
	impl->provider->detach_stencil_buffer(texture, level, zoffset);
}

void CL_FrameBuffer::attach_depth_buffer(const CL_RenderBuffer &render_buffer)
{
	impl->provider->attach_depth_buffer(render_buffer);
}

void CL_FrameBuffer::detach_depth_buffer( const CL_RenderBuffer &render_buffer )
{
	impl->provider->detach_depth_buffer(render_buffer);
}

void CL_FrameBuffer::attach_depth_buffer(const CL_Texture &texture, int level, int zoffset)
{
	impl->provider->attach_depth_buffer(texture, level, zoffset);
}

void CL_FrameBuffer::attach_depth_buffer(const CL_Texture &texture, CL_TextureSubtype subtype, int level, int zoffset)
{
	impl->provider->attach_depth_buffer(texture, subtype, level, zoffset);
}

void CL_FrameBuffer::detach_depth_buffer(const CL_Texture &texture, int level, int zoffset)
{
	impl->provider->detach_depth_buffer(texture, level, zoffset);
}

void CL_FrameBuffer::attach_depth_stencil_buffer(const CL_RenderBuffer &render_buffer)
{
	impl->provider->attach_depth_stencil_buffer(render_buffer);
}

void CL_FrameBuffer::detach_depth_stencil_buffer( const CL_RenderBuffer &render_buffer )
{
	impl->provider->detach_depth_stencil_buffer(render_buffer);
}

void CL_FrameBuffer::attach_depth_stencil_buffer(const CL_Texture &texture, int level, int zoffset)
{
	impl->provider->attach_depth_stencil_buffer(texture, level, zoffset);
}

void CL_FrameBuffer::attach_depth_stencil_buffer(const CL_Texture &texture, CL_TextureSubtype subtype, int level, int zoffset)
{
	impl->provider->attach_depth_stencil_buffer(texture, subtype, level, zoffset);
}

void CL_FrameBuffer::detach_depth_stencil_buffer(const CL_Texture &texture, int level, int zoffset)
{
	impl->provider->detach_depth_stencil_buffer(texture, level, zoffset);
}

void CL_FrameBuffer::set_bind_target(CL_FrameBufferBindTarget target)
{
	impl->provider->set_bind_target(target);
}

/////////////////////////////////////////////////////////////////////////////
// CL_FrameBuffer Implementation:
