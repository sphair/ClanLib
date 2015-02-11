
#pragma once

class WorkspaceViewController;

class WorkspacePageViewController : public clan::ViewController
{
public:
	WorkspacePageViewController();

	const std::string &get_page_id() const { return page_id; }

	void set_tab_title(const std::string &text);
	void increment_message_count();

private:
	static std::string generate_page_id() { int id = next_page_id++; return clan::StringHelp::int_to_text(id); }
	static int next_page_id;

	WorkspaceViewController *workspace = nullptr;
	std::string page_id = generate_page_id();

	int message_count = 0;

	friend class WorkspaceViewController;
};
