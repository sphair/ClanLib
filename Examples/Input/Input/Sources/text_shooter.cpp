/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
#include "text_shooter.h"

TextShooter::TextShooter(Font &new_vector_font, const std::string &new_text) : vector_font(new_vector_font), text(new_text)
{
	duration = 2 * 1000;
	start_time = 0;
	end_fade_time = 1000.0f;
	initial_white_time = 1000.0f;
	use_red_component = false;
	use_green_component = false;
	use_blue_component = false;
}

void TextShooter::set_start_position(const Vec3f &new_start_position)
{
	start_position = new_start_position;
}

void TextShooter::set_end_position(const Vec3f &new_end_position)
{
	end_position = new_end_position;
}

void TextShooter::set_duration(int new_duration)
{
	duration = new_duration;
}

void TextShooter::set_start_time(uint64_t new_start_time)
{
	start_time = new_start_time;
}

void TextShooter::set_end_fade_time(int new_end_fade_time)
{
	end_fade_time = (float) new_end_fade_time;
}

void TextShooter::set_initial_white_time(int new_initial_white_time)
{
	initial_white_time = (float) new_initial_white_time;
}

void TextShooter::set_color_component(bool new_red, bool new_green, bool new_blue)
{
	use_red_component = new_red;
	use_green_component = new_green;
	use_blue_component = new_blue;
}

bool TextShooter::draw(Canvas &canvas, uint64_t current_time)
{
	int time_delta = current_time - start_time;
	if (time_delta < 0)	// Not on screen
		return true;

	if (time_delta >= duration)	// Reached destination
		return false;

	Vec3f current_position;

	// Get position
	current_position.x = start_position.x + (((end_position.x - start_position.x) * time_delta) / duration);
	current_position.y = start_position.y + (((end_position.y - start_position.y) * time_delta) / duration);
	current_position.z = start_position.z + (((end_position.z - start_position.z) * time_delta) / duration);

	// Get fade towards end
	float font_alpha = (float) (duration  - time_delta);
	font_alpha /= end_fade_time;
	if (font_alpha > 1.0f) font_alpha = 1.0f;

	// Get initial white colour setting
	float font_white = (float) time_delta;
	font_white /= initial_white_time;
	if (font_white > 1.0f) font_white = 1.0f;
	font_white = 1.0f - font_white;

	// Calculate color
	Colorf font_color;
	if (use_red_component)
	{
		font_color.r = 1.0f;
	}
	else
	{
		font_color.r = font_white;
	}

	if (use_green_component)
	{
		font_color.g = 1.0f;
	}
	else
	{
		font_color.g = font_white;
	}

	if (use_blue_component)
	{
		font_color.b = 1.0f;
	}
	else
	{
		font_color.b = font_white;
	}

	font_color.a = font_alpha;

	// Draw the text
	current_position.x = ((current_position.x + 1.0f)  * canvas.get_width()) / 2.0f;
	current_position.y = canvas.get_height() - ((current_position.y + 1.0f)  * canvas.get_height()) / 2.0f;

	canvas.set_transform(clan::Mat4f::translate(current_position.x, current_position.y, 0.0f) * clan::Mat4f::scale(1.0f/current_position.z, 1.0f/current_position.z, 1.0f/current_position.z));

	Sizef text_size = vector_font.measure_text(canvas, text).bbox_size;

	clan::Brush brush = clan::Brush::solid(font_color);
	vector_font.draw_text(canvas, clan::Pointf(-text_size.width/2, text_size.height/4), text, font_color);
	canvas.set_transform(clan::Mat4f::identity());

	return true;
}
