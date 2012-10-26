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
**    Harry Storbacka
**    Kenneth Gangstoe
**    Mark Page
*/

#include "GUI/precomp.h"
#include "API/Display/2D/color.h"
#include "API/Display/2D/image.h"
#include "API/Core/Text/string_help.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_component.h"
#include "gui_theme_provider_default.h"
#include "../gui_css_strings.h"
#include "API/Display/Font/font.h"
#include "API/Display/2D/canvas.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// GUIThemeProvider_Default Construction:

GUIThemeProvider_Default::GUIThemeProvider_Default()
{
	prop_no_draw = GUIThemePartProperty("no-draw");
	prop_bg_image = GUIThemePartProperty("bg-image");
	prop_bg_image_sizing = GUIThemePartProperty("bg-image-sizing");
	prop_bg_image_sizing_left = GUIThemePartProperty("bg-image-sizing-left");
	prop_bg_image_sizing_top = GUIThemePartProperty("bg-image-sizing-top");
	prop_bg_image_sizing_right = GUIThemePartProperty("bg-image-sizing-right");
	prop_bg_image_sizing_bottom = GUIThemePartProperty("bg-image-sizing-bottom");
	prop_margin_top = GUIThemePartProperty("margin-top", "0");
	prop_margin_left = GUIThemePartProperty("margin-left", "0");
	prop_margin_right = GUIThemePartProperty("margin-right", "0");
	prop_margin_bottom = GUIThemePartProperty("margin-bottom", "0");
	prop_border_top = GUIThemePartProperty("border-top", "0");
	prop_border_left = GUIThemePartProperty("border-left", "0");
	prop_border_right = GUIThemePartProperty("border-right", "0");
	prop_border_bottom = GUIThemePartProperty("border-bottom", "0");
	prop_bg_color = GUIThemePartProperty("bg-color");
	prop_border_color = GUIThemePartProperty("border-color");
	prop_text_color = GUIThemePartProperty(CssStr::text_color, "black");
	prop_align = GUIThemePartProperty(CssStr::align, CssStr::left);
	prop_vertical_align = GUIThemePartProperty(CssStr::vertical_align, CssStr::center);
}

GUIThemeProvider_Default::~GUIThemeProvider_Default()
{
	std::map<GUIComponent *, ComponentCache *>::iterator it;
	for (it = cache.begin(); it != cache.end(); ++it)
	{
		delete it->second;
	}
}

/////////////////////////////////////////////////////////////////////////////
// GUIThemeProvider_Default Attributes:

std::string GUIThemeProvider_Default::get_property(
	const GUIThemePart &part,
	const std::string &name,
	const std::string &css_value)
{
	return css_value;
}

/////////////////////////////////////////////////////////////////////////////
// GUIThemeProvider_Default Operations:

void GUIThemeProvider_Default::add_resources(ResourceManager new_resources)
{
	resources.add_resources(new_resources);
}

void GUIThemeProvider_Default::set_resources(ResourceManager new_resources)
{
	resources = new_resources;
}

void GUIThemeProvider_Default::render_box(
	Canvas &canvas,
	GUIThemePart &part,
	const Rect &rect,
	const Rect &clip_rect)
{
	if (!is_no_draw(part))
	{
		render_background(canvas, part, rect, clip_rect);
		render_border(canvas, part, rect, clip_rect);
	}
}

bool GUIThemeProvider_Default::is_no_draw(GUIThemePart &part)
{
	return !part.get_property(prop_no_draw).empty();
}

bool GUIThemeProvider_Default::is_image_background(GUIThemePart &part)
{
	return !part.get_property(prop_bg_image).empty();
}

void GUIThemeProvider_Default::render_background(Canvas &canvas, GUIThemePart &part, const Rect &rect, const Rect &clip_rect)
{
	Rect background_rect = get_background_rect(part, rect);
	if (is_image_background(part))
		render_image_background(canvas, part, rect, clip_rect);
	else
		render_solid_background(canvas, part, rect, clip_rect);
}

void GUIThemeProvider_Default::render_image_background(Canvas &canvas, GUIThemePart &part, const Rect &rect, const Rect &clip_rect)
{
	Rect background_rect = get_background_rect(part, rect);
	std::string size_mode = part.get_property(prop_bg_image_sizing);
	if (size_mode == "tile")
		render_image_tile_background(canvas, part, background_rect, clip_rect);
	else if (size_mode.find("center") != std::string::npos || size_mode.find("left") != std::string::npos || size_mode.find("right") != std::string::npos)
		render_image_fixed_background(canvas, part, background_rect, clip_rect, size_mode);
	else if (size_mode == "repeat")
		render_image_repeat_background(canvas, part, background_rect, clip_rect);
	else
		render_image_stretch_background(canvas, part, background_rect, clip_rect);
}

Image &GUIThemeProvider_Default::get_background_image(Canvas &canvas, GUIThemePart &part)
{
	std::string bg_image = part.get_property(prop_bg_image);
	return get_image(canvas, part.get_component(), bg_image);
}

void GUIThemeProvider_Default::render_image_tile_background(Canvas &canvas, GUIThemePart &part, const Rect &rect, const Rect &clip_rect)
{
	Image &image = get_background_image(canvas, part);
	int spr_width = image.get_width();
	int xpos = 0;
	while(xpos < rect.get_width())
	{
		image.draw(canvas, (float)xpos + rect.left, (float)rect.top);
		xpos += spr_width;
	}
}

void GUIThemeProvider_Default::render_image_fixed_background(Canvas &canvas, GUIThemePart &part, const Rect &rect, const Rect &clip_rect, const std::string &size_mode)
{
	Image &image = get_background_image(canvas, part);
	int spr_width = image.get_width();

	int x, y;

	if (size_mode == "top left")
	{
		x = rect.left;
		y = rect.top;
	}
	else if (size_mode == "top center")
	{
		x = int((float)rect.left + rect.get_width()/2.0f - spr_width/2.0f);
		y = rect.top;
	}
	else if (size_mode == "top right")
	{
		x = int((float)rect.right - spr_width);
		y = rect.top;
	}
	else if (size_mode == "left")
	{
		x = rect.left;
		y = int((float)rect.top + rect.get_height()/2.0f - image.get_height()/2.0f);
	}
	else if (size_mode == "center")
	{
		x = int((float)rect.left + rect.get_width()/2.0f - spr_width/2.0f);
		y = int((float)rect.top + rect.get_height()/2.0f - image.get_height()/2.0f);
	}
	else if (size_mode == "right")
	{
		x = int((float)rect.right - spr_width);
		y = int((float)rect.top + rect.get_height()/2.0f - image.get_height()/2.0f);
	}
	else if (size_mode == "bottom left")
	{
		x = rect.left;
		y = int((float)rect.top - image.get_height());
	}
	else if (size_mode == "bottom center")
	{
		x = int((float)rect.left + rect.get_width()/2.0f - spr_width/2.0f);
		y = int((float)rect.top - image.get_height());
	}
	else if (size_mode == "bottom right")
	{
		x = int((float)rect.right - spr_width);
		y = int((float)rect.top - image.get_height());
	}

	image.draw(canvas, x, y);
}

void GUIThemeProvider_Default::render_image_repeat_background(Canvas &canvas, GUIThemePart &part, const Rect &rect, const Rect &clip_rect)
{
	Image &image = get_background_image(canvas, part);

	std::vector<ImagePart> image_parts = calc_stretch_image_parts(rect, image, part);
	size_t size = image_parts.size();
	for (size_t index = 0; index < size; index++)
	{
		ImagePart &ip = image_parts[index];

		if (index == 1 || index == 7)
		{
			render_image_repeat_horizontally(canvas, ip, image);
		}
		else if (index == 3 || index == 5)
		{
			render_image_repeat_vertically(canvas, ip, image);
		}
		else if (index == 4)
		{
			// index 4 (the center piece) is repeated both ways.
			// todo.
		}
		else
		{
			image.draw(canvas, image_parts[index].source_rect, image_parts[index].dest_rect);
		}
	}

//	debug_colorize_parts(canvas, image_parts);
}

void GUIThemeProvider_Default::render_image_repeat_horizontally(Canvas &canvas, ImagePart &ip, Image &image)
{
	int src_width = ip.source_rect.get_width();
	int dest_width = ip.dest_rect.get_width();
	int repeat_times = int(1 + (double(dest_width) / double(src_width)));
	Rect dest_rect = ip.dest_rect;
	dest_rect.right = dest_rect.left + src_width;

	for (int i=0; i<repeat_times; i++)
	{
		if (dest_rect.right > (ip.dest_rect.right))
		{
			// The last part might go too far - adjust rect size. 
			int diff = dest_rect.right - ip.dest_rect.right;
			ip.source_rect.right -= diff;
			dest_rect.right -= diff;
		}
		image.draw(canvas, ip.source_rect, dest_rect);
		dest_rect.translate(src_width, 0);
	}
}

void GUIThemeProvider_Default::render_image_repeat_vertically(Canvas &canvas, ImagePart &ip, Image &image)
{
	int src_height = ip.source_rect.get_height();
	int dest_height = ip.dest_rect.get_height();
	int repeat_times = int(1 + (double(dest_height) / double(src_height)));
	Rect dest_rect = ip.dest_rect;
	dest_rect.bottom = dest_rect.top + src_height;

	for (int i=0; i<repeat_times; i++)
	{
		if (dest_rect.bottom > (ip.dest_rect.bottom))
		{
			// The last part might go too far - adjust rect size. 
			int diff = dest_rect.bottom - ip.dest_rect.bottom;
			ip.source_rect.bottom -= diff;
			dest_rect.bottom -= diff;
		}
		image.draw(canvas, ip.source_rect, dest_rect);
		dest_rect.translate(0, src_height);
	}
}

void GUIThemeProvider_Default::render_image_stretch_background(Canvas &canvas, GUIThemePart &part, const Rect &rect, const Rect &clip_rect)
{
	Image &image = get_background_image(canvas, part);
	std::vector<ImagePart> image_parts = calc_stretch_image_parts(rect, image, part);
	size_t size = image_parts.size();
	for (size_t index = 0; index < size; index++)
		image.draw(canvas, image_parts[index].source_rect, image_parts[index].dest_rect);
//	debug_colorize_parts(canvas, image_parts);
}

void GUIThemeProvider_Default::debug_colorize_parts(Canvas &canvas, const std::vector<ImagePart> &parts)
{
	Colorf fill_color[] =
	{
		Colorf::violet, Colorf::yellow, Colorf::wheat, Colorf::blanchedalmond, Colorf::bisque, Colorf::burlywood,
		Colorf::seashell, Colorf::tomato, Colorf::plum, Colorf::peachpuff
	};

	for (size_t index = 0; index < parts.size(); index++)
		canvas.fill(parts[index].dest_rect, fill_color[index%10]);
}

void GUIThemeProvider_Default::render_solid_background(Canvas &canvas, GUIThemePart &part, const Rect &rect, const Rect &clip_rect)
{
	Colorf bg_color( part.get_property(prop_bg_color) );
	canvas.fill(get_background_rect(part, rect), bg_color);
}

void GUIThemeProvider_Default::render_border(Canvas &canvas, GUIThemePart &part, const Rect &rect, const Rect &clip_rect)
{
	Rect outer_rect = get_background_rect(part, rect);
	Rect inner_rect = get_inner_border_rect(part, outer_rect);
	Colorf border_color( part.get_property(prop_border_color) );
	if (border_color.a != 0.0f)
	{
		canvas.fill(get_left_border(outer_rect, inner_rect), border_color);
		canvas.fill(get_top_border(outer_rect, inner_rect), border_color);
		canvas.fill(get_bottom_border(outer_rect, inner_rect), border_color);
		canvas.fill(get_right_border(outer_rect, inner_rect), border_color);
	}
}

Rect GUIThemeProvider_Default::get_background_rect(GUIThemePart &part, const Rect &rect)
{
	int margin_top = part.get_property_int(prop_margin_top);
	int margin_left = part.get_property_int(prop_margin_left);
	int margin_right = part.get_property_int(prop_margin_right);
	int margin_bottom = part.get_property_int(prop_margin_bottom);
	Rect rect2 = rect;
	rect2.shrink(margin_left, margin_top, margin_right, margin_bottom);
	return rect2;
}

Rect GUIThemeProvider_Default::get_inner_border_rect(GUIThemePart &part, const Rect &rect)
{
	int border_top = part.get_property_int(prop_border_top);
	int border_left = part.get_property_int(prop_border_left);
	int border_right = part.get_property_int(prop_border_right);
	int border_bottom = part.get_property_int(prop_border_bottom);
	Rect rect2 = rect;
	rect2.shrink(border_left, border_top, border_right, border_bottom);
	return rect2;
}

Rect GUIThemeProvider_Default::get_left_border(const Rect &outer_rect, const Rect &inner_rect)
{
	return Rect(outer_rect.left, inner_rect.top, inner_rect.left, inner_rect.bottom);
}

Rect GUIThemeProvider_Default::get_top_border(const Rect &outer_rect, const Rect &inner_rect)
{
	return Rect(outer_rect.left, outer_rect.top, outer_rect.right, inner_rect.top);
}

Rect GUIThemeProvider_Default::get_right_border(const Rect &outer_rect, const Rect &inner_rect)
{
	return Rect(inner_rect.right, inner_rect.top, outer_rect.right, inner_rect.bottom);
}

Rect GUIThemeProvider_Default::get_bottom_border(const Rect &outer_rect, const Rect &inner_rect)
{
	return Rect(outer_rect.left, inner_rect.bottom, outer_rect.right, outer_rect.bottom);
}

std::vector<GUIThemeProvider_Default::ImagePart> GUIThemeProvider_Default::calc_stretch_image_parts(
	const Rect &dest,
	const Image &sprite,
	GUIThemePart &part)
{
	int sizing_left = part.get_property_int(prop_bg_image_sizing_left);
	int sizing_top = part.get_property_int(prop_bg_image_sizing_top);
	int sizing_right = part.get_property_int(prop_bg_image_sizing_right);
	int sizing_bottom = part.get_property_int(prop_bg_image_sizing_bottom);

	int src_width = sprite.get_width();
	int src_height = sprite.get_height();

	int src_grid_x[4];
	int src_grid_y[4];
	int dest_grid_x[4];
	int dest_grid_y[4];

	int rows = 0;
	int cols = 0;

	src_grid_x[cols++] = 0;
	src_grid_x[cols++] = sizing_left;
	src_grid_x[cols++] = src_width - sizing_right;
	src_grid_x[cols++] = src_width;

	src_grid_y[rows++] = 0;
	src_grid_y[rows++] = sizing_top;
	src_grid_y[rows++] = src_height - sizing_bottom;
	src_grid_y[rows++] = src_height;

	rows = 0;
	cols = 0;

	dest_grid_x[cols++] = dest.left;
	dest_grid_x[cols++] = dest.left + sizing_left;
	dest_grid_x[cols++] = dest.right - sizing_right;
	dest_grid_x[cols++] = dest.right;

	dest_grid_y[rows++] = dest.top;
	dest_grid_y[rows++] = dest.top + sizing_top;
	dest_grid_y[rows++] = dest.bottom - sizing_bottom;
	dest_grid_y[rows++] = dest.bottom;

	std::vector<ImagePart> image_parts;
	for (int y = 0; y < rows-1; y++)
	{
		for (int x = 0; x < cols-1; x++)
		{
			ImagePart part;
			part.source_rect = Rect(src_grid_x[x], src_grid_y[y], src_grid_x[x+1], src_grid_y[y+1]);
			part.dest_rect = Rect(dest_grid_x[x], dest_grid_y[y], dest_grid_x[x+1], dest_grid_y[y+1]);
			if (part.source_rect.get_width() > 0 && part.source_rect.get_height() > 0 &&
				part.dest_rect.get_width() > 0 && part.dest_rect.get_height() > 0)
			{
				image_parts.push_back(part);
			}
		}
	}

	return image_parts;
}

void GUIThemeProvider_Default::component_destroyed(GUIComponent *component)
{
	std::map<GUIComponent *, ComponentCache *>::iterator it;
	it = cache.find(component);
	if (it == cache.end())
		return;
	ComponentCache *c = it->second;
	cache.erase(it);
	delete c;
}

Rect GUIThemeProvider_Default::render_text(
	Canvas &canvas,
	GUIThemePart &part,
	Font &font,
	const std::string &text,
	const Rect &content_rect,
	const Rect &clip_rect)
{
	Size text_size = font.get_text_size(canvas, text);

	Colorf text_color = part.get_property(prop_text_color);
	std::string h_align = part.get_property(prop_align); // left|center|right|justify
	std::string v_align = part.get_property(prop_vertical_align); // top|center|bottom

	int x = content_rect.left, y = content_rect.top;
	if (h_align == CssStr::center)
	{
		x = content_rect.get_center().x - font.get_text_size(canvas, text).width / 2.0f;
	}
	else if (h_align == CssStr::right)
	{
		x = content_rect.right - text_size.width;
	}

	if (v_align == CssStr::top)
	{
		y = part.get_vertical_text_align(canvas, font, content_rect).top;
	}
	else if (v_align == CssStr::center)
	{
		y = part.get_vertical_text_align(canvas, font, content_rect).baseline;
	}
	else if (v_align == CssStr::bottom)
	{
		y = part.get_vertical_text_align(canvas, font, content_rect).bottom;
	}

	font.draw_text_ellipsis(canvas, x, y, content_rect, text, text_color);

	return Rect(Point(x,y), text_size);
}

/////////////////////////////////////////////////////////////////////////////
// GUIThemeProvider_Default Implementation:

GUIThemeProvider_Default::ComponentCache *GUIThemeProvider_Default::get_component_cache(GUIComponent *component)
{
	std::map<GUIComponent *, ComponentCache *>::iterator it;
	it = cache.find(component);
	if (it == cache.end())
	{
		ComponentCache *c = new ComponentCache;
		cache[component] = c;
		return c;
	}
	return it->second;
}

Image &GUIThemeProvider_Default::get_image(Canvas &canvas, GUIComponent *component, const std::string &name)
{
	ComponentCache *cache = get_component_cache(component);
	if (cache->images.find(name) == cache->images.end())
		cache->images[name] = Image(canvas, name, &resources);
	return cache->images[name];
}

}
