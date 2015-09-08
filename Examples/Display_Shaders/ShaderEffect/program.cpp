
#include "precomp.h"
#include "program.h"
#include <ClanLib/application.h>

using namespace clan;

clan::ApplicationInstance<Program> clanapp;

Program::Program()
{
	// We support all display targets, in order listed here
	clan::OpenGLTarget::set_current();

	window = DisplayWindow("Hello ShaderEffect", 1024, 1024, false, true);
	sc.connect(window.sig_window_close(), [&](){exit = true; });

	gc = window.get_gc();

	uniforms.time = 0.0f;
	uniforms.resolution = Vec3f(800, 600, 0);
	uniforms.xparticle_count = particle_count;
	for(int i=0; i<particle_count; ++i) 
	{
		uniforms.positions[i] = Vec4f((float)(rand() % 300 + 200.0f), (float)(rand() % 300 + 200.0f), (float)(rand() % 15), 1.0f);
	}

	uniformVector = UniformVector<Uniforms>(gc, &uniforms, 1);

	ShaderEffectDescription effect_description;
	effect_description.set_glsl_version(150);
	effect_description.set_vertex_shader(File::read_text("Resources/vertex_shader.glsl"));
	effect_description.set_fragment_shader(File::read_text("Resources/fragment_shader.glsl"));
	effect_description.set_attribute_screen_quad("PositionInProjection");
	effect_description.set_frag_data_to_back_buffer("FragColor");
	effect_description.set_uniform_block("Uniforms", uniformVector);
	effect = ShaderEffect(gc, effect_description);

	start_time = System::get_time();
}

bool Program::update()
{
	InputDevice mouse = window.get_ic().get_mouse();

	float time = System::get_time() / 1000.0f;
	uniforms.time = time;
	uniforms.mouse = Vec4f(mouse.get_position().x / 800.0f, mouse.get_position().y / 600.0f, 0, 0);

	for(int i=0; i< uniforms.xparticle_count; ++i)
	{
		uniforms.positions[i].x += sinf(time + i * 2.0f) / 40.0f;
		uniforms.positions[i].y += cosf(time + i * 2.0f) / 40.0f;
	}
	uniformVector.upload_data(gc, &uniforms, 1);

	effect.draw(gc);
	window.flip(0);
	return !exit;
}



