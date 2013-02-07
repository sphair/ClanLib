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
**    Magnus Norddahl
*/

#pragma once

namespace clan
{

class Dock;
class DockingGroup;
class ToolWindow;
class ViewFrame;
class DockedFrame;
class DockableComponent;

class Workspace : public GUIComponent
{
public:
	Workspace(GUIComponent *main_window);
	static Workspace *instance() { return _instance; }

	void register_dockable(DockableComponent *dockable);
	void load_dockables_state();
	void save_dockables_state();
	Dock *get_root_dock();
	Dock *find_dock(int dock_id);

	int generate_dock_id();

private:
	void on_render(Canvas &canvas, const Rect &update_box);
	void show_as_docked(DockableComponent *dockable, Dock *dock);
	void show_as_window(DockableComponent *dockable);
	void hide(DockableComponent *dockable);
	void load_docking_group(std::string registry_path, DockingGroup *group);
	void save_docking_group(std::string reigstry_path, DockingGroup *group);
	void load_dock(std::string registry_path, DockingGroup *group);
	void save_dock(std::string registry_path, DockingGroup *group);
	bool try_delete_key(std::string registry_path);
	std::string get_state_string(DockableComponent *dockable);
	void set_state_string(DockableComponent *dockable, const std::string state);
	void load_dockables(std::string registry_path);
	void save_dockables(std::string registry_path);
	DockingGroup *root_docking_group;
	GUIThemePart part_empty;
	std::vector<ToolWindow *> tool_windows;
	std::vector<DockableComponent *> dockable_components;

	static Workspace *_instance;

	friend class DockableComponent;
	friend class DockIconsOverlay;

	int next_dockinggroup_id;
};

}
