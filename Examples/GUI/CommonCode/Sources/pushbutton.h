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

#pragma once

#include "info.h"


class PushButton : public clan::Window
{
public:
	PushButton(clan::GUIManager &manager, clan::ResourceManager &application_resources);
	~PushButton();

private:
	bool on_close();
	void on_clicked(clan::PushButton *pushbutton);
	void on_checked_disable(clan::CheckBox *checkbox);
	void on_unchecked_disable(clan::CheckBox *checkbox);
	void on_checked_icon(clan::CheckBox *checkbox);
	void on_unchecked_icon(clan::CheckBox *checkbox);
	void on_checked_toggle(clan::CheckBox *checkbox);
	void on_unchecked_toggle(clan::CheckBox *checkbox);
	void on_unchecked_flat(clan::CheckBox *checkbox);

private:
	clan::PushButton *pushbutton1;
	clan::CheckBox *checkbox_disable;
	clan::CheckBox *checkbox_icon;
	clan::CheckBox *checkbox_toggle;

	clan::Image test_image;

	Info *info_clicked;
};
