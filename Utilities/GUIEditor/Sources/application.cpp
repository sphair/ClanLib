
#include "precomp.h"
#include "application.h"
#include "main_window.h"
#include "component_type.h"

Application::Application()
{
	ComponentTypes::initialize();

	CL_String base_path = CL_System::get_exe_path();
	if(CL_FileHelp::file_exists(base_path + cl_text("resources.xml")) == false)
		base_path += cl_text("..\\");
	CL_Directory::set_current(base_path);

	resources = CL_ResourceManager(cl_text("resources.xml"));
	CL_ResourceManager resources2(cl_text("../../Resources/GUIThemeLuna/resources.xml"));
	resources.add_resources(resources2);
	theme.set_resources(resources);
	gui.set_window_manager(window_manager);
	gui.set_theme(theme);
	gui.set_css_document(cl_text("theme.css"));
}

Application::~Application()
{
	ComponentTypes::deinitialize();
}

void Application::run(const CL_String &filename)
{
	MainWindow main_window(this);

	if(!filename.empty())
		main_window.load(filename);

	gui.exec();
}
