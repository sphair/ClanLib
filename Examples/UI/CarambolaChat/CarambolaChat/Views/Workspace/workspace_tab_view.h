
#pragma once

class WorkspaceTabView : public clan::View
{
public:
	WorkspaceTabView(const std::string &label_text, bool app_label);

	std::shared_ptr<clan::LabelView> label;
	std::shared_ptr<clan::LabelView> message_count;
	std::shared_ptr<clan::ImageView> close;
};
