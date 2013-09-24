
#pragma once

class SpriteResourceItem;

class CreateCollisionDataDialog : public clan::Window
{
public:
	CreateCollisionDataDialog(clan::GUIComponent *owner, SpriteResourceItem *sprite_item);

private:
	void on_browse();
	void on_generate();
	void on_close();
	bool on_window_close();
	void on_file_edit(clan::InputEvent &event);

	void update_expected_filenames();

	void generate_collision(const std::string &filename, const std::string &directory, clan::OutlineAccuracy accuracy);

	static clan::GUITopLevelDescription get_description();
	clan::GUILayoutCorners layout;

	clan::LineEdit *edit_directory;
	clan::LineEdit *edit_filename;
	clan::PushButton *button_generate;
	clan::PushButton *button_close;
	clan::PushButton *button_browse;
	clan::Label *label_expected_filenames;
	clan::RadioButton *radio_raw;
	clan::RadioButton *radio_poor;
	clan::RadioButton *radio_low;
	clan::RadioButton *radio_medium;
	clan::RadioButton *radio_high;

	SpriteResourceItem *sprite_item;
};
