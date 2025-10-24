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
#include "css_whitespace_eraser.h"
#include "css_box_element.h"
#include "css_box_text.h"
#include "css_box_properties.h"

void CL_CSSWhitespaceEraser::remove_whitespace(CL_CSSBoxElement *root_element)
{
	CL_CSSBoxNode *cur = root_element;
	while (cur)
	{
		CL_CSSBoxText *child_text = dynamic_cast<CL_CSSBoxText*>(cur);
		if (child_text)
		{
			const CL_CSSBoxProperties &text_properties = child_text->get_properties();
			if (text_properties.white_space.type != CL_CSSBoxWhiteSpace::type_pre &&
				text_properties.white_space.type != CL_CSSBoxWhiteSpace::type_pre_wrap)
			{
				size_t pos = 0;
				while (true)
				{
					pos = child_text->processed_text.find('\n', pos);
					if (pos == CL_String::npos)
						break;
					if (text_properties.white_space.type != CL_CSSBoxWhiteSpace::type_pre_line)
					{
						// linefeed characters are transformed for rendering purpose into one of the
						// following characters: a space character, a zero width space character (U+200B),
						// or no character (i.e., not rendered), according to UA-specific algorithms based
						// on the content script. 
						child_text->processed_text[pos] = ' ';
					}

					pos = erase_space_before(child_text, pos);
					erase_space_after(child_text, pos+1);
					pos++;
				}
				pos = 0;
				while (true)
				{
					pos = child_text->processed_text.find('\t', pos);
					if (pos == CL_String::npos)
						break;
					child_text->processed_text[pos] = ' ';
				}
				pos = 0;
				while (true)
				{
					pos = child_text->processed_text.find(' ', pos);
					if (pos == CL_String::npos)
						break;
					size_t pos2 = child_text->processed_text.find_first_not_of(' ', pos+1);
					if (pos2 == CL_String::npos)
						pos2 = child_text->processed_text.length();

					pos = erase_space_before(child_text, pos2-1);
					pos++;
				}
			}
		}

		cur = get_next(cur);
	}
}

void CL_CSSWhitespaceEraser::erase(CL_CSSBoxText *cur_text, size_t pos, size_t len)
{
	len = cl_min(len, cur_text->processed_text.length());
	cur_text->processed_text.erase(pos, len);
	// to do: adjust processed_selection_start and processed_selection_end if needed
}

size_t CL_CSSWhitespaceEraser::erase_space_before(CL_CSSBoxText *text, size_t pos)
{
	CL_CSSBoxNode *cur = text;
	bool first_pass = true;
	while (cur)
	{
		CL_CSSBoxText *cur_text = dynamic_cast<CL_CSSBoxText*>(cur);
		if (cur_text)
		{
			const CL_CSSBoxProperties &text_properties = cur_text->get_properties();
			if (text_properties.white_space.type != CL_CSSBoxWhiteSpace::type_pre &&
				text_properties.white_space.type != CL_CSSBoxWhiteSpace::type_pre_wrap)
			{
				if (pos > cur_text->processed_text.size())
					pos = cur_text->processed_text.size();
				for (size_t i = pos; i > 0; i--)
				{
					if (cur_text->processed_text[i-1] != ' ' &&
						cur_text->processed_text[i-1] != '\t' &&
						cur_text->processed_text[i-1] != '\r')
					{
						erase(cur_text, i, pos-i);
						if (first_pass)
							return i;
						else
							return 0;
					}
				}
				erase(cur_text, 0, pos);
				pos = CL_String::npos;
			}
		}
		cur = get_prev(cur);
		first_pass = false;
	}
	return 0;
}

void CL_CSSWhitespaceEraser::erase_space_after(CL_CSSBoxText *text, size_t pos)
{
	CL_CSSBoxNode *cur = text;
	while (cur)
	{
		CL_CSSBoxText *cur_text = dynamic_cast<CL_CSSBoxText*>(cur);
		if (cur_text)
		{
			const CL_CSSBoxProperties &text_properties = cur_text->get_properties();
			if (text_properties.white_space.type != CL_CSSBoxWhiteSpace::type_pre &&
				text_properties.white_space.type != CL_CSSBoxWhiteSpace::type_pre_wrap)
			{
				for (size_t i = pos; i < cur_text->processed_text.size(); i++)
				{
					if (cur_text->processed_text[i] != ' ' &&
						cur_text->processed_text[i] != '\t' &&
						cur_text->processed_text[i] != '\r')
					{
						erase(cur_text, pos, i-pos);
						return;
					}
				}
				erase(cur_text, pos);
				pos = 0;
			}
		}
		cur = get_next(cur);
	}
}

CL_CSSBoxNode *CL_CSSWhitespaceEraser::get_prev(CL_CSSBoxNode *cur)
{
	// to do: avoid CSSBoxDisplay::type_none
	CL_CSSBoxNode *prev = cur->get_prev_sibling();
	if (prev)
	{
		while (true)
		{
			CL_CSSBoxNode *child = prev->get_last_child();
			if (child == 0)
				break;
			prev = child;
		}
	}
	else
	{
		prev = cur->get_parent();
	}
	cur = prev;
	return cur;
}

CL_CSSBoxNode *CL_CSSWhitespaceEraser::get_next(CL_CSSBoxNode *cur)
{
	// to do: avoid CSSBoxDisplay::type_none
	CL_CSSBoxNode *next = 0;
	next = cur->get_first_child();
	if (!next)
		next = cur->get_next_sibling();
	while (!next && cur)
	{
		cur = cur->get_parent();
		if (cur)
			next = cur->get_next_sibling();
	}
	cur = next;
	return next;
}
