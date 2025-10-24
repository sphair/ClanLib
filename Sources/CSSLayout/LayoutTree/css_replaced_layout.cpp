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
#include "css_replaced_layout.h"
#include "css_layout_cursor.h"
#include "css_block_formatting_context.h"
#include "../BoxTree/css_box_object.h"
#include "API/CSSLayout/css_replaced_component.h"

CL_CSSReplacedLayout::CL_CSSReplacedLayout(CL_CSSBoxObject *block_element)
: CL_CSSLayoutTreeNode(block_element), component(0)
{
	component = block_element->get_component();
}

CL_CSSReplacedLayout::~CL_CSSReplacedLayout()
{
}

void CL_CSSReplacedLayout::prepare_children()
{
}

void CL_CSSReplacedLayout::layout_content(CL_GraphicContext &gc, CL_CSSLayoutCursor &cursor, LayoutStrategy strategy)
{
	cursor.apply_margin();
	box = CL_Rect((int)(cursor.x+0.5f), (int)(cursor.y+0.5f), (int)(cursor.x+used.width+0.5f), (int)(cursor.y+used.height+0.5f));
	cursor.y += used.height;
}

void CL_CSSReplacedLayout::render(CL_GraphicContext &gc, CL_CSSResourceCache *resources)
{
	render_non_content(gc, resources);
	if (!component)
	{
		CL_Rect b = box;
		b.translate(formatting_context->get_x(), formatting_context->get_y());
		if (image.is_null())
			image = CL_Image(gc, "../../../Examples/CTalk/carambola-48.png");
		if (!image.is_null())
			image.draw(gc, b);
	}
}

void CL_CSSReplacedLayout::set_component_geometry()
{
	if (component)
	{
		CL_Rect b = box;
		b.translate(formatting_context->get_x(), formatting_context->get_y());
		component->set_geometry(b);
	}
}

int CL_CSSReplacedLayout::get_first_line_baseline()
{
	CL_Rect b = box;
	b.translate(formatting_context->get_x(), formatting_context->get_y());
	if (component)
		return b.bottom-8;
	else
		return b.bottom;
}

int CL_CSSReplacedLayout::get_last_line_baseline()
{
	return get_first_line_baseline();
}
