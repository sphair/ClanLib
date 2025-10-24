
#include "precomp.h"
#include "application.h"
#include "main_window.h"
#include "component_type.h"

Application::Application()
{
	ComponentTypes::initialize();

	resources = CL_ResourceManager(cl_text("resources.xml"));
	CL_ResourceManager resources2(cl_text("../../Resources/GUIThemeLuna/resources.xml"));
	resources.add_resources(resources2);
	theme = CL_SharedPtr<CL_GUITheme>(new CL_GUIThemeDefault);
	theme->set_resources(resources);
	window_manager = CL_SharedPtr<CL_GUIWindowManagerSystem>(new CL_GUIWindowManagerSystem);
	gui = CL_SharedPtr<CL_GUIManager>(new CL_GUIManager);
	gui->set_window_manager(window_manager);
	gui->set_theme(theme);
	gui->set_css_document(cl_text("theme.css"));
}

Application::~Application()
{
	ComponentTypes::deinitialize();
}

void Application::run()
{
	MainWindow main_window(this);
	gui->exec();
}
