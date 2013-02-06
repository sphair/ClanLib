
#include "precomp.h"
#include "document_editor.h"
#include "ui_controller.h"

using namespace clan;

DocumentEditor::DocumentEditor(UIController *controller, std::string filename)
: controller(controller), filename(filename)
{
	func_visibility_change().set(this, &DocumentEditor::on_visibility_change);
}

void DocumentEditor::on_visibility_change(bool new_state)
{
	if (new_state)
		controller->set_active(this);
}

void DocumentEditor::set_dirty(bool dirty)
{
	std::string title = get_title();
	if(title.length() > 1 && title[0] == '*' && title[1] == ' ')
		title = title.substr(2);

	if(dirty)
		set_title("* " + title);
	else
		set_title(title);
}
