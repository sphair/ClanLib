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
#include "positioned_layout.h"
#include <algorithm>

namespace clan
{
	void PositionedLayout::layout_subviews(Canvas &canvas, View *view)
	{
		for (const std::shared_ptr<View> &subview : view->subviews())
		{
			if (subview->local_root())
			{
				subview->layout_local();
				continue;
			}
			else if (subview->hidden())
			{
				continue;
			}
			else if (subview->style_cascade().computed_value("position").is_keyword("absolute"))
			{
				// To do: decide how we determine the containing box used for absolute positioning. For now, use the parent content box.
				layout_from_containing_box(canvas, subview.get(), view->geometry().content);
			}
			else if (subview->style_cascade().computed_value("position").is_keyword("fixed"))
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

				layout_from_containing_box(canvas, subview.get(), offset_initial_containing_box);
			}

			layout_subviews(canvas, subview.get());
		}
	}

	ViewGeometry PositionedLayout::get_geometry(Canvas &canvas, View *view, const Rectf &containing_box)
	{
		float x = 0.0f;
		float width = 0.0f;

		if (!view->style_cascade().computed_value("left").is_keyword("auto") && !view->style_cascade().computed_value("right").is_keyword("auto"))
		{
			x = view->style_cascade().computed_value("left").number;
			width = clan::max(containing_box.get_width() - view->style_cascade().computed_value("right").number - x, 0.0f);
		}
		else if (!view->style_cascade().computed_value("left").is_keyword("auto") && !view->style_cascade().computed_value("width").is_keyword("auto"))
		{
			x = view->style_cascade().computed_value("left").number;
			width = view->style_cascade().computed_value("width").number;
		}
		else if (!view->style_cascade().computed_value("right").is_keyword("auto") && !view->style_cascade().computed_value("width").is_keyword("auto"))
		{
			width = view->style_cascade().computed_value("width").number;
			x = containing_box.get_width() - view->style_cascade().computed_value("right").number - width;
		}
		else if (!view->style_cascade().computed_value("left").is_keyword("auto"))
		{
			x = view->style_cascade().computed_value("left").number;
			width = view->get_preferred_width(canvas);
		}
		else if (!view->style_cascade().computed_value("right").is_keyword("auto"))
		{
			width = view->get_preferred_width(canvas);
			x = containing_box.get_width() - view->style_cascade().computed_value("right").number - width;
		}
		else
		{
			x = 0.0f;
			width = view->get_preferred_width(canvas);
		}

		float y = 0.0f;
		float height = 0.0f;

		if (!view->style_cascade().computed_value("top").is_keyword("auto") && !view->style_cascade().computed_value("bottom").is_keyword("auto"))
		{
			y = view->style_cascade().computed_value("top").number;
			height = clan::max(containing_box.get_height() - view->style_cascade().computed_value("bottom").number - y, 0.0f);
		}
		else if (!view->style_cascade().computed_value("top").is_keyword("auto") && !view->style_cascade().computed_value("height").is_keyword("auto"))
		{
			y = view->style_cascade().computed_value("top").number;
			height = view->style_cascade().computed_value("height").number;
		}
		else if (!view->style_cascade().computed_value("bottom").is_keyword("auto") && !view->style_cascade().computed_value("height").is_keyword("auto"))
		{
			height = view->style_cascade().computed_value("height").number;
			y = containing_box.get_height() - view->style_cascade().computed_value("bottom").number - height;
		}
		else if (!view->style_cascade().computed_value("top").is_keyword("auto"))
		{
			y = view->style_cascade().computed_value("top").number;
			height = view->get_preferred_height(canvas, width);
		}
		else if (!view->style_cascade().computed_value("bottom").is_keyword("auto"))
		{
			height = view->get_preferred_height(canvas, width);
			y = containing_box.get_height() - view->style_cascade().computed_value("bottom").number - height;
		}
		else
		{
			y = 0.0f;
			height = view->get_preferred_height(canvas, width);
		}

		return ViewGeometry::from_content_box(view->style_cascade(), Rectf::xywh(x, y, width, height));
	}

	void PositionedLayout::layout_from_containing_box(Canvas &canvas, View *view, const Rectf &containing_box)
	{
		view->set_geometry(get_geometry(canvas, view, containing_box));
		view->layout_subviews(canvas);
	}
}
