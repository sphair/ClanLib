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
#include "box_style_impl.h"
#include "API/UI/Style/box_geometry.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/2D/path.h"
#include "API/Display/2D/brush.h"
#include <algorithm>

namespace clan
{
	void BoxStyleImpl::render(Canvas &canvas, const BoxGeometry &geometry) const
	{
		if (background.color.a != 0.0f || !background.stops.empty() || background.image)
		{
			Rectf border_box = geometry.border_box();
			Rectf padding_box = geometry.padding_box();

			Pointf border_points[2 * 4];
			border_points[0] = Pointf(border_box.left + border.top_left_radius.x, border_box.top);
			border_points[1] = Pointf(border_box.right - border.top_right_radius.x, border_box.top);
			border_points[2] = Pointf(border_box.right, border_box.top + border.top_right_radius.y);
			border_points[3] = Pointf(border_box.right, border_box.bottom - border.bottom_right_radius.y);
			border_points[4] = Pointf(border_box.right - border.bottom_right_radius.x, border_box.bottom);
			border_points[5] = Pointf(border_box.left + border.bottom_left_radius.x, border_box.bottom);
			border_points[6] = Pointf(border_box.left, border_box.bottom - border.bottom_left_radius.y);
			border_points[7] = Pointf(border_box.left, border_box.top + border.top_left_radius.y);

			Pointf padding_points[2 * 4];
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

			// Border path (the path defining the actual border)

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

			if (background.shadow_color.a != 0.0f && !background.shadow_inset)
			{
				// To do: support shadow_spread_distance

				float shadow_blur_radius = background.shadow_blur_radius;
				Pointf shadow_offset = background.shadow_offset;
				Colorf shadow_color = background.shadow_color;
				Colorf transparent = shadow_color;
				transparent.a = 0.0f;

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
					brush_top_left.radius_x = shadow_blur_radius + border.top_left_radius.x;
					brush_top_left.radius_y = shadow_blur_radius + border.top_left_radius.y;
					brush_top_left.center_point = Pointf(border_box.left + border.top_left_radius.x, border_box.top + border.top_left_radius.y);
					brush_top_left.stops = shadow_blur_stops(shadow_color, shadow_blur_radius, border.top_left_radius.x / brush_top_left.radius_x);

					Brush brush_top_right;
					brush_top_right.type = BrushType::radial;
					brush_top_right.radius_x = shadow_blur_radius + border.top_right_radius.x;
					brush_top_right.radius_y = shadow_blur_radius + border.top_right_radius.y;
					brush_top_right.center_point = Pointf(border_box.right - border.top_right_radius.x, border_box.top + border.top_right_radius.y);
					brush_top_right.stops = shadow_blur_stops(shadow_color, shadow_blur_radius, border.top_right_radius.x / brush_top_right.radius_x);

					Brush brush_bottom_right;
					brush_bottom_right.type = BrushType::radial;
					brush_bottom_right.radius_x = shadow_blur_radius + border.bottom_right_radius.x;
					brush_bottom_right.radius_y = shadow_blur_radius + border.bottom_right_radius.y;
					brush_bottom_right.center_point = Pointf(border_box.right - border.bottom_right_radius.x, border_box.bottom - border.bottom_right_radius.y);
					brush_bottom_right.stops = shadow_blur_stops(shadow_color, shadow_blur_radius, border.bottom_right_radius.x / brush_bottom_right.radius_x);

					Brush brush_bottom_left;
					brush_bottom_left.type = BrushType::radial;
					brush_bottom_left.radius_x = shadow_blur_radius + border.bottom_left_radius.x;
					brush_bottom_left.radius_y = shadow_blur_radius + border.bottom_left_radius.y;
					brush_bottom_left.center_point = Pointf(border_box.left + border.bottom_left_radius.x, border_box.bottom - border.bottom_left_radius.y);
					brush_bottom_left.stops = shadow_blur_stops(shadow_color, shadow_blur_radius, border.bottom_left_radius.x / brush_bottom_left.radius_x);

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

			if (background.color.a != 0.0f)
			{
				border_area_path.fill(canvas, Brush(background.color));
			}

			if (!background.stops.empty())
			{
				float angle = Angle::from_degrees(background.angle.to_degrees() - 90.0f).normalize().to_radians();
				Pointf center = padding_box.get_center();
				Pointf length {
					0.5f * padding_box.get_width () * std::cos(angle),
					0.5f * padding_box.get_height() * std::sin(angle)
				};

				Brush brush;
				brush.type = BrushType::linear;
				brush.start_point = center - length;
				brush.end_point   = center + length;

				for (const BoxGradientStop &stop : background.stops)
					brush.stops.push_back(BrushGradientStop(stop.color, stop.position));

				border_area_path.fill(canvas, brush);
			}

			if (background.image)
			{
				Brush brush;
				brush.type = BrushType::image;
				brush.image = background.image->get_image(canvas);
				brush.transform = Mat3f::translate(border_box.left, border_box.top);
				border_area_path.fill(canvas, brush);
			}

			if (border.top.type == BoxBorderValue::type_solid)
			{
				border_path.fill(canvas, Brush(border.top.color));
			}
		}
	}

	std::vector<BrushGradientStop> BoxStyleImpl::shadow_blur_stops(const Colorf &shadow_color, float shadow_blur_radius, float start_t)
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

	float BoxStyleImpl::mix(float a, float b, float t)
	{
		return a * (1.0f - t) + b * t;
	}
}
