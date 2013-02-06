
#pragma once

#include "ui_controller_listener.h"

class DocumentEditorRibbon : public UIControllerListener
{
public:
	DocumentEditorRibbon(UIController *controller);
	void set_active(DocumentEditor *editor);
	void editor_destroyed(DocumentEditor *editor);

private:
	void on_button_cut_clicked();
	void on_button_paste_clicked();
	void on_button_copy_clicked();
	void on_button_undo_clicked();
	void on_button_redo_clicked();
	void on_button_save_clicked();

	clan::RibbonSection *clipboard_section;
	clan::PushButton *button_cut;
	clan::PushButton *button_paste;
	clan::PushButton *button_copy;

	clan::RibbonSection *edit_section;
	clan::PushButton *button_undo;
	clan::PushButton *button_redo;
	clan::PushButton *button_save;

	DocumentEditor *editor;
};
