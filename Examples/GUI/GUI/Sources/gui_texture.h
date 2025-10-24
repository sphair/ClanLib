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

#pragma once

#include "scrollbar.h"
#include "pushbutton.h"
#include "lineedit.h"
#include "checkbox.h"
#include "slider.h"
#include "radiobutton.h"
#include "progressbar.h"
#include "tabpage.h"
#include "menubar.h"
#include "spin.h"
#include "combobox.h"
#include "manager.h"
#include "listview.h"

class GUI;

class GUI_Texture
{
public:
	GUI_Texture(GUI *gui);
	~GUI_Texture();

	bool run();

	CL_DisplayWindow *get_window() { return window_ptr; }
	CL_GUIManager &get_gui_manager();
	CL_ResourceManager &get_resources_internal();
	CL_ResourceManager &get_resources_gui();

	void set_constant_repaint(bool enable);

private:
	void gui_repaint();
	void set_projection_matrix();
	float get_zpos_at_position(float xpos, float ypos);
	void setup_shader();

private:
	CL_ResourceManager resources_gui;
	CL_GUIThemeDefault theme;
	GUI *gui;
	CL_DisplayWindow *window_ptr;
	CL_GUIWindowManagerTexture wm;

	CL_AutoPtr<ScrollBar> scrollbar;
	CL_AutoPtr<PushButton> pushbutton;
	CL_AutoPtr<LineEdit> lineedit;
	CL_AutoPtr<CheckBox> checkbox;
	CL_AutoPtr<Slider> slider;
	CL_AutoPtr<RadioButton> radiobutton;
	CL_AutoPtr<ProgressBar> progressbar;
	CL_AutoPtr<TabPage> tabpage;
	CL_AutoPtr<MenuBar> menubar;
	CL_AutoPtr<Spin> spin;
	CL_AutoPtr<ComboBox> combobox;
	CL_AutoPtr<Manager> manager;
	CL_AutoPtr<ListView> listview;

};



