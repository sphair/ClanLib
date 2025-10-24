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
**    Mark Page
*/

#include "precomp.h"
#include "gui_system.h"
#include "GUI.h"
#include "app.h"

GUI_System::GUI_System(GUI *gui) : gui(gui)
{
	CL_GUIManager *gui_manager = &gui->get_gui_manager();

	gui_manager->set_window_manager(wm);

	resources_gui = CL_ResourceManager(gui->get_resources_location());

	theme.set_resources(resources_gui);
	gui_manager->set_theme(theme); 

	gui_manager->set_css_document(gui->get_theme_location());

	pushbutton.reset(new PushButton(gui) );
	lineedit.reset(new LineEdit(gui) );
	checkbox.reset(new CheckBox(gui) );
	slider.reset(new Slider(gui) );
	radiobutton.reset(new RadioButton(gui) );
	scrollbar.reset(new ScrollBar(gui) );
	progressbar.reset(new ProgressBar(gui) );
	tabpage.reset(new TabPage(gui) );
	menubar.reset(new MenuBar(gui) );
	spin.reset(new Spin(gui) );
	combobox.reset(new ComboBox(gui) );
	manager.reset(new Manager(gui) );
	listview.reset(new ListView(gui) );
}

GUI_System::~GUI_System()
{
}

bool GUI_System::run()
{
	return true;
}

CL_ResourceManager &GUI_System::get_resources_internal()
{
	return gui->get_resources_internal();
}

CL_ResourceManager &GUI_System::get_resources_gui()
{
	return resources_gui;
}

CL_GUIManager &GUI_System::get_gui_manager()
{
	return gui->get_gui_manager();
}

void GUI_System::set_constant_repaint(bool enable)
{
	pushbutton->set_constant_repaint(enable);
	lineedit->set_constant_repaint(enable);
	checkbox->set_constant_repaint(enable);
	slider->set_constant_repaint(enable);
	radiobutton->set_constant_repaint(enable);
	scrollbar->set_constant_repaint(enable);
	progressbar->set_constant_repaint(enable);
	tabpage->set_constant_repaint(enable);
	menubar->set_constant_repaint(enable);
	spin->set_constant_repaint(enable);
	combobox->set_constant_repaint(enable);
	manager->set_constant_repaint(enable);
	listview->set_constant_repaint(enable);
}
