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
	void on_slider_changed(Slider *slider);
	void set_slider_to_spin(Slider *slider, Spin *spin);
	void on_spin_changed(Spin *spin);
	void on_imagetype_selected(RadioButton *radiobutton);

private:
	GUI *gui;
	Window *window;

	RadioButton *radiobutton_sprite;
	RadioButton *radiobutton_image;
	RadioButton *radiobutton_texture;
	RadioButton *radiobutton_font;

	RadioButton *radiobutton_linear;
	RadioButton *radiobutton_nearest;

	CheckBox *checkbox_outline;
	CheckBox *checkbox_antialias;

	Slider *slider_scale;
	Spin *spin_scale;
	Label *label_scale;

	Slider *slider_rotation;
	Spin *spin_rotation;
	Label *label_rotation;

	Slider *slider_translate;
	Spin *spin_translate;
	Label *label_translate;
	Label *label_translate2;

	Slider *slider_texture_translate;
	Spin *spin_texture_translate;
	Label *label_texture_translate;
	Label *label_texture_translate2;

};

