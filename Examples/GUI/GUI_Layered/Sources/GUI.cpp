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
#include "GUI.h"
#include "app.h"

GUI::GUI(App *app) : app(app)
{
	if (clan::FileHelp::file_exists("../../../Resources/GUIThemeAero/theme.css"))
	{
		current_theme = new_theme = Theme::theme_aero;
	}
	else
	{
		current_theme = new_theme = Theme::theme_basic;
	}

	resources_internal = clan::ResourceManager("../CommonCode/Resources/resources.xml");

	clan::Canvas canvas = app->get_window()->get_canvas();
	fps_font = clan::Font(canvas, "Tahoma", 24);

	reset_manager();
}

GUI::~GUI()
{
}

bool GUI::run(clan::GameTime &game_time)
{

	clan::Canvas canvas = app->get_window()->get_canvas();

	canvas.set_map_mode(clan::MapMode(map_2d_upper_left));
	canvas.set_viewport(canvas.get_size());
	canvas.set_modelview(clan::Mat4f::identity());

	canvas.clear(clan::Colorf(0.0f,0.0f,0.0f, 0.0f));

	run_manager();

	std::string fps = clan::string_format("FPS: %1", clan::StringHelp::float_to_text(game_time.get_updates_per_second(), 1));
	fps_font.draw_text(canvas, canvas.get_width() - 100 - 2, 24 - 2, fps, clan::Colorf(0.0f, 0.0f, 0.0f, 1.0f));
	fps_font.draw_text(canvas, canvas.get_width() - 100, 24, fps, clan::Colorf(4.0f, 4.0f, 1.0f, 1.0f));

	fps_font.draw_text(canvas, 24, canvas.get_height() - 48, "Rendering GUI onto a clan::Texture, then onto the layered OpenGL window.",  clan::Colorf(4.0f, 4.0f, 1.0f, 1.0f));

	clan::KeepAlive::process();
	app->get_window()->flip(0);

	if (gui_manager.get_exit_flag())
		return false;

	return true;
}

void GUI::reset_manager()
{
	current_theme = new_theme;
	app->get_window()->set_cursor(cursor_wait);

	gui_layered.reset(new GUI_Layered(this) );

	app->get_window()->set_cursor(cursor_arrow);

}

void GUI::run_manager()
{
	if (gui_layered.get())
		gui_layered->run();
}

void GUI::set_theme(Theme::gui_theme theme)
{
	// TODO: It would be nice to be able to change the theme dynamically, without having to rebuild the windows
	new_theme = theme;
}

const char *GUI::get_theme_location()
{
	if (current_theme == Theme::theme_aero)
		return "../../../Resources/GUIThemeAero/theme.css";

	if (current_theme == Theme::theme_aero_packed)
		return "../../../Resources/GUIThemeAeroPacked/theme.css";

	if (current_theme == Theme::theme_basic)
		return "../../../Resources/GUIThemeBasic/theme.css";

//	if (current_theme == Theme::theme_basic_packed)
	return "../../../Resources/GUIThemeBasicPacked/theme.css";
}

const char *GUI::get_resources_location()
{
	if (current_theme == Theme::theme_aero)
		return "../../../Resources/GUIThemeAero/resources.xml";

	if (current_theme == Theme::theme_aero_packed)
		return "../../../Resources/GUIThemeAeroPacked/resources.xml";

	if (current_theme == Theme::theme_basic)
		return "../../../Resources/GUIThemeBasic/resources.xml";

//	if (current_theme == Theme::theme_basic_packed)
	return "../../../Resources/GUIThemeBasicPacked/resources.xml";
}

clan::DisplayWindow *GUI::get_window()
{
	return app->get_window();
}

