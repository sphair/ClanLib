
#include "precomp.h"
#include "dockable_component.h"
#include "dock.h"
#include "workspace.h"

using namespace clan;

DockableComponent::DockableComponent()
: GUIComponent(Workspace::instance(), "dockable"), dock(0), last_docked_id(0), visibility(state_hidden), title(title), last_visibility(state_hidden)
{
	func_resized().set(this, &DockableComponent::on_resized);
	func_render().set(this, &DockableComponent::on_render);
	Workspace::instance()->register_dockable(this);
	Workspace::instance()->hide(this);
}

void DockableComponent::set_title(const std::string &new_title)
{
	title = new_title;
	if(dock) dock->on_title_changed();
}

void DockableComponent::show()
{
	Dock *dock = 0;
	switch (last_visibility)
	{
	default:
	case state_window:
	case state_hidden:
		show_as_window();
		break;
	case state_docked:
		dock = Workspace::instance()->find_dock(last_docked_id);
		if (dock)
			show_as_docked(dock);
		else
			show_as_window();
		break;
	}
}

void DockableComponent::show_as_docked(Dock *dock)
{
	Workspace::instance()->show_as_docked(this, dock);
	last_docked_id = dock->get_id();
}

void DockableComponent::show_as_window()
{
	Workspace::instance()->show_as_window(this);
}

void DockableComponent::hide()
{
	Workspace::instance()->hide(this);
}
