
#include "precomp.h"
#include "custom.h"

using namespace clan;

Custom::Custom(GUIComponent *parent)
: GUIComponent(parent, "custom")
{
	func_render().set(this, &Custom::on_render);
}

float Custom::get_preferred_content_width()
{
	return 100.0f;
}

float Custom::get_preferred_content_height(float width)
{
	return 100.0f;
}

void Custom::on_render(Canvas &canvas, const Rect &update_rect)
{
	Rect rect = get_geometry();
	canvas.fill(rect, Colorf::red);
}
