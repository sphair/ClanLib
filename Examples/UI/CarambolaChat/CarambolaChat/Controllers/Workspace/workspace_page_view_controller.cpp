
#include "precomp.h"
#include "workspace_page_view_controller.h"
#include "workspace_view_controller.h"

using namespace clan;

WorkspacePageViewController::WorkspacePageViewController()
{
	view->style()->set("flex: auto");
}

void WorkspacePageViewController::set_tab_title(const std::string &text)
{
	if (workspace)
		workspace->set_label(page_id, text);
}

void WorkspacePageViewController::increment_message_count()
{
	if (workspace)
	{
		if (workspace->is_message_count_hidden(page_id))
			message_count = 0;
		message_count++;
		workspace->set_message_count(page_id, StringHelp::int_to_text(message_count));
	}
}

int WorkspacePageViewController::next_page_id = 0;
