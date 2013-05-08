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
#include "workspace.h"
#include "API/GameIDE/Workspace/dockable_component.h"
#include "tool_window.h"
#include "docking_group.h"
#include "view_dock.h"
#include "view_frame.h"
#include "docked_frame.h"
#include "tab_dock.h"
#include "document_dock.h"

namespace clan
{

Workspace *Workspace::_instance = 0;

Workspace::Workspace(GUIComponent *main_window)
: GUIComponent(main_window, "workspace"), root_docking_group(0), next_dockinggroup_id(1)
{
	part_empty = GUIThemePart(this, "empty-center");
	_instance = this;
	func_render().set(this, &Workspace::on_render);

	root_docking_group = new DockingGroup(this);
}

Dock *Workspace::get_root_dock()
{
	return root_docking_group->get_center();
}

void Workspace::on_render(Canvas &canvas, const Rect &update_box)
{
	if (root_docking_group->get_center()->is_empty())
	{
		int x = root_docking_group->get_geometry().left;
		int y = root_docking_group->get_geometry().top;
		Rect center_box = root_docking_group->get_center()->get_geometry().translate(x, y);
		part_empty.render_box(canvas, center_box);
	}
}

void Workspace::register_dockable(DockableComponent *dockable)
{
	dockable->set_visible(false, false);
	dockable->visibility = DockableComponent::state_hidden;
	dockable_components.push_back(dockable);
}

void Workspace::load_docking_group(std::string registry_path, DockingGroup *group)
{
#ifdef WIN32
	try
	{
		RegistryKey key(RegistryKey::key_current_user, registry_path);
		std::vector<std::string> subkeys = key.get_subkey_names();
		std::sort(subkeys.begin(), subkeys.end());
		for (size_t i = 0; i < subkeys.size(); i++)
		{
			if (subkeys[i].substr(0, 4) == "left")
			{
				DockingGroup *child_group = new DockingGroup(group);
				load_docking_group(PathHelp::combine(registry_path, subkeys[i]), child_group);
				group->left.push_back(child_group);
			}
			else if (subkeys[i].substr(0, 5) == "right")
			{
				DockingGroup *child_group = new DockingGroup(group);
				load_docking_group(PathHelp::combine(registry_path, subkeys[i]), child_group);
				group->right.push_back(child_group);
			}
			else if (subkeys[i].substr(0, 3) == "top")
			{
				DockingGroup *child_group = new DockingGroup(group);
				load_docking_group(PathHelp::combine(registry_path, subkeys[i]), child_group);
				group->top.push_back(child_group);
			}
			else if (subkeys[i].substr(0, 6) == "bottom")
			{
				DockingGroup *child_group = new DockingGroup(group);
				load_docking_group(PathHelp::combine(registry_path, subkeys[i]), child_group);
				group->bottom.push_back(child_group);
			}
			else if (subkeys[i].substr(0, 6) == "center")
			{
				load_dock(PathHelp::combine(registry_path, subkeys[i]), group);
			}
		}
	}
	catch (Exception)
	{
	}
#endif
}

void Workspace::load_dock(std::string registry_path, DockingGroup *group)
{
#ifdef WIN32
	try
	{
		RegistryKey key(RegistryKey::key_current_user, registry_path);
		int id = key.get_value_int("id");
		next_dockinggroup_id = id + 1;

		std::string type = key.get_value_string("type");
		int width = key.get_value_int("width");
		int height = key.get_value_int("height");

		Dock *dock = 0;
		if(type == "tab")
			dock = new TabDock(group, id);
		else if(type == "view")
			dock = new ViewDock(group, id);
		else if(type == "document")
			dock = new DocumentDock(group, id);

		if(dock)
		{
			group->set_center(dock);

			if (group != root_docking_group)
				group->set_geometry(Rect(Point(0,0), Size(width, height)));
		}
	}
	catch (Exception)
	{
	}
#endif
}

void Workspace::save_docking_group(std::string registry_path, DockingGroup *group)
{
#ifdef WIN32
	try
	{
		try_delete_key(registry_path);
		RegistryKey key(RegistryKey::key_current_user, registry_path);
		for (size_t i = 0; i < group->left.size(); i++)
			save_docking_group(PathHelp::combine(registry_path, string_format("left%1", (int)i)), group->left[i]);
		for (size_t i = 0; i < group->right.size(); i++)
			save_docking_group(PathHelp::combine(registry_path, string_format("right%1", (int)i)), group->right[i]);
		for (size_t i = 0; i < group->top.size(); i++)
			save_docking_group(PathHelp::combine(registry_path, string_format("top%1", (int)i)), group->top[i]);
		for (size_t i = 0; i < group->bottom.size(); i++)
			save_docking_group(PathHelp::combine(registry_path, string_format("bottom%1", (int)i)), group->bottom[i]);
		save_dock(PathHelp::combine(registry_path, "center"), group);
	}
	catch (Exception)
	{
	}
#endif
}

void Workspace::save_dock(std::string registry_path, DockingGroup *group)
{
#ifdef WIN32
	try
	{
		try_delete_key(registry_path);
		RegistryKey key(RegistryKey::key_current_user, registry_path);

		Dock *dock = group->get_center();

		key.set_value_int("id", dock->get_id());

		key.set_value_int("width", group->get_size().width);
		key.set_value_int("height", group->get_size().height);

		// We probably should have an enum or similar for this :)
		if(dynamic_cast<TabDock*> (dock))
			key.set_value_string("type", "tab");
		else if(dynamic_cast<ViewDock*> (dock))
			key.set_value_string("type", "view");
		else if(dynamic_cast<DocumentDock*> (dock))
			key.set_value_string("type", "document");
	}
	catch (Exception)
	{
	}
#endif
}

bool Workspace::try_delete_key(std::string registry_path)
{
#ifdef WIN32
	try
	{
		RegistryKey::delete_key(RegistryKey::key_current_user, registry_path, true);
		return true;
	}
	catch (Exception)
	{
		return false;
	}
#endif
}

void Workspace::load_dockables_state()
{
	load_docking_group("SOFTWARE\\Clanlib.org\\GameIDE\\Workspace\\Docks", root_docking_group);
	load_dockables("SOFTWARE\\Clanlib.org\\GameIDE\\Workspace\\Dockables");

	// No docks loaded so create a default
	if(root_docking_group->get_center() == 0)
	{
		Dock *dock = new DocumentDock(root_docking_group, generate_dock_id());
		root_docking_group->set_center(dock);
	}

	root_docking_group->update_layout();
}

void Workspace::load_dockables(std::string registry_path)
{
#ifdef WIN32
	try
	{
		RegistryKey key(RegistryKey::key_current_user, registry_path);
		std::vector<std::string> values = key.get_value_names();
		for (size_t i = 0; i < values.size(); i++)
		{
			std::string component_name = values[i];
			std::string state = key.get_value_string(values[i]);
			DockableComponent *dockable = 0;
			for (size_t j = 0; j < dockable_components.size(); j++)
			{
				if (dockable_components[j]->get_title() == component_name)
				{
					dockable = dockable_components[j];
					break;
				}
			}
			if (dockable)
				set_state_string(dockable, state);
		}
	}
	catch (Exception)
	{
	}
#endif
}

void Workspace::save_dockables_state()
{
	for (size_t i = 0; i < tool_windows.size(); i++)
		tool_windows[i]->get_dockable()->set_dockable_window_geometry(tool_windows[i]->get_window_geometry());
	
	save_docking_group("SOFTWARE\\Clanlib.org\\GameIDE\\Workspace\\Docks", root_docking_group);
	save_dockables("SOFTWARE\\Clanlib.org\\GameIDE\\Workspace\\Dockables");
}

void Workspace::save_dockables(std::string registry_path)
{
#ifdef WIN32
	try
	{
		try_delete_key(registry_path);
		RegistryKey key(RegistryKey::key_current_user, registry_path);
		for (size_t i = 0; i < dockable_components.size(); i++)
		{
			std::string component_name = dockable_components[i]->get_title();
			std::string state = get_state_string(dockable_components[i]);
			key.set_value_string(component_name, state);
		}
	}
	catch (Exception)
	{
	}
#endif
}

void Workspace::show_as_docked(DockableComponent *dockable, Dock *new_dock)
{
	if (!dockable->dock || !dockable->dock->is_docked(dockable))
	{
		hide(dockable);
		dockable->visibility = DockableComponent::state_docked;
		dockable->last_visibility = dockable->visibility;
		dockable->dock = new_dock;
		new_dock->dock(dockable);
		root_docking_group->update_layout();
	}
	else
	{
		if(dockable->dock)
		{
			dockable->dock->set_active(dockable);
		}
	}
}

void Workspace::show_as_window(DockableComponent *dockable)
{
	if (dockable->visibility != DockableComponent::state_window)
	{
		hide(dockable);
		dockable->visibility = DockableComponent::state_window;
		dockable->last_visibility = dockable->visibility;
		tool_windows.push_back(new ToolWindow(dockable, dockable->window_geometry));
		root_docking_group->update_layout();
	}
}

void Workspace::hide(DockableComponent *dockable)
{
	if (dockable->visibility != DockableComponent::state_hidden)
	{
		dockable->last_visibility = dockable->visibility;
		dockable->visibility = DockableComponent::state_hidden;
		dockable->set_visible(false, false);
		if (dockable->dock)
		{
			dockable->set_parent_component(this);
			dockable->dock->undock(dockable);
		}
		dockable->dock = 0;

		for (size_t i = 0; i < tool_windows.size(); i++)
		{
			if (tool_windows[i]->get_dockable() == dockable)
			{
				dockable->set_parent_component(this);
				delete tool_windows[i];
				tool_windows.erase(tool_windows.begin() + i);
			}
		}

		root_docking_group->update_layout();
	}
}

std::string Workspace::get_state_string(DockableComponent *dockable)
{
	Rect window_geometry = dockable->window_geometry;
	return string_format("%1;%2;%3;%4;%5;%6;%7", window_geometry.left, window_geometry.top, window_geometry.get_width(), window_geometry.get_height(), (int)dockable->visibility, (int)dockable->last_visibility, dockable->last_docked_id);
}

void Workspace::set_state_string(DockableComponent *dockable, const std::string state)
{
	try
	{
		std::vector<std::string> values = StringHelp::split_text(state, ";");
		if (values.size() == 7)
		{
			Point pos(StringHelp::text_to_int(values[0]), StringHelp::text_to_int(values[1]));
			Size size(StringHelp::text_to_int(values[2]), StringHelp::text_to_int(values[3]));
			int visibility_state = StringHelp::text_to_int(values[4]);
			int last_visibility_state = StringHelp::text_to_int(values[5]);
			int last_docked_id = StringHelp::text_to_int(values[6]);
			if (size.width > 0 && size.height > 0)
				dockable->set_window_geometry(Rect(pos, size));

			dockable->hide();
			dockable->last_docked_id = last_docked_id;

			switch (last_visibility_state)
			{
			case DockableComponent::state_window:
			case DockableComponent::state_hidden:
			case DockableComponent::state_docked:
				dockable->last_visibility = (DockableComponent::VisibilityState)last_visibility_state;
				break;
			default:
				break;
			}

			switch (visibility_state)
			{
			case DockableComponent::state_window:
			case DockableComponent::state_docked:
				dockable->show();
				break;
			default:
				break;
			}
		}
	}
	catch (Exception)
	{
		// Input comes from registry so it could be wrong.  Ignore state in that case.
	}
}

int Workspace::generate_dock_id()
{
	return next_dockinggroup_id++;
}

Dock *Workspace::find_dock(int dock_id)
{
	return root_docking_group->find_dock(dock_id);
}

}
