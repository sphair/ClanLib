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
**    Harry Storbacka
*/

#include "Display/precomp.h"
#include "API/Display/2D/rounded_rect.h"
#include "rounded_rect_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_RoundedRect Construction:

CL_RoundedRect::CL_RoundedRect(CL_Sizef size, float rounding_in_pixels)
: impl(new CL_RoundedRect_Impl)
{
	impl->size = size;
	impl->rounding = rounding_in_pixels;
}

CL_RoundedRect::~CL_RoundedRect()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_RoundedRect Attributes:

CL_Sizef CL_RoundedRect::get_size() const
{
	return impl->size;
}

float CL_RoundedRect::get_width() const
{
	return impl->size.width;
}

float CL_RoundedRect::get_height() const
{
	return impl->size.height;
}

CL_Pointf CL_RoundedRect::get_control_point_tl() const
{
	return impl->cp_tl;
}

CL_Pointf CL_RoundedRect::get_control_point_tr() const
{
	return impl->cp_tr;
}

CL_Pointf CL_RoundedRect::get_control_point_bl() const
{
	return impl->cp_bl;
}

CL_Pointf CL_RoundedRect::get_control_point_br() const
{
	return impl->cp_br;
}

float CL_RoundedRect::get_rounding() const
{
	return impl->rounding;
}

CL_Sizef CL_RoundedRect::get_rounding_top_left() const
{
	return impl->round_tl;
}

CL_Sizef CL_RoundedRect::get_rounding_top_right() const
{
	return impl->round_tr;
}

CL_Sizef CL_RoundedRect::get_rounding_bottom_left() const
{
	return impl->round_bl;
}

CL_Sizef CL_RoundedRect::get_rounding_bottom_right() const
{
	return impl->round_br;
}

/////////////////////////////////////////////////////////////////////////////
// CL_RoundedRect Operations:

void CL_RoundedRect::draw(
	CL_GraphicContext &gc,
	const CL_Pointf &position,
	const CL_Colorf &color,
	CL_Origin origin)
{
	impl->draw(gc, position, color, origin);
}

void CL_RoundedRect::fill(
	CL_GraphicContext &gc,
	const CL_Pointf &position,
	const CL_Colorf &color,
	CL_Origin origin)
{
	impl->fill(gc, position, color, origin);
}

void CL_RoundedRect::fill(
	CL_GraphicContext &gc,
	const CL_Pointf &position,
	const CL_Gradient &gradient,
	CL_Origin origin)
{
	impl->fill(gc, position, gradient, origin);
}

void CL_RoundedRect::set_size(const CL_Sizef &size)
{
	impl->size = size;
	impl->outline_needs_update = true;
	impl->triangulation_needs_update = true;
}

void CL_RoundedRect::set_control_point_tl(const CL_Pointf &pos)
{
	impl->cp_tl = pos;
	impl->outline_needs_update = true;
	impl->triangulation_needs_update = true;
}

void CL_RoundedRect::set_control_point_tr(const CL_Pointf &pos)
{
	impl->cp_tr = pos;
	impl->outline_needs_update = true;
	impl->triangulation_needs_update = true;
}

void CL_RoundedRect::set_control_point_bl(const CL_Pointf &pos)
{
	impl->cp_bl = pos;
	impl->outline_needs_update = true;
	impl->triangulation_needs_update = true;
}

void CL_RoundedRect::set_control_point_br(const CL_Pointf &pos)
{
	impl->cp_br = pos;
	impl->outline_needs_update = true;
	impl->triangulation_needs_update = true;
}

void CL_RoundedRect::set_rounding(float offset)
{
	impl->rounding = offset;
	impl->outline_needs_update = true;
	impl->triangulation_needs_update = true;
}

void CL_RoundedRect::set_rounding_top_left(const CL_Sizef &offset)
{
	impl->round_tl = offset;
	impl->outline_needs_update = true;
	impl->triangulation_needs_update = true;
}

void CL_RoundedRect::set_rounding_top_right(const CL_Sizef &offset)
{
	impl->round_tr = offset;
	impl->outline_needs_update = true;
	impl->triangulation_needs_update = true;
}

void CL_RoundedRect::set_rounding_bottom_left(const CL_Sizef &offset)
{
	impl->round_bl = offset;
	impl->outline_needs_update = true;
	impl->triangulation_needs_update = true;
}

void CL_RoundedRect::set_rounding_bottom_right(const CL_Sizef &offset)
{
	impl->round_br = offset;
	impl->outline_needs_update = true;
	impl->triangulation_needs_update = true;
}

/////////////////////////////////////////////////////////////////////////////
// CL_RoundedRect Implementation:
