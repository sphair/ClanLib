/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
#include "document_editor_ribbon.h"
#include "API/GameIDE/UIController/document_editor.h"
#include "API/GameIDE/UIController/ui_controller.h"
#include "API/GameIDE/UIController/ui_ribbon_section.h"

namespace clan
{

DocumentEditorRibbon::DocumentEditorRibbon(UIController *controller)
	: UIControllerListener(controller), clipboard_section(0), button_cut(0), button_paste(0), button_copy(0), editor(0)
{
	clipboard_section = new UIRibbonSection(controller, "Home", "Clipboard");

	XMLResourceDocument resources = clipboard_section->get_resources();
	Canvas canvas = clipboard_section->get_canvas();

	RibbonGroup *clipboard_group1 = new RibbonGroup(clipboard_section);
	RibbonGroup *clipboard_group2 = new RibbonGroup(clipboard_section);

	button_paste = new PushButton(clipboard_group1);
	button_cut = new PushButton(clipboard_group2);
	button_copy = new PushButton(clipboard_group2);

	button_paste->set_text("Paste");
	button_paste->set_icon(Image(canvas, "IconClipboardPaste24", &resources));
	button_paste->set_icon_position(PushButton::icon_top);
	button_cut->set_text("Cut");
	button_copy->set_text("Copy");

	button_paste->func_clicked().set(this, &DocumentEditorRibbon::on_button_paste_clicked);
	button_cut->func_clicked().set(this, &DocumentEditorRibbon::on_button_cut_clicked);
	button_copy->func_clicked().set(this, &DocumentEditorRibbon::on_button_copy_clicked);

	edit_section = new UIRibbonSection(controller, "Home", "Edit");

	RibbonGroup *edit_group1 = new RibbonGroup(edit_section);
	RibbonGroup *edit_group2 = new RibbonGroup(edit_section);

	button_undo = new PushButton(edit_group1);
	button_redo = new PushButton(edit_group1);
	button_save = new PushButton(edit_group2);

	button_undo->set_text("Undo");
	button_undo->set_icon(Image(canvas, "IconEditUndo", &resources));
	button_undo->set_icon_position(PushButton::icon_left);
	button_redo->set_text("Redo");
	button_redo->set_icon(Image(canvas, "IconEditRedo", &resources));
	button_redo->set_icon_position(PushButton::icon_left);
	button_save->set_text("Save");
	button_save->set_icon(Image(canvas, "IconSave24", &resources));
	button_save->set_icon_position(PushButton::icon_top);

	button_undo->func_clicked().set(this, &DocumentEditorRibbon::on_button_undo_clicked);
	button_redo->func_clicked().set(this, &DocumentEditorRibbon::on_button_redo_clicked);
	button_save->func_clicked().set(this, &DocumentEditorRibbon::on_button_save_clicked);

	// Always show the clipboard and save buttons
	clipboard_section->show_section(true);
	edit_section->show_section(true);
}

void DocumentEditorRibbon::set_active(DocumentEditor *editor)
{
	this->editor = editor;
}

void DocumentEditorRibbon::editor_destroyed(DocumentEditor *editor)
{
	if (this->editor == editor)
		this->editor = 0;
}

void DocumentEditorRibbon::on_button_cut_clicked()
{
	if (editor)
		editor->cut();
}

void DocumentEditorRibbon::on_button_paste_clicked()
{
	if (editor)
		editor->paste();
}

void DocumentEditorRibbon::on_button_copy_clicked()
{
	if (editor)
		editor->copy();
}

void DocumentEditorRibbon::on_button_undo_clicked()
{
	if (editor)
		editor->undo();
}

void DocumentEditorRibbon::on_button_redo_clicked()
{
	if (editor)
		editor->redo();
}

void DocumentEditorRibbon::on_button_save_clicked()
{
	if (editor)
		editor->save();
}

}
