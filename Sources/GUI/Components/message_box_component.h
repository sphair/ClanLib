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
**    Harry Storbacka
*/

#pragma once

#include "API/GUI/gui_component.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/Components/message_box.h"

namespace clan
{

class MessageBox_Impl;
class PushButton;

class MessageBoxComponent : public GUIComponent
{
public:
	MessageBoxComponent(
		GUIComponent *owner,
		std::string detail_text,
		MessageBoxButtons buttons,
		MessageBoxIcon icon);

	virtual ~MessageBoxComponent();

// Attributes:
public:

// Operations
public:
	Size layout_content();

	bool on_window_close(Window *window);

// Implementation
private:
	void on_render(Canvas &canvas, const Rect &dirty_rect);

	std::string detail_text;
	GUIThemePart part_component;
	GUIThemePart part_icon;
	GUIComponent *component;
	PushButton *button_ok;
	PushButton *button_cancel;
	PushButton *button_yes;
	PushButton *button_no;

	Rect rect_icon;
	Rect rect_text;

	MessageBoxButtons messagebox_buttons;
	MessageBoxIcon messagebox_icon;

	void create_buttons();
	void set_css_class();
	void create_parts();

	void on_button_clicked(MessageBoxResult result);
};

}
