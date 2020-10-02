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
#include "style_border_image_renderer.h"
#include "API/UI/View/view_geometry.h"
#include "API/UI/Style/style.h"
#include "API/UI/Style/style_cascade.h"
#include "API/UI/Style/style_get_value.h"
#include "API/UI/UIThread/ui_thread.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/2D/image.h"
#include <algorithm>

namespace clan
{
	StyleBorderImageRenderer::StyleBorderImageRenderer(Canvas &canvas, const ViewGeometry &geometry, const StyleCascade &style) : canvas(canvas), geometry(geometry), style(style)
	{
	}

	void StyleBorderImageRenderer::render()
	{
		if (!style.computed_value("border-image-source").is_url())
			return;

		Image image = Image::resource(canvas, style.computed_value("border-image-source").text(), UIThread::get_resources());
		if (image)
		{
			int slice_left = get_left_slice_value(image.get_width());
			int slice_right = get_right_slice_value(image.get_width());
			int slice_top = get_top_slice_value(image.get_height());
			int slice_bottom = get_bottom_slice_value(image.get_height());
			bool fill_center = style.computed_value("border-image-slice-center").is_keyword("fill");

			Rectf border_image_area = get_border_image_area();

			float grid_left = get_left_grid(border_image_area.get_width(), slice_left);
			float grid_right = get_right_grid(border_image_area.get_width(), slice_right);
			float grid_top = get_top_grid(border_image_area.get_height(), slice_top);
			float grid_bottom = get_bottom_grid(border_image_area.get_height(), slice_bottom);

			float x[4] = { border_image_area.left, border_image_area.left + grid_left, border_image_area.right - grid_right, border_image_area.right };
			float y[4] = { border_image_area.top, border_image_area.top + grid_top, border_image_area.bottom - grid_bottom, border_image_area.bottom };
			int sx[4] = { 0, slice_left, (int)image.get_width() - slice_right, (int)image.get_width() };
			int sy[4] = { 0, slice_top, (int)image.get_height() - slice_bottom, (int)image.get_height() };
			
			StyleGetValue repeat_x = style.computed_value("border-image-repeat-x");
			StyleGetValue repeat_y = style.computed_value("border-image-repeat-y");

			for (int yy = 0; yy < 3; yy++)
			{
				for (int xx = 0; xx < 3; xx++)
				{
					if ((xx != 1 && yy != 1) || fill_center)
						draw_area(image, x[xx], y[yy], x[xx + 1] - x[xx], y[yy + 1] - y[yy], sx[xx], sy[yy], sx[xx + 1] - sx[xx], sy[yy + 1] - sy[yy], repeat_x, repeat_y);
				}
			}
		}
	}
	
	StyleBorderImageRenderer::TileRepeatInfo StyleBorderImageRenderer::repeat_info(float x, float w, int sw, const StyleGetValue &repeat)
	{
		TileRepeatInfo info;
		
		if (sw <= 0)
			return info;
		
		if (repeat.is_keyword("repeat"))
		{
			info.count = (int)std::ceil(w / sw);
			info.start = x + (w - info.count * sw) * 0.5f;
			info.width = sw;
		}
		else if (repeat.is_keyword("stretch"))
		{
			info.start = x;
			info.width = w;
			info.count = 1;
		}
		else if (repeat.is_keyword("round"))
		{
			info.start = x;
			info.count = std::max((int)std::round(w / sw), 1);
			info.width = sw * w / (sw * info.count);
		}
		else if (repeat.is_keyword("space"))
		{
			info.start = x;
			info.width = sw;
			info.count = (int)std::floor(w / sw);
			if (info.count > 1)
				info.space = w / (info.count - 1) - sw;
			else if (info.count == 1)
				info.start = x + (w - info.count * sw) * 0.5f;
		}
		
		return info;
	}

	void StyleBorderImageRenderer::draw_area(const Image &image, float x, float y, float w, float h, int sx, int sy, int sw, int sh, const StyleGetValue &repeat_x, const StyleGetValue &repeat_y)
	{
		TileRepeatInfo tile_x = repeat_info(x, w, sw, repeat_x);
		TileRepeatInfo tile_y = repeat_info(y, h, sh, repeat_y);
		
		Rectf clip = Rectf::xywh(x, y, w, h);
		Rectf src = Rectf::xywh(sx, sy, sw, sh);
		
		for (int yy = 0; yy < tile_y.count; yy++)
		{
			float top = tile_y.start + (tile_y.width + tile_y.space) * yy;
			float bottom = tile_y.start + (tile_y.width + tile_y.space) * (yy + 1) - tile_y.space;
			
			for (int xx = 0; xx < tile_x.count; xx++)
			{
				float left = tile_x.start + (tile_x.width + tile_x.space) * xx;
				float right = tile_x.start + (tile_x.width + tile_x.space) * (xx + 1) - tile_x.space;
				
				Rectf dest(left, top, right, bottom);
				
				Rectf dest_clipped = dest;
				dest_clipped.clip(clip);
				
				float tleft = (dest_clipped.left - dest.left) / dest.get_width();
				float tright = (dest_clipped.right - dest.left) / dest.get_width();
				float ttop = (dest_clipped.top - dest.top) / dest.get_height();
				float tbottom = (dest_clipped.bottom - dest.top) / dest.get_height();
				
				Rectf src_clipped(mix(src.left, src.right, tleft), mix(src.top, src.bottom, ttop), mix(src.left, src.right, tright), mix(src.top, src.bottom, tbottom));
				
				image.draw(canvas, src_clipped, dest_clipped);
			}
		}
	}

	Rectf StyleBorderImageRenderer::get_border_image_area() const
	{
		Rectf box = geometry.border_box();

		StyleGetValue outset_left = style.computed_value("border-image-outset-left");
		StyleGetValue outset_right = style.computed_value("border-image-outset-right");
		StyleGetValue outset_top = style.computed_value("border-image-outset-top");
		StyleGetValue outset_bottom = style.computed_value("border-image-outset-bottom");

		if (outset_left.is_length() || outset_left.is_number())
			box.left -= outset_left.number();

		if (outset_right.is_length() || outset_right.is_number())
			box.right += outset_right.number();

		if (outset_top.is_length() || outset_top.is_number())
			box.top -= outset_top.number();

		if (outset_bottom.is_length() || outset_bottom.is_number())
			box.bottom += outset_bottom.number();

		return box;
	}

	float StyleBorderImageRenderer::get_left_grid(float image_area_width, float auto_width) const
	{
		StyleGetValue border_image_width = style.computed_value("border-image-width-left");

		if (border_image_width.is_percentage())
			return border_image_width.number() * image_area_width / 100.0f;
		else if (border_image_width.is_number())
			return border_image_width.number() * geometry.border_left;
		else
			return auto_width;
	}

	float StyleBorderImageRenderer::get_right_grid(float image_area_width, float auto_width) const
	{
		StyleGetValue border_image_width = style.computed_value("border-image-width-right");

		if (border_image_width.is_percentage())
			return border_image_width.number() * image_area_width / 100.0f;
		else if (border_image_width.is_number())
			return border_image_width.number() * geometry.border_right;
		else
			return auto_width;
	}

	float StyleBorderImageRenderer::get_top_grid(float image_area_height, float auto_height) const
	{
		StyleGetValue border_image_width = style.computed_value("border-image-width-top");

		if (border_image_width.is_percentage())
			return border_image_width.number() * image_area_height / 100.0f;
		else if (border_image_width.is_number())
			return border_image_width.number() * geometry.border_top;
		else
			return auto_height;
	}

	float StyleBorderImageRenderer::get_bottom_grid(float image_area_height, float auto_height) const
	{
		StyleGetValue border_image_width = style.computed_value("border-image-width-bottom");

		if (border_image_width.is_percentage())
			return border_image_width.number() * image_area_height / 100.0f;
		else if (border_image_width.is_number())
			return border_image_width.number() * geometry.border_bottom;
		else
			return auto_height;
	}

	int StyleBorderImageRenderer::get_left_slice_value(int image_width) const
	{
		StyleGetValue border_image_slice = style.computed_value("border-image-slice-left");

		int v = 0;
		if (border_image_slice.is_percentage())
			v = (int)std::round(border_image_slice.number() * image_width / 100.0f);
		else
			v = (int)std::round(border_image_slice.number());
		return max(0, min(image_width, v));
	}

	int StyleBorderImageRenderer::get_right_slice_value(int image_width) const
	{
		StyleGetValue border_image_slice = style.computed_value("border-image-slice-right");

		int v = 0;
		if (border_image_slice.is_percentage())
			v = (int)std::round(border_image_slice.number() * image_width / 100.0f);
		else
			v = (int)std::round(border_image_slice.number());
		return max(0, min(image_width, v));
	}

	int StyleBorderImageRenderer::get_top_slice_value(int image_height) const
	{
		StyleGetValue border_image_slice = style.computed_value("border-image-slice-top");

		int v = 0;
		if (border_image_slice.is_percentage())
			v = (int)std::round(border_image_slice.number() * image_height / 100.0f);
		else
			v = (int)std::round(border_image_slice.number());
		return max(0, min(image_height, v));
	}

	int StyleBorderImageRenderer::get_bottom_slice_value(int image_height) const
	{
		StyleGetValue border_image_slice = style.computed_value("border-image-slice-bottom");

		int v = 0;
		if (border_image_slice.is_percentage())
			v = (int)std::round(border_image_slice.number() * image_height / 100.0f);
		else
			v = (int)std::round(border_image_slice.number());
		return max(0, min(image_height, v));
	}
}
