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

class OptionValue
{
public:
	OptionValue();
	~OptionValue();

	void setup(clan::GUIComponent *base_component, const std::string &name, int xpos, int ypos);

	std::string css_type_length;
	float css_value;
	bool css_enabled;

private:
	void update_spin();
	float get_slider_value(float max_value);
	void set_slider_value(float value, float max_value);
	void slider_changed();
	void combobox_changed(int index);
	void spin_changed();
	void checkbox_changed();
private:
	clan::Slider *slider;
	clan::CheckBox *checkbox;
	clan::ComboBox *combobox;
	clan::PopupMenu popupmenu;
	clan::Spin *spin;

};

