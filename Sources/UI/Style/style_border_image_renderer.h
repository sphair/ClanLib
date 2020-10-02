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

#include <memory>

namespace clan
{
	class Canvas;
	class Image;
	class Colorf;
	class Rectf;
	class StyleCascade;
	class StyleGetValue;
	class ViewGeometry;

	class StyleBorderImageRenderer
	{
	public:
		StyleBorderImageRenderer(Canvas &canvas, const ViewGeometry &geometry, const StyleCascade &style);
		void render();

	private:
		struct TileRepeatInfo
		{
			float start = 0.0f;
			float width = 0.0f;
			float space = 0.0f;
			int count = 1;
		};
		
		TileRepeatInfo repeat_info(float x, float w, int sw, const StyleGetValue &repeat_x);
		
		void draw_area(const Image &image, float x, float y, float w, float h, int sx, int sy, int sw, int sh, const StyleGetValue &repeat_x, const StyleGetValue &repeat_y);
		Rectf get_border_image_area() const;
		float get_left_grid(float image_area_width, float auto_width) const;
		float get_right_grid(float image_area_width, float auto_width) const;
		float get_top_grid(float image_area_height, float auto_height) const;
		float get_bottom_grid(float image_area_height, float auto_height) const;
		int get_left_slice_value(int image_width) const;
		int get_right_slice_value(int image_width) const;
		int get_top_slice_value(int image_height) const;
		int get_bottom_slice_value(int image_height) const;

		Canvas &canvas;
		const ViewGeometry &geometry;
		const StyleCascade &style;
	};
}
