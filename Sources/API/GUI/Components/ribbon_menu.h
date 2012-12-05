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
*/

#pragma once

#include "../../Display/2D/image.h"
#include "../gui_component.h"
#include "../gui_message_input.h"
#include "../gui_theme_part.h"

namespace clan
{

class RibbonMenuItem
{
public:
	RibbonMenuItem() : separator(false) { }
	RibbonMenuItem(Image image, std::string text) : separator(false), image(image), text(text) { }

	bool separator;
	Image image;
	std::string text;
	Callback_v0 func_clicked;
};

class RibbonMenu : public GUIComponent
{
public:
	Callback_v0 &add_item(Image image, std::string text) { items.push_back(RibbonMenuItem(image, text)); return items.back().func_clicked; }
	void add_separator() { items.push_back(RibbonMenuItem()); items.back().separator = true; }

private:
	RibbonMenu(GUIComponent *owner);
	void start(const Point &pos);
	void end();
	void on_render(Canvas &canvas, const Rect &update_box);
	void on_resized();
	void on_filter_message(std::shared_ptr<GUIMessage> &message);
	void on_filter_input_message(GUIMessage_Input &message);
	int update_item_boxes();
	static GUITopLevelDescription create_toplevel_description();

	int current_item;
	std::vector<RibbonMenuItem> items;
	std::vector<Rect> item_boxes;
	GUIThemePart part_menu_item;
	GUIThemePart part_menu_item_selected;
	GUIThemePart part_menu_item_separator;

	bool running;
	Slot slot_filter;

	friend class Ribbon;
};

}
