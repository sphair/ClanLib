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
*/

#include "precomp.h"
#include "Main.h"
#include "LightContainer.h"

int ExampleCanvas::start(const std::vector<std::string> &args)
{ 
	quit = false;

	// Set a videomode - 1024x256
	clan::DisplayWindowDescription desc;
	desc.set_allow_resize(false);
	desc.set_title("ClanLib - Advanced Canvas");
	desc.set_size(clan::Size(1024, 768), true);
	desc.set_fullscreen(false);
	clan::DisplayWindow window(desc);
	canvas = clan::Canvas(window);

	// Seed randoms
	srand(clan::System::get_time());

	// Connect Slots
    clan::SlotContainer cc;
	cc.connect(window.sig_window_close(), clan::bind_member(this, &ExampleCanvas::on_window_close));
	cc.connect(window.get_ic().get_keyboard().sig_key_up(), clan::bind_member(this, &ExampleCanvas::on_keyboard_up));
	cc.connect(window.get_ic().get_mouse().sig_key_up(), clan::bind_member(this, &ExampleCanvas::on_mouse_up));
	cc.connect(window.get_ic().get_mouse().sig_pointer_move(), clan::bind_member(this, &ExampleCanvas::on_mouse_move));

	// Load the surfaces
	// -- This is our ground texture.
	background = clan::Image(canvas, "Resources/background.png");

	// -- This is the light mask that draws over everything (and is attached to our framebuffer).
	if(USE_SCALE)
		light_mask = clan::Texture2D(canvas, "Resources/scale_mask.png");
	else
		light_mask = clan::Texture2D(canvas, "Resources/full_mask.png");

	// Create the framebuffer, and attach ground texture into its color buffer
	fb_lightmask = clan::FrameBuffer(canvas);
	fb_lightmask.attach_color(0, light_mask);

	clan::Canvas canvas_fb(canvas, fb_lightmask);

	// Just a bunch of variables for keeping time and tracking FPS
	clan::GameTime game_time;

	// Some day/night cycle variables.
	float daylight = 0.50f;
	bool daylight_forward = false;

	// Setup our mouse pointer light.
	mouse_light = new Light(canvas);
	mouse_light->set_scale(3.0f, 3.0f);
	mouse_light->set_color(clan::Colorf((float)get_random(0,100)/100,(float)get_random(0,100)/100,(float)get_random(0,100)/100,0.20f));
	lights.add(mouse_light);

	// Run until someone presses escape
	while (!quit)
	{
		// Manage our time
		game_time.update();

		float micro_second = game_time.get_time_elapsed() / 1000.0f;

		// Manage our day/night cycle
		if(daylight_forward)
		{
			daylight += micro_second * 0.10f;	// Add a smaller amount of our delta time to slow the transition a little.
			if(daylight >= 1.0f)
				daylight_forward = false;
		}
		else
		{
			daylight -= micro_second * 0.10f;
			if(daylight <= 0.30f)
				daylight_forward = true;
		}

		// Updates
		lights.update(micro_second);

		// ** Draw Regular Game Objects/Images ** \\

		// Draw background
		background.draw(canvas, 0, 0);

		// Draw your normal game objects around here...

		// Draw colored lights.
		if(LIGHTS_COLOR)
			lights.draw(canvas);

		// ** Clip Light Mask ** \\

			// Clear it from last frame.
		// -- Here is where day/night can be accomplished, but interpolating the color/alpha values.
		canvas_fb.clear(clan::Colorf(0.0f, 0.0f, 0.0f, daylight));
		
		// Draw the Light cutouts
		lights.draw_clips(canvas_fb);

		// We're done making our changes to the texture, so reset the buffer.
		canvas_fb.flush();
	
		// Draw the lightmask texture
		// The color of the texture here can influence your output.  I chose to keep it simple
		// by staying white with 100% alpha.
		clan::Image light_mask_image(light_mask, light_mask.get_size());
		if(USE_SCALE)
			canvas.mult_scale(CANVAS_SCALE_X,CANVAS_SCALE_Y);
		light_mask_image.draw(canvas, clan::Rect(light_mask.get_size()));

		canvas.set_modelview(clan::Mat4f::identity());
		
		// Flip the display, showing on the screen what we have drawn (no v-sync)
		window.flip(0);

		// This call updates input and performs other "housekeeping"
		clan::KeepAlive::process();
	}

	// Cleanup
	lights.clear();

	return 0;
}

void ExampleCanvas::on_mouse_move(const clan::InputEvent &key)
{
	// Update our mouse light to reflect the current mouse position.
	mouse_light->set_translation((float)key.mouse_pos.x, (float)key.mouse_pos.y);
};

void ExampleCanvas::on_keyboard_up(const clan::InputEvent &key)
{
	if(key.id == clan::keycode_escape)
		quit = true;
}

void ExampleCanvas::on_mouse_up(const clan::InputEvent &key)
{
	// Here we create a random light.
	int r = get_random(0,100);
	int g = get_random(0,100);
	int b = get_random(0,100);
	int scale = get_random(100,500);

	Light *light = new Light(canvas);
	light->set_translation((float)get_random(50,900), (float)get_random(50,700));
	light->set_scale((float)scale/100,(float)scale/100);
	light->set_color(clan::Colorf( (float)r/100, (float)g/100, (float)b/100, 0.20f));
	light->set_life(5000);	// Die in 5 seconds.

	lights.add(light);
}

void ExampleCanvas::on_window_close()
{
	quit = true;
}

int ExampleCanvas::get_random(int low, int high)
{ 
	return low+int(rand() * (high - low) / (RAND_MAX+1.0)); 
}
