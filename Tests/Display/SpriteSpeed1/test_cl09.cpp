#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

// This is the Application class (That is instantiated by the Program Class)
class App
{
public:
	int start(const std::vector<CL_String> &args);

private:
	void on_input_up(const CL_InputEvent &key, const CL_InputState &state);
	void on_window_close();
	int dump_fps();
	int calc_fps(int frame_time);

	void draw_equal_tex_equal_sprites(CL_GraphicContext &gc, int sprite_count, float time_elapsed);
	void draw_equal_tex_diff_sprites(CL_GraphicContext &gc, int sprite_count, float time_elapsed);
	void draw_equal_tex_diff_sprites_batch(CL_GraphicContext &gc, int sprite_count, float time_elapsed);

	void draw_diff_tex_diff_sprites(CL_GraphicContext &gc, int sprite_count, float time_elapsed);
	void draw_diff_tex_diff_sprites_batch(CL_GraphicContext &gc, int sprite_count, float time_elapsed);

private:
	bool quit;

	CL_Sprite explosion1;
	CL_Sprite explosion2;
	std::vector<CL_Sprite> explosions_same_tex;
	std::vector<CL_Sprite> explosions_diff_tex;

	int running_test;
};

// This is the Program class that is called by CL_ClanApplication
class Program
{
public:
	static int main(const std::vector<CL_String> &args)
	{
		// Initialize ClanLib base components
		CL_SetupCore setup_core;

		// Initialize the ClanLib display component
		CL_SetupDisplay setup_display;

		// Initilize the OpenGL drivers
		CL_SetupGL setup_gl;

		// Start the Application
		App app;
		int retval = app.start(args);
		return retval;
	}
};

// Instantiate CL_ClanApplication, informing it where the Program is located
CL_ClanApplication app(&Program::main);

// The start of the Application
int App::start(const std::vector<CL_String> &args)
{
	quit = false;

	running_test = 0;

	// Create a console window for text-output if not available
	CL_ConsoleWindow console("Console", 80, 200);
	
	CL_Console::write_line("Press 1-5 for different tests! (Test 3 and 5 not applicable for ClanLib 0.8)");			

	try
	{
		// This opens a window, including the frame size
		// If you want more control over the window, pass CL_DisplayWindowDescription to CL_DisplayWindow
		// (This is useful to create a borderless window of a specific size)
		// If you require target specific control over the window, use the derived CL_OpenGLWindowDescription
		// (This contains the multisampling options)
		CL_DisplayWindow window("ClanLib SpriteSpeed Test", 1000, 1000);

		// Connect the Window close event
		CL_Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close);

		// Connect a keyboard handler to on_key_up()
		CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

		// Get the graphic context
		CL_GraphicContext gc = window.get_gc();

		CL_ResourceManager resources("resources.xml");

		explosion1 = CL_Sprite(gc, "Explosion1", &resources);
		explosion2 = CL_Sprite(gc, "Explosion2", &resources);

		explosions_same_tex.reserve(10000);
		for(int i=0; i<10000; i++)
		{
			explosions_same_tex.push_back(CL_Sprite(explosion1));

			if(i % 2 == 0)
				explosions_diff_tex.push_back(CL_Sprite(explosion1));
			else
				explosions_diff_tex.push_back(CL_Sprite(explosion2));
		}

		// Run until someone presses escape
		while (!quit)
		{
			float delta_time = dump_fps() / 1000.0f;

			gc.clear(CL_Colorf(0.0f,0.0f,0.2f));

			if(running_test == 0)
				CL_System::sleep(100);
			if(running_test == 1)
				draw_equal_tex_equal_sprites(gc, 10000, delta_time);
			if(running_test == 2)
				draw_equal_tex_diff_sprites(gc, 10000, delta_time);
			if(running_test == 3)
				draw_equal_tex_diff_sprites_batch(gc, 10000, delta_time);
			if(running_test == 4)
				draw_diff_tex_diff_sprites(gc, 10000, delta_time);
			if(running_test == 5)
				draw_diff_tex_diff_sprites_batch(gc, 10000, delta_time);

			// Flip the display, showing on the screen what we have drawed since last call to flip()
			window.flip();

			// This call processes user input and other events
			CL_KeepAlive::process();
		}

		explosions_same_tex.clear();
		explosions_diff_tex.clear();
		explosion1 = CL_Sprite();
		explosion2 = CL_Sprite();
	}
	catch(CL_Exception& exception)
	{
		CL_Console::write_line("Exception caught:");
		CL_Console::write_line(exception.message);

		// Display the stack trace (if available)
		std::vector<CL_String> stacktrace = exception.get_stack_trace();
		int size = stacktrace.size();
		if (size > 0)
		{
			CL_Console::write_line("Stack Trace:");
			for (int cnt=0; cnt < size; cnt++)
			{
				CL_Console::write_line(stacktrace[cnt]);
			}
		}

		console.display_close_message();

		return -1;
	}
	return 0;
}

// A key was pressed
void App::on_input_up(const CL_InputEvent &key, const CL_InputState &state)
{
	if(key.id == CL_KEY_ESCAPE)
		quit = true;

	if(key.id == CL_KEY_1 && running_test != 1)
	{
		running_test = 1;
		CL_Console::write_line("Running test 1: draw_equal_tex_equal_sprites");
	}
	if(key.id == CL_KEY_2 && running_test != 2)
	{
		running_test = 2;
		CL_Console::write_line("Running test 2: draw_equal_tex_diff_sprites");
	}
	if(key.id == CL_KEY_3 && running_test != 3)
	{
		running_test = 3;
		CL_Console::write_line("Running test 3: draw_equal_tex_diff_sprites_batch");
	}
	if(key.id == CL_KEY_4 && running_test != 4)
	{
		running_test = 4;
		CL_Console::write_line("Running test 4: draw_diff_tex_diff_sprites");
	}
	if(key.id == CL_KEY_5 && running_test != 5)
	{
		running_test = 5;
		CL_Console::write_line("Running test 5: draw_diff_tex_diff_sprites_batch");
	}
}

// The window was closed
void App::on_window_close()
{
	quit = true;
}

int App::dump_fps()
{
	// Calculate time since last update
	static int start_time = CL_System::get_time();
	static int fps_dump_time = 0;

	int cur_time = CL_System::get_time();
	int delta_time = cur_time - start_time;
	start_time = cur_time;
	
	int fps = calc_fps(delta_time);

	fps_dump_time += delta_time;
	if(fps_dump_time >= 1000)
	{
		CL_Console::write_line("fps:" + CL_StringHelp::int_to_text(fps));
		fps_dump_time = 0;
	}

	return delta_time;
}

int App::calc_fps(int frame_time)
{
	static int fps_result = 0;
	static int fps_counter = 0;
	static int total_time = 0;

	total_time += frame_time;
	if(total_time >= 1000)	// One second has passed
	{
		fps_result = fps_counter + 1;
		fps_counter = total_time = 0;
	}
	fps_counter++;	// Increase fps

	return fps_result;
}

void App::draw_equal_tex_equal_sprites(CL_GraphicContext &gc, int sprite_count, float time_elapsed)
{
	int count = 0;
	for(int x=0; x<100; ++x)
	{
		for(int y=0; y<100; ++y)
		{
			explosion1.draw(gc, x * 10.0f, y * 10.0f);
			explosion1.update(time_elapsed);

			count++;
			if(count == sprite_count)
				return;
		}
	}
}

void App::draw_equal_tex_diff_sprites(CL_GraphicContext &gc, int sprite_count, float time_elapsed)
{
	int count = 0;
	for(int x=0; x<100; ++x)
	{
		for(int y=0; y<100; ++y)
		{
			explosions_same_tex[count].draw(gc, x * 10.0f, y * 10.0f);
			explosions_same_tex[count].update(time_elapsed);

			count++;
			if(count == sprite_count || count == 10000)
				return;
		}
	}
}

void App::draw_equal_tex_diff_sprites_batch(CL_GraphicContext &gc, int sprite_count, float time_elapsed)
{
	// Batching is builtin in 2.0..
	draw_equal_tex_diff_sprites(gc, sprite_count, time_elapsed);
}

void App::draw_diff_tex_diff_sprites(CL_GraphicContext &gc, int sprite_count, float time_elapsed)
{
	int count = 0;
	for(int x=0; x<100; ++x)
	{
		for(int y=0; y<100; ++y)
		{
			explosions_diff_tex[count].draw(gc, x * 10.0f, y * 10.0f);
			explosions_diff_tex[count].update(time_elapsed);

			count++;
			if(count == sprite_count || count == 10000)
				return;
		}
	}
}

void App::draw_diff_tex_diff_sprites_batch(CL_GraphicContext &gc, int sprite_count, float time_elapsed)
{
	// Batching is builtin in 2.0..
	draw_diff_tex_diff_sprites(gc, sprite_count, time_elapsed);
}