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

#include "API/Display/Font/font.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/2D/span_layout.h"
#include "API/Display/2D/image.h"

class CL_SpanLayout_Impl
{
public:
	CL_SpanLayout_Impl();
	~CL_SpanLayout_Impl();

	void clear();

	void add_text(const CL_String &text, const CL_Font &font, const CL_Colorf &color, int id = -1);
	void add_image(const CL_Image &image, int baseline_offset, int id = -1);
	void add_component(CL_SpanComponent *component, int baseline_offset = 0, int id = -1);

	void layout(CL_GraphicContext &gc, int max_width);
	CL_SpanLayout::HitTestResult hit_test(CL_GraphicContext &gc, const CL_Point &pos);
	void draw_layout(CL_GraphicContext &gc);
	void set_position(const CL_Point &pos) { position = pos; }
	CL_Size get_size() const;
	std::vector<CL_Rect> get_rect_by_id(int id) const;
	void set_align(CL_SpanAlign align);

	CL_Size find_preferred_size(CL_GraphicContext &gc);

	void set_selection_range(CL_String::size_type start, CL_String::size_type end);
	void set_selection_colors(const CL_Colorf &foreground, const CL_Colorf &background);

	CL_String get_combined_text() const;

	void set_component_geometry();

	bool cursor_visible;
	CL_String::size_type cursor_pos;
	bool cursor_overwrite_mode;
	CL_Colorf cursor_color;

private:
	struct TextBlock
	{
		TextBlock() : start(0), end(0) { }

		unsigned int start, end;
	};

	enum ObjectType
	{
		object_text,
		object_image,
		object_component
	};

	enum FloatType
	{
		float_none,
		float_left,
		float_right
	};

	struct SpanObject
	{
		SpanObject() : type(object_text), float_type(float_none), start(0), end(0), component(0), baseline_offset(0), id(-1) { }

		ObjectType type;
		FloatType float_type;

		CL_Font font;
		CL_Colorf color;
		unsigned int start, end;

		CL_Image image;
		CL_SpanComponent *component;
		int baseline_offset;

		int id;
	};

	struct LineSegment
	{
		LineSegment() : type(object_text), start(0), end(0), x_position(0), width(0), ascender(0), descender(0), component(0), baseline_offset(0), id(-1) { }

		ObjectType type;

		CL_Font font;
		CL_Colorf color;
		int start, end;
		int ascender;
		int descender;

		int x_position;
		int width;

		CL_Image image;
		CL_SpanComponent *component;
		int baseline_offset;

		int id;
	};

	struct Line
	{
		Line() : height(0), ascender(0), width(0) { }

		int width;	// Width of the entire line (including spaces)
		int height;
		int ascender;
		std::vector<LineSegment> segments;
	};

	struct TextSizeResult
	{
		TextSizeResult() : width(0),height(0),ascender(0),descender(0),objects_traversed(0) { }
		int start, end;
		int width;
		int height;
		int ascender, descender;
		int objects_traversed;
		std::vector<LineSegment> segments;
	};

	struct CurrentLine
	{
		CurrentLine() : object_index(0), x_position(0), y_position(0) { }

		std::vector<SpanObject>::size_type object_index;
		Line cur_line;
		int x_position;
		int y_position;
	};

	struct FloatBox
	{
		FloatBox() : type(object_image), component(0), id(-1) { }

		CL_Rect rect;
		ObjectType type;
		CL_Image image;
		CL_SpanComponent *component;
		int id;
	};

	TextSizeResult find_text_size(CL_GraphicContext &gc, const TextBlock &block, unsigned int object_index);
	std::vector<TextBlock> find_text_blocks();
	void layout_lines(CL_GraphicContext & gc, int max_width);
	void layout_text(CL_GraphicContext & gc, std::vector<TextBlock> blocks, std::vector<TextBlock>::size_type block_index, CurrentLine &current_line, int max_width);
	void layout_block(CurrentLine &current_line, int max_width, std::vector<TextBlock> &blocks, std::vector<TextBlock>::size_type block_index);
	void layout_float_block(CurrentLine &current_line, int max_width);
	void layout_inline_block(CurrentLine &current_line, int max_width, std::vector<TextBlock> &blocks, std::vector<TextBlock>::size_type block_index);
	void reflow_line(CurrentLine &current_line, int max_width);
	FloatBox float_box_left(FloatBox float_box, int max_width);
	FloatBox float_box_right(FloatBox float_box, int max_width);
	FloatBox float_box_any(FloatBox box, int max_width, const std::vector<FloatBox> &floats1);
	bool box_fits_on_line(const FloatBox &box, int max_width);
	void place_line_segments(CurrentLine &current_line, TextSizeResult &text_size_result);
	void force_place_line_segments(CurrentLine &current_line, TextSizeResult &text_size_result, int max_width);
	void next_line(CurrentLine &current_line);
	bool is_newline(const TextBlock &block);
	bool is_whitespace(const TextBlock &block);
	bool fits_on_line(int x_position, const TextSizeResult &text_size_result, int max_width);
	bool larger_than_line(const TextSizeResult &text_size_result, int max_width);
	void align_justify(int max_width);
	void align_center(int max_width);
	void align_right(int max_width);
	void draw_layout_image(CL_GraphicContext &gc, Line &line, LineSegment &segment, int x, int y);
	void draw_layout_text(CL_GraphicContext &gc, Line &line, LineSegment &segment, int x, int y);
	CL_String::size_type sel_start, sel_end;
	CL_Colorf sel_foreground, sel_background;

	CL_String text;
	std::vector<SpanObject> objects;
	std::vector<Line> lines;
	CL_Point position;

	std::vector<FloatBox> floats_left, floats_right;

	CL_SpanAlign alignment;

	struct LayoutCache
	{
		LayoutCache() : object_index(-1) { }
		int object_index;
		CL_FontMetrics metrics;
	};
	LayoutCache layout_cache;
};
