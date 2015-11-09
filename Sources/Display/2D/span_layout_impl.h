/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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

namespace clan
{
	class Canvas;

	/// \brief Span component class
	class SpanComponent
	{
	public:
		virtual ~SpanComponent() { }

		/// \brief Get Size
		///
		/// \return size
		virtual Size get_size() const = 0;

		/// \brief Set geometry
		///
		/// \param geometry = Rect
		virtual void set_geometry(const Rect &geometry) = 0;
	};

	/// \brief Span Component Binder (templated class)
	///
	template<typename T>
	class SpanComponentBinder : public SpanComponent
	{
	public:

		/// \brief Constructs a SpanComponentBinder
		///
		/// \param component = T
		SpanComponentBinder(T *component)
			: component(component)
		{
		}

		/// \brief Get Size
		///
		/// \return size
		Size get_size() const override
		{
			return component->get_size();
		}

		/// \brief Set geometry
		///
		/// \param geometry = Rect
		void set_geometry(const Rect &geometry) override
		{
			component->set_geometry(geometry);
		}

	private:
		T *component;
	};

	class SpanLayout_Impl
	{
	public:
		SpanLayout_Impl();
		~SpanLayout_Impl();

		void clear();

		void add_text(const std::string &text, const Font &font, const Colorf &color, int id = -1);
		void add_image(const Image &image, int baseline_offset, int id = -1);
		void add_component(SpanComponent *component, int baseline_offset = 0, int id = -1);

		void layout(Canvas &canvasc, int max_width);
		SpanLayout::HitTestResult hit_test(Canvas &canvas, const Point &pos);
		void draw_layout(Canvas &canvas);
		void draw_layout_ellipsis(Canvas &canvas, const Rect &content_rect);
		void set_position(const Point &pos) { position = pos; }
		Rect get_rect() const;
		std::vector<Rect> get_rect_by_id(int id) const;
		void set_align(SpanAlign align);

		Size find_preferred_size(Canvas &canvas);

		void set_selection_range(std::string::size_type start, std::string::size_type end);
		void set_selection_colors(const Colorf &foreground, const Colorf &background);

		std::string get_combined_text() const;

		void set_component_geometry();

		int get_first_baseline_offset();
		int get_last_baseline_offset();

		bool cursor_visible;
		std::string::size_type cursor_pos;
		bool cursor_overwrite_mode;
		Colorf cursor_color;

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
			SpanObject() : type(object_text), float_type(float_none), start(0), end(0), component(nullptr), baseline_offset(0), id(-1) { }

			ObjectType type;
			FloatType float_type;

			Font font;
			Colorf color;
			unsigned int start, end;

			Image image;
			SpanComponent *component;
			int baseline_offset;

			int id;
		};

		struct LineSegment
		{
			LineSegment() { }

			ObjectType type = object_text;

			Font font;
			Colorf color;
			int start = 0, end = 0;
			int ascender = 0;
			int descender = 0;

			int x_position = 0;
			int width = 0;

			Image image;
			SpanComponent *component = nullptr;
			int baseline_offset = 0;

			int id = -1;
		};

		struct Line
		{
			Line() { }

			int width = 0;	// Width of the entire line (including spaces)
			int height = 0;
			int ascender = 0;
			std::vector<LineSegment> segments;
		};

		struct TextSizeResult
		{
			TextSizeResult()  { }
			int start = 0, end = 0;
			int width = 0;
			int height = 0;
			int ascender = 0, descender = 0;
			int objects_traversed = 0;
			std::vector<LineSegment> segments;
		};

		struct CurrentLine
		{
			CurrentLine() { }

			std::vector<SpanObject>::size_type object_index = 0;
			Line cur_line;
			int x_position = 0;
			int y_position = 0;
		};

		struct FloatBox
		{
			FloatBox() { }

			Rect rect;
			ObjectType type = object_image;
			Image image;
			SpanComponent *component = nullptr;
			int id = 1;
		};

		TextSizeResult find_text_size(Canvas &canvas, const TextBlock &block, unsigned int object_index);
		std::vector<TextBlock> find_text_blocks();
		void layout_lines(Canvas &canvas, int max_width);
		void layout_text(Canvas &canvas, std::vector<TextBlock> blocks, std::vector<TextBlock>::size_type block_index, CurrentLine &current_line, int max_width);
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
		void draw_layout_image(Canvas &canvas, Line &line, LineSegment &segment, int x, int y);
		void draw_layout_text(Canvas &canvas, Line &line, LineSegment &segment, int x, int y);
		std::string::size_type sel_start, sel_end;
		Colorf sel_foreground, sel_background;

		std::string text;
		std::vector<SpanObject> objects;
		std::vector<Line> lines;
		Point position;

		std::vector<FloatBox> floats_left, floats_right;

		SpanAlign alignment;

		struct LayoutCache
		{
			LayoutCache() : object_index(-1) { }
			int object_index;
			FontMetrics metrics;
		};
		LayoutCache layout_cache;

		bool is_ellipsis_draw;
		Rect ellipsis_content_rect;
	};
}
