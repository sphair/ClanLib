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

#include "UI/precomp.h"
#include "style_background_renderer.h"
#include "API/UI/View/view_geometry.h"
#include "API/UI/Style/style_cascade.h"
#include "API/UI/Style/style_get_value.h"
#include "API/UI/UIThread/ui_thread.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/2D/image.h"
#include "API/Display/2D/path.h"
#include "API/Display/2D/brush.h"
#include "API/UI/Image/image_source.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Math/line.h"

namespace clan
{
	StyleBackgroundRenderer::StyleBackgroundRenderer(Canvas &canvas, const ViewGeometry &geometry, const StyleCascade &style) : canvas(canvas), geometry(geometry), style(style)
	{
	}

	void StyleBackgroundRenderer::render_background()
	{
		render_box_shadow();

		int num_layers = style.array_size("background-image");

		StyleGetValue bg_color = style.computed_value("background-color");
		if (bg_color.is_color() && bg_color.color().a != 0.0f)
		{
			auto border_points = get_border_points();

			if (is_render_border_antialias_fix_required())
			{
				float delta = 1.0f;
				border_points[0] += Pointf(delta, delta);
				border_points[1] += Pointf(-delta, delta);
				border_points[2] += Pointf(-delta, delta);
				border_points[3] += Pointf(-delta, -delta);
				border_points[4] += Pointf(-delta, -delta);
				border_points[5] += Pointf(delta, -delta);
				border_points[6] += Pointf(delta, -delta);
				border_points[7] += Pointf(delta, delta);
			}

			// To do: take get_layer_clip(num_layers - 1) into account

			Path background_area = get_border_area_path(border_points);
			background_area.fill(canvas, Brush(bg_color.color()));
		}

		for (int index = num_layers - 1; index >= 0; index--)
		{
			StyleGetValue layer_image = style.computed_value("background-image[" + StringHelp::int_to_text(index) + "]");
			if (layer_image.is_keyword("none"))
				continue;

			if (layer_image.is_url())
			{
				render_background_image(layer_image, index);
			}
			else if (layer_image.is_keyword("linear-gradient"))
			{
				render_background_linear_gradient(index);
			}
			else if (layer_image.is_keyword("radial-gradient"))
			{
				render_background_radial_gradient(index);
			}
			else if (layer_image.is_keyword("repeating-linear-gradient"))
			{
				render_background_repeating_linear_gradient(index);
			}
			else if (layer_image.is_keyword("repeating-radial-gradient"))
			{
				render_background_repeating_radial_gradient(index);
			}
		}
	}

	void StyleBackgroundRenderer::render_background_image(const StyleGetValue &layer_image, int index)
	{
		Image image;

		if (layer_image.is_url())
			image = ImageSource::from_resource(layer_image.text())->image(canvas);

		if (image)
		{
			Rectf clip_box = get_clip_box(index);
			Rectf origin_box = get_origin_box(index);
			Sizef image_size = get_image_size(index, image, origin_box);

			//canvas.push_cliprect(clip_box);

			StyleGetValue repeat_x = get_layer_repeat_x(index);
			StyleGetValue repeat_y = get_layer_repeat_y(index);

			float y = get_start_y(index, clip_box, origin_box, image_size);
			while (true)
			{
				float x = get_start_x(index, clip_box, origin_box, image_size);
				while (true)
				{
					Rectf image_source(0.0f, 0.0f, image.get_size());
					Rectf image_dest(x, y, image_size);

					if (image_dest.left < clip_box.left)
					{
						float delta = clip_box.left - image_dest.left;
						image_source.left += delta * image.get_width() / image_size.width;
						image_dest.left += delta;
					}
					if (image_dest.right > clip_box.right)
					{
						float delta = clip_box.right - image_dest.right;
						image_source.right += delta * image.get_width() / image_size.width;
						image_dest.right += delta;
					}
					if (image_dest.top < clip_box.top)
					{
						float delta = clip_box.top - image_dest.top;
						image_source.top += delta * image.get_height() / image_size.height;
						image_dest.top += delta;
					}
					if (image_dest.bottom > clip_box.bottom)
					{
						float delta = clip_box.bottom - image_dest.bottom;
						image_source.bottom += delta * image.get_height() / image_size.height;
						image_dest.bottom += delta;
					}

					image.draw(canvas, image_source, image_dest);

					if (repeat_x.is_keyword("no-repeat"))
					{
						break;
					}
					else if (repeat_x.is_keyword("repeat"))
					{
						x += image_size.width;
					}
					else if (repeat_x.is_keyword("space"))
					{
						x += image_size.width;
					}
					else if (repeat_x.is_keyword("round"))
					{
						x += image_size.width;
					}

					if (x >= clip_box.right)
						break;
				}

				if (repeat_y.is_keyword("no-repeat"))
				{
					break;
				}
				else if (repeat_y.is_keyword("repeat"))
				{
					y += image_size.height;
				}
				else if (repeat_y.is_keyword("space"))
				{
					y += image_size.height;
				}
				else if (repeat_y.is_keyword("round"))
				{
					y += image_size.height;
				}

				if (y >= clip_box.bottom)
					break;
			}

			//canvas.pop_cliprect();
		}
	}

	void StyleBackgroundRenderer::render_background_linear_gradient(int index)
	{
		std::string prop_name = "background-image[" + StringHelp::int_to_text(index) + "]";

		auto prop_angle = style.computed_value(prop_name + ".angle");
		int num_stops = style.array_size(prop_name + ".stop");
		if (num_stops <= 0)
			return;

		Rectf clip_box = get_clip_box(index);
		Pointf center = clip_box.get_center();

		Brush brush;
		brush.type = BrushType::linear;

		if (prop_angle.is_angle())
		{
			float angle = std::fmod(prop_angle.number(), 2 * PI);
			if (angle < 0.0f)
				angle += 2 * PI;

			float dx = std::sin(angle);
			float dy = -std::cos(angle);

			Pointf corner1, corner2;

			int corner_index = (int)(angle * 2 / PI);
			switch (corner_index)
			{
			default:
			case 0:
				corner1 = clip_box.get_bottom_left();
				corner2 = clip_box.get_top_right();
				break;
			case 1:
				corner1 = clip_box.get_top_left();
				corner2 = clip_box.get_bottom_right();
				break;
			case 2:
				corner1 = clip_box.get_top_right();
				corner2 = clip_box.get_bottom_left();
				break;
			case 3:
				corner1 = clip_box.get_bottom_right();
				corner2 = clip_box.get_top_left();
				break;
			}

			bool intersect = false;
			brush.start_point = Line2f(center, center + Vec2f(dx, dy)).get_intersection(Line2f(corner1, corner1 + Vec2f(-dy, dx)), intersect);
			brush.end_point = Line2f(center, center + Vec2f(dx, dy)).get_intersection(Line2f(corner2, corner2 + Vec2f(-dy, dx)), intersect);
		}
		else if (prop_angle.is_keyword("top-left"))
		{
			brush.end_point = Pointf(clip_box.right, clip_box.bottom);
			brush.start_point = Pointf(clip_box.left, clip_box.top);
		}
		else if (prop_angle.is_keyword("top-right"))
		{
			brush.end_point = Pointf(clip_box.left, clip_box.bottom);
			brush.start_point = Pointf(clip_box.right, clip_box.top);
		}
		else if (prop_angle.is_keyword("bottom-left"))
		{
			brush.start_point = Pointf(clip_box.right, clip_box.top);
			brush.end_point = Pointf(clip_box.left, clip_box.bottom);
		}
		else if (prop_angle.is_keyword("bottom-right"))
		{
			brush.start_point = Pointf(clip_box.left, clip_box.top);
			brush.end_point = Pointf(clip_box.right, clip_box.bottom);
		}
		else
		{
			return;
		}

		float gradient_length = (brush.end_point - brush.start_point).length();
		if (gradient_length <= 0.0f)
			return;

		Path border_area_path = get_border_area_path(get_border_points());

		float last_position = 0.0f;
		for (int stop_index = 0; stop_index < num_stops; stop_index++)
		{
			std::string stop_prop_name = prop_name + ".stop[" + StringHelp::int_to_text(stop_index) + "]";

			auto prop_color = style.computed_value(stop_prop_name);
			auto prop_position = style.computed_value(stop_prop_name + ".get_position");

			float position = 0.0f;
			if (prop_position.is_number())
				position = prop_position.number();
			else if (prop_position.is_percentage())
				position = prop_position.number() / 100.0f;
			else if (prop_position.is_length())
				position = prop_position.number() / gradient_length;
			else if (stop_index == 0)
				position = 0.0f;
			else if (stop_index + 1 == num_stops)
				position = 1.0f;

			// to do: support evenly spread positions if position isn't specified for stops

			if (stop_index > 0)
				position = std::max(position, last_position);

			last_position = position;

			brush.stops.push_back(BrushGradientStop(prop_color.color(), position));
		}

		border_area_path.fill(canvas, brush);
	}

	void StyleBackgroundRenderer::render_background_radial_gradient(int index)
	{

	}

	void StyleBackgroundRenderer::render_background_repeating_linear_gradient(int index)
	{

	}

	void StyleBackgroundRenderer::render_background_repeating_radial_gradient(int index)
	{

	}

	void StyleBackgroundRenderer::render_border()
	{
		int num_layers = style.array_size("background-image");
		if (!get_layer_clip(num_layers - 1).is_keyword("border-box"))
			return;

		StyleGetValue style_top = style.computed_value("border-top-style");
		if (style_top.is_keyword("solid"))
		{
			Colorf color = style.computed_value("border-top-color").color();
			if (color.a > 0.0f)
			{
				auto border_points = get_border_points();
				auto padding_points = get_padding_points(border_points);
				Path border_path = get_border_stroke_path(border_points, padding_points);
				border_path.fill(canvas, Brush(color));
			}
		}
	}

	bool StyleBackgroundRenderer::is_render_border_antialias_fix_required()
	{
		StyleGetValue style_top = style.computed_value("border-top-style");
		if (style_top.is_keyword("solid"))
		{
			Colorf color = style.computed_value("border-top-color").color();
			if (color.a > 0.5f)
				return true;
		}
		return false;
	}

	float StyleBackgroundRenderer::get_start_x(int index, const Rectf &clip_box, const Rectf &origin_box, const Sizef &image_size)
	{
		float x;
		if (get_layer_repeat_x(index).is_keyword("space") && image_size.width * 2 > origin_box.get_width())
		{
			x = origin_box.left;
		}
		else
		{
			x = origin_box.left;
			StyleGetValue pos_x = get_layer_position_x(index);
			if (pos_x.is_keyword("left"))
				x = origin_box.left;
			else if (pos_x.is_keyword("center"))
				x = origin_box.left + (origin_box.get_width() - image_size.width) / 2;
			else if (pos_x.is_keyword("right"))
				x = origin_box.right - image_size.width;
			else if (pos_x.is_percentage())
				x = origin_box.left + (origin_box.get_width() - image_size.width) * pos_x.number() / 100.0f;
			else if (pos_x.is_length())
				x = origin_box.left + pos_x.number();
		}

		StyleGetValue repeat_x = get_layer_repeat_x(index);
		if (repeat_x.is_keyword("repeat") || repeat_x.is_keyword("space"))
		{
			if (x > clip_box.left)
				x = clip_box.left - std::fmod(image_size.width - (x - clip_box.left), image_size.width);
		}

		return x;
	}

	float StyleBackgroundRenderer::get_start_y(int index, const Rectf &clip_box, const Rectf &origin_box, const Sizef &image_size)
	{
		float y;
		if (get_layer_repeat_y(index).is_keyword("space") && image_size.height * 2 > origin_box.get_height())
		{
			y = origin_box.top;
		}
		else
		{
			y = origin_box.top;
			StyleGetValue pos_y = get_layer_position_y(index);
			if (pos_y.is_keyword("top"))
				y = origin_box.top;
			else if (pos_y.is_keyword("center"))
				y = origin_box.top + (origin_box.get_height() - image_size.height) / 2;
			else if (pos_y.is_keyword("bottom"))
				y = origin_box.bottom - image_size.height;
			else if (pos_y.is_percentage())
				y = origin_box.top + (origin_box.get_height() - image_size.height) * pos_y.number() / 100.0f;
			else if (pos_y.is_length())
				y = origin_box.top + pos_y.number();
		}

		StyleGetValue repeat_y = get_layer_repeat_y(index);
		if (repeat_y.is_keyword("repeat") || repeat_y.is_keyword("space"))
		{
			if (y > clip_box.top)
				y = clip_box.top - std::fmod(image_size.height - (y - clip_box.top), image_size.height);
		}

		return y;
	}

	Sizef StyleBackgroundRenderer::get_image_size(int index, const Image &image, Rectf origin_box)
	{
		Sizef size;
		StyleGetValue size_x = get_layer_size_x(index);
		StyleGetValue size_y = get_layer_size_y(index);
		
		if (size_x.is_keyword("contain"))
		{
			if (origin_box.get_height()*image.get_width() / image.get_height() <= origin_box.get_width())
				size = Sizef(origin_box.get_height()*image.get_width() / image.get_height(), origin_box.get_height());
			else
				size = Sizef(origin_box.get_width(), origin_box.get_width()*image.get_height() / image.get_width());
		}
		else if (size_x.is_keyword("cover"))
		{
			if (origin_box.get_height()*image.get_width() / image.get_height() >= origin_box.get_width())
				size = Sizef(origin_box.get_height()*image.get_width() / image.get_height(), origin_box.get_height());
			else
				size = Sizef(origin_box.get_width(), origin_box.get_width()*image.get_height() / image.get_width());
		}
		else
		{
			float width = image.get_width();
			if (size_x.is_length())
			{
				width = size_x.number();
			}
			else if (size_x.is_percentage())
			{
				width = size_x.number() * width / 100.0f;
			}

			float height = image.get_height();
			if (size_y.is_length())
			{
				height = size_y.number();
			}
			else if (size_y.is_percentage())
			{
				height = size_y.number() * height / 100.0f;
			}

			size = Sizef(width, height);
		}

		StyleGetValue repeat_x = get_layer_repeat_x(index);
		StyleGetValue repeat_y = get_layer_repeat_y(index);

		if (repeat_x.is_keyword("round"))
		{
			if (size.width != 0)
				size.width = origin_box.get_width() / (int)(origin_box.get_width() / (float)size.width + 0.5f);
		}

		if (repeat_y.is_keyword("round"))
		{
			if (size.height != 0)
				size.height = origin_box.get_height() / (int)(origin_box.get_height() / (float)size.height + 0.5f);
		}

		if (repeat_x.is_keyword("round") && size_y.is_keyword("auto"))
		{
			size.height = size.width*image.get_height() / image.get_width();
		}
		else if (repeat_y.is_keyword("round") && size_x.is_keyword("auto"))
		{
			size.width = size.height*image.get_width() / image.get_height();
		}

		return size;
	}

	Rectf StyleBackgroundRenderer::get_clip_box(int index)
	{
		//if (is_root)
		//	return initial_containing_box;

		StyleGetValue clip = get_layer_clip(index);
		if (clip.is_keyword("border-box"))
			return geometry.border_box();
		else if (clip.is_keyword("padding-box"))
			return geometry.padding_box();
		else if (clip.is_keyword("content-box"))
			return geometry.content_box();
		else
			return Rectf();
	}

	Rectf StyleBackgroundRenderer::get_origin_box(int index)
	{
		StyleGetValue attachment = get_layer_attachment(index);
		/*if (attachment.is_keyword("fixed"))
		{
			return initial_containing_box;
		}
		else*/ if (attachment.is_keyword("local") || attachment.is_keyword("scroll"))
		{
			StyleGetValue origin = get_layer_origin(index);
			if (origin.is_keyword("border-box"))
				return geometry.border_box();
			else if (origin.is_keyword("padding-box"))
				return geometry.padding_box();
			else if (origin.is_keyword("content-box"))
				return geometry.content_box();
			else
				return Rectf();
		}
		else
		{
			return Rectf();
		}
	}

	StyleGetValue StyleBackgroundRenderer::get_layer_clip(int index)
	{
		int count = style.array_size("background-clip");
		return style.computed_value("background-clip[" + StringHelp::int_to_text(index % count) + "]");
	}

	StyleGetValue StyleBackgroundRenderer::get_layer_origin(int index)
	{
		int count = style.array_size("background-origin");
		return style.computed_value("background-origin[" + StringHelp::int_to_text(index % count) + "]");
	}

	StyleGetValue StyleBackgroundRenderer::get_layer_size_x(int index)
	{
		int count = style.array_size("background-size-x");
		return style.computed_value("background-size-x[" + StringHelp::int_to_text(index % count) + "]");
	}

	StyleGetValue StyleBackgroundRenderer::get_layer_size_y(int index)
	{
		int count = style.array_size("background-size-y");
		return style.computed_value("background-size-y[" + StringHelp::int_to_text(index % count) + "]");
	}

	StyleGetValue StyleBackgroundRenderer::get_layer_position_x(int index)
	{
		int count = style.array_size("background-position-x");
		return style.computed_value("background-position-x[" + StringHelp::int_to_text(index % count) + "]");
	}

	StyleGetValue StyleBackgroundRenderer::get_layer_position_y(int index)
	{
		int count = style.array_size("background-position-y");
		return style.computed_value("background-position-y[" + StringHelp::int_to_text(index % count) + "]");
	}

	StyleGetValue StyleBackgroundRenderer::get_layer_attachment(int index)
	{
		int count = style.array_size("background-attachment");
		return style.computed_value("background-attachment[" + StringHelp::int_to_text(index % count) + "]");
	}

	StyleGetValue StyleBackgroundRenderer::get_layer_repeat_x(int index)
	{
		int count = style.array_size("background-repeat-x");
		return style.computed_value("background-repeat-x[" + StringHelp::int_to_text(index % count) + "]");
	}

	StyleGetValue StyleBackgroundRenderer::get_layer_repeat_y(int index)
	{
		int count = style.array_size("background-repeat-y");
		return style.computed_value("background-repeat-y[" + StringHelp::int_to_text(index % count) + "]");
	}

	std::array<Pointf, 2 * 4> StyleBackgroundRenderer::get_border_points()
	{
		float top_left_x = get_horizontal_radius(style.computed_value("border-top-left-radius-x"));
		float top_left_y = get_vertical_radius(style.computed_value("border-top-left-radius-y"));
		float top_right_x = get_horizontal_radius(style.computed_value("border-top-right-radius-x"));
		float top_right_y = get_vertical_radius(style.computed_value("border-top-right-radius-y"));
		float bottom_left_x = get_horizontal_radius(style.computed_value("border-bottom-left-radius-x"));
		float bottom_left_y = get_vertical_radius(style.computed_value("border-bottom-left-radius-y"));
		float bottom_right_x = get_horizontal_radius(style.computed_value("border-bottom-right-radius-x"));
		float bottom_right_y = get_vertical_radius(style.computed_value("border-bottom-right-radius-y"));

		Rectf border_box = geometry.border_box();

		std::array<Pointf, 2 * 4> border_points;
		border_points[0] = Pointf(border_box.left + top_left_x, border_box.top);
		border_points[1] = Pointf(border_box.right - top_right_x, border_box.top);
		border_points[2] = Pointf(border_box.right, border_box.top + top_right_y);
		border_points[3] = Pointf(border_box.right, border_box.bottom - bottom_right_y);
		border_points[4] = Pointf(border_box.right - bottom_right_x, border_box.bottom);
		border_points[5] = Pointf(border_box.left + bottom_left_x, border_box.bottom);
		border_points[6] = Pointf(border_box.left, border_box.bottom - bottom_left_y);
		border_points[7] = Pointf(border_box.left, border_box.top + top_left_y);
		return border_points;
	}

	std::array<Pointf, 2 * 4> StyleBackgroundRenderer::get_padding_points(const std::array<Pointf, 2 * 4> &border_points)
	{
		Rectf padding_box = geometry.padding_box();

		std::array<Pointf, 2 * 4> padding_points;
		padding_points[0] = Pointf(padding_box.left, padding_box.top);
		padding_points[1] = Pointf(padding_box.right, padding_box.top);
		padding_points[2] = Pointf(padding_box.right, padding_box.top);
		padding_points[3] = Pointf(padding_box.right, padding_box.bottom);
		padding_points[4] = Pointf(padding_box.right, padding_box.bottom);
		padding_points[5] = Pointf(padding_box.left, padding_box.bottom);
		padding_points[6] = Pointf(padding_box.left, padding_box.bottom);
		padding_points[7] = Pointf(padding_box.left, padding_box.top);

		padding_points[0].x = clan::max(padding_points[0].x, border_points[0].x);
		padding_points[0].y = clan::max(padding_points[0].y, border_points[0].y);
		padding_points[1].x = clan::min(padding_points[1].x, border_points[1].x);
		padding_points[1].y = clan::max(padding_points[1].y, border_points[1].y);
		padding_points[2].x = clan::min(padding_points[2].x, border_points[2].x);
		padding_points[2].y = clan::max(padding_points[2].y, border_points[2].y);
		padding_points[3].x = clan::min(padding_points[3].x, border_points[3].x);
		padding_points[3].y = clan::min(padding_points[3].y, border_points[3].y);
		padding_points[4].x = clan::min(padding_points[4].x, border_points[4].x);
		padding_points[4].y = clan::min(padding_points[4].y, border_points[4].y);
		padding_points[5].x = clan::max(padding_points[5].x, border_points[5].x);
		padding_points[5].y = clan::min(padding_points[5].y, border_points[5].y);
		padding_points[6].x = clan::max(padding_points[6].x, border_points[6].x);
		padding_points[6].y = clan::min(padding_points[6].y, border_points[6].y);
		padding_points[7].x = clan::max(padding_points[7].x, border_points[7].x);
		padding_points[7].y = clan::max(padding_points[7].y, border_points[7].y);

		return padding_points;
	}

	Path StyleBackgroundRenderer::get_border_area_path(const std::array<Pointf, 2 * 4> &border_points)
	{
		float kappa = 0.552228474f;

		// Border area path (to be used for filling)

		Path border_area_path;

		border_area_path.move_to(border_points[0]);
		border_area_path.line_to(border_points[1]);
		if (border_points[1] != border_points[2])
		{
			border_area_path.bezier_to(
				Pointf(mix(border_points[1].x, border_points[2].x, kappa), border_points[1].y),
				Pointf(border_points[2].x, mix(border_points[1].y, border_points[2].y, kappa)),
				border_points[2]);
		}

		border_area_path.line_to(border_points[3]);
		if (border_points[3] != border_points[4])
		{
			border_area_path.bezier_to(
				Pointf(border_points[3].x, mix(border_points[3].y, border_points[4].y, kappa)),
				Pointf(mix(border_points[3].x, border_points[4].x, kappa), border_points[4].y),
				border_points[4]);
		}

		border_area_path.line_to(border_points[5]);
		if (border_points[5] != border_points[6])
		{
			border_area_path.bezier_to(
				Pointf(mix(border_points[5].x, border_points[6].x, kappa), border_points[5].y),
				Pointf(border_points[6].x, mix(border_points[5].y, border_points[6].y, kappa)),
				border_points[6]);
		}

		border_area_path.line_to(border_points[7]);
		if (border_points[7] != border_points[0])
		{
			border_area_path.bezier_to(
				Pointf(border_points[7].x, mix(border_points[7].y, border_points[0].y, kappa)),
				Pointf(mix(border_points[7].x, border_points[0].x, kappa), border_points[0].y),
				border_points[0]);
		}

		border_area_path.close();

		return border_area_path;
	}

	Path StyleBackgroundRenderer::get_border_stroke_path(const std::array<Pointf, 2 * 4> &border_points, const std::array<Pointf, 2 * 4> &padding_points)
	{
		// Border path (the path defining the actual border)

		float kappa = 0.552228474f;

		Path border_path;

		border_path.move_to(border_points[0]);
		border_path.line_to(border_points[1]);
		if (border_points[1] != border_points[2])
		{
			border_path.bezier_to(
				Pointf(mix(border_points[1].x, border_points[2].x, kappa), border_points[1].y),
				Pointf(border_points[2].x, mix(border_points[1].y, border_points[2].y, kappa)),
				border_points[2]);
		}

		border_path.line_to(border_points[3]);
		if (border_points[3] != border_points[4])
		{
			border_path.bezier_to(
				Pointf(border_points[3].x, mix(border_points[3].y, border_points[4].y, kappa)),
				Pointf(mix(border_points[3].x, border_points[4].x, kappa), border_points[4].y),
				border_points[4]);
		}

		border_path.line_to(border_points[5]);
		if (border_points[5] != border_points[6])
		{
			border_path.bezier_to(
				Pointf(mix(border_points[5].x, border_points[6].x, kappa), border_points[5].y),
				Pointf(border_points[6].x, mix(border_points[5].y, border_points[6].y, kappa)),
				border_points[6]);
		}

		border_path.line_to(border_points[7]);
		if (border_points[7] != border_points[0])
		{
			border_path.bezier_to(
				Pointf(border_points[7].x, mix(border_points[7].y, border_points[0].y, kappa)),
				Pointf(mix(border_points[7].x, border_points[0].x, kappa), border_points[0].y),
				border_points[0]);
		}

		border_path.close();

		border_path.move_to(padding_points[0]);
		border_path.line_to(padding_points[1]);
		if (padding_points[1] != padding_points[2])
		{
			border_path.bezier_to(
				Pointf(mix(padding_points[1].x, padding_points[2].x, kappa), padding_points[1].y),
				Pointf(padding_points[2].x, mix(padding_points[1].y, padding_points[2].y, kappa)),
				padding_points[2]);
		}

		border_path.line_to(padding_points[3]);
		if (padding_points[3] != padding_points[4])
		{
			border_path.bezier_to(
				Pointf(padding_points[3].x, mix(padding_points[3].y, padding_points[4].y, kappa)),
				Pointf(mix(padding_points[3].x, padding_points[4].x, kappa), padding_points[4].y),
				padding_points[4]);
		}

		border_path.line_to(padding_points[5]);
		if (padding_points[5] != padding_points[6])
		{
			border_path.bezier_to(
				Pointf(mix(padding_points[5].x, padding_points[6].x, kappa), padding_points[5].y),
				Pointf(padding_points[6].x, mix(padding_points[5].y, padding_points[6].y, kappa)),
				padding_points[6]);
		}

		border_path.line_to(padding_points[7]);
		if (padding_points[7] != padding_points[0])
		{
			border_path.bezier_to(
				Pointf(padding_points[7].x, mix(padding_points[7].y, padding_points[0].y, kappa)),
				Pointf(mix(padding_points[7].x, padding_points[0].x, kappa), padding_points[0].y),
				padding_points[0]);
		}

		border_path.close();

		return border_path;
	}

	void StyleBackgroundRenderer::render_box_shadow()
	{
		int num_shadows = style.array_size("box-shadow-style");
		if (num_shadows == 0)
			return;

		Rectf border_box = geometry.border_box();
		auto border_points = get_border_points();

		for (int index = num_shadows - 1; index >= 0; index--)
		{
			auto layer_style = style.computed_value("box-shadow-style[" + StringHelp::int_to_text(index) + "]");

			// To do: support inset

			if (!layer_style.is_keyword("outset"))
				continue;

			auto layer_color = style.computed_value("box-shadow-color[" + StringHelp::int_to_text(index) + "]");
			if (layer_color.color().a <= 0.0f)
				continue;

			auto layer_offset_x = style.computed_value("box-shadow-horizontal-offset[" + StringHelp::int_to_text(index) + "]");
			auto layer_offset_y = style.computed_value("box-shadow-vertical-offset[" + StringHelp::int_to_text(index) + "]");
			auto layer_blur_radius = style.computed_value("box-shadow-blur-radius[" + StringHelp::int_to_text(index) + "]");
			//auto layer_spread_distance = style.computed_value("box-shadow-spread-distance[" + StringHelp::int_to_text(index) + "]");

			// To do: support shadow_spread_distance

			float shadow_blur_radius = layer_blur_radius.number();
			Pointf shadow_offset(layer_offset_x.number(), layer_offset_y.number());
			Colorf shadow_color = layer_color.color();
			Colorf transparent = shadow_color;
			transparent.a = 0.0f;

			float kappa = 0.552228474f;

			float top_left_x = get_horizontal_radius(style.computed_value("border-top-left-radius-x"));
			float top_left_y = get_vertical_radius(style.computed_value("border-top-left-radius-y"));
			float top_right_x = get_horizontal_radius(style.computed_value("border-top-right-radius-x"));
			float top_right_y = get_vertical_radius(style.computed_value("border-top-right-radius-y"));
			float bottom_left_x = get_horizontal_radius(style.computed_value("border-bottom-left-radius-x"));
			float bottom_left_y = get_vertical_radius(style.computed_value("border-bottom-left-radius-y"));
			float bottom_right_x = get_horizontal_radius(style.computed_value("border-bottom-right-radius-x"));
			float bottom_right_y = get_vertical_radius(style.computed_value("border-bottom-right-radius-y"));

			if (shadow_blur_radius != 0.0f)
			{
				Path top_left;
				top_left.move_to(Pointf(border_box.left - shadow_blur_radius, border_box.top - shadow_blur_radius));
				top_left.line_to(Pointf(border_points[0].x, border_points[0].y - shadow_blur_radius));
				top_left.line_to(border_points[0]);
				if (border_points[7] != border_points[0])
				{
					top_left.bezier_to(
						Pointf(border_points[0].x, mix(border_points[0].y, border_points[7].y, kappa)),
						Pointf(mix(border_points[0].x, border_points[7].x, kappa), border_points[7].y),
						border_points[7]);
				}
				top_left.line_to(Pointf(border_points[7].x - shadow_blur_radius, border_points[7].y));
				top_left.close();

				Path top_right;
				top_right.move_to(Pointf(border_points[1].x, border_points[1].y - shadow_blur_radius));
				top_right.line_to(Pointf(border_box.right + shadow_blur_radius, border_box.top - shadow_blur_radius));
				top_right.line_to(Pointf(border_points[2].x + shadow_blur_radius, border_points[2].y));
				top_right.line_to(border_points[2]);
				if (border_points[1] != border_points[2])
				{
					top_right.bezier_to(
						Pointf(border_points[2].x, mix(border_points[2].y, border_points[1].y, kappa)),
						Pointf(mix(border_points[2].x, border_points[1].x, kappa), border_points[1].y),
						border_points[1]);
				}
				top_right.close();

				Path bottom_right;
				bottom_right.move_to(Pointf(border_box.right + shadow_blur_radius, border_box.bottom + shadow_blur_radius));
				bottom_right.line_to(Pointf(border_points[4].x, border_points[4].y + shadow_blur_radius));
				bottom_right.line_to(border_points[4]);
				if (border_points[4] != border_points[3])
				{
					bottom_right.bezier_to(
						Pointf(border_points[4].x, mix(border_points[4].y, border_points[3].y, kappa)),
						Pointf(mix(border_points[4].x, border_points[3].x, kappa), border_points[3].y),
						border_points[3]);
				}
				bottom_right.line_to(Pointf(border_points[3].x + shadow_blur_radius, border_points[3].y));
				bottom_right.close();

				Path bottom_left;
				bottom_left.move_to(Pointf(border_box.left - shadow_blur_radius, border_box.bottom + shadow_blur_radius));
				bottom_left.line_to(Pointf(border_points[6].x - shadow_blur_radius, border_points[6].y));
				bottom_left.line_to(border_points[6]);
				if (border_points[6] != border_points[5])
				{
					bottom_left.bezier_to(
						Pointf(border_points[6].x, mix(border_points[6].y, border_points[5].y, kappa)),
						Pointf(mix(border_points[6].x, border_points[5].x, kappa), border_points[5].y),
						border_points[5]);
				}
				bottom_left.line_to(Pointf(border_points[5].x, border_points[5].y + shadow_blur_radius));
				bottom_left.close();

				Brush brush_top_left;
				brush_top_left.type = BrushType::radial;
				brush_top_left.radius_x = shadow_blur_radius + top_left_x;
				brush_top_left.radius_y = shadow_blur_radius + top_left_y;
				brush_top_left.center_point = Pointf(border_box.left + top_left_x, border_box.top + top_left_y);
				brush_top_left.stops = shadow_blur_stops(shadow_color, shadow_blur_radius, top_left_x / brush_top_left.radius_x);

				Brush brush_top_right;
				brush_top_right.type = BrushType::radial;
				brush_top_right.radius_x = shadow_blur_radius + top_right_x;
				brush_top_right.radius_y = shadow_blur_radius + top_right_y;
				brush_top_right.center_point = Pointf(border_box.right - top_right_x, border_box.top + top_right_y);
				brush_top_right.stops = shadow_blur_stops(shadow_color, shadow_blur_radius, top_right_x / brush_top_right.radius_x);

				Brush brush_bottom_right;
				brush_bottom_right.type = BrushType::radial;
				brush_bottom_right.radius_x = shadow_blur_radius + bottom_right_x;
				brush_bottom_right.radius_y = shadow_blur_radius + bottom_right_y;
				brush_bottom_right.center_point = Pointf(border_box.right - bottom_right_x, border_box.bottom - bottom_right_y);
				brush_bottom_right.stops = shadow_blur_stops(shadow_color, shadow_blur_radius, bottom_right_x / brush_bottom_right.radius_x);

				Brush brush_bottom_left;
				brush_bottom_left.type = BrushType::radial;
				brush_bottom_left.radius_x = shadow_blur_radius + bottom_left_x;
				brush_bottom_left.radius_y = shadow_blur_radius + bottom_left_y;
				brush_bottom_left.center_point = Pointf(border_box.left + bottom_left_x, border_box.bottom - bottom_left_y);
				brush_bottom_left.stops = shadow_blur_stops(shadow_color, shadow_blur_radius, bottom_left_x / brush_bottom_left.radius_x);

				top_left.fill(canvas, brush_top_left);
				top_right.fill(canvas, brush_top_right);
				bottom_right.fill(canvas, brush_bottom_right);
				bottom_left.fill(canvas, brush_bottom_left);

				Brush brush_linear;
				brush_linear.type = BrushType::linear;
				brush_linear.stops = shadow_blur_stops(shadow_color, shadow_blur_radius, 0.0f);

				Path top;
				top.move_to(Pointf(border_points[0].x, border_points[0].y - shadow_blur_radius));
				top.line_to(Pointf(border_points[1].x, border_points[1].y - shadow_blur_radius));
				top.line_to(border_points[1]);
				top.line_to(border_points[0]);
				top.close();

				Path right;
				right.move_to(Pointf(border_points[2].x + shadow_blur_radius, border_points[2].y));
				right.line_to(Pointf(border_points[3].x + shadow_blur_radius, border_points[3].y));
				right.line_to(border_points[3]);
				right.line_to(border_points[2]);
				right.close();

				Path bottom;
				bottom.move_to(Pointf(border_points[4].x, border_points[4].y + shadow_blur_radius));
				bottom.line_to(Pointf(border_points[5].x, border_points[5].y + shadow_blur_radius));
				bottom.line_to(border_points[5]);
				bottom.line_to(border_points[4]);
				bottom.close();

				Path left;
				left.move_to(Pointf(border_points[6].x - shadow_blur_radius, border_points[6].y));
				left.line_to(Pointf(border_points[7].x - shadow_blur_radius, border_points[7].y));
				left.line_to(border_points[7]);
				left.line_to(border_points[6]);
				left.close();

				brush_linear.start_point = border_points[0];
				brush_linear.end_point = Pointf(border_points[0].x, border_points[0].y - shadow_blur_radius);
				top.fill(canvas, brush_linear);

				brush_linear.start_point = border_points[2];
				brush_linear.end_point = Pointf(border_points[2].x + shadow_blur_radius, border_points[2].y);
				right.fill(canvas, brush_linear);

				brush_linear.start_point = border_points[4];
				brush_linear.end_point = Pointf(border_points[4].x, border_points[4].y + shadow_blur_radius);
				bottom.fill(canvas, brush_linear);

				brush_linear.start_point = border_points[6];
				brush_linear.end_point = Pointf(border_points[6].x - shadow_blur_radius, border_points[6].y);
				left.fill(canvas, brush_linear);
			}
		}
	}

	std::vector<BrushGradientStop> StyleBackgroundRenderer::shadow_blur_stops(const Colorf &shadow_color, float shadow_blur_radius, float start_t)
	{
		std::vector<BrushGradientStop> stops;
		stops.push_back(BrushGradientStop(shadow_color, start_t));
		for (float step = 1.0f; step < shadow_blur_radius - 1.0f; step += 1.0f)
		{
			float t = step / shadow_blur_radius;
			float a = (1.0f - t) * (1.0f - t);
			float final_t = start_t + t * (1.0f - start_t);
			stops.push_back(BrushGradientStop(Colorf(shadow_color.r, shadow_color.g, shadow_color.b, shadow_color.a * a), final_t));
		}
		stops.push_back(BrushGradientStop(Colorf(shadow_color.r, shadow_color.g, shadow_color.b, 0.0f), 1.0f));
		return stops;
	}

	float StyleBackgroundRenderer::mix(float a, float b, float t)
	{
		return a * (1.0f - t) + b * t;
	}

	float StyleBackgroundRenderer::get_horizontal_radius(const StyleGetValue &border_radius) const
	{
		if (border_radius.is_length())
			return border_radius.number();
		else if (border_radius.is_percentage())
			return border_radius.number() * geometry.border_box().get_width() / 100.0f;
		else
			return 0.0f;
	}

	float StyleBackgroundRenderer::get_vertical_radius(const StyleGetValue &border_radius) const
	{
		if (border_radius.is_length())
			return border_radius.number();
		else if (border_radius.is_percentage())
			return border_radius.number() * geometry.border_box().get_height() / 100.0f;
		else
			return 0.0f;
	}

	Colorf StyleBackgroundRenderer::get_light_color(const StyleGetValue &border_color) const
	{
		Colorf light = border_color.color();
		light.r = min(1.0f, light.r * 1.2f);
		light.g = min(1.0f, light.g * 1.2f);
		light.b = min(1.0f, light.b * 1.2f);
		return light;
	}

	Colorf StyleBackgroundRenderer::get_dark_color(const StyleGetValue &border_color) const
	{
		Colorf dark = border_color.color();
		dark.r *= 0.8f;
		dark.g *= 0.8f;
		dark.b *= 0.8f;
		return dark;
	}
}
