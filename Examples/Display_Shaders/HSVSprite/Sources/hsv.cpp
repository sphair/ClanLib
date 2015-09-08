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
**    Magnus Norddahl
*/

#include "precomp.h"
#include "hsv.h"
#include "hsv_sprite.h"
#include "hsv_sprite_batch.h"

clan::ApplicationInstance<HSV> clanapp;

HSV::HSV()
{
	clan::OpenGLTarget::set_current();

	window = DisplayWindow("ClanLib HSV Sprite", 1024, 768);
	sc.connect(window.sig_window_close(), clan::bind_member(this, &HSV::on_close));
	sc.connect(window.get_ic().get_keyboard().sig_key_up(), clan::bind_member(this, &HSV::on_input_up));
	canvas = Canvas(window);

	font = clan::Font("Tahoma", 11);

	sprite_batcher = std::make_shared<HSVSpriteBatch>(canvas);
	car1 = std::make_shared<HSVSprite>(canvas, sprite_batcher.get(), "Resources/spaceshoot_body_moving1.png");
	car2 = std::make_shared<HSVSprite>(canvas, sprite_batcher.get(), "Resources/ferrari_maranello.png");

	last_fps_update = System::get_time();
	last_time = last_fps_update;
}

bool HSV::update()
{
	HSVSprite *cars[] = { car1.get(), car2.get() };

	InputContext ic = window.get_ic();
	uint64_t current_time = System::get_time();
	float time_delta_ms = static_cast<float> (current_time - last_time);
	last_time = current_time;

	if (ic.get_keyboard().get_keycode(keycode_left))
		hue_offset += 0.0005f * time_delta_ms;
	else if (ic.get_keyboard().get_keycode(keycode_right))
		hue_offset -= 0.0005f * time_delta_ms;
	if (hue_offset < -1.0f)
		hue_offset += 1.0f;
	if (hue_offset > 1.0f)
		hue_offset -= 1.0f;

	canvas.clear(Colorf::darkslategrey);
	float car_hue = hue_offset;
	for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 7; x++)
		{
			cars[(x+y)%2]->draw(canvas, 60+x*128, 60+y*64, car_hue);

			car_hue += 0.02f;
			if (car_hue < -1.0f)
				car_hue += 1.0f;
			if (car_hue > 1.0f)
				car_hue -= 1.0f;
		}
	}

	fps++;
	if (System::get_time() - last_fps_update > 2000)
	{
		fps_text = string_format("%1 fps", fps/2.0f);
		last_fps_update = System::get_time();
		fps = 0;
	}

	Sizef fps_size = font.measure_text(canvas, fps_text).bbox_size;
	font.draw_text(canvas, canvas.get_width()-10-fps_size.width, 16, fps_text);
	font.draw_text(canvas, 32, 730, "Use cursor keys left and right");

	window.flip(0);

	return !quit;
}

void HSV::on_close()
{
	quit = true;
}

void HSV::on_input_up(const InputEvent &key)
{
	if(key.id == keycode_escape)
	{
		quit = true;
	}
}


