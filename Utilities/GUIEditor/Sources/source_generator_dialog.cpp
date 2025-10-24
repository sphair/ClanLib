/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
**    Harry Storbacka
*/

#include "precomp.h"
#include "source_generator_dialog.h"

/////////////////////////////////////////////////////////////////////////////
// SourceGenerator Construction:

SourceGeneratorDialog::SourceGeneratorDialog(CL_GUIManager *gui_manager)
: CL_Window(gui_manager, get_startup_description())
{
	set_layout(layout);

	create_components("Dialogs/source_generator.xml");

	btn_generate = dynamic_cast<CL_PushButton*>(get_named_item("button_generate"));
	btn_browse_h = dynamic_cast<CL_PushButton*>(get_named_item("button_browse_h"));
	btn_browse_cpp = dynamic_cast<CL_PushButton*>(get_named_item("button_browse_cpp"));
	combo_h_filename = dynamic_cast<CL_ComboBox*>(get_named_item("combobox_header"));
	combo_cpp_filename = dynamic_cast<CL_ComboBox*>(get_named_item("combobox_source"));

	btn_generate->func_clicked().set(this, &SourceGeneratorDialog::on_btn_generate);
	btn_browse_h->func_clicked().set(this, &SourceGeneratorDialog::on_btn_browse_h);
	btn_browse_cpp->func_clicked().set(this, &SourceGeneratorDialog::on_btn_browse_cpp);
}

/////////////////////////////////////////////////////////////////////////////
// SourceGenerator Attributes:


/////////////////////////////////////////////////////////////////////////////
// SourceGenerator Operations:


/////////////////////////////////////////////////////////////////////////////
// SourceGenerator Implementation:

CL_GUITopLevelDescription SourceGeneratorDialog::get_startup_description()
{
	CL_GUITopLevelDescription desc;
	desc.set_title("ClanLib GUI Editor");
	desc.set_allow_resize(true);
	desc.set_position(CL_RectPS(400, 400, 104, 208), false);
	return desc;
}

void SourceGeneratorDialog::on_btn_browse_cpp()
{
//	CL_String filename = show_open_file_dialog();
//	combo_cpp_filename->set_text(filename);
}

void SourceGeneratorDialog::on_btn_browse_h()
{
//	CL_String filename = show_open_file_dialog();
//	combo_h_filename->set_text(filename);
}

void SourceGeneratorDialog::on_btn_generate()
{

}
