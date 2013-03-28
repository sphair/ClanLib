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

#include "API/GameIDE/UIController/ui_controller_listener.h"

namespace clan
{

class TextEditor;
class UIRibbonSection;

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

	UIRibbonSection *text_section;
	PushButton *button_lowercase;
	PushButton *button_uppercase;

	UIRibbonSection *macro_section;
	PushButton *button_macro_run;
	PushButton *button_macro_record;
	PushButton *button_macro_stop;

	UIRibbonSection *view_section;
	CheckBox *check_eol;
	ComboBox *combo_wsmode;

	TextEditor *editor;
};

}
