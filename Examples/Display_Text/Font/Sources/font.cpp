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

#include "precomp.h"
#include "font.h"

// The start of the Application
int App::start(const std::vector<CL_String> &args)
{
	quit = false;

	CL_DisplayWindowDescription win_desc;
	win_desc.set_allow_resize(true);
	win_desc.set_title("Font Example Application");
	win_desc.set_size(CL_Size( 1000, 700 ), false);

	CL_DisplayWindow window(win_desc);
	window_ptr = &window;
	CL_Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close);
	CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

	app_resources = CL_ResourceManager("Resources/resources.xml");

	CL_String theme;
	if (CL_FileHelp::file_exists("../../../Resources/GUIThemeAero/theme.css"))
		theme = "../../../Resources/GUIThemeAero";
	else if (CL_FileHelp::file_exists("../../../Resources/GUIThemeBasic/theme.css"))
		theme = "../../../Resources/GUIThemeBasic";
	else
		throw CL_Exception("No themes found");

	CL_GUIWindowManagerTexture wm(window);
	CL_GUIManager gui(wm, theme);

	wm_ptr = &wm;

	CL_GUITopLevelDescription gui_desc;
	gui_desc.set_title("Options");
	gui_desc.set_position(CL_Rect(10, 10, 250, 400), false);
	CL_Window gui_window(&gui, gui_desc);
	gui_window_ptr = &gui_window;

	int offset_x = 10;
	int offset_y = 40;
	int width = 200;
	int height = 20;
	const int gap = 26;

	CL_PushButton button_class_system(&gui_window);
	button_class_system.set_geometry(CL_Rect(offset_x, offset_y, offset_x + width, offset_y + height));
	button_class_system.func_clicked().set(this, &App::on_button_clicked_class_system, &button_class_system);
	button_class_system.set_text("Class: System");
	offset_y += gap;

	CL_PushButton button_class_freetype(&gui_window);
	button_class_freetype.set_geometry(CL_Rect(offset_x, offset_y, offset_x + width, offset_y + height));
	button_class_freetype.func_clicked().set(this, &App::on_button_clicked_class_freetype, &button_class_freetype);
	button_class_freetype.set_text("Class: Freetype");
	offset_y += gap;

	CL_PushButton button_class_vector(&gui_window);
	button_class_vector.set_geometry(CL_Rect(offset_x, offset_y, offset_x + width, offset_y + height));
	button_class_vector.func_clicked().set(this, &App::on_button_clicked_class_vector, &button_class_vector);
	button_class_vector.set_text("Class: Vector");
	offset_y += gap;

	CL_PushButton button_class_sprite(&gui_window);
	button_class_sprite.set_geometry(CL_Rect(offset_x, offset_y, offset_x + width, offset_y + height));
	button_class_sprite.func_clicked().set(this, &App::on_button_clicked_class_sprite, &button_class_sprite);
	button_class_sprite.set_text("Class: Sprite");
	offset_y += gap;

	CL_PushButton button_typeface_tahoma(&gui_window);
	button_typeface_tahoma_ptr = &button_typeface_tahoma;
	button_typeface_tahoma.set_geometry(CL_Rect(offset_x, offset_y, offset_x + width, offset_y + height));
	button_typeface_tahoma.func_clicked().set(this, &App::on_button_clicked_typeface_tahoma, &button_typeface_tahoma);
	button_typeface_tahoma.set_text("Typeface: Tahoma");
	offset_y += gap;

	CL_PushButton button_typeface_sans(&gui_window);
	button_typeface_sans_ptr = &button_typeface_sans;
	button_typeface_sans.set_geometry(CL_Rect(offset_x, offset_y, offset_x + width, offset_y + height));
	button_typeface_sans.func_clicked().set(this, &App::on_button_clicked_typeface_sans, &button_typeface_sans);
	button_typeface_sans.set_text("Typeface: Microsoft Sans Serif");
	offset_y += gap;

	CL_CheckBox checkbox1(&gui_window);
	checkbox1.set_geometry(CL_Rect(offset_x, offset_y, offset_x + 80, offset_y + height));
	checkbox1.func_state_changed().set(this, &App::on_checkbox_state_underline, &checkbox1);
	checkbox1.set_text("Underline");

	CL_CheckBox checkbox2(&gui_window);
	checkbox2.set_geometry(CL_Rect(offset_x+100, offset_y, offset_x + 180, offset_y + height));
	checkbox2.func_state_changed().set(this, &App::on_checkbox_state_strikeout, &checkbox2);
	checkbox2.set_text("Strikeout");
	offset_y += gap;

	CL_CheckBox checkbox3(&gui_window);
	checkbox3.set_geometry(CL_Rect(offset_x, offset_y, offset_x + 80, offset_y + height));
	checkbox3.func_state_changed().set(this, &App::on_checkbox_state_italic, &checkbox3);
	checkbox3.set_text("Italic");

	CL_CheckBox checkbox4(&gui_window);
	checkbox4.set_geometry(CL_Rect(offset_x+100, offset_y, offset_x + 180, offset_y + height));
	checkbox4.func_state_changed().set(this, &App::on_checkbox_state_antialias, &checkbox4);
	checkbox4.set_text("Anti Alias");
	offset_y += gap;

	CL_PushButton button_weight_light(&gui_window);
	button_weight_light.set_geometry(CL_Rect(offset_x, offset_y, offset_x + 60, offset_y + height));
	button_weight_light.func_clicked().set(this, &App::on_button_clicked_weight_light, &button_weight_light);
	button_weight_light.set_text("Light");
	CL_PushButton button_weight_normal(&gui_window);
	button_weight_normal.set_geometry(CL_Rect(offset_x+70, offset_y, offset_x + 130, offset_y + height));
	button_weight_normal.func_clicked().set(this, &App::on_button_clicked_weight_normal, &button_weight_normal);
	button_weight_normal.set_text("Normal");
	CL_PushButton button_weight_bold(&gui_window);
	button_weight_bold.set_geometry(CL_Rect(offset_x+140, offset_y, offset_x + 200, offset_y + height));
	button_weight_bold.func_clicked().set(this, &App::on_button_clicked_weight_bold, &button_weight_bold);
	button_weight_bold.set_text("Bold");
	offset_y += gap;

	CL_PushButton button_size_16(&gui_window);
	button_size_16.set_geometry(CL_Rect(offset_x, offset_y, offset_x + 60, offset_y + height));
	button_size_16.func_clicked().set(this, &App::on_button_clicked_size_16, &button_size_16);
	button_size_16.set_text("Size 16");
	CL_PushButton button_size_32(&gui_window);
	button_size_32.set_geometry(CL_Rect(offset_x+70, offset_y, offset_x + 130, offset_y + height));
	button_size_32.func_clicked().set(this, &App::on_button_clicked_size_32, &button_size_32);
	button_size_32.set_text("Size 32");
	CL_PushButton button_size_64(&gui_window);
	button_size_64.set_geometry(CL_Rect(offset_x+140, offset_y, offset_x + 200, offset_y + height));
	button_size_64.func_clicked().set(this, &App::on_button_clicked_size_64, &button_size_64);
	button_size_64.set_text("Size 64");
	offset_y += gap;

	font_text = "Ω(The quick brown fox 0123456789)";

	CL_LineEdit lineedit1(&gui_window);
	lineedit_text_ptr = &lineedit1;
	lineedit1.set_geometry(CL_Rect(offset_x, offset_y, offset_x + width, offset_y + 30));
	lineedit1.set_text(font_text); 
	lineedit1.func_after_edit_changed().set(this, &App::on_lineedit_changed);

	last_fps = 0.0f;
	selected_fontclass = font_system;
	font_desc.set_typeface_name("Microsoft Sans Serif");
	font_desc.set_height(32);
	font_desc.set_weight(400);
	select_font();

	CL_GraphicContext gc = window_ptr->get_gc();
	small_font = CL_Font(gc, "Tahoma", 16);

	while(!quit)
	{
		render();
	}

	small_font = CL_Font();
	selected_font = CL_Font();

	return 0;
}

// A key was pressed
void App::on_input_up(const CL_InputEvent &key, const CL_InputState &state)
{
	if(key.id == CL_KEY_ESCAPE)
	{
		quit = true;
	}
}

// The window was closed
void App::on_window_close()
{
	quit = true;
}

void App::render()
{
	int start_time = CL_System::get_time();

	CL_GraphicContext gc = window_ptr->get_gc();
	gc.set_map_mode(CL_MapMode(cl_map_2d_upper_left));

	gc.clear(CL_Colorf(0.0f,0.0f,0.2f));

	std::vector<CL_GUIWindowManagerTextureWindow> windows = wm_ptr->get_windows();
	std::vector<CL_GUIWindowManagerTextureWindow>::size_type index, size;
	size = windows.size();
	for (index = 0; index < size; index++)
	{
		CL_GUIWindowManagerTextureWindow window = windows[index];
		CL_Subtexture subtexture = window.get_texture();
		CL_Image image(gc, subtexture);
		image.draw(gc, window.get_geometry().left, window.get_geometry().top);
	}

	draw_font_example();
	draw_font_info();

	last_fps = 1000.0f / (CL_System::get_time()-start_time);

	wm_ptr->process();
	wm_ptr->draw_windows(gc);

	window_ptr->flip(1);

	CL_KeepAlive::process();
}

void App::select_font()
{
	CL_GraphicContext gc = window_ptr->get_gc();
	switch (selected_fontclass)
	{
		case font_freetype:
			selected_font = CL_Font_Freetype(gc, font_desc);
			break;
		case font_system:
			selected_font = CL_Font_System(gc, font_desc);
			break;
		case font_vector:
			selected_font = CL_Font_Vector(font_desc);
			break;
		case font_sprite:
			selected_font = CL_Font_Sprite(gc, "ClanFont", &app_resources);
			break;
	}

	font_metrics = selected_font.get_font_metrics(gc);
	font_size = selected_font.get_text_size(gc, font_text);
}

void App::draw_font_example()
{
	int offset_x = 10;
	int offset_y = 600;
	int descender = (int) font_metrics.get_descent();

	CL_GraphicContext gc = window_ptr->get_gc();
	
	// Surrounding box
	CL_Draw::fill(gc, CL_Rect(offset_x, offset_y+descender, offset_x + font_size.width, offset_y+descender - font_size.height), CL_Colorf(0.0f, 0.0f, 0.0f));

	// Draw the external leading line
	int external_leading_offset = offset_y+font_metrics.get_external_leading() + font_metrics.get_descent();
	CL_Draw::line(gc, offset_x, external_leading_offset, offset_x + font_size.width, external_leading_offset, CL_Colorf(0.5f, 0.5f, 0.0f));

	// Draw the internal leading line
	int internal_leading_offset = offset_y - font_metrics.get_ascent() + font_metrics.get_internal_leading();
	CL_Draw::line(gc, offset_x, internal_leading_offset, offset_x + font_size.width, internal_leading_offset, CL_Colorf(0.5f, 0.5f, 0.0f));

	// Draw outline to the bounding box
	CL_Draw::line(gc, offset_x, offset_y+descender, offset_x + font_size.width, offset_y+descender, CL_Colorf(1.0f, 0.0f, 0.0f));
	CL_Draw::line(gc, offset_x, offset_y+descender- font_size.height, offset_x + font_size.width, offset_y+descender - font_size.height, CL_Colorf(1.0f, 0.0f, 0.0f));
	CL_Draw::line(gc, offset_x + font_size.width, offset_y+descender, offset_x + font_size.width, offset_y+descender - font_size.height, CL_Colorf(1.0f, 0.0f, 0.0f));
	CL_Draw::line(gc, offset_x, offset_y+descender, offset_x, offset_y+descender - font_size.height, CL_Colorf(1.0f, 0.0f, 0.0f));

	// Base line
	CL_Draw::line(gc, offset_x, offset_y, offset_x + font_size.width, offset_y, CL_Colorf(0.0f, 1.0f, 0.0f));

	selected_font.draw_text(gc, offset_x, offset_y, font_text,  CL_Colorf::white);
}
