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

#include "UI/precomp.h"
#include "API/UI/StandardViews/label_view.h"
#include "API/UI/UIThread/ui_thread.h"
#include "API/UI/Style/style.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/2D/path.h"
#include "API/Display/2D/pen.h"
#include "API/Display/2D/brush.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/Font/glyph_metrics.h"
#include <cmath>

namespace clan
{
	class LabelViewImpl
	{
	public:
		std::string _text;
		TextAlignment text_alignment = TextAlignment::left;
		Font font;
		LineBreakMode _line_break_mode = LineBreakMode::truncating_tail;

		Font &get_font(LabelView *view, Canvas &canvas)
		{
			if (font.is_null())
				font = view->style_cascade().get_font(canvas);
			return font;
		}
	};

	LabelView::LabelView() : impl(new LabelViewImpl())
	{
	}

	void LabelView::layout_subviews(Canvas &canvas)
	{
		View::layout_subviews(canvas);
		impl->font = style_cascade().get_font(canvas);	// Reset the font on new layout
	}

	std::string LabelView::text() const
	{
		return impl->_text;
	}

	void LabelView::set_text(const std::string &value)
	{
		impl->_text = value;
		set_needs_layout();
	}

	TextAlignment LabelView::text_alignment() const
	{
		return impl->text_alignment;
	}

	void LabelView::set_text_alignment(TextAlignment alignment)
	{
		impl->text_alignment = alignment;
	}

	LineBreakMode LabelView::line_break_mode() const
	{
		return impl->_line_break_mode;
	}

	void LabelView::set_line_break_mode(LineBreakMode value)
	{
		impl->_line_break_mode = value;
		set_needs_layout();
	}

	void LabelView::render_content(Canvas &canvas)
	{
		Font font = impl->get_font(this, canvas);
		FontMetrics font_metrics = font.get_font_metrics(canvas);
		float baseline = font_metrics.get_baseline_offset();

		std::string clipped_text = impl->_text;
		GlyphMetrics advance = font.measure_text(canvas, clipped_text);

		if (advance.advance.width > geometry().content_width)
		{
			std::string ellipsis = StringHelp::unicode_to_utf8(0x2026);
			GlyphMetrics ellipsis_advance = font.measure_text(canvas, ellipsis);

			switch (impl->_line_break_mode)
			{
			case LineBreakMode::clipping:
				clipped_text = clipped_text.substr(0, font.clip_from_left(canvas, clipped_text, geometry().content_width));
				break;
			case LineBreakMode::truncating_head:
				clipped_text = ellipsis + clipped_text.substr(font.clip_from_right(canvas, clipped_text, geometry().content_width - ellipsis_advance.advance.width));
				break;
			case LineBreakMode::truncating_middle:
				{
					std::string text_left = clipped_text.substr(0, font.clip_from_left(canvas, clipped_text, geometry().content_width * 0.5f - ellipsis_advance.advance.width));
					std::string text_right = clipped_text.substr(font.clip_from_right(canvas, clipped_text, geometry().content_width * 0.5f - ellipsis_advance.advance.width));
					clipped_text = text_left + ellipsis + text_right;
				}
				break;
			case LineBreakMode::truncating_tail:
				clipped_text = clipped_text.substr(0, font.clip_from_left(canvas, clipped_text, geometry().content_width - ellipsis_advance.advance.width)) + ellipsis;
				break;
			default:
				break;
			}

			advance = font.measure_text(canvas, clipped_text);
			
			if (advance.advance.width > geometry().content_width)
				return; // Still no room.  Draw nothing!
		}

		Colorf color = style_cascade().computed_value("color").color();

		if (impl->text_alignment == TextAlignment::left)
		{
			font.draw_text(canvas, Pointf(0.0f, baseline), clipped_text, color);
		}
		else if (impl->text_alignment == TextAlignment::right)
		{
			font.draw_text(canvas, Pointf(geometry().content_width - advance.advance.width, baseline), clipped_text, color);
		}
		else if (impl->text_alignment == TextAlignment::center)
		{
			font.draw_text(canvas, Pointf(std::round((geometry().content_width - advance.advance.width) * 0.5f), baseline), clipped_text, color);
		}
	}

	float LabelView::calculate_preferred_width(Canvas &canvas)
	{
		if (style_cascade().computed_value("width").is_keyword("auto"))
		{
			Font font = impl->get_font(this, canvas);
			return font.measure_text(canvas, impl->_text).advance.width + 1.0f;
		}
		else
			return style_cascade().computed_value("width").number();
	}

	float LabelView::calculate_preferred_height(Canvas &canvas, float width)
	{
		if (style_cascade().computed_value("height").is_keyword("auto"))
		{
			Font font = impl->get_font(this, canvas);
			return font.get_font_metrics(canvas).get_line_height();
		}
		else
			return style_cascade().computed_value("height").number();
	}

	float LabelView::calculate_first_baseline_offset(Canvas &canvas, float width)
	{
		Font font = impl->get_font(this, canvas);
		return font.get_font_metrics(canvas).get_baseline_offset();
	}

	float LabelView::calculate_last_baseline_offset(Canvas &canvas, float width)
	{
		return get_first_baseline_offset(canvas, width);
	}
}
