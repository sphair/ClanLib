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
*/

#include "precomp.h"
#include "app.h"

#include "radial_menu.h"
#include "game_component.h"
#include "toolbar.h"

int App::main(const std::vector<std::string> &args)
{
	DisplayWindow display_window("Custom GUI Components", 640, 480);

	GUIManager gui(display_window, "Resources");

	display_window.sig_window_close().connect({this, &App::on_window_close, &gui});

	//  Note - If you are using the GL1 target, you will get a perfomance increase by enabling these 2 lines
	//   It reduces the number of internal FrameBuffer swaps. The GL1 target (OpenGL 1.3), performs this slowly
	//   Setting the texture group here, lets the GUI Texture Window Manager know the optimum texture size of all root components
	//TextureGroup texture_group(display_window.get_gc(), Size(1024, 1024));
	//((GUIWindowManagerTexture)gui.get_window_manager()).set_texture_group()

	Rect viewport = display_window.get_viewport();

	GameComponent game_component(viewport, &gui);

	Rect toolbar_rect = Rect((viewport.right - 448) / 2, viewport.bottom - 56, (viewport.right - 448) / 2 + 448, viewport.bottom);
	Toolbar toolbar(toolbar_rect, &game_component);	// GameComponent is the "desktop" that the toolbar sits on, as an owner

	Canvas canvas = game_component.get_canvas();
	toolbar.add_item(Sprite(canvas, "Resources/Images/spell1.png"), Sprite(canvas, "Resources/Images/spell1_selected.png"), Sprite(canvas, "Resources/Images/spell1_clicked.png"));
	toolbar.add_item(Sprite(canvas, "Resources/Images/spell2.png"), Sprite(canvas, "Resources/Images/spell2_selected.png"), Sprite(canvas, "Resources/Images/spell2_clicked.png"));
	toolbar.add_item(Sprite(canvas, "Resources/Images/spell3.png"), Sprite(canvas, "Resources/Images/spell3_selected.png"), Sprite(canvas, "Resources/Images/spell3_clicked.png"));
	toolbar.add_item(Sprite(canvas, "Resources/Images/spell4.png"), Sprite(canvas, "Resources/Images/spell4_selected.png"), Sprite(canvas, "Resources/Images/spell4_clicked.png"));
	toolbar.add_item(Sprite(canvas, "Resources/Images/spell5.png"), Sprite(canvas, "Resources/Images/spell5_selected.png"), Sprite(canvas, "Resources/Images/spell5_clicked.png"));
	toolbar.add_item(Sprite(canvas, "Resources/Images/spell6.png"), Sprite(canvas, "Resources/Images/spell6_selected.png"), Sprite(canvas, "Resources/Images/spell6_clicked.png"));
	toolbar.add_item(Sprite(canvas, "Resources/Images/spell7.png"), Sprite(canvas, "Resources/Images/spell7_selected.png"), Sprite(canvas, "Resources/Images/spell7_clicked.png"));

	gui.exec();

	return 0;
}

void App::on_window_close(GUIManager *gui)
{
	gui->exit_with_code(0);
}
