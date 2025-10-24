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

#include "css_layout_tree_node.h"

class CL_CSSBoxNode;
class CL_CSSBoxElement;
class CL_CSSBoxText;
class CL_CSSInlineObject;
class CL_CSSInlineLineBreakOpportunity;
class CL_CSSInlineLineBox;
class CL_CSSInlineLineBoxCursor;
class CL_CSSBlockFormattingContext;
class CL_CSSResourceCache;
class CL_CSSLayoutCursor;
class CL_CSSBlockLayout;

class CL_CSSInlineLayout : public CL_CSSLayoutTreeNode
{
public:
	CL_CSSInlineLayout(CL_CSSBoxElement *block_element);
	~CL_CSSInlineLayout();

	void push_back(CL_CSSBoxNode *box_node, CL_CSSLayoutTreeNode *layout_node = 0);
	bool is_empty() const;

	void calculate_top_down_sizes();
	void render(CL_GraphicContext &gc, CL_CSSResourceCache *resources);
	void set_component_geometry();
	int get_first_line_baseline();
	int get_last_line_baseline();
	bool find_content_box(CL_CSSBoxElement *element, CL_Rect &out_rect);

	bool contains_node(const CL_CSSBoxNode *node) const;
	CL_Rect get_cursor_box(CL_GraphicContext &gc, CL_CSSResourceCache *resources, CL_CSSBoxText *text_node, CL_String::size_type pos) const;
	CL_CSSLayoutHitTestResult hit_test(const CL_Point &pos) const;

	const std::vector<CL_CSSInlineObject> &get_objects() const { return objects; }
	const std::vector<CL_CSSInlineLineBox> &get_line_boxes() const { return line_boxes; }

private:
	bool margin_collapses();
	void prepare_children();
	void layout_content(CL_GraphicContext &gc, CL_CSSLayoutCursor &cursor, LayoutStrategy strategy);
	void create_line_boxes(CL_GraphicContext &gc, CL_CSSLayoutCursor &cursor, LayoutStrategy strategy);
	void layout_inline_blocks(CL_GraphicContext &gc, CL_CSSLayoutCursor &layout_cursor, LayoutStrategy strategy);
	void place_line_box(CL_CSSInlineLineBox &line, CL_CSSLayoutCursor &layout_cursor, int y);
	void apply_text_indent(CL_CSSLayoutCursor &layout_cursor, CL_CSSInlineLineBox &line);
	void create_linebreak_opportunities();
	CL_CSSInlineLineBoxCursor find_next_linebreak_opportunity(CL_CSSInlineLineBoxCursor cursor, bool &out_is_newline);
	int find_width(CL_GraphicContext &gc, const CL_CSSInlineLineBoxCursor &start, const CL_CSSInlineLineBoxCursor &end, bool start_of_line);
	int find_object_width(CL_GraphicContext &gc, CL_CSSResourceCache *resources, CL_CSSInlineObject &object, size_t text_start, size_t text_end, bool &start_of_line);
	int find_block_width(CL_GraphicContext &gc, CL_CSSResourceCache *resources, CL_CSSInlineObject &object, bool &start_of_line);
	int find_text_width(CL_GraphicContext &gc, CL_CSSResourceCache *resources, CL_CSSBoxText *text, size_t text_start, size_t text_end, bool &start_of_line);
	bool create_line_segments(CL_GraphicContext &gc, CL_CSSLayoutCursor &layout_cursor, const CL_CSSInlineLineBoxCursor &start, const CL_CSSInlineLineBoxCursor &end, CL_CSSInlineLineBox &line, bool &start_of_line);
	void create_object_segment(CL_GraphicContext &gc, CL_CSSLayoutCursor &layout_cursor, size_t object_index, size_t text_start, size_t text_end, CL_CSSInlineLineBox &line, bool &start_of_line);
	void create_block_segment(CL_GraphicContext &gc, CL_CSSLayoutCursor &layout_cursor, size_t object_index, CL_CSSInlineLineBox &line, bool &start_of_line);
	void create_text_segment(CL_GraphicContext &gc, CL_CSSLayoutCursor &layout_cursor, size_t object_index, size_t text_start, size_t text_end, CL_CSSInlineLineBox &line, bool &start_of_line);
	void update_line_box_height(CL_GraphicContext &gc, CL_CSSResourceCache *resources, CL_CSSInlineLineBox &line);
	void apply_line_box_alignment(CL_CSSInlineLineBox &line);
	int find_baseline_offset(CL_GraphicContext &gc, CL_CSSResourceCache *resources, const CL_CSSBoxElement *element);
	bool stop_at_block_level(CL_CSSInlineLineBoxCursor &cursor, CL_CSSInlineLineBoxCursor &next_linebreak);

	std::vector<CL_CSSInlineObject> objects;
	std::vector<CL_CSSInlineLineBreakOpportunity> linebreak_opportunities;
	std::vector<CL_CSSInlineLineBox> line_boxes;
};
