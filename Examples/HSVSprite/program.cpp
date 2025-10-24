
#include "precomp.h"
#include "program.h"
#include "hsv_sprite.h"
#include "hsv_sprite_batch.h"

CL_ClanApplication clanapp(&Program::main);

int Program::main(const std::vector<CL_String> &args)
{
	CL_SetupCore setup_core;
	CL_SetupDisplay setup_display;
	CL_SetupGL setup_gl;
	Program program;
	try
	{
		return program.run();
	}
	catch (CL_Exception &e)
	{
#ifdef WIN32
		MessageBox(0, e.get_message_and_stack_trace().c_str(), TEXT("Uncaught Exception"), MB_OK);
#else
		CL_Console::write_line(e.message);
#endif
		return 0;
	}
}

Program::Program()
: quit(false)
{
}

int Program::run()
{
	CL_DisplayWindow window("ClanLib HSV Sprite", 1024, 768);
	CL_Slot slot = window.sig_window_close().connect(this, &Program::on_close);
	CL_GraphicContext gc = window.get_gc();
	CL_InputContext ic = window.get_ic();

	CL_Font font(gc, "Tahoma", -11);

	HSVSpriteBatch sprite_batcher(gc);
	HSVSprite car1(gc, &sprite_batcher, "../SpritesRTS/Gfx/spaceshoot_body_moving1.png");
	HSVSprite car2(gc, &sprite_batcher, "Resources/ferrari_maranello.png");
	HSVSprite *cars[] = { &car1, &car2 };

	unsigned int last_fps_update = CL_System::get_time();
	int fps = 0;
	CL_String fps_text;

	float hue_offset = 0.0;
	while (!quit)
	{
		if (ic.get_keyboard().get_keycode(CL_KEY_LEFT))
			hue_offset += 0.005f;
		else if (ic.get_keyboard().get_keycode(CL_KEY_RIGHT))
			hue_offset -= 0.005f;
		if (hue_offset < -1.0f)
			hue_offset += 1.0f;
		if (hue_offset > 1.0f)
			hue_offset -= 1.0f;

		gc.clear(CL_Colorf::darkslategrey);
		float car_hue = hue_offset;
		for (int y = 0; y < 10; y++)
		{
			for (int x = 0; x < 7; x++)
			{
				cars[(x+y)%2]->draw(gc, 60+x*128, 60+y*64, car_hue);

				car_hue += 0.02f;
				if (car_hue < -1.0f)
					car_hue += 1.0f;
				if (car_hue > 1.0f)
					car_hue -= 1.0f;
			}
		}

		fps++;
		if (CL_System::get_time() - last_fps_update > 2000)
		{
			fps_text = cl_format("%1 fps", fps/2.0f);
			last_fps_update = CL_System::get_time();
			fps = 0;
		}

		CL_Size fps_size = font.get_text_size(gc, fps_text);
		font.draw_text(gc, gc.get_width()-10-fps_size.width, 16, fps_text);

		window.flip(0);
		CL_KeepAlive::process();
	}

	return 0;
}

void Program::on_close()
{
	quit = true;
}
