
#include "precomp.h"
#include "program.h"
#include <ClanLib/application.h>

CL_ClanApplication clanapp(&Program::main);

int Program::main(const std::vector<CL_String> &args)
{
	CL_SetupCore setup_core;
	CL_SetupDisplay setup_display;
	CL_SetupSWRender setup_swrender;

	CL_DisplayWindow window("Hello", 370*4, 130*4, false, true);
	CL_InputContext ic = window.get_ic();
	CL_GraphicContext gc = window.get_gc();

	CL_Font font(gc, "Tahoma", -11);
	int frames = 0;
	unsigned int time = CL_System::get_time();
	float fps = 0.0f;

	CL_Vec4f vertices[3] =
	{
		CL_Vec4f(10.0f, 10.0f, 1.0f, 1.0f),
		CL_Vec4f(900.0f, 200.0f, 1.0f, 1.0f),
		CL_Vec4f(10.0f, 600.0f, 1.0f, 1.0f)
	};
	CL_Vec2f texcoords[3] =
	{
		CL_Vec2f(0.0f, 0.0f),
		CL_Vec2f(1.0f, 0.0f),
		CL_Vec2f(0.0f, 1.0f)
	};
	CL_PrimitivesArray primarray(gc);
	primarray.set_attributes(0, vertices);
	primarray.set_attribute(1, CL_Colorf::blue);
	primarray.set_attributes(2, texcoords);

	CL_Texture texture(gc, "../../../Examples/Display/Basic2D/Resources/logo.png");
	CL_Image image(gc, "../../../Examples/Display/Basic2D/Resources/logo.png");

	while (!ic.get_keyboard().get_keycode(CL_KEY_ESCAPE))
	{
//		gc.clear(CL_Colorf::white);
/*
		gc.set_program_object(cl_program_single_texture);
		gc.set_texture(0, texture);
		gc.draw_primitives(cl_triangles, 3, primarray);
		gc.reset_texture(0);
		gc.reset_program_object();
*/

		CL_Rectf rect(0.0f, 0.0f, (float)gc.get_width(), (float)gc.get_height());
		CL_Rectf rect2(0.0f, 0.0f, (float)gc.get_width()/4.0f, (float)gc.get_height()/4.0f);

		gc.set_texture(0, texture);
		for (int i = 0; i < 100*16; i++)
			CL_Draw::texture(gc, rect2, CL_Colorf::white);
		gc.reset_texture(0);

//		image.draw(gc, rect);
		font.draw_text(gc, 10, 30, cl_format("%1 fps", fps), CL_Colorf::black);
		frames++;
		window.flip();
		int delta = CL_System::get_time()-time;
		if (delta > 5000)
		{
			fps = frames/(delta/1000.0f);
			time = CL_System::get_time();
			frames = 0;
		}
		CL_KeepAlive::process();
	}

	return 0;
}
