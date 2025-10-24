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

#include "SWRender/precomp.h"
#include "swr_pixel_buffer_provider.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SWRenderPixelBufferProvider Construction:

CL_SWRenderPixelBufferProvider::CL_SWRenderPixelBufferProvider()
{
}

CL_SWRenderPixelBufferProvider::~CL_SWRenderPixelBufferProvider()
{
}

void CL_SWRenderPixelBufferProvider::create(const void *data, int new_width, int new_height, CL_PixelBufferDirection direction, CL_TextureFormat new_format, CL_BufferUsage usage)
{
	// Maybe create a normal CL_PixelBuffer inside here to store the data?
}

void CL_SWRenderPixelBufferProvider::destroy()
{
	delete this;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SWRenderPixelBufferProvider Attributes:

void *CL_SWRenderPixelBufferProvider::get_data()
{
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CL_SWRenderPixelBufferProvider Operations:

void CL_SWRenderPixelBufferProvider::lock(CL_BufferAccess access)
{
}

void CL_SWRenderPixelBufferProvider::unlock()
{
}

void CL_SWRenderPixelBufferProvider::upload_data(const CL_Rect &dest_rect, const void *data)
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_SWRenderPixelBufferProvider Implementation:
