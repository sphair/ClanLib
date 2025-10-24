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
*/

#include "D3D9/precomp.h"
#include "d3d9_frame_buffer_provider.h"
#include "API/Core/Math/size.h"

/////////////////////////////////////////////////////////////////////////////
// CL_D3D9FrameBufferProvider Construction:

CL_D3D9FrameBufferProvider::CL_D3D9FrameBufferProvider()
{
}

CL_D3D9FrameBufferProvider::~CL_D3D9FrameBufferProvider()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_D3D9FrameBufferProvider Attributes:

CL_Size CL_D3D9FrameBufferProvider::get_attachment_size(int buffer_id) const
{
	return CL_Size();
}

/////////////////////////////////////////////////////////////////////////////
// CL_D3D9FrameBufferProvider Operations:

void CL_D3D9FrameBufferProvider::destroy()
{
	delete this;
}

void CL_D3D9FrameBufferProvider::attach_color_buffer(int color_buffer, const CL_RenderBuffer &render_buffer)
{
}

void CL_D3D9FrameBufferProvider::attach_color_buffer(int color_buffer, const CL_Texture &texture, int level, int zoffset)
{
}

void CL_D3D9FrameBufferProvider::attach_color_buffer(int color_buffer, const CL_Texture &texture, CL_TextureSubtype subtype, int level, int zoffset)
{
}

void CL_D3D9FrameBufferProvider::detach_color_buffer(int color_buffer)
{
}

void CL_D3D9FrameBufferProvider::attach_stencil_buffer(const CL_RenderBuffer &render_buffer)
{
}

void CL_D3D9FrameBufferProvider::detach_stencil_buffer()
{
}

void CL_D3D9FrameBufferProvider::attach_depth_buffer(const CL_RenderBuffer &render_buffer)
{
}

void CL_D3D9FrameBufferProvider::attach_depth_buffer(const CL_Texture &texture, CL_TextureSubtype subtype, int level, int zoffset)
{
}

void CL_D3D9FrameBufferProvider::detach_depth_buffer()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_D3D9FrameBufferProvider Implementation:
