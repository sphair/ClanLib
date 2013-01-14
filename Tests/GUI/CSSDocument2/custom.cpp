
#include "precomp.h"
#include "custom.h"

using namespace clan;

Custom::Custom(GUIComponent *parent)
: GUIComponent(parent, "custom")
{
	func_render().set(this, &Custom::on_render);

	box1 = GUIThemePart(this, "box1");
	box2 = GUIThemePart(this, "box2");
	box3 = GUIThemePart(this, "box3");
}

float Custom::get_preferred_content_width()
{
	return 900.0f;
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
	Rect box3_rect(box2_rect.right, box2_rect.top, box3.get_css_size());

	box1.render_box(canvas, box1_rect);
	box2.render_box(canvas, box2_rect);
	box3.render_box(canvas, box3_rect);

	static char message_1[] = "gratulerer med dagen";
	static char message_2[] = "rød grønn blå";
	static char message_3[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aenean sit amet orci et eros lobortis accumsan id sed nulla. Curabitur bibendum lobortis ante ac fringilla";

	Rect box1_content_box = box1.get_content_box(box1_rect);
	Rect box2_content_box = box2.get_content_box(box2_rect);
	Rect box3_content_box = box2.get_content_box(box3_rect);

	// Note, get_render_text_size() exists, just does not require a content box for the theme part
	Rect text1_rect = box1.get_render_text_box(canvas, message_1, box1_content_box);
	Rect text2_rect = box2.get_render_text_box(canvas, message_2, box2_content_box);
	Rect text3_rect = box3.get_render_text_span_box(canvas, message_3, box3_content_box);

	canvas.draw_fill(text1_rect, Colorf(0.5f, 0.5f, 0.5f, 0.5f));
	canvas.draw_fill(text2_rect, Colorf(0.5f, 0.5f, 0.5f, 0.5f));
	canvas.draw_fill(text3_rect, Colorf(0.5f, 0.5f, 0.5f, 0.5f));
	
	Rect rendered_text1_rect = box1.render_text(canvas, message_1, box1_content_box);
	Rect rendered_text2_rect = box2.render_text(canvas, message_2, box2_content_box);
	Rect rendered_text3_rect = box3.render_text_span(canvas, message_3, box3_content_box);

	canvas.draw_box(rendered_text1_rect, Colorf(0.5f, 0.5f, 0.5f, 1.0f));
	canvas.draw_box(rendered_text2_rect, Colorf(0.5f, 0.5f, 0.5f, 1.0f));
	canvas.draw_box(rendered_text3_rect, Colorf(0.5f, 0.5f, 0.5f, 1.0f));

}
