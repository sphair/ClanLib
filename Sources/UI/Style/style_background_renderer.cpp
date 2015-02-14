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
#include "style_background_renderer.h"
#include "API/UI/Style/box_geometry.h"
#include "API/UI/Style/style.h"
#include "API/UI/UIThread/ui_thread.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/2D/image.h"

namespace clan
{
	StyleBackgroundRenderer::StyleBackgroundRenderer(Canvas &canvas, const BoxGeometry &geometry, const Style &style) : canvas(canvas), geometry(geometry), style(style)
	{
	}

	void StyleBackgroundRenderer::render()
	{
		int num_layers = style.array_size("background-image");

		StyleValue bg_color = style.computed_value("background-color");
		if (bg_color.is_color() && bg_color.color.a != 0.0f)
		{
			Rectf clip_box = get_clip_box(num_layers - 1);
			canvas.fill_rect(clip_box, bg_color.color);
		}

		for (int index = num_layers - 1; index >= 0; index--)
		{
			StyleValue layer_image = style.computed_value("background-image[" + StringHelp::int_to_text(index) + "]");
			if (layer_image.is_keyword("none"))
				continue;

			Image image;
			
			if (layer_image.is_image())
				image = Image::resource(canvas, layer_image.text, UIThread::get_resources());

			if (!image.is_null())
			{
				Rectf clip_box = get_clip_box(index);
				Rectf origin_box = get_origin_box(index);
				Sizef image_size = get_image_size(index, image, origin_box);

				canvas.push_cliprect(clip_box);

				StyleValue repeat_x = get_layer_repeat_x(index);
				StyleValue repeat_y = get_layer_repeat_y(index);

				float y = get_start_y(index, clip_box, origin_box, image_size);
				while (true)
				{
					float x = get_start_x(index, clip_box, origin_box, image_size);
					while (true)
					{
						image.draw(canvas, Rectf(x, y, x + image_size.width, y + image_size.height));

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

				canvas.pop_cliprect();
			}
		}
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
			StyleValue pos_x = get_layer_position_x(index);
			if (pos_x.is_keyword("left"))
				x = origin_box.left;
			else if (pos_x.is_keyword("center"))
				x = origin_box.left + (origin_box.get_width() - image_size.width) / 2;
			else if (pos_x.is_keyword("right"))
				x = origin_box.right - image_size.width;
			else if (pos_x.is_percentage())
				x = origin_box.left + (origin_box.get_width() - image_size.width) * pos_x.number / 100.0f;
			else if (pos_x.is_length())
				x = origin_box.left + pos_x.number;
		}

		StyleValue repeat_x = get_layer_repeat_x(index);
		if (repeat_x.is_keyword("repeat") || repeat_x.is_keyword("space"))
		{
			if (x > clip_box.left)
				x = clip_box.left - std::fmod(image_size.width - clip_box.left + x, image_size.width);
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
			StyleValue pos_y = get_layer_position_x(index);
			if (pos_y.is_keyword("top"))
				y = origin_box.top;
			else if (pos_y.is_keyword("center"))
				y = origin_box.top + (origin_box.get_height() - image_size.height) / 2;
			else if (pos_y.is_keyword("bottom"))
				y = origin_box.bottom - image_size.height;
			else if (pos_y.is_percentage())
				y = origin_box.top + (origin_box.get_height() - image_size.height) * pos_y.number / 100.0f;
			else if (pos_y.is_length())
				y = origin_box.top + pos_y.number;
		}

		StyleValue repeat_y = get_layer_repeat_y(index);
		if (repeat_y.is_keyword("repeat") || repeat_y.is_keyword("space"))
		{
			if (y > clip_box.top)
				y = clip_box.top - std::fmod(image_size.height - clip_box.top + y, image_size.height);
		}

		return y;
	}

	Sizef StyleBackgroundRenderer::get_image_size(int index, Image &image, Rectf origin_box)
	{
		Sizef size;
		StyleValue size_x = get_layer_size_x(index);
		StyleValue size_y = get_layer_size_y(index);
		
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
				width = size_x.number;
			}
			else if (size_x.is_percentage())
			{
				width = size_x.number * width / 100.0f;
			}

			float height = image.get_height();
			if (size_y.is_length())
			{
				height = size_y.number;
			}
			else if (size_y.is_percentage())
			{
				height = size_y.number * height / 100.0f;
			}

			size = Sizef(width, height);
		}

		StyleValue repeat_x = get_layer_repeat_x(index);
		StyleValue repeat_y = get_layer_repeat_y(index);

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

		StyleValue clip = get_layer_clip(index);
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
		StyleValue attachment = get_layer_attachment(index);
		/*if (attachment.is_keyword("fixed"))
		{
			return initial_containing_box;
		}
		else*/ if (attachment.is_keyword("local") || attachment.is_keyword("scroll"))
		{
			StyleValue origin = get_layer_origin(index);
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

	StyleValue StyleBackgroundRenderer::get_layer_clip(int index)
	{
		int count = style.array_size("background-clip");
		return style.computed_value("background-clip[" + StringHelp::int_to_text(index % count) + "]");
	}

	StyleValue StyleBackgroundRenderer::get_layer_origin(int index)
	{
		int count = style.array_size("background-origin");
		return style.computed_value("background-origin[" + StringHelp::int_to_text(index % count) + "]");
	}

	StyleValue StyleBackgroundRenderer::get_layer_size_x(int index)
	{
		int count = style.array_size("background-size-x");
		return style.computed_value("background-size-x[" + StringHelp::int_to_text(index % count) + "]");
	}

	StyleValue StyleBackgroundRenderer::get_layer_size_y(int index)
	{
		int count = style.array_size("background-size-y");
		return style.computed_value("background-size-y[" + StringHelp::int_to_text(index % count) + "]");
	}

	StyleValue StyleBackgroundRenderer::get_layer_position_x(int index)
	{
		int count = style.array_size("background-position-x");
		return style.computed_value("background-position-x[" + StringHelp::int_to_text(index % count) + "]");
	}

	StyleValue StyleBackgroundRenderer::get_layer_position_y(int index)
	{
		int count = style.array_size("background-position-y");
		return style.computed_value("background-position-y[" + StringHelp::int_to_text(index % count) + "]");
	}

	StyleValue StyleBackgroundRenderer::get_layer_attachment(int index)
	{
		int count = style.array_size("background-attachment");
		return style.computed_value("background-attachment[" + StringHelp::int_to_text(index % count) + "]");
	}

	StyleValue StyleBackgroundRenderer::get_layer_repeat_x(int index)
	{
		int count = style.array_size("background-repeat-x");
		return style.computed_value("background-repeat-x[" + StringHelp::int_to_text(index % count) + "]");
	}

	StyleValue StyleBackgroundRenderer::get_layer_repeat_y(int index)
	{
		int count = style.array_size("background-repeat-y");
		return style.computed_value("background-repeat-y[" + StringHelp::int_to_text(index % count) + "]");
	}

	/*
	void StyleLayoutTreeNode::render_background(StyleLayoutGraphics *graphics, StyleResourceCache *resource_cache, StyleBoxElement *element_node, Rect padding_box, Rect paint_box)
	{
		if (element_node->computed_properties.background_color.type == StyleValueBackgroundColor::type_color)
			graphics->fill(paint_box, element_node->computed_properties.background_color.color);

		if (element_node->computed_properties.background_image.type == StyleValueBackgroundImage::type_uri)
		{
			Image &image = graphics->get_image(element_node->computed_properties.background_image.url);
			if (!image.is_null())
			{
				int x = padding_box.left;
				switch (element_node->computed_properties.background_position.type_x)
				{
				case StyleValueBackgroundPosition::type1_left:
					x = padding_box.left;
					break;
				case StyleValueBackgroundPosition::type1_center:
					x = padding_box.left + (padding_box.get_width() - image.get_width()) / 2;
					break;
				case StyleValueBackgroundPosition::type1_right:
					x = padding_box.right - image.get_width();
					break;
				case StyleValueBackgroundPosition::type1_percentage:
					x = used_to_actual(padding_box.left + (padding_box.get_width()-image.get_width()) * element_node->computed_properties.background_position.percentage_x / 100.0f);
					break;
				case StyleValueBackgroundPosition::type1_length:
					x = used_to_actual(padding_box.left + element_node->computed_properties.background_position.length_x.value);
					break;
				}

				int y = padding_box.top;
				switch (element_node->computed_properties.background_position.type_y)
				{
				case StyleValueBackgroundPosition::type2_top:
					y = padding_box.top;
					break;
				case StyleValueBackgroundPosition::type2_center:
					y = padding_box.top + (padding_box.get_height() - image.get_height()) / 2;
					break;
				case StyleValueBackgroundPosition::type2_bottom:
					y = padding_box.bottom - image.get_height();
					break;
				case StyleValueBackgroundPosition::type2_percentage:
					y = used_to_actual(padding_box.top + (padding_box.get_height()-image.get_height()) * element_node->computed_properties.background_position.percentage_y / 100.0f);
					break;
				case StyleValueBackgroundPosition::type2_length:
					y = used_to_actual(padding_box.top + element_node->computed_properties.background_position.length_y.value);
					break;
				}

				graphics->push_cliprect(paint_box);
				if (element_node->computed_properties.background_repeat.type == StyleValueBackgroundRepeat::type_no_repeat)
				{
					graphics->draw_image(image, x, y);
				}
				else if (element_node->computed_properties.background_repeat.type == StyleValueBackgroundRepeat::type_repeat_x)
				{
					int start_x;
					if (x >= paint_box.left)
						start_x = paint_box.left - (x-paint_box.left)%image.get_width();
					else
						start_x = paint_box.left - (paint_box.left-x)%image.get_width();

					for (x = start_x; x < paint_box.right; x += image.get_width())
						graphics->draw_image(image, x, y);
				}
				else if (element_node->computed_properties.background_repeat.type == StyleValueBackgroundRepeat::type_repeat_y)
				{
					int start_y;
					if (y >= paint_box.top)
						start_y = paint_box.top - (y-paint_box.top)%image.get_height();
					else
						start_y = paint_box.top - (paint_box.top-y)%image.get_height();

					for (y = start_y; y < paint_box.bottom; y += image.get_height())
						graphics->draw_image(image, x, y);
				}
				else if (element_node->computed_properties.background_repeat.type == StyleValueBackgroundRepeat::type_repeat)
				{
					int start_x, start_y;
					if (x >= paint_box.left)
						start_x = paint_box.left - (x-paint_box.left)%image.get_width();
					else
						start_x = paint_box.left - (paint_box.left-x)%image.get_width();

					if (y >= paint_box.top)
						start_y = paint_box.top - (y-paint_box.top)%image.get_height();
					else
						start_y = paint_box.top - (paint_box.top-y)%image.get_height();

					for (y = start_y; y < paint_box.bottom; y += image.get_height())
						for (x = start_x; x < paint_box.right; x += image.get_width())
							graphics->draw_image(image, x, y);
				}
				graphics->pop_cliprect();
			}
		}
	}
	*/

}
