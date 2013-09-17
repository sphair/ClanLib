#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
using namespace clan;

// This is the Application class (That is instantiated by the Program Class)
class App
{
public:
	int start(const std::vector<std::string> &args);

private:
	void on_input_up(const InputEvent &key);
	void on_window_close();

private:
	bool quit;
};

// This is the Program class that is called by Application
class Program
{
public:
	static int main(const std::vector<std::string> &args)
	{
		// Initialize ClanLib base components
		SetupCore setup_core;

		// Initialize the ClanLib display component
		SetupDisplay setup_display;

		// Initilize the OpenGL drivers
		SetupGL setup_gl;

		// Start the Application
		App app;
		int retval = app.start(args);
		return retval;
	}
};

// Instantiate Application, informing it where the Program is located
Application app(&Program::main);

// The start of the Application
int App::start(const std::vector<std::string> &args)
{
	quit = false;

	DisplayWindow window("ClanLib Font Sprite Test", 1024, 480);

	// Connect the Window close event
	Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close);

	// Connect a keyboard handler to on_key_up()
	Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

	// Create the canvas
	Canvas canvas(window);

	// Load some fonts from the resource file
	ResourceManager resources = clan::XMLResourceManager::create(clan::XMLResourceDocument("font.xml"));
	Font font1 = Font::resource(canvas, FontDescription("Font1"), resources);
	Font font2 = Font::resource(canvas, FontDescription("Font2"), resources);

	// Run until someone presses escape
	while (!quit)
	{
		canvas.clear(Colorf::red);

		font1.draw_text(canvas, 25, 25, "ClanLib: Phear the Power!");
			
		font2.draw_text(canvas, 3, 155, "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZzÆæØøÅå0123456789[]()!#$&%/\\=-+~'`\".,:;*?");

		font2.draw_text(canvas, 10.0f, 200.0f, "Hello World\nMy Message");

		// Flip the display, showing on the screen what we have drawed
		// since last call to flip()
		window.flip(1);

		// This call processes user input and other events
		KeepAlive::process();
	}

	return 0;
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
