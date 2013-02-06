
#pragma once

#include "../UIController/ui_controller_listener.h"

class TextEditor;

class TextEditorRibbon : public UIControllerListener
{
public:
	TextEditorRibbon(UIController *controller);
	void set_active(DocumentEditor *editor);
	void editor_destroyed(DocumentEditor *editor);

private:
	void on_button_lowercase();
	void on_button_uppercase();
	void on_button_macro_run();
	void on_button_macro_record();
	void on_button_macro_stop();
	void on_check_eol_checked();
	void on_check_eol_unchecked();
	void on_combo_wsmode_invisible();
	void on_combo_wsmode_visible_always();
	void on_combo_wsmode_visible_after_indent();

	clan::RibbonSection *text_section;
	clan::PushButton *button_lowercase;
	clan::PushButton *button_uppercase;

	clan::RibbonSection *macro_section;
	clan::PushButton *button_macro_run;
	clan::PushButton *button_macro_record;
	clan::PushButton *button_macro_stop;

	clan::RibbonSection *view_section;
	clan::CheckBox *check_eol;
	clan::ComboBox *combo_wsmode;

	TextEditor *editor;
};
