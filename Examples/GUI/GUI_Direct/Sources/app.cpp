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
#include "app.h"
#include "balls.h"

App::App() : quit(false)
{
}

// The start of the Application
int App::start(const std::vector<std::string> &args)
{
	clan::DisplayWindowDescription win_desc;
	win_desc.set_allow_resize(true);
	win_desc.set_title("GUI Direct Application");
	win_desc.set_size(clan::Size( 1100, 900 ), false);

	clan::DisplayWindow window(win_desc);

	window.sig_window_close().connect({this, &App::on_window_close});
    window.get_ic().get_keyboard().sig_key_up().connect({this, &App::on_input_up});

	clan::Canvas canvas(window);
	Balls balls;
	balls.Init(canvas);
	clan::Font fps_font(canvas, "tahoma", 20);

	std::string theme;
	if (clan::FileHelp::file_exists("../../../Resources/GUIThemeAero/theme.css"))
		theme = "../../../Resources/GUIThemeAero";
	else if (clan::FileHelp::file_exists("../../../Resources/GUIThemeBasic/theme.css"))
		theme = "../../../Resources/GUIThemeBasic";
	else
		throw clan::Exception("Not themes found");

	clan::GUIWindowManagerDirect wm_direct(window, canvas);
	clan::GUIManager gui(wm_direct, theme);

	win_desc.set_position(canvas.get_size(), false);
	clan::GUIComponent *window_component = new clan::GUIComponent(&gui, win_desc, "");

	clan::PushButton *button = new clan::PushButton(window_component);
	button->set_geometry(clan::Rect(10, 10, clan::Size(160, 40)));
	button->func_clicked().set(this, &App::on_button_clicked, button);
	button->set_text("Button #1");

	clan::PushButton *button2 = new clan::PushButton(window_component);
	button2->set_geometry(clan::Rect(200, 10, clan::Size(160, 40)));
	button2->func_clicked().set(this, &App::on_button_clicked, button2);
	button2->set_text("Button #2");

	label = new clan::Label(window_component);
	label->set_geometry(clan::Rect(10, 160, clan::Size(330, 20)));
	label->set_text("Click the button");

	clan::GameTime game_time;
	while(!quit)
	{
		game_time.update();

		canvas.fill_rect(canvas.get_size(), clan::Gradient(clan::Colorf(0.8f, 0.8f, 0.8f, 1.0f), clan::Colorf(0.0f, 0.0f, 1.0f, 1.0f)));

		gui.render_windows();
	
		balls.Run(canvas, game_time.get_time_elapsed());
	
		std::string fps = clan::string_format("%1 fps", clan::StringHelp::float_to_text(game_time.get_updates_per_second(), 1));
		fps_font.draw_text(canvas, canvas.get_width() - 100, 30, fps);

		window.flip(0);
		clan::KeepAlive::process();
	}

	return 0;
}

void App::on_button_clicked(clan::PushButton *button)
{
	label->set_text("You clicked " + button->get_text());
}

// A key was pressed
void App::on_input_up(const clan::InputEvent &key)
{
	if(key.id == clan::keycode_escape)
	{
		quit = true;
	}
}

// The window was closed
void App::on_window_close()
{
	quit = true;
}

