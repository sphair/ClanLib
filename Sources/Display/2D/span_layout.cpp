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

#include "Display/precomp.h"
#include "API/Display/2D/span_layout.h"
#include "span_layout_impl.h"

CL_SpanLayout::CL_SpanLayout()
: impl(new CL_SpanLayout_Impl)
{
}

CL_SpanLayout::~CL_SpanLayout()
{
}

void CL_SpanLayout::clear()
{
	impl->clear();
}

void CL_SpanLayout::add_text(const CL_String &text, const CL_Font &font, const CL_Colorf &color, int id)
{
	impl->add_text(text, font, color, id);
}

void CL_SpanLayout::add_image(const CL_Image &image, int baseline_offset, int id)
{
	impl->add_image(image, baseline_offset, id);
}

void CL_SpanLayout::add_component_helper(CL_SpanComponent *component, int baseline_offset, int id)
{
	impl->add_component(component, baseline_offset, id);
}

void CL_SpanLayout::layout(CL_GraphicContext &gc, int max_width)
{
	impl->layout(gc, max_width);
}

CL_SpanLayout::HitTestResult CL_SpanLayout::hit_test(CL_GraphicContext &gc, const CL_Point &pos)
{
	return impl->hit_test(gc, pos);
}

void CL_SpanLayout::draw_layout(CL_GraphicContext &gc)
{
	impl->draw_layout(gc);
}

void CL_SpanLayout::set_position(const CL_Point &pos)
{
	impl->set_position(pos);
}

CL_Size CL_SpanLayout::get_size() const
{
	return impl->get_size();
}

std::vector<CL_Rect> CL_SpanLayout::get_rect_by_id(int id) const
{
	return impl->get_rect_by_id(id);
}

void CL_SpanLayout::set_align(CL_SpanAlign align)
{
	impl->set_align(align);
}

CL_Size CL_SpanLayout::find_preferred_size(CL_GraphicContext &gc)
{
	return impl->find_preferred_size(gc);
}

void CL_SpanLayout::set_selection_range(CL_String::size_type start, CL_String::size_type end)
{
	impl->set_selection_range(start, end);
}

void CL_SpanLayout::set_selection_colors(const CL_Colorf &foreground, const CL_Colorf &background)
{
	impl->set_selection_colors(foreground, background);
}

CL_String CL_SpanLayout::get_combined_text() const
{
	return impl->get_combined_text();
}

void CL_SpanLayout::set_component_geometry()
{
	impl->set_component_geometry();
}
