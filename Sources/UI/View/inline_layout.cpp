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
#include "inline_layout.h"
#include <algorithm>

namespace clan
{
	float InlineLayout::get_preferred_width(Canvas &canvas, View *view)
	{
		if (!view->style()->computed_value("width").is_keyword("auto"))
			return view->style()->computed_value("width").number;

		float width = 0.0f;
		for (const std::shared_ptr<View> &view : view->subviews())
		{
			if (view->style()->computed_value("position").is_keyword("static") && !view->hidden())
			{
				width += view->style()->computed_value("margin-left").number;
				width += view->style()->computed_value("border-left-width").number;
				width += view->style()->computed_value("padding-left").number;
				width += view->get_preferred_width(canvas);
				width += view->style()->computed_value("padding-right").number;
				width += view->style()->computed_value("border-right-width").number;
				width += view->style()->computed_value("margin-right").number;
			}
		}
		return width;
	}

	float InlineLayout::get_preferred_height(Canvas &canvas, View *view, float width)
	{
		if (!view->style()->computed_value("height").is_keyword("auto"))
			return view->style()->computed_value("height").number;

		float x = 0.0f;
		float y = 0.0f;
		float line_height = 0.0f;

		for (const std::shared_ptr<View> &subview : view->subviews())
		{
			if (subview->style()->computed_value("position").is_keyword("static") && !subview->hidden())
			{
				float subview_width = subview->get_preferred_width(canvas);
				float subview_height = subview->get_preferred_height(canvas, subview_width);

				float margin_box_width = 0.0f;
				margin_box_width += subview->style()->computed_value("margin-left").number;
				margin_box_width += subview->style()->computed_value("border-left-width").number;
				margin_box_width += subview->style()->computed_value("padding-left").number;
				margin_box_width += subview_width;
				margin_box_width += subview->style()->computed_value("padding-right").number;
				margin_box_width += subview->style()->computed_value("border-right-width").number;
				margin_box_width += subview->style()->computed_value("margin-right").number;

				float margin_box_height = 0.0f;
				margin_box_height += subview->style()->computed_value("margin-top").number;
				margin_box_height += subview->style()->computed_value("border-top-width").number;
				margin_box_height += subview->style()->computed_value("padding-top").number;
				margin_box_height += subview_height;
				margin_box_height += subview->style()->computed_value("padding-bottom").number;
				margin_box_height += subview->style()->computed_value("border-bottom-width").number;
				margin_box_height += subview->style()->computed_value("margin-bottom").number;

				if (x != 0.0f && x + margin_box_width > width)
				{
					x = 0.0f;
					y += line_height;
					line_height = 0.0f;
				}

				x += margin_box_width;
				line_height = clan::max(line_height, margin_box_height);
			}
		}

		return y + line_height;
	}

	float InlineLayout::get_first_baseline_offset(Canvas &canvas, View *view, float width)
	{
		float offset = 0.0f;
		for (const std::shared_ptr<View> &subview : view->subviews())
		{
			if (!subview->hidden())
				offset = clan::min(offset, subview->get_first_baseline_offset(canvas, width));
		}
		return offset;
	}

	float InlineLayout::get_last_baseline_offset(Canvas &canvas, View *view, float width)
	{
		float offset = 0.0f;
		for (const std::shared_ptr<View> &subview : view->subviews())
		{
			if (!subview->hidden())
				offset = clan::min(offset, subview->get_last_baseline_offset(canvas, width));
		}
		return offset;
	}

	void InlineLayout::layout_subviews(Canvas &canvas, View *view)
	{
		float x = 0.0f;
		float y = 0.0f;
		float line_height = 0.0f;

		for (const std::shared_ptr<View> &subview : view->subviews())
		{
			if (subview->style()->computed_value("position").is_keyword("static") && !subview->hidden())
			{
				float subview_width = subview->get_preferred_width(canvas);
				float subview_height = subview->get_preferred_height(canvas, subview_width);

				float margin_box_width = 0.0f;
				margin_box_width += subview->style()->computed_value("margin-left").number;
				margin_box_width += subview->style()->computed_value("border-left-width").number;
				margin_box_width += subview->style()->computed_value("padding-left").number;
				margin_box_width += subview_width;
				margin_box_width += subview->style()->computed_value("padding-right").number;
				margin_box_width += subview->style()->computed_value("border-right-width").number;
				margin_box_width += subview->style()->computed_value("margin-right").number;

				float margin_box_height = 0.0f;
				margin_box_height += subview->style()->computed_value("margin-top").number;
				margin_box_height += subview->style()->computed_value("border-top-width").number;
				margin_box_height += subview->style()->computed_value("padding-top").number;
				margin_box_height += subview_height;
				margin_box_height += subview->style()->computed_value("padding-bottom").number;
				margin_box_height += subview->style()->computed_value("border-bottom-width").number;
				margin_box_height += subview->style()->computed_value("margin-bottom").number;

				if (x != 0.0f && x + margin_box_width > view->geometry().content.get_width())
				{
					x = 0.0f;
					y += line_height;
					line_height = 0.0f;
				}

				x += subview->style()->computed_value("margin-left").number;
				x += subview->style()->computed_value("border-left-width").number;
				x += subview->style()->computed_value("padding-left").number;

				float content_y = y;
				content_y += subview->style()->computed_value("margin-top").number;
				content_y += subview->style()->computed_value("border-top-width").number;
				content_y += subview->style()->computed_value("padding-top").number;

				subview->set_geometry(BoxGeometry::from_content_box(subview->style(), Rectf::xywh(x, content_y, subview_width, subview_height)));

				x += subview_width;
				x += subview->style()->computed_value("padding-right").number;
				x += subview->style()->computed_value("border-right-width").number;
				x += subview->style()->computed_value("margin-right").number;

				line_height = clan::max(line_height, margin_box_height);

				subview->layout_subviews(canvas);
			}
		}
	}
}
