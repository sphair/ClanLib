/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
**    Mark Page
*/

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gui.h>
#include <ClanLib/gl.h>
#include "gui.h"
#include "app.h"

#include <cstdio>

GUI::GUI(App *app) : app(app), window_ptr(app->get_window()), wm(*window_ptr)
{
	gui.set_window_manager(wm);

	CL_String resource_filename("../../../Resources/GUIThemeAeroPacked/resources.xml");
	CL_String theme_filename("../../../Resources/GUIThemeAeroPacked/theme.css");
	if (!CL_FileHelp::file_exists(resource_filename))
	{
		resource_filename = "../../../Resources/GUIThemeBasicPacked/resources.xml";
		theme_filename = "../../../Resources/GUIThemeBasicPacked/theme.css";
	}



	resources_gui = CL_ResourceManager(resource_filename);
	resources_internal = CL_ResourceManager("resources.xml");

	theme.set_resources(resources_gui);
	gui.set_theme(theme); 
	gui.set_css_document(theme_filename);

	CL_GraphicContext gc = window_ptr->get_gc();

	CL_ResourceManager resources("resources.xml");
	sprite_grid_normal = CL_Sprite(gc, "sprite_grid_normal", &resources);
	sprite_grid_outline = CL_Sprite(gc, "sprite_grid_outline", &resources);
	image_grid_normal = CL_Image(gc, "image_grid_normal", &resources);
	image_grid_outline = CL_Image(gc, "image_grid_outline", &resources);
	texture_grid_normal = CL_Texture("texture_grid_normal", &resources, gc);
	texture_grid_outline = CL_Texture("texture_grid_outline", &resources, gc);

	display_image_size = CL_Size(64, 64);
	display_image_geometry = CL_Rect(320, 40, CL_Size(512, 512));
	source_image_geometry = CL_Rect(16, 16, CL_Size(32, 32));

	display_image = CL_Texture(gc, display_image_size.width, display_image_size.height);
	display_image.set_wrap_mode(cl_wrap_clamp_to_edge, cl_wrap_clamp_to_edge, cl_wrap_clamp_to_edge);
	display_image.set_min_filter(cl_filter_nearest);
	display_image.set_mag_filter(cl_filter_nearest);

	display_framebuffer = CL_FrameBuffer(gc);
	display_framebuffer.attach_color_buffer(0, display_image);

	font_texture_group = CL_TextureGroup(gc, CL_Size(512, 512));
	font = CL_Font_System(gc, "Tahoma", 32);
	font.set_texture_group(font_texture_group);
	font_small = CL_Font_System(gc, "Tahoma", 16);
	font_small.set_texture_group(font_texture_group);

	CL_FontDescription font_desc;
	font_desc.set_typeface_name("Tahoma");
	font_desc.set_height(64);
	font_desc.set_anti_alias(false);
	font_large = CL_Font_System(gc, font_desc);
	font_large.set_texture_group(font_texture_group);

	font_desc.set_anti_alias(true);
	font_large_antialias = CL_Font_System(gc, font_desc);
	font_large_antialias.set_texture_group(font_texture_group);

	panel.reset(new Panel(this) );
}

GUI::~GUI()
{
}

bool GUI::run()
{
	CL_GraphicContext gc = window_ptr->get_gc();

	CL_Draw::gradient_fill(gc, window_ptr->get_viewport(), CL_Gradient(CL_Colorf(0.4f, 0.4f, 0.4f, 1.0f), CL_Colorf(0.0f, 0.0f, 0.0f, 1.0f)));

	gc.set_map_mode(CL_MapMode(cl_map_2d_upper_left));

	wm.process();
	wm.draw_windows(gc);

	write_display_image();

	display_modelview_matrix("ModelView Matrix", 50.0f, 400.0f, last_modelview_matrix);

	show_display_image();

	draw_reference_images();

	window_ptr->flip(1);

	CL_KeepAlive::process();

	return true;
}

void GUI::write_display_image()
{
	CL_GraphicContext gc = window_ptr->get_gc();
	gc.set_frame_buffer(display_framebuffer);

	gc.clear(CL_Colorf::black);

	float translate = panel->get_translate();
	gc.push_translate(translate, translate);

	float scale = panel->get_scale();

	float width = source_image_geometry.get_width();
	float height = source_image_geometry.get_height();

	width *= scale;
	height *= scale;

	CL_Rectf dest_geometry((display_image_size.width - width)/2, (display_image_size.height - height)/2, CL_Sizef(width, height));

	float rotation = panel->get_rotation();
	CL_Angle angle(rotation, cl_degrees);

	CL_Vec2f center = dest_geometry.get_center();
	gc.mult_translate(center.x, center.y);
	gc.mult_rotate(angle);
	gc.mult_translate(-center.x, -center.y);

	gc.flush_batcher();
	last_modelview_matrix = gc.get_modelview();

	if (panel->is_image())
	{
		if (panel->is_outlined())
		{
			image_grid_outline.draw(gc, source_image_geometry, dest_geometry);
		}
		else
		{
			image_grid_normal.draw(gc, source_image_geometry, dest_geometry);
		}
	}

	if (panel->is_font())
	{
		gc.mult_scale(scale, scale);

		if (panel->is_antialias())
		{
			font_large_antialias.draw_text(gc, dest_geometry.left, dest_geometry.right, "a");
		}
		else
		{
			font_large.draw_text(gc, dest_geometry.left, dest_geometry.right, "a");
		}
	}

	if (panel->is_sprite())
	{
		if (panel->is_outlined())
		{
			if (panel->is_linear())
				sprite_grid_outline.set_linear_filter(true);
			if (panel->is_nearest())
				sprite_grid_outline.set_linear_filter(false);

			sprite_grid_outline.draw(gc, source_image_geometry, dest_geometry);
		}
		else
		{
			if (panel->is_linear())
				sprite_grid_normal.set_linear_filter(true);
			if (panel->is_nearest())
				sprite_grid_normal.set_linear_filter(false);

			sprite_grid_normal.draw(gc, source_image_geometry, dest_geometry);
		}
	}
	if (panel->is_texture())
	{
		if (panel->is_outlined())
		{
			if (panel->is_linear())
			{
				texture_grid_outline.set_min_filter(cl_filter_linear);
				texture_grid_outline.set_mag_filter(cl_filter_linear);
			}
			if (panel->is_nearest())
			{
				texture_grid_outline.set_min_filter(cl_filter_nearest);
				texture_grid_outline.set_mag_filter(cl_filter_nearest);
			}

			draw_texture(gc, texture_grid_outline, source_image_geometry, dest_geometry);
		}
		else
		{
			if (panel->is_linear())
			{
				texture_grid_normal.set_min_filter(cl_filter_linear);
				texture_grid_normal.set_mag_filter(cl_filter_linear);
			}
			if (panel->is_nearest())
			{
				texture_grid_normal.set_min_filter(cl_filter_nearest);
				texture_grid_normal.set_mag_filter(cl_filter_nearest);
			}

			draw_texture(gc, texture_grid_normal, source_image_geometry, dest_geometry);
		}
	}
	gc.pop_modelview();

	gc.reset_frame_buffer();
}

void GUI::show_display_image()
{
	CL_GraphicContext gc = window_ptr->get_gc();

	font.draw_text(gc, display_image_geometry.left, display_image_geometry.top - 10, "Scaled Image");

	gc.set_texture(0, display_image);
	CL_Draw::texture(gc, display_image_geometry, CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f));
	gc.reset_texture(0);

	CL_Rectf rect = display_image_geometry;

	// Draw white outline
	CL_Draw::line(gc, rect.left-1, rect.top-1, rect.right+1, rect.top-1, CL_Colorf::white);
	CL_Draw::line(gc, rect.left-1, rect.top-1, rect.left-1, rect.bottom+1, CL_Colorf::white);
	CL_Draw::line(gc, rect.left-1, rect.bottom+1, rect.right+1, rect.bottom+1, CL_Colorf::white);
	CL_Draw::line(gc, rect.right+1, rect.top-1, rect.right+1, rect.bottom+1, CL_Colorf::white);

	// Draw pixel grid (vertical lines)
	for (int xpos = 1; xpos < display_image_size.width; xpos++)
	{
		int xoffset = (xpos * display_image_geometry.get_width()) / display_image_size.width;
		xoffset += display_image_geometry.left;
		CL_Draw::line(gc, (float) xoffset, rect.top, (float) xoffset, rect.bottom, CL_Colorf::grey);
	}

	// Draw pixel grid (horizontal lines)
	for (int ypos = 1; ypos < display_image_size.height; ypos++)
	{
		int yoffset = (ypos * display_image_geometry.get_height()) / display_image_size.height;
		yoffset += display_image_geometry.top;
		CL_Draw::line(gc, rect.left, (float) (yoffset), rect.right, (float) (yoffset), CL_Colorf::grey);
	}

}

void GUI::draw_reference_images()
{
	CL_GraphicContext gc = window_ptr->get_gc();

	float xpos = 880;
	float ypos = 100;

	font.draw_text(gc, xpos, ypos - 10, "Source Image");
	if (panel->is_image())
	{
		if (panel->is_outlined())
		{
			image_grid_outline.draw(gc, xpos, ypos);
		}
		else
		{
			image_grid_normal.draw(gc, xpos, ypos);
		}

	}
	if (panel->is_sprite())
	{
		if (panel->is_outlined())
		{
			sprite_grid_outline.draw(gc, xpos, ypos);
		}
		else
		{
			sprite_grid_normal.draw(gc, xpos, ypos);
		}
	}
	if (panel->is_texture())
	{
		if (panel->is_outlined())
		{
			draw_texture(gc, texture_grid_outline, xpos, ypos);
		}
		else
		{
			draw_texture(gc, texture_grid_normal, xpos, ypos);
		}
	}

	ypos = 240;
	font.draw_text(gc, xpos, ypos - 10, "Source Area");
	CL_Rectf dest_geometry(xpos, ypos, CL_Sizef((float) source_image_geometry.get_width(), (float) source_image_geometry.get_height()));
	if (panel->is_image())
	{
		if (panel->is_outlined())
		{
			image_grid_outline.draw(gc, source_image_geometry, dest_geometry);
		}
		else
		{
			image_grid_normal.draw(gc, source_image_geometry, dest_geometry);
		}

	}
	if (panel->is_sprite())
	{
		if (panel->is_outlined())
		{
			sprite_grid_outline.draw(gc, source_image_geometry, dest_geometry);
		}
		else
		{
			sprite_grid_normal.draw(gc, source_image_geometry, dest_geometry);
		}
	}

	if (panel->is_texture())
	{
		if (panel->is_outlined())
		{
			draw_texture(gc, texture_grid_outline, source_image_geometry, dest_geometry);
		}
		else
		{
			draw_texture(gc, texture_grid_normal, source_image_geometry, dest_geometry);
		}
	}

	ypos = 380;
	font.draw_text(gc, xpos, ypos - 10, "Destination");
	gc.set_texture(0, display_image);
	CL_Draw::texture(gc, CL_Rectf(( float) xpos, (float) ypos, CL_Sizef(display_image_size)), CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f));
	gc.reset_texture(0);

}

void GUI::display_modelview_matrix(const char *matrix_name, float xpos, float ypos, const CL_Mat4f &matrix)
{
	CL_GraphicContext gc = window_ptr->get_gc();

	font.draw_text(gc, xpos, ypos - 10, matrix_name);

	const int matrix_width = 200;
	const int matrix_height = 100;

	// Draw white outline
	CL_Draw::line(gc, xpos, ypos, xpos + matrix_width, ypos, CL_Colorf::white);
	CL_Draw::line(gc, xpos, ypos + matrix_height, xpos + matrix_width, ypos + matrix_height, CL_Colorf::white);
	CL_Draw::line(gc, xpos, ypos, xpos, ypos + matrix_height, CL_Colorf::white);
	CL_Draw::line(gc, xpos+matrix_width, ypos, xpos+matrix_width, ypos + matrix_height, CL_Colorf::white);

	const int matrix_rows = 4;
	const int matrix_colums = 4;

	// Draw grid (vertical lines)
	for (int grid_xpos = 1; grid_xpos < matrix_rows; grid_xpos++)
	{
		int xoffset = (grid_xpos * matrix_width) / matrix_rows;
		xoffset += xpos;
		CL_Draw::line(gc, (float) xoffset, ypos+1, (float) xoffset, ypos + matrix_height-1, CL_Colorf::grey);
	}

	// Draw grid (horizontal lines)
	for (int grid_ypos = 1; grid_ypos < matrix_colums; grid_ypos++)
	{
		int yoffset = (grid_ypos * matrix_height) / matrix_colums;
		yoffset += ypos;
		CL_Draw::line(gc, xpos+1, (float) (yoffset), xpos + matrix_width-1, (float) (yoffset), CL_Colorf::grey);
	}

	char buffer[64];
	// Draw the values
	for (int grid_ypos = 0; grid_ypos < matrix_colums; grid_ypos++)
	{
		int yoffset = ((grid_ypos * matrix_height) + matrix_height/2) / matrix_colums;
		yoffset += ypos;
		for (int grid_xpos = 0; grid_xpos < matrix_rows; grid_xpos++)
		{
			int xoffset = ((grid_xpos * matrix_width) + matrix_width/2) / matrix_rows;
			xoffset += xpos;

			sprintf(buffer, "%#0.3f", matrix[grid_xpos * matrix_rows + grid_ypos]);
			CL_Size size = font_small.get_text_size(gc, buffer);

			font_small.draw_text(gc, xoffset - size.width/2, yoffset + size.height/2, buffer);
		}
	}

}

void GUI::draw_texture(CL_GraphicContext &gc, CL_Texture &texture, CL_Rect &source, CL_Rectf &dest)
{
	float texture_translate = panel->get_texture_translate();

	gc.set_texture(0, texture);
	CL_Rectf source_rect;
	source_rect.left = ( (float) source.left+texture_translate) / texture.get_width();
	source_rect.top = ( (float) source.top+texture_translate) / texture.get_height();
	source_rect.right = ( (float) source.right+texture_translate) / texture.get_width();
	source_rect.bottom = ( (float) source.bottom+texture_translate) / texture.get_height();
	CL_Draw::texture(gc, dest, CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f), source_rect);
	gc.reset_texture(0);
}


void GUI::draw_texture(CL_GraphicContext &gc, CL_Texture &texture, int xpos, int ypos)
{
	CL_Rectf dest_rect = CL_Rectf(( float) xpos, (float) ypos, CL_Sizef(texture.get_size()));
	CL_Rect source = CL_Rect(0,0, CL_Size(texture.get_width(), texture.get_height()));
	draw_texture(gc, texture, source, dest_rect);
}

