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
#include "positioned_layout.h"
#include <algorithm>

namespace clan
{
	void PositionedLayout::layout_subviews(View *view)
	{
		for (const std::shared_ptr<View> &subview : view->subviews())
		{
			if (subview->hidden()) continue;

			if (subview->box_style.is_absolute())
			{
				// To do: decide how we determine the containing box used for absolute positioning. For now, use the parent content box.
				layout_from_containing_box(subview.get(), view->geometry().content);
			}
			else if (subview->box_style.is_fixed())
			{
				Rectf offset_initial_containing_box;
				View *current = view->superview();
				if (current)
				{
					Pointf offset = view->geometry().content.get_top_left();
					while (true)
					{
						offset = offset + view->geometry().content.get_top_left();
						View *superview = current->superview();
						if (!superview)
						{
							offset_initial_containing_box = current->geometry().content_box();
							offset_initial_containing_box.set_top_left(offset_initial_containing_box.get_top_left() - offset);
							break;
						}
					}
				}
				else
				{
					offset_initial_containing_box = view->geometry().content;
				}

				layout_from_containing_box(subview.get(), offset_initial_containing_box);
			}
		}
	}

	void PositionedLayout::layout_from_containing_box(View *view, const Rectf &containing_box)
	{
		float x = 0.0f;
		float width = 0.0f;

		if (!view->box_style.is_left_auto() && !view->box_style.is_right_auto())
		{
			x = view->box_style.left();
			width = clan::max(containing_box.get_width() - view->box_style.right() - x, 0.0f);
		}
		else if (!view->box_style.is_left_auto() && !view->box_style.is_width_auto())
		{
			x = view->box_style.left();
			width = view->box_style.width();
		}
		else if (!view->box_style.is_right_auto() && !view->box_style.is_width_auto())
		{
			width = view->box_style.width();
			x = containing_box.get_width() - view->box_style.right() - width;
		}
		else if (!view->box_style.is_left_auto())
		{
			x = view->box_style.left();
			width = view->get_preferred_width();
		}
		else if (!view->box_style.is_right_auto())
		{
			width = view->get_preferred_width();
			x = containing_box.get_width() - view->box_style.right() - width;
		}
		else
		{
			x = 0.0f;
			width = view->get_preferred_width();
		}

		float y = 0.0f;
		float height = 0.0f;

		if (!view->box_style.is_top_auto() && !view->box_style.is_bottom_auto())
		{
			y = view->box_style.top();
			height = clan::max(containing_box.get_height() - view->box_style.bottom() - y, 0.0f);
		}
		else if (!view->box_style.is_top_auto() && !view->box_style.is_height_auto())
		{
			y = view->box_style.top();
			height = view->box_style.height();
		}
		else if (!view->box_style.is_bottom_auto() && !view->box_style.is_height_auto())
		{
			height = view->box_style.height();
			y = containing_box.get_height() - view->box_style.bottom() - height;
		}
		else if (!view->box_style.is_top_auto())
		{
			y = view->box_style.top();
			height = view->get_preferred_height(width);
		}
		else if (!view->box_style.is_bottom_auto())
		{
			height = view->get_preferred_height(width);
			y = containing_box.get_height() - view->box_style.bottom() - height;
		}
		else
		{
			y = 0.0f;
			height = view->get_preferred_height(width);
		}

		view->set_geometry(ViewGeometry::from_content_box(view->box_style, Rectf::xywh(x, y, width, height)));

		view->layout_subviews();
	}
}
