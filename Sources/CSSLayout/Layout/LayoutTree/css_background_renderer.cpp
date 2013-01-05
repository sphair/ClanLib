/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
#include "css_background_renderer.h"
#include "css_layout_graphics.h"
#include "../css_resource_cache.h"
#include "../BoxTree/css_box_element.h"

namespace clan
{

CSSBackgroundRenderer::CSSBackgroundRenderer(CSSLayoutGraphics *graphics, CSSResourceCache *resource_cache, const CSSBoxProperties &computed_properties)
: graphics(graphics), resource_cache(resource_cache), computed_properties(computed_properties), is_root(false)
{
}

void CSSBackgroundRenderer::set_border_box(Rect new_border_box)
{
	border_box = new_border_box;
}

void CSSBackgroundRenderer::set_padding_box(Rect new_padding_box)
{
	padding_box = new_padding_box;
}

void CSSBackgroundRenderer::set_content_box(Rect new_content_box)
{
	content_box = new_content_box;
}

void CSSBackgroundRenderer::set_initial_containing_box(Rect new_initial_containing_box)
{
	initial_containing_box = new_initial_containing_box;
}

void CSSBackgroundRenderer::set_is_root(bool new_is_root)
{
	is_root = new_is_root;
}

void CSSBackgroundRenderer::render()
{
	if (computed_properties.background_color.type == CSSBoxBackgroundColor::type_color && computed_properties.background_color.color.a != 0.0f)
	{
		Rect clip_box = get_clip_box(computed_properties.background_image.images.size()-1);
		graphics->fill(clip_box, computed_properties.background_color.color);
	}

	for (size_t i = computed_properties.background_image.images.size(); i > 0; i--)
	{
		size_t index = i-1;

		if (computed_properties.background_image.images[index].type == CSSBoxBackgroundImage::image_type_none)
			continue;

		Image &image = graphics->get_image(computed_properties.background_image.images[index].url);
		if (!image.is_null())
		{
			Rect clip_box = get_clip_box(index);
			Rect origin_box = get_origin_box(index);
			Size image_size = get_image_size(index, image, origin_box);

			//FIXME: graphics->push_cliprect(clip_box);

			CSSBoxBackgroundRepeat::RepeatStyle repeat_x = get_layer_repeat_x(index);
			CSSBoxBackgroundRepeat::RepeatStyle repeat_y = get_layer_repeat_y(index);

			CSSActualValue y = get_start_y(index, clip_box, origin_box, image_size);
			while (true)
			{
				CSSActualValue x = get_start_x(index, clip_box, origin_box, image_size);
				while (true)
				{
					graphics->draw_image(image, Rect(x, y, x + image_size.width, y + image_size.height));

					if (repeat_x == CSSBoxBackgroundRepeat::style_no_repeat)
					{
						break;
					}
					else if (repeat_x == CSSBoxBackgroundRepeat::style_repeat)
					{
						x += image_size.width;
					}
					else if (repeat_x == CSSBoxBackgroundRepeat::style_space)
					{
						x += image_size.width;
					}
					else if (repeat_x == CSSBoxBackgroundRepeat::style_round)
					{
						x += image_size.width;
					}

					if (x >= clip_box.right)
						break;
				}

				if (repeat_y == CSSBoxBackgroundRepeat::style_no_repeat)
				{
					break;
				}
				else if (repeat_y == CSSBoxBackgroundRepeat::style_repeat)
				{
					y += image_size.height;
				}
				else if (repeat_y == CSSBoxBackgroundRepeat::style_space)
				{
					y += image_size.height;
				}
				else if (repeat_y == CSSBoxBackgroundRepeat::style_round)
				{
					y += image_size.height;
				}

				if (y >= clip_box.bottom)
					break;
			}

			//FIXME: graphics->pop_cliprect();
		}
	}
}

CSSActualValue CSSBackgroundRenderer::get_start_x(size_t index, const Rect &clip_box, const Rect &origin_box, const Size &image_size)
{
	CSSActualValue x;
	if (get_layer_repeat_x(index) == CSSBoxBackgroundRepeat::style_space && image_size.width * 2 > origin_box.get_width())
	{
		x = origin_box.left;
	}
	else
	{
		x = origin_box.left;
		switch (get_layer_position(index).type_x)
		{
		case CSSBoxBackgroundPosition::type1_left:
			x = origin_box.left;
			break;
		case CSSBoxBackgroundPosition::type1_center:
			x = origin_box.left + (origin_box.get_width() - image_size.width) / 2;
			break;
		case CSSBoxBackgroundPosition::type1_right:
			x = origin_box.right - image_size.width;
			break;
		case CSSBoxBackgroundPosition::type1_percentage:
			x = used_to_actual(origin_box.left + (origin_box.get_width()-image_size.width) * get_layer_position(index).percentage_x / 100.0f);
			break;
		case CSSBoxBackgroundPosition::type1_length:
			x = used_to_actual(origin_box.left + get_layer_position(index).length_x.value);
			break;
		}
	}

	CSSBoxBackgroundRepeat::RepeatStyle repeat_x = get_layer_repeat_x(index);
	if (repeat_x == CSSBoxBackgroundRepeat::style_repeat || repeat_x == CSSBoxBackgroundRepeat::style_space)
	{
		if (x > clip_box.left)
			x = clip_box.left - ((image_size.width - clip_box.left + x)%image_size.width);
	}

	return x;
}

CSSActualValue CSSBackgroundRenderer::get_start_y(size_t index, const Rect &clip_box, const Rect &origin_box, const Size &image_size)
{
	CSSActualValue y;
	if (get_layer_repeat_y(index) == CSSBoxBackgroundRepeat::style_space && image_size.height * 2 > origin_box.get_height())
	{
		y = origin_box.top;
	}
	else
	{
		y = origin_box.top;
		switch (get_layer_position(index).type_y)
		{
		case CSSBoxBackgroundPosition::type2_top:
			y = origin_box.top;
			break;
		case CSSBoxBackgroundPosition::type2_center:
			y = origin_box.top + (origin_box.get_height() - image_size.height) / 2;
			break;
		case CSSBoxBackgroundPosition::type2_bottom:
			y = origin_box.bottom - image_size.height;
			break;
		case CSSBoxBackgroundPosition::type2_percentage:
			y = used_to_actual(origin_box.top + (origin_box.get_height()-image_size.height) * get_layer_position(index).percentage_y / 100.0f);
			break;
		case CSSBoxBackgroundPosition::type2_length:
			y = used_to_actual(origin_box.top + get_layer_position(index).length_y.value);
			break;
		}
	}

	CSSBoxBackgroundRepeat::RepeatStyle repeat_y = get_layer_repeat_y(index);
	if (repeat_y == CSSBoxBackgroundRepeat::style_repeat || repeat_y == CSSBoxBackgroundRepeat::style_space)
	{
		if (y > clip_box.top)
			y = clip_box.top - ((image_size.height - clip_box.top + y)%image_size.height);
	}

	return y;
}

Size CSSBackgroundRenderer::get_image_size(size_t index, Image &image, Rect origin_box)
{
	Size size;
	switch (get_layer_size(index).type)
	{
	case CSSBoxBackgroundSize::size_contain:
		if (origin_box.get_height()*image.get_width() / image.get_height() <= origin_box.get_width())
			size = Size(origin_box.get_height()*image.get_width() / image.get_height(), origin_box.get_height());
		else
			size = Size(origin_box.get_width(), origin_box.get_width()*image.get_height() / image.get_width());
		break;

	case CSSBoxBackgroundSize::size_cover:
		if (origin_box.get_height()*image.get_width() / image.get_height() >= origin_box.get_width())
			size = Size(origin_box.get_height()*image.get_width() / image.get_height(), origin_box.get_height());
		else
			size = Size(origin_box.get_width(), origin_box.get_width()*image.get_height() / image.get_width());
		break;

	case CSSBoxBackgroundSize::size_values:
		{
			int width = image.get_width();
			switch (get_layer_size(index).value_x)
			{
			case CSSBoxBackgroundSize::value_type_length:
				width = used_to_actual(get_layer_size(index).length_x.value);
				break;
			case CSSBoxBackgroundSize::value_type_percentage:
				width = used_to_actual(get_layer_size(index).percentage_x * width / 100.0f);
				break;
			default:
				break;
			}

			int height = image.get_height();
			switch (get_layer_size(index).value_y)
			{
			case CSSBoxBackgroundSize::value_type_length:
				height = used_to_actual(get_layer_size(index).length_y.value);
				break;
			case CSSBoxBackgroundSize::value_type_percentage:
				height = used_to_actual(get_layer_size(index).percentage_y * height / 100.0f);
				break;
			default:
				break;
			}

			size = Size(width, height);
		}
		break;

	default:
		size = image.get_size();
		break;
	}

	CSSBoxBackgroundRepeat::RepeatStyle repeat_x = get_layer_repeat_x(index);
	CSSBoxBackgroundRepeat::RepeatStyle repeat_y = get_layer_repeat_y(index);

	if (repeat_x == CSSBoxBackgroundRepeat::style_round)
	{
		if (size.width != 0)
			size.width = origin_box.get_width() / (int)(origin_box.get_width() / (float)size.width + 0.5f);
	}

	if (repeat_y == CSSBoxBackgroundRepeat::style_round)
	{
		if (size.height != 0)
			size.height = origin_box.get_height() / (int)(origin_box.get_height() / (float)size.height + 0.5f);
	}

	if (get_layer_size(index).type == CSSBoxBackgroundSize::size_values)
	{
		if (repeat_x == CSSBoxBackgroundRepeat::style_round && get_layer_size(index).value_y == CSSBoxBackgroundSize::value_type_auto)
		{
			size.height = size.width*image.get_height() / image.get_width();
		}
		else if (repeat_y == CSSBoxBackgroundRepeat::style_round && get_layer_size(index).value_x == CSSBoxBackgroundSize::value_type_auto)
		{
			size.width = size.height*image.get_width() / image.get_height();
		}
	}

	return size;
}

Rect CSSBackgroundRenderer::get_clip_box(size_t index)
{
	if (is_root)
		return initial_containing_box;

	switch (get_layer_clip(index))
	{
	case CSSBoxBackgroundClip::clip_border_box:
		return border_box;
	case CSSBoxBackgroundClip::clip_padding_box:
		return padding_box;
	case CSSBoxBackgroundClip::clip_content_box:
		return content_box;
	default:
		return Rect();
	}
}

Rect CSSBackgroundRenderer::get_origin_box(size_t index)
{
	switch (get_layer_attachment(index))
	{
	case CSSBoxBackgroundAttachment::attachment_fixed:
		return initial_containing_box;
	case CSSBoxBackgroundAttachment::attachment_local:
		// In local mode the background scrolls with the content of formatting roots.
		// Since all scrollable elements are currently always at the top we can get away
		// by using the same code as for the 'scroll' attachment mode.
	case CSSBoxBackgroundAttachment::attachment_scroll:
		switch (get_layer_origin(index))
		{
		case CSSBoxBackgroundOrigin::origin_border_box:
			return border_box;
		case CSSBoxBackgroundOrigin::origin_padding_box:
			return padding_box;
		case CSSBoxBackgroundOrigin::origin_content_box:
			return content_box;
		default:
			return Rect();
		}
	default:
		return Rect();
	}
}

CSSBoxBackgroundClip::ClipType CSSBackgroundRenderer::get_layer_clip(size_t index)
{
	size_t count = computed_properties.background_clip.values.size();
	return computed_properties.background_clip.values[index % count];
}

CSSBoxBackgroundOrigin::OriginType CSSBackgroundRenderer::get_layer_origin(size_t index)
{
	size_t count = computed_properties.background_origin.values.size();
	return computed_properties.background_origin.values[index % count];
}

CSSBoxBackgroundSize::Size CSSBackgroundRenderer::get_layer_size(size_t index)
{
	size_t count = computed_properties.background_size.values.size();
	return computed_properties.background_size.values[index % count];
}

CSSBoxBackgroundPosition::Position CSSBackgroundRenderer::get_layer_position(size_t index)
{
	size_t count = computed_properties.background_position.positions.size();
	return computed_properties.background_position.positions[index % count];
}

CSSBoxBackgroundAttachment::Attachment CSSBackgroundRenderer::get_layer_attachment(size_t index)
{
	size_t count = computed_properties.background_attachment.attachments.size();
	return computed_properties.background_attachment.attachments[index % count];
}

CSSBoxBackgroundRepeat::RepeatStyle CSSBackgroundRenderer::get_layer_repeat_x(size_t index)
{
	size_t count = computed_properties.background_repeat.repeat_x.size();
	return computed_properties.background_repeat.repeat_x[index % count];
}

CSSBoxBackgroundRepeat::RepeatStyle CSSBackgroundRenderer::get_layer_repeat_y(size_t index)
{
	size_t count = computed_properties.background_repeat.repeat_y.size();
	return computed_properties.background_repeat.repeat_y[index % count];
}

/*
void CSSLayoutTreeNode::render_background(CSSLayoutGraphics *graphics, CSSResourceCache *resource_cache, CSSBoxElement *element_node, Rect padding_box, Rect paint_box)
{
	if (element_node->computed_properties.background_color.type == CSSBoxBackgroundColor::type_color)
		graphics->fill(paint_box, element_node->computed_properties.background_color.color);

	if (element_node->computed_properties.background_image.type == CSSBoxBackgroundImage::type_uri)
	{
		Image &image = graphics->get_image(element_node->computed_properties.background_image.url);
		if (!image.is_null())
		{
			int x = padding_box.left;
			switch (element_node->computed_properties.background_position.type_x)
			{
			case CSSBoxBackgroundPosition::type1_left:
				x = padding_box.left;
				break;
			case CSSBoxBackgroundPosition::type1_center:
				x = padding_box.left + (padding_box.get_width() - image.get_width()) / 2;
				break;
			case CSSBoxBackgroundPosition::type1_right:
				x = padding_box.right - image.get_width();
				break;
			case CSSBoxBackgroundPosition::type1_percentage:
				x = used_to_actual(padding_box.left + (padding_box.get_width()-image.get_width()) * element_node->computed_properties.background_position.percentage_x / 100.0f);
				break;
			case CSSBoxBackgroundPosition::type1_length:
				x = used_to_actual(padding_box.left + element_node->computed_properties.background_position.length_x.value);
				break;
			}

			int y = padding_box.top;
			switch (element_node->computed_properties.background_position.type_y)
			{
			case CSSBoxBackgroundPosition::type2_top:
				y = padding_box.top;
				break;
			case CSSBoxBackgroundPosition::type2_center:
				y = padding_box.top + (padding_box.get_height() - image.get_height()) / 2;
				break;
			case CSSBoxBackgroundPosition::type2_bottom:
				y = padding_box.bottom - image.get_height();
				break;
			case CSSBoxBackgroundPosition::type2_percentage:
				y = used_to_actual(padding_box.top + (padding_box.get_height()-image.get_height()) * element_node->computed_properties.background_position.percentage_y / 100.0f);
				break;
			case CSSBoxBackgroundPosition::type2_length:
				y = used_to_actual(padding_box.top + element_node->computed_properties.background_position.length_y.value);
				break;
			}

			graphics->push_cliprect(paint_box);
			if (element_node->computed_properties.background_repeat.type == CSSBoxBackgroundRepeat::type_no_repeat)
			{
				graphics->draw_image(image, x, y);
			}
			else if (element_node->computed_properties.background_repeat.type == CSSBoxBackgroundRepeat::type_repeat_x)
			{
				int start_x;
				if (x >= paint_box.left)
					start_x = paint_box.left - (x-paint_box.left)%image.get_width();
				else
					start_x = paint_box.left - (paint_box.left-x)%image.get_width();

				for (x = start_x; x < paint_box.right; x += image.get_width())
					graphics->draw_image(image, x, y);
			}
			else if (element_node->computed_properties.background_repeat.type == CSSBoxBackgroundRepeat::type_repeat_y)
			{
				int start_y;
				if (y >= paint_box.top)
					start_y = paint_box.top - (y-paint_box.top)%image.get_height();
				else
					start_y = paint_box.top - (paint_box.top-y)%image.get_height();

				for (y = start_y; y < paint_box.bottom; y += image.get_height())
					graphics->draw_image(image, x, y);
			}
			else if (element_node->computed_properties.background_repeat.type == CSSBoxBackgroundRepeat::type_repeat)
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
