/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
		current_theme =  Theme::theme_aero;
	}
	else
	{
		current_theme =  Theme::theme_basic;
	}

	resources_internal = clan::ResourceManager("../CommonCode/Resources/resources.xml");

	clan::Canvas canvas = app->get_canvas();
	fps_font = clan::Font(canvas, "Tahoma", 24);

	balls.Init(canvas);

	gui_direct.reset(new GUI_Direct(this) );
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
		start_time = clan::System::get_time();
	}

	clan::Canvas canvas = app->get_canvas();

	canvas.fill_rect(canvas.get_size(), clan::Gradient(clan::Colorf(0.4f, 0.4f, 0.4f, 1.0f), clan::Colorf(0.0f, 0.0f, 0.0f, 1.0f)));

	gui_direct->gui_manager.render_windows();
	
	std::string fps = clan::string_format("FPS: %1", last_fps);
	fps_font.draw_text(canvas, canvas.get_width() - 100 - 2, 24 - 2, fps, clan::Colorf(0.0f, 0.0f, 0.0f, 1.0f));
	fps_font.draw_text(canvas, canvas.get_width() - 100, 24, fps, clan::Colorf::white);

	fps_font.draw_text(canvas, 24, canvas.get_height() - 48, "Rendering GUI, Directly onto the display window. Some demo windows disabled to improve FPS (in gui_direct.cpp)", clan::Colorf(1.0f, 1.0f, 1.0f, 1.0f));

	fps_count++;
	int time = clan::System::get_time();
	total_time += time - start_time;
	start_time = time;
	if(total_time >= 1000)
	{
		last_fps = fps_count;
		total_time -= 1000;
		fps_count = 0;
	}

	balls.Run(canvas);
	
	clan::KeepAlive::process();
	canvas.flush();
	app->get_window()->flip(0);

	return true;
}

const char *GUI::get_theme_location()
{
	if (current_theme == Theme::theme_aero)
		return "../../../Resources/GUIThemeAero";

	if (current_theme == Theme::theme_aero_packed)
		return "../../../Resources/GUIThemeAeroPacked";

	if (current_theme == Theme::theme_basic)
		return "../../../Resources/GUIThemeBasic";

	return "../../../Resources/GUIThemeAero";
}

clan::DisplayWindow *GUI::get_window()
{
	return app->get_window();
}

