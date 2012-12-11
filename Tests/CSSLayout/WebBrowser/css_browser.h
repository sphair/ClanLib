
#pragma once

class CSSView;

class CSSBrowser : public clan::Window
{
public:
	CSSBrowser(clan::GUIManager *gui);

private:
	static clan::GUITopLevelDescription get_window_description();
	bool on_close();
	void on_resized();
	void on_combobox_item_selected(int index);
	void on_combobox_enter_pressed();

	clan::Label *label;
	clan::ComboBox *combobox;
	CSSView *view;
};
