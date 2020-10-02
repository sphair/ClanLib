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
#include "timing.h"

clan::ApplicationInstance<Timing> clanapp;

Timing::Timing()
{
#if defined(WIN32) && !defined(__MINGW32__)
	clan::D3DTarget::set_current();
#else
	clan::OpenGLTarget::set_current();
#endif

	// Set the window
	clan::DisplayWindowDescription desc;
	desc.set_title("ClanLib Timing Example");
	desc.set_size(clan::Size(800, 600), true);
	desc.set_allow_resize(true);

	window = clan::DisplayWindow(desc);
	// Connect the Window close event
	sc.connect(window.sig_window_close(), clan::bind_member(this, &Timing::on_window_close));

	// Connect a keyboard handler to on_key_up()
	sc.connect(window.get_keyboard().sig_key_up(), clan::bind_member(this, &Timing::on_input_up));

	// Get the graphic context
	canvas = clan::Canvas(window);

	set_stars(canvas, 100);

	game_time = clan::GameTime(60, 60);
}

bool Timing::update()
{
	game_time.update();

	canvas.clear(clan::Colorf(0.0f,0.0f,0.0f));

	draw_graphics(canvas, game_time.get_time_elapsed());

	window.flip();

	return !quit;
}

// A key was pressed
void Timing::on_input_up(const clan::InputEvent &key)
{
	if(key.id == clan::keycode_escape)
	{
		quit = true;
	}
}

// The window was closed
void Timing::on_window_close()
{
	quit = true;
}

void Timing::draw_graphics(clan::Canvas &canvas, float time_delta)
{
	int gc_width = canvas.get_width();
	std::vector<Star>::size_type max, cnt;
	max = stars.size();
	for (cnt=0; cnt<max; cnt++)
	{
		float xpos = stars[cnt].xpos;
		xpos += time_delta * stars[cnt].speed;
		if (xpos >= gc_width)
			xpos -= (gc_width + 8);
		stars[cnt].xpos = xpos;

		canvas.fill_circle(xpos, stars[cnt].ypos, 6.0f, stars[cnt].color);
	}
}

void Timing::set_stars(clan::Canvas &canvas, int star_cnt)
{
	stars.clear();
	stars.resize(star_cnt);
	unsigned int random = 1231;
	int gc_width = canvas.get_width();
	int gc_height = canvas.get_height();

	for (int cnt=0; cnt < star_cnt; cnt++)
	{
		stars[cnt].xpos = (float) (random % gc_width);
		random+= 143222321;
		stars[cnt].ypos = (float) (random % gc_height);
		random+= 89079086;
		stars[cnt].speed = (((float) (random % 256)) ) + 10.0f;
		random*= 595443965;
		stars[cnt].color.r = (((float) (random % 256)) / 256.0f);
		random*= 196243625;
		stars[cnt].color.b = (((float) (random % 256)) / 256.0f);
		random*= 14365;
		stars[cnt].color.g = (((float) (random % 256)) / 256.0f);
		stars[cnt].color.a = 1.0f;

	}

}

