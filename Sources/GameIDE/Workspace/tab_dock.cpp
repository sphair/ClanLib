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
**    Magnus Norddahl
*/

#include "GameIDE/precomp.h"
#include "tab_dock.h"
#include "docked_frame.h"
#include "API/GameIDE/Workspace/dockable_component.h"

namespace clan
{

TabDock::TabDock(DockingGroup *parent, int id)
: Dock(parent, id), button_close(0), active_tab_index(-1)
{
	set_tag_name("tab-dock");

	header = new GUIComponent(this, "header");
	title_label = new Label(header);
	title_label->set_class("title", true);
	button_close = new PushButton(header);
	button_close->set_class("close", true);

	frame_area = new GUIComponent(this, "frame-area");

	tab_button_row = new GUIComponent(this, "tab-button-row");

	button_close->func_clicked().set(this, &TabDock::on_button_close_clicked);
}

void TabDock::on_button_close_clicked()
{
	if (!tabs.empty())
		tabs[active_tab_index]->get_dockable()->hide();
}

bool TabDock::on_input_doubleclick(const InputEvent &input_event, DockableComponent *dockable)
{
	if (input_event.id == mouse_left)
	{
		dockable->show_as_window();
		return true;
	}
	else
	{
		return false;
	}
}

bool TabDock::on_input_pressed(const InputEvent &input_event, DockableComponent *dockable)
{
	if (input_event.id == mouse_left)
	{
		set_active(dockable);
		return true;
	}
	else
	{
		return false;
	}
}

bool TabDock::is_docked(DockableComponent *dockable)
{
	return find_tab_index(dockable) != -1;
}

void TabDock::dock(DockableComponent *dockable)
{
	if (!is_docked(dockable))
	{
		int tab_index = tabs.size();

		tabs.push_back(new DockedFrame(frame_area, dockable));

		TabButton *button = new TabButton(tab_button_row);
		//button->icon->set_image(dockable->get_icon());
		button->label->set_text(dockable->get_title());

		button->func_input_pressed().set(this, &TabDock::on_input_pressed, dockable);
		button->func_input_doubleclick().set(this, &TabDock::on_input_doubleclick, dockable);

		tab_buttons.push_back(button);

		get_top_level_component()->update_layout();
	}
}

void TabDock::undock(DockableComponent *dockable)
{
	int i = find_tab_index(dockable);
	if (i != -1)
	{
		delete tabs[i];
		tabs.erase(tabs.begin() + i);

		delete tab_buttons[i];
		tab_buttons.erase(tab_buttons.begin() + i);

		active_tab_index = -1;
		activate_tab(min((int)i, ((int)tabs.size()) - 1));

		get_top_level_component()->update_layout();
	}
}

void TabDock::activate_tab(int tab_index)
{
	if (active_tab_index != tab_index)
	{
		if (active_tab_index != -1)
		{
			tabs[active_tab_index]->set_visible(false, false);
			tab_buttons[active_tab_index]->set_class("active", false);
		}

		active_tab_index = tab_index;

		tabs[active_tab_index]->set_visible(true, false);
		tab_buttons[active_tab_index]->set_class("active", true);

		title_label->set_text(tabs[active_tab_index]->get_dockable()->get_title());
	}
}

void TabDock::set_active(DockableComponent *dockable)
{
	int index = find_tab_index(dockable);
	if (index != -1)
		activate_tab(index);
}

int TabDock::find_tab_index(DockableComponent *dockable) const
{
	for (size_t i = 0; i < tabs.size(); i++)
	{
		if (tabs[i]->get_dockable() == dockable)
		{
			return i;
		}
	}
	return -1;
}

}
