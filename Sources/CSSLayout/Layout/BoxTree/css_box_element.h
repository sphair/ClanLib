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

#pragma once

#include "css_box_node.h"
#include "API/CSSLayout/ComputedValues/css_computed_values.h"
#include "API/CSSLayout/ComputedValues/css_computed_box.h"

namespace clan
{

class CSSBoxElement : public CSSBoxNode
{
public:
	CSSBoxElement();

	std::string name;
	CSSComputedValues computed_values;
	int col_span;
	int row_span;

	bool has_block_level_children() const;
	bool is_block_level() const;
	bool is_inline_block_level() const;
	bool is_shrink_to_fit() const;
	bool is_float() const;
	bool is_table() const;
	bool is_inline_table() const;
	bool is_table_row() const;
	bool is_table_cell() const;
	bool is_table_row_group() const;
	bool is_table_header_group() const;
	bool is_table_footer_group() const;
	bool is_table_column() const;
	bool is_table_column_group() const;
	bool is_table_caption() const;
	bool is_absolute() const;
	bool is_fixed() const;
	bool is_display_none() const;
	bool is_overflow_visible() const;
};

}
