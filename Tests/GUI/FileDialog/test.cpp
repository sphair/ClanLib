#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gdi.h>

class App
{
public:
	int main(const std::vector<CL_String> &args)
	{
		CL_ConsoleWindow console("Console");

		try
		{
			CL_ResourceManager resources;
			resources.load("../../../Resources/GUIThemeAero/resources.xml");

			CL_GUIManager gui;

			CL_GUIWindowManagerSystem wm;
			gui.set_window_manager(wm);

			CL_GUIThemeDefault theme;
			theme.set_resources(resources);
			gui.set_theme(theme);
			gui.set_css_document("../../../Resources/GUIThemeAero/theme.css");

			CL_DisplayWindowDescription win_desc;
			win_desc.set_allow_resize(true);
			win_desc.set_title("FileDialog");
			win_desc.set_position(CL_Rect(200,200,600,600), false);

			window = new CL_Window(&gui, win_desc);			
			window->func_close().set(this, &App::on_close); 

 			lineedit = new CL_LineEdit(window);
			lineedit->set_geometry(CL_Rect(20, 20, 355, 40));
 			lineedit->set_text(""); 

			button = new CL_PushButton(window);
			button->set_geometry(CL_Rect(355, 20, 375, 40));
			button->set_text("...");
			button->func_clicked().set(this, &App::on_button_clicked);

			gui.exec();
		}
		catch (CL_Exception e)
		{
 			CL_Console::write_line(e.message);
			console.display_close_message();
		}

		return 0;
	}

	void on_close()
	{
		window->exit_with_code(0);
	}

	void on_button_clicked()
	{
		CL_OpenFileDialog dialog(window);

		dialog.add_filter("All", "*.*");
		dialog.add_filter("Code", "*.cpp;*.h", true);
		dialog.add_filter("XML", "*.xml");

		dialog.set_title("Please select a file");

		dialog.set_multi_select(true);

		bool ok = dialog.show();
		if(ok)
			lineedit->set_text(dialog.get_filename());
	}

	CL_Window *window;
	CL_LineEdit *lineedit;
	CL_PushButton *button;
};

// This is the Program class that is called by CL_ClanApplication
class Program
{
public:
	static int main(const std::vector<CL_String> &args)
	{
		CL_SetupCore setup_core;
		CL_SetupDisplay setup_display;
		CL_SetupGDI setup_gdi;

		// Start the Application
		App app;
		int retval = app.main(args);
		return retval;
	}
};

// Instantiate CL_ClanApplication, informing it where the Program is located
CL_ClanApplication app(&Program::main);

