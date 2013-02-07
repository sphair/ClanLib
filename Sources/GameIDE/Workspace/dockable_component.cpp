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
*/

#include "GameIDE/precomp.h"
#include "API/GameIDE/Workspace/dockable_component.h"
#include "dock.h"
#include "workspace.h"

namespace clan
{

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

}
