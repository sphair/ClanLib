
#pragma once

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
