/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

#include "CSSLayout/precomp.h"
#include "API/CSSLayout/css_layout_text.h"
#include "css_layout_node_impl.h"
#include "css_layout_impl.h"
#include "BoxTree/css_box_text.h"
#include "LayoutTree/css_inline_layout.h"
#include "LayoutTree/css_inline_object.h"
#include "LayoutTree/css_inline_line_box.h"
#include "LayoutTree/css_inline_line_segment.h"

CL_CSSLayoutText::CL_CSSLayoutText()
{
}

CL_String CL_CSSLayoutText::get_text() const
{
	if (!is_null())
		return static_cast<const CL_CSSBoxText*>(impl->box_node)->get_text();
	else
		return CL_String();
}

void CL_CSSLayoutText::set_text(const CL_String &text)
{
	if (!is_null())
		static_cast<CL_CSSBoxText*>(impl->box_node)->set_text(text);
}

std::vector<CL_CSSLayoutText::LineSpan> CL_CSSLayoutText::get_line_spans()
{
	std::vector<LineSpan> line_spans;
	if (!is_null())
	{
		CL_CSSInlineLayout *inline_layout = impl->layout_impl->layout_tree.find_inline_layout(static_cast<CL_CSSBoxText*>(impl->box_node));
		if (inline_layout)
		{
			const std::vector<CL_CSSInlineObject> &objects = inline_layout->get_objects();
			const std::vector<CL_CSSInlineLineBox> &line_boxes = inline_layout->get_line_boxes();
			for (size_t i = 0; i < line_boxes.size(); i++)
			{
				LineSpan line_span;
				bool found = false;
				for (size_t j = 0; j < line_boxes[i].segments.size(); j++)
				{
					if (objects[line_boxes[i].segments[j].object_index].node == impl->box_node)
					{
						line_span.text_end = line_boxes[i].segments[j].text_end;
						line_span.box.right = line_boxes[i].segments[j].right;
						if (!found)
						{
							line_span.text_start = line_boxes[i].segments[j].text_start;
							line_span.box.left = line_boxes[i].segments[j].left;
							found = true;
						}
					}
				}
				if (found)
				{
					line_span.box.top = line_boxes[i].box.top;
					line_span.box.bottom = line_boxes[i].box.bottom;
					line_span.box.left += line_boxes[i].box.left;
					line_span.box.right += line_boxes[i].box.left;
					line_spans.push_back(line_span);
				}
			}
		}
	}
	return line_spans;
}

CL_Rect CL_CSSLayoutText::get_cursor_box(CL_GraphicContext &gc, CL_String::size_type pos)
{
	if (!is_null())
	{
		return impl->layout_impl->layout_tree.get_cursor_box(gc, &impl->layout_impl->resource_cache, static_cast<CL_CSSBoxText*>(impl->box_node), pos);
	}
	else
	{
		return CL_Rect();
	}
}

CL_CSSLayoutText::CL_CSSLayoutText(const CL_SharedPtr<CL_CSSLayoutNode_Impl> &impl)
: CL_CSSLayoutNode(impl)
{
}
