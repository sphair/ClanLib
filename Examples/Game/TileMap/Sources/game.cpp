/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
**    Kenneth Gangstoe
*/

#include "precomp.h"
#include "game.h"
#include "tilemap.h"

void Game::run()
{
	quit = false;

	CL_DisplayWindowDescription desc;
	desc.set_title("ClanLib TileMap Example");
	desc.set_size(CL_Size(640, 480), true);
	desc.set_allow_resize(false);

	CL_DisplayWindow window(desc);

	CL_Slot slot_quit = window.sig_window_close().connect(this, &Game::on_window_close);
	CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &Game::on_input_up);

	CL_GraphicContext gc = window.get_gc();

	CL_ResourceManager resources("resources.xml");

	TileMap map;
	map.load(gc, "tavern", resources);

	// Run until someone presses escape, or closes the window
	while (!quit)
	{
		int x = window.get_ic().get_mouse().get_x() - gc.get_width();
		int y = window.get_ic().get_mouse().get_y() - gc.get_height();

		// ** Enable these 3 lines to display the example magnified **
		//CL_Mat4f matrix = CL_Mat4f::scale( 2.0f, 2.0f, 1.0f);
		//x /= 2; y /= 2;
		//gc.set_modelview(matrix);

		map.set_scroll(x, y);

		gc.clear(CL_Colorf::black);

		map.draw(gc);

		// Flip the display, showing on the screen what we have drawed since last call to flip()
		window.flip(1);

		// This call processes user input and other events
		CL_KeepAlive::process(0);
	}
}

void Game::on_input_up(const CL_InputEvent &key, const CL_InputState &state)
{
	if(key.id == CL_KEY_ESCAPE)
	{
		quit = true;
	}
}

void Game::on_window_close()
{
	quit = true;
}
