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

class Theme : public clan::Window
{
public:
	enum gui_theme
	{
		theme_aero,
		theme_aero_packed,
		theme_basic,
		theme_basic_packed
	};

	Theme(clan::GUIManager &manager, gui_theme default_theme);

	void run(clan::Canvas &canvas);

	gui_theme get_theme() const {return current_theme; }

private:
	bool on_close();
	void on_theme_selected(clan::RadioButton *radiobutton);

private:
	clan::GroupBox *groupbox;

	clan::RadioButton *radiobutton_aero;
	clan::RadioButton *radiobutton_aero_packed;
	clan::RadioButton *radiobutton_basic;
	clan::RadioButton *radiobutton_basic_packed;

	gui_theme current_theme;

};

