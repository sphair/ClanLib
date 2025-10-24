/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
	if (CL_FileHelp::file_exists("../../../Resources/GUIThemeAero/theme.css"))
	{
		current_theme = new_theme = theme_aero;
	}
	else
	{
		current_theme = new_theme = theme_basic;
	}

	current_manager = manager_texture;
//	current_manager = manager_system;

	resources_internal = CL_ResourceManager("Resources/resources.xml");

	CL_GraphicContext gc = app->get_window()->get_gc();
	font = CL_Font(gc, "Tahoma", 16);
	fps_font = CL_Font(gc, "Tahoma", 24);

	reset_manager();
}

GUI::~GUI()
{
}

bool GUI::run()
{
	static int total_time = 0, fps_count = 0, last_fps= 0;
	static int start_time = 0;

	if (start_time == 0)
	{
		start_time = CL_System::get_time();
	}

	check_manager_change();

	gui_manager.exec(false);

	CL_GraphicContext gc = app->get_window()->get_gc();

	gc.set_map_mode(CL_MapMode(cl_map_2d_upper_left));

	CL_Draw::gradient_fill(gc, app->get_window()->get_viewport(), CL_Gradient(CL_Colorf(0.4f, 0.4f, 0.4f, 1.0f), CL_Colorf(0.0f, 0.0f, 0.0f, 1.0f)));

	run_manager();

	CL_String fps = cl_format("FPS: %1", last_fps);
	fps_font.draw_text(gc, gc.get_width() - 100 - 2, 24 - 2, fps, CL_Colorf(0.0f, 0.0f, 0.0f, 1.0f));
	fps_font.draw_text(gc, gc.get_width() - 100, 24, fps, CL_Colorf::white);

	fps_count++;
	int time = CL_System::get_time();
	total_time += time - start_time;
	start_time = time;
	if(total_time >= 1000)
	{
		last_fps = fps_count;
		total_time -= 1000;
		fps_count = 0;
	}

	app->get_window()->flip(0);

	return true;
}

void GUI::check_manager_change()
{
	if (current_theme == new_theme)
	{
		if (actual_manager == current_manager)
			return;
	}

	reset_manager();
}

void GUI::reset_manager()
{
	current_theme = new_theme;
	actual_manager = current_manager;
	app->get_window()->set_cursor(cl_cursor_wait);

	if (current_manager == manager_texture)
	{
		gui_system.reset();
		gui_texture.reset();

		gui_texture.reset(new GUI_Texture(this) );
	}

	if (current_manager == manager_system)
	{
		gui_system.reset();
		gui_texture.reset();

		gui_system.reset(new GUI_System(this) );
	}
	app->get_window()->set_cursor(cl_cursor_arrow);

}

void GUI::run_manager()
{
	if (gui_texture.get())
		gui_texture->run();

	if (gui_system.get())
		gui_system->run();
}

void GUI::set_constant_repaint(bool enable)
{
	if (current_manager == manager_texture)
	{
		gui_texture->set_constant_repaint(enable);
	}

	if (current_manager == manager_system)
	{
		gui_system->set_constant_repaint(enable);
	}
}

void GUI::set_theme(gui_theme theme)
{
	// TODO: It would be nice to be able to change the theme dynamically, without having to rebuild the windows
	new_theme = theme;
}

const char *GUI::get_theme_location()
{
	if (current_theme == theme_aero)
		return "../../../Resources/GUIThemeAero/theme.css";

	if (current_theme == theme_aero_packed)
		return "../../../Resources/GUIThemeAeroPacked/theme.css";

	if (current_theme == theme_basic)
		return "../../../Resources/GUIThemeBasic/theme.css";

	if (current_theme == theme_basic_packed)
		return "../../../Resources/GUIThemeBasicPacked/theme.css";

	if (current_theme == theme_luna_packed)
		return "../../../Resources/GUIThemeLunaPacked/theme.css";

	return "../../../Resources/GUIThemeLuna/theme.css";
}

const char *GUI::get_resources_location()
{
	if (current_theme == theme_aero)
		return "../../../Resources/GUIThemeAero/resources.xml";

	if (current_theme == theme_aero_packed)
		return "../../../Resources/GUIThemeAeroPacked/resources.xml";

	if (current_theme == theme_basic)
		return "../../../Resources/GUIThemeBasic/resources.xml";

	if (current_theme == theme_basic_packed)
		return "../../../Resources/GUIThemeBasicPacked/resources.xml";

	if (current_theme == theme_luna_packed)
		return "../../../Resources/GUIThemeLunaPacked/resources.xml";

	return "../../../Resources/GUIThemeLuna/resources.xml";
}

CL_DisplayWindow *GUI::get_window()
{
	return app->get_window();
}

