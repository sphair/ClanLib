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
**    Mark Page
*/

#pragma once

#include "panel.h"

class App;

class GUI
{
public:
	GUI(App *app);
	~GUI();

	bool run();

	DisplayWindow *get_window() { return window_ptr; }
	GUIManager &get_gui() { return gui; }
	ResourceManager &get_resources() { return resources_internal; }

private:
	void write_display_image();
	void show_display_image();
	void draw_reference_images();
	void display_modelview_matrix(const char *matrix_name, float xpos, float ypos, const Mat4f &matrix);
	void draw_texture(GraphicContext &gc, Texture &texture, Rect &source, Rectf &dest);
	void draw_texture(GraphicContext &gc, Texture &texture, int xpos, int ypos);

private:
	GUIManager gui;
	ResourceManager resources_gui;
	ResourceManager resources_internal;
	GUIThemeDefault theme;
	App *app;
	DisplayWindow *window_ptr;
	GUIWindowManagerTexture wm;

	std::auto_ptr<Panel> panel;

	Sprite sprite_grid_normal;
	Sprite sprite_grid_outline;
	Image image_grid_normal;
	Image image_grid_outline;
	Texture texture_grid_normal;
	Texture texture_grid_outline;

	Texture display_image;
	FrameBuffer display_framebuffer;

	Rect source_image_geometry;
	Size display_image_size;
	Rect display_image_geometry;

	Font_System font;
	Font_System font_small;
	Font_System font_large;
	Font_System font_large_antialias;

	TextureGroup font_texture_group;

	Mat4f last_modelview_matrix;
};
