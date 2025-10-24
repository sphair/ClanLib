
#include "precomp.h"
#include "program.h"

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
		CL_Console::write_line(e.get_message_and_stack_trace());
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
	CL_DisplayWindow window("ClanLib HSV Example", 1024, 768);
	CL_Slot slot = window.sig_window_close().connect(this, &Program::on_close);
	CL_GraphicContext gc = window.get_gc();
	CL_InputContext ic = window.get_ic();
	CL_ProgramObject program = create_shader_program(gc);
	CL_Texture texture = create_texture(gc);
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

		render_texture(gc, program, texture, hue_offset);
		window.flip();
		CL_KeepAlive::process(10);
	}

	return 0;
}

void Program::on_close()
{
	quit = true;
}

CL_ProgramObject Program::create_shader_program(CL_GraphicContext &gc)
{
	CL_ProgramObject program = CL_ProgramObject::load(gc, "vertex.glsl", "fragment.glsl");
	program.bind_attribute_location(0, "Position");
	program.bind_attribute_location(1, "HueOffset0");
	program.bind_attribute_location(2, "TexCoord0");
	if (!program.link())
		throw CL_Exception("Unable to link program");
	return program;
}

CL_Texture Program::create_texture(CL_GraphicContext &gc)
{
	return CL_Texture(gc, "../DiceWar/Resources/lobby_background2.png");
}

void Program::render_texture(CL_GraphicContext &gc, CL_ProgramObject &program, CL_Texture &texture, float hue_offset)
{
	CL_Rectf rect(0.0f, 0.0f, (float)gc.get_width(), (float)gc.get_height());
	CL_Rectf texture_unit1_coords(0.0f, 0.0f, 1.0f, 1.0f);

	CL_Vec2f positions[6] =
	{
		CL_Vec2f(rect.left, rect.top),
		CL_Vec2f(rect.right, rect.top),
		CL_Vec2f(rect.left, rect.bottom),
		CL_Vec2f(rect.right, rect.top),
		CL_Vec2f(rect.left, rect.bottom),
		CL_Vec2f(rect.right, rect.bottom)
	};

	CL_Vec2f tex1_coords[6] =
	{
		CL_Vec2f(texture_unit1_coords.left, texture_unit1_coords.top),
		CL_Vec2f(texture_unit1_coords.right, texture_unit1_coords.top),
		CL_Vec2f(texture_unit1_coords.left, texture_unit1_coords.bottom),
		CL_Vec2f(texture_unit1_coords.right, texture_unit1_coords.top),
		CL_Vec2f(texture_unit1_coords.left, texture_unit1_coords.bottom),
		CL_Vec2f(texture_unit1_coords.right, texture_unit1_coords.bottom)
	};

	CL_PrimitivesArray primarray(gc);
	primarray.set_attributes(0, positions);
	primarray.set_attribute(1, CL_Vec1f(hue_offset));
	primarray.set_attributes(2, tex1_coords);

	gc.set_program_object(program);
	gc.draw_primitives(cl_triangles, 6, primarray);
	gc.reset_program_object();
}
