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

#pragma once

#include "css_box_node.h"

class CL_CSSBoxElement;
class CL_CSSBoxProperties;

class CL_CSSBoxText : public CL_CSSBoxNode
{
public:
	CL_CSSBoxText() : selection_start(0), selection_end(0), processed_selection_start(0), processed_selection_end(0) { }

	const CL_String &get_text() const { return text; }
	void set_text(const CL_String &text);

	CL_String text;
	CL_String::size_type selection_start, selection_end;
	CL_String processed_text;
	CL_String::size_type processed_selection_start, processed_selection_end;

	const CL_CSSBoxElement *get_parent_element() const;
	const CL_CSSBoxProperties &get_properties() const;
};
