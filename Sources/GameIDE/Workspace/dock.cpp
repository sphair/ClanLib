
#include "precomp.h"
#include "dock.h"
#include "docking_group.h"

using namespace clan;

Dock::Dock(DockingGroup *parent, int id)
: GUIComponent(parent, "dock"), id(id)
{
}

void Dock::on_title_changed()
{
	update_layout();
	request_repaint();
}
