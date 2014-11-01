
#pragma once

class WorkspaceViewController;

class ChatWindowViewController : public clan::ViewController
{
public:
	ChatWindowViewController();

	std::shared_ptr<WorkspaceViewController> workspace;

	std::shared_ptr<clan::WindowView> window_view() { return std::static_pointer_cast<clan::WindowView>(view); }
};
