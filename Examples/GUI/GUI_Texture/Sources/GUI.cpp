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

	resources_internal = clan::DisplayCache("../CommonCode/Resources/resources.xml");

	canvas = clan::Canvas(*app->get_window());
	fps_font = clan::Font(canvas, "Tahoma", 24);

	balls.Init(canvas);

	reset_manager();
}

GUI::~GUI()
{
}

bool GUI::run(clan::GameTime &game_time)
{
	canvas.set_map_mode(clan::MapMode(clan::map_2d_upper_left));

	canvas.fill_rect(app->get_window()->get_viewport(), clan::Gradient(clan::Colorf(0.4f, 0.4f, 0.4f, 1.0f), clan::Colorf(0.0f, 0.0f, 0.0f, 1.0f)));

	run_manager(canvas);

	std::string fps = clan::string_format("FPS: %1", clan::StringHelp::float_to_text(game_time.get_updates_per_second(), 1));
	fps_font.draw_text(canvas, canvas.get_width() - 100 - 2, 24 - 2, fps, clan::Colorf(0.0f, 0.0f, 0.0f, 1.0f));
	fps_font.draw_text(canvas, canvas.get_width() - 100, 24, fps, clan::Colorf::white);

	fps_font.draw_text(canvas, 24, canvas.get_height() - 16, "Rendering GUI onto a clan::Texture, then onto the display window.", clan::Colorf(1.0f, 1.0f, 1.0f, 1.0f));

	balls.Run(canvas, game_time.get_time_elapsed());

	clan::KeepAlive::process();
	canvas.flip(0);

	return true;
}


void GUI::reset_manager()
{
	current_theme = new_theme;
	app->get_window()->set_cursor(clan::cursor_wait);

	gui_texture.reset();
	gui_texture.reset(new GUI_Texture(this) );

	app->get_window()->set_cursor(clan::cursor_arrow);

}

void GUI::run_manager(clan::Canvas &canvas)
{
	if (gui_texture.get())
	{
		new_theme = gui_texture->get_theme();
		if (new_theme != current_theme)
			reset_manager();
	}

	if (gui_texture.get())
	{
		gui_texture->run(canvas);
	}

}

const char *GUI::get_theme_location()
{
	if (current_theme == Theme::theme_aero)
		return "../../../Resources/GUIThemeAero";

	if (current_theme == Theme::theme_aero_packed)
		return "../../../Resources/GUIThemeAeroPacked";

	if (current_theme == Theme::theme_basic)
		return "../../../Resources/GUIThemeBasic";

	if (current_theme == Theme::theme_basic_packed)
		return "../../../Resources/GUIThemeBasicPacked";

	return "../../../Resources/GUIThemeAero";
}

