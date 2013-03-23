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

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// SWRenderPixelBufferProvider Construction:

SWRenderPixelBufferProvider::SWRenderPixelBufferProvider()
{
}

SWRenderPixelBufferProvider::~SWRenderPixelBufferProvider()
{
}

void SWRenderPixelBufferProvider::create(const void *data, const Size &new_size, PixelBufferDirection direction, TextureFormat new_format, BufferUsage usage)
{
	// Maybe create a normal PixelBuffer inside here to store the data?
}

/////////////////////////////////////////////////////////////////////////////
// SWRenderPixelBufferProvider Attributes:

void *SWRenderPixelBufferProvider::get_data()
{
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// SWRenderPixelBufferProvider Operations:

void SWRenderPixelBufferProvider::lock(GraphicContext &gc, BufferAccess access)
{
}
void SWRenderPixelBufferProvider::unlock()
{
}
void SWRenderPixelBufferProvider::upload_data(GraphicContext &gc, const Rect &dest_rect, const void *data)
{
}

/////////////////////////////////////////////////////////////////////////////
// SWRenderPixelBufferProvider Implementation:

}
