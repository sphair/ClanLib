/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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

#include "UI/precomp.h"
#include "API/UI/StandardViews/span_layout_view.h"
#include "span_layout_view_impl.h"

namespace clan
{
	SpanLayoutView::SpanLayoutView() : impl(new SpanLayoutViewImpl())
	{
		impl->view = this;
		style()->set("layout: none");
	}

	const std::shared_ptr<Style> &SpanLayoutView::text_style(const std::string &text_class) const
	{
		const auto it = impl->text_classes.find(text_class);
		if (it != impl->text_classes.end())
			return it->second;

		auto &style = impl->text_classes[text_class];
		style = std::make_shared<Style>();
		return style;
	}

	void SpanLayoutView::set_text_alignment(TextAlignment alignment)
	{
		impl->set_text_alignment(alignment);
		set_needs_layout();
		set_needs_render();
	}

	void SpanLayoutView::clear()
	{
		impl->clear();

		std::vector<std::shared_ptr<View>> subviews_copy = subviews();
		for (auto &view : subviews_copy)
			view->remove_from_super();
	}

	void SpanLayoutView::add_text(const std::string &text, const std::string &text_class)
	{
		impl->add_text(text, text_style(text_class));
		set_needs_layout();
		set_needs_render();
	}

	void SpanLayoutView::add_subview(const std::shared_ptr<View> &view, float baseline_offset)
	{
		View::add_subview(view);
		impl->set_last_baseline_offset(baseline_offset);
	}

	void SpanLayoutView::subview_added(const std::shared_ptr<View> &view)
	{
		impl->add_subview(view);
	}

	void SpanLayoutView::subview_removed(const std::shared_ptr<View> &view)
	{
		impl->remove_subview(view);
	}

	void SpanLayoutView::render_content(Canvas &canvas)
	{
		return impl->render_content(canvas, geometry().content_width);
	}

	float SpanLayoutView::calculate_preferred_width(Canvas &canvas)
	{
		if (style_cascade().computed_value("width").is_keyword("auto"))
			return impl->get_preferred_width(canvas);
		else
			return style_cascade().computed_value("width").number();
	}

	float SpanLayoutView::calculate_preferred_height(Canvas &canvas, float width)
	{
		if (style_cascade().computed_value("height").is_keyword("auto"))
			return impl->get_preferred_height(canvas, width);
		else
			return style_cascade().computed_value("height").number();
	}

	float SpanLayoutView::calculate_first_baseline_offset(Canvas &canvas, float width)
	{
		return impl->get_first_baseline_offset(canvas, width);
	}

	float SpanLayoutView::calculate_last_baseline_offset(Canvas &canvas, float width)
	{
		return impl->get_last_baseline_offset(canvas, width);
	}

	void SpanLayoutView::layout_subviews(Canvas &canvas)
	{
		View::layout_subviews(canvas);
		impl->layout_views(canvas, geometry().content_width);
	}
}
