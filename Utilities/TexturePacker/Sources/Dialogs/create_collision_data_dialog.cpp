
#include "precomp.h"
#include "create_collision_data_dialog.h"
#include "../texture_packer.h"

CreateCollisionDataDialog::CreateCollisionDataDialog(clan::GUIComponent *owner, SpriteResourceItem *sprite_item)
: clan::Window(owner, get_description()), sprite_item(sprite_item)
{
	set_layout(layout);

	create_components("Resources/create_collision_data.gui");

	edit_directory = clan::LineEdit::get_named_item(this, "editDirectory");
	edit_filename = clan::LineEdit::get_named_item(this, "editFilename");
	button_browse = clan::PushButton::get_named_item(this, "buttonBrowse");
	button_generate =  clan::PushButton::get_named_item(this, "buttonGenerate");
	button_close =  clan::PushButton::get_named_item(this, "buttonClose");
	radio_high = clan::RadioButton::get_named_item(this, "radiobuttonHigh");
	radio_medium = clan::RadioButton::get_named_item(this, "radiobuttonMedium");
	radio_low = clan::RadioButton::get_named_item(this, "radiobuttonLow");
	radio_poor = clan::RadioButton::get_named_item(this, "radiobuttonPoor");
	radio_raw = clan::RadioButton::get_named_item(this, "radiobuttonRaw");
	label_expected_filenames = clan::Label::get_named_item(this, "labelExpectedFilenames");

	edit_directory->set_focus();
	edit_filename->set_text(sprite_item->resource.get_name());
	radio_medium->set_selected(true);

	button_browse->func_clicked().set(this, &CreateCollisionDataDialog::on_browse);
	button_generate->func_clicked().set(this, &CreateCollisionDataDialog::on_generate);
	button_generate->set_default(true);
	button_close->func_clicked().set(this, &CreateCollisionDataDialog::on_close);
	button_close->set_cancel(true);
	edit_filename->func_after_edit_changed().set(this, &CreateCollisionDataDialog::on_file_edit);
	edit_directory->func_after_edit_changed().set(this, &CreateCollisionDataDialog::on_file_edit);

	func_close().set(this, &CreateCollisionDataDialog::on_window_close);

	update_expected_filenames();

	set_visible(true, true);
}

clan::GUITopLevelDescription CreateCollisionDataDialog::get_description()
{
	clan::GUITopLevelDescription desc;
	desc.set_title("Create Collision Data");
	desc.set_allow_resize(false);
	desc.set_visible(false);
	desc.show_minimize_button(false);
	desc.show_maximize_button(false);
	desc.show_sysmenu(false);
	return desc;
}

void CreateCollisionDataDialog::on_file_edit(clan::InputEvent &event)
{
	update_expected_filenames();
}

bool CreateCollisionDataDialog::on_window_close()
{
	exit_with_code(0);
	return true;
}

void CreateCollisionDataDialog::on_browse()
{
	clan::BrowseFolderDialog dlg(this);

	if(dlg.show())
	{
		edit_directory->set_text(dlg.get_selected_path());
		update_expected_filenames();
	}
}

void CreateCollisionDataDialog::on_generate()
{
	std::string directory = clan::StringHelp::trim(edit_directory->get_text());
	if(directory.length() == 0)
	{
		clan::message_box(this, "Missing output directory", "You need to specify an output directory.", clan::mb_buttons_ok, clan::mb_icon_error);
		return;
	}

	std::string filename = clan::StringHelp::trim(edit_filename->get_text());
	if(filename.length() == 0)
	{
		clan::message_box(this, "Missing output filename", "You need to specify an output filename.", clan::mb_buttons_ok, clan::mb_icon_error);
		return;
	}

	clan::OutlineAccuracy accuracy;
	if(radio_high->is_selected())
		accuracy = clan::accuracy_high;
	else if(radio_low->is_selected())
		accuracy = clan::accuracy_low;
	else if(radio_medium->is_selected())
		accuracy = clan::accuracy_medium;
	else if(radio_poor->is_selected())
		accuracy = clan::accuracy_poor;
	else if(radio_raw->is_selected())
		accuracy = clan::accuracy_raw;

	generate_collision(filename, directory, accuracy);

	exit_with_code(1);
}

void CreateCollisionDataDialog::on_close()
{
	exit_with_code(0);
}

void CreateCollisionDataDialog::generate_collision(const std::string &filename, const std::string &directory, clan::OutlineAccuracy accuracy)
{
	clan::Canvas canvas = get_canvas();
	std::vector<clan::CollisionOutline> collision_outlines = sprite_item->sprite.create_collision_outlines(canvas, 128, accuracy);

	for(unsigned int i = 0; i < collision_outlines.size(); ++i)
	{

		std::string output_filename;

		if(collision_outlines.size() > 1)
		{
			clan::StringFormat f("%1\\%2_%3.out"); 
			f.set_arg(1, directory); 
			f.set_arg(2, filename);
			f.set_arg(3, i, 3);
			output_filename = f.get_result();
		}
		else
		{
			clan::StringFormat f("%1\\%2.out"); 
			f.set_arg(1, directory); 
			f.set_arg(2, filename);
			output_filename = f.get_result();
		}

		collision_outlines[i].save(output_filename);
	}

	std::string msg = clan::string_format("%1 collision outlines generated", collision_outlines.size());
	clan::message_box(this, "Collision outlines generated", msg, clan::mb_buttons_ok, clan::mb_icon_info);
}

void CreateCollisionDataDialog::update_expected_filenames()
{
	if(sprite_item->sprite.get_frame_count() > 1)
	{
		label_expected_filenames->set_text(
			clan::string_format("Expected output: %1\\%2_xxx.out", edit_directory->get_text(), edit_filename->get_text()));
	}
	else
	{
		label_expected_filenames->set_text(
			clan::string_format("Expected output: %1\\%2.out", edit_directory->get_text(), edit_filename->get_text()));
	}
}
