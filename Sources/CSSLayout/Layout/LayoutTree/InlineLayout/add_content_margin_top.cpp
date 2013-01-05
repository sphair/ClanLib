/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
#include "add_content_margin_top.h"
#include "CSSLayout/BoxTree/css_box_text.h"

namespace clan
{

CSSInlineLayoutAddContentMarginTop::CSSInlineLayoutAddContentMarginTop(CSSLayoutCursor &cursor)
: result(false), cursor(cursor), start_of_line(true)
{
}

bool CSSInlineLayoutAddContentMarginTop::node(CSSInlineGeneratedBox *cur)
{
	CSSBoxNode *node = cur->box_node;
	CSSBoxText *text = dynamic_cast<CSSBoxText*>(node);
	if (text)
	{
		size_t text_start = 0;
		size_t text_end = text->processed_text.length();
		CSSInlineLayout::adjust_start_of_line_text_range(text, text_start, text_end, start_of_line);

		if (text->processed_text.substr(text_start, text_end - text_start).find_first_not_of(' ') != std::string::npos)
		{
			result = true;
			return false;
		}
	}
	else if (cur->layout_node)
	{
		if (cur->layout_node->add_margin_top(cursor))
		{
			result = true;
			return false;
		}
	}
	return true;
}

}
