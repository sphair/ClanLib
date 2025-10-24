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

class GUI;

class Panel
{
public:
	Panel(GUI *gui);

	bool is_sprite();
	bool is_image();
	bool is_font();
	bool is_texture();
	bool is_linear();
	bool is_nearest();
	bool is_outlined();
	bool is_antialias();
	float get_scale();
	float get_rotation();
	float get_translate();
	float get_texture_translate();

private:
	void on_slider_changed(CL_Slider *slider);
	void set_slider_to_spin(CL_Slider *slider, CL_Spin *spin);
	void on_spin_changed(CL_Spin *spin);
	void on_imagetype_selected(CL_RadioButton *radiobutton);

private:
	GUI *gui;
	CL_Window *window;

	CL_RadioButton *radiobutton_sprite;
	CL_RadioButton *radiobutton_image;
	CL_RadioButton *radiobutton_texture;
	CL_RadioButton *radiobutton_font;

	CL_RadioButton *radiobutton_linear;
	CL_RadioButton *radiobutton_nearest;

	CL_CheckBox *checkbox_outline;
	CL_CheckBox *checkbox_antialias;

	CL_Slider *slider_scale;
	CL_Spin *spin_scale;
	CL_Label *label_scale;

	CL_Slider *slider_rotation;
	CL_Spin *spin_rotation;
	CL_Label *label_rotation;

	CL_Slider *slider_translate;
	CL_Spin *spin_translate;
	CL_Label *label_translate;
	CL_Label *label_translate2;

	CL_Slider *slider_texture_translate;
	CL_Spin *spin_texture_translate;
	CL_Label *label_texture_translate;
	CL_Label *label_texture_translate2;

};

