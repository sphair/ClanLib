#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

class App
{
public:
	void on_window_close()
	{
		guimanager.exit_with_code(0);
	}

	int main(const std::vector<CL_String> &args)
	{
		try
		{
			CL_DisplayWindowDescription desc;
			desc.set_title("Window");
			desc.set_size(CL_Size(600, 600), true);
			CL_DisplayWindow displaywindow(desc);

			CL_Slot slot_window_close = displaywindow.sig_window_close().connect(this, &App::on_window_close);

			CL_ResourceManager resources("../../../Resources/GUIThemeAero/resources.xml");

			CL_GUIThemeDefault theme;
			theme.set_resources(resources);

			CL_GUIWindowManagerTexture windowmanager(displaywindow);
		//	CL_GUIWindowManagerSystem windowmanager;

			guimanager.set_window_manager(windowmanager);
			guimanager.set_theme(theme);
			guimanager.set_css_document("../../../Resources/GUIThemeAero/theme.css");

			CL_GUIComponent *toplevelcomponent = new CL_GUIComponent(&guimanager, CL_GUITopLevelDescription(CL_Rect(50, 50, 500, 500), false));
			CL_Window *window = new CL_Window(toplevelcomponent, CL_GUITopLevelDescription(CL_Rect(0, 0, 200, 200), false));

			guimanager.exec();
		}
		catch (CL_Exception e)
		{
			CL_ConsoleWindow console("Console");
			CL_Console::write_line(e.message);
			console.display_close_message();
		}

		return 0;
	}

private:
	CL_GUIManager guimanager;
};

class Program
{
public:
	static int main(const std::vector<CL_String> &args)
	{
		CL_SetupCore setup_core;
		CL_SetupDisplay setup_display;
		CL_SetupGL setup_gl;

		App app;
		return app.main(args);
	}
};

CL_ClanApplication app(&Program::main);
