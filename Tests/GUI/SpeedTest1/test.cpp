#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
using namespace clan;

class FPSLabel : public Label
{
public:
	FPSLabel(GUIComponent *parent)
	: Label(parent), frames_rendered(0)
	{
		cb_label_render = func_render();
		func_render().set(this, &FPSLabel::on_render);
		timer.func_expired().set(this, &FPSLabel::on_5secs_passed);
		timer.start(5000);
	}

private:
	void on_render(Canvas &canvas, const Rect &update_rect)
	{
		frames_rendered++;
		cb_label_render.invoke(canvas, update_rect);
	}

	void on_5secs_passed()
	{
		double fps = frames_rendered / 5.0;
		set_text(string_format("%1 frames per second", fps));
		Console::write_line("%1 frames per second", fps);
		frames_rendered = 0;
	}

	Callback_v2<Canvas &, const Rect &> cb_label_render;
	int frames_rendered;
	Timer timer;
};

class App
{
public:
	Label *label;

	int main(const std::vector<std::string> &args)
	{
		GUIWindowManagerSystem wm;

		GUIManager gui;
		gui.set_window_manager(wm);

		gui.add_theme("../../../Resources/GUIThemeAero/theme.css");
		//gui.add_theme("theme.css");

		gui.add_resources(clan::XMLResourceDocument("../../../Resources/GUIThemeAero/resources.xml"));
		//gui.add_resources(clan::XMLResourceDocument("resources.xml"));

		GUITopLevelDescription window_desc;
		window_desc.set_allow_resize(true);
		window_desc.set_size(Size(600, 600), false);
		Window window(&gui, window_desc);
//			window.set_id_name("mainmenu");
		window.func_close().set(this, &App::on_close, &window);

		GUILayoutCorners layout;
		window.set_layout(layout);

		window.create_components("dialog.xml");

		FPSLabel fps_label(&window);
		fps_label.set_geometry(Rect(10, 10, 100, 40));

		while (!gui.get_exit_flag())
		{
			gui.process_messages(0);
			window.request_repaint();
		}

		return gui.get_exit_code();
	}

	bool on_close(Window *win)
	{
		win->exit_with_code(0);
		return true;
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

		// Start the Application
		App app;
		int retval = app.main(args);
		return retval;
	}
};

// Instantiate Application, informing it where the Program is located
Application app(&Program::main);

