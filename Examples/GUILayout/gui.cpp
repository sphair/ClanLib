#include <ClanLib/application.h>
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/gui.h>

class App
{
public:
	int start(const std::vector<CL_String> &args);

private:
	void on_button_clicked(CL_PushButton *button);
	bool on_close(CL_Window *win);

private:
	CL_Label *label;
};

int App::start(const std::vector<CL_String> &args)
{
	try
	{
		CL_GUIManager gui;

		CL_String resource_filename("../../Resources/GUIThemeAero/resources.xml");
		CL_String theme_filename("../../Resources/GUIThemeAero/theme.css");
		if (!CL_FileHelp::file_exists(resource_filename))
		{
			resource_filename = "../../Resources/GUIThemeBasic/resources.xml";
			theme_filename = "../../Resources/GUIThemeBasic/theme.css";
		}

		CL_ResourceManager resources(resource_filename);

		CL_GUIWindowManagerSystem wm;
		gui.set_window_manager(wm);

		CL_GUIThemeDefault theme;
		theme.set_resources(resources);
		gui.set_theme(theme); 
		gui.set_css_document(theme_filename);

		CL_DisplayWindowDescription win_desc;
		win_desc.set_allow_resize(true);
		win_desc.set_title("GUILayout Test Application");
		win_desc.set_position(CL_Rect(200, 200, 540, 440), false);
		CL_Window window(&gui, win_desc);
		window.func_close().set(this, &App::on_close, &window);

		CL_GUILayoutCorners layout;
		window.set_layout(layout);

		window.create_components("layout.xml");

		CL_PushButton *button = dynamic_cast<CL_PushButton*>(window.get_named_item("MyButton"));
		if (!button)
			throw CL_Exception("Who has stolen MyButton?");
		button->func_clicked().set(this, &App::on_button_clicked, button);

		label = dynamic_cast<CL_Label*>(window.get_named_item("MyLabel"));
		if (!button)
			throw CL_Exception("Who has stolen MyLabel?");

		gui.exec();
	}
	catch(CL_Exception &exception)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console", 80, 160);
		CL_Console::write_line("Exception caught: " + exception.get_message_and_stack_trace());
		console.display_close_message();

		return -1;
	}
	return 0;
}

void App::on_button_clicked(CL_PushButton *button)
{
	label->set_text("You clicked " + button->get_text());
}

bool App::on_close(CL_Window *win)
{
	win->exit_with_code(0);
	return true;
}

// This is the Program class that is called by CL_ClanApplication
class Program
{
public:
	static int main(const std::vector<CL_String> &args)
	{
		CL_SetupCore setup_core;
		CL_SetupDisplay setup_display;
		CL_SetupGL setup_gl;
		CL_SetupGUI setup_gui;

		// Start the Application
		App app;
		int retval = app.start(args);
		return retval;
	}
};

// Instantiate CL_ClanApplication, informing it where the Program is located
CL_ClanApplication app(&Program::main);

