
#pragma once

class CSSView;

class CSSBrowser : public CL_Window
{
public:
	CSSBrowser(CL_GUIManager *gui);

private:
	static CL_GUITopLevelDescription get_window_description();
	bool on_close();
	void on_resized();
	void on_combobox_item_selected(int index);
	void on_combobox_enter_pressed();

	CL_Label *label;
	CL_ComboBox *combobox;
	CSSView *view;
};
