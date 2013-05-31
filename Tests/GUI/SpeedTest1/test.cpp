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
	void on_render(GraphicContext &gc, const Rect &update_rect)
	{
		frames_rendered++;
		cb_label_render.invoke(gc, update_rect);
	}

	void on_5secs_passed()
	{
		double fps = frames_rendered / 5.0;
		set_text(string_format("%1 frames per second", fps));
		Console::write_line("%1 frames per second", fps);
		frames_rendered = 0;
	}

	Callback_v2<GraphicContext &, const Rect &> cb_label_render;
	int frames_rendered;
	Timer timer;
};

class App
{
public:
	Label *label;

	int main(const std::vector<std::string> &args)
	{
		ConsoleWindow console("Console");
		try
		{
			GUIWindowManagerSystem wm;

			GUIManager gui;
			gui.set_window_manager(wm);

			DisplayCache resources("resources.xml");
			DisplayCache resources2("../../../Resources/GUIThemeLuna/resources.xml");
			resources.add_resources(resources2);

			GUIThemeDefault theme;
			theme.set_resources(resources);
			gui.set_theme(theme);
			gui.set_css_document("theme.css");

			GUITopLevelDescription window_desc;
			window_desc.set_allow_resize(true);
			window_desc.set_position(RectPS(500, 600, 270, 140), false);
			Window window(&gui, window_desc);
//			window.set_id_name("mainmenu");
			window.func_close().set(this, &App::on_close, &window);

			// Replace the Tahoma system font with a texture font
			GraphicContext gc = window.get_gc();
			FontDescription font_description;
			font_description.set_typeface_name("Tahoma");
			font_description.set_height(-11);
			Font font_bitmap(gc, font_description);
			gui.register_font(font_bitmap, font_description);
			
			GUILayoutCorners layout;
			window.set_layout(layout);

			window.create_components("dialog.xml");

			FPSLabel fps_label(&window);
			fps_label.set_geometry(Rect(10, 10, 100, 40));

//			gui.exec();

			while (!gui.get_exit_flag())
			{
				gui.process_messages(0);
				window.request_repaint();
			}

			return gui.get_exit_code();
		}
		catch (Exception e)
		{
			Console::write_line(e.message);
			console.display_close_message();
		}

		return 0;
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

