#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/d3d.h>
using namespace clan;

class CustomComponent : public GUIComponent
{
public:
	CustomComponent(const Rect &position, GUIManager *gui_manager, Colorf color) : GUIComponent(gui_manager, GUITopLevelDescription(position, false))
	{
		this->color = color;

		func_render().set(this, &CustomComponent::on_render);
	}

	void on_render(GraphicContext& gc, const Rect& clip_rect)
	{
		Draw::fill(gc, Rect(0, 0, get_size()), color);

		cl_log_event("test", "on_render: %1, %2", get_size().width, get_size().height);
	}

	Colorf color;
};

class App
{
public:
	int main(const std::vector<std::string> &args)
	{
		try
		{
			DisplayWindowDescription desc;
			desc.set_title("Custom Component");
			desc.set_size(Size(1024, 768), true);
			DisplayWindow displaywindow(desc);

			Slot slotWindowClose = displaywindow.sig_window_close().connect(this, &App::on_close);

			DisplayCache resources("../../../Resources/GUIThemeAero/resources.xml");

			GUIWindowManagerTexture wm(displaywindow);

			GUIThemeDefault theme;
			theme.set_resources(resources);

			GUIManager gui;
			gui.set_window_manager(wm);
			gui.set_theme(theme); 
			gui.set_css_document("../../../Resources/GUIThemeAero/theme.css");

			CustomComponent comp1(Rect(10, 10, 400, 200), &gui, Colorf::beige);
//			CustomComponent comp2(Rect(20, 210, 500, 400), &gui, Colorf::green);
//			CustomComponent comp3(Rect(50, 20, 600, 300), &gui, Colorf::blue);
			CustomComponent comp3(Rect(87, 200, 600, 300), &gui, Colorf::blue);
//			CustomComponent comp3(Rect(88, 200, 600, 300), &gui, Colorf::blue);

			gui.exec();
		}
		catch (Exception e)
		{
			ConsoleWindow console("Console");
			Console::write_line(e.message);
			console.display_close_message();
		}

		return 0;
	}

	void on_close()
	{
		exit(0);
	}
};

// This is the Program class that is called by Application
class Program
{
public:
	static int main(const std::vector<std::string> &args)
	{
		SetupCore setup_core;
		SetupDisplay setup_display;
		SetupGL setup_gl;
//		SetupGDI setup_gl;

		ConsoleWindow window("Output");
		ConsoleLogger logger;

		// Start the Application
		App app;
		return app.main(args);
	}
};

// Instantiate Application, informing it where the Program is located
Application app(&Program::main);

