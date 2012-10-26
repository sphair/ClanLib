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

#pragma once

#include "css_used_values.h"
#include "css_layout_hit_test_result.h"

namespace clan
{

class CSSBoxText;
class CSSBoxElement;
class CSSBlockFormattingContext;
class CSSStackingContext;
class CSSResourceCache;
class CSSLayoutCursor;
class CSSLayoutHitTestResult;
class CSSBoxMarginWidth;
class CSSBoxPaddingWidth;
class CSSLayoutGraphics;

class CSSLayoutTreeNode
{
public:
	CSSLayoutTreeNode(CSSBoxElement *element_node);
	virtual ~CSSLayoutTreeNode();

	enum LayoutStrategy
	{
		normal_strategy,
		preferred_strategy,
		minimum_strategy
	};

	void prepare(CSSBlockFormattingContext *current_formatting_context, CSSStackingContext *current_stacking_context);
	virtual void set_component_geometry() = 0;
	virtual int get_first_line_baseline() = 0;
	virtual int get_last_line_baseline() = 0;
	virtual bool find_content_box(CSSBoxElement *element, Rect &out_rect) { return false; }
	virtual bool get_cursor_box(CSSLayoutGraphics *graphics, CSSResourceCache *resources, CSSBoxText *text_node, std::string::size_type pos, Rect &out_box) { return false; }

	virtual void render_layer_background(CSSLayoutGraphics *graphics, CSSResourceCache *resources, bool root) = 0;
	virtual void render_layer_non_inline(CSSLayoutGraphics *graphics, CSSResourceCache *resources) = 0;
	virtual void render_layer_floats(CSSLayoutGraphics *graphics, CSSResourceCache *resources) = 0;
	virtual void render_layer_inline(CSSLayoutGraphics *graphics, CSSResourceCache *resources) = 0;
	virtual void render_layer_positioned(CSSLayoutGraphics *graphics, CSSResourceCache *resources) = 0;

	void layout_float(CSSLayoutGraphics *graphics, CSSResourceCache *resources, LayoutStrategy strategy = normal_strategy);
	void set_root_block_position(int x, int y);
	void set_root_content_position(int x, int y);
	virtual void layout_absolute_and_fixed_content(CSSLayoutGraphics *graphics, CSSResourceCache *resources, Rect containing_block, const Size &viewport_size) { }
	void layout_normal(CSSLayoutGraphics *graphics, CSSLayoutCursor &cursor, LayoutStrategy strategy);

	void calc_preferred(CSSLayoutGraphics *graphics, CSSResourceCache *resources);
	void calc_minimum(CSSLayoutGraphics *graphics, CSSResourceCache *resources);
	void layout_formatting_root_helper(CSSLayoutGraphics *graphics, CSSResourceCache *resources, LayoutStrategy strategy);
	void layout_absolute_or_fixed(CSSLayoutGraphics *graphics, CSSResourceCache *resources, const Rect &containing_block, const Size &viewport_size);

	virtual void calculate_top_down_widths(LayoutStrategy strategy);
	virtual void calculate_top_down_heights();
	virtual void calculate_content_top_down_heights() { }
	bool add_margin_top(CSSLayoutCursor &cursor);
	CSSUsedValue get_local_relative_x() const;
	CSSUsedValue get_local_relative_y() const;
	static CSSUsedValue get_local_relative_x(const CSSBoxElement *element, CSSUsedValue containing_width);
	static CSSUsedValue get_local_relative_y(const CSSBoxElement *element, CSSUsedValue containing_height);
	virtual bool is_empty() const { return false; }
	CSSActualValue get_block_width() const;
	CSSActualValue get_block_height() const;
	virtual bool is_replaced() const { return false; }
	virtual CSSLayoutHitTestResult hit_test(CSSLayoutGraphics *graphics, CSSResourceCache *resource_cache, const Point &pos) const { return CSSLayoutHitTestResult(); }

	CSSBoxElement *get_element_node() { return element_node; }
	CSSStackingContext *get_stacking_context() { return stacking_context; }

	struct LTRB
	{
		LTRB() : left(0.0f), right(0.0f), top(0.0f), bottom(0.0f) { }
		CSSUsedValue left, right, top, bottom;
	};

	LTRB margin;
	LTRB border;
	LTRB padding;
	CSSUsedWidth width;
	CSSUsedHeight height;
	CSSUsedValue css_min_width;
	CSSUsedValue css_min_height;
	CSSUsedWidth css_max_width;
	CSSUsedHeight css_max_height;
	CSSUsedWidth containing_width;
	CSSUsedHeight containing_height;
	CSSUsedValue preferred_width;
	CSSUsedValue min_width;
	bool preferred_width_calculated;
	bool min_width_calculated;
	CSSUsedValue relative_x, relative_y;

	Rect content_box;
	LTRB static_position;
	CSSLayoutTreeNode *static_position_parent;

	bool is_formatting_context_root() const { return formatting_context_root; }
	CSSBlockFormattingContext *get_formatting_context() { return formatting_context; }

	static CSSUsedValue get_css_margin_width(const CSSBoxMarginWidth &margin_width, CSSUsedWidth containing_width);
	static CSSUsedValue get_css_margin_height(const CSSBoxMarginWidth &margin_width, CSSUsedHeight containing_height);
	static CSSUsedValue get_css_padding_width(const CSSBoxPaddingWidth &padding_width, CSSUsedWidth containing_width);
	static CSSUsedValue get_css_padding_height(const CSSBoxPaddingWidth &padding_width, CSSUsedHeight containing_height);

protected:
	virtual void prepare_children() = 0;
	virtual void layout_content(CSSLayoutGraphics *graphics, CSSLayoutCursor &cursor, LayoutStrategy strategy) = 0;
	virtual bool add_content_margin_top(CSSLayoutCursor &cursor) { return false; }

	void render_non_content(CSSLayoutGraphics *graphics, CSSResourceCache *resource_cache, bool root);
	void render_border(CSSLayoutGraphics *graphics, CSSResourceCache *resource_cache);
	void render_background(CSSLayoutGraphics *graphics, CSSResourceCache *resource_cache, bool root);
	Rect get_border_box() const;
	Rect get_padding_box() const;

	CSSBoxElement *element_node;
	CSSBlockFormattingContext *formatting_context;
	bool formatting_context_root;
	CSSStackingContext *stacking_context;
	bool stacking_context_root;

private:
	void set_formatting_context(CSSBlockFormattingContext *formatting_context, bool is_root);
	void establish_stacking_context_if_needed(CSSStackingContext *current_stacking_context);

	void layout_shrink_to_fit(CSSLayoutGraphics *graphics, CSSResourceCache *resources, CSSUsedValue available_width);

	void calculate_absolute_widths(LayoutStrategy strategy);
	void calculate_absolute_heights();
	void apply_absolute_widths_constraint(CSSUsedValue constraint_width, CSSUsedValue left, CSSUsedValue right);
	void apply_absolute_heights_constraint(CSSUsedValue constraint_height, CSSUsedValue top, CSSUsedValue bottom);
	void calculate_static_widths(LayoutStrategy strategy);
	void calculate_static_heights();
};

}
