#include <ClanLib/application.h>
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/gl1.h>
#include <ClanLib/gdi.h>
#include <ClanLib/gui.h>

//Enable this line if you have ClanSDL installed
//#define HAVE_CLANSDL

#ifdef HAVE_CLANSDL
#include <ClanLib/sdl.h>
#endif

#include <cstdlib>

#include "framerate_counter.h"

typedef struct _FontFall
{
	CL_StringRef text;
	int xpos;
	float ypos;
	CL_Colorf color;
} FontFall;

class App
{
public:
	enum RenderTarget
	{
		opengl1,
#ifdef HAVE_CLANSDL
		sdl,
#endif
		opengl2,
		gdi
	};

	App();

	int start(const std::vector<CL_String> &args);

private:
	void on_button_clicked(RenderTarget selected_target);
	bool on_close(CL_Window *win);
	void on_input_up(const CL_InputEvent &key, const CL_InputState &state);
	void on_window_close();

	bool get_display_target();

	void run_demo();
	FontFall new_fontfall(int window_width);

private:
	RenderTarget render_target;
	CL_Window *current_window;
	bool quit;
};

App::App()
{
	render_target = gdi;
}

int App::start(const std::vector<CL_String> &args)
{
	try
	{
		CL_SetupGL target_opengl2;
		CL_SetupGL1 target_opengl1;
#ifdef HAVE_CLANSDL
		CL_SetupSDL target_sdl;
#endif
		CL_SetupGDI target_gdi;

		// Since GDI is compatible and fast - Use that as the default
		target_gdi.set_current();

		if (get_display_target())
		{
			switch (render_target)
			{
				case (opengl1):
					target_opengl1.set_current();
					break;

				case (opengl2):
					target_opengl2.set_current();
					break;

				case (gdi):
					target_gdi.set_current();
					break;

#ifdef HAVE_CLANSDL
				case (sdl):
					target_sdl.set_current();
					break;
#endif
			}
			run_demo();
		}
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

bool App::get_display_target()
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
	win_desc.set_title("Display Target Application");
	win_desc.set_position(CL_Rect(200, 200, 540, 440), false);
	CL_Window window(&gui, win_desc);
	current_window = &window;
	window.func_close().set(this, &App::on_close, &window);

	CL_GUILayoutCorners layout;
	window.set_layout(layout);

	window.create_components("layout.xml");

	CL_PushButton *button;
	button = dynamic_cast<CL_PushButton*>(window.get_named_item("button_gl"));
	if (!button)
		throw CL_Exception("Who has stolen button_gl?");
	button->func_clicked().set(this, &App::on_button_clicked, opengl2);

	button = dynamic_cast<CL_PushButton*>(window.get_named_item("button_gl1"));
	if (!button)
		throw CL_Exception("Who has stolen button_gl1?");
	button->func_clicked().set(this, &App::on_button_clicked, opengl1);

	button = dynamic_cast<CL_PushButton*>(window.get_named_item("button_sdl"));
	if (!button)
		throw CL_Exception("Who has stolen button_sdl?");
#ifdef HAVE_CLANSDL
	button->func_clicked().set(this, &App::on_button_clicked, sdl);
#else
	button->set_enabled(false);
#endif

	button = dynamic_cast<CL_PushButton*>(window.get_named_item("button_gdi"));
	if (!button)
		throw CL_Exception("Who has stolen button_gdi?");
	button->func_clicked().set(this, &App::on_button_clicked, gdi);

	if (gui.exec())
	{
		return false;
	}
	return true;
}

void App::on_button_clicked(RenderTarget selected_target)
{
	render_target = selected_target;
	current_window->exit_with_code(0);
}

bool App::on_close(CL_Window *win)
{
	win->exit_with_code(1);
	return true;
}

void App::run_demo()
{
	CL_DisplayWindowDescription desc;
	desc.set_title("Target Display (with resizable window)");
	desc.set_size(CL_Size(640, 480), true);
	desc.set_allow_resize(true);
	CL_DisplayWindow window(desc);

	// Connect the Window close event
	CL_Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close);

	// Connect a keyboard handler to on_key_up()
	CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

	// Get the graphic context
	CL_GraphicContext gc = window.get_gc();

	CL_Font target_font(gc, "tahoma", 32);
	CL_Font fps_font(gc, "tahoma", 20);
	CL_Font fall_font(gc, "tahoma", 16);

	std::vector<FontFall> font_fall;

	FramerateCounter frameratecounter;

	unsigned int time_last = CL_System::get_time();
	float fontfall_ticker = 0.0f;

	quit = false;
	// Run until someone presses escape
	while (!quit)
	{
		unsigned int time_now = CL_System::get_time();
		float time_delta = (float) (time_now - time_last);
		time_last = time_now;

		fontfall_ticker += time_delta;
		if (fontfall_ticker >= 10.0f)
		{
			fontfall_ticker = 0.0f;
			font_fall.push_back(new_fontfall(gc.get_width()));
		}

		gc.clear(CL_Colorf(0.0f,0.0f,0.0f));

		CL_String fps = cl_format("%1 fps", frameratecounter.get_framerate());
		fps_font.draw_text(gc, gc.get_width() - 100, 30, fps);

		CL_String words = cl_format("%1 words", (int) font_fall.size());
		fps_font.draw_text(gc, 100, 30, words);

		const int font_xpos = 32;
		const int font_ypos = 64;
		switch (render_target)
		{
			case (opengl1):
				target_font.draw_text(gc, font_xpos, font_ypos, "OpenGL 1.3 (clanGL1)");
				break;

			case (opengl2):
				target_font.draw_text(gc, font_xpos, font_ypos, "OpenGL 2.0 and better (clanGL)");
				break;

			case (gdi):
				target_font.draw_text(gc, font_xpos, font_ypos, "Software renderer (clanGDI)");
				break;

#ifdef HAVE_CLANSDL
			case (sdl):
				target_font.draw_text(gc, font_xpos, font_ypos, "SDL Renderer (clanSDL)");
				break;
#endif
		}

		float max_height = (float) (gc.get_height() + 20);
		float half_height = (float) gc.get_height() / 2.0f;
		for (std::vector<FontFall>::iterator it = font_fall.begin(); it != font_fall.end();)
		{
			FontFall &item = *it;
			item.ypos += time_delta / 10.0f;
			if (item.ypos >= max_height)
			{
				it = font_fall.erase(it);
			}
			else
			{
				item.color.a = item.ypos / (half_height);
				if (item.color.a > 1.0f)
					item.color.a = 1.0f;
				fall_font.draw_text(gc, item.xpos, (int) item.ypos, item.text, item.color);
				++it;
			}
		}

		window.flip(0);
		frameratecounter.frame_shown();

		CL_KeepAlive::process(0);
	}
}

FontFall App::new_fontfall(int window_width)
{
	static CL_String words[] = 
	{
		"Apple","Ball","ClanLib","Dog","Egg","Fish","Ghost","Hunter",
		"Ice cream","Jumper","Kite","Lion","Matrix","Nose","Orange","Panda",
		"Queen","Ruler","Screen saver","Trousers","Umbrella","Violin","Window","Xylophone",
		"Yacht","Zebra"
	};

	static int word_counter = 0;

	FontFall fontfall;
	// (Note, do not randomize the word order, because it would will make comparing display target fps more difficult)
	fontfall.text = words[++word_counter % (sizeof(words)/sizeof(CL_String))];
	unsigned int value = rand();
	fontfall.xpos = (value % window_width) - 10;
	fontfall.ypos = 0.0f;
	fontfall.color.r = (rand() & 255) / 255.0f;
	fontfall.color.g = (rand() & 255) / 255.0f;
	fontfall.color.b = (rand() & 255) / 255.0f;
	fontfall.color.a = 1.0f;
	return fontfall;
}

// This is the Program class that is called by CL_ClanApplication
class Program
{
public:
	static int main(const std::vector<CL_String> &args)
	{
		CL_SetupCore setup_core;
		CL_SetupDisplay setup_display;
		CL_SetupGUI setup_gui;

		// Start the Application
		App app;
		int retval = app.start(args);
		return retval;
	}
};

// A key was pressed
void App::on_input_up(const CL_InputEvent &key, const CL_InputState &state)
{
	if(key.id == CL_KEY_ESCAPE)
	{
		quit = true;
	}
}

// The window was closed
void App::on_window_close()
{
	quit = true;
}

// Instantiate CL_ClanApplication, informing it where the Program is located
CL_ClanApplication app(&Program::main);

