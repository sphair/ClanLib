/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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

#include "API/UI/View/view.h"
#include "view_layout.h"

namespace clan
{
	enum class FlexViolation
	{
		none,
		min_violation,
		max_violation
	};

	class FlexLayoutItem
	{
	public:
		View *view = nullptr;

		bool definite_main_size = false;
		bool definite_cross_size = false;

		bool definite_min_main_size = false;
		bool definite_max_main_size = false;

		bool definite_min_cross_size = false;
		bool definite_max_cross_size = false;

		bool main_auto_margin_start = false;
		bool main_auto_margin_end = false;
		bool cross_auto_margin_start = false;
		bool cross_auto_margin_end = false;

		float main_size = 0.0f;
		float cross_size = 0.0f;

		float min_main_size = 0.0f;
		float max_main_size = 1e10f;

		float min_cross_size = 0.0f;
		float max_cross_size = 1e10f;

		float main_noncontent_start = 0.0f;
		float main_noncontent_end = 0.0f;
		float cross_noncontent_start = 0.0f;
		float cross_noncontent_end = 0.0f;

		float flex_base_size = 0.0f;
		float flex_preferred_main_size = 0.0f;
		float flex_grow = 0.0f;
		float flex_shrink = 0.0f;

		float flex_preferred_cross_size = 0.0f;

		float used_main_size = 0.0f;
		float used_cross_size = 0.0f;

		float used_main_pos = 0.0f;
		float used_cross_pos = 0.0f;

		bool frozen = false;
		float scaled_flex_shrink = 0.0f;
		FlexViolation flex_violation = FlexViolation::none;

		float strut_size = 0.0f;
		bool collapsed = false;
	};

	class FlexLayoutLine
	{
	public:
		typedef std::vector<FlexLayoutItem>::iterator iterator;

		FlexLayoutLine(iterator begin, iterator end) : _first(begin), _second(end) { }

		iterator begin() { return _first; }
		iterator end() { return _second; }

		void set_end(iterator end) { _second = end; }

		float total_flex_preferred_main_size = 0.0f;
		float total_main_noncontent = 0.0f;

		float cross_size = 0.0f;

	private:
		iterator _first, _second;
	};

	enum class FlexDirection
	{
		row,
		column
	};

	enum class FlexWrap
	{
		nowrap,
		wrap,
		wrap_reverse
	};

	enum class FlexLayoutMode
	{
		normal,
		preferred_width,
		preferred_height
	};

	class FlexLayout : public ViewLayout
	{
	public:
		float preferred_width(Canvas &canvas, View *view) override;
		float preferred_height(Canvas &canvas, View *view, float width) override;
		float first_baseline_offset(Canvas &canvas, View *view, float width) override;
		float last_baseline_offset(Canvas &canvas, View *view, float width) override;
		void layout_children(Canvas &canvas, View *view) override;

	private:
		void calculate_layout(Canvas &canvas, View *view, FlexLayoutMode mode = FlexLayoutMode::normal, float layout_width = 0.0f);

		void create_items(Canvas &canvas, View *view);
		void create_row_items(Canvas &canvas, View *view);
		void create_column_items(Canvas &canvas, View *view);
		void create_lines(Canvas &canvas, View *view);
		void flex_lines(Canvas &canvas, View *view);

		void calculate_items_preferred_cross_size(Canvas &canvas, View *view);
		void calculate_lines_cross_size(Canvas &canvas, View *view);

		void main_axis_alignment(Canvas &canvas, View *view);
		void cross_axis_alignment(Canvas &canvas, View *view);

		FlexLayoutMode layout_mode = FlexLayoutMode::normal;
		float layout_width = 0.0f;

		FlexDirection direction = FlexDirection::row;
		FlexWrap wrap = FlexWrap::nowrap;

		float container_main_size = 0.0f;
		float container_cross_size = 0.0f;

		bool known_container_main_size = false;
		bool known_container_cross_size = false;

		std::vector<FlexLayoutItem> items;
		std::vector<FlexLayoutLine> lines;
		
		bool restarted_layout = false;
	};
}
