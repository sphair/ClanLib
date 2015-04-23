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
#include "vbox_layout.h"
#include <algorithm>
#include <cmath>

namespace clan
{
	float VBoxLayout::get_preferred_width(Canvas &canvas, View *view)
	{
		if (!view->style_cascade().computed_value("width").is_keyword("auto"))
			return view->style_cascade().computed_value("width").number;

		float width = 0.0f;
		for (const std::shared_ptr<View> &subview : view->subviews())
		{
			if (subview->is_static_position_and_visible())
			{
				float margin_box_width = 0.0f;
				margin_box_width += subview->style_cascade().computed_value("margin-left").number;
				margin_box_width += subview->style_cascade().computed_value("border-left-width").number;
				margin_box_width += subview->style_cascade().computed_value("padding-left").number;
				if (subview->style_cascade().computed_value("flex-basis").is_keyword("main-size"))
					margin_box_width += subview->get_preferred_width(canvas);
				else
					margin_box_width += subview->style_cascade().computed_value("flex-basis").number;
				margin_box_width += subview->style_cascade().computed_value("padding-right").number;
				margin_box_width += subview->style_cascade().computed_value("border-right-width").number;
				margin_box_width += subview->style_cascade().computed_value("margin-right").number;
				width = clan::max(width, margin_box_width);
			}
		}
		return width;
	}

	float VBoxLayout::get_preferred_height(Canvas &canvas, View *view, float width)
	{
		if (!view->style_cascade().computed_value("height").is_keyword("auto"))
			return view->style_cascade().computed_value("height").number;

		float height = 0.0f;
		for (const std::shared_ptr<View> &subview : view->subviews())
		{
			if (subview->is_static_position_and_visible())
			{
				float left_noncontent = 0.0f;
				left_noncontent += subview->style_cascade().computed_value("margin-left").number;
				left_noncontent += subview->style_cascade().computed_value("border-left-width").number;
				left_noncontent += subview->style_cascade().computed_value("padding-left").number;

				float right_noncontent = 0.0f;
				right_noncontent += subview->style_cascade().computed_value("margin-right").number;
				right_noncontent += subview->style_cascade().computed_value("border-right-width").number;
				right_noncontent += subview->style_cascade().computed_value("padding-right").number;

				float subview_width = width - left_noncontent - right_noncontent;
				if (subview_width < 0.0f)
				{
					right_noncontent = 0.0f;
					subview_width = width - left_noncontent;
					if (subview_width < 0.0f)
					{
						left_noncontent = 0.0f;
						subview_width = width;
					}
				}

				height += subview->style_cascade().computed_value("margin-top").number;
				height += subview->style_cascade().computed_value("border-top-width").number;
				height += subview->style_cascade().computed_value("padding-top").number;
				height += subview->get_preferred_height(canvas, subview_width);
				height += subview->style_cascade().computed_value("padding-bottom").number;
				height += subview->style_cascade().computed_value("border-bottom-width").number;
				height += subview->style_cascade().computed_value("margin-bottom").number;
			}
		}
		return height;
	}

	float VBoxLayout::get_first_baseline_offset(Canvas &canvas, View *view, float width)
	{
		const auto &subviews = view->subviews();
		for (const auto & subview : subviews)
		{
			if (subview->is_static_position_and_visible())
				return (subview)->get_first_baseline_offset(canvas, width);
		}
		return 0.0f;
	}

	float VBoxLayout::get_last_baseline_offset(Canvas &canvas, View *view, float width)
	{
		const auto &subviews = view->subviews();
		for (auto it = subviews.rbegin(); it != subviews.rend(); ++it)
		{
			if ((*it)->is_static_position_and_visible())
				return (*it)->get_last_baseline_offset(canvas, width);
		}
		return 0.0f;
	}

	void VBoxLayout::layout_subviews(Canvas &canvas, View *view)
	{
		// Calculate flex properties:

		float total_grow_factor = 0.0f;
		float total_shrink_factor = 0.0f;
		float basis_height = 0.0f;
		float noncontent_height = 0.0f;
		for (const std::shared_ptr<View> &subview : view->subviews())
		{
			if (subview->is_static_position_and_visible())
			{
				noncontent_height += subview->style_cascade().computed_value("margin-top").number;
				noncontent_height += subview->style_cascade().computed_value("border-top-width").number;
				noncontent_height += subview->style_cascade().computed_value("padding-top").number;
				noncontent_height += subview->style_cascade().computed_value("padding-bottom").number;
				noncontent_height += subview->style_cascade().computed_value("border-bottom-width").number;
				noncontent_height += subview->style_cascade().computed_value("margin-bottom").number;

				total_grow_factor += subview->style_cascade().computed_value("flex-grow").number;
				total_shrink_factor += subview->style_cascade().computed_value("flex-shrink").number;

				if (subview->style_cascade().computed_value("flex-basis").is_keyword("main-size"))
				{
					float left_noncontent = 0.0f;
					left_noncontent += subview->style_cascade().computed_value("margin-left").number;
					left_noncontent += subview->style_cascade().computed_value("border-left-width").number;
					left_noncontent += subview->style_cascade().computed_value("padding-left").number;

					float right_noncontent = 0.0f;
					right_noncontent += subview->style_cascade().computed_value("margin-right").number;
					right_noncontent += subview->style_cascade().computed_value("border-right-width").number;
					right_noncontent += subview->style_cascade().computed_value("padding-right").number;

					float subview_width = view->geometry().content_width - left_noncontent - right_noncontent;
					if (subview_width < 0.0f)
					{
						right_noncontent = 0.0f;
						subview_width = view->geometry().content_height - left_noncontent;
						if (subview_width < 0.0f)
						{
							left_noncontent = 0.0f;
							subview_width = view->geometry().content_width;
						}
					}

					basis_height += subview->get_preferred_height(canvas, subview_width);
				}
				else
				{
					basis_height += subview->style_cascade().computed_value("flex-basis").number;
				}
			}
		}

		float free_space = view->geometry().content_height - noncontent_height - basis_height;

		// Layout using flex properties:

		float y = 0.0f;
		for (const std::shared_ptr<View> &subview : view->subviews())
		{
			if (subview->is_static_position_and_visible())
			{
				float left_noncontent = 0.0f;
				left_noncontent += subview->style_cascade().computed_value("margin-left").number;
				left_noncontent += subview->style_cascade().computed_value("border-left-width").number;
				left_noncontent += subview->style_cascade().computed_value("padding-left").number;

				float right_noncontent = 0.0f;
				right_noncontent += subview->style_cascade().computed_value("margin-right").number;
				right_noncontent += subview->style_cascade().computed_value("border-right-width").number;
				right_noncontent += subview->style_cascade().computed_value("padding-right").number;

				float subview_width = view->geometry().content_width - left_noncontent - right_noncontent;
				if (subview_width < 0.0f)
				{
					right_noncontent = 0.0f;
					subview_width = view->geometry().content_width - left_noncontent;
					if (subview_width < 0.0f)
					{
						left_noncontent = 0.0f;
						subview_width = view->geometry().content_width;
					}
				}

				float subview_height = subview->style_cascade().computed_value("flex-basis").number;
				if (subview->style_cascade().computed_value("flex-basis").is_keyword("main-size"))
					subview_height = subview->get_preferred_height(canvas, subview_width);

				if (free_space < 0.0f && total_shrink_factor != 0.0f)
					subview_height += free_space * subview->style_cascade().computed_value("flex-shrink").number / total_shrink_factor;
				else if (free_space > 0.0f && total_grow_factor != 0.0f)
					subview_height += free_space * subview->style_cascade().computed_value("flex-grow").number / total_grow_factor;

				subview_height = std::round(subview_height); // To do: this way of rounding may cause the total height to go beyond the available content height

				y += subview->style_cascade().computed_value("margin-top").number;
				y += subview->style_cascade().computed_value("border-top-width").number;
				y += subview->style_cascade().computed_value("padding-top").number;

				subview->set_geometry(ViewGeometry::from_content_box(subview->style_cascade(), Rectf::xywh(left_noncontent, y, subview_width, subview_height)));

				y += subview_height;
				y += subview->style_cascade().computed_value("padding-bottom").number;
				y += subview->style_cascade().computed_value("border-bottom-width").number;
				y += subview->style_cascade().computed_value("margin-bottom").number;

				subview->layout_subviews(canvas);
			}
		}
	}
}
