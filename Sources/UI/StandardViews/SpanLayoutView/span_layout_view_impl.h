/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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

#include "API/UI/StandardViews/span_layout_view.h"
#include "API/UI/Style/style.h"
#include <map>

namespace clan
{
	enum class SpanObjectType
	{
		text,
		view
	};

	enum class SpanFloatType
	{
		none,
		left,
		right
	};

	struct SpanObject
	{
		SpanObjectType type = SpanObjectType::text;
		SpanFloatType float_type = SpanFloatType::none;
		
		StyleCascade style_cascade;
		void set_style(const StyleCascade *parent_cascade, const std::shared_ptr<Style> new_style)
		{
			style = new_style;
			style_cascade = StyleCascade({ style.get() }, parent_cascade);
		}

		size_t start = 0;
		size_t end = 0;

		std::shared_ptr<View> view;
		float baseline_offset = 0.0f;

		int id = -1;

		const Font &get_font(Canvas &canvas)
		{
			if (!font)
				font = style_cascade.font(canvas);
			return font;
		}

	private:
		Font font;
		std::shared_ptr<Style> style;
	};

	struct SpanLineMetrics
	{
		SpanLineMetrics() { }
		SpanLineMetrics(size_t object_start, size_t object_end, size_t text_start, size_t text_end, float ascent, float descent, float advance_width)
			: object_start(object_start), object_end(object_end), text_start(text_start), text_end(text_end), ascent(ascent), descent(descent), advance_width(advance_width)
		{
		}

		size_t object_start = 0;
		size_t object_end = 0;
		size_t text_start = 0;
		size_t text_end = 0;
		float ascent = 0.0f;
		float descent = 0.0f;
		float advance_width = 0.0f;
	};

	class SpanLayoutViewImpl
	{
	public:
		SpanLayoutViewImpl();

		SpanLayoutView *view = nullptr;

		void clear();
		void add_text(const std::string &text, const std::shared_ptr<Style> &style, int id = -1);
		void add_child(const std::shared_ptr<View> &view, float baseline_offset = 0.0f, int id = -1);
		void set_last_baseline_offset(float baseline_offset);
		void remove_child(const std::shared_ptr<View> &view);

		void render_content(Canvas &canvas, float width);
		void layout_views(Canvas &canvas, float width);

		float preferred_width(Canvas &canvas);
		float preferred_height(Canvas &canvas, float width);
		float first_baseline_offset(Canvas &canvas, float width);
		float last_baseline_offset(Canvas &canvas, float width);

		void set_text_alignment(TextAlignment alignment);

		mutable std::map<std::string, std::shared_ptr<Style>> text_classes;

	private:
		SpanLineMetrics find_line_metrics(Canvas &canvas, size_t obj_start, size_t text_start, float width);

		std::string text;
		std::vector<SpanObject> objects;
		TextAlignment alignment = TextAlignment::left;
	};
}
