
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
	return 800.0f;
}

float Custom::get_preferred_content_height(float width)
{
	return 100.0f;
}

void Custom::on_render(Canvas &canvas, const Rect &update_rect)
{
	Rect content_rect = get_content_box();

	Rect box1_rect(content_rect.left, content_rect.top, box1.get_css_size());
	Rect box2_rect(box1_rect.right, box1_rect.top, box2.get_css_size());

	box1.render_box(canvas, box1_rect);
	box2.render_box(canvas, box2_rect);

	static char message_1[] = "gratulerer med dagen";
	static char message_2[] = "rød grønn blå";

	Rect box1_content_box = box1.get_content_box(box1_rect);
	Rect box2_content_box = box2.get_content_box(box2_rect);

	// Note, get_render_text_size() exists, just does not require a content box for the theme part
	Rect text1_rect = box1.get_render_text_box(canvas, message_1, box1_content_box);
	Rect text2_rect = box2.get_render_text_box(canvas, message_2, box2_content_box);

	canvas.fill(text1_rect, Colorf(0.5f, 0.5f, 0.5f, 0.5f));
	canvas.fill(text2_rect, Colorf(0.5f, 0.5f, 0.5f, 0.5f));
	
	box1.render_text(canvas, message_1, box1_content_box);
	box2.render_text(canvas, message_2, box2_content_box);

}
