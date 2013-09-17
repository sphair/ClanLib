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
#include "gui.h"

int App::start(const std::vector<std::string> &args)
{
	std::string theme;
	if (clan::FileHelp::file_exists("../../../Resources/GUIThemeAero/theme.css"))
		theme = "../../../Resources/GUIThemeAero";
	else if (clan::FileHelp::file_exists("../../../Resources/GUIThemeBasic/theme.css"))
		theme = "../../../Resources/GUIThemeBasic";
	else
		throw clan::Exception("Not themes found");

	clan::GUIManager gui(theme);

	clan::DisplayWindowDescription win_desc;
	win_desc.set_allow_resize(true);
	win_desc.set_title("GUILayout Test Application");
	win_desc.set_position(clan::Rect(200, 200, 540, 440), false);
	win_desc.set_visible(false);
	clan::Window window(&gui, win_desc);
	window.func_close().set(this, &App::on_close, &window);

	clan::GUILayoutCorners layout;
	window.set_layout(layout);

	window.create_components("Resources/layout.xml");

	clan::PushButton *button = clan::PushButton::get_named_item(&window, "MyButton");
	button->func_clicked().set(this, &App::on_button_clicked, button);

	label = clan::Label::get_named_item(&window, "MyLabel");

	window.set_visible(true);

	gui.exec();
	return 0;
}

void App::on_button_clicked(clan::PushButton *button)
{
	label->set_text("You clicked " + button->get_text());
}

bool App::on_close(clan::Window *win)
{
	win->exit_with_code(0);
	return true;
}

