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

#include "CSSLayout/Layout/LayoutTree/css_layout_tree_node.h"
#include "inline_generated_box.h"
#include "linebreak_opportunity.h"

namespace clan
{

class CSSInlinePosition;

class CSSInlineLayout : public CSSLayoutTreeNode
{
public:
	CSSInlineLayout(CSSBoxElement *element);
	void add_box(CSSInlineGeneratedBox *box);

	void set_component_geometry();
	int get_first_line_baseline();
	int get_last_line_baseline();

	void prepare_children();
	void calculate_content_top_down_heights();
	bool add_content_margin_top(CSSLayoutCursor &cursor);
	bool is_empty() const;
	void layout_content(CSSLayoutGraphics *graphics, CSSLayoutCursor &cursor, LayoutStrategy strategy);
	void layout_absolute_and_fixed_content(CSSLayoutGraphics *graphics, CSSResourceCache *resources, Rect containing_block, const Size &viewport_size);

	void render_layer_background(CSSLayoutGraphics *graphics, CSSResourceCache *resources, bool root);
	void render_layer_non_inline(CSSLayoutGraphics *graphics, CSSResourceCache *resources);
	void render_layer_floats(CSSLayoutGraphics *graphics, CSSResourceCache *resources);
	void render_layer_inline(CSSLayoutGraphics *graphics, CSSResourceCache *resources);
	void render_layer_positioned(CSSLayoutGraphics *graphics, CSSResourceCache *resources);

	bool find_content_box(CSSBoxElement *element, Rect &out_rect);
	bool get_cursor_box(CSSLayoutGraphics *graphics, CSSResourceCache *resources, CSSBoxText *text_node, std::string::size_type pos, Rect &out_box);
	// CSSLayoutHitTestResult hit_test(CSSLayoutGraphics *graphics, CSSResourceCache *resource_cache, const Point &pos) const;


	static void adjust_start_of_line_text_range(CSSBoxText *text, size_t &text_start, size_t &text_end, bool &start_of_line);

private:
	void layout_inline_blocks_and_floats(CSSLayoutGraphics *graphics, CSSResourceCache *resources, LayoutStrategy strategy);
	void create_linebreak_opportunities();
	CSSActualValue get_width(CSSLayoutGraphics *graphics, CSSResourceCache *resources, CSSInlinePosition start, CSSInlinePosition end, bool &start_of_line);
	bool place_floats(CSSInlinePosition start, CSSInlinePosition end, CSSActualValue x, CSSActualValue y, LayoutStrategy strategy);
	void generate_line(CSSInlinePosition start, CSSInlinePosition end);
	void generate_block_line(CSSInlinePosition pos);
	void layout_line(CSSInlineGeneratedBox *line, Rect &line_box, CSSLayoutGraphics *graphics, CSSResourceCache *resources);
	void align_line(CSSInlineGeneratedBox *line, CSSLayoutGraphics *graphics, CSSResourceCache *resources, bool last_line);
	void layout_block_line(CSSInlineGeneratedBox *line, CSSLayoutGraphics *graphics, CSSLayoutCursor &cursor, LayoutStrategy strategy);
	bool is_empty_line(CSSInlinePosition start, CSSInlinePosition end) const;
	CSSInlinePosition begin() const;
	CSSInlinePosition end() const;

	std::vector<CSSInlineGeneratedBox *> lines;
	std::vector<CSSLayoutTreeNode *> floats;

	std::vector<CSSInlineLineBreakOpportunity> linebreak_opportunities;
	CSSInlineGeneratedBox boxes;

	friend class CSSInlineLayoutLayoutLine;
	friend class CSSInlineLayoutPlaceFloats;
};

}
