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

#include "../../CommonCode/Sources/scrollbar.h"
#include "../../CommonCode/Sources/pushbutton.h"
#include "../../CommonCode/Sources/lineedit.h"
#include "../../CommonCode/Sources/checkbox.h"
#include "../../CommonCode/Sources/slider.h"
#include "../../CommonCode/Sources/radiobutton.h"
#include "../../CommonCode/Sources/progressbar.h"
#include "../../CommonCode/Sources/tabpage.h"
#include "../../CommonCode/Sources/menubar.h"
#include "../../CommonCode/Sources/spin.h"
#include "../../CommonCode/Sources/combobox.h"
#include "../../CommonCode/Sources/theme.h"
#include "../../CommonCode/Sources/listview.h"

class GUI;

class GUI_Direct
{
public:
	GUI_Direct(GUI *gui);
	~GUI_Direct();

private:

private:
	clan::ResourceManager resources_gui;
	clan::GUIThemeDefault theme;
	GUI *gui;
	clan::DisplayWindow *window_ptr;
	clan::GUIWindowManagerDirect wm;

	ScrollBar *scrollbar;
	PushButton *pushbutton;
	LineEdit *lineedit;
	CheckBox *checkbox;
	Slider *slider;
	RadioButton *radiobutton;
	ProgressBar *progressbar;
	TabPage *tabpage;
	MenuBar *menubar;
	Spin *spin;
	ComboBox *combobox;
	ListView *listview;

};



