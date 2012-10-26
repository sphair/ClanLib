#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
using namespace clan;

class App
{
public:
	void on_window_close()
	{
		guimanager.exit_with_code(0);
	}

	int main(const std::vector<std::string> &args)
	{
		try
		{
			DisplayWindowDescription desc;
			desc.set_title("Window");
			desc.set_size(Size(600, 600), true);
			DisplayWindow displaywindow(desc);

			Slot slot_window_close = displaywindow.sig_window_close().connect(this, &App::on_window_close);

			ResourceManager resources("../../../Resources/GUIThemeAero/resources.xml");

			GUIThemeDefault theme;
			theme.set_resources(resources);

			GUIWindowManagerTexture windowmanager(displaywindow);
		//	GUIWindowManagerSystem windowmanager;

			guimanager.set_window_manager(windowmanager);
			guimanager.set_theme(theme);
			guimanager.set_css_document("../../../Resources/GUIThemeAero/theme.css");

			GUIComponent *toplevelcomponent = new GUIComponent(&guimanager, GUITopLevelDescription(Rect(50, 50, 500, 500), false));
			Window *window = new Window(toplevelcomponent, GUITopLevelDescription(Rect(0, 0, 200, 200), false));

			guimanager.exec();
		}
		catch (Exception e)
		{
			ConsoleWindow console("Console");
			Console::write_line(e.message);
			console.display_close_message();
		}

		return 0;
	}

private:
	GUIManager guimanager;
};

class Program
{
public:
	static int main(const std::vector<std::string> &args)
	{
		SetupCore setup_core;
		SetupDisplay setup_display;
		SetupGL setup_gl;

		App app;
		return app.main(args);
	}
};

Application app(&Program::main);
