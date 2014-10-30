/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
	float InlineLayout::get_preferred_width(View *view)
	{
		if (!view->box_style.is_width_auto())
			return view->box_style.width();

		float width = 0.0f;
		for (const std::shared_ptr<View> &view : view->subviews())
		{
			if (view->box_style.is_static() && !view->hidden())
			{
				width += view->box_style.margin_left();
				width += view->box_style.border_left();
				width += view->box_style.padding_left();
				width += view->get_preferred_width();
				width += view->box_style.padding_right();
				width += view->box_style.border_right();
				width += view->box_style.margin_right();
			}
		}
		return width;
	}

	float InlineLayout::get_preferred_height(View *view, float width)
	{
		if (!view->box_style.is_height_auto())
			return view->box_style.height();

		float x = 0.0f;
		float y = 0.0f;
		float line_height = 0.0f;

		for (const std::shared_ptr<View> &subview : view->subviews())
		{
			if (subview->box_style.is_static() && !subview->hidden())
			{
				float subview_width = subview->get_preferred_width();
				float subview_height = subview->get_preferred_height(subview_width);

				float margin_box_width = 0.0f;
				margin_box_width += subview->box_style.margin_left();
				margin_box_width += subview->box_style.border_left();
				margin_box_width += subview->box_style.padding_left();
				margin_box_width += subview_width;
				margin_box_width += subview->box_style.padding_right();
				margin_box_width += subview->box_style.border_right();
				margin_box_width += subview->box_style.margin_right();

				float margin_box_height = 0.0f;
				margin_box_height += subview->box_style.margin_top();
				margin_box_height += subview->box_style.border_top();
				margin_box_height += subview->box_style.padding_top();
				margin_box_height += subview_height;
				margin_box_height += subview->box_style.padding_bottom();
				margin_box_height += subview->box_style.border_bottom();
				margin_box_height += subview->box_style.margin_bottom();

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

	float InlineLayout::get_first_baseline_offset(View *view, float width)
	{
		float offset = 0.0f;
		for (const std::shared_ptr<View> &subview : view->subviews())
		{
			if (!subview->hidden())
				offset = clan::min(offset, subview->get_first_baseline_offset(width));
		}
		return offset;
	}

	float InlineLayout::get_last_baseline_offset(View *view, float width)
	{
		float offset = 0.0f;
		for (const std::shared_ptr<View> &subview : view->subviews())
		{
			if (!subview->hidden())
				offset = clan::min(offset, subview->get_last_baseline_offset(width));
		}
		return offset;
	}

	void InlineLayout::layout_subviews(View *view)
	{
		float x = 0.0f;
		float y = 0.0f;
		float line_height = 0.0f;

		for (const std::shared_ptr<View> &subview : view->subviews())
		{
			if (subview->box_style.is_static() && !subview->hidden())
			{
				float subview_width = subview->get_preferred_width();
				float subview_height = subview->get_preferred_height(subview_width);

				float margin_box_width = 0.0f;
				margin_box_width += subview->box_style.margin_left();
				margin_box_width += subview->box_style.border_left();
				margin_box_width += subview->box_style.padding_left();
				margin_box_width += subview_width;
				margin_box_width += subview->box_style.padding_right();
				margin_box_width += subview->box_style.border_right();
				margin_box_width += subview->box_style.margin_right();

				float margin_box_height = 0.0f;
				margin_box_height += subview->box_style.margin_top();
				margin_box_height += subview->box_style.border_top();
				margin_box_height += subview->box_style.padding_top();
				margin_box_height += subview_height;
				margin_box_height += subview->box_style.padding_bottom();
				margin_box_height += subview->box_style.border_bottom();
				margin_box_height += subview->box_style.margin_bottom();

				if (x != 0.0f && x + margin_box_width > view->geometry().content.get_width())
				{
					x = 0.0f;
					y += line_height;
					line_height = 0.0f;
				}

				x += subview->box_style.margin_left();
				x += subview->box_style.border_left();
				x += subview->box_style.padding_left();

				float content_y = y;
				content_y += subview->box_style.margin_top();
				content_y += subview->box_style.border_top();
				content_y += subview->box_style.padding_top();

				subview->set_geometry(ViewGeometry::from_content_box(subview->box_style, Rectf::xywh(x, content_y, subview_width, subview_height)));

				x += subview_width;
				x += subview->box_style.padding_right();
				x += subview->box_style.border_right();
				x += subview->box_style.margin_right();

				line_height = clan::max(line_height, margin_box_height);

				subview->layout_subviews();
			}
		}
	}
}
