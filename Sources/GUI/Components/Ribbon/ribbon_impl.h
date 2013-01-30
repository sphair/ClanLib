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
**    Magnus Norddahl
**    Harry Storbacka
**    Mark Page
*/

#pragma once

namespace clan
{

class Ribbon;

class Ribbon_Impl
{
public:
	Ribbon_Impl();
	~Ribbon_Impl();

	void add_page(RibbonPage *page);
	void on_menu_button_clicked();
	void on_render(Canvas &canvas, const Rect &clip_rect);
	void on_resized();
	bool on_input_pressed(const InputEvent &e);
	bool on_input_released(const InputEvent &e);
	bool on_input_pointer_moved(const InputEvent &e);
	void paint_tabs(Canvas &canvas, const Rect &clip_rect);

	Ribbon *component;

	PushButton *menu_button;
	RibbonMenu *menu;
	std::vector<RibbonPage *> pages;
	GUIThemePart part_tab;
	GUIThemePart part_tab_background;
	unsigned int current_page_index;

};

}
