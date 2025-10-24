#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/swrender.h>

class CustomComponent : public CL_GUIComponent
{
public:
	CustomComponent(const CL_Rect &position, CL_GUIManager *gui_manager, CL_Colorf color) : CL_GUIComponent(gui_manager, CL_GUITopLevelDescription(position, false))
	{
		this->color = color;

		func_render().set(this, &CustomComponent::on_render);
	}

	void on_render(CL_GraphicContext& gc, const CL_Rect& clip_rect)
	{
		CL_Draw::fill(gc, CL_Rect(0, 0, get_size()), color);

		cl_log_event("test", "on_render: %1, %2", get_size().width, get_size().height);
	}

	CL_Colorf color;
};

class App
{
public:
	int main(const std::vector<CL_String> &args)
	{
		try
		{
			CL_DisplayWindowDescription desc;
			desc.set_title("Custom Component");
			desc.set_size(CL_Size(1024, 768), true);
			CL_DisplayWindow displaywindow(desc);

			CL_Slot slotWindowClose = displaywindow.sig_window_close().connect(this, &App::on_close);

			CL_ResourceManager resources("../../../Resources/GUIThemeAero/resources.xml");

			CL_GUIWindowManagerTexture wm(displaywindow);

			CL_GUIThemeDefault theme;
			theme.set_resources(resources);

			CL_GUIManager gui;
			gui.set_window_manager(wm);
			gui.set_theme(theme); 
			gui.set_css_document("../../../Resources/GUIThemeAero/theme.css");

			CustomComponent comp1(CL_Rect(10, 10, 400, 200), &gui, CL_Colorf::beige);
//			CustomComponent comp2(CL_Rect(20, 210, 500, 400), &gui, CL_Colorf::green);
//			CustomComponent comp3(CL_Rect(50, 20, 600, 300), &gui, CL_Colorf::blue);
			CustomComponent comp3(CL_Rect(87, 200, 600, 300), &gui, CL_Colorf::blue);
//			CustomComponent comp3(CL_Rect(88, 200, 600, 300), &gui, CL_Colorf::blue);

			gui.exec();
		}
		catch (CL_Exception e)
		{
			CL_ConsoleWindow console("Console");
			CL_Console::write_line(e.message);
			console.display_close_message();
		}

		return 0;
	}

	void on_close()
	{
		exit(0);
	}
};

// This is the Program class that is called by CL_ClanApplication
class Program
{
public:
	static int main(const std::vector<CL_String> &args)
	{
		CL_SetupCore setup_core;
		CL_SetupDisplay setup_display;
		CL_SetupGL setup_gl;
//		CL_SetupGDI setup_gl;

		CL_ConsoleWindow window("Output");
		CL_ConsoleLogger logger;

		// Start the Application
		App app;
		return app.main(args);
	}
};

// Instantiate CL_ClanApplication, informing it where the Program is located
CL_ClanApplication app(&Program::main);

