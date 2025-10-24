
#include "precomp.h"
#include "create_collision_data_dialog.h"

CreateCollisionDataDialog::CreateCollisionDataDialog(CL_GUIComponent *owner)
: CL_Window(owner, get_description())
{
	set_layout(layout);

	create_components("Resources/create_collision_data.gui");

	edit_directory = CL_LineEdit::get_named_item(this, "editDirectory");
	button_browse = CL_PushButton::get_named_item(this, "buttonBrowse");
	button_generate =  CL_PushButton::get_named_item(this, "buttonGenerate");
	button_close =  CL_PushButton::get_named_item(this, "buttonClose");
	radio_high = CL_RadioButton::get_named_item(this, "radiobuttonHigh");
	radio_medium = CL_RadioButton::get_named_item(this, "radiobuttonMedium");
	radio_low = CL_RadioButton::get_named_item(this, "radiobuttonLow");
	radio_poor = CL_RadioButton::get_named_item(this, "radiobuttonPoor");
	radio_raw = CL_RadioButton::get_named_item(this, "radiobuttonRaw");

	edit_directory->set_focus();

	radio_medium->set_selected(true);

	button_browse->func_clicked().set(this, &CreateCollisionDataDialog::on_browse);
	button_generate->func_clicked().set(this, &CreateCollisionDataDialog::on_generate);
	button_generate->set_default(true);
	button_close->func_clicked().set(this, &CreateCollisionDataDialog::on_close);
	button_close->set_cancel(true);
	
	func_close().set(this, &CreateCollisionDataDialog::on_window_close);

	set_visible(true, true);
}

CL_GUITopLevelDescription CreateCollisionDataDialog::get_description()
{
	CL_GUITopLevelDescription desc;
	desc.set_title("Create Collision Data");
	desc.set_allow_resize(false);
	desc.set_visible(false);
	desc.show_minimize_button(false);
	desc.show_maximize_button(false);
	desc.show_sysmenu(false);
	return desc;
}

bool CreateCollisionDataDialog::on_window_close()
{
	exit_with_code(0);
	return true;
}

void CreateCollisionDataDialog::on_browse()
{
	CL_BrowseFolderDialog dlg(this);

	if(dlg.show())
		edit_directory->set_text(dlg.get_selected_path());
}

void CreateCollisionDataDialog::on_generate()
{
	exit_with_code(1);
}

void CreateCollisionDataDialog::on_close()
{
	exit_with_code(0);
}
