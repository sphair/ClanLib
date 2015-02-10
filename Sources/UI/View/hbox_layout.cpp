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
#include "hbox_layout.h"
#include <algorithm>
#include <cmath>

namespace clan
{
	float HBoxLayout::get_preferred_width(Canvas &canvas, View *view)
	{
		if (!view->style()->computed_value("width").is_keyword("auto"))
			return view->style()->computed_value("width").number;

		float width = 0.0f;
		for (const std::shared_ptr<View> &subview : view->subviews())
		{
			if (subview->style()->computed_value("position").is_keyword("static") && !subview->hidden())
			{
				width += subview->style()->computed_value("margin-left").number;
				width += subview->style()->computed_value("border-left-width").number;
				width += subview->style()->computed_value("padding-left").number;
				if (subview->style()->computed_value("flex-basis").is_keyword("auto"))
					width += subview->get_preferred_width(canvas);
				else
					width += subview->style()->computed_value("flex-basis").number;
				width += subview->style()->computed_value("padding-right").number;
				width += subview->style()->computed_value("border-right-width").number;
				width += subview->style()->computed_value("margin-right").number;
			}
		}
		return width;
	}

	float HBoxLayout::get_preferred_height(Canvas &canvas, View *view, float width)
	{
		if (!view->style()->computed_value("height").is_keyword("auto"))
			return view->style()->computed_value("height").number;

		// Calculate flex properties:

		float total_grow_factor = 0.0f;
		float total_shrink_factor = 0.0f;
		float basis_width = 0.0f;
		float noncontent_width = 0.0f;
		for (const std::shared_ptr<View> &subview : view->subviews())
		{
			if (subview->style()->computed_value("position").is_keyword("static") && !subview->hidden())
			{
				noncontent_width += subview->style()->computed_value("margin-left").number;
				noncontent_width += subview->style()->computed_value("border-left-width").number;
				noncontent_width += subview->style()->computed_value("padding-left").number;
				noncontent_width += subview->style()->computed_value("padding-right").number;
				noncontent_width += subview->style()->computed_value("border-right-width").number;
				noncontent_width += subview->style()->computed_value("margin-right").number;

				total_grow_factor += subview->style()->computed_value("flex-grow").number;
				total_shrink_factor += subview->style()->computed_value("flex-shrink").number;

				if (subview->style()->computed_value("flex-basis").is_keyword("auto"))
					basis_width += subview->get_preferred_width(canvas);
				else
					basis_width += subview->style()->computed_value("flex-basis").number;
			}
		}

		float free_space = width - noncontent_width - basis_width;

		// Layout using flex properties:

		float height = 0.0f;
		for (const std::shared_ptr<View> &subview : view->subviews())
		{
			if (subview->style()->computed_value("position").is_keyword("static") && !subview->hidden())
			{
				float subview_width = subview->style()->computed_value("flex-basis").number;
				if (subview->style()->computed_value("flex-basis").is_keyword("auto"))
					subview_width = subview->get_preferred_width(canvas);

				if (free_space < 0.0f && total_shrink_factor != 0.0f)
					subview_width += subview->style()->computed_value("flex-shrink").number * free_space / total_shrink_factor;
				else if (free_space > 0.0f && total_grow_factor != 0.0f)
					subview_width += subview->style()->computed_value("flex-grow").number * free_space / total_grow_factor;

				subview_width = std::round(subview_width); // To do: this way of rounding may cause the total width to go beyond the available content width

				float margin_box_height = 0.0f;
				margin_box_height += subview->style()->computed_value("margin-top").number;
				margin_box_height += subview->style()->computed_value("border-top-width").number;
				margin_box_height += subview->style()->computed_value("padding-top").number;
				margin_box_height += subview->get_preferred_height(canvas, subview_width);
				margin_box_height += subview->style()->computed_value("padding-bottom").number;
				margin_box_height += subview->style()->computed_value("border-bottom-width").number;
				margin_box_height += subview->style()->computed_value("margin-bottom").number;
				height = clan::max(height, margin_box_height);
			}
		}
		return height;
	}

	float HBoxLayout::get_first_baseline_offset(Canvas &canvas, View *view, float width)
	{
		const auto &subviews = view->subviews();
		for (const auto & subview : subviews)
		{
			if (!(subview)->hidden())
				return (subview)->get_first_baseline_offset(canvas, width);
		}
		return 0.0f;
	}

	float HBoxLayout::get_last_baseline_offset(Canvas &canvas, View *view, float width)
	{
		const auto &subviews = view->subviews();
		for (auto it = subviews.rbegin(); it != subviews.rend(); ++it)
		{
			if (!(*it)->hidden())
				return (*it)->get_last_baseline_offset(canvas, width);
		}
		return 0.0f;
	}

	void HBoxLayout::layout_subviews(Canvas &canvas, View *view)
	{
		// Calculate flex properties:

		float total_grow_factor = 0.0f;
		float total_shrink_factor = 0.0f;
		float basis_width = 0.0f;
		float noncontent_width = 0.0f;
		for (const std::shared_ptr<View> &subview : view->subviews())
		{
			if (subview->style()->computed_value("position").is_keyword("static") && !subview->hidden())
			{
				noncontent_width += subview->style()->computed_value("margin-left").number;
				noncontent_width += subview->style()->computed_value("border-left-width").number;
				noncontent_width += subview->style()->computed_value("padding-left").number;
				noncontent_width += subview->style()->computed_value("padding-right").number;
				noncontent_width += subview->style()->computed_value("border-right-width").number;
				noncontent_width += subview->style()->computed_value("margin-right").number;

				total_grow_factor += subview->style()->computed_value("flex-grow").number;
				total_shrink_factor += subview->style()->computed_value("flex-shrink").number;

				if (subview->style()->computed_value("flex-basis").is_keyword("auto"))
					basis_width += subview->get_preferred_width(canvas);
				else
					basis_width += subview->style()->computed_value("flex-basis").number;
			}
		}

		float free_space = view->geometry().content.get_width() - noncontent_width - basis_width;

		// Layout using flex properties:

		float x = 0.0f;
		for (const std::shared_ptr<View> &subview : view->subviews())
		{
			if (subview->style()->computed_value("position").is_keyword("static") && !subview->hidden())
			{
				float subview_width = subview->style()->computed_value("flex-basis").number;
				if (subview->style()->computed_value("flex-basis").is_keyword("auto"))
					subview_width = subview->get_preferred_width(canvas);

				if (free_space < 0.0f && total_shrink_factor != 0.0f)
					subview_width += subview->style()->computed_value("flex-shrink").number * free_space / total_shrink_factor;
				else if (free_space > 0.0f && total_grow_factor != 0.0f)
					subview_width += subview->style()->computed_value("flex-grow").number * free_space / total_grow_factor;

				subview_width = std::round(subview_width); // To do: this way of rounding may cause the total width to go beyond the available content width

				float top_noncontent = 0.0f;
				top_noncontent += subview->style()->computed_value("margin-top").number;
				top_noncontent += subview->style()->computed_value("border-top-width").number;
				top_noncontent += subview->style()->computed_value("padding-top").number;

				float bottom_noncontent = 0.0f;
				bottom_noncontent += subview->style()->computed_value("margin-bottom").number;
				bottom_noncontent += subview->style()->computed_value("border-bottom-width").number;
				bottom_noncontent += subview->style()->computed_value("padding-bottom").number;

				float subview_height = subview->get_preferred_height(canvas, subview_width);
				float available_margin = view->geometry().content.get_height() - subview_height - top_noncontent - bottom_noncontent;

				if (subview->style()->computed_value("margin-top").is_keyword("auto") && subview->style()->computed_value("margin-bottom").is_keyword("auto"))
				{
					top_noncontent += available_margin * 0.5f;
					bottom_noncontent += available_margin * 0.5f;
				}
				else if (subview->style()->computed_value("margin-top").is_keyword("auto"))
				{
					top_noncontent += available_margin;
				}
				else if (subview->style()->computed_value("margin-bottom").is_keyword("auto"))
				{
					top_noncontent -= available_margin;
				}
				else
				{
					subview_height = view->geometry().content.get_height() - top_noncontent - bottom_noncontent;
					if (subview_height < 0.0f)
					{
						bottom_noncontent = 0.0f;
						subview_height = view->geometry().content.get_height() - top_noncontent;
						if (subview_height < 0.0f)
						{
							top_noncontent = 0.0f;
							subview_height = view->geometry().content.get_height();
						}
					}
				}

				x += subview->style()->computed_value("margin-left").number;
				x += subview->style()->computed_value("border-left-width").number;
				x += subview->style()->computed_value("padding-left").number;

				subview->set_geometry(BoxGeometry::from_content_box(subview->box_style, Rectf::xywh(x, top_noncontent, subview_width, subview_height)));

				x += subview_width;
				x += subview->style()->computed_value("padding-right").number;
				x += subview->style()->computed_value("border-right-width").number;
				x += subview->style()->computed_value("margin-right").number;

				subview->layout_subviews(canvas);
			}
		}
	}
}
