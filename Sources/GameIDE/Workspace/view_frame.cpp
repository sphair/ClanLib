
#include "precomp.h"
#include "view_frame.h"
#include "workspace.h"
#include "dockable_component.h"

using namespace clan;

ViewFrame::ViewFrame(GUIComponent *parent, DockableComponent *dockable)
: GUIComponent(parent, "view-frame"), dockable(dockable)
{
	part_header = GUIThemePart(this, "header");
	func_resized().set(this, &ViewFrame::on_resized);
	func_render().set(this, &ViewFrame::on_render);
	func_input_doubleclick().set(this, &ViewFrame::on_input_doubleclick);
	dockable->set_parent_component(this);
	dockable->set_visible(true, false);
	set_visible(true, false);
	on_resized();
	request_repaint();
}

void ViewFrame::on_render(Canvas &canvas, const Rect &update_box)
{
	part_header.render_box(canvas, header_box);
	part_header.render_text(canvas, dockable->get_title(), part_header.get_content_box(header_box));
}

void ViewFrame::on_resized()
{
	Rect client_box = get_content_box();
	int header_height = part_header.get_css_height();
	scene_box = Rect(client_box.left, client_box.top, client_box.right, client_box.bottom - header_height);
	header_box = Rect(client_box.left, client_box.bottom - header_height, client_box.right, client_box.bottom);
	dockable->set_geometry(scene_box);
}

bool ViewFrame::on_input_doubleclick(const InputEvent &input_event)
{
	if (input_event.id == mouse_left)
	{
		if(header_box.contains(input_event.mouse_pos))
		{
			dockable->show_as_window();
		}
		return true;
	}
	else
	{
		return false;
	}
}