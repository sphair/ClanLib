
#include "precomp.h"
#include "workspace_tab_view.h"

using namespace clan;

WorkspaceTabView::WorkspaceTabView(const std::string &label_text, bool app_label)
{
	style()->set("flex: 0 1 main-size");
	style()->set("padding: 0 8px");
	style()->set("flex-direction: row");
	if (app_label)
	{
		style()->set("background: linear-gradient(to bottom, rgb(71,130,192) 0%, rgb(45,102,177) 50%, rgb(26,64,136) 50%, rgb(65,154,207))");
		style("hot")->set("background: linear-gradient(to bottom, rgb(121,185,239) 0%, rgb(71,126,205) 50%, rgb(17,78,175) 50%, rgb(85,207,255))");
	}
	else
	{
		style("selected")->set("background: white");
	}
	

	label = std::make_shared<LabelView>();
	label->style()->set("font: 12px/26px 'Source Sans Pro'");
	label->set_text(label_text);
	if (app_label)
	{
		label->style()->set("color: white");
	}
	add_subview(label);

	message_count = std::make_shared<LabelView>();
	close = std::make_shared<ImageView>();

	if (!app_label)
	{
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

		close->set_image(ImageSource::from_resource("Icons/cross-small.png"));
		close->style()->set("margin: auto 0 auto 8px");
		add_subview(close);
	}
}
