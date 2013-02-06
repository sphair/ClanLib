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
namespace clan
{
class SolutionModel;

class DlgAddNewProject : public clan::GUIComponent
{
public:
	DlgAddNewProject(clan::GUIComponent *owner, SolutionModel &model);

private:
	void on_resized();
	bool on_close();
	void on_button_ok_clicked();
	void on_button_cancel_clicked();
	void on_button_browse_location_clicked();
	void on_check_create_solution_directory_state_changed();
	static clan::GUITopLevelDescription get_toplevel_description();

	clan::Label *label_name;
	clan::LineEdit *lineedit_name;
	clan::Label *label_location;
	clan::LineEdit *lineedit_location;
	clan::PushButton *button_browse_location;
	clan::Label *label_solution_name;
	clan::LineEdit *lineedit_solution_name;
	clan::CheckBox *check_create_solution_directory;
	clan::PushButton *button_ok;
	clan::PushButton *button_cancel;

	SolutionModel &model;
};

}

