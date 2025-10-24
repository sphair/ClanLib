#include <ClanLib/application.h>
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <GL/gl.h>
#include <GL/glu.h>

/////////////////////////////////////////////////////////////////////////////
// Application class

class Application : public CL_ClanApplication
{
public:
	int main(int argc, char** argv);

	void sprite_test_manual(CL_DisplayWindow &window);
	void sprite_test_resources(CL_DisplayWindow &window);
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

		CL_DisplayWindow window("ClanLib Sprite Test", 1024, 768, false, true);

//		sprite_test_manual(window);
		sprite_test_resources(window);
		
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

void Application::sprite_test_manual(CL_DisplayWindow &window)
{
/*	CL_SpriteDescription desc;
	
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
	
	CL_GraphicContext *gc = window.get_gc();

	float last_time = (float)CL_System::get_time();

	float colors[26][20][3];
	for(int y=0; y<19; y++)
	{
		for(int x=0; x<26; x++) 
		{
			colors[x][y][0] = (rand() % 100) / 100.0f;
			colors[x][y][1] = (rand() % 100) / 100.0f;
			colors[x][y][2] = (rand() % 100) / 100.0f;
		}
	}

	while (!CL_Keyboard::get_keycode(CL_KEY_ESCAPE))
	{
		// Calculate amount of time since last frame
		float delta_time = (CL_System::get_time() - last_time) / 1000.0f;
		last_time = (float)CL_System::get_time();

		// Rotate sprite based on time
		sprite.rotate(100.0f * delta_time);
		sprite.set_scale(1.0f, 1.0f);
		sprite.set_alignment(origin_top_left);
		
		// Update sprite animation based on time 
		sprite.update(delta_time);

		gc->clear();

		for(int y=0; y<19; y++)
		{
			for(int x=0; x<26; x++) 
			{
				sprite.set_color(colors[x][y][0], colors[x][y][1], colors[x][y][2]);
				sprite.draw(x * 40, y * 40, gc);
			}
		}

		sprite.set_color(1.0f, 1.0f, 1.0f);
		sprite.set_alignment(origin_center);
		sprite.set_scale(2.0f, 2.0f);
		sprite.draw(gc->get_width()/2-100, gc->get_height()/2, gc);
		sprite.draw(gc->get_width()/2+100, gc->get_height()/2, gc);

		window.flip();
		CL_System::keep_alive(10);
	}
*/}

void Application::sprite_test_resources(CL_DisplayWindow &window)
{
	CL_ResourceManager resources("resources.xml", false);
	CL_Font font("font_gray", &resources);

	CL_Sprite spr1("spr1", &resources);
	CL_Sprite spr2("spr2", &resources);
	CL_Sprite spr3("spr3", &resources);
	CL_Sprite spr4("spr4", &resources);
	CL_Sprite spr5("spr5", &resources);
	CL_Sprite spr6("spr6", &resources);
	CL_Sprite spr7("spr7", &resources);
	CL_Sprite spr8("spr8", &resources);
	CL_Sprite spr9("spr9", &resources);
	CL_Sprite spr10("spr10", &resources);
	CL_Sprite spr10b("spr10", &resources);
	CL_Sprite spr10c("spr10", &resources);
	CL_Sprite spr11("spr11", &resources);
	CL_Sprite spr12("spr12", &resources);
	CL_Sprite spr13("spr13", &resources);
	CL_Sprite spr14("spr14", &resources);
	CL_Sprite spr15("spr15", &resources);
	CL_Sprite spr16("spr16", &resources);
	CL_Sprite spr17("spr17", &resources);
	CL_Sprite spr18("spr18", &resources);
	CL_Sprite spr19("spr19", &resources);
	CL_Sprite spr20("spr20", &resources);
	CL_Sprite spr21("spr21", &resources);
	CL_Sprite spr22("spr22", &resources);
	CL_Sprite spr23("spr23", &resources);
	CL_Sprite spr24("spr24", &resources);
	CL_Sprite spr25("spr25", &resources);
	CL_Sprite spr26("spr26", &resources);
	CL_Sprite spr27("spr27", &resources);

	CL_GraphicContext *gc = window.get_gc();

	float last_time = (float)CL_System::get_time();
	while (!CL_Keyboard::get_keycode(CL_KEY_ESCAPE))
	{
		CL_Size s = spr1.get_frame_size(0);

		// Calculate amount of time since last frame
		float delta_time = (CL_System::get_time() - last_time) / 1000.0f;
		last_time = (float)CL_System::get_time();

		gc->clear(CL_Color(255, 0, 0));
		
		font.draw(CL_Rect(160*0, 0, 160*1-10, 1000), "1: Non-animated, single frame");
		spr1.draw(10+160*0, 20);

		font.draw(CL_Rect(160*1, 0, 160*2-10, 1000), "2: 180 base-angle");
		spr2.draw(10+160*1, 20);
		font.draw(CL_Rect(160*2, 0, 160*3-10, 1000), "3: 180 base-angle, 2x1 scale");
		spr3.draw(10+160*2, 20);
		font.draw(CL_Rect(160*3, 0, 160*4-10, 1000), "4: 1x0.5 scale");
		spr4.draw(10+160*3, 20);
		font.draw(CL_Rect(160*4, 0, 160*5-10, 1000), "5: 2x2 scale");
		spr5.draw(10+160*4, 20);
		
		font.draw(CL_Rect(160*1, 130, 160*2-10, 1000), "6: Colour red");
		spr6.draw(10+160*1, 150);
		font.draw(CL_Rect(160*2, 130, 160*3-10, 1000), "7: Colour greenish");
		spr7.draw(10+160*2, 150);
		font.draw(CL_Rect(160*3, 130, 160*4-10, 1000), "8: Half-transparent");
		spr8.draw(10+160*3, 150);
		font.draw(CL_Rect(160*4, 130, 160*5-10, 1000), "9: Half-transparent, blue");
		spr9.draw(10+160*4, 150);

		font.draw(CL_Rect(160*0, 130*2, 160*1-10, 1000), "10: Simple animation");
		spr10.draw(10+160*0, 130*2+20);
		spr10.update();
		font.draw(CL_Rect(160*1, 130*2, 160*2-10, 1000), "11: Fast animation");
		spr11.draw(10+160*1, 130*2+20);
		spr11.update();
		font.draw(CL_Rect(160*2, 130*2, 160*3-10, 1000), "12: Ping-pong animation");
		spr12.draw(10+160*2, 130*2+20);
		spr12.update();
		font.draw(CL_Rect(160*3, 130*2, 160*4-10, 1000), "13: Backward animation");
		spr13.draw(10+160*3, 130*2+20);
		spr13.update();
		font.draw(CL_Rect(160*4, 130*2, 160*5-10, 1000), "10b: Rotating animation");
		spr10b.draw(10+160*4, 130*2+20);
		spr10b.rotate(1.0f);
		spr10b.update();
		font.draw(CL_Rect(160*5, 130*2, 160*6-10, 1000), "10c: Rotating, 2x scaled animation");
		spr10c.set_scale(2.0f, 2.0f);
		spr10c.draw(10+160*5, 130*2+20);
		spr10c.rotate(1.0f);
		spr10c.update();
		font.draw(CL_Rect(160*0, 130*3, 160*1-10, 1000), "14: No-loop animation");
		spr14.draw(10+160*0, 130*3+20);
		spr14.update();
		font.draw(CL_Rect(160*1, 130*3, 160*2-10, 1000), "15: No-loop animation, last_frame on_finish");
		spr15.draw(10+160*1, 130*3+20);
		spr15.update();
		font.draw(CL_Rect(160*2, 130*3, 160*3-10, 1000), "16: No-loop animation, first_frame on_finish");
		spr16.draw(10+160*2, 130*3+20);
		spr16.update();

		font.draw(CL_Rect(160*3, 130*3, 160*4-10, 1000), "21: Frame-modifications, first 15 frames fast");
		spr21.draw(10+160*3, 130*3+20);
		spr21.update();
		font.draw(CL_Rect(160*4, 130*3, 160*5-10, 1000), "22: Frame-modifications, first 15 frames with position-offset (not supported yet)");
		spr22.draw(10+160*4, 130*3+20);
		spr22.update();

		font.draw(CL_Rect(160*0, 130*4, 160*1-10, 1000), "17: Center translation");
		spr17.draw(10+160*0, 130*4+20);
		CL_Display::draw_rect(CL_Rect(9+160*0, 130*4+19, 11+160*0, 130*4+21), CL_Color::white);
		spr17.update();
		font.draw(CL_Rect(160*1, 130*4, 160*2-10, 1000), "18: Top-left translation");
		spr18.draw(10+160*1, 130*4+20);
		CL_Display::draw_rect(CL_Rect(9+160*1, 130*4+19, 11+160*1, 130*4+21), CL_Color::white);
		spr18.update();
		font.draw(CL_Rect(160*2, 130*4, 160*3-10, 1000), "19: Bottom-right translation");
		spr19.draw(10+160*2, 130*4+20);
		CL_Display::draw_rect(CL_Rect(9+160*2, 130*4+19, 11+160*2, 130*4+21), CL_Color::white);
		spr19.update();
		font.draw(CL_Rect(160*3, 130*4, 160*4-10, 1000), "20: Center +10,+10 translation, animated");
		spr20.draw(10+160*3, 130*4+20);
		CL_Display::draw_rect(CL_Rect(9+160*3, 130*4+19, 11+160*3, 130*4+21), CL_Color::white);
		spr20.update();

		font.draw(CL_Rect(160*1, 130*5, 160*2-10, 1000), "23: Top-left rotation");
		spr23.draw(10+160*1, 130*5+20);
		CL_Display::draw_rect(CL_Rect(9+160*1, 130*5+19, 11+160*1, 130*5+21), CL_Color::white);
		spr23.rotate(1.0f);
		
		font.draw(CL_Rect(160*2, 130*5, 160*3-10, 1000), "24: Bottom-right rotation, animated");
		spr24.draw(10+160*2, 130*5+20);
		CL_Display::draw_rect(CL_Rect(9+160*2+s.width, 130*5+19+s.height, 11+160*2+s.width, 130*5+21+s.height), CL_Color::white);
		spr24.rotate(1.0f);
		spr24.update();

		font.draw(CL_Rect(160*3, 130*5, 160*4-10, 1000), "25: Top-left +10,+10 rotation, 2x scaled, animated");
		spr25.draw(10+160*3, 130*5+20);
		CL_Display::draw_rect(CL_Rect(9+160*3+10, 130*5+19+10, 11+160*3+10, 130*5+21+10), CL_Color::white);
		spr25.rotate(1.0f);
		spr25.update();

		font.draw(CL_Rect(160*4, 130*5, 160*5-10, 1000), "25: Animated sprite using description");
		spr26.draw(10+160*4, 130*5+20);
		spr26.update();
		font.draw(CL_Rect(160*5, 130*5, 160*6-10, 1000), "25: Animated sprite using description plus extra frames");
		spr27.draw(10+160*5, 130*5+20);
		spr27.update();

		window.flip();
		CL_System::keep_alive(10);
	}
}
