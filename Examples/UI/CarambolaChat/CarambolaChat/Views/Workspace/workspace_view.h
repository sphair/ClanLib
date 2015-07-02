
#pragma once

class WorkspaceTabView;

class WorkspaceView : public clan::View
{
public:
	WorkspaceView();

	std::shared_ptr<clan::View> toolbar;

	void add_page(const std::string &id, const std::string &label_text, std::shared_ptr<View> page, bool app_page);
	void remove_page(std::shared_ptr<View> page);

	void set_label(const std::string &id, const std::string &text);
	void set_message_count(const std::string &id, const std::string &text);
	bool is_message_count_hidden(const std::string &id) const;

private:
	struct TabPage
	{
	public:
		std::shared_ptr<WorkspaceTabView> tab;
		std::shared_ptr<clan::View> page;
	};

	void set_selected(TabPage &tab, bool selected, bool animated = false);

	std::shared_ptr<clan::View> labels_group;
	std::shared_ptr<clan::View> pages_group;

	std::map<std::string, TabPage> tabs;
};
