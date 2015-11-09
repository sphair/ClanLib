
#include "precomp.h"
#include "app.h"
#include "Controller/main_window_controller.h"

using namespace clan;

ApplicationInstance<App> app;

App::App()
{
	OpenGLTarget::set_current();
	use_timeout_timing(0x7fffff);

	resources = FileResourceManager::create();
	ui_thread = UIThread(resources);

	window_manager.set_exit_on_last_close();
	window_manager.present_main<MainWindowController>();
}
