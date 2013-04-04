
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
		struct Uniforms
		{
			Vec3f resolution;
			float time;
		};

int Program::main(const std::vector<std::string> &args)
{
	SetupCore setup_core;
	SetupDisplay setup_display;
	SetupGL setup_gl;

	try
	{
		DisplayWindow window("Hello ShaderEffect", 1024, 1024, false, true);
		Slot slot = window.sig_window_close().connect(&Program::exit_func);

		GraphicContext gc = window.get_gc();


		Uniforms uniforms;
		uniforms.time = 0.0f;
		uniforms.resolution = Vec3f(1024.0f, 1024.0f, 0.0f);
		UniformVector<Uniforms> uniformVector(gc, &uniforms, 1);

		ShaderEffectDescription effect_description;
		effect_description.set_vertex_shader(File::read_text("Resources/vertex_shader.glsl"));
		effect_description.set_fragment_shader(File::read_text("Resources/fragment_shader.glsl"));
		effect_description.set_attribute_screen_quad("PositionInProjection");
		effect_description.set_frag_data_to_back_buffer("FragColor");
		effect_description.set_uniform_block("Uniforms", uniformVector);
		ShaderEffect effect(gc, effect_description);

		unsigned int start_time = System::get_time();
		while (!exit)
		{
			uniforms.time = (System::get_time() - start_time) / 1000.0f;
			uniformVector.upload_data(gc, &uniforms, 1);

			effect.draw(gc);
			window.flip(0);
			KeepAlive::process();
		}
	}
	catch (Exception &e)
	{
		ConsoleWindow console_window("Debug");
		Console::write_line("Unhandled exception: %1", e.get_message_and_stack_trace());
		Console::wait_for_key();
		return -1;
	}

	return 0;
}
