/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

#include "Compute/precomp.h"
#include "API/Compute/compute_kernel.h"
#include "API/Compute/compute_buffer.h"
#include "API/Compute/compute_context.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/Render/render_buffer.h"
#include "GL/GL3/gl3_pixel_buffer_provider.h"
#include "GL/GL3/gl3_vertex_array_buffer_provider.h"
#include "GL/GL3/gl3_element_array_buffer_provider.h"
#include "GL/GL3/gl3_texture_provider.h"
#include "GL/GL3/gl3_render_buffer_provider.h"
#ifdef WIN32
#include <D3D11.h>
#include "D3D/d3d_pixel_buffer_provider.h"
#include "D3D/d3d_vertex_array_buffer_provider.h"
#include "D3D/d3d_element_array_buffer_provider.h"
#include "D3D/d3d_texture_provider.h"
#include "D3D/d3d_render_buffer_provider.h"
#endif
#include "compute_buffer_impl.h"
#include "compute_context_impl.h"

namespace clan
{

ComputeBuffer::ComputeBuffer()
{
}

ComputeBuffer::ComputeBuffer(ComputeContext &context, int size, BufferAccess access, bool alloc_host_pointer)
: impl(std::make_shared<ComputeBuffer_Impl>(context, size, access, alloc_host_pointer))
{
}

ComputeBuffer::~ComputeBuffer()
{
}

void ComputeBuffer::throw_if_null() const
{
	if (is_null())
		throw Exception("CL_ComputeBuffer is null!");
}

ComputeBuffer ComputeBuffer::from_pixel_buffer(ComputeContext &context, PixelBuffer &pixel_buffer, BufferAccess access)
{
	GL3PixelBufferProvider *gl_provider = dynamic_cast<GL3PixelBufferProvider*>(pixel_buffer.get_provider());
	if (gl_provider)
	{
		ComputeBuffer buffer;
		buffer.impl = std::shared_ptr<ComputeBuffer_Impl>(new ComputeBuffer_Impl(context));
		buffer.impl->handle = buffer.impl->bindings->CreateFromGLBuffer(context.impl->handle, ComputeBuffer_Impl::to_mem_flags(access), gl_provider->get_handle(), 0);
		if (buffer.impl->handle == 0)
			throw Exception("clCreateFromGLBuffer failed");
		return buffer;
	}
	else
	{
		throw Exception("Unsupported display target");
	}
}

ComputeBuffer ComputeBuffer::from_vertex_buffer(ComputeContext &context, VertexArrayBuffer &vertex_buffer, BufferAccess access)
{
	GL3VertexArrayBufferProvider *gl_provider = dynamic_cast<GL3VertexArrayBufferProvider*>(vertex_buffer.get_provider());
#ifdef WIN32
	D3DVertexArrayBufferProvider *d3d_provider = dynamic_cast<D3DVertexArrayBufferProvider*>(vertex_buffer.get_provider());
#endif
	if (gl_provider)
	{
		ComputeBuffer buffer;
		buffer.impl = std::shared_ptr<ComputeBuffer_Impl>(new ComputeBuffer_Impl(context));
		buffer.impl->handle = buffer.impl->bindings->CreateFromGLBuffer(context.impl->handle, ComputeBuffer_Impl::to_mem_flags(access), gl_provider->get_handle(), 0);
		if (buffer.impl->handle == 0)
			throw Exception("clCreateFromGLBuffer failed");
		return buffer;
	}
#ifdef WIN32
	else if (d3d_provider)
	{
		ComputeBuffer buffer;
		buffer.impl = std::shared_ptr<ComputeBuffer_Impl>(new ComputeBuffer_Impl(context));
		buffer.impl->handle = buffer.impl->bindings->CreateFromD3D11BufferKHR(context.impl->handle, ComputeBuffer_Impl::to_mem_flags(access), d3d_provider->get_buffer(ComPtr<ID3D11Device>()), 0);
		if (buffer.impl->handle == 0)
			throw Exception("clCreateFromD3D11BufferKHR failed");
		return buffer;
	}
#endif
	else
	{
		throw Exception("Unsupported display target");
	}
}

ComputeBuffer ComputeBuffer::from_element_buffer(ComputeContext &context, ElementArrayBuffer &element_buffer, BufferAccess access)
{
	GL3ElementArrayBufferProvider *gl_provider = dynamic_cast<GL3ElementArrayBufferProvider*>(element_buffer.get_provider());
#ifdef WIN32
	D3DElementArrayBufferProvider *d3d_provider = dynamic_cast<D3DElementArrayBufferProvider*>(element_buffer.get_provider());
#endif
	if (gl_provider)
	{
		ComputeBuffer buffer;
		buffer.impl = std::shared_ptr<ComputeBuffer_Impl>(new ComputeBuffer_Impl(context));
		buffer.impl->handle = buffer.impl->bindings->CreateFromGLBuffer(context.impl->handle, ComputeBuffer_Impl::to_mem_flags(access), gl_provider->get_handle(), 0);
		if (buffer.impl->handle == 0)
			throw Exception("clCreateFromGLBuffer failed");
		return buffer;
	}
#ifdef WIN32
	else if (d3d_provider)
	{
		ComputeBuffer buffer;
		buffer.impl = std::shared_ptr<ComputeBuffer_Impl>(new ComputeBuffer_Impl(context));
		buffer.impl->handle = buffer.impl->bindings->CreateFromD3D11BufferKHR(context.impl->handle, ComputeBuffer_Impl::to_mem_flags(access), d3d_provider->get_buffer(ComPtr<ID3D11Device>()), 0);
		if (buffer.impl->handle == 0)
			throw Exception("clCreateFromD3D11BufferKHR failed");
		return buffer;
	}
#endif
	else
	{
		throw Exception("Unsupported display target");
	}
}

ComputeBuffer ComputeBuffer::from_texture(ComputeContext &context, Texture &texture, int mipmap_level, BufferAccess access)
{
	GL3TextureProvider *gl_provider = dynamic_cast<GL3TextureProvider*>(texture.get_provider());
#ifdef WIN32
	D3DTextureProvider *d3d_provider = dynamic_cast<D3DTextureProvider*>(texture.get_provider());
#endif
	if (gl_provider)
	{
		ComputeBuffer buffer;
		buffer.impl = std::shared_ptr<ComputeBuffer_Impl>(new ComputeBuffer_Impl(context));
		if (buffer.impl->bindings->CreateFromGLTexture)
		{
			buffer.impl->handle = buffer.impl->bindings->CreateFromGLTexture(context.impl->handle, ComputeBuffer_Impl::to_mem_flags(access), gl_provider->get_texture_type(), mipmap_level, gl_provider->get_handle(), 0);
			if (buffer.impl->handle == 0)
				throw Exception("clCreateFromGLTexture failed");
		}
		else if (gl_provider->get_texture_type() == GL_TEXTURE_3D)
		{
			buffer.impl->handle = buffer.impl->bindings->CreateFromGLTexture3D(context.impl->handle, ComputeBuffer_Impl::to_mem_flags(access), gl_provider->get_texture_type(), mipmap_level, gl_provider->get_handle(), 0);
			if (buffer.impl->handle == 0)
				throw Exception("clCreateFromGLTexture3D failed");
		}
		else
		{
			buffer.impl->handle = buffer.impl->bindings->CreateFromGLTexture2D(context.impl->handle, ComputeBuffer_Impl::to_mem_flags(access), gl_provider->get_texture_type(), mipmap_level, gl_provider->get_handle(), 0);
			if (buffer.impl->handle == 0)
				throw Exception("clCreateFromGLTexture2D failed");
		}

		return buffer;
	}
#ifdef WIN32
	else if (d3d_provider)
	{
		ComputeBuffer buffer;
		buffer.impl = std::shared_ptr<ComputeBuffer_Impl>(new ComputeBuffer_Impl(context));
		buffer.impl->handle = buffer.impl->bindings->CreateFromD3D11Texture2DKHR(context.impl->handle, ComputeBuffer_Impl::to_mem_flags(access), d3d_provider->get_texture_2d(ComPtr<ID3D11Device>()), mipmap_level, 0);
		if (buffer.impl->handle == 0)
			throw Exception("clCreateFromD3D11Texture2DKHR failed");
		return buffer;
	}
#endif
	else
	{
		throw Exception("Unsupported display target");
	}
}

ComputeBuffer ComputeBuffer::from_render_buffer(ComputeContext &context, RenderBuffer &render_buffer, BufferAccess access)
{
	GL3RenderBufferProvider *gl_provider = dynamic_cast<GL3RenderBufferProvider*>(render_buffer.get_provider());
#ifdef WIN32
	D3DRenderBufferProvider *d3d_provider = dynamic_cast<D3DRenderBufferProvider*>(render_buffer.get_provider());
#endif
	if (gl_provider)
	{
		ComputeBuffer buffer;
		buffer.impl = std::shared_ptr<ComputeBuffer_Impl>(new ComputeBuffer_Impl(context));
		buffer.impl->handle = buffer.impl->bindings->CreateFromGLRenderbuffer(context.impl->handle, ComputeBuffer_Impl::to_mem_flags(access), gl_provider->get_handle(), 0);
		if (buffer.impl->handle == 0)
			throw Exception("clCreateFromGLBuffer failed");
		return buffer;
	}
#ifdef WIN32
	else if (d3d_provider)
	{
		ComputeBuffer buffer;
		buffer.impl = std::shared_ptr<ComputeBuffer_Impl>(new ComputeBuffer_Impl(context));
		buffer.impl->handle = buffer.impl->bindings->CreateFromD3D11Texture2DKHR(context.impl->handle, ComputeBuffer_Impl::to_mem_flags(access), d3d_provider->get_texture(ComPtr<ID3D11Device>()), 0, 0);
		if (buffer.impl->handle == 0)
			throw Exception("clCreateFromD3D11Texture2DKHR failed");
		return buffer;
	}
#endif
	else
	{
		throw Exception("Unsupported display target");
	}
}

}
