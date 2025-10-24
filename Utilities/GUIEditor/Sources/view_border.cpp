
#include "precomp.h"
#include "view_border.h"

ViewBorder::ViewBorder(CL_GUIComponent *parent)
: CL_GUIComponent(parent)
{
	set_type_name("viewborder");
	func_resized().set(this, &ViewBorder::on_resized);
	func_render().set(this, &ViewBorder::on_render);
	theme_part = CL_GUIThemePart(this);
}

void ViewBorder::on_resized()
{
	CL_Rect content_area = theme_part.get_content_box(CL_Rect(CL_Point(0, 0), get_geometry().get_size()));
	CL_GUIComponent *child = get_first_child();
	while (child)
	{
		child->set_geometry(content_area);
		child = child->get_next_sibling();
	}
}

void ViewBorder::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	theme_part.render_box(gc, get_geometry().get_size(), update_rect);
}
