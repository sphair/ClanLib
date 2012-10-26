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
#include "gui.h"

int App::start(const std::vector<CL_String> &args)
{
	CL_String theme;
	if (CL_FileHelp::file_exists("../../../Resources/GUIThemeAero/theme.css"))
		theme = "../../../Resources/GUIThemeAero";
	else if (CL_FileHelp::file_exists("../../../Resources/GUIThemeBasic/theme.css"))
		theme = "../../../Resources/GUIThemeBasic";
	else
		throw CL_Exception("Not themes found");

	CL_GUIManager gui(theme);
	gui.add_theme("Theme");

	CL_DisplayWindowDescription win_desc;
	win_desc.set_allow_resize(true);
	win_desc.set_title("BasicGUI Test Application");
	win_desc.set_position(CL_Rect(200, 200, 640, 440), false);
	CL_Window window(&gui, win_desc);
	window.func_close().set(this, &App::on_close, &window);

	CL_Rect client_area = window.get_client_area();

	CL_PushButton button(&window);
	button.set_class_name("custom");
	button.set_geometry(CL_Rect(client_area.left + 10, client_area.top + 10, CL_Size(160, 40)));
	button.func_clicked().set(this, &App::on_button_clicked, &button);
	button.set_text("Custom Button");

	CL_PushButton button_disabled(&window);
	button_disabled.set_class_name("custom");
	button_disabled.set_geometry(CL_Rect(client_area.left + 10, client_area.top + 60, CL_Size(160, 40)));
	button_disabled.func_clicked().set(this, &App::on_button_clicked, &button_disabled);
	button_disabled.set_text("Custom Button Disabled");
	button_disabled.set_enabled(false);

	CL_PushButton button2(&window);
	button2.set_geometry(CL_Rect(client_area.left + 200, client_area.top + 10, CL_Size(160, 40)));
	button2.func_clicked().set(this, &App::on_button_clicked, &button2);
	button2.set_text("Normal Button");

	CL_PushButton button_disabled2(&window);
	button_disabled2.set_geometry(CL_Rect(client_area.left + 200, client_area.top + 60, CL_Size(160, 40)));
	button_disabled2.func_clicked().set(this, &App::on_button_clicked, &button_disabled2);
	button_disabled2.set_text("Normal Button Disabled");
	button_disabled2.set_enabled(false);

	label = new CL_Label(&window);
	label->set_geometry(CL_Rect(client_area.left + 10, client_area.top + 160, CL_Size(330, 20)));
	label->set_text("Click the button");

	gui.exec();

	return 0;
}

void App::on_button_clicked(CL_PushButton *button)
{
	label->set_text("You clicked " + button->get_text());
}

bool App::on_close(CL_Window *win)
{
	win->exit_with_code(0);
	return true;
}

