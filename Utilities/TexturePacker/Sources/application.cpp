#include "precomp.h"
#include "application.h"
#include "main_window.h"

Application::Application()
{
	resources = CL_ResourceManager("../../Resources/GUIThemeLuna/resources.xml");
	theme = CL_SharedPtr<CL_GUITheme>(new CL_GUIThemeDefault);
	theme->set_resources(resources);
	window_manager = CL_SharedPtr<CL_GUIWindowManagerSystem>(new CL_GUIWindowManagerSystem);
	gui = CL_SharedPtr<CL_GUIManager>(new CL_GUIManager);
	gui->set_window_manager(window_manager);
	gui->set_theme(theme);
	gui->set_css_document("../../Resources/GUIThemeLuna/theme.css");
}

Application::~Application()
{
}

void Application::run()
{
	MainWindow main_window(this);
	gui->exec();
}
