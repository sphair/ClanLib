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
#include "block_layout.h"
#include <algorithm>

namespace clan
{
	float BlockLayout::get_preferred_width(View *view)
	{
		if (!view->style.is_width_auto())
			return view->style.width();

		float width = 0.0f;
		for (const std::shared_ptr<View> &subview : view->subviews())
		{
			if (subview->style.is_static() && !subview->hidden())
			{
				float margin_box_width = 0.0f;
				margin_box_width += subview->style.margin_left();
				margin_box_width += subview->style.border_left();
				margin_box_width += subview->style.padding_left();
				margin_box_width += subview->get_preferred_width();
				margin_box_width += subview->style.padding_right();
				margin_box_width += subview->style.border_right();
				margin_box_width += subview->style.margin_right();
				width = clan::max(width, margin_box_width);
			}
		}
		return width;
	}

	float BlockLayout::get_preferred_height(View *view, float width)
	{
		if (!view->style.is_height_auto())
			return view->style.height();

		float height = 0.0f;
		for (const std::shared_ptr<View> &subview : view->subviews())
		{
			if (subview->style.is_static() && !subview->hidden())
			{
				height += subview->style.margin_top();
				height += subview->style.border_top();
				height += subview->style.padding_top();
				height += subview->get_preferred_height(width);
				height += subview->style.padding_bottom();
				height += subview->style.border_bottom();
				height += subview->style.margin_bottom();
			}
		}
		return height;
	}

	float BlockLayout::get_first_baseline_offset(View *view, float width)
	{
		const auto &subviews = view->subviews();
		for (auto it = subviews.begin(); it != subviews.end(); ++it)
		{
			if (!(*it)->hidden())
				return (*it)->get_first_baseline_offset(width);
		}
		return 0.0f;
	}

	float BlockLayout::get_last_baseline_offset(View *view, float width)
	{
		const auto &subviews = view->subviews();
		for (auto it = subviews.rbegin(); it != subviews.rend(); ++it)
		{
			if (!(*it)->hidden())
				return (*it)->get_last_baseline_offset(width);
		}
		return 0.0f;
	}

	void BlockLayout::layout_subviews(View *view)
	{
		float y = 0.0f;
		for (const std::shared_ptr<View> &subview : view->subviews())
		{
			if (subview->style.is_static() && !subview->hidden())
			{
				float left_noncontent = 0.0f;
				left_noncontent += subview->style.margin_left();
				left_noncontent += subview->style.border_left();
				left_noncontent += subview->style.padding_left();

				float right_noncontent = 0.0f;
				right_noncontent += subview->style.margin_right();
				right_noncontent += subview->style.border_right();
				right_noncontent += subview->style.padding_right();

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

				float subview_height = subview->get_preferred_height(subview_width);

				y += subview->style.margin_top();
				y += subview->style.border_top();
				y += subview->style.padding_top();

				subview->set_geometry(ViewGeometry::from_content_box(subview->style, Rectf::xywh(left_noncontent, y, subview_width, subview_height)));

				y += subview_height;
				y += subview->style.padding_bottom();
				y += subview->style.border_bottom();
				y += subview->style.margin_bottom();

				subview->layout_subviews();
			}
		}
	}
}
