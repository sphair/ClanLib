/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
*/

#include "GUI/precomp.h"
#include "API/Display/2D/color.h"
#include "API/Display/2D/draw.h"
#include "API/Display/2D/image.h"
#include "API/Core/Text/string_help.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_component.h"
#include "gui_theme_default_impl.h"
#include "gui_css_strings.h"
#include "API/Display/Font/font.h"

/////////////////////////////////////////////////////////////////////////////
// CL_GUIThemeDefault_Impl Construction:

CL_GUIThemeDefault_Impl::CL_GUIThemeDefault_Impl()
{
	prop_no_draw = CL_GUIThemePartProperty(cl_text("no-draw"));
	prop_bg_image = CL_GUIThemePartProperty(cl_text("bg-image"));
	prop_bg_image_sizing = CL_GUIThemePartProperty(cl_text("bg-image-sizing"));
	prop_bg_image_sizing_left = CL_GUIThemePartProperty(cl_text("bg-image-sizing-left"));
	prop_bg_image_sizing_top = CL_GUIThemePartProperty(cl_text("bg-image-sizing-top"));
	prop_bg_image_sizing_right = CL_GUIThemePartProperty(cl_text("bg-image-sizing-right"));
	prop_bg_image_sizing_bottom = CL_GUIThemePartProperty(cl_text("bg-image-sizing-bottom"));
	prop_margin_top = CL_GUIThemePartProperty(cl_text("margin-top"), cl_text("0"));
	prop_margin_left = CL_GUIThemePartProperty(cl_text("margin-left"), cl_text("0"));
	prop_margin_right = CL_GUIThemePartProperty(cl_text("margin-right"), cl_text("0"));
	prop_margin_bottom = CL_GUIThemePartProperty(cl_text("margin-bottom"), cl_text("0"));
	prop_border_top = CL_GUIThemePartProperty(cl_text("border-top"), cl_text("0"));
	prop_border_left = CL_GUIThemePartProperty(cl_text("border-left"), cl_text("0"));
	prop_border_right = CL_GUIThemePartProperty(cl_text("border-right"), cl_text("0"));
	prop_border_bottom = CL_GUIThemePartProperty(cl_text("border-bottom"), cl_text("0"));
	prop_bg_color = CL_GUIThemePartProperty(cl_text("bg-color"));
	prop_border_color = CL_GUIThemePartProperty(cl_text("border-color"));
	prop_text_color = CL_GUIThemePartProperty(CssStr::text_color, cl_text("black"));
	prop_align = CL_GUIThemePartProperty(CssStr::align, CssStr::left);
	prop_vertical_align = CL_GUIThemePartProperty(CssStr::vertical_align, CssStr::center);
}

CL_GUIThemeDefault_Impl::~CL_GUIThemeDefault_Impl()
{
	std::map<CL_GUIComponent *, ComponentCache *>::iterator it;
	for (it = cache.begin(); it != cache.end(); ++it)
	{
		delete it->second;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIThemeDefault_Impl Attributes:

CL_StringRef CL_GUIThemeDefault_Impl::get_property(
	const CL_GUIThemePart &part,
	const CL_StringRef &name,
	const CL_StringRef &css_value)
{
	return css_value;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIThemeDefault_Impl Operations:

void CL_GUIThemeDefault_Impl::render_box(
	CL_GraphicContext &gc,
	CL_GUIThemePart &part,
	const CL_Rect &rect,
	const CL_Rect &clip_rect)
{
	CL_TempString no_draw = part.get_property(prop_no_draw);
	if (!no_draw.empty())
		return;

	CL_TempString bg_image = part.get_property(prop_bg_image);
	if (!bg_image.empty())
	{
		CL_Image &image = get_image(gc, part.get_component(), bg_image);

		int spr_width = image.get_width();
		//int spr_height = sprite.get_height();

		CL_TempString size_mode = part.get_property(prop_bg_image_sizing);

		if (size_mode == cl_text("tile"))
		{
			int xpos = 0;
			while(xpos < rect.get_width())
			{
				image.draw(gc, (float)xpos + rect.left, (float)rect.top);
				xpos += spr_width;
			}
		}
		else if (size_mode == cl_text("center"))
		{
			image.draw(gc, 
				int((float)rect.left + rect.get_width()/2.0f - spr_width/2.0f),
				int((float)rect.top + rect.get_height()/2.0f - image.get_height()/2.0f));
		}
		else
		{
			int bg_image_sizing_left = part.get_property_int(prop_bg_image_sizing_left);
			int bg_image_sizing_top = part.get_property_int(prop_bg_image_sizing_top);
			int bg_image_sizing_right = part.get_property_int(prop_bg_image_sizing_right);
			int bg_image_sizing_bottom = part.get_property_int(prop_bg_image_sizing_bottom);

			std::vector<ImagePart> image_parts = calc_stretch_image_parts(
				rect, image,
				bg_image_sizing_left,
				bg_image_sizing_top,
				bg_image_sizing_right,
				bg_image_sizing_bottom);

			std::vector<ImagePart>::size_type index, size;
			size = image_parts.size();
/*
			CL_Colorf fill_color[] =
			{
				CL_Colorf::violet, CL_Colorf::yellow, CL_Colorf::wheat, CL_Colorf::blanchedalmond, CL_Colorf::bisque, CL_Colorf::burlywood,
				CL_Colorf::seashell, CL_Colorf::tomato, CL_Colorf::plum, CL_Colorf::peachpuff
			};
*/
			if (size_mode == cl_text("repeat"))
			{
				for (index = 0; index < size; index++)
				{
					ImagePart &ip = image_parts[index];

					if (index == 1 || index == 7)
					{
						// image_parts[] indexes 1 and 7 are repeated horizontally.

						int src_width = ip.source_rect.get_width();
						int dest_width = ip.dest_rect.get_width();
						int repeat_times = int(1 + (double(dest_width) / double(src_width)));
						CL_Rect dest_rect = ip.dest_rect;
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
							image.draw(gc, ip.source_rect, dest_rect);
							dest_rect.translate(src_width, 0);
						}
					}
					else if (index == 3 || index == 5)
					{
						// image_parts[] indexes 3 and 5 are repeated vertically.

						int src_height = ip.source_rect.get_height();
						int dest_height = ip.dest_rect.get_height();
						int repeat_times = int(1 + (double(dest_height) / double(src_height)));
						CL_Rect dest_rect = ip.dest_rect;
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
							image.draw(gc, ip.source_rect, dest_rect);
							dest_rect.translate(0, src_height);
						}
					}
					else if (index == 4)
					{
						// index 4 (the center piece) is repeated both ways.
						// todo.
					}
					else
					{
						image.draw(gc, image_parts[index].source_rect, image_parts[index].dest_rect);
					}

					// CL_Draw::fill(gc, image_parts[index].dest_rect, fill_color[index%10]);
				}
			}
			else
			{
				for (index = 0; index < size; index++)
				{
					image.draw(gc, image_parts[index].source_rect, image_parts[index].dest_rect);
					// CL_Draw::fill(gc, image_parts[index].dest_rect, fill_color[index%10]);
				}
			}
		}
	}
	else
	{
		int margin_top = part.get_property_int(prop_margin_top);
		int margin_left = part.get_property_int(prop_margin_left);
		int margin_right = part.get_property_int(prop_margin_right);
		int margin_bottom = part.get_property_int(prop_margin_bottom);

		int border_top = part.get_property_int(prop_border_top);
		int border_left = part.get_property_int(prop_border_left);
		int border_right = part.get_property_int(prop_border_right);
		int border_bottom = part.get_property_int(prop_border_bottom);

		CL_Rect tmp_rect = rect;

		tmp_rect.top += margin_top;
		tmp_rect.left += margin_left;
		tmp_rect.right -= margin_right;
		tmp_rect.bottom -= margin_bottom;

		CL_Colorf bg_color( part.get_property(prop_bg_color) );
		CL_Colorf border_color( part.get_property(prop_border_color) );

		CL_Rect rleft(tmp_rect.left, tmp_rect.top, tmp_rect.left+border_left, tmp_rect.bottom);
		CL_Rect rright(tmp_rect.right, tmp_rect.top, tmp_rect.right-border_right, tmp_rect.bottom);
		CL_Rect rtop(tmp_rect.left+border_left, tmp_rect.top, tmp_rect.right-border_right, tmp_rect.top+border_top);
		CL_Rect rbottom(tmp_rect.left+border_left, tmp_rect.bottom-border_bottom, tmp_rect.right-border_right, tmp_rect.bottom);

		CL_Draw::fill(gc, rleft, border_color);
		CL_Draw::fill(gc, rtop, border_color);
		CL_Draw::fill(gc, rbottom, border_color);
		CL_Draw::fill(gc, rright, border_color);

		tmp_rect.top += border_top;
		tmp_rect.left += border_left;
		tmp_rect.right -= (border_right);
		tmp_rect.bottom -= (border_bottom);

		CL_Draw::fill(gc, tmp_rect, bg_color);
	}
}

std::vector<CL_GUIThemeDefault_Impl::ImagePart> CL_GUIThemeDefault_Impl::calc_stretch_image_parts(
	const CL_Rect &dest,
	const CL_Image &sprite,
	int sizing_left,
	int sizing_top,
	int sizing_right,
	int sizing_bottom)
{
	int src_width = sprite.get_width();
	int src_height = sprite.get_height();
	int dest_width = dest.get_width();
	int dest_height = dest.get_height();

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
			part.source_rect = CL_Rect(src_grid_x[x], src_grid_y[y], src_grid_x[x+1], src_grid_y[y+1]);
			part.dest_rect = CL_Rect(dest_grid_x[x], dest_grid_y[y], dest_grid_x[x+1], dest_grid_y[y+1]);
			if (part.source_rect.get_width() > 0 && part.source_rect.get_height() > 0 &&
				part.dest_rect.get_width() > 0 && part.dest_rect.get_height() > 0)
			{
				image_parts.push_back(part);
			}
		}
	}

	return image_parts;
}

void CL_GUIThemeDefault_Impl::component_destroyed(CL_GUIComponent *component)
{
	std::map<CL_GUIComponent *, ComponentCache *>::iterator it;
	it = cache.find(component);
	if (it == cache.end())
		return;
	ComponentCache *c = it->second;
	cache.erase(it);
	delete c;
}

CL_Rect CL_GUIThemeDefault_Impl::render_text(
	CL_GraphicContext &gc,
	CL_GUIThemePart &part,
	CL_Font &font,
	const CL_StringRef &text,
	const CL_Rect &content_rect,
	const CL_Rect &clip_rect)
{
	CL_Size text_size = font.get_text_size(gc, text);

	CL_Colorf text_color = part.get_property(prop_text_color);
	CL_String h_align = part.get_property(prop_align); // left|center|right|justify
	CL_String v_align = part.get_property(prop_vertical_align); // top|center|bottom

	int x = content_rect.left, y = content_rect.top;
	if (h_align == CssStr::center)
	{
		x = content_rect.get_center().x - font.get_text_size(gc, text).width / 2.0f;
	}
	else if (h_align == CssStr::right)
	{
		x = content_rect.right - text_size.width;
	}

	if (v_align == CssStr::top)
	{
		y = part.get_vertical_text_align(gc, font, content_rect).top;
	}
	else if (v_align == CssStr::center)
	{
		y = part.get_vertical_text_align(gc, font, content_rect).baseline;
	}
	else if (v_align == CssStr::bottom)
	{
		y = part.get_vertical_text_align(gc, font, content_rect).bottom;
	}

	font.draw_text(gc, x,y,text,text_color);

	return CL_Rect(CL_Point(x,y), text_size);
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIThemeDefault_Impl Implementation:

CL_GUIThemeDefault_Impl::ComponentCache *CL_GUIThemeDefault_Impl::get_component_cache(CL_GUIComponent *component)
{
	std::map<CL_GUIComponent *, ComponentCache *>::iterator it;
	it = cache.find(component);
	if (it == cache.end())
	{
		ComponentCache *c = new ComponentCache;
		cache[component] = c;
		return c;
	}
	return it->second;
}

CL_Image &CL_GUIThemeDefault_Impl::get_image(CL_GraphicContext &gc, CL_GUIComponent *component, const CL_StringRef &name)
{
	ComponentCache *cache = get_component_cache(component);
	if (cache->images.find(name) == cache->images.end())
		cache->images[name] = CL_Image(gc, name, &resources);
	return cache->images[name];
}
