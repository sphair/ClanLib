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

#include "CSSLayout/precomp.h"
#include "API/CSSLayout/Layout/css_layout_text.h"
#include "css_layout_node_impl.h"
#include "css_layout_impl.h"
#include "BoxTree/css_box_text.h"
#include "LayoutTree/InlineLayout/css_inline_layout.h"
#include "LayoutTree/css_layout_graphics.h"

namespace clan
{

CSSLayoutText::CSSLayoutText()
{
}

std::string CSSLayoutText::get_text() const
{
	if (!is_null())
		return static_cast<const CSSBoxText*>(impl->box_node)->get_text();
	else
		return std::string();
}

void CSSLayoutText::set_text(const std::string &text)
{
	if (!is_null())
		static_cast<CSSBoxText*>(impl->box_node)->set_text(text);
}

Rect CSSLayoutText::get_cursor_box(Canvas &canvas, std::string::size_type pos)
{
	if (!is_null())
	{
		std::shared_ptr<CSSLayout_Impl> layout_impl = impl->layout_impl.lock();

		CSSLayoutGraphics graphics(canvas, &layout_impl->resource_cache, layout_impl->viewport, 0);
		return layout_impl->layout_tree.get_cursor_box(&graphics, &layout_impl->resource_cache, static_cast<CSSBoxText*>(impl->box_node), pos);
	}
	else
	{
		return Rect();
	}
}

CSSLayoutText::CSSLayoutText(const std::shared_ptr<CSSLayoutNode_Impl> &impl)
: CSSLayoutNode(impl)
{
}

}
