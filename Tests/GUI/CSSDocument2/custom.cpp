
#include "precomp.h"
#include "custom.h"

using namespace clan;

Custom::Custom(GUIComponent *parent)
: GUIComponent(parent, "custom")
{
	func_render().set(this, &Custom::on_render);

	box1 = GUIThemePart(this, "box1");
	box2 = GUIThemePart(this, "box2");
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
	Rect content_rect = get_content_box();

	Rect box1_rect(content_rect.left, content_rect.top, Size(box1.get_css_width(), 40));
	Rect box2_rect(box1_rect.right, box1_rect.top, Size(box2.get_css_width(), 40));

	box1.render_box(canvas, box1_rect);
	box2.render_box(canvas, box2_rect);

	box1.render_text(canvas, "1", box1.get_content_box(box1_rect));
	box2.render_text(canvas, "2", box2.get_content_box(box2_rect));

}
