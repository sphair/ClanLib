#pragma once

#include "../view.h"

class IcoCreatorView : public View
{
public:
	IcoCreatorView(CL_GUIComponent *parent, MainWindow *mainwindow);
	~IcoCreatorView();

private:
	void on_resized();
	void on_button_browse(int index);
	void on_button_save();

	CL_Label *label_selected_file[11];
	CL_LineEdit *lineedit_selected_file[11];
	CL_PushButton *button_browse_file[11];

	CL_Label *label_result;

	CL_PushButton *button_save;
};
