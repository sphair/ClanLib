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

#include "docking_group.h"

namespace clan
{

class DockableComponent;
class DockingGroup;

class Dock : public GUIComponent
{
public:
	Dock(DockingGroup *parent, int id);
	DockingGroup *get_group() { return static_cast<DockingGroup*>(get_parent_component()); }

	int get_id() const { return id; }

protected:
	virtual void update_layout() { }
	virtual bool is_docked(DockableComponent *dockable) = 0;
	virtual void dock(DockableComponent *dockable) = 0;
	virtual void undock(DockableComponent *dockable) = 0;
	virtual bool is_empty() const = 0;
	virtual void set_active(DockableComponent *dockable) = 0;

	void on_title_changed();

	friend class DockableComponent;
	friend class Workspace;
	friend class DockingGroup;

	int id;
};

}
