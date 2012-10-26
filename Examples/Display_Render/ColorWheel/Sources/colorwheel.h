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

class ColorWheel : public GUIComponent
{
public:
	ColorWheel(GraphicContext &gc, GUIManager &gui, Rect gui_position);
	virtual ~ColorWheel();

	Slider *slider_saturation_outer;
	Slider *slider_saturation_inner;
	Slider *slider_value_outer;
	Slider *slider_value_inner;
	RadioButton *radiobutton_HSV;
	RadioButton *radiobutton_HSL;

private:
	void on_render(GraphicContext &gc, const Rect &update_rect);
	void create_colorwheel(const Pointf &center, float radius);
	Slider *create_slider(int xpos, int ypos);
	void draw_labels(GraphicContext &gc);
	void get_options();
	float get_value(Slider *slider);
	void option_changed();
	void on_selected(RadioButton *radiobutton);

	float saturation_outer;
	float saturation_inner;
	float value_outer;
	float value_inner;
	bool is_hsl;

	const static int colorwheel_segments = 360 / 2;
	Vec2f colorwheel_positions[colorwheel_segments * 3];
	Vec4f colorwheel_colors[colorwheel_segments * 3];

	Font font;
};

