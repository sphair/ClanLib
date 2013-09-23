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

class Options : public clan::GUIComponent
{
public:
	Options(clan::GUIManager &gui, clan::Rect gui_position);
	virtual ~Options();

	int width;
private:
	void update_all_slider_text();
	void slider_width_changed();
	void set_value(clan::Slider *slider, float value, float max_value);
	clan::Label *create_slider_label(clan::Slider *slider);
	void on_render(clan::Canvas &canvas, const clan::Rect &update_rect);
	clan::Slider *create_slider(int xpos, int ypos);
	float get_value(clan::Slider *slider, float max_value);

private:
	clan::Slider *slider_width;
	clan::Label *label_width;

};

