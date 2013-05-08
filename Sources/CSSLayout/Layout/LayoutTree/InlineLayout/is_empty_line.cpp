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
#include "is_empty_line.h"
#include "CSSLayout/Layout/BoxTree/css_box_text.h"

namespace clan
{

CSSInlineLayoutIsEmptyLine::CSSInlineLayoutIsEmptyLine()
: start_of_line(true)
{
}

bool CSSInlineLayoutIsEmptyLine::node(CSSInlineGeneratedBox *cur, size_t text_start, size_t text_end)
{
	CSSBoxText *text = dynamic_cast<CSSBoxText *>(cur->box_node);
	CSSBoxElement *element = dynamic_cast<CSSBoxElement *>(cur->box_node);
	if (text)
	{
		CSSInlineLayout::adjust_start_of_line_text_range(text, text_start, text_end, start_of_line);
		if (text->processed_text.substr(text_start, text_end - text_start).find_first_not_of(' ') != std::string::npos)
			return false;
	}
	else if (cur->layout_node && !cur->layout_node->get_element_node()->is_float())
	{
		if (!cur->layout_node->is_empty())
			return false;
	}
	return true;
}

}
