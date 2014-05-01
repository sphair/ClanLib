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
**    Kenneth Gangstoe
*/

#include "precomp.h"
#include "game.h"
#include "tilemap.h"

void Game::run()
{
	quit = false;

	DisplayWindowDescription desc;
	desc.set_title("ClanLib TileMap Example");
	desc.set_size(Size(640, 480), true);
	desc.set_allow_resize(false);

	DisplayWindow window(desc);
    CallbackContainer cc;

	cc.connect(window.sig_window_close(), Callback<void()>(this, &Game::on_window_close));
	cc.connect(window.get_ic().get_keyboard().sig_key_up(), Callback<void()>(this, &Game::on_input_up));

	Canvas canvas(window);

	clan::XMLResourceDocument xml_resource_document("resources.xml");
	ResourceManager resources = clan::XMLResourceManager::create(xml_resource_document);

	TileMap map;
	map.load(canvas, "tavern", resources, xml_resource_document);

	// Run until someone presses escape, or closes the window
	while (!quit)
	{
		int x = window.get_ic().get_mouse().get_x() - canvas.get_width();
		int y = window.get_ic().get_mouse().get_y() - canvas.get_height();

		// ** Enable these 3 lines to display the example magnified **
		//Mat4f matrix = Mat4f::scale( 2.0f, 2.0f, 1.0f);
		//x /= 2; y /= 2;
		//canvas.set_modelview(matrix);

		map.set_scroll(x, y);

		canvas.clear(Colorf::black);

		map.draw(canvas);


		// Flip the display, showing on the screen what we have drawed since last call to flip()
		window.flip(1);

		// This call processes user input and other events
		KeepAlive::process(0);
	}
}

void Game::on_input_up(const InputEvent &key)
{
	if(key.id == keycode_escape)
	{
		quit = true;
	}
}

void Game::on_window_close()
{
	quit = true;
}
