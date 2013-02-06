
#pragma once

#include "../UIController/document_editor.h"
#include "scintilla_editor.h"

class TextEditor : public DocumentEditor
{
public:
	TextEditor(UIController *controller, std::string filename);
	void load();

	static void register_ui(UIController *controller);

	void copy();
	void cut();
	void paste();
	void undo();
	void redo();
	void save();
	void lowercase();
	void uppercase();
	void macro_run();
	void macro_record();
	void macro_stop();
	void eol_checked();
	void eol_unchecked();
	void wsmode_invisible();
	void wsmode_visible_always();
	void wsmode_visible_after_indent();

private:
	void on_resized();
	void on_macro_recorded(const std::string &recorded);
	void on_save_point_reached();
	void on_save_point_left();

	void initialize_xml_style_and_document(std::string language);

	ScintillaEditor *textedit;
	std::vector<std::string> recorded_macro;
};
 