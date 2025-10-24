#include <ClanLib/application.h>
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

#include <fstream>
#include "time.h"

/////////////////////////////////////////////////////////////////////////////
// Application class

class Application : public CL_ClanApplication
{
public:
	int main(int argc, char** argv);

	void init_log(int loops);
	void log(const std::string &channel, const std::string &message);
	
	int small_gradient_nooverdraw(CL_DisplayWindow &window, int loops);
	int small_gradient_overdraw(CL_DisplayWindow &window, int loops);
	int big_gradient_noalpha(CL_DisplayWindow &window, int loops);
	int big_gradient_alpha(CL_DisplayWindow &window, int loops);
	int lines_and_rects(CL_DisplayWindow &window, int loops);
	int surface_large(CL_DisplayWindow &window, int loops);
	int surface_small(CL_DisplayWindow &window, int loops);
	int surface_small_and_large(CL_DisplayWindow &window, int loops);
	int sprite(CL_DisplayWindow &window, int loops);
	int sprite_multiple(CL_DisplayWindow &window, int loops);
	int font(CL_DisplayWindow &window, int loops);
	int complex1(CL_DisplayWindow &window, int loops);
	int pixelbuffer_tga_load(CL_DisplayWindow &window, int loops);
	int pixelbuffer_png_load(CL_DisplayWindow &window, int loops);
	int pixelbuffer_jpg_load(CL_DisplayWindow &window, int loops);
	int surface_tga_load(CL_DisplayWindow &window, int loops);
	int surface_png_load(CL_DisplayWindow &window, int loops);
	int surface_jpg_load(CL_DisplayWindow &window, int loops);
	int sprite_load(CL_DisplayWindow &window, int loops);
	int font_load(CL_DisplayWindow &window, int loops);
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

		int loops = 500;

		CL_DisplayWindow window("ClanLib Benchmark", 1024, 768, false, true);

		init_log(loops);
		
		std::map<std::string, int> results[2];
		std::map<std::string, int>::iterator it;

		results[0]["1-01a. Small gradient - no overdraw"] = small_gradient_nooverdraw(window, loops);
		results[0]["1-01b. Small gradient - overdraw"] = small_gradient_overdraw(window, loops);
		results[0]["1-02a. Big gradient - no alpha"] = big_gradient_noalpha(window, loops);
		results[0]["1-02b. Big gradient - alpha"] = big_gradient_alpha(window, loops);
		results[0]["1-03a. Lines and rects"] = lines_and_rects(window, loops);
		results[0]["1-04a. Surface small"] = surface_small(window, loops);
		results[0]["1-04b. Surface large"] = surface_large(window, loops);
		results[0]["1-04c. Surface small & large"] = surface_small_and_large(window, loops);
		results[0]["1-05a. Single sprite"] = sprite(window, loops);
		results[0]["1-05b. Multiple sprites"] = sprite_multiple(window, loops);
		results[0]["1-06a. Font"] = font(window, loops);
		results[0]["1-07a. Lines, Surfaces, Sprites and Fonts"] = complex1(window, loops);

		int total1 = 0;
		for(it = results[0].begin(); it != results[0].end(); ++it)
		{
			log((*it).first, CL_String::from_int((*it).second));
			total1 += (*it).second;
		}
		log("SCORE DRAWING", CL_String::from_int(total1));

		results[1]["2-01a. Pixelbuffer tga loading"] = pixelbuffer_tga_load(window, loops);
		results[1]["2-01b. Pixelbuffer png loading"] = pixelbuffer_png_load(window, loops);
		results[1]["2-01c. Pixelbuffer jpg loading"] = pixelbuffer_jpg_load(window, loops);
		results[1]["2-02a. Surface tga creation"] = surface_tga_load(window, loops);
		results[1]["2-02b. Surface png creation"] = surface_png_load(window, loops);
		results[1]["2-02c. Surface jpg creation"] = surface_jpg_load(window, loops);
		results[1]["2-03a. Sprite creation"] = sprite_load(window, loops);
		results[1]["2-04a. Font creation"] = font_load(window, loops);

		int total2 = 0;
		for(it = results[1].begin(); it != results[1].end(); ++it)
		{
			log((*it).first, CL_String::from_int((*it).second));
			total2 += (*it).second;
		}
		log("SCORE LOADING", CL_String::from_int(total2));

		log("SCORE TOTAL", CL_String::from_int(total1 + total2));

		CL_SetupGL::deinit();
		CL_SetupDisplay::deinit();
		CL_SetupCore::deinit();
	}
	catch (CL_Error error)
	{
		std::cout << "Uncaught error: " << error.message.c_str() << std::endl;
	}

	debug_console.display_close_message();

	return 0;
}

void Application::init_log(int loops)
{
	std::ofstream file;
	file.open("benchmark_results.txt", std::ios::out | std::ios::app);

	time_t t = time(NULL);
	file << std::endl << "Benchmark v0.4 - " << loops << " loops" << std::endl;
	file << "ClanLib v" << CL_VERSION_STRING << std::endl;
	file << ctime(&t);
}

void Application::log(const std::string &channel, const std::string &message)
{
	std::ofstream file;
	file.open("benchmark_results.txt", std::ios::out | std::ios::app);

	std::string log = CL_String::format("%1: %2", channel, message);

	file << log << std::endl;
	std::cout << log << std::endl;

	file.close();
}

int Application::small_gradient_nooverdraw(CL_DisplayWindow &window, int loops)
{
	CL_GraphicContext *gc = window.get_gc();
	int start_time = CL_System::get_time();

	int frames = 0;
	while (frames < loops)
	{
		gc->clear();

		for(int y=0; y<70; ++y)
			for(int x=0; x<100; ++x)
				gc->fill_rect(
					CL_Rect(x * 10, y * 10, x * 10 + 10, y * 10 + 10),
					CL_Gradient(CL_Color::white, CL_Color::green, CL_Color::red, CL_Color::blue));

		window.flip();
		CL_System::keep_alive();

		frames++;
	}

	return int(frames / ((CL_System::get_time() - start_time) / 1000.0f));
}

int Application::small_gradient_overdraw(CL_DisplayWindow &window, int loops)
{
	CL_GraphicContext *gc = window.get_gc();
	int start_time = CL_System::get_time();

	int frames = 0;
	while (frames < loops)
	{
		gc->clear();

		for(int i=0; i<300; ++i)
			for(int y=0; y<7; ++y)
				for(int x=0; x<10; ++x)
					gc->fill_rect(
						CL_Rect(x * 10, y * 10, x * 10 + 10, y * 10 + 10),
						CL_Gradient(CL_Color::white, CL_Color::green, CL_Color::red, CL_Color::blue));

		window.flip();
		CL_System::keep_alive();

		frames++;
	}

	return int(frames / ((CL_System::get_time() - start_time) / 1000.0f));
}

int Application::big_gradient_noalpha(CL_DisplayWindow &window, int loops)
{
	CL_GraphicContext *gc = window.get_gc();
	int start_time = CL_System::get_time();

	int frames = 0;
	while (frames < loops)
	{
		gc->clear();

		for(int i=0; i<10; i++)
			for(int y=0; y<7; ++y)
				for(int x=0; x<10; ++x)
					gc->fill_rect(
						CL_Rect(x * 100, y * 100, x * 100 + 100, y * 100 + 100),
						CL_Gradient(CL_Color::white, CL_Color::green, CL_Color::red, CL_Color::blue));

		window.flip();
		CL_System::keep_alive();

		frames++;
	}

	return int(frames / ((CL_System::get_time() - start_time) / 1000.0f));
}

int Application::big_gradient_alpha(CL_DisplayWindow &window, int loops)
{
	CL_GraphicContext *gc = window.get_gc();
	int start_time = CL_System::get_time();

	int frames = 0;
	while (frames < loops)
	{
		gc->clear();

		for(int i=0; i<10; i++)
			for(int y=0; y<7; ++y)
				for(int x=0; x<10; ++x)
					gc->fill_rect(
						CL_Rect(x * 100, y * 100, x * 100 + 100, y * 100 + 100),
						CL_Gradient(CL_Color(255,255,255,100), CL_Color(0,255,0,100), CL_Color(255,0,0,100), CL_Color(0,0,255,100)));

		window.flip();
		CL_System::keep_alive();

		frames++;
	}

	return int(frames / ((CL_System::get_time() - start_time) / 1000.0f));
}

int Application::lines_and_rects(CL_DisplayWindow &window, int loops)
{
	CL_GraphicContext *gc = window.get_gc();
	int start_time = CL_System::get_time();

	int frames = 0;
	while (frames < loops)
	{
		gc->clear();

		for(int y=0; y<700; y += 30)
			for(int x=0; x<1000; x += 30)
			{
				gc->draw_line(0, 0, x, y, CL_Color(255, 255, 255));
				gc->draw_rect(CL_Rect(0, 0, x, y), CL_Color(255, 0, 255));
			}

		window.flip();
		CL_System::keep_alive();

		frames++;
	}

	return int(frames / ((CL_System::get_time() - start_time) / 1000.0f));
}

int Application::surface_large(CL_DisplayWindow &window, int loops)
{
	CL_GraphicContext *gc = window.get_gc();

	CL_Surface surface("images/image1.tga");
	surface.set_alpha(0.5f);

	int start_time = CL_System::get_time();

	int frames = 0;
	while (frames < loops)
	{
		gc->clear();

		for(int y=0; y<700; y += 60)
			for(int x=0; x<1000; x += 60)
				surface.draw(x, y, gc);

		window.flip();
		CL_System::keep_alive();

		frames++;
	}

	return int(frames / ((CL_System::get_time() - start_time) / 1000.0f));
}

int Application::surface_small(CL_DisplayWindow &window, int loops)
{
	CL_GraphicContext *gc = window.get_gc();

	CL_Surface surface("images/image2.tga");

	int start_time = CL_System::get_time();

	int frames = 0;
	while (frames < loops)
	{
		gc->clear();

		for(int y=0; y<700; y += 20)
			for(int x=0; x<1000; x += 20)
				surface.draw(x, y, gc);

		window.flip();
		CL_System::keep_alive();

		frames++;
	}

	return int(frames / ((CL_System::get_time() - start_time) / 1000.0f));
}

int Application::surface_small_and_large(CL_DisplayWindow &window, int loops)
{
	CL_GraphicContext *gc = window.get_gc();

	CL_Surface surface1("images/image1.tga");
	CL_Surface surface2("images/image2.tga");

	int start_time = CL_System::get_time();

	int frames = 0;
	while (frames < loops)
	{
		gc->clear();

		for(int y=0; y<700; y += 80)
			for(int x=0; x<1000; x += 80)
			{
				surface1.draw(x, y, gc);
				surface2.draw(x, y, gc);
			}

		window.flip();
		CL_System::keep_alive();

		frames++;
	}

	return int(frames / ((CL_System::get_time() - start_time) / 1000.0f));
}

int Application::font(CL_DisplayWindow &window, int loops)
{
	CL_GraphicContext *gc = window.get_gc();

	CL_SpriteDescription desc;
	desc.add_paletteclipped_frames(
		new CL_PCXProvider("images/clanfont.pcx"), 129, 209, true);

	CL_Sprite sprite(desc);
	CL_Font font(
		sprite,
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
		10);
	font.set_alignment(origin_center);
	font.set_color(0.0f, 1.0f, 0.0f, 1.0f);

	int start_time = CL_System::get_time();

	int frames = 0;
	while (frames < loops)
	{
		gc->clear();

		for(int y=0; y<700; y += 40)
			for(int x=0; x<1000; x += 40)
				font.draw(x, y, "Bitmap font", gc);

		window.flip();
		CL_System::keep_alive();

		frames++;
	}

	return int(frames / ((CL_System::get_time() - start_time) / 1000.0f));
}

int Application::sprite(CL_DisplayWindow &window, int loops)
{
	CL_GraphicContext *gc = window.get_gc();

	CL_SpriteDescription desc;
	desc.add_frame(new CL_TargaProvider("images/manwalk_0001.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0002.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0003.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0004.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0005.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0006.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0007.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0008.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0009.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0010.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0011.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0012.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0013.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0014.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0015.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0016.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0017.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0018.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0019.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0020.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0021.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0022.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0023.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0024.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0025.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0026.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0027.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0028.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0029.tga"), true);

	CL_Sprite sprite(desc);
	sprite.set_alignment(origin_center);

	int start_time = CL_System::get_time();

	int frames = 0;
	while (frames < loops)
	{
		gc->clear();

		for(int y=0; y<700; y += 16)
			for(int x=0; x<1000; x += 16)
				sprite.draw(x, y, gc);

		sprite.update();

		window.flip();
		CL_System::keep_alive();

		frames++;
	}

	return int(frames / ((CL_System::get_time() - start_time) / 1000.0f));
}

int Application::sprite_multiple(CL_DisplayWindow &window, int loops)
{
	CL_GraphicContext *gc = window.get_gc();

	CL_SpriteDescription desc1;
	desc1.add_frame(new CL_TargaProvider("images/manwalk_0001.tga"), true);
	desc1.add_frame(new CL_TargaProvider("images/manwalk_0002.tga"), true);
	desc1.add_frame(new CL_TargaProvider("images/manwalk_0003.tga"), true);
	desc1.add_frame(new CL_TargaProvider("images/manwalk_0004.tga"), true);
	desc1.add_frame(new CL_TargaProvider("images/manwalk_0005.tga"), true);
	desc1.add_frame(new CL_TargaProvider("images/manwalk_0006.tga"), true);
	desc1.add_frame(new CL_TargaProvider("images/manwalk_0007.tga"), true);
	desc1.add_frame(new CL_TargaProvider("images/manwalk_0008.tga"), true);
	desc1.add_frame(new CL_TargaProvider("images/manwalk_0009.tga"), true);
	desc1.add_frame(new CL_TargaProvider("images/manwalk_0010.tga"), true);

	CL_SpriteDescription desc2;
	desc2.add_frame(new CL_TargaProvider("images/manwalk_0011.tga"), true);
	desc2.add_frame(new CL_TargaProvider("images/manwalk_0012.tga"), true);
	desc2.add_frame(new CL_TargaProvider("images/manwalk_0013.tga"), true);
	desc2.add_frame(new CL_TargaProvider("images/manwalk_0014.tga"), true);
	desc2.add_frame(new CL_TargaProvider("images/manwalk_0015.tga"), true);
	desc2.add_frame(new CL_TargaProvider("images/manwalk_0016.tga"), true);
	desc2.add_frame(new CL_TargaProvider("images/manwalk_0017.tga"), true);
	desc2.add_frame(new CL_TargaProvider("images/manwalk_0018.tga"), true);
	desc2.add_frame(new CL_TargaProvider("images/manwalk_0019.tga"), true);
	desc2.add_frame(new CL_TargaProvider("images/manwalk_0020.tga"), true);

	CL_SpriteDescription desc3;
	desc3.add_frame(new CL_TargaProvider("images/manwalk_0021.tga"), true);
	desc3.add_frame(new CL_TargaProvider("images/manwalk_0022.tga"), true);
	desc3.add_frame(new CL_TargaProvider("images/manwalk_0023.tga"), true);
	desc3.add_frame(new CL_TargaProvider("images/manwalk_0024.tga"), true);
	desc3.add_frame(new CL_TargaProvider("images/manwalk_0025.tga"), true);
	desc3.add_frame(new CL_TargaProvider("images/manwalk_0026.tga"), true);
	desc3.add_frame(new CL_TargaProvider("images/manwalk_0027.tga"), true);
	desc3.add_frame(new CL_TargaProvider("images/manwalk_0028.tga"), true);
	desc3.add_frame(new CL_TargaProvider("images/manwalk_0029.tga"), true);

	CL_Sprite sprite1(desc1);
	CL_Sprite sprite2(desc2);
	CL_Sprite sprite3(desc3);

	int start_time = CL_System::get_time();

	int frames = 0;
	while (frames < loops)
	{
		gc->clear();

		for(int y=0; y<700; y += 32)
		{
			for(int x=0; x<1000; x += 32)
			{
				sprite1.draw(x, y, gc);
				sprite2.draw(x + 10, y + 10, gc);
				sprite3.draw(x + 20, y + 20, gc);
			}
		}

		sprite1.update();
		sprite2.update();
		sprite3.update();

		window.flip();
		CL_System::keep_alive();

		frames++;
	}

	return int(frames / ((CL_System::get_time() - start_time) / 1000.0f));
}

int Application::complex1(CL_DisplayWindow &window, int loops)
{
	CL_GraphicContext *gc = window.get_gc();

	CL_SpriteDescription desc;
	desc.add_frame(new CL_TargaProvider("images/manwalk_0001.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0002.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0003.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0004.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0005.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0006.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0007.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0008.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0009.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0010.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0011.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0012.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0013.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0014.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0015.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0016.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0017.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0018.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0019.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0020.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0021.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0022.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0023.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0024.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0025.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0026.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0027.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0028.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0029.tga"), true);
	CL_Sprite sprite(desc);

	CL_SpriteDescription fontdesc;
	fontdesc.add_paletteclipped_frames(
		new CL_PCXProvider("images/clanfont.pcx"), 129, 209, true);
	CL_Sprite fontsprite(fontdesc);
	CL_Font font(
		fontsprite,
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
		10);

	CL_Surface surface1("images/image1.tga");
	CL_Surface surface2("images/image2.tga");

	int start_time = CL_System::get_time();

	int frames = 0;
	while (frames < loops)
	{
		gc->clear();

		for(int y=0; y<700; y += 150)
		{
			for(int x=0; x<1000; x += 150)
			{
				gc->draw_line(0, 0, x, y, CL_Color(255, 255, 255));
				gc->draw_rect(CL_Rect(0, 0, x, y), CL_Color(255, 0, 255, 100));
				gc->fill_rect(CL_Rect(0, 0, x, y), CL_Color(255, 0, 255, 100));
				gc->fill_rect(
					CL_Rect(x, y, x + 10, y + 10),
					CL_Gradient(CL_Color::white, CL_Color::green, CL_Color::red, CL_Color::blue));
				sprite.draw(x, y, gc);
				surface1.draw(x, y, gc);
				surface2.draw(x, y, gc);
				font.draw(x, y, "Complex", gc);
			}
		}

		sprite.update();

		window.flip();
		CL_System::keep_alive();

		frames++;
	}

	return int(frames / ((CL_System::get_time() - start_time) / 1000.0f));
}

int Application::pixelbuffer_jpg_load(CL_DisplayWindow &window, int loops)
{
	CL_GraphicContext *gc = window.get_gc();

	int start_time = CL_System::get_time();

	int frames = 0;
	while (frames < loops)
	{
		CL_PixelBuffer *b = CL_ProviderFactory::load("images/image1.jpg");
		b->lock();
		b->unlock();

		delete b;
		frames++;
	}

	return int(frames / ((CL_System::get_time() - start_time) / 1000.0f));
}

int Application::pixelbuffer_tga_load(CL_DisplayWindow &window, int loops)
{
	CL_GraphicContext *gc = window.get_gc();

	int start_time = CL_System::get_time();

	int frames = 0;
	while (frames < loops / 5)
	{
		CL_PixelBuffer *b = CL_ProviderFactory::load("images/image1.tga");
		b->lock();
		b->unlock();

		delete b;
		frames++;
	}

	return int(frames / ((CL_System::get_time() - start_time) / 1000.0f));
}

int Application::pixelbuffer_png_load(CL_DisplayWindow &window, int loops)
{
	CL_GraphicContext *gc = window.get_gc();

	int start_time = CL_System::get_time();

	int frames = 0;
	while (frames < loops / 5)
	{
		CL_PixelBuffer *b = CL_ProviderFactory::load("images/image1.png");
		b->lock();
		b->unlock();

		delete b;
		frames++;
	}

	return int(frames / ((CL_System::get_time() - start_time) / 1000.0f));
}

int Application::surface_tga_load(CL_DisplayWindow &window, int loops)
{
	CL_GraphicContext *gc = window.get_gc();

	CL_PixelBuffer *b = CL_ProviderFactory::load("images/image1.tga");

	int start_time = CL_System::get_time();

	int frames = 0;
	while (frames < loops / 5)
	{
		CL_Surface *surface = new CL_Surface(b);
		delete surface;
		frames++;
	}

	int end_time = CL_System::get_time();

	delete b;

	return int(frames / ((end_time - start_time) / 1000.0f));
}

int Application::surface_png_load(CL_DisplayWindow &window, int loops)
{
	CL_GraphicContext *gc = window.get_gc();

	CL_PixelBuffer *b = CL_ProviderFactory::load("images/image1.png");

	int start_time = CL_System::get_time();

	int frames = 0;
	while (frames < loops / 5)
	{
		CL_Surface *surface = new CL_Surface(b);
		delete surface;
		frames++;
	}

	int end_time = CL_System::get_time();

	delete b;

	return int(frames / ((end_time - start_time) / 1000.0f));
}

int Application::surface_jpg_load(CL_DisplayWindow &window, int loops)
{
	CL_GraphicContext *gc = window.get_gc();

	CL_PixelBuffer *b = CL_ProviderFactory::load("images/image1.jpg");

	int start_time = CL_System::get_time();

	int frames = 0;
	while (frames < loops / 5)
	{
		CL_Surface *surface = new CL_Surface(b);
		delete surface;
		frames++;
	}

	int end_time = CL_System::get_time();

	delete b;

	return int(frames / ((end_time - start_time) / 1000.0f));
}

int Application::sprite_load(CL_DisplayWindow &window, int loops)
{
	CL_GraphicContext *gc = window.get_gc();

	CL_SpriteDescription desc;
	desc.add_frame(new CL_TargaProvider("images/manwalk_0001.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0002.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0003.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0004.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0005.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0006.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0007.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0008.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0009.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0010.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0011.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0012.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0013.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0014.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0015.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0016.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0017.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0018.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0019.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0020.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0021.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0022.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0023.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0024.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0025.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0026.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0027.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0028.tga"), true);
	desc.add_frame(new CL_TargaProvider("images/manwalk_0029.tga"), true);

	int start_time = CL_System::get_time();

	int frames = 0;
	while (frames < loops / 5)
	{
		CL_Sprite *sprite = new CL_Sprite(desc);
		delete sprite;
		frames++;
	}

	return int(frames / ((CL_System::get_time() - start_time) / 1000.0f));
}

int Application::font_load(CL_DisplayWindow &window, int loops)
{
	CL_GraphicContext *gc = window.get_gc();

	CL_SpriteDescription fontdesc;
	fontdesc.add_paletteclipped_frames(
		new CL_PCXProvider("images/clanfont.pcx"), 129, 209, true);

	int start_time = CL_System::get_time();

	int frames = 0;
	while (frames < loops / 5)
	{
		CL_Sprite *fontsprite = new CL_Sprite(fontdesc);
		CL_Font *font = new CL_Font(
			*fontsprite,
			"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
			10);

		delete font;
		delete fontsprite;

		frames++;
	}

	return int(frames / ((CL_System::get_time() - start_time) / 1000.0f));
}
