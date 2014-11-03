
#pragma once

class WorkspaceViewController : public clan::ViewController
{
public:
	WorkspaceViewController();

	std::shared_ptr<clan::View> toolbar;

	void add_page(const std::string &label, std::shared_ptr<clan::ViewController> controller);

protected:
	void child_controller_added(const std::shared_ptr<clan::ViewController> &view_controller) override;
	void child_controller_removed(const std::shared_ptr<clan::ViewController> &view_controller) override;

private:
	struct TabPage
	{
	public:
		std::shared_ptr<clan::View> tab;
		std::shared_ptr<clan::LabelView> label;
		std::shared_ptr<clan::ViewController> page;
	};

	void set_selected(TabPage &tab, bool selected, bool animated = false);

	std::shared_ptr<clan::View> labels_group;
	std::shared_ptr<clan::View> pages_group;

	std::vector<TabPage> tabs;

	clan::SlotContainer slots;
};
