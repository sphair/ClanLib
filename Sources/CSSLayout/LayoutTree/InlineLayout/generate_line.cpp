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
#include "generate_line.h"

namespace clan
{

CSSInlineLayoutGenerateLine::CSSInlineLayoutGenerateLine(CSSInlineGeneratedBox *start_box, CSSUsedValue relative_x, CSSUsedValue relative_y)
{
	line.reset(new CSSInlineGeneratedBox());
	parent = begin_tree(start_box);
	line->relative_x = relative_x;
	line->relative_y = relative_y;
	start_of_line = true;
}

bool CSSInlineLayoutGenerateLine::node(CSSInlineGeneratedBox *box, size_t text_start, size_t text_end)
{
	CSSInlineGeneratedBox *output = 0;
	CSSBoxText *text = dynamic_cast<CSSBoxText*>(box->box_node);
	if (text)
	{
		CSSInlineLayout::adjust_start_of_line_text_range(text, text_start, text_end, start_of_line);

		output = new CSSInlineGeneratedBox();
		output->box_node = box->box_node;
		output->layout_node = box->layout_node;
		output->relative_x = box->relative_x;
		output->relative_y = box->relative_y;
		output->text_start = text_start;
		output->text_end = text_end;
		output->closing = false;
		parent->add_box(output);
	}
	else if (box->layout_node == 0)
	{
		output = new CSSInlineGeneratedBox();
		output->box_node = box->box_node;
		output->relative_x = box->relative_x;
		output->relative_y = box->relative_y;
		output->closing = false;
		parent->add_box(output);
	}
	else if (!box->layout_node->get_element_node()->is_float())
	{
		start_of_line = false;

		output = new CSSInlineGeneratedBox();
		output->box_node = box->box_node;
		output->layout_node = box->layout_node;
		output->relative_x = box->relative_x;
		output->relative_y = box->relative_y;
		parent->add_box(output);
	}
	parent = output;
	return true;
}

void CSSInlineLayoutGenerateLine::close_node(CSSInlineGeneratedBox *box)
{
	if (parent)
	{
		parent->closing = true;
		parent = parent->parent;
	}
}

CSSInlineGeneratedBox *CSSInlineLayoutGenerateLine::begin_tree(CSSInlineGeneratedBox *start_box)
{
	CSSInlineGeneratedBox *input = start_box;
	CSSInlineGeneratedBox *output = 0;
	CSSInlineGeneratedBox *cur = 0;
	try
	{
		while (input->parent->parent)
		{
			input = input->parent;
			CSSInlineGeneratedBox *parent = new CSSInlineGeneratedBox();
			parent->opening = false;
			parent->box_node = input->box_node;
			parent->layout_node = input->layout_node;
			parent->relative_x = input->relative_x;
			parent->relative_y = input->relative_y;
			if (cur)
				parent->add_box(cur);
			else
				output = parent;
			cur = parent;
		}
		if (cur)
			line->add_box(cur);
		if (output == 0)
			output = line.get();
		return output;
	}
	catch (...)
	{
		delete cur;
		throw;
	}
}

}
