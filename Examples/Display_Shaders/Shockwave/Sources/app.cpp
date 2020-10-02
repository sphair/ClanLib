/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
*/

#include "precomp.h"
#include "app.h"

clan::ApplicationInstance<App> clanapp;

App::App()
{
	clan::OpenGLTarget::set_current();

	clan::DisplayWindowDescription description;
	description.set_title("Shockwave Shader");
	description.set_size(clan::Size(1024, 768), true);

	window = clan::DisplayWindow(description);
	canvas = clan::Canvas(window);
 
	sc.connect(window.get_keyboard().sig_key_up(), clan::bind_member(this, &App::on_input_up));
	sc.connect(window.sig_window_close(), clan::bind_member(this, &App::window_close));

	// Create offscreen texture
	texture_offscreen = clan::Texture2D(canvas, canvas.get_width(), canvas.get_height());
	texture_offscreen.set_min_filter(clan::TextureFilter::nearest);
	texture_offscreen.set_mag_filter(clan::TextureFilter::nearest);

	// Create offscreen framebuffer
	clan::FrameBuffer framebuffer_offscreen(canvas);
	framebuffer_offscreen.attach_color(0, texture_offscreen);
	canvas_offscreen = clan::Canvas(canvas, framebuffer_offscreen);

	background = clan::Image(canvas, "../PostProcessing/Resources/background.png");
	ball = clan::Image(canvas, "../PostProcessing/Resources/ball.png");
	ball.set_alignment(clan::Origin::center);

	// Load and link shaders
	shader = clan::ProgramObject::load(canvas, "Resources/vertex_shader.glsl", "Resources/fragment_shader.glsl");
	shader.bind_attribute_location(0, "Position");
	shader.bind_attribute_location(1, "TexCoord0");
	shader.bind_frag_data_location(0, "cl_FragColor");

	if (!shader.link())
		throw clan::Exception("Unable to link shader program: Error:" + shader.get_info_log());
	shader.set_uniform1i("Texture0", 0);

	font = clan::Font("tahoma", 32);

	// Shader idea and code from http://www.geeks3d.com/20091116/shader-library-2d-shockwave-post-processing-filter-glsl/
	// Shader enhanced for clanlib

	gpu_positions = clan::VertexArrayVector<clan::Vec2f>(canvas, 6);
	gpu_tex1_coords = clan::VertexArrayVector<clan::Vec2f>(canvas, 6);
	gpu_uniforms = clan::UniformVector<ProgramUniforms>(canvas, 1);
	gpu_primitives_array = clan::PrimitivesArray(canvas);
	gpu_primitives_array.set_attributes(0, gpu_positions);
	gpu_primitives_array.set_attributes(1, gpu_tex1_coords);

	uniforms.shockParams = clan::Vec3f(10.0f, 0.8f, 0.1f);

	startTime = clan::System::get_time();
	uniforms.glow = 0.1f;

}

bool App::update()
{
	clan::InputDevice keyboard = window.get_keyboard();

	timer = (clan::System::get_time() - startTime) / 1000.0f;

	uniforms.time = (timer - shockwave_start_time) / shockwave_rate;

	// Render standard image to offscreen buffer
	background.draw(canvas_offscreen, 0, 0);
	float xpos = canvas.get_width() / 2 + 200 * sinf(timer / 2.0f);
	float ypos = canvas.get_height() / 2 + 200 * cosf(timer / 2.0f);
	ball.draw(canvas_offscreen, xpos, ypos);
	canvas_offscreen.flush();

	uniforms.center.x = xpos / ((float) canvas.get_width());
	uniforms.center.y = ypos / ((float) canvas.get_height());

	render_shockwave(canvas, texture_offscreen, shader);

	const int gap = 32;
	font.draw_text(canvas, 10, 64 + gap*0, "Press 'M' to emit a shockwave");
	font.draw_text(canvas, 10, 64 + gap*1, "base: " + clan::StringHelp::float_to_text(uniforms.shockParams.x) + " (Press Q,W)");
	font.draw_text(canvas, 10, 64 + gap*2, "exponent: " + clan::StringHelp::float_to_text(uniforms.shockParams.y) + " (Press A,S)");
	font.draw_text(canvas, 10, 64 + gap*3, "distance: " + clan::StringHelp::float_to_text(uniforms.shockParams.z) + " (Press Z,X)");
	font.draw_text(canvas, 10, 64 + gap*4, "rate: " + clan::StringHelp::float_to_text(shockwave_rate) + " (Press E,R)");
	font.draw_text(canvas, 10, 64 + gap*5, "glow: " + clan::StringHelp::float_to_text(uniforms.glow) + " (Press D,F)");

	window.flip(1);

	return !quit;
}


void App::window_close()
{
	quit = true;
}

void App::render_shockwave(clan::Canvas &canvas, clan::Texture2D &source_texture, clan::ProgramObject &program_object)
{
	canvas.flush();
	clan::GraphicContext gc = canvas.get_gc();

	gc.set_texture(0, source_texture);
	gc.set_program_object(program_object);

	uniforms.cl_ModelViewProjectionMatrix = canvas.get_projection() * canvas.get_transform();
	gpu_uniforms.upload_data(gc, &uniforms, 1);
	gc.set_uniform_buffer(0, gpu_uniforms);

	draw_texture(gc, clan::Rectf(0,0,canvas.get_width(),canvas.get_height()), clan::Rectf(0.0f, 0.0f, 1.0f, 1.0f));

	gc.reset_program_object();
	gc.reset_texture(0);
}

void App::on_input_up(const clan::InputEvent &key)
{
	if(key.id == clan::keycode_escape)
	{
		quit = true;
	}

	if (key.id == clan::keycode_m)
	{
		shockwave_start_time = timer;
	}

	if (key.id == clan::keycode_q)
	{
		uniforms.shockParams.x -= 1.0f;
	}
	else if (key.id == clan::keycode_w)
	{
		uniforms.shockParams.x += 1.0f;
	}
	else if (key.id == clan::keycode_a)
	{
		uniforms.shockParams.y -= 0.1f;
	}
	else if (key.id == clan::keycode_s)
	{
		uniforms.shockParams.y += 0.1f;
	}
	else if (key.id == clan::keycode_z)
	{
		uniforms.shockParams.z -= 0.01f;
	}
	else if (key.id == clan::keycode_x)
	{
		uniforms.shockParams.z += 0.01f;
	}
	else if (key.id == clan::keycode_e)
	{
		shockwave_rate -= 0.2f;
		if (shockwave_rate < 0.2f)
			shockwave_rate = 0.2f;
	}
	else if (key.id == clan::keycode_r)
	{
		shockwave_rate += 0.2f;
	}
	else if (key.id == clan::keycode_d)
	{
		uniforms.glow -= 0.02f;
	}
	else if (key.id == clan::keycode_f)
	{
		uniforms.glow += 0.02f;
	}
}

void App::draw_texture(clan::GraphicContext &gc, const clan::Rectf &rect, const clan::Rectf &texture_unit1_coords)
{
	clan::Vec2f positions[6] =
	{
		clan::Vec2f(rect.left, rect.top),
		clan::Vec2f(rect.right, rect.top),
		clan::Vec2f(rect.left, rect.bottom),
		clan::Vec2f(rect.right, rect.top),
		clan::Vec2f(rect.left, rect.bottom),
		clan::Vec2f(rect.right, rect.bottom)
	};

	clan::Vec2f tex1_coords[6] =
	{
		clan::Vec2f(texture_unit1_coords.left, texture_unit1_coords.top),
		clan::Vec2f(texture_unit1_coords.right, texture_unit1_coords.top),
		clan::Vec2f(texture_unit1_coords.left, texture_unit1_coords.bottom),
		clan::Vec2f(texture_unit1_coords.right, texture_unit1_coords.top),
		clan::Vec2f(texture_unit1_coords.left, texture_unit1_coords.bottom),
		clan::Vec2f(texture_unit1_coords.right, texture_unit1_coords.bottom)
	};

	gpu_positions.upload_data(gc, 0, positions, 6);
	gpu_tex1_coords.upload_data(gc, 0, tex1_coords, 6);

	gc.draw_primitives(clan::PrimitivesType::triangles, 6, gpu_primitives_array);
}

