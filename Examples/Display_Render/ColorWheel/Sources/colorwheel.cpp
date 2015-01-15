/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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

#include "colorwheel.h"
/*
ColorWheel::ColorWheel(clan::Canvas &canvas, clan::GUIManager &gui, clan::Rect gui_position) : clan::GUIComponent(&gui, clan::GUITopLevelDescription("ColorWheel", gui_position, false))
{
	clan::CSSValueBackgroundColor color;
	color.color = clan::Colorf::black;
	set_style(color, true);	// We want a black background


	slider_saturation_outer = create_slider(16, 16);
	slider_saturation_inner = create_slider(16, 48);
	slider_value_outer = create_slider(16, 80);
	slider_value_inner = create_slider(16, 112);

	slider_saturation_outer->set_position(slider_saturation_outer->get_max());
	slider_saturation_inner->set_position(0);
	slider_value_outer->set_position(slider_value_outer->get_max());
	slider_value_inner->set_position(0);

	radiobutton_HSV = new clan::RadioButton(this);
	radiobutton_HSV->set_geometry(clan::Rect(16, 0 , clan::Size(64, 16)));
	radiobutton_HSV->set_text("         ");		// ** I'm got lazy to set the text color to white, so I draw it elsewhere
	radiobutton_HSV->set_selected(true);
	radiobutton_HSV->set_group_name("Main Group");
	radiobutton_HSV->func_selected() = bind_member(this, &ColorWheel::on_selected, radiobutton_HSV);

	radiobutton_HSL = new clan::RadioButton(this);
	radiobutton_HSL->set_geometry(clan::Rect(100, 0, clan::Size(64, 16)));
	radiobutton_HSL->set_text("         ");
	radiobutton_HSL->set_group_name("Main Group");
	radiobutton_HSL->func_selected() = bind_member(this, &ColorWheel::on_selected, radiobutton_HSL);

	radiobutton_HSV->set_selected(true);

	func_render() = bind_member(this, &ColorWheel::on_render);

	font = clan::Font(canvas, "tahoma", 16);

}

ColorWheel::~ColorWheel()
{

}

void ColorWheel::on_render(clan::Canvas &canvas, const clan::Rect &update_rect)
{
	get_options();

}

void ColorWheel::get_options()
{
	saturation_outer = get_value(slider_saturation_outer);
	saturation_inner = get_value(slider_saturation_inner);
	value_outer = get_value(slider_value_outer);
	value_inner = get_value(slider_value_inner);
}

float ColorWheel::get_value(clan::Slider *slider)
{
	float value = (float) slider->get_position();
	value /= (float) slider->get_max();
	return value;
}

clan::Slider *ColorWheel::create_slider(int xpos, int ypos)
{
	clan::Slider *component = new clan::Slider(this);
	component->set_geometry(clan::Rect(xpos, ypos, clan::Size(256, 17)));
	component->set_vertical(false);
	component->set_horizontal(true);
	component->set_min(0);
	component->set_max(1000);
	component->set_tick_count(100);
	component->set_page_step(100);
	component->set_position(0);
	component->set_lock_to_ticks(false);

	component->func_value_changed() = bind_member(this, &ColorWheel::option_changed);

	return component;

}

void ColorWheel::option_changed()
{
	request_repaint();
}

void ColorWheel::on_selected(clan::RadioButton *radiobutton)
{
	if (radiobutton == radiobutton_HSL)
	{
		is_hsl = true;
	}
	else
	{
		is_hsl = false;
	}
	request_repaint();

}
*/
