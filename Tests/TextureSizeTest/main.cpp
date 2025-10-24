#include <ClanLib/application.h>
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <GL/gl.h>
#include <GL/glu.h>

// #include "C:\Program Files\Rational\Quantify\pure.h"

/////////////////////////////////////////////////////////////////////////////
// Application class

class Application : public CL_ClanApplication
{
public:
	int main(int argc, char** argv);

	void test_artifact(CL_DisplayWindow &window);
	void test_texturesize(CL_DisplayWindow &window);
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
		CL_SetupGL::init();

		CL_DisplayWindow window("ClanLib Texture Size test program", 1024, 768, false, true);

		test_artifact(window);
				
		CL_SetupGL::deinit();
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

void Application::test_artifact(CL_DisplayWindow &window)
{
/*	CL_Surface ImageTGA("images/ship.tga");
	CL_Surface ImagePNG("images/ship.png");

	ImageTGA.set_alignment(origin_center);
	ImagePNG.set_alignment(origin_center);

	CL_SpriteDescription desc;
	desc.add_frame(new CL_PNGProvider("images/ship.png"), true);
	CL_Sprite sprite(desc);

	while(!CL_Keyboard::get_keycode(CL_KEY_ESCAPE))
	{
		CL_Display::clear();

		if(CL_Keyboard::get_keycode(CL_KEY_SPACE))
		{
			ImageTGA.rotate(0.01f);
			ImagePNG.rotate(0.01f);
			sprite.rotate(0.01f);
		}

		ImageTGA.draw(100, 100);
		ImagePNG.draw(200, 100);
		
		sprite.draw(100,200);

		CL_System::keep_alive();
		CL_Display::flip();
	}
*/}

void Application::test_texturesize(CL_DisplayWindow &window)
{
/*	CL_Size max_texture_size = window.get_gc()->get_max_surface_size();
	std::cout << "Max texture size: " << max_texture_size.width << "x" << max_texture_size.height << std::endl;
	
	std::cout << "2045x2045: ";	
	CL_SpriteDescription desc1;
	desc1.add_frame(new CL_JPEGProvider("images/2045x2045.jpg"), true);
	CL_Sprite sprite1(desc1);
	std::cout << "is ok" << std::endl;

	std::cout << "2046x2046: ";	
	CL_SpriteDescription desc2;
	desc2.add_frame(new CL_JPEGProvider("images/2046x2046.jpg"), true); 
	CL_Sprite sprite2(desc2);
	std::cout << "is ok" << std::endl;

	std::cout << "2047x2047: ";
	CL_SpriteDescription desc3;
	desc3.add_frame(new CL_JPEGProvider("images/2047x2047.jpg"), true);
	CL_Sprite sprite3(desc3);
	std::cout << "is ok" << std::endl;

	std::cout << "2048x2048: ";
	CL_SpriteDescription desc4;
	desc4.add_frame(new CL_JPEGProvider("images/2048x2048.jpg"), true);
	CL_Sprite sprite4(desc4);
	std::cout << "is ok" << std::endl;

	std::cout << "2049x2049: ";	
	CL_SpriteDescription desc5;
	desc5.add_frame(new CL_JPEGProvider("images/2049x2049.jpg"), true); 
	CL_Sprite sprite5(desc5);
	std::cout << "is ok" << std::endl;
	
	while(!CL_Keyboard::get_keycode(CL_KEY_ESCAPE))
	{
		CL_Display::clear();
		CL_System::keep_alive();
		CL_Display::flip();
	}
*/
}
