
#pragma once

class WorkspaceView;
class WorkspacePageViewController;

class WorkspaceViewController : public clan::ViewController
{
public:
	WorkspaceViewController();

	void add_page(const std::string &label, std::shared_ptr<WorkspacePageViewController> controller, bool app_page);

protected:
	void child_controller_added(const std::shared_ptr<clan::ViewController> &view_controller) override;
	void child_controller_removed(const std::shared_ptr<clan::ViewController> &view_controller) override;

private:
	void set_label(const std::string &id, const std::string &text);
	bool is_message_count_hidden(const std::string &id) const;
	void set_message_count(const std::string &id, const std::string &text);

	WorkspaceView *workspace_view() const;

	clan::SlotContainer slots;

	friend class WorkspacePageViewController;
};
