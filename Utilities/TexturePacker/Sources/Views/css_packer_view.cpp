#include "precomp.h"
#include "css_packer_view.h"

CssPackerView::CssPackerView(CL_GUIComponent *parent, MainWindow *mainwindow)
: View(parent, mainwindow, "CSS Packer")
{
	func_resized().set(this, &CssPackerView::on_resized);

	label_selected_file = new CL_Label(this);
	label_selected_file->set_text("Input CSS file:");

	lineedit_selected_file = new CL_LineEdit(this);

	button_browse_file = new CL_PushButton(this);
	button_browse_file->set_text("...");
	button_browse_file->func_clicked().set(this, &CssPackerView::on_button_browse);

	button_save = new CL_PushButton(this);
	button_save->set_text("Save CSS file");
	button_save->func_clicked().set(this, &CssPackerView::on_button_save);

	label_result = new CL_Label(this);

	on_resized();
}

CssPackerView::~CssPackerView()
{
}

void CssPackerView::on_resized()
{
	CL_Size size = get_size();

	label_selected_file->set_geometry(CL_Rect(10, 13, 80, 30));
	lineedit_selected_file->set_geometry(CL_Rect(86, 10, size.width - 20 - 20, 30));
	button_browse_file->set_geometry(CL_Rect(10 + size.width - 20 - 30, 10, 10 + size.width - 20, 30));

	button_save->set_geometry(CL_Rect(size.width - 130, 70, CL_Size(120, 25)));

	label_result->set_geometry(CL_Rect(10, 100, size.width - 20, 130));
}

void CssPackerView::on_button_browse()
{
	CL_OpenFileDialog dlg(this);
	dlg.add_filter("CSS files", "*.css", true);
	dlg.add_filter("All files", "*.*");

	dlg.set_initial_directory(CL_System::get_exe_path());

	if(dlg.show())
	{
		CL_String filename = dlg.get_filename();

		try
		{
			int imports = load_css_file(filename);

			label_result->set_text(cl_format("Loaded CSS file with %1 imports", imports));

			lineedit_selected_file->set_text(filename);
		}
		catch(CL_Exception error)
		{
			label_result->set_text(cl_format("Error: %1", error.message));
		}
	}
}

void CssPackerView::on_button_save()
{
	CL_SaveFileDialog dlg(this);

	dlg.add_filter("CSS files", "*.css", true);
	dlg.add_filter("All files", "*.*");

	dlg.set_initial_directory(CL_System::get_exe_path());

	if(dlg.show())
	{
		CL_String filename = dlg.get_filename();

		try
		{
			save_css_file(filename);

			label_result->set_text(cl_format("Saved resources: %1", filename));
		}
		catch(CL_Exception error)
		{
			label_result->set_text(cl_format("Save error: %1", error.message));
		}
	}
}

int CssPackerView::load_css_file(const CL_String &file)
{
	return packer.load(file);
}

void CssPackerView::save_css_file(const CL_String &file)
{
	packer.save(file);
}
