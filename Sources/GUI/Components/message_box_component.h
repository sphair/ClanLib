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
**    Harry Storbacka
*/

#pragma once

#include "API/GUI/gui_component.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/Components/message_box.h"

class CL_MessageBox_Impl;
class CL_PushButton;

class CL_MessageBoxComponent : public CL_GUIComponent
{
public:
	CL_MessageBoxComponent(
		CL_GUIComponent *owner,
		CL_String detail_text,
		CL_MessageBoxButtons buttons,
		CL_MessageBoxIcon icon);

	virtual ~CL_MessageBoxComponent();

// Attributes:
public:

// Operations
public:
	CL_Size layout_content();

	bool on_window_close(CL_Window *window);

// Implementation
private:
	void on_render(CL_GraphicContext &gc, const CL_Rect &dirty_rect);

	CL_String detail_text;
	CL_GUIThemePart part_component;
	CL_GUIThemePart part_icon;
	CL_GUIComponent *component;
	CL_PushButton *button_ok;
	CL_PushButton *button_cancel;
	CL_PushButton *button_yes;
	CL_PushButton *button_no;

	CL_Rect rect_icon;
	CL_Rect rect_text;

	CL_MessageBoxButtons messagebox_buttons;
	CL_MessageBoxIcon messagebox_icon;

	void create_buttons();
	void set_css_class();
	void create_parts();

	void on_button_clicked(CL_MessageBoxResult result);
};
