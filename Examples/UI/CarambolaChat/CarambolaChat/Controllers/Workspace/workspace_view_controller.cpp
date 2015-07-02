
#include "precomp.h"
#include "workspace_view_controller.h"
#include "workspace_page_view_controller.h"
#include "Views/Workspace/workspace_view.h"

using namespace clan;

WorkspaceViewController::WorkspaceViewController()
{
	view = std::make_shared<WorkspaceView>();
}

WorkspaceView *WorkspaceViewController::workspace_view() const
{
	return static_cast<WorkspaceView*>(view.get());
}

void WorkspaceViewController::add_page(const std::string &label_text, std::shared_ptr<WorkspacePageViewController> controller, bool app_page)
{
	controller->workspace = this;
	workspace_view()->add_page(controller->get_page_id(), label_text, controller->view, app_page);
	add_child_controller(controller);
}

void WorkspaceViewController::set_label(const std::string &id, const std::string &text)
{
	workspace_view()->set_label(id, text);
}

bool WorkspaceViewController::is_message_count_hidden(const std::string &id) const
{
	return workspace_view()->is_message_count_hidden(id);
}

void WorkspaceViewController::set_message_count(const std::string &id, const std::string &text)
{
	workspace_view()->set_message_count(id, text);
}

void WorkspaceViewController::child_controller_added(const std::shared_ptr<ViewController> &view_controller)
{
}

void WorkspaceViewController::child_controller_removed(const std::shared_ptr<ViewController> &view_controller)
{
	workspace_view()->remove_page(view_controller->view);
}
