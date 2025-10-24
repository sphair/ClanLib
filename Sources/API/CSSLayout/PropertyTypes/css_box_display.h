/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

#include "../css_box_property.h"

class CL_CSSBoxFloat;
class CL_CSSBoxPosition;

class CL_CSSBoxDisplay : public CL_CSSBoxProperty
{
public:
	CL_CSSBoxDisplay();
	void compute(const CL_CSSBoxDisplay *parent, CL_CSSResourceCache *layout, float em_size, float ex_size, const CL_CSSBoxPosition &position, CL_CSSBoxFloat &float_box);
	CL_String to_string() const;

	enum Type
	{
		type_inline,
		type_block,
		type_list_item,
		type_run_in,
		type_inline_block,
		type_table,
		type_inline_table,
		type_table_row_group,
		type_table_header_group,
		type_table_footer_group,
		type_table_row,
		type_table_column_group,
		type_table_column,
		type_table_cell,
		type_table_caption,
		type_none,
		type_inherit
	};
	Type type;

private:
	void apply_table_9_7();
};
