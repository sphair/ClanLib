#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

class FPSLabel : public CL_Label
{
public:
	FPSLabel(CL_GUIComponent *parent)
	: CL_Label(parent), frames_rendered(0)
	{
		cb_label_render = func_render();
		func_render().set(this, &FPSLabel::on_render);
		timer.func_expired().set(this, &FPSLabel::on_5secs_passed);
		timer.start(5000);
	}

private:
	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
	{
		frames_rendered++;
		cb_label_render.invoke(gc, update_rect);
	}

	void on_5secs_passed()
	{
		double fps = frames_rendered / 5.0;
		set_text(cl_format("%1 frames per second", fps));
		CL_Console::write_line("%1 frames per second", fps);
		frames_rendered = 0;
	}

	CL_Callback_v2<CL_GraphicContext &, const CL_Rect &> cb_label_render;
	int frames_rendered;
	CL_Timer timer;
};

class App
{
public:
	CL_Label *label;

	int main(const std::vector<CL_String> &args)
	{
		CL_ConsoleWindow console("Console");
		try
		{
			CL_GUIWindowManagerSystem wm;

			CL_GUIManager gui;
			gui.set_window_manager(wm);

			CL_ResourceManager resources("resources.xml");
			CL_ResourceManager resources2("../../../Resources/GUIThemeLuna/resources.xml");
			resources.add_resources(resources2);

			CL_GUIThemeDefault theme;
			theme.set_resources(resources);
			gui.set_theme(theme);
			gui.set_css_document("theme.css");

			CL_GUITopLevelDescription window_desc;
			window_desc.set_allow_resize(true);
			window_desc.set_position(CL_RectPS(500, 600, 270, 140), false);
			CL_Window window(&gui, window_desc);
//			window.set_id_name("mainmenu");
			window.func_close().set(this, &App::on_close, &window);

			// Replace the Tahoma system font with a texture font
			CL_GraphicContext gc = window.get_gc();
			CL_FontDescription font_description;
			font_description.set_typeface_name("Tahoma");
			font_description.set_height(-11);
			CL_Font font_bitmap(gc, font_description);
			gui.register_font(font_bitmap, font_description);
			
			CL_GUILayoutCorners layout;
			window.set_layout(layout);

			window.create_components("dialog.xml");

			FPSLabel fps_label(&window);
			fps_label.set_geometry(CL_Rect(10, 10, 100, 40));

//			gui.exec();

			while (!gui.get_exit_flag())
			{
				gui.process_messages(0);
				window.request_repaint();
			}

			return gui.get_exit_code();
		}
		catch (CL_Exception e)
		{
			CL_Console::write_line(e.message);
			console.display_close_message();
		}

		return 0;
	}

	bool on_close(CL_Window *win)
	{
		win->exit_with_code(0);
		return true;
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

		// Start the Application
		App app;
		int retval = app.main(args);
		return retval;
	}
};

// Instantiate CL_ClanApplication, informing it where the Program is located
CL_ClanApplication app(&Program::main);

