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

}

