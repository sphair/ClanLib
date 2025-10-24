#pragma once

#include "../view.h"
#include "../css_packer.h"

class CssPackerView : public View
{
public:
	CssPackerView(CL_GUIComponent *parent, MainWindow *mainwindow);
	~CssPackerView();

private:
	void on_resized();
	void on_button_browse();
	void on_button_save();

	int load_css_file(const CL_String &file);
	void save_css_file(const CL_String &file);

	CL_Label *label_selected_file;
	CL_LineEdit *lineedit_selected_file;
	CL_PushButton *button_browse_file;

	CL_Label *label_result;

	CL_PushButton *button_save;

	CssPacker packer;
};
