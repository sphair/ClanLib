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
#include "particle.h"
#include "Demos/simple.h"
#include "Demos/circle.h"
#include "Demos/circle2.h"
#include "Demos/cmotion.h"
#include "Demos/explosion.h"
#include "Demos/msmall.h"
#include "Demos/shooting.h"
#include "Demos/usercollision.h"

// The start of the Application
int Particle::start(const std::vector<std::string> &args)
{
	quit = false;

	// Set the window
	clan::DisplayWindow window("LinearParticle Example - Main Menu", 640, 480, false);

	// Connect the Window close event
    clan::CallbackContainer cc;
	cc.connect(window.sig_window_close(), clan::Callback<void()>(this, &Particle::on_window_close));

	// Get the graphic context
	clan::Canvas canvas(window);

	clan::Font font(canvas, "tahoma", 20);

	// Run until someone presses escape
	while (!quit)
	{
		canvas.clear(clan::Colorf(0.0f,0.0f,0.2f));

		int ypos = 32;
		const int ygap = 24;
		font.draw_text(canvas, 32, ypos, "Linear Particle Example - Main Menu");
		ypos += ygap*2;
		font.draw_text(canvas, 32, ypos, "1) Simple"); ypos += ygap;
		font.draw_text(canvas, 32, ypos, "2) Circle"); ypos += ygap;
		font.draw_text(canvas, 32, ypos, "3) Circle2"); ypos += ygap;
		font.draw_text(canvas, 32, ypos, "4) MSmall"); ypos += ygap;
		font.draw_text(canvas, 32, ypos, "5) Shooting"); ypos += ygap;
		font.draw_text(canvas, 32, ypos, "6) Explosion"); ypos += ygap;
		font.draw_text(canvas, 32, ypos, "7) CMotion"); ypos += ygap;
		font.draw_text(canvas, 32, ypos, "8) UserCollision"); ypos += ygap;

		ypos += ygap;
		font.draw_text(canvas, 32, ypos, "Press Escape to exit the example and return to this screen."); ypos += ygap;

		clan::InputDevice keyboard = window.get_ic().get_keyboard();
		if (keyboard.get_keycode(clan::keycode_escape))
			break;

		bool reset_title_flag = false;
		if (keyboard.get_keycode(clan::keycode_1))
		{
			DemoSimple demo; demo.run(window); reset_title_flag = true;
		}
		else if (keyboard.get_keycode(clan::keycode_2))
		{
			DemoCircle demo; demo.run(window); reset_title_flag = true;
		}
		else if (keyboard.get_keycode(clan::keycode_3))
		{
			DemoCircle2 demo; demo.run(window); reset_title_flag = true;
		}
		else if (keyboard.get_keycode(clan::keycode_4))
		{
			DemoMSmall demo; demo.run(window); reset_title_flag = true;
		}
		else if (keyboard.get_keycode(clan::keycode_5))
		{
			DemoShooting demo; demo.run(window); reset_title_flag = true;
		}
		else if (keyboard.get_keycode(clan::keycode_6))
		{
			DemoExplosion demo; demo.run(window); reset_title_flag = true;
		}
		else if (keyboard.get_keycode(clan::keycode_7))
		{
			DemoCMotion demo; demo.run(window); reset_title_flag = true;
		}
		else if (keyboard.get_keycode(clan::keycode_8))
		{
			DemoUserCollision demo; demo.run(window); reset_title_flag = true;
		}

		if (reset_title_flag)
			window.set_title("LinearParticle Example - Main Menu");

		window.flip(1);

		clan::KeepAlive::process(0);
	}

	return 0;
}

// The window was closed
void Particle::on_window_close()
{
	quit = true;
}



