#pragma once

class Application
{
public:
	Application();
	~Application();

	void run();

	CL_ResourceManager get_resources() { return resources; }
	CL_GUIManager *get_gui() { return gui; }

private:
	CL_ResourceManager resources;
	CL_SharedPtr<CL_GUITheme> theme;
	CL_SharedPtr<CL_GUIWindowManagerSystem> window_manager;
	CL_SharedPtr<CL_GUIManager> gui;
};
