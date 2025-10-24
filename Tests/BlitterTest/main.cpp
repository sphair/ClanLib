#include <ClanLib/application.h>
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/dx7.h>

/////////////////////////////////////////////////////////////////////////////
// Application class

class Application : public CL_ClanApplication
{
public:
	int main(int argc, char** argv);

	void blitter_test(CL_DisplayWindow &window);
	void on_input_down(const CL_InputEvent &key);

	bool quit;
} app;

/////////////////////////////////////////////////////////////////////////////
// Application main entry point:

int Application::main(int argc, char** argv)
{
	CL_ConsoleWindow debug_console("Debug console");
	debug_console.redirect_stdio();
	try
	{
		CL_SetupCore::init();
		CL_SetupDisplay::init();
		CL_SetupDX7::init();

		CL_DisplayWindow window("ClanLib clanDisplay 2.0 test program", 800, 600);

		CL_Slot slot_input_down = CL_Keyboard::sig_key_down().connect(
			this, &Application::on_input_down);

		quit = false;

		blitter_test(window);
		
		CL_SetupDX7::deinit();
		CL_SetupDisplay::deinit();
		CL_SetupCore::deinit();
	}
	catch (CL_Error error)
	{
		std::cout << "Uncaught error: " << error.message.c_str() << std::endl;
		debug_console.wait_for_key();
	}

	return 0;
}

void Application::on_input_down(const CL_InputEvent &key)
{
	std::cout << "Key down: " << key.device.get_key_name(key.id).c_str() << " (" << key.id << ")" << std::endl;
	if (key.id == CL_KEY_ESCAPE) quit = true;
}

void Application::blitter_test(CL_DisplayWindow &window)
{
	static int col = 0;
	static int col_delta = 1;

	std::list<CL_Surface> images;
	std::list<std::string> text;
	
	images.push_back(CL_Surface(new CL_PCXProvider("images/test8.pcx"), true));	text.push_back("test8.pcx");
	images.push_back(CL_Surface(new CL_PNGProvider("images/test8.png"), true));	text.push_back("test8.png");
	images.push_back(CL_Surface(new CL_TargaProvider("images/test8.tga"), true)); text.push_back("test8.tga");
	images.push_back(CL_Surface(new CL_PNGProvider("images/test8_alpha.png"), true)); text.push_back("test8_alpha.png");
	images.push_back(CL_Surface(new CL_TargaProvider("images/test16.tga"), true)); text.push_back("test16.tga");
	images.push_back(CL_Surface(new CL_JPEGProvider("images/test24.jpg"), true)); text.push_back("test24.jpg");
	images.push_back(CL_Surface(new CL_PCXProvider("images/test24.pcx"), true)); text.push_back("test24.pcx");
	images.push_back(CL_Surface(new CL_PNGProvider("images/test24.png"), true)); text.push_back("test24.png");
	images.push_back(CL_Surface(new CL_TargaProvider("images/test24.tga"), true)); text.push_back("images/test24.tga");
	images.push_back(CL_Surface(new CL_PNGProvider("images/test32.png"), true)); text.push_back("images/test32.png");
	images.push_back(CL_Surface(new CL_TargaProvider("images/test32.tga"), true)); text.push_back("images/test32.tga");
	images.push_back(CL_Surface(new CL_PNGProvider("images/test32_alpha.png"), true)); text.push_back("images/test32_alpha.png");
	images.push_back(CL_Surface(new CL_TargaProvider("images/test32_alpha.tga"), true)); text.push_back("images/test32_alpha.tga");
	
	CL_GraphicContext *gc = window.get_gc();

	CL_Font sysfont("Comic Sans MS", 15);
	sysfont.set_alignment(origin_center);

	while (!quit)
	{
		// Clear screen
		CL_Display::clear(CL_Color(col, col, 0));

		// Fade background between black and white
		col += col_delta;
		if(col > 255 || col < 0)
		{
			col_delta = -col_delta;
			col += col_delta;
		}

		gc->draw_line(200, 0, 200, 600, CL_Color(100,100,100));
		gc->draw_line(600, 0, 600, 600, CL_Color(100,100,100));

		gc->draw_line(0, 150, 800, 150, CL_Color(100,100,100));
		gc->draw_line(0, 450, 800, 450, CL_Color(100,100,100));

		gc->draw_line(400, 0, 400, 600, CL_Color(255,255,255));
		gc->draw_line(0, 300, 800, 300, CL_Color(255,255,255));

		int x = 0;
		int y = 0;
		std::list<CL_Surface>::iterator i1;
		std::list<std::string>::iterator i2;
		for(i1 = images.begin(), i2 = text.begin(); i1 != images.end(); ++i1, ++i2)
		{
			(*i1).set_scale(0.37f, 0.37f);
			(*i1).draw(x + 5, y + 5);
			sysfont.draw(x + 100, y + 140, (*i2));
			
			x += 200;
			if(x == 800)
			{
				x = 0;
				y += 150;
			}

			break; // only draw first image for now (needed for dx7 testing)
		}
	
		window.flip();
		CL_System::keep_alive(10);
	}

	quit = false;
}
