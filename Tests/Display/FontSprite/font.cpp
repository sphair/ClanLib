#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
using namespace clan;

class App : public clan::Application
{
public:
	App();
	bool update() override;
	
private:
	void on_input_up(const InputEvent &key);
	void on_window_close();

private:
	bool quit;
	clan::SlotContainer sc;
	clan::DisplayWindow window;
	clan::Canvas canvas;
	clan::Font font1;
	clan::Font font2;

};

clan::ApplicationInstance<App> clanapp;

App::App()
{
	clan::OpenGLTarget::set_current();

	quit = false;

	window = DisplayWindow("ClanLib Font Sprite Test", 1024, 480);

	// Connect the Window close event
	sc.connect(window.sig_window_close(), this, &App::on_window_close);

	// Connect a keyboard handler to on_key_up()
	sc.connect(window.get_keyboard().sig_key_up(), this, &App::on_input_up);

	// Create the canvas
	canvas = Canvas(window);

	// Load some fonts from the resource file
	ResourceManager resources = clan::XMLResourceManager::create(clan::XMLResourceDocument("font.xml"));

	FontDescription font_desc;
	font_desc.set_height(20);
	font_desc.set_subpixel(false);
	font_desc.set_line_height(26);

	font1 = Font::resource(canvas, "Font1", font_desc, resources);
	font2 = Font::resource(canvas, "Font2", font_desc, resources);
}

bool App::update()
{
	canvas.clear(Colorf::red);

	font1.draw_text(canvas, 25, 25, "ClanLib: Phear the Power!");
			
	font2.draw_text(canvas, 3, 155, "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZzÆæØøÅå0123456789[]()!#$&%/\\=-+~'`\".,:;*?");

	font2.draw_text(canvas, 10.0f, 200.0f, "Hello World\nMy Message");

	// Flip the display, showing on the screen what we have drawed
	// since last call to flip()
	window.flip(1);

	return !quit;
}

// A key was pressed
void App::on_input_up(const InputEvent &key)
{
	if(key.id == keycode_escape)
	{
		quit = true;
	}
}

// The window was closed
void App::on_window_close()
{
	quit = true;
}
