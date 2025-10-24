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

#pragma once

#include "panel.h"

class App;

class GUI
{
public:
	GUI(App *app);
	~GUI();

	bool run();

	CL_DisplayWindow *get_window() { return window_ptr; }
	CL_GUIManager &get_gui() { return gui; }
	CL_ResourceManager &get_resources() { return resources_internal; }

private:
	void write_display_image();
	void show_display_image();
	void draw_reference_images();
	void display_modelview_matrix(const char *matrix_name, float xpos, float ypos, const CL_Mat4f &matrix);
	void draw_texture(CL_GraphicContext &gc, CL_Texture &texture, CL_Rect &source, CL_Rectf &dest);
	void draw_texture(CL_GraphicContext &gc, CL_Texture &texture, int xpos, int ypos);

private:
	CL_GUIManager gui;
	CL_ResourceManager resources_gui;
	CL_ResourceManager resources_internal;
	CL_GUIThemeDefault theme;
	App *app;
	CL_DisplayWindow *window_ptr;
	CL_GUIWindowManagerTexture wm;

	CL_AutoPtr<Panel> panel;

	CL_Sprite sprite_grid_normal;
	CL_Sprite sprite_grid_outline;
	CL_Image image_grid_normal;
	CL_Image image_grid_outline;
	CL_Texture texture_grid_normal;
	CL_Texture texture_grid_outline;

	CL_Texture display_image;
	CL_FrameBuffer display_framebuffer;

	CL_Rect source_image_geometry;
	CL_Size display_image_size;
	CL_Rect display_image_geometry;

	CL_Font_System font;
	CL_Font_System font_small;
	CL_Font_System font_large;
	CL_Font_System font_large_antialias;

	CL_TextureGroup font_texture_group;

	CL_Mat4f last_modelview_matrix;
};
