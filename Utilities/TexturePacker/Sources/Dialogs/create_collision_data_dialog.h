
#pragma once

class SpriteResourceItem;

class CreateCollisionDataDialog : public CL_Window
{
public:
	CreateCollisionDataDialog(CL_GUIComponent *owner, SpriteResourceItem *sprite_item);

private:
	void on_browse();
	void on_generate();
	void on_close();
	bool on_window_close();
	void on_file_edit(CL_InputEvent &event);

	void update_expected_filenames();

	void generate_collision(const CL_String &filename, const CL_String &directory, CL_OutlineAccuracy accuracy);

	static CL_GUITopLevelDescription get_description();
	CL_GUILayoutCorners layout;

	CL_LineEdit *edit_directory;
	CL_LineEdit *edit_filename;
	CL_PushButton *button_generate;
	CL_PushButton *button_close;
	CL_PushButton *button_browse;
	CL_Label *label_expected_filenames;
	CL_RadioButton *radio_raw;
	CL_RadioButton *radio_poor;
	CL_RadioButton *radio_low;
	CL_RadioButton *radio_medium;
	CL_RadioButton *radio_high;

	SpriteResourceItem *sprite_item;
};
