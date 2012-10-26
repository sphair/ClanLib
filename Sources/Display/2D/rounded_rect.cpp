/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// RoundedRect Construction:

RoundedRect::RoundedRect(Sizef size, float rounding_in_pixels)
: impl(new RoundedRect_Impl)
{
	impl->size = size;
	impl->rounding = rounding_in_pixels;
}

RoundedRect::~RoundedRect()
{
}

/////////////////////////////////////////////////////////////////////////////
// RoundedRect Attributes:

Sizef RoundedRect::get_size() const
{
	return impl->size;
}

float RoundedRect::get_width() const
{
	return impl->size.width;
}

float RoundedRect::get_height() const
{
	return impl->size.height;
}

Pointf RoundedRect::get_control_point_tl() const
{
	return impl->cp_tl;
}

Pointf RoundedRect::get_control_point_tr() const
{
	return impl->cp_tr;
}

Pointf RoundedRect::get_control_point_bl() const
{
	return impl->cp_bl;
}

Pointf RoundedRect::get_control_point_br() const
{
	return impl->cp_br;
}

float RoundedRect::get_rounding() const
{
	return impl->rounding;
}

Sizef RoundedRect::get_rounding_top_left() const
{
	return impl->round_tl;
}

Sizef RoundedRect::get_rounding_top_right() const
{
	return impl->round_tr;
}

Sizef RoundedRect::get_rounding_bottom_left() const
{
	return impl->round_bl;
}

Sizef RoundedRect::get_rounding_bottom_right() const
{
	return impl->round_br;
}

/////////////////////////////////////////////////////////////////////////////
// RoundedRect Operations:

void RoundedRect::draw(
	Canvas &canvas,
	const Pointf &position,
	const Colorf &color,
	Origin origin)
{
	impl->draw(canvas, position, color, origin);
}

void RoundedRect::fill(
	Canvas &canvas,
	const Pointf &position,
	const Colorf &color,
	Origin origin)
{
	impl->fill(canvas, position, color, origin);
}

void RoundedRect::fill(
	Canvas &canvas,
	const Pointf &position,
	const Gradient &gradient,
	Origin origin)
{
	impl->fill(canvas, position, gradient, origin);
}

void RoundedRect::set_size(const Sizef &size)
{
	impl->size = size;
	impl->outline_needs_update = true;
	impl->triangulation_needs_update = true;
}

void RoundedRect::set_control_point_tl(const Pointf &pos)
{
	impl->cp_tl = pos;
	impl->outline_needs_update = true;
	impl->triangulation_needs_update = true;
}

void RoundedRect::set_control_point_tr(const Pointf &pos)
{
	impl->cp_tr = pos;
	impl->outline_needs_update = true;
	impl->triangulation_needs_update = true;
}

void RoundedRect::set_control_point_bl(const Pointf &pos)
{
	impl->cp_bl = pos;
	impl->outline_needs_update = true;
	impl->triangulation_needs_update = true;
}

void RoundedRect::set_control_point_br(const Pointf &pos)
{
	impl->cp_br = pos;
	impl->outline_needs_update = true;
	impl->triangulation_needs_update = true;
}

void RoundedRect::set_rounding(float offset)
{
	impl->rounding = offset;
	impl->outline_needs_update = true;
	impl->triangulation_needs_update = true;
}

void RoundedRect::set_rounding_top_left(const Sizef &offset)
{
	impl->round_tl = offset;
	impl->outline_needs_update = true;
	impl->triangulation_needs_update = true;
}

void RoundedRect::set_rounding_top_right(const Sizef &offset)
{
	impl->round_tr = offset;
	impl->outline_needs_update = true;
	impl->triangulation_needs_update = true;
}

void RoundedRect::set_rounding_bottom_left(const Sizef &offset)
{
	impl->round_bl = offset;
	impl->outline_needs_update = true;
	impl->triangulation_needs_update = true;
}

void RoundedRect::set_rounding_bottom_right(const Sizef &offset)
{
	impl->round_br = offset;
	impl->outline_needs_update = true;
	impl->triangulation_needs_update = true;
}

/////////////////////////////////////////////////////////////////////////////
// RoundedRect Implementation:

}
