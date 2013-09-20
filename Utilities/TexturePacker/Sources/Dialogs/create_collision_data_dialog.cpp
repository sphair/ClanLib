
#include "precomp.h"
#include "create_collision_data_dialog.h"
#include "../texture_packer.h"

CreateCollisionDataDialog::CreateCollisionDataDialog(GUIComponent *owner, SpriteResourceItem *sprite_item)
: GUIComponent(owner, get_description()), sprite_item(sprite_item)
{
	set_layout(layout);

	create_components("Resources/create_collision_data.gui");

	edit_directory = LineEdit::get_named_item(this, "editDirectory");
	edit_filename = LineEdit::get_named_item(this, "editFilename");
	button_browse = PushButton::get_named_item(this, "buttonBrowse");
	button_generate =  PushButton::get_named_item(this, "buttonGenerate");
	button_close =  PushButton::get_named_item(this, "buttonClose");
	radio_high = RadioButton::get_named_item(this, "radiobuttonHigh");
	radio_medium = RadioButton::get_named_item(this, "radiobuttonMedium");
	radio_low = RadioButton::get_named_item(this, "radiobuttonLow");
	radio_poor = RadioButton::get_named_item(this, "radiobuttonPoor");
	radio_raw = RadioButton::get_named_item(this, "radiobuttonRaw");
	label_expected_filenames = Label::get_named_item(this, "labelExpectedFilenames");

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

GUITopLevelDescription CreateCollisionDataDialog::get_description()
{
	GUITopLevelDescription desc;
	desc.set_title("Create Collision Data");
	desc.set_allow_resize(false);
	desc.set_visible(false);
	desc.show_minimize_button(false);
	desc.show_maximize_button(false);
	desc.show_sysmenu(false);
	return desc;
}

void CreateCollisionDataDialog::on_file_edit(InputEvent &event)
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
	BrowseFolderDialog dlg(this);

	if(dlg.show())
	{
		edit_directory->set_text(dlg.get_selected_path());
		update_expected_filenames();
	}
}

void CreateCollisionDataDialog::on_generate()
{
	std::string directory = StringHelp::trim(edit_directory->get_text());
	if(directory.length() == 0)
	{
		message_box(this, "Missing output directory", "You need to specify an output directory.", mb_buttons_ok, mb_icon_error);
		return;
	}

	std::string filename = StringHelp::trim(edit_filename->get_text());
	if(filename.length() == 0)
	{
		message_box(this, "Missing output filename", "You need to specify an output filename.", mb_buttons_ok, mb_icon_error);
		return;
	}

	OutlineAccuracy accuracy;
	if(radio_high->is_selected())
		accuracy = accuracy_high;
	else if(radio_low->is_selected())
		accuracy = accuracy_low;
	else if(radio_medium->is_selected())
		accuracy = accuracy_medium;
	else if(radio_poor->is_selected())
		accuracy = accuracy_poor;
	else if(radio_raw->is_selected())
		accuracy = accuracy_raw;

	generate_collision(filename, directory, accuracy);

	exit_with_code(1);
}

void CreateCollisionDataDialog::on_close()
{
	exit_with_code(0);
}

void CreateCollisionDataDialog::generate_collision(const std::string &filename, const std::string &directory, OutlineAccuracy accuracy)
{
	Canvas canvas = get_canvas();
	unsigned int frame_size = sprite_item->sprite.get_frame_count();
	for(unsigned int i = 0; i < frame_size; ++i)
	{
		Subtexture subtexture = sprite_item->sprite.get_frame_texture(i);
		PixelBuffer pb = subtexture.get_texture().get_pixeldata(canvas, tf_rgba8);
		pb = pb.copy(subtexture.get_geometry());

		CollisionOutline generated(pb, 128, accuracy);

		std::string output_filename;

		if(frame_size > 1)
		{
			StringFormat f("%1\\%2_%3.out"); 
			f.set_arg(1, directory); 
			f.set_arg(2, filename);
			f.set_arg(3, (int) i, 3);
			output_filename = f.get_result();
		}
		else
		{
			StringFormat f("%1\\%2.out"); 
			f.set_arg(1, directory); 
			f.set_arg(2, filename);
			output_filename = f.get_result();
		}

		generated.save(output_filename);
	}

	std::string msg = string_format("%1 collision outlines generated", (int)frame_size);
	message_box(this, "Collision outlines generated", msg, mb_buttons_ok, mb_icon_info);

}

void CreateCollisionDataDialog::update_expected_filenames()
{
	if(sprite_item->sprite.get_frame_count() > 1)
	{
		label_expected_filenames->set_text(
			string_format("Expected output: %1\\%2_xxx.out", edit_directory->get_text(), edit_filename->get_text()));
	}
	else
	{
		label_expected_filenames->set_text(
			string_format("Expected output: %1\\%2.out", edit_directory->get_text(), edit_filename->get_text()));
	}
}
