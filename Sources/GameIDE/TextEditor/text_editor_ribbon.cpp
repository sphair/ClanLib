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

#include "GameIDE/precomp.h"
#include "text_editor_ribbon.h"
#include "text_editor.h"
#include "API/GameIDE/UIController/document_editor.h"
#include "API/GameIDE/UIController/ui_controller.h"

namespace clan
{

TextEditorRibbon::TextEditorRibbon(UIController *controller)
	: UIControllerListener(controller), editor(0)
{
	text_section = controller->get_ribbon_section("Home", "Text");
	macro_section = controller->get_ribbon_section("Home", "Macro");
	view_section = controller->get_ribbon_section("Home", "Whitespace");

	ResourceManager resources = text_section->get_resources();
	Canvas canvas = text_section->get_canvas();

	RibbonGroup *text_group1 = new RibbonGroup(text_section);

	button_lowercase = new PushButton(text_group1);
	button_lowercase->set_text("Lowercase");
	button_lowercase->set_icon(Image(canvas, "IconEditLowercase", &resources));
	button_lowercase->set_icon_position(PushButton::icon_left);
	button_lowercase->func_clicked().set(this, &TextEditorRibbon::on_button_lowercase);

	button_uppercase = new PushButton(text_group1);
	button_uppercase->set_text("Uppercase");
	button_uppercase->set_icon(Image(canvas, "IconEditUppercase", &resources));
	button_uppercase->set_icon_position(PushButton::icon_left);
	button_uppercase->func_clicked().set(this, &TextEditorRibbon::on_button_uppercase);

	RibbonGroup *macro_group1 = new RibbonGroup(macro_section);
	RibbonGroup *macro_group2 = new RibbonGroup(macro_section);

	button_macro_run = new PushButton(macro_group1);
	button_macro_run->set_text("Run");
	button_macro_run->set_icon(Image(canvas, "IconMacroRun24", &resources));
	button_macro_run->set_icon_position(PushButton::icon_top);
	button_macro_run->func_clicked().set(this, &TextEditorRibbon::on_button_macro_run);

	button_macro_record = new PushButton(macro_group2);
	button_macro_record->set_text("Record");
	button_macro_record->set_icon(Image(canvas, "IconMacroRecord", &resources));
	button_macro_record->set_icon_position(PushButton::icon_left);
	button_macro_record->func_clicked().set(this, &TextEditorRibbon::on_button_macro_record);

	button_macro_stop = new PushButton(macro_group2);
	button_macro_stop->set_text("Stop");
	button_macro_stop->set_icon(Image(canvas, "IconMacroStop", &resources));
	button_macro_stop->set_icon_position(PushButton::icon_left);
	button_macro_stop->func_clicked().set(this, &TextEditorRibbon::on_button_macro_stop);

	RibbonGroup *view_group1 = new RibbonGroup(view_section);

	check_eol = new CheckBox(view_group1);
	check_eol->set_text("Show end of line");
	check_eol->func_checked().set(this, &TextEditorRibbon::on_check_eol_checked);
	check_eol->func_unchecked().set(this, &TextEditorRibbon::on_check_eol_unchecked);

	PopupMenu popup_menu;
	popup_menu.insert_item("Invisible").func_clicked().set(this, &TextEditorRibbon::on_combo_wsmode_invisible);
	popup_menu.insert_item("Visible Always").func_clicked().set(this, &TextEditorRibbon::on_combo_wsmode_visible_always);
	popup_menu.insert_item("Visible After Indent").func_clicked().set(this, &TextEditorRibbon::on_combo_wsmode_visible_after_indent);

	combo_wsmode = new ComboBox(view_group1);
	combo_wsmode->set_popup_menu(popup_menu);
	combo_wsmode->set_selected_item(0);
}

void TextEditorRibbon::set_active(DocumentEditor *doc_editor)
{
	editor = dynamic_cast<TextEditor *>(doc_editor);

	// TBD: Clipboard should probably always be visible
	if (editor)
	{
		get_controller()->show_ribbon_section("Home", "Text");
		get_controller()->show_ribbon_section("Home", "Macro");
		get_controller()->show_ribbon_section("Home", "Whitespace");
	}
	else
	{
		get_controller()->hide_ribbon_section("Home", "Text");
		get_controller()->hide_ribbon_section("Home", "Macro");
		get_controller()->hide_ribbon_section("Home", "Whitespace");
	}
}

void TextEditorRibbon::editor_destroyed(DocumentEditor *doc_editor)
{
	if (editor == doc_editor)
		editor = 0;
}

void TextEditorRibbon::on_button_lowercase()
{
	if (editor)
		editor->lowercase();
}

void TextEditorRibbon::on_button_uppercase()
{
	if (editor)
		editor->uppercase();
}

void TextEditorRibbon::on_button_macro_run()
{
	if (editor)
		editor->macro_run();
}

void TextEditorRibbon::on_button_macro_record()
{
	if (editor)
		editor->macro_record();
}

void TextEditorRibbon::on_button_macro_stop()
{
	if (editor)
		editor->macro_stop();
}

void TextEditorRibbon::on_check_eol_checked()
{
	if (editor)
		editor->eol_checked();
}

void TextEditorRibbon::on_check_eol_unchecked()
{
	if (editor)
		editor->eol_unchecked();
}

void TextEditorRibbon::on_combo_wsmode_invisible()
{
	if (editor)
		editor->wsmode_invisible();
}

void TextEditorRibbon::on_combo_wsmode_visible_always()
{
	if (editor)
		editor->wsmode_visible_always();
}

void TextEditorRibbon::on_combo_wsmode_visible_after_indent()
{
	if (editor)
		editor->wsmode_visible_after_indent();
}

}

