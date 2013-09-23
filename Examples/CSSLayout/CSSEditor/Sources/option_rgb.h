/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

class OptionRGB
{
public:
	OptionRGB();
	~OptionRGB();

	void setup(clan::GUIComponent *base_component, const std::string &name, int xpos, int ypos);

	bool css_enabled;
	int css_red;
	int css_green;
	int css_blue;

private:
	clan::Slider *create_slider(clan::GUIComponent *base_component, const clan::Rect &rect);
	float get_slider_value(clan::Slider *slider, float max_value);
	void set_slider_value(clan::Slider *slider, float value, float max_value);
	void slider_red_changed();
	void spin_red_changed();
	void slider_green_changed();
	void spin_green_changed();
	void slider_blue_changed();
	void spin_blue_changed();
	void checkbox_changed();
private:
	clan::CheckBox *checkbox;

	clan::Slider *slider_red;
	clan::Spin *spin_red;
	clan::Slider *slider_green;
	clan::Spin *spin_green;
	clan::Slider *slider_blue;
	clan::Spin *spin_blue;

};

