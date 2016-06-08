
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
	auto pRootView = window_manager.present_main<MainWindowController>()->root_view();

	// Exit run loop when ESC pressed.
	pRootView->slots.connect(pRootView->sig_key_press(), [&](clan::KeyEvent &e)
	{ if (e.key() == clan::Key::escape) RunLoop::exit(); }
	);

	// Need for receive a keyboard events.
	pRootView->set_focus();
}
