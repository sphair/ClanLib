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

#pragma once

#include "gui_layered.h"

#include "../../CommonCode/Sources/theme.h"

class App;

class GUI
{
public:
	GUI(App *app);
	~GUI();

	bool run();

	clan::DisplayWindow *get_window();
	App *get_app() { return app; }
	clan::GUIManager &get_gui_manager() { return gui_manager; }
	clan::ResourceManager &get_resources_internal() { return resources_internal; }

	void set_theme(Theme::gui_theme theme);
	Theme::gui_theme get_theme() {return current_theme;}

	const char *get_theme_location();
	const char *get_resources_location();

private:
	void gui_repaint();
	void gui_exec();
	void run_manager();
	void reset_manager();

private:
	clan::GUIManager gui_manager;
	clan::ResourceManager resources_internal;
	App *app;
	clan::Font fps_font;
	 std::unique_ptr<GUI_Layered> gui_layered;

	Theme::gui_theme new_theme;
	Theme::gui_theme current_theme;
};



