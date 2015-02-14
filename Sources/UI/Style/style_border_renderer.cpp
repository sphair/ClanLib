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
#include "style_border_renderer.h"
#include "API/UI/Style/box_geometry.h"
#include "API/UI/Style/style.h"
#include "API/UI/UIThread/ui_thread.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/2D/image.h"

namespace clan
{
	StyleBorderRenderer::StyleBorderRenderer(Canvas &canvas, const BoxGeometry &geometry, const Style &style) : canvas(canvas), geometry(geometry), style(style)
	{
	}

	void StyleBorderRenderer::render()
	{
		if (style.computed_value("border-image-source").is_url())
		{
			Image &image = Image::resource(canvas, style.computed_value("border-image-source").text, UIThread::get_resources());
			if (!image.is_null())
			{
				int slice_left = get_left_slice_value(image.get_width());
				int slice_right = get_right_slice_value(image.get_width());
				int slice_top = get_top_slice_value(image.get_height());
				int slice_bottom = get_bottom_slice_value(image.get_height());
				bool fill_center = style.computed_value("border-image-slice-center").is_keyword("fill");

				Rect border_image_area = get_border_image_area();

				int grid_left = get_left_grid(border_image_area.get_width(), slice_left);
				int grid_right = get_right_grid(border_image_area.get_width(), slice_right);
				int grid_top = get_top_grid(border_image_area.get_height(), slice_top);
				int grid_bottom = get_bottom_grid(border_image_area.get_height(), slice_bottom);

				int x[4] = { border_image_area.left, border_image_area.left + grid_left, border_image_area.right - grid_right, border_image_area.right };
				int y[4] = { border_image_area.top, border_image_area.top + grid_top, border_image_area.bottom - grid_bottom, border_image_area.bottom };
				int sx[4] = { 0, slice_left, image.get_width() - slice_right, image.get_width() };
				int sy[4] = { 0, slice_top, image.get_height() - slice_bottom, image.get_height() };

				for (int yy = 0; yy < 3; yy++)
				{
					for (int xx = 0; xx < 3; xx++)
					{
						if ((xx != 1 && yy != 1) || fill_center)
							draw_area(image, x[xx], y[yy], x[xx + 1] - x[xx], y[yy + 1] - y[yy], sx[xx], sy[yy], sx[xx + 1] - sx[xx], sy[yy + 1] - sy[yy]);
					}
				}
			}
		}
		else
		{
			float outer_radius_top_left_x = get_horizontal_radius(style.computed_value("border-top-left-radius-x"));
			float outer_radius_top_left_y = get_vertical_radius(style.computed_value("border-top-left-radius-y"));
			float outer_radius_top_right_x = get_horizontal_radius(style.computed_value("border-top-right-radius-x"));
			float outer_radius_top_right_y = get_vertical_radius(style.computed_value("border-top-right-radius-y"));
			float outer_radius_bottom_left_x = get_horizontal_radius(style.computed_value("border-bottom-left-radius-x"));
			float outer_radius_bottom_left_y = get_vertical_radius(style.computed_value("border-bottom-left-radius-y"));
			float outer_radius_bottom_right_x = get_horizontal_radius(style.computed_value("border-bottom-right-radius-x"));
			float outer_radius_bottom_right_y = get_vertical_radius(style.computed_value("border-bottom-right-radius-y"));

			float inner_radius_top_left_x = max(0.0f, outer_radius_top_left_x - geometry.border_left);
			float inner_radius_top_left_y = max(0.0f, outer_radius_top_left_y - geometry.border_top);
			float inner_radius_top_right_x = max(0.0f, outer_radius_top_right_x - geometry.border_right);
			float inner_radius_top_right_y = max(0.0f, outer_radius_top_right_y - geometry.border_top);
			float inner_radius_bottom_left_x = max(0.0f, outer_radius_bottom_left_x - geometry.border_left);
			float inner_radius_bottom_left_y = max(0.0f, outer_radius_bottom_left_y - geometry.border_bottom);
			float inner_radius_bottom_right_x = max(0.0f, outer_radius_bottom_right_x - geometry.border_right);
			float inner_radius_bottom_right_y = max(0.0f, outer_radius_bottom_right_y - geometry.border_bottom);

			Rectf border_box = geometry.border_box();

			Point center_top_left(border_box.left + (int)std::round(outer_radius_top_left_x), border_box.top + (int)std::round(outer_radius_top_left_y));
			Point center_top_right(border_box.right - (int)std::round(outer_radius_top_right_x), border_box.top + (int)std::round(outer_radius_top_right_y));
			Point center_bottom_left(border_box.left + (int)std::round(outer_radius_bottom_left_x), border_box.bottom - (int)std::round(outer_radius_bottom_left_y));
			Point center_bottom_right(border_box.right - (int)std::round(outer_radius_bottom_right_x), border_box.bottom - (int)std::round(outer_radius_bottom_right_y));

			StyleValue style_top = style.computed_value("border-top-style");
			StyleValue style_right = style.computed_value("border-right-style");
			StyleValue style_bottom = style.computed_value("border-bottom-style");
			StyleValue style_left = style.computed_value("border-left-style");

			if (style_top.is_keyword("solid"))
			{
				canvas.fill_rect(Rect(center_top_left.x, border_box.top, center_top_right.x, border_box.top + geometry.border_top), style.computed_value("border-top-color").color);
			}
			else if (style_top.is_keyword("dotted"))
			{
				//canvas.dot_horizontal(Rect(center_top_left.x, border_box.top, center_top_right.x, border_box.top + geometry.border_top), style.computed_value("border-top-color").color);
			}
			else if (style_top.is_keyword("dashed"))
			{
				//canvas.dash_horizontal(Rect(center_top_left.x, border_box.top, center_top_right.x, border_box.top + geometry.border_top), style.computed_value("border-top-color").color);
			}
			else if (style_top.is_keyword("double"))
			{
				canvas.fill_rect(Rect(center_top_left.x, border_box.top, center_top_right.x, border_box.top + geometry.border_top / 3), style.computed_value("border-top-color").color);
				canvas.fill_rect(Rect(center_top_left.x, border_box.top + geometry.border_top * 2 / 3, center_top_right.x, border_box.top + geometry.border_top), style.computed_value("border-top-color").color);
			}
			else if (style_top.is_keyword("groove"))
			{
				canvas.fill_rect(Rect(center_top_left.x, border_box.top, center_top_right.x, border_box.top + geometry.border_top / 2), get_dark_color(style.computed_value("border-top-color")));
				canvas.fill_rect(Rect(center_top_left.x, border_box.top + geometry.border_top / 2, center_top_right.x, border_box.top + geometry.border_top), get_light_color(style.computed_value("border-top-color")));
			}
			else if (style_top.is_keyword("ridge"))
			{
				canvas.fill_rect(Rect(center_top_left.x, border_box.top, center_top_right.x, border_box.top + geometry.border_top / 2), get_light_color(style.computed_value("border-top-color")));
				canvas.fill_rect(Rect(center_top_left.x, border_box.top + geometry.border_top / 2, center_top_right.x, border_box.top + geometry.border_top), get_dark_color(style.computed_value("border-top-color")));
			}
			else if (style_top.is_keyword("inset"))
			{
				canvas.fill_rect(Rect(center_top_left.x, border_box.top, center_top_right.x, border_box.top + geometry.border_top), get_dark_color(style.computed_value("border-top-color")));
			}
			else if (style_top.is_keyword("outset"))
			{
				canvas.fill_rect(Rect(center_top_left.x, border_box.top, center_top_right.x, border_box.top + geometry.border_top), get_light_color(style.computed_value("border-top-color")));
			}

			if (style_bottom.is_keyword("solid"))
			{
				canvas.fill_rect(Rect(center_bottom_left.x, border_box.bottom - geometry.border_bottom, center_bottom_right.x, border_box.bottom), style.computed_value("border-bottom-color").color);
			}
			else if (style_bottom.is_keyword("dotted"))
			{
				//canvas.dot_horizontal(Rect(center_bottom_left.x, border_box.bottom - geometry.border_bottom, center_bottom_right.x, border_box.bottom), style.computed_value("border-bottom-color").color);
			}
			else if (style_bottom.is_keyword("dashed"))
			{
				//canvas.dash_horizontal(Rect(center_bottom_left.x, border_box.bottom - geometry.border_bottom, center_bottom_right.x, border_box.bottom), style.computed_value("border-bottom-color").color);
			}
			else if (style_bottom.is_keyword("double"))
			{
				canvas.fill_rect(Rect(center_bottom_left.x, border_box.bottom - geometry.border_bottom, center_bottom_right.x, border_box.bottom - geometry.border_bottom * 2 / 3), style.computed_value("border-bottom-color").color);
				canvas.fill_rect(Rect(center_bottom_left.x, border_box.bottom - geometry.border_bottom * 1 / 3, center_bottom_right.x, border_box.bottom), style.computed_value("border-bottom-color").color);
			}
			else if (style_bottom.is_keyword("groove"))
			{
				canvas.fill_rect(Rect(center_bottom_left.x, border_box.bottom - geometry.border_bottom, center_bottom_right.x, border_box.bottom - geometry.border_bottom / 2), get_dark_color(style.computed_value("border-bottom-color")));
				canvas.fill_rect(Rect(center_bottom_left.x, border_box.bottom - geometry.border_bottom / 2, center_bottom_right.x, border_box.bottom), get_light_color(style.computed_value("border-bottom-color")));
			}
			else if (style_bottom.is_keyword("ridge"))
			{
				canvas.fill_rect(Rect(center_bottom_left.x, border_box.bottom - geometry.border_bottom, center_bottom_right.x, border_box.bottom - geometry.border_bottom / 2), get_light_color(style.computed_value("border-bottom-color")));
				canvas.fill_rect(Rect(center_bottom_left.x, border_box.bottom - geometry.border_bottom / 2, center_bottom_right.x, border_box.bottom), get_dark_color(style.computed_value("border-bottom-color")));
			}
			else if (style_bottom.is_keyword("inset"))
			{
				canvas.fill_rect(Rect(center_bottom_left.x, border_box.bottom - geometry.border_bottom, center_bottom_right.x, border_box.bottom), get_light_color(style.computed_value("border-bottom-color")));
			}
			else if (style_bottom.is_keyword("outset"))
			{
				canvas.fill_rect(Rect(center_bottom_left.x, border_box.bottom - geometry.border_bottom, center_bottom_right.x, border_box.bottom), get_dark_color(style.computed_value("border-bottom-color")));
			}

			if (style_left.is_keyword("solid"))
			{
				canvas.fill_rect(Rect(border_box.left, center_top_left.y, border_box.left + geometry.border_left, center_bottom_left.y), style.computed_value("border-left-color").color);
			}
			else if (style_left.is_keyword("dotted"))
			{
				//canvas.dot_vertical(Rect(border_box.left, center_top_left.y, border_box.left + geometry.border_left, center_bottom_left.y), style.computed_value("border-left-color").color);
			}
			else if (style_left.is_keyword("dashed"))
			{
				//canvas.dash_vertical(Rect(border_box.left, center_top_left.y, border_box.left + geometry.border_left, center_bottom_left.y), style.computed_value("border-left-color").color);
			}
			else if (style_left.is_keyword("double"))
			{
				canvas.fill_rect(Rect(border_box.left, center_top_left.y, border_box.left + geometry.border_left / 3, center_bottom_left.y), style.computed_value("border-left-color").color);
				canvas.fill_rect(Rect(border_box.left + geometry.border_left * 2 / 3, center_top_left.y, border_box.left + geometry.border_left, center_bottom_left.y), style.computed_value("border-left-color").color);
			}
			else if (style_left.is_keyword("groove"))
			{
				canvas.fill_rect(Rect(border_box.left, center_top_left.y, border_box.left + geometry.border_left / 2, center_bottom_left.y), get_dark_color(style.computed_value("border-left-color")));
				canvas.fill_rect(Rect(border_box.left + geometry.border_left / 2, center_top_left.y, border_box.left + geometry.border_left, center_bottom_left.y), get_light_color(style.computed_value("border-left-color")));
			}
			else if (style_left.is_keyword("ridge"))
			{
				canvas.fill_rect(Rect(border_box.left, center_top_left.y, border_box.left + geometry.border_left / 2, center_bottom_left.y), get_light_color(style.computed_value("border-left-color")));
				canvas.fill_rect(Rect(border_box.left + geometry.border_left / 2, center_top_left.y, border_box.left + geometry.border_left, center_bottom_left.y), get_dark_color(style.computed_value("border-left-color")));
			}
			else if (style_left.is_keyword("inset"))
			{
				canvas.fill_rect(Rect(border_box.left, center_top_left.y, border_box.left + geometry.border_left, center_bottom_left.y), get_dark_color(style.computed_value("border-left-color")));
			}
			else if (style_left.is_keyword("outset"))
			{
				canvas.fill_rect(Rect(border_box.left, center_top_left.y, border_box.left + geometry.border_left, center_bottom_left.y), get_light_color(style.computed_value("border-left-color")));
			}

			if (style_right.is_keyword("solid"))
			{
				canvas.fill_rect(Rect(border_box.right - geometry.border_right, center_top_right.y, border_box.right, center_bottom_right.y), style.computed_value("border-right-color").color);
			}
			else if (style_right.is_keyword("dotted"))
			{
				//canvas.dot_vertical(Rect(border_box.right - geometry.border_right, center_top_right.y, border_box.right, center_bottom_right.y), style.computed_value("border-right-color").color);
			}
			else if (style_right.is_keyword("dashed"))
			{
				//canvas.dash_vertical(Rect(border_box.right - geometry.border_right, center_top_right.y, border_box.right, center_bottom_right.y), style.computed_value("border-right-color").color);
			}
			else if (style_right.is_keyword("double"))
			{
				canvas.fill_rect(Rect(border_box.right - geometry.border_right, center_top_right.y, border_box.right - geometry.border_right * 2 / 3, center_bottom_right.y), style.computed_value("border-right-color").color);
				canvas.fill_rect(Rect(border_box.right - geometry.border_right / 3, center_top_right.y, border_box.right, center_bottom_right.y), style.computed_value("border-right-color").color);
			}
			else if (style_right.is_keyword("groove"))
			{
				canvas.fill_rect(Rect(border_box.right - geometry.border_right, center_top_right.y, border_box.right - geometry.border_right / 2, center_bottom_right.y), get_dark_color(style.computed_value("border-right-color")));
				canvas.fill_rect(Rect(border_box.right - geometry.border_right / 2, center_top_right.y, border_box.right, center_bottom_right.y), get_light_color(style.computed_value("border-right-color")));
			}
			else if (style_right.is_keyword("ridge"))
			{
				canvas.fill_rect(Rect(border_box.right - geometry.border_right, center_top_right.y, border_box.right - geometry.border_right / 2, center_bottom_right.y), get_light_color(style.computed_value("border-right-color")));
				canvas.fill_rect(Rect(border_box.right - geometry.border_right / 2, center_top_right.y, border_box.right, center_bottom_right.y), get_dark_color(style.computed_value("border-right-color")));
			}
			else if (style_right.is_keyword("inset"))
			{
				canvas.fill_rect(Rect(border_box.right - geometry.border_right, center_top_right.y, border_box.right, center_bottom_right.y), get_light_color(style.computed_value("border-right-color")));
			}
			else if (style_right.is_keyword("outset"))
			{
				canvas.fill_rect(Rect(border_box.right - geometry.border_right, center_top_right.y, border_box.right, center_bottom_right.y), get_dark_color(style.computed_value("border-right-color")));
			}
		}
	}

	void StyleBorderRenderer::draw_area(Image &image, int x, int y, int w, int h, int sx, int sy, int sw, int sh)
	{
		StyleValue repeat_x = style.computed_value("border-image-repeat-x");
		StyleValue repeat_y = style.computed_value("border-image-repeat-y");

		if ((repeat_x.is_keyword("repeat") || repeat_x.is_keyword("stretch")) && (repeat_y.is_keyword("stretch") || repeat_y.is_keyword("repeat")))
		{
			int image_width = w;
			int image_height = h;
			if (repeat_x.is_keyword("repeat"))
				image_width = sw;
			if (repeat_y.is_keyword("repeat"))
				image_height = sh;

			// This is wrong.  It is only a temporary hack so the focus almost works for clanGUI
			// See specification http://www.w3.org/TR/style3-background/#the-border-image-repeat

			for (int dy = y; dy < (h + y); dy += image_height)
			{
				for (int dx = x; dx < (w + x); dx += image_width)
				{
					image.draw(canvas, Rect(dx, dy, dx + image_width, dy + image_height), Rect(sx, sy, sx + sw, sy + sh));
				}
			}
		}
		else
		{
			image.draw(canvas, Rect(x, y, x + w, y + h), Rect(sx, sy, sx + sw, sy + sh));
			// Support me
		}
	}

	Rect StyleBorderRenderer::get_border_image_area() const
	{
		Rect box = geometry.border_box();

		StyleValue outset_left = style.computed_value("border-image-outset-left");
		StyleValue outset_right = style.computed_value("border-image-outset-right");
		StyleValue outset_top = style.computed_value("border-image-outset-top");
		StyleValue outset_bottom = style.computed_value("border-image-outset-bottom");

		if (outset_left.is_length() || outset_left.is_number())
			box.left -= outset_left.number;

		if (outset_right.is_length() || outset_right.is_number())
			box.right += outset_right.number;

		if (outset_top.is_length() || outset_top.is_number())
			box.top -= outset_top.number;

		if (outset_bottom.is_length() || outset_bottom.is_number())
			box.bottom += outset_bottom.number;

		return box;
	}

	float StyleBorderRenderer::get_left_grid(float image_area_width, float auto_width) const
	{
		StyleValue border_image_width = style.computed_value("border-image-width-left");

		if (border_image_width.is_percentage())
			return border_image_width.number * image_area_width / 100.0f;
		else if (border_image_width.is_number())
			return border_image_width.number * geometry.border_left;
		else
			return auto_width;
	}

	float StyleBorderRenderer::get_right_grid(float image_area_width, float auto_width) const
	{
		StyleValue border_image_width = style.computed_value("border-image-width-right");

		if (border_image_width.is_percentage())
			return border_image_width.number * image_area_width / 100.0f;
		else if (border_image_width.is_number())
			return border_image_width.number * geometry.border_right;
		else
			return auto_width;
	}

	float StyleBorderRenderer::get_top_grid(float image_area_height, float auto_height) const
	{
		StyleValue border_image_width = style.computed_value("border-image-width-top");

		if (border_image_width.is_percentage())
			return border_image_width.number * image_area_height / 100.0f;
		else if (border_image_width.is_number())
			return border_image_width.number * geometry.border_top;
		else
			return auto_height;
	}

	float StyleBorderRenderer::get_bottom_grid(float image_area_height, float auto_height) const
	{
		StyleValue border_image_width = style.computed_value("border-image-width-bottom");

		if (border_image_width.is_percentage())
			return border_image_width.number * image_area_height / 100.0f;
		else if (border_image_width.is_number())
			return border_image_width.number * geometry.border_bottom;
		else
			return auto_height;
	}

	int StyleBorderRenderer::get_left_slice_value(int image_width) const
	{
		StyleValue border_image_slice = style.computed_value("border-image-slice-left");

		int v = 0;
		if (border_image_slice.is_percentage())
			v = (int)std::round(border_image_slice.number * image_width / 100.0f);
		else
			v = (int)std::round(border_image_slice.number);
		return max(0, min(image_width, v));
	}

	int StyleBorderRenderer::get_right_slice_value(int image_width) const
	{
		StyleValue border_image_slice = style.computed_value("border-image-slice-right");

		int v = 0;
		if (border_image_slice.is_percentage())
			v = (int)std::round(border_image_slice.number * image_width / 100.0f);
		else
			v = (int)std::round(border_image_slice.number);
		return max(0, min(image_width, v));
	}

	int StyleBorderRenderer::get_top_slice_value(int image_height) const
	{
		StyleValue border_image_slice = style.computed_value("border-image-slice-top");

		int v = 0;
		if (border_image_slice.is_percentage())
			v = (int)std::round(border_image_slice.number * image_height / 100.0f);
		else
			v = (int)std::round(border_image_slice.number);
		return max(0, min(image_height, v));
	}

	int StyleBorderRenderer::get_bottom_slice_value(int image_height) const
	{
		StyleValue border_image_slice = style.computed_value("border-image-slice-bottom");

		int v = 0;
		if (border_image_slice.is_percentage())
			v = (int)std::round(border_image_slice.number * image_height / 100.0f);
		else
			v = (int)std::round(border_image_slice.number);
		return max(0, min(image_height, v));
	}

	float StyleBorderRenderer::get_horizontal_radius(const StyleValue &border_radius) const
	{
		if (border_radius.is_length())
			return border_radius.number;
		else if (border_radius.is_percentage())
			return border_radius.number * geometry.border_box().get_width() / 100.0f;
		else
			return 0.0f;
	}

	float StyleBorderRenderer::get_vertical_radius(const StyleValue &border_radius) const
	{
		if (border_radius.is_length())
			return border_radius.number;
		else if (border_radius.is_percentage())
			return border_radius.number * geometry.border_box().get_height() / 100.0f;
		else
			return 0.0f;
	}

	Colorf StyleBorderRenderer::get_light_color(const StyleValue &border_color) const
	{
		Colorf light = border_color.color;
		light.r = min(1.0f, light.r * 1.2f);
		light.g = min(1.0f, light.g * 1.2f);
		light.b = min(1.0f, light.b * 1.2f);
		return light;
	}

	Colorf StyleBorderRenderer::get_dark_color(const StyleValue &border_color) const
	{
		Colorf dark = border_color.color;
		dark.r *= 0.8f;
		dark.g *= 0.8f;
		dark.b *= 0.8f;
		return dark;
	}
}
