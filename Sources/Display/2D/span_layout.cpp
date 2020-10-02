/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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

namespace clan
{
	SpanLayout::SpanLayout()
		: impl(std::make_shared<SpanLayout_Impl>())
	{
	}

	SpanLayout::~SpanLayout()
	{
	}

	void SpanLayout::clear()
	{
		impl->clear();
	}

	template<typename T>
	void SpanLayout::add_component(T *component, int baseline_offset, int id)
	{
		add_component_helper(new SpanComponentBinder<T>(component), baseline_offset, id);
	}

	void SpanLayout::add_text(const std::string &text, const Font &font, const Colorf &color, int id)
	{
		impl->add_text(text, font, color, id);
	}

	void SpanLayout::add_image(const Image &image, int baseline_offset, int id)
	{
		impl->add_image(image, baseline_offset, id);
	}

	void SpanLayout::add_component_helper(SpanComponent *component, int baseline_offset, int id)
	{
		impl->add_component(component, baseline_offset, id);
	}

	void SpanLayout::layout(Canvas &canvas, int max_width)
	{
		impl->layout(canvas, max_width);
	}

	SpanLayout::HitTestResult SpanLayout::hit_test(Canvas &canvas, const Point &pos)
	{
		return impl->hit_test(canvas, pos);
	}

	void SpanLayout::draw_layout(Canvas &canvas)
	{
		impl->draw_layout(canvas);
	}

	void SpanLayout::draw_layout_ellipsis(Canvas &canvas, const Rect &content_rect)
	{
		impl->draw_layout_ellipsis(canvas, content_rect);
	}

	void SpanLayout::set_position(const Point &pos)
	{
		impl->set_position(pos);
	}

	Size SpanLayout::get_size() const
	{
		return impl->get_rect().get_size();
	}

	Rect SpanLayout::get_rect() const
	{
		return impl->get_rect();
	}

	std::vector<Rect> SpanLayout::get_rect_by_id(int id) const
	{
		return impl->get_rect_by_id(id);
	}

	void SpanLayout::set_align(SpanAlign align)
	{
		impl->set_align(align);
	}

	Size SpanLayout::find_preferred_size(Canvas &canvas)
	{
		return impl->find_preferred_size(canvas);
	}

	void SpanLayout::set_selection_range(std::string::size_type start, std::string::size_type end)
	{
		impl->set_selection_range(start, end);
	}

	void SpanLayout::set_selection_colors(const Colorf &foreground, const Colorf &background)
	{
		impl->set_selection_colors(foreground, background);
	}

	std::string SpanLayout::get_combined_text() const
	{
		return impl->get_combined_text();
	}

	void SpanLayout::set_component_geometry()
	{
		impl->set_component_geometry();
	}

	void SpanLayout::show_cursor()
	{
		impl->cursor_visible = true;
	}

	void SpanLayout::hide_cursor()
	{
		impl->cursor_visible = false;
	}

	void SpanLayout::set_cursor_pos(std::string::size_type pos)
	{
		impl->cursor_pos = pos;
	}

	void SpanLayout::set_cursor_overwrite_mode(bool enable)
	{
		impl->cursor_overwrite_mode = enable;
	}

	void SpanLayout::set_cursor_color(const Colorf &color)
	{
		impl->cursor_color = color;
	}

	int SpanLayout::get_first_baseline_offset()
	{
		return impl->get_first_baseline_offset();
	}

	int SpanLayout::get_last_baseline_offset()
	{
		return impl->get_last_baseline_offset();
	}
}
