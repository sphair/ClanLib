
#pragma once

class CreateCollisionDataDialog : public CL_Window
{
public:
	CreateCollisionDataDialog(CL_GUIComponent *owner);

private:
	void on_browse();
	void on_generate();
	void on_close();
	bool on_window_close();

	static CL_GUITopLevelDescription get_description();

	CL_GUILayoutCorners layout;

	CL_LineEdit *edit_directory;
	CL_PushButton *button_generate;
	CL_PushButton *button_close;
	CL_PushButton *button_browse;

	CL_RadioButton *radio_raw;
	CL_RadioButton *radio_poor;
	CL_RadioButton *radio_low;
	CL_RadioButton *radio_medium;
	CL_RadioButton *radio_high;

};
