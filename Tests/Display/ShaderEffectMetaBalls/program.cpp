
#include "precomp.h"
#include "program.h"
#include <ClanLib/application.h>

using namespace clan;

Application clanapp(&Program::main);

bool Program::exit = false;

void Program::exit_func()
{
	exit = true;
}

int Program::main(const std::vector<std::string> &args)
{
	SetupCore setup_core;
	SetupDisplay setup_display;
	SetupGL setup_gl;

	ConsoleWindow console_window("Debug");

	try
	{
		DisplayWindow window("Hello ShaderEffect", 800, 600, false, true);
		Slot slot = window.sig_window_close().connect(&Program::exit_func);

		GraphicContext gc = window.get_gc();
		InputDevice mouse = window.get_ic().get_mouse();

		struct Uniforms
		{
			Vec3f resolution;
			float time;
			Vec4f mouse;

			Vec4f positions[3];
			int particle_count;
		};

		Uniforms uniforms;
		uniforms.time = 0.0f;
		uniforms.resolution = Vec3f(800, 600, 0);
		uniforms.particle_count = 3;
		uniforms.positions[0] = Vec4f(0.0f, 0.0f, 5.0f, 1.0f);
		uniforms.positions[1] = Vec4f(0.0f, 0.0f, 20.0f, 1.0f);
		uniforms.positions[2] = Vec4f(0.0f, 0.0f, 15.0f, 1.0f);

		UniformVector<Uniforms> uniformVector(gc, &uniforms, 1);

		ShaderEffectDescription effect_description;
		effect_description.set_vertex_shader(File::read_text("Resources/vertex_shader.glsl"));
		effect_description.set_fragment_shader(File::read_text("Resources/fragment_shader.glsl"));
		effect_description.set_attribute_screen_quad("PositionInProjection");
		effect_description.set_frag_data_to_back_buffer("FragColor");
		effect_description.set_uniform_block("Uniforms", uniformVector);

		ShaderEffect effect(gc, effect_description);

		while (!exit)
		{
			float time = System::get_time() / 1000.0f;
			uniforms.time = time;
			uniforms.mouse = Vec4f(mouse.get_x() / 800.0f, mouse.get_y() / 600.0f, 0, 0);
			uniforms.positions[0].x = 200.0f + sinf(time + 0.5f) * 100;
			uniforms.positions[0].y = 200.0f + cosf(time + 0.5f) * 100;
			uniforms.positions[1].x = 250.0f + sinf(time) * 100;
			uniforms.positions[1].y = 250.0f + cosf(time) * 100;
			uniforms.positions[2].x = 300.0f + sinf(time + 1.0f) * 100;
			uniforms.positions[2].y = 300.0f + cosf(time + 1.0f) * 100;

			uniformVector.upload_data(gc, &uniforms, 1);

			effect.draw(gc);
			window.flip(0);
			KeepAlive::process();
		}
	}
	catch (Exception &e)
	{
		Console::write_line("Unhandled exception: %1", e.get_message_and_stack_trace());
		Console::wait_for_key();
		return -1;
	}

	return 0;
}
