
#include "precomp.h"
#include "view_dock.h"
#include "view_frame.h"

using namespace clan;

ViewDock::ViewDock(DockingGroup *parent, int id)
: Dock(parent, id)
{
	func_resized().set(this, &ViewDock::on_resized);
}

void ViewDock::update_layout()
{
	on_resized();
}

bool ViewDock::is_docked(DockableComponent *dockable)
{
	for (size_t i = 0; i < views.size(); i++)
	{
		if (views[i]->get_dockable() == dockable)
			return true;
	}
	return false;
}

void ViewDock::dock(DockableComponent *dockable)
{
	if (!is_docked(dockable))
	{
		views.push_back(new ViewFrame(this, dockable));
	}
}

void ViewDock::undock(DockableComponent *dockable)
{
	for (size_t i = 0; i < views.size(); i++)
	{
		if (views[i]->get_dockable() == dockable)
		{
			delete views[i];
			views.erase(views.begin() + i);
			break;
		}
	}
}

void ViewDock::set_active(DockableComponent *dockable)
{
}

void ViewDock::on_resized()
{
	Rect client_box = get_size();
	if (views.size() == 1)
	{
		views[0]->set_geometry(client_box);
	}
	else
	{
		size_t vert_views = (views.size() + 1) / 2;
		for (size_t i = 0; i < vert_views; i++)
		{
			int top = client_box.top + i * client_box.get_height() / vert_views;
			int bottom = client_box.top + (i + 1) * client_box.get_height() / vert_views;
			Rect view_box1(client_box.left, top, client_box.left + (client_box.get_width() - 5) / 2, bottom);
			Rect view_box2(client_box.left + 5 + (client_box.get_width() - 5) / 2, top, client_box.right, bottom);
			views[i * 2]->set_geometry(view_box1);
			if (i * 2 + 1 < views.size())
				views[i * 2 + 1]->set_geometry(view_box2);
		}
	}
}
