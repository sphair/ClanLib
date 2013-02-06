/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
*/
#pragma once

#include "API/GameIDE/UIController/document_editor.h"
#include "scintilla_editor.h"
namespace clan
{
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

}

 