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

#include "css_layout_tree_node.h"
#include "../BoxTree/css_box_element.h"

class CL_CSSBoxWhiteSpace;
class CL_CSSBoxText;

class CL_CSSInlineGeneratedBox
{
public:
	CL_CSSInlineGeneratedBox();
	~CL_CSSInlineGeneratedBox();

	void add_box(CL_CSSInlineGeneratedBox *child);
	bool is_block_line() const { return first_child == 0; }

	bool opening, closing;
	size_t text_start, text_end;
	CL_CSSBoxNode *box_node;
	CL_CSSLayoutTreeNode *layout_node;
	bool floated;
	CL_CSSActualValue x, width;
	CL_CSSActualValue y, height, ascent, descent, baseline_offset;
	CL_CSSUsedValue relative_x, relative_y;
	CL_CSSInlineGeneratedBox *parent;
	CL_CSSInlineGeneratedBox *first_child;
	CL_CSSInlineGeneratedBox *last_child;
	CL_CSSInlineGeneratedBox *next_sibling;
};

class CL_CSSInlinePosition
{
public:
	CL_CSSInlinePosition() : box(0), text_pos(0) { }
	CL_CSSInlinePosition(CL_CSSInlineGeneratedBox *box, size_t text_pos) : box(box), text_pos(text_pos) { }

	bool is_block_level() const
	{
		return box && box->layout_node && box->layout_node->get_element_node()->is_block_level();
	}

	bool is_float() const
	{
		return box && box->layout_node && box->layout_node->get_element_node()->is_float();
	}

	bool operator==(const CL_CSSInlinePosition &other) const { return box == other.box && text_pos == other.text_pos; }
	bool operator!=(const CL_CSSInlinePosition &other) const { return box != other.box || text_pos != other.text_pos; }

	CL_CSSInlineGeneratedBox *box;
	size_t text_pos;
};

class CL_CSSInlineLineBreakOpportunity2
{
public:
	CL_CSSInlineLineBreakOpportunity2(CL_CSSInlineGeneratedBox *box, size_t text_offset, bool newline = false)
	: pos(box, text_offset), newline(newline)
	{
	}

	CL_CSSInlinePosition pos;
	bool newline;
};

class CL_CSSInlineLayout : public CL_CSSLayoutTreeNode
{
public:
	CL_CSSInlineLayout(CL_CSSBoxElement *element);
	void add_box(CL_CSSInlineGeneratedBox *box);

	void set_component_geometry();
	int get_first_line_baseline();
	int get_last_line_baseline();

	void prepare_children();
	void calculate_content_top_down_heights();
	bool add_content_margin_top(CL_CSSLayoutCursor &cursor);
	bool is_empty() const;
	void layout_content(CL_CSSLayoutGraphics *graphics, CL_CSSLayoutCursor &cursor, LayoutStrategy strategy);
	void layout_absolute_and_fixed_content(CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resources, CL_Rect containing_block, const CL_Size &viewport_size);

	void render_layer_background(CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resources, bool root);
	void render_layer_non_inline(CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resources);
	void render_layer_floats(CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resources);
	void render_layer_inline(CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resources);
	void render_layer_positioned(CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resources);

	bool find_content_box(CL_CSSBoxElement *element, CL_Rect &out_rect);
	bool get_cursor_box(CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resources, CL_CSSBoxText *text_node, CL_String::size_type pos, CL_Rect &out_box);
	// CL_CSSLayoutHitTestResult hit_test(CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resource_cache, const CL_Point &pos) const;

private:
	void layout_inline_blocks_and_floats(CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resources, LayoutStrategy strategy);
	void create_linebreak_opportunities();
	bool should_break_at_end_of_spaces(const CL_CSSBoxWhiteSpace &whitespace);
	CL_CSSActualValue get_width(CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resources, CL_CSSInlinePosition start, CL_CSSInlinePosition end, bool &start_of_line);
	bool place_floats(CL_CSSInlinePosition start, CL_CSSInlinePosition end, CL_CSSActualValue x, CL_CSSActualValue y, LayoutStrategy strategy);
	void adjust_start_of_line_text_range(CL_CSSBoxText *text, size_t &text_start, size_t &text_end, bool &start_of_line) const;
	void generate_line(CL_CSSInlinePosition start, CL_CSSInlinePosition end);
	CL_CSSInlineGeneratedBox *begin_tree(CL_CSSInlinePosition start, CL_CSSInlineGeneratedBox *line);
	void generate_block_line(CL_CSSInlinePosition pos);
	void layout_line(CL_CSSInlineGeneratedBox *line, CL_Rect &line_box, CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resources);
	void align_line(CL_CSSInlineGeneratedBox *line, CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resources, bool last_line);
	int find_word_count(CL_CSSInlineGeneratedBox *line);
	void split_text(CL_CSSInlineGeneratedBox *box, size_t text_pos, CL_CSSLayoutGraphics *graphics, CL_CSSResourceCache *resources);
	void expand_box(CL_CSSInlineGeneratedBox *box, CL_CSSActualValue extra);
	void layout_block_line(CL_CSSInlineGeneratedBox *line, CL_CSSLayoutGraphics *graphics, CL_CSSLayoutCursor &cursor, LayoutStrategy strategy);
	bool is_empty_line(CL_CSSInlinePosition start, CL_CSSInlinePosition end) const;
	CL_CSSInlinePosition begin() const;
	CL_CSSInlinePosition end() const;

	std::vector<CL_CSSInlineGeneratedBox *> lines;
	std::vector<CL_CSSLayoutTreeNode *> floats;

	std::vector<CL_CSSInlineLineBreakOpportunity2> linebreak_opportunities;
	CL_CSSInlineGeneratedBox boxes;
};
