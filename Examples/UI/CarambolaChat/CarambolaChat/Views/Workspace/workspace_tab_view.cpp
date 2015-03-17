
#include "precomp.h"
#include "workspace_tab_view.h"

using namespace clan;

WorkspaceTabView::WorkspaceTabView(const std::string &label_text)
{
	style()->set("flex: 0 1 main-size");
	style()->set("border-radius: 2px 2px 0 0");
	style()->set("padding: 2px 7px 2px 10px");
	style()->set("flex-direction: row");

	label = std::make_shared<LabelView>();
	label->style()->set("font: 12px/26px 'Source Sans Pro'");
	label->style()->set("-clan-font-rendering: anti-alias");
	label->set_text(label_text);
	add_subview(label);

	message_count = std::make_shared<LabelView>();
	message_count->style()->set("margin: auto 0 auto 5px");
	message_count->style()->set("background: rgb(230,230,230)");
	message_count->style()->set("padding: 1px 5px");
	message_count->style()->set("border: 1px solid rgb(230,230,230)");
	message_count->style()->set("border-radius: 6px");
	message_count->style()->set("font: bold 9px/15px 'Source Sans Pro'");
	message_count->style()->set("color: black");
	message_count->set_hidden();
	add_subview(message_count);

	close = std::make_shared<ImageView>();
	close->set_image(ImageSource::from_resource("Icons/cross-small.png"));
	close->style()->set("margin: 9px 0 9px 8px");
	add_subview(close);
}
