
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

	DisplayWindow window("Hello ShaderEffect", 800, 600, false, true);
	Slot slot = window.sig_window_close().connect(&Program::exit_func);

	GraphicContext gc = window.get_gc();
	InputDevice mouse = window.get_ic().get_mouse();
	Texture2D texture(gc, "../../../Examples/GUI/CommonCode/Resources/tux.png");

	struct Uniforms
	{
		Vec3f resolution;
		float time;
		Vec4f mouse;
	};

	Uniforms uniforms;
	uniforms.time = 0.0f;
	uniforms.resolution = Vec3f(800, 600, 0);
	UniformVector<Uniforms> uniformVector(gc, &uniforms, 1);

	ShaderEffectDescription effect_description;
	effect_description.set_vertex_shader(File::read_text("Resources/vertex_shader.glsl"));
	effect_description.set_fragment_shader(File::read_text("Resources/fragment_shader.glsl"));
	effect_description.set_attribute_screen_quad("PositionInProjection");
	effect_description.set_frag_data_to_back_buffer("FragColor");
	effect_description.set_uniform_block("Uniforms", uniformVector);
	effect_description.set_texture("Tux", texture);
	ShaderEffect effect(gc, effect_description);

	while (!exit)
	{
		uniforms.time = System::get_time() / 1000.0f;
		uniforms.mouse = Vec4f(mouse.get_x() / 800.0f, mouse.get_y() / 600.0f, 0, 0);
		uniformVector.upload_data(gc, &uniforms, 1);

		effect.draw(gc);
		window.flip(0);
		KeepAlive::process();
	}

	return 0;
}
