/*
**  FMP_Example
**  Copyright (c) 2005  Erik Ylvisaker
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
**    Erik Ylvisaker
**    (if your name is missing here, please add it)
*/


#include "FMPExample.h"

#include <string>
#include <vector>

using std::string;
using std::vector;

FMP_Example app;

FMP_Example::FMP_Example(void)
{
}

FMP_Example::~FMP_Example(void)
{
}


bool done = false;
int x = 0, y = 0;
bool keyleft = false, keyright = false, keyup = false, keydown = false;

CL_FlexibleMap map;
vector<string> files;
int fileindex = 0;
CL_DisplayWindow *window_ptr = NULL;

bool display[4];

int FMP_Example::main(int argc, char **argv)
{

	CL_SetupCore setup_core;
	CL_SetupDisplay setupdisp;
	CL_SetupGL setupgl;

	CL_DisplayWindow window("CL_FlexibleMap Example", 700, 512);

	window_ptr = &window;

	// Connect the Window close event
	CL_Slot slot_quit = window.sig_window_close().connect(this, &FMP_Example::on_window_close);

	// Connect keyboard handlers
	CL_Slot slot_keyboard_up = CL_Keyboard::sig_key_up().connect(this, &FMP_Example::on_key_up);
	CL_Slot slot_keyboard_down = CL_Keyboard::sig_key_down().connect(this, &FMP_Example::on_key_down);

	// load up the names of all the map files
	CL_DirectoryScanner sc;


	if (sc.scan(".", "*.fmp"))
	{
		while (sc.next())
		{	
			files.push_back(sc.get_name());
		}

	}

	if (files.size() == 0)
	{
		// error, we need to have FMP files in the same directory for this example.
		return 0;
		
	}

	// load the first one.  Make load maps only after initializing the display.
	map.load(files[fileindex]);

	// setup frame-rate-independent animations.  For each animating tile, mappy has a parameter
	// called delay.  The parameter to set_frame_independent_animation() tells CL_FlexibleMap to wait
	// that many milliseconds times the delay parameter before cycling to the next frame.
	map.set_frame_independent_animation(10);

	window.set_title("CL_FlexibleMap Example - " + files[fileindex]);

	// turn on all the tile-layers
	for (int i = 0; i < 4; i++)
		display[i] = true;

	// how many pixels to move each frame if the arrow keys are held down
	const int stepsize = 3;

	// for demonstrating animate-once tiles
	unsigned int last_time = CL_System::get_time();

	while (!done)
	{
		// Draw a nice blue gradient in the background, so we can see where the transparency works
		CL_Display::fill_rect(
			CL_Rect(0, 0, 700, 512), 
			CL_Gradient(CL_Color::navy, CL_Color::navy, CL_Color::black, CL_Color::black));
		
		// allow for scrolling around the map
		if (CL_Keyboard::get_keycode(CL_KEY_DOWN))
			y += 3;
		if (CL_Keyboard::get_keycode(CL_KEY_LEFT))
			x -= 3;
		if (CL_Keyboard::get_keycode(CL_KEY_RIGHT))
			x += 3;
		if (CL_Keyboard::get_keycode(CL_KEY_UP))
			y -= 3;

		// call this everytime you want to update your animating tiles.  If you are using
		// frame-rate-independent animations, just call this every cycle, and the rest will
		// be taken care of.
		map.update_anim();

		// this is to demonstrate animate once tiles.  It will tell them to animate every
		// 2 seconds (2000 milliseconds)
		if (CL_System::get_time() - last_time > 2000)
		{
			last_time = CL_System::get_time();

			map.start_animate_once();
		}

		// modify settings for the base layer
		map.set_current_layer(0);

		// You can set a smaller display area, and the map will be drawn only within that area
		// for a nice effect, at the end call CL_Display::draw_rect and pass the same rectangle
		// to give the map a border.
		//map.set_display_area(CL_Rect(100, 60, 420, 260));

		// This tells the map how to interpret the draw_point chosen.  origin_center
		// will make it so that when set_draw_point is called with point x,y, that point
		// will be the center of the display area.
		// (this does not need to be called every frame)
		map.set_anchor_point(origin_center);


		// tell the layer what point should be the top-left of the screen
		map.get_current_layer().set_draw_point(x, y);

		// set how the map should display regions outside the map.  Choices are
		// border_none, border_wrap, border_default_tile
		// (this does not need to be called every frame)
		map.get_current_layer().set_border_style(CL_FlexibleMap::border_none);
		
		// if you set the border style to border_default_tile, this line of code will
		// set the default tile to whatever appears in the top-left cornder of the map.
		// These can be negative, indicating an animating tile should be used (kinda annoying, but neat that it works.)
		// (this does not need to be called every frame)
		map.get_current_layer().set_default_tile(map.get_current_layer().get_map_data(0, 0));

		// each tile structure has a background and three foreground tiles
		// drawing them is separate, so that you can draw sprites in between each layer
		if (display[0])		map.draw_background();
		if (display[1])		map.draw_foreground();
		if (display[2])		map.draw_foreground2();
		if (display[3])		map.draw_foreground3();

		// check to see if there's another layer
		if (map.get_layer_count() > 1)
		{
			// ok, draw the second layer
			map.set_current_layer(1);

			// for parallax, you can to set_draw_point(x * 2, y * 2) for example.
			map.get_current_layer().set_draw_point(x, y);


			//map.get_current_layer().set_border_style(CL_FlexibleMap::border_wrap);

			// set layer 1 to 50% transparency
			// (this does not need to be called every frame)
			map.get_current_layer().set_alpha(128);

			map.draw_background();
		}


		// Flip the display, showing on the screen what we have drawed
		// since last call to flip_display()
		CL_Display::flip();

		// This call updates input and performs other "housekeeping"
		// call this each frame
		CL_System::keep_alive(1);


	}

	// it's best to destroy your CL_FlexibleMap objects before Clanlib frees its memory.
	map.clear();

	return 0;
}

void FMP_Example::on_window_close()
{
	done = true;
}

void FMP_Example::on_key_down(const CL_InputEvent &e)
{
	// key down events for numeric keypad + and - to cycle through maps
	// keys 1-4 will toggle tile layers

	if (CL_Keyboard::get_keycode(CL_KEY_ADD))
	{
		fileindex++;

		if (fileindex >= (signed) files.size())
			fileindex = 0;

		map.load(files[fileindex]);
		map.set_frame_independent_animation(10);

		window_ptr->set_title("CL_FlexibleMap Example - " + files[fileindex]);
	}
	else if (CL_Keyboard::get_keycode(CL_KEY_SUBTRACT))
	{
		fileindex--;

		if (fileindex < 0)
			fileindex = (signed)files.size() - 1;

		map.load(files[fileindex]);
		map.set_frame_independent_animation(10);

		window_ptr->set_title("CL_FlexibleMap Example - " + files[fileindex]);

	}
	else if (CL_Keyboard::get_keycode(CL_KEY_1)) 	display[0] = !display[0];
	else if (CL_Keyboard::get_keycode(CL_KEY_2)) 	display[1] = !display[1];
	else if (CL_Keyboard::get_keycode(CL_KEY_3)) 	display[2] = !display[2];
	else if (CL_Keyboard::get_keycode(CL_KEY_4)) 	display[3] = !display[3];

}

void FMP_Example::on_key_up(const CL_InputEvent &e)
{

}
