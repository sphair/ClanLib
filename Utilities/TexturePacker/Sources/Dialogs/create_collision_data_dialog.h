
#pragma once

class SpriteResourceItem;

class CreateCollisionDataDialog : public GUIComponent
{
public:
	CreateCollisionDataDialog(GUIComponent *owner, SpriteResourceItem *sprite_item);

private:
	void on_browse();
	void on_generate();
	void on_close();
	bool on_window_close();
	void on_file_edit(InputEvent &event);

	void update_expected_filenames();

	void generate_collision(const std::string &filename, const std::string &directory, OutlineAccuracy accuracy);

	static GUITopLevelDescription get_description();
	GUILayoutCorners layout;

	LineEdit *edit_directory;
	LineEdit *edit_filename;
	PushButton *button_generate;
	PushButton *button_close;
	PushButton *button_browse;
	Label *label_expected_filenames;
	RadioButton *radio_raw;
	RadioButton *radio_poor;
	RadioButton *radio_low;
	RadioButton *radio_medium;
	RadioButton *radio_high;

	SpriteResourceItem *sprite_item;
};
