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
#include "app.h"

clan::ApplicationInstance<App> clanapp;

App::App()
{
	// We support all display targets, in order listed here
#ifdef WIN32
	clan::D3DTarget::set_current();
#endif
	clan::OpenGLTarget::set_current();
	// Set the window
	clan::DisplayWindowDescription desc;
	desc.set_title("ClanLib Basic Sound Example");
	desc.set_size(clan::Size(640, 480), true);
	desc.set_allow_resize(true);

	window = clan::DisplayWindow(desc);
	canvas = clan::Canvas(window);

	// Connect the Window close event
	sc.connect(window.sig_window_close(), [&](){quit = true; });

	// Connect a keyboard handler to on_key_up()
	sc.connect(window.get_ic().get_keyboard().sig_key_up(), clan::bind_member(this, &App::on_input_up));
	sc.connect(window.get_ic().get_keyboard().sig_key_down(), clan::bind_member(this, &App::on_input_down));

	sfx_pacman_start = clan::SoundBuffer("Resources/start.wav");
	sfx_cheer = clan::SoundBuffer("Resources/cheer1.ogg");
	sound_output = clan::SoundOutput(44100, 192);

	font = clan::Font("tahoma", 24);

	game_time.reset();
}

bool App::update()
{
	canvas.clear(clan::Colorf(0.0f, 0.0f, 0.2f));
	font.draw_text(canvas, 32, 32, "Press 1 or 2", clan::Colorf::white);
	window.flip(1);

	return !quit;
}

// A key was pressed
void App::on_input_up(const clan::InputEvent &key)
{
	if (key.id == clan::keycode_escape)
	{
		quit = true;
	}

}

void App::on_input_down(const clan::InputEvent &key)
{
	if (key.str == "1")
	{
		sfx_pacman_start.play();
	}
	if (key.str == "2")
	{
		sfx_cheer.play();
	}
}

