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

enum FontClass
{
	font_freetype,
	font_system,
	font_vector,
	font_sprite
};

// This is the Application class (That is instantiated by the Program Class)
class App
{
public:
	int start(const std::vector<CL_String> &args);

private:
	void on_button_clicked_class_freetype(CL_PushButton *button);
	void on_button_clicked_class_system(CL_PushButton *button);
	void on_button_clicked_class_vector(CL_PushButton *button);
	void on_button_clicked_class_sprite(CL_PushButton *button);
	void on_button_clicked_typeface_tahoma(CL_PushButton *button);
	void on_button_clicked_typeface_sans(CL_PushButton *button);
	void on_button_clicked_typeface_ttf(CL_PushButton *button);
	void on_checkbox_state_underline(CL_CheckBox *checkbox);
	void on_checkbox_state_italic(CL_CheckBox *checkbox);
	void on_checkbox_state_strikeout(CL_CheckBox *checkbox);
	void on_checkbox_state_antialias(CL_CheckBox *checkbox);
	void on_button_clicked_weight_light(CL_PushButton *button);
	void on_button_clicked_weight_normal(CL_PushButton *button);
	void on_button_clicked_weight_bold(CL_PushButton *button);
	void on_button_clicked_size_16(CL_PushButton *button);
	void on_button_clicked_size_32(CL_PushButton *button);
	void on_button_clicked_size_64(CL_PushButton *button);

	void on_input_up(const CL_InputEvent &key, const CL_InputState &state);
	void on_window_close();
	void on_lineedit_changed(CL_InputEvent &e);
	void render();
	void select_font();
	void draw_font_example();
	void draw_font_info();

private:
	bool quit;
	CL_DisplayWindow *window_ptr;
	CL_Window *gui_window_ptr;
	CL_GUIWindowManagerTexture *wm_ptr;
	CL_LineEdit *lineedit_text_ptr;
	CL_PushButton *button_typeface_tahoma_ptr;
	CL_PushButton *button_typeface_sans_ptr;
	CL_String font_text;
	CL_FontMetrics font_metrics;
	CL_FontDescription font_desc;
	CL_Size font_size;
	CL_Font small_font;
	CL_Font selected_font;
	FontClass selected_fontclass;
	float last_fps;
	CL_ResourceManager app_resources;
};



