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

#include "UI/precomp.h"

#include "span_layout_view_impl.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/2D/brush.h"
#include "API/Display/Font/glyph_metrics.h"
#include "API/Display/Font/font_metrics.h"
#include <algorithm>

namespace clan
{
	SpanLayoutViewImpl::SpanLayoutViewImpl()
	{
	}

	void SpanLayoutViewImpl::set_text_alignment(TextAlignment text_alignment)
	{
		alignment = text_alignment;
	}

	void SpanLayoutViewImpl::clear()
	{
		objects.clear();
		text.clear();
	}

	void SpanLayoutViewImpl::add_text(const std::string &more_text, const std::shared_ptr<Style> &style, int id)
	{
		SpanObject object;
		object.type = SpanObjectType::text;
		object.start = text.length();
		object.end = object.start + more_text.length();
		object.set_style(&view->style_cascade(), style);
		object.id = id;
		objects.push_back(object);
		text += more_text;
	}

	void SpanLayoutViewImpl::add_child(const std::shared_ptr<View> &view, float baseline_offset, int id)
	{
		SpanObject object;
		object.type = SpanObjectType::view;
		object.view = view;
		object.baseline_offset = baseline_offset;
		object.id = id;
		object.start = text.length();
		object.end = object.start + 1;
		objects.push_back(object);
		text += "*";
	}

	void SpanLayoutViewImpl::set_last_baseline_offset(float baseline_offset)
	{
		objects.back().baseline_offset = baseline_offset;
	}

	void SpanLayoutViewImpl::remove_child(const std::shared_ptr<View> &view)
	{
		for (auto it = objects.begin(); it != objects.end(); ++it)
		{
			if (it->view == view)
			{
				objects.erase(it);
				break;
			}
		}
	}

	void SpanLayoutViewImpl::render_content(Canvas &canvas, float width)
	{
		float y = 0.0f;
		size_t obj_start = 0;
		size_t text_start = 0;
		while (obj_start < objects.size())
		{
			SpanLineMetrics metrics = find_line_metrics(canvas, obj_start, text_start, width);

			float x = 0.0f;

			if (alignment == TextAlignment::center)
			{
				x = (width - metrics.advance_width) * 0.5f;
			}
			else if (alignment == TextAlignment::right)
			{
				x = width - metrics.advance_width;
			}

			size_t obj_end = metrics.text_end != 0 ? metrics.object_end + 1 : metrics.object_end;
			for (size_t obj_index = metrics.object_start; obj_index < obj_end; obj_index++)
			{
				SpanObject &object = objects[obj_index];

				if (object.type == SpanObjectType::text)
				{
					size_t obj_text_start = obj_index == metrics.object_start ? metrics.text_start : object.start;
					size_t obj_text_end = obj_index == metrics.object_end ? metrics.text_end : object.end;
					size_t obj_text_length = obj_text_end - obj_text_start;

					std::string obj_text = text.substr(obj_text_start, obj_text_length);

					GlyphMetrics advance = object.get_font(canvas).measure_text(canvas, obj_text);

					clan::Font font = object.get_font(canvas);
					font.draw_text(canvas, x, y + metrics.ascent + object.baseline_offset, obj_text, object.style_cascade.computed_value("color").color());

					x += advance.advance.width;
				}
				else if (object.type == SpanObjectType::view)
				{
					x += object.view->geometry().margin_box().get_width();
				}
			}

			y += metrics.ascent + metrics.descent;

			obj_start = metrics.object_end;
			text_start = metrics.text_end;
		}
	}

	void SpanLayoutViewImpl::layout_views(Canvas &canvas, float width)
	{
		float y = 0.0f;
		size_t obj_start = 0;
		size_t text_start = 0;
		while (obj_start < objects.size())
		{
			SpanLineMetrics metrics = find_line_metrics(canvas, obj_start, text_start, width);

			float x = 0.0f;

			if (alignment == TextAlignment::center)
			{
				x = (width - metrics.advance_width) * 0.5f;
			}
			else if (alignment == TextAlignment::right)
			{
				x = width - metrics.advance_width;
			}

			size_t obj_end = metrics.text_end != 0 ? metrics.object_end + 1 : metrics.object_end;
			for (size_t obj_index = metrics.object_start; obj_index < obj_end; obj_index++)
			{
				SpanObject &object = objects[obj_index];

				if (object.type == SpanObjectType::text)
				{
					size_t obj_text_start = obj_index == metrics.object_start ? metrics.text_start : object.start;
					size_t obj_text_end = obj_index == metrics.object_end ? metrics.text_end : object.end;
					size_t obj_text_length = obj_text_end - obj_text_start;

					GlyphMetrics advance = object.get_font(canvas).measure_text(canvas, text.substr(obj_text_start, obj_text_length));
					x += advance.advance.width;
				}
				else if (object.type == SpanObjectType::view)
				{
					float obj_x = x;
					float obj_y = y + metrics.ascent + object.baseline_offset;

					float obj_width = object.view->preferred_width(canvas);
					float obj_height = object.view->preferred_height(canvas, obj_width);
					float obj_baseline_offset = object.view->first_baseline_offset(canvas, obj_width);

					if (obj_baseline_offset == 0.0f) // Hmm, do we need first_baseline_offset to be able to return that there is no baseline?
						obj_baseline_offset = obj_height;

					obj_width += object.view->style_cascade().computed_value("margin-left").number();
					obj_width += object.view->style_cascade().computed_value("border-left-width").number();
					obj_width += object.view->style_cascade().computed_value("padding-left").number();
					obj_width += object.view->style_cascade().computed_value("margin-right").number();
					obj_width += object.view->style_cascade().computed_value("border-right-width").number();
					obj_width += object.view->style_cascade().computed_value("padding-right").number();

					obj_height += object.view->style_cascade().computed_value("margin-top").number();
					obj_height += object.view->style_cascade().computed_value("border-top-width").number();
					obj_height += object.view->style_cascade().computed_value("padding-top").number();
					obj_height += object.view->style_cascade().computed_value("margin-bottom").number();
					obj_height += object.view->style_cascade().computed_value("border-bottom-width").number();
					obj_height += object.view->style_cascade().computed_value("padding-bottom").number();

					obj_baseline_offset += object.view->style_cascade().computed_value("margin-top").number();
					obj_baseline_offset += object.view->style_cascade().computed_value("border-top-width").number();
					obj_baseline_offset += object.view->style_cascade().computed_value("padding-top").number();

					obj_y -= obj_baseline_offset;

					object.view->set_geometry(ViewGeometry::from_margin_box(object.view->style_cascade(), Rectf::xywh(obj_x, obj_y, obj_width, obj_height)));

					x += obj_width;
				}
			}

			y += metrics.ascent + metrics.descent;

			obj_start = metrics.object_end;
			text_start = metrics.text_end;
		}
	}

	float SpanLayoutViewImpl::preferred_width(Canvas &canvas)
	{
		float x = 0.0f;
		for (SpanObject &object : objects)
		{
			if (object.type == SpanObjectType::text)
			{
				GlyphMetrics advance = object.get_font(canvas).measure_text(canvas, text.substr(object.start, object.end - object.start));
				x += advance.advance.width;
			}
			else if (object.type == SpanObjectType::view)
			{
				x += object.view->preferred_width(canvas);
			}
		}
		return x;
	}

	float SpanLayoutViewImpl::preferred_height(Canvas &canvas, float width)
	{
		float y = 0.0f;
		size_t obj_start = 0;
		size_t text_start = 0;
		while (obj_start < objects.size())
		{
			SpanLineMetrics metrics = find_line_metrics(canvas, obj_start, text_start, width);
			y += metrics.ascent + metrics.descent;

			obj_start = metrics.object_end;
			text_start = metrics.text_end;
		}
		return y;
	}

	float SpanLayoutViewImpl::first_baseline_offset(Canvas &canvas, float width)
	{
		return find_line_metrics(canvas, 0, 0, width).ascent;
	}

	float SpanLayoutViewImpl::last_baseline_offset(Canvas &canvas, float width)
	{
		float y = 0.0f;
		size_t obj_start = 0;
		size_t text_start = 0;
		while (true)
		{
			SpanLineMetrics metrics = find_line_metrics(canvas, obj_start, text_start, width);

			if (metrics.object_end == objects.size())
				return y + metrics.ascent;

			y += metrics.ascent + metrics.descent;
			obj_start = metrics.object_end;
			text_start = metrics.text_end;
		}
	}

	SpanLineMetrics SpanLayoutViewImpl::find_line_metrics(Canvas &canvas, size_t obj_start, size_t text_start, float width)
	{
		float line_ascent = 0.0f;
		float line_descent = 0.0f;
		float x = 0.0f;

		for (size_t obj_index = obj_start; obj_index < objects.size(); obj_index++)
		{
			SpanObject &object = objects[obj_index];

			float obj_ascent = 0.0f;
			float obj_descent = 0.0f;
			float obj_advance_width = 0.0f;

			size_t obj_text_start = obj_index == obj_start ? text_start : object.start;
			size_t obj_text_length = object.end - obj_text_start;

			if (object.type == SpanObjectType::text)
			{
				std::string obj_text = text.substr(obj_text_start, obj_text_length);

				obj_advance_width = object.get_font(canvas).measure_text(canvas, obj_text).advance.width;

				FontMetrics font_metrics = object.get_font(canvas).get_font_metrics(canvas);
				obj_ascent = font_metrics.get_baseline_offset();
				obj_descent = font_metrics.get_line_height() - font_metrics.get_baseline_offset();

				if (x + obj_advance_width > width)
				{
					size_t char_clip_pos = object.get_font(canvas).clip_from_left(canvas, obj_text, width - x);

					size_t word_clip_pos = char_clip_pos;
					if (word_clip_pos > 0)
					{
						word_clip_pos = obj_text.find_last_of(" \t", word_clip_pos - 1);
						word_clip_pos = word_clip_pos == std::string::npos ? 0 : word_clip_pos + 1;
					}

					if (x == 0.0f && word_clip_pos == 0)
					{
						word_clip_pos = char_clip_pos;
					}

					if (x != 0.0f || word_clip_pos > 0)
					{
						obj_text_length = word_clip_pos;
						obj_advance_width = object.get_font(canvas).measure_text(canvas, text.substr(obj_text_start, obj_text_length)).advance.width;

						obj_ascent = std::max(obj_ascent + object.baseline_offset, 0.0f);
						obj_descent = std::max(obj_descent - object.baseline_offset, 0.0f);

						line_ascent = std::max(line_ascent, obj_ascent);
						line_descent = std::max(line_descent, obj_descent);

						x += obj_advance_width;

						return SpanLineMetrics(obj_start, obj_index, text_start, obj_text_start + obj_text_length, line_ascent, line_descent, x);
					}
				}
			}
			else if (object.type == SpanObjectType::view)
			{
				float obj_width = object.view->preferred_width(canvas);
				float obj_height = object.view->preferred_height(canvas, obj_width);
				float obj_baseline_offset = object.view->first_baseline_offset(canvas, obj_width);

				if (obj_baseline_offset == 0.0f) // Hmm, do we need first_baseline_offset to be able to return that there is no baseline?
					obj_baseline_offset = obj_height;

				obj_width += object.view->style_cascade().computed_value("margin-left").number();
				obj_width += object.view->style_cascade().computed_value("border-left-width").number();
				obj_width += object.view->style_cascade().computed_value("padding-left").number();
				obj_width += object.view->style_cascade().computed_value("margin-right").number();
				obj_width += object.view->style_cascade().computed_value("border-right-width").number();
				obj_width += object.view->style_cascade().computed_value("padding-right").number();

				obj_height += object.view->style_cascade().computed_value("margin-top").number();
				obj_height += object.view->style_cascade().computed_value("border-top-width").number();
				obj_height += object.view->style_cascade().computed_value("padding-top").number();
				obj_height += object.view->style_cascade().computed_value("margin-bottom").number();
				obj_height += object.view->style_cascade().computed_value("border-bottom-width").number();
				obj_height += object.view->style_cascade().computed_value("padding-bottom").number();

				obj_baseline_offset += object.view->style_cascade().computed_value("margin-top").number();
				obj_baseline_offset += object.view->style_cascade().computed_value("border-top-width").number();
				obj_baseline_offset += object.view->style_cascade().computed_value("padding-top").number();

				obj_ascent = obj_baseline_offset;
				obj_descent = obj_height - obj_baseline_offset;

				obj_advance_width = obj_width;

				if (x != 0.0f && x + obj_advance_width > width)
				{
					return SpanLineMetrics(obj_start, obj_index, text_start, obj_text_start + obj_text_length, line_ascent, line_descent, x);
				}
			}

			obj_ascent = std::max(obj_ascent + object.baseline_offset, 0.0f);
			obj_descent = std::max(obj_descent - object.baseline_offset, 0.0f);

			line_ascent = std::max(line_ascent, obj_ascent);
			line_descent = std::max(line_descent, obj_descent);

			x += obj_advance_width;
		}

		return SpanLineMetrics(obj_start, objects.size(), text_start, 0, line_ascent, line_descent, x);
	}
}
