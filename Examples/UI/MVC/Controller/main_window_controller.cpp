
#include "precomp.h"
#include "main_window_controller.h"
#include "Model/app_model.h"

using namespace clan;

MainWindowController::MainWindowController()
{
	set_title("UI Model/View/Controller Example");
	set_root_view(form_view);

	// Grab data from model:
	form_view->name->textfield->set_text(AppModel::instance()->name);
	form_view->description->textfield->set_text(AppModel::instance()->description);

	// Send data back to model:
	slots.connect(form_view->name->textfield->sig_after_edit_changed(), [this](KeyEvent &) { AppModel::instance()->name = form_view->name->textfield->text(); });
	slots.connect(form_view->description->textfield->sig_after_edit_changed(), [this](KeyEvent &) { AppModel::instance()->description = form_view->description->textfield->text(); });
}
