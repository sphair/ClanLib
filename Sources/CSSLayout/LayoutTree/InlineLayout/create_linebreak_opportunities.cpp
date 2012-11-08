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
#include "create_linebreak_opportunities.h"
#include "CSSLayout/BoxTree/css_box_text.h"

namespace clan
{

CSSInlineLayoutCreateLinebreakOpportunities::CSSInlineLayoutCreateLinebreakOpportunities(std::vector<CSSInlineLineBreakOpportunity> &linebreak_opportunities)
: prev_space(true), linebreak_opportunities(linebreak_opportunities)
{
	linebreak_opportunities.clear();
}

bool CSSInlineLayoutCreateLinebreakOpportunities::node(CSSInlineGeneratedBox *cur)
{
	CSSBoxNode *node = cur->box_node;
	CSSBoxText *text = dynamic_cast<CSSBoxText*>(node);
	if (text)
	{
		for (size_t j = 0; j < text->processed_text.size(); j++)
		{
			if (text->processed_text[j] == ' ')
			{
				prev_space = true;
			}
			else if (text->processed_text[j] == '\n')
			{
				linebreak_opportunities.push_back(CSSInlineLineBreakOpportunity(cur, j+1, true));
				prev_space = false;
			}
			else
			{
				if (prev_space && should_break_at_end_of_spaces(text->get_properties().white_space))
					linebreak_opportunities.push_back(CSSInlineLineBreakOpportunity(cur, j));
				prev_space = false;
			}
		}
	}
	else if (cur->layout_node && cur->layout_node->get_element_node()->is_block_level() && !cur->layout_node->get_element_node()->is_float())
	{
		linebreak_opportunities.push_back(CSSInlineLineBreakOpportunity(cur, 0));
	}
	return true;
}

bool CSSInlineLayoutCreateLinebreakOpportunities::should_break_at_end_of_spaces(const CSSBoxWhiteSpace &whitespace)
{
	return whitespace.type != CSSBoxWhiteSpace::type_pre || whitespace.type == CSSBoxWhiteSpace::type_nowrap;
}

}
