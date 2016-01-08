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

#include <array>

namespace clan
{
	class Canvas;
	class Image;
	class Path;
	class Colorf;
	class Pointf;
	class BrushGradientStop;
	class StyleCascade;
	class StyleGetValue;
	class ViewGeometry;

	class StyleBackgroundRenderer
	{
	public:
		StyleBackgroundRenderer(Canvas &canvas, const ViewGeometry &geometry, const StyleCascade &style);
		void render_background();
		void render_border();

	private:
		void render_box_shadow();
		void render_background_image(const StyleGetValue &layer_image, int index);
		void render_background_linear_gradient(int index);
		void render_background_radial_gradient(int index);
		void render_background_repeating_linear_gradient(int index);
		void render_background_repeating_radial_gradient(int index);

		float get_start_x(int index, const Rectf &clip_box, const Rectf &origin_box, const Sizef &image_size);
		float get_start_y(int index, const Rectf &clip_box, const Rectf &origin_box, const Sizef &image_size);
		Sizef get_image_size(int index, Image &image, Rectf origin_box);
		Rectf get_clip_box(int index);
		Rectf get_origin_box(int index);
		StyleGetValue get_layer_clip(int index);
		StyleGetValue get_layer_origin(int index);
		StyleGetValue get_layer_size_x(int index);
		StyleGetValue get_layer_size_y(int index);
		StyleGetValue get_layer_position_x(int index);
		StyleGetValue get_layer_position_y(int index);
		StyleGetValue get_layer_attachment(int index);
		StyleGetValue get_layer_repeat_x(int index);
		StyleGetValue get_layer_repeat_y(int index);

		float get_horizontal_radius(const StyleGetValue &border_radius) const;
		float get_vertical_radius(const StyleGetValue &border_radius) const;
		Colorf get_light_color(const StyleGetValue &border_color) const;
		Colorf get_dark_color(const StyleGetValue &border_color) const;

		std::array<Pointf, 2 * 4> get_border_points();
		std::array<Pointf, 2 * 4> get_padding_points(const std::array<Pointf, 2 * 4> &border_points);

		static Path get_border_area_path(const std::array<Pointf, 2 * 4> &border_points);
		static Path get_border_stroke_path(const std::array<Pointf, 2 * 4> &border_points, const std::array<Pointf, 2 * 4> &padding_points);

		static std::vector<BrushGradientStop> shadow_blur_stops(const Colorf &shadow_color, float shadow_blur_radius, float start_t);
		static float mix(float a, float b, float t);

		Canvas &canvas;
		const ViewGeometry &geometry;
		const StyleCascade &style;

		//Rectf initial_containing_box;
		//bool is_root = false;
	};

}
