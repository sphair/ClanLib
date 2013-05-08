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

#include "CSSLayout/precomp.h"
#include "css_border_renderer.h"
#include "css_layout_graphics.h"
#include "../../css_resource_cache.h"
#include "../BoxTree/css_box_element.h"

namespace clan
{

CSSBorderRenderer::CSSBorderRenderer(CSSLayoutGraphics *graphics, CSSResourceCache *resource_cache, const CSSComputedBorder &computed_properties)
: graphics(graphics), resource_cache(resource_cache), computed_properties(computed_properties), border_left(0), border_top(0), border_right(0), border_bottom(0)
{
}

void CSSBorderRenderer::set_border_values(CSSUsedValue new_border_left, CSSUsedValue new_border_top, CSSUsedValue new_border_right, CSSUsedValue new_border_bottom)
{
	border_left = new_border_left;
	border_top = new_border_top;
	border_right = new_border_right;
	border_bottom = new_border_bottom;
}

void CSSBorderRenderer::set_border_box(Rect new_border_box)
{
	border_box = new_border_box;
}

void CSSBorderRenderer::render()
{
	if (computed_properties.border_image_source.type == CSSValueBorderImageSource::type_image)
	{
		Image &image = graphics->get_image(computed_properties.border_image_source.url);
		if (!image.is_null())
		{
			int slice_left = get_left_slice_value(computed_properties.border_image_slice, image.get_width());
			int slice_right = get_right_slice_value(computed_properties.border_image_slice, image.get_width());
			int slice_top = get_top_slice_value(computed_properties.border_image_slice, image.get_height());
			int slice_bottom = get_bottom_slice_value(computed_properties.border_image_slice, image.get_height());
			bool fill_center = computed_properties.border_image_slice.fill_center;

			Rect border_image_area = get_border_image_area();

			int grid_left = get_left_grid(computed_properties.border_image_width, border_image_area.get_width(), slice_left);
			int grid_right = get_right_grid(computed_properties.border_image_width, border_image_area.get_width(), slice_right);
			int grid_top = get_top_grid(computed_properties.border_image_width, border_image_area.get_height(), slice_top);
			int grid_bottom = get_bottom_grid(computed_properties.border_image_width, border_image_area.get_height(), slice_bottom);

			int x[4] = { border_image_area.left, border_image_area.left + grid_left, border_image_area.right - grid_right, border_image_area.right };
			int y[4] = { border_image_area.top, border_image_area.top + grid_top, border_image_area.bottom - grid_bottom, border_image_area.bottom };
			int sx[4] = { 0, slice_left, image.get_width()-slice_right, image.get_width() };
			int sy[4] = { 0, slice_top, image.get_height()-slice_bottom, image.get_height() };

			for (int yy = 0; yy < 3; yy++)
			{
				for (int xx = 0; xx < 3; xx++)
				{
					if ((xx != 1 && yy != 1) || fill_center)
						draw_area(image, x[xx], y[yy], x[xx+1] - x[xx], y[yy+1] - y[yy], sx[xx], sy[yy], sx[xx+1] - sx[xx], sy[yy+1] - sy[yy]);
				}
			}
		}
	}
	else
	{
		CSSUsedValue outer_radius_top_left_x = get_horizontal_radius(computed_properties.border_radius_top_left);
		CSSUsedValue outer_radius_top_left_y = get_vertical_radius(computed_properties.border_radius_top_left);
		CSSUsedValue outer_radius_top_right_x = get_horizontal_radius(computed_properties.border_radius_top_right);
		CSSUsedValue outer_radius_top_right_y = get_vertical_radius(computed_properties.border_radius_top_right);
		CSSUsedValue outer_radius_bottom_left_x = get_horizontal_radius(computed_properties.border_radius_bottom_left);
		CSSUsedValue outer_radius_bottom_left_y = get_vertical_radius(computed_properties.border_radius_bottom_left);
		CSSUsedValue outer_radius_bottom_right_x = get_horizontal_radius(computed_properties.border_radius_bottom_right);
		CSSUsedValue outer_radius_bottom_right_y = get_vertical_radius(computed_properties.border_radius_bottom_right);

		CSSUsedValue inner_radius_top_left_x = max(0.0f, outer_radius_top_left_x - border_left);
		CSSUsedValue inner_radius_top_left_y = max(0.0f, outer_radius_top_left_y - border_top);
		CSSUsedValue inner_radius_top_right_x = max(0.0f, outer_radius_top_right_x - border_right);
		CSSUsedValue inner_radius_top_right_y = max(0.0f, outer_radius_top_right_y - border_top);
		CSSUsedValue inner_radius_bottom_left_x = max(0.0f, outer_radius_bottom_left_x - border_left);
		CSSUsedValue inner_radius_bottom_left_y = max(0.0f, outer_radius_bottom_left_y - border_bottom);
		CSSUsedValue inner_radius_bottom_right_x = max(0.0f, outer_radius_bottom_right_x - border_right);
		CSSUsedValue inner_radius_bottom_right_y = max(0.0f, outer_radius_bottom_right_y - border_bottom);

		Point center_top_left(border_box.left + used_to_actual(outer_radius_top_left_x), border_box.top + used_to_actual(outer_radius_top_left_y));
		Point center_top_right(border_box.right - used_to_actual(outer_radius_top_right_x), border_box.top + used_to_actual(outer_radius_top_right_y));
		Point center_bottom_left(border_box.left + used_to_actual(outer_radius_bottom_left_x), border_box.bottom - used_to_actual(outer_radius_bottom_left_y));
		Point center_bottom_right(border_box.right - used_to_actual(outer_radius_bottom_right_x), border_box.bottom - used_to_actual(outer_radius_bottom_right_y));

		if (computed_properties.border_style_top.type == CSSValueBorderStyle::type_solid)
		{
			graphics->fill(Rect(center_top_left.x, border_box.top, center_top_right.x, border_box.top+border_top), computed_properties.border_color_top.color);
		}
		else if (computed_properties.border_style_top.type == CSSValueBorderStyle::type_dotted)
		{
			graphics->dot_horizontal(Rect(center_top_left.x, border_box.top, center_top_right.x, border_box.top+border_top), computed_properties.border_color_top.color);
		}
		else if (computed_properties.border_style_top.type == CSSValueBorderStyle::type_dashed)
		{
			graphics->dash_horizontal(Rect(center_top_left.x, border_box.top, center_top_right.x, border_box.top+border_top), computed_properties.border_color_top.color);
		}
		else if (computed_properties.border_style_top.type == CSSValueBorderStyle::type_double)
		{
			graphics->fill(Rect(center_top_left.x, border_box.top, center_top_right.x, border_box.top+border_top/3), computed_properties.border_color_top.color);
			graphics->fill(Rect(center_top_left.x, border_box.top+border_top*2/3, center_top_right.x, border_box.top+border_top), computed_properties.border_color_top.color);
		}
		else if (computed_properties.border_style_top.type == CSSValueBorderStyle::type_groove)
		{
			graphics->fill(Rect(center_top_left.x, border_box.top, center_top_right.x, border_box.top+border_top/2), get_dark_color(computed_properties.border_color_top));
			graphics->fill(Rect(center_top_left.x, border_box.top+border_top/2, center_top_right.x, border_box.top+border_top), get_light_color(computed_properties.border_color_top));
		}
		else if (computed_properties.border_style_top.type == CSSValueBorderStyle::type_ridge)
		{
			graphics->fill(Rect(center_top_left.x, border_box.top, center_top_right.x, border_box.top+border_top/2), get_light_color(computed_properties.border_color_top));
			graphics->fill(Rect(center_top_left.x, border_box.top+border_top/2, center_top_right.x, border_box.top+border_top), get_dark_color(computed_properties.border_color_top));
		}
		else if (computed_properties.border_style_top.type == CSSValueBorderStyle::type_inset)
		{
			graphics->fill(Rect(center_top_left.x, border_box.top, center_top_right.x, border_box.top+border_top), get_dark_color(computed_properties.border_color_top));
		}
		else if (computed_properties.border_style_top.type == CSSValueBorderStyle::type_outset)
		{
			graphics->fill(Rect(center_top_left.x, border_box.top, center_top_right.x, border_box.top+border_top), get_light_color(computed_properties.border_color_top));
		}

		if (computed_properties.border_style_bottom.type == CSSValueBorderStyle::type_solid)
		{
			graphics->fill(Rect(center_bottom_left.x, border_box.bottom-border_bottom, center_bottom_right.x, border_box.bottom), computed_properties.border_color_bottom.color);
		}
		else if (computed_properties.border_style_bottom.type == CSSValueBorderStyle::type_dotted)
		{
			graphics->dot_horizontal(Rect(center_bottom_left.x, border_box.bottom-border_bottom, center_bottom_right.x, border_box.bottom), computed_properties.border_color_bottom.color);
		}
		else if (computed_properties.border_style_bottom.type == CSSValueBorderStyle::type_dashed)
		{
			graphics->dash_horizontal(Rect(center_bottom_left.x, border_box.bottom-border_bottom, center_bottom_right.x, border_box.bottom), computed_properties.border_color_bottom.color);
		}
		else if (computed_properties.border_style_bottom.type == CSSValueBorderStyle::type_double)
		{
			graphics->fill(Rect(center_bottom_left.x, border_box.bottom-border_bottom, center_bottom_right.x, border_box.bottom-border_bottom*2/3), computed_properties.border_color_bottom.color);
			graphics->fill(Rect(center_bottom_left.x, border_box.bottom-border_bottom*1/3, center_bottom_right.x, border_box.bottom), computed_properties.border_color_bottom.color);
		}
		else if (computed_properties.border_style_bottom.type == CSSValueBorderStyle::type_groove)
		{
			graphics->fill(Rect(center_bottom_left.x, border_box.bottom-border_bottom, center_bottom_right.x, border_box.bottom-border_bottom/2), get_dark_color(computed_properties.border_color_bottom));
			graphics->fill(Rect(center_bottom_left.x, border_box.bottom-border_bottom/2, center_bottom_right.x, border_box.bottom), get_light_color(computed_properties.border_color_bottom));
		}
		else if (computed_properties.border_style_bottom.type == CSSValueBorderStyle::type_ridge)
		{
			graphics->fill(Rect(center_bottom_left.x, border_box.bottom-border_bottom, center_bottom_right.x, border_box.bottom-border_bottom/2), get_light_color(computed_properties.border_color_bottom));
			graphics->fill(Rect(center_bottom_left.x, border_box.bottom-border_bottom/2, center_bottom_right.x, border_box.bottom), get_dark_color(computed_properties.border_color_bottom));
		}
		else if (computed_properties.border_style_bottom.type == CSSValueBorderStyle::type_inset)
		{
			graphics->fill(Rect(center_bottom_left.x, border_box.bottom-border_bottom, center_bottom_right.x, border_box.bottom), get_light_color(computed_properties.border_color_bottom));
		}
		else if (computed_properties.border_style_bottom.type == CSSValueBorderStyle::type_outset)
		{
			graphics->fill(Rect(center_bottom_left.x, border_box.bottom-border_bottom, center_bottom_right.x, border_box.bottom), get_dark_color(computed_properties.border_color_bottom));
		}

		if (computed_properties.border_style_left.type == CSSValueBorderStyle::type_solid)
		{
			graphics->fill(Rect(border_box.left, center_top_left.y, border_box.left+border_left, center_bottom_left.y), computed_properties.border_color_left.color);
		}
		else if (computed_properties.border_style_left.type == CSSValueBorderStyle::type_dotted)
		{
			graphics->dot_vertical(Rect(border_box.left, center_top_left.y, border_box.left+border_left, center_bottom_left.y), computed_properties.border_color_left.color);
		}
		else if (computed_properties.border_style_left.type == CSSValueBorderStyle::type_dashed)
		{
			graphics->dash_vertical(Rect(border_box.left, center_top_left.y, border_box.left+border_left, center_bottom_left.y), computed_properties.border_color_left.color);
		}
		else if (computed_properties.border_style_left.type == CSSValueBorderStyle::type_double)
		{
			graphics->fill(Rect(border_box.left, center_top_left.y, border_box.left+border_left/3, center_bottom_left.y), computed_properties.border_color_left.color);
			graphics->fill(Rect(border_box.left+border_left*2/3, center_top_left.y, border_box.left+border_left, center_bottom_left.y), computed_properties.border_color_left.color);
		}
		else if (computed_properties.border_style_left.type == CSSValueBorderStyle::type_groove)
		{
			graphics->fill(Rect(border_box.left, center_top_left.y, border_box.left+border_left/2, center_bottom_left.y), get_dark_color(computed_properties.border_color_left));
			graphics->fill(Rect(border_box.left+border_left/2, center_top_left.y, border_box.left+border_left, center_bottom_left.y), get_light_color(computed_properties.border_color_left));
		}
		else if (computed_properties.border_style_left.type == CSSValueBorderStyle::type_ridge)
		{
			graphics->fill(Rect(border_box.left, center_top_left.y, border_box.left+border_left/2, center_bottom_left.y), get_light_color(computed_properties.border_color_left));
			graphics->fill(Rect(border_box.left+border_left/2, center_top_left.y, border_box.left+border_left, center_bottom_left.y), get_dark_color(computed_properties.border_color_left));
		}
		else if (computed_properties.border_style_left.type == CSSValueBorderStyle::type_inset)
		{
			graphics->fill(Rect(border_box.left, center_top_left.y, border_box.left+border_left, center_bottom_left.y), get_dark_color(computed_properties.border_color_left));
		}
		else if (computed_properties.border_style_left.type == CSSValueBorderStyle::type_outset)
		{
			graphics->fill(Rect(border_box.left, center_top_left.y, border_box.left+border_left, center_bottom_left.y), get_light_color(computed_properties.border_color_left));
		}

		if (computed_properties.border_style_right.type == CSSValueBorderStyle::type_solid)
		{
			graphics->fill(Rect(border_box.right-border_right, center_top_right.y, border_box.right, center_bottom_right.y), computed_properties.border_color_right.color);
		}
		else if (computed_properties.border_style_right.type == CSSValueBorderStyle::type_dotted)
		{
			graphics->dot_vertical(Rect(border_box.right-border_right, center_top_right.y, border_box.right, center_bottom_right.y), computed_properties.border_color_right.color);
		}
		else if (computed_properties.border_style_right.type == CSSValueBorderStyle::type_dashed)
		{
			graphics->dash_vertical(Rect(border_box.right-border_right, center_top_right.y, border_box.right, center_bottom_right.y), computed_properties.border_color_right.color);
		}
		else if (computed_properties.border_style_right.type == CSSValueBorderStyle::type_double)
		{
			graphics->fill(Rect(border_box.right-border_right, center_top_right.y, border_box.right-border_right*2/3, center_bottom_right.y), computed_properties.border_color_right.color);
			graphics->fill(Rect(border_box.right-border_right/3, center_top_right.y, border_box.right, center_bottom_right.y), computed_properties.border_color_right.color);
		}
		else if (computed_properties.border_style_right.type == CSSValueBorderStyle::type_groove)
		{
			graphics->fill(Rect(border_box.right-border_right, center_top_right.y, border_box.right-border_right/2, center_bottom_right.y), get_dark_color(computed_properties.border_color_right));
			graphics->fill(Rect(border_box.right-border_right/2, center_top_right.y, border_box.right, center_bottom_right.y), get_light_color(computed_properties.border_color_right));
		}
		else if (computed_properties.border_style_right.type == CSSValueBorderStyle::type_ridge)
		{
			graphics->fill(Rect(border_box.right-border_right, center_top_right.y, border_box.right-border_right/2, center_bottom_right.y), get_light_color(computed_properties.border_color_right));
			graphics->fill(Rect(border_box.right-border_right/2, center_top_right.y, border_box.right, center_bottom_right.y), get_dark_color(computed_properties.border_color_right));
		}
		else if (computed_properties.border_style_right.type == CSSValueBorderStyle::type_inset)
		{
			graphics->fill(Rect(border_box.right-border_right, center_top_right.y, border_box.right, center_bottom_right.y), get_light_color(computed_properties.border_color_right));
		}
		else if (computed_properties.border_style_right.type == CSSValueBorderStyle::type_outset)
		{
			graphics->fill(Rect(border_box.right-border_right, center_top_right.y, border_box.right, center_bottom_right.y), get_dark_color(computed_properties.border_color_right));
		}
	}
}

void CSSBorderRenderer::draw_area(Image &image, int x, int y, int w, int h, int sx, int sy, int sw, int sh)
{
	CSSValueBorderImageRepeat::RepeatType repeat_x = computed_properties.border_image_repeat.repeat_x;
	CSSValueBorderImageRepeat::RepeatType repeat_y = computed_properties.border_image_repeat.repeat_y;

	if ( (   (repeat_x == CSSValueBorderImageRepeat::repeat_type_repeat) || (repeat_x == CSSValueBorderImageRepeat::repeat_type_stretch) ) 
		&& ( (repeat_y == CSSValueBorderImageRepeat::repeat_type_stretch) || (repeat_y == CSSValueBorderImageRepeat::repeat_type_repeat) ) )
	{
		int image_width = w;
		int image_height = h;
		if (repeat_x == CSSValueBorderImageRepeat::repeat_type_repeat)
			image_width = sw;
		if (repeat_y == CSSValueBorderImageRepeat::repeat_type_repeat)
			image_height = sh;

		// This is wrong.  It is only a temporary hack so the focus almost works for clanGUI
		// See specification http://www.w3.org/TR/css3-background/#the-border-image-repeat

		for (int dy = y; dy < (h+y); dy+=image_height)
		{
			for (int dx = x; dx < (w+x); dx+=image_width)
			{
				graphics->draw_image(image, Rect(dx, dy, dx + image_width, dy + image_height), Rect(sx, sy, sx + sw, sy + sh));
			}
		}
	}
	else
	{
		graphics->draw_image(image, Rect(x, y, x + w, y + h), Rect(sx, sy, sx + sw, sy + sh));
		// Support me
	}

}

Rect CSSBorderRenderer::get_border_image_area() const
{
	Rect box = border_box;

	if (computed_properties.border_image_outset.value_left == CSSValueBorderImageOutset::value_type_length)
		box.left -= used_to_actual(computed_properties.border_image_outset.length_left.value);
	else if (computed_properties.border_image_outset.value_left == CSSValueBorderImageOutset::value_type_number)
		box.left -= used_to_actual(computed_properties.border_image_outset.number_left);

	if (computed_properties.border_image_outset.value_right == CSSValueBorderImageOutset::value_type_length)
		box.right += used_to_actual(computed_properties.border_image_outset.length_right.value);
	else if (computed_properties.border_image_outset.value_right == CSSValueBorderImageOutset::value_type_number)
		box.right += used_to_actual(computed_properties.border_image_outset.number_right);

	if (computed_properties.border_image_outset.value_top == CSSValueBorderImageOutset::value_type_length)
		box.top -= used_to_actual(computed_properties.border_image_outset.length_top.value);
	else if (computed_properties.border_image_outset.value_top == CSSValueBorderImageOutset::value_type_number)
		box.top -= used_to_actual(computed_properties.border_image_outset.number_top);

	if (computed_properties.border_image_outset.value_bottom == CSSValueBorderImageOutset::value_type_length)
		box.bottom += used_to_actual(computed_properties.border_image_outset.length_bottom.value);
	else if (computed_properties.border_image_outset.value_bottom == CSSValueBorderImageOutset::value_type_number)
		box.bottom += used_to_actual(computed_properties.border_image_outset.number_bottom);

	return box;
}

int CSSBorderRenderer::get_left_grid(const CSSValueBorderImageWidth &border_image_width, int image_area_width, int auto_width) const
{
	if (border_image_width.value_left == CSSValueBorderImageWidth::value_type_percentage)
		return used_to_actual(border_image_width.percentage_left * image_area_width);
	else if (border_image_width.value_left == CSSValueBorderImageWidth::value_type_number)
		return used_to_actual(border_image_width.number_left * border_left);
	else
		return auto_width;
}

int CSSBorderRenderer::get_right_grid(const CSSValueBorderImageWidth &border_image_width, int image_area_width, int auto_width) const
{
	if (border_image_width.value_right == CSSValueBorderImageWidth::value_type_percentage)
		return used_to_actual(border_image_width.percentage_right * image_area_width);
	else if (border_image_width.value_right == CSSValueBorderImageWidth::value_type_number)
		return used_to_actual(border_image_width.number_right * border_right);
	else
		return auto_width;
}

int CSSBorderRenderer::get_top_grid(const CSSValueBorderImageWidth &border_image_width, int image_area_height, int auto_height) const
{
	if (border_image_width.value_top == CSSValueBorderImageWidth::value_type_percentage)
		return used_to_actual(border_image_width.percentage_top * image_area_height);
	else if (border_image_width.value_top == CSSValueBorderImageWidth::value_type_number)
		return used_to_actual(border_image_width.number_top * border_top);
	else
		return auto_height;
}

int CSSBorderRenderer::get_bottom_grid(const CSSValueBorderImageWidth &border_image_width, int image_area_height, int auto_height) const
{
	if (border_image_width.value_bottom == CSSValueBorderImageWidth::value_type_percentage)
		return used_to_actual(border_image_width.percentage_bottom * image_area_height);
	else if (border_image_width.value_bottom == CSSValueBorderImageWidth::value_type_number)
		return used_to_actual(border_image_width.number_bottom * border_bottom);
	else
		return auto_height;
}

int CSSBorderRenderer::get_left_slice_value(const CSSValueBorderImageSlice &border_image_slice, int image_width) const
{
	int v = 0;
	if (border_image_slice.value_left == CSSValueBorderImageSlice::value_type_percentage)
		v = used_to_actual(border_image_slice.percentage_left * image_width);
	else
		v = used_to_actual(border_image_slice.number_left);
	return max(0, min(image_width, v));
}

int CSSBorderRenderer::get_right_slice_value(const CSSValueBorderImageSlice &border_image_slice, int image_width) const
{
	int v = 0;
	if (border_image_slice.value_right == CSSValueBorderImageSlice::value_type_percentage)
		v = used_to_actual(border_image_slice.percentage_right * image_width);
	else
		v = used_to_actual(border_image_slice.number_right);
	return max(0, min(image_width, v));
}

int CSSBorderRenderer::get_top_slice_value(const CSSValueBorderImageSlice &border_image_slice, int image_height) const
{
	int v = 0;
	if (border_image_slice.value_top == CSSValueBorderImageSlice::value_type_percentage)
		v = used_to_actual(border_image_slice.percentage_top * image_height);
	else
		v = used_to_actual(border_image_slice.number_top);
	return max(0, min(image_height, v));
}

int CSSBorderRenderer::get_bottom_slice_value(const CSSValueBorderImageSlice &border_image_slice, int image_height) const
{
	int v = 0;
	if (border_image_slice.value_bottom == CSSValueBorderImageSlice::value_type_percentage)
		v = used_to_actual(border_image_slice.percentage_bottom * image_height);
	else
		v = used_to_actual(border_image_slice.number_bottom);
	return max(0, min(image_height, v));
}

CSSUsedValue CSSBorderRenderer::get_horizontal_radius(const CSSValueBorderRadius &border_radius) const
{
	if (border_radius.value_type1 == CSSValueBorderRadius::value_type_length)
		return border_radius.length1.value;
	else if (border_radius.value_type1 == CSSValueBorderRadius::value_type_percentage)
		return border_radius.percentage1 * border_box.get_width() / 100.0f;
	else
		return 0.0f;
}

CSSUsedValue CSSBorderRenderer::get_vertical_radius(const CSSValueBorderRadius &border_radius) const
{
	if (border_radius.value_type2 == CSSValueBorderRadius::value_type_length)
		return border_radius.length2.value;
	else if (border_radius.value_type2 == CSSValueBorderRadius::value_type_percentage)
		return border_radius.percentage2 * border_box.get_height() / 100.0f;
	else
		return 0.0f;
}

Colorf CSSBorderRenderer::get_light_color(const CSSValueBorderColor &border_color) const
{
	Colorf light = border_color.color;
	light.r = min(1.0f, light.r * 1.2f);
	light.g = min(1.0f, light.g * 1.2f);
	light.b = min(1.0f, light.b * 1.2f);
	return light;
}

Colorf CSSBorderRenderer::get_dark_color(const CSSValueBorderColor &border_color) const
{
	Colorf dark = border_color.color;
	dark.r *= 0.8f;
	dark.g *= 0.8f;
	dark.b *= 0.8f;
	return dark;
}

}
