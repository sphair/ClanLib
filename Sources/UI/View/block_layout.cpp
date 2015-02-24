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
#include "block_layout.h"
#include <algorithm>

namespace clan
{
	float BlockLayout::get_preferred_width(Canvas &canvas, View *view)
	{
		auto style_width = view->style()->computed_value("width");
		if (!style_width.is_keyword("auto"))
			return style_width.number;

		float width = 0.0f;
		for (const std::shared_ptr<View> &subview : view->subviews())
		{
			if (subview->style()->computed_value("position").is_keyword("static") && !subview->hidden())
			{
				float margin_box_width = 0.0f;
				margin_box_width += subview->style()->computed_value("margin-left").number;
				margin_box_width += subview->style()->computed_value("border-left-width").number;
				margin_box_width += subview->style()->computed_value("padding-left").number;
				margin_box_width += subview->get_preferred_width(canvas);
				margin_box_width += subview->style()->computed_value("padding-right").number;
				margin_box_width += subview->style()->computed_value("border-right-width").number;
				margin_box_width += subview->style()->computed_value("margin-right").number;
				width = clan::max(width, margin_box_width);
			}
		}
		return width;
	}

	float BlockLayout::get_preferred_height(Canvas &canvas, View *view, float width)
	{
		auto style_height = view->style()->computed_value("height");
		if (!style_height.is_keyword("auto"))
			return style_height.number;

		float height = 0.0f;
		for (const std::shared_ptr<View> &subview : view->subviews())
		{
			if (subview->style()->computed_value("position").is_keyword("static") && !subview->hidden())
			{
				height += subview->style()->computed_value("margin-top").number;
				height += subview->style()->computed_value("border-top-width").number;
				height += subview->style()->computed_value("padding-top").number;
				height += subview->get_preferred_height(canvas, width);
				height += subview->style()->computed_value("padding-bottom").number;
				height += subview->style()->computed_value("border-bottom-width").number;
				height += subview->style()->computed_value("margin-bottom").number;
			}
		}
		return height;
	}

	float BlockLayout::get_first_baseline_offset(Canvas &canvas, View *view, float width)
	{
		const auto &subviews = view->subviews();
		for (const auto & subview : subviews)
		{
			if (!(subview)->hidden())
				return (subview)->get_first_baseline_offset(canvas, width);
		}
		return 0.0f;
	}

	float BlockLayout::get_last_baseline_offset(Canvas &canvas, View *view, float width)
	{
		const auto &subviews = view->subviews();
		for (auto it = subviews.rbegin(); it != subviews.rend(); ++it)
		{
			if (!(*it)->hidden())
				return (*it)->get_last_baseline_offset(canvas, width);
		}
		return 0.0f;
	}

	void BlockLayout::layout_subviews(Canvas &canvas, View *view)
	{
		float y = 0.0f;
		for (const std::shared_ptr<View> &subview : view->subviews())
		{
			if (subview->style()->computed_value("position").is_keyword("static") && !subview->hidden())
			{
				float left_noncontent = 0.0f;
				left_noncontent += subview->style()->computed_value("margin-left").number;
				left_noncontent += subview->style()->computed_value("border-left-width").number;
				left_noncontent += subview->style()->computed_value("padding-left").number;

				float right_noncontent = 0.0f;
				right_noncontent += subview->style()->computed_value("margin-right").number;
				right_noncontent += subview->style()->computed_value("border-right-width").number;
				right_noncontent += subview->style()->computed_value("padding-right").number;

				float subview_width = view->geometry().content.get_width() - left_noncontent - right_noncontent;
				if (subview_width < 0.0f)
				{
					right_noncontent = 0.0f;
					subview_width = view->geometry().content.get_width() - left_noncontent;
					if (subview_width < 0.0f)
					{
						left_noncontent = 0.0f;
						subview_width = view->geometry().content.get_width();
					}
				}

				float subview_height = subview->get_preferred_height(canvas, subview_width);

				y += subview->style()->computed_value("margin-top").number;
				y += subview->style()->computed_value("border-top-width").number;
				y += subview->style()->computed_value("padding-top").number;

				subview->set_geometry(BoxGeometry::from_content_box(subview->style(), Rectf::xywh(left_noncontent, y, subview_width, subview_height)));

				y += subview_height;
				y += subview->style()->computed_value("padding-bottom").number;
				y += subview->style()->computed_value("border-bottom-width").number;
				y += subview->style()->computed_value("margin-bottom").number;

				subview->layout_subviews(canvas);
			}
		}
	}
}
