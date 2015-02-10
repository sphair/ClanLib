
#include "precomp.h"
#include "workspace_tab_view.h"

using namespace clan;

WorkspaceTabView::WorkspaceTabView(const std::string &label_text)
{
	box_style.set_flex(0.0f, 1.0f);
	box_style.set_border_radius(2.0f, 2.0f, 0.0f, 0.0f);
	box_style.set_padding(10.0f, 2.0f, 7.0f, 2.0f);
	box_style.set_layout_hbox();

	label = std::make_shared<LabelView>();
	label->text_style().set_font("Source Sans Pro", 12.0f, 26.0f);
	label->text_style().set_target_transparent();
	label->set_text(label_text);
	add_subview(label);

	message_count = std::make_shared<LabelView>();
	message_count->box_style.set_margin(5.0f, 0.0f, 0.0f, 0.0f);
	message_count->box_style.set_margin_top_auto();
	message_count->box_style.set_margin_bottom_auto();
	message_count->box_style.set_background(Colorf::gray90);
	message_count->box_style.set_padding(5.0f, 1.0f);
	message_count->box_style.set_border(Colorf::gray90, 1.0f);
	message_count->box_style.set_border_radius(6.0f);
	message_count->text_style().set_font("Source Sans Pro", 9.0f, 15.0f);
	message_count->text_style().set_weight_bold();
	message_count->text_style().set_color(Colorf::black);
	message_count->set_hidden();
	add_subview(message_count);

	close = std::make_shared<ImageView>();
	close->set_image(ImageSource::from_resource("Icons/cross-small.png"));
	close->box_style.set_margin(8.0f, 9.0f, 0.0f, 9.0f);
	add_subview(close);
}
