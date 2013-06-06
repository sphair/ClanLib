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
#include "gui_layered.h"
#include "GUI.h"
#include "app.h"

GUI_Layered::GUI_Layered(GUI *gui) : gui(gui), canvas(gui->get_app()->get_canvas()), wm(*gui->get_app()->get_window())
{
	gui_manager = clan::GUIManager(wm, gui->get_theme_location());

	wm.func_input_intercept().set(this, &GUI_Layered::wm_input_intercept);

	// Use a texture group to store all the gui textures
	clan::TextureGroup texture_group(clan::Size(1024, 1024));
	wm.set_texture_group(texture_group);	// Note: This line is optional

	//resources_gui = clan::ResourceManager(gui->get_resources_location());

	//theme.set_resources(resources_gui);
	//gui_manager->set_theme(theme); 

	//gui_manager->set_css_document(gui->get_theme_location());


	setup_shader();

	wobble_offset = 0.0f;
	last_time = clan::System::get_time();
	lens_zoom = 3.2f;
	lens_near = 0.1f;
	lens_far = 10000.0f;
	lens_aspect = 1.0f;

	// Note, clan::GUIManager deletes these automatically, after GUI_Direct has gone out of scope in the clan::GUIManager destructor
	lineedit = new LineEdit(gui_manager);
	slider = new Slider(gui_manager);
	radiobutton = new RadioButton(gui_manager);
	scrollbar = new ScrollBar(gui_manager);
	menubar = new MenuBar(gui_manager, gui->get_resources_internal());
	spin = new Spin(gui_manager);
	combobox = new ComboBox(gui_manager);

	panel3d = new Panel3D(gui_manager);

}

GUI_Layered::~GUI_Layered()
{
}

bool GUI_Layered::run()
{
	wm.process();
	run3d();
	return true;
}

clan::ResourceManager &GUI_Layered::get_resources_internal()
{
	return gui->get_resources_internal();
}

