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

#include "precomp.h"
#include "info.h"

Info::Info(clan::GUIComponent *parent): clan::GUIComponent(parent, "Info")
{
	activated_time = 0;
	activated_flag = false;
	func_render().set(this, &Info::on_render);
}

Info::~Info()
{

}

void Info::set(int new_xpos, int new_ypos, const std::string &new_name)
{
	name = new_name;
	activated_time = 0;
	activated_flag = false;
	request_repaint();

	set_geometry(clan::Rect(new_xpos, new_ypos, clan::Size(120, 16)));
}

void Info::set_comment(std::string &new_comment)
{
	comment = new_comment;
	request_repaint();
}

void Info::draw(clan::Canvas &canvas)
{
	std::string text_to_draw = name;

	clan::Rect draw_rect = get_geometry();

	int draw_xpos = 0;
	int draw_ypos = 0;

	clan::Font font = get_font();

	clan::FontMetrics metrics = font.get_font_metrics();
	draw_ypos += (int) metrics.get_ascent();

	clan::Colorf color;

	int time_diff = clan::System::get_time() - activated_time;
	float color_value = 1.0f - ( ( (float) time_diff ) / 1000.0f);
	if ( (color_value <= 1.0f) && (color_value > 0.0f) )
	{
		color = clan::Colorf(color_value, color_value/2.0f, color_value/2.0f, color_value);
		font.draw_text(canvas, draw_xpos, draw_ypos, "#", color);
		color = clan::Colorf(color_value, color_value/2.0f, color_value/2.0f, 1.0f);
		text_to_draw = name + comment;

		set_constant_repaint(true);
	}
	else
	{
		color = clan::Colorf(0.0f, 0.0f, 0.0f, 1.0f);
		set_constant_repaint(false);
	}

	font.draw_text(canvas, draw_xpos + 16-1, draw_ypos-1, text_to_draw, color);

}

void Info::activate()
{
	activated_time = clan::System::get_time();
	activated_flag = true;
	request_repaint();
}

void Info::on_render(clan::Canvas &canvas, const clan::Rect &update_rect)
{
	draw(canvas);
}
