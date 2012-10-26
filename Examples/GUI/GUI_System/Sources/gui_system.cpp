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
#include "gui_system.h"
#include "GUI.h"

GUI_System::GUI_System(GUI *gui) : gui(gui)
{
	clan::GUIManager *gui_manager = &gui->get_gui_manager();

	gui_manager->set_window_manager(wm);

	resources_gui = clan::ResourceManager(gui->get_resources_location());

	theme.set_resources(resources_gui);
	gui_manager->set_theme(theme); 

	gui_manager->set_css_document(gui->get_theme_location());

	// Note, clan::GUIManager deletes these automatically, after GUI_Direct has gone out of scope in the clan::GUIManager destructor
	pushbutton = new PushButton(gui->get_gui_manager(), gui->get_resources_internal());
	lineedit = new LineEdit(gui->get_gui_manager());
	checkbox = new CheckBox(gui->get_gui_manager());
	slider = new Slider(gui->get_gui_manager());
	radiobutton = new RadioButton(gui->get_gui_manager());
	scrollbar = new ScrollBar(gui->get_gui_manager());
	progressbar = new ProgressBar(gui->get_gui_manager());
	tabpage = new TabPage(gui->get_gui_manager());
	menubar = new MenuBar(gui->get_gui_manager(), gui->get_resources_internal());
	spin = new Spin(gui->get_gui_manager());
	combobox = new ComboBox(gui->get_gui_manager());
	listview = new ListView(gui->get_gui_manager());

	pushbutton->func_close().set(this, &GUI_System::on_close,  dynamic_cast<clan::Window *>(pushbutton));
	lineedit->func_close().set(this, &GUI_System::on_close,  dynamic_cast<clan::Window *>(lineedit));
	checkbox->func_close().set(this, &GUI_System::on_close,  dynamic_cast<clan::Window *>(checkbox));
	slider->func_close().set(this, &GUI_System::on_close,  dynamic_cast<clan::Window *>(slider));
	radiobutton->func_close().set(this, &GUI_System::on_close,  dynamic_cast<clan::Window *>(radiobutton));
	scrollbar->func_close().set(this, &GUI_System::on_close,  dynamic_cast<clan::Window *>(scrollbar));
	progressbar->func_close().set(this, &GUI_System::on_close,  dynamic_cast<clan::Window *>(progressbar));
	tabpage->func_close().set(this, &GUI_System::on_close,  dynamic_cast<clan::Window *>(tabpage));
	menubar->func_close().set(this, &GUI_System::on_close,  dynamic_cast<clan::Window *>(menubar));
	spin->func_close().set(this, &GUI_System::on_close,  dynamic_cast<clan::Window *>(spin));
	combobox->func_close().set(this, &GUI_System::on_close,  dynamic_cast<clan::Window *>(combobox));
	listview->func_close().set(this, &GUI_System::on_close,  dynamic_cast<clan::Window *>(listview));

}

GUI_System::~GUI_System()
{
}

bool GUI_System::run()
{
	return true;
}

bool GUI_System::on_close(clan::Window *win)
{
	gui->get_gui_manager().exit_with_code(0);
	return true;
}

