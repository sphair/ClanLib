
#include "precomp.h"
#include "docked_frame.h"
#include "workspace.h"
#include "dockable_component.h"

using namespace clan;

DockedFrame::DockedFrame(GUIComponent *parent, DockableComponent *dockable)
: GUIComponent(parent, "docked-frame"), dockable(dockable)
{
	func_resized().set(this, &DockedFrame::on_resized);
	func_render().set(this, &DockedFrame::on_render);
	dockable->set_parent_component(this);
	dockable->set_visible(true, false);
	set_visible(false, false);
	on_resized();
	request_repaint();
}

void DockedFrame::on_render(Canvas &canvas, const Rect &update_box)
{
}

void DockedFrame::on_resized()
{
	dockable->set_geometry(get_size());
}
