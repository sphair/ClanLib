/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include "Display/display_precomp.h"
#include "API/Display/canvas.h"
#include "canvas_generic.h"
#include "display_target.h"
#include "API/Display/pixel_buffer.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Canvas construction:

CL_Canvas::CL_Canvas() : impl(CL_DisplayTarget::current()->create_canvas())
{
}

CL_Canvas::CL_Canvas(const CL_Surface &initial_active_surface) : impl(CL_DisplayTarget::current()->create_canvas())
{
	impl->select_surface(initial_active_surface.impl);
}

CL_Canvas::CL_Canvas(const CL_Canvas &copy) : impl(copy.impl)
{
}

CL_Canvas::~CL_Canvas()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_Canvas attributes:

CL_GraphicContext *CL_Canvas::get_gc()
{
	return impl->get_gc();
}

CL_PixelBuffer CL_Canvas::get_pixeldata()
{
	return impl->get_pixeldata(CL_Rect(0, 0, get_width(), get_height()));
}

CL_PixelBuffer CL_Canvas::get_pixeldata(const CL_Rect &area)
{
	return impl->get_pixeldata(area);
}

int CL_Canvas::get_width() const
{
	return impl->get_width();
}

int CL_Canvas::get_height() const
{
	return impl->get_height();
}

/////////////////////////////////////////////////////////////////////////////
// CL_Canvas operations:

CL_Canvas &CL_Canvas::operator =(const CL_Canvas &copy)
{
	impl = copy.impl;
	return *this;
}

void CL_Canvas::set_pixeldata(const CL_PixelBuffer &data)
{
	impl->set_pixeldata(CL_Point(0,0), CL_Rect(0, 0, data.get_width(), data.get_height()), data);
}

void CL_Canvas::set_pixeldata(const CL_Point &dest, const CL_PixelBuffer &data)
{
	impl->set_pixeldata(dest, CL_Rect(0, 0, data.get_width(), data.get_height()), data);
}

void CL_Canvas::set_pixeldata(const CL_Point &dest, const CL_Rect &src, const CL_PixelBuffer &data)
{
	impl->set_pixeldata(dest, src, data);
}

void CL_Canvas::select_surface(CL_Surface &surface)
{
	impl->select_surface(surface.impl);
}

void CL_Canvas::sync_surface()
{
	impl->sync_surface();
}

/////////////////////////////////////////////////////////////////////////////
// CL_Canvas implementation:
