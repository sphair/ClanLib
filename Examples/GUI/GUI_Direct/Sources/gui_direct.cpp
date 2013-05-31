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
#include "gui_direct.h"
#include "GUI.h"
#include "app.h"

GUI_Direct::GUI_Direct(GUI *gui) : gui(gui), window_ptr(gui->get_app()->get_window()), wm(*window_ptr, gui->get_app()->get_canvas())
{
	gui_manager = clan::GUIManager(wm, gui->get_theme_location());

	//resources_gui = clan::DisplayCache(gui->get_resources_location());
	//gui_manager->set_css_document(gui->get_theme_location());

	// Note, clan::GUIManager deletes these automatically, after GUI_Direct has gone out of scope in the clan::GUIManager destructor
	pushbutton = new PushButton(gui_manager, gui->get_resources_internal());
	lineedit = new LineEdit(gui_manager);
	//checkbox = new CheckBox(gui_manager);
	slider = new Slider(gui_manager);
	//radiobutton = new RadioButton(gui_manager);
	scrollbar = new ScrollBar(gui_manager);
	//progressbar = new ProgressBar(gui_manager);
	//tabpage = new TabPage(gui_manager);
	//menubar = new MenuBar(gui_manager, gui->get_resources_internal());
	//spin = new Spin(gui_manager);
	combobox = new ComboBox(gui_manager);
	//listview = new ListView(gui_manager);
}

GUI_Direct::~GUI_Direct()
{
}
