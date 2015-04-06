
#include "precomp.h"
#include "workspace_tab_view.h"

using namespace clan;

WorkspaceTabView::WorkspaceTabView(const std::string &label_text)
{
	style()->set("flex: 0 1 main-size");
	style()->set("border-radius: 4px 0 0 4px");
	style()->set("padding: 0 7px 0 10px");
	style()->set("flex-direction: row");
	style()->set("margin-right: 5px");
	style("selected")->set("background: rgb(219, 234, 249)");
	

	label = std::make_shared<LabelView>();
	label->style()->set("font: 12px/26px 'Source Sans Pro'");
	label->set_text(label_text);
	add_subview(label);

	message_count = std::make_shared<LabelView>();
	message_count->style()->set("margin: auto 0 auto 5px");
	message_count->style()->set("padding: 0 6px 1px 5px");
	message_count->style()->set("border: 1px solid transparent");
	message_count->style()->set("border-radius: 6px");
	message_count->style()->set("font: bold 9px/15px 'Source Sans Pro'");
	message_count->style()->set("color: black");
	message_count->style()->set("width: 10px");
	message_count->style("highlight")->set("background: rgb(230,230,230)");
	message_count->style("highlight")->set("border-color: rgb(230,230,230)");
	message_count->set_text_alignment(TextAlignment::center);
	add_subview(message_count);

	close = std::make_shared<ImageView>();
	close->set_image(ImageSource::from_resource("Icons/cross-small.png"));
	close->style()->set("margin: auto 0 auto 8px");
	add_subview(close);
}
