
#include "precomp.h"
#include "document_dock.h"
#include "docked_frame.h"
#include "dockable_component.h"

using namespace clan;

DocumentDock::DocumentDock(DockingGroup *parent, int id)
: Dock(parent, id), active_tab_index(-1)
{
	set_tag_name("document-dock");

	tab_button_row = new GUIComponent(this, "tab-button-row");

	frame_area = new GUIComponent(this, "frame-area");
}

void DocumentDock::on_button_close_clicked(DockableComponent *dockable)
{
	dockable->hide();
}

bool DocumentDock::on_input_pressed(const InputEvent &input_event, DockableComponent *dockable)
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

bool DocumentDock::is_docked(DockableComponent *dockable)
{
	return find_tab_index(dockable) != -1;
}

void DocumentDock::dock(DockableComponent *dockable)
{
	if (!is_docked(dockable))
	{
		int tab_index = tabs.size();

		tabs.push_back(new DockedFrame(frame_area, dockable));

		TabButton *button = new TabButton(tab_button_row);
		button->label->set_text(dockable->get_title());

		button->func_input_pressed().set(this, &DocumentDock::on_input_pressed, dockable);
		button->button_close->func_clicked().set(this, &DocumentDock::on_button_close_clicked, dockable);

		tab_buttons.push_back(button);
	}
}

void DocumentDock::undock(DockableComponent *dockable)
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
	}
}

void DocumentDock::activate_tab(int tab_index)
{
	if (active_tab_index != tab_index)
	{
		if (active_tab_index != -1)
		{
			tabs[active_tab_index]->set_visible(false, false);
			tab_buttons[active_tab_index]->set_class("");
		}

		active_tab_index = tab_index;

		tabs[active_tab_index]->set_visible(true, false);
		tab_buttons[active_tab_index]->set_class("active");
	}
}

void DocumentDock::set_active(DockableComponent *dockable)
{
	int index = find_tab_index(dockable);
	if (index != -1)
		activate_tab(index);
}

int DocumentDock::find_tab_index(DockableComponent *dockable) const
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
