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
**    Mark Page
*/

#include "SWRender/precomp.h"
#include "API/SWRender/swr_graphic_context.h"
#include "API/SWRender/pixel_command.h"

// NON-SSE2 stub

/////////////////////////////////////////////////////////////////////////////
// CL_GraphicContext_SWRender_Impl Class:

class CL_GraphicContext_SWRender_Impl
{
public:
	CL_GraphicContext_SWRender_Impl()
	{
	}

	~CL_GraphicContext_SWRender_Impl()
	{
	}
};

/////////////////////////////////////////////////////////////////////////////
// CL_GraphicContext_SWRender Construction:

CL_GraphicContext_SWRender::CL_GraphicContext_SWRender(CL_GraphicContext &gc) : CL_GraphicContext(gc),
 impl(new CL_GraphicContext_SWRender_Impl)
{
}

CL_GraphicContext_SWRender::~CL_GraphicContext_SWRender()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_GraphicContext_SWRender Attributes:

void CL_GraphicContext_SWRender::throw_if_null() const
{
	if (!impl)
		throw CL_Exception("CL_GraphicContext_SWRender is null");
}

CL_PixelPipeline *CL_GraphicContext_SWRender::get_pipeline() const
{
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GraphicContext_SWRender Operations:

void CL_GraphicContext_SWRender::draw_pixels_bicubic(int x, int y, int zoom_number, int zoom_denominator, const CL_PixelBuffer &pixels)
{
}

void CL_GraphicContext_SWRender::queue_command(CL_UniquePtr<CL_PixelCommand> &command)
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_GraphicContext_SWRender Implementation:

