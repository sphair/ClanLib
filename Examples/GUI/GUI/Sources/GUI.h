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

#pragma once

#include "gui_texture.h"
#include "gui_system.h"

class App;

enum ManagerType
{
	manager_system,
	manager_texture
};

class GUI
{
public:
	GUI(App *app);
	~GUI();

	enum gui_theme
	{
		theme_aero,
		theme_aero_packed,
		theme_luna,
		theme_luna_packed,
		theme_basic,
		theme_basic_packed
	};

	bool run();

	CL_DisplayWindow *get_window();
	App *get_app() { return app; }
	CL_Font &get_font() { return font; }
	CL_GUIManager &get_gui_manager() { return gui_manager; }
	CL_ResourceManager &get_resources_internal() { return resources_internal; }

	ManagerType get_manager() {return actual_manager;}
	void set_manager(ManagerType manager) {current_manager = manager;}

	void set_constant_repaint(bool enable);

	void set_theme(gui_theme theme);
	gui_theme get_theme() {return current_theme;}

	const char *get_theme_location();
	const char *get_resources_location();

private:
	void gui_repaint();
	void gui_exec();
	void check_manager_change();
	void run_manager();
	void reset_manager();

private:
	CL_GUIManager gui_manager;
	CL_ResourceManager resources_internal;
	App *app;
	CL_Font font;
	CL_Font fps_font;
	CL_AutoPtr<GUI_Texture> gui_texture;
	CL_AutoPtr<GUI_System> gui_system;

	gui_theme new_theme;
	gui_theme current_theme;

	ManagerType current_manager;
	ManagerType actual_manager;		// Used to detect if the manager has changed
};



