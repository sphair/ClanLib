/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
	description.set_title("NightVision Shader");
	description.set_size(clan::Size(1024, 768), true);

	window = clan::DisplayWindow(description);
	canvas = clan::Canvas(window);
 
	sc.connect(window.get_keyboard().sig_key_up(), clan::bind_member(this, &App::on_input_up));
	sc.connect(window.sig_window_close(), clan::bind_member(this, &App::window_close));

	// Create offscreen texture
	texture_offscreen = clan::Texture2D(canvas, canvas.get_width(), canvas.get_height());
	texture_offscreen.set_min_filter(clan::filter_nearest);
	texture_offscreen.set_mag_filter(clan::filter_nearest);

	texture_mask = clan::Texture2D(canvas, canvas.get_width(), canvas.get_height());
	texture_mask.set_min_filter(clan::filter_nearest);
	texture_mask.set_mag_filter(clan::filter_nearest);

	// Create offscreen framebuffer
	framebuffer_offscreen = clan::FrameBuffer(canvas);
	framebuffer_offscreen.attach_color(0, texture_offscreen);
	canvas_offscreen = clan::Canvas(canvas, framebuffer_offscreen);

	framebuffer_mask = clan::FrameBuffer(canvas);
	framebuffer_mask.attach_color(0, texture_mask);
	canvas_mask = clan::Canvas(canvas, framebuffer_mask);

	background = clan::Image(canvas, "../PostProcessing/Resources/background.png");
	ball = clan::Image(canvas, "../PostProcessing/Resources/ball.png");
	ball.set_alignment(clan::origin_center);
	noise_texture = clan::Texture2D(canvas, "Resources/noise_texture_0001.png");
	noise_texture.set_wrap_mode(clan::wrap_repeat, clan::wrap_repeat);

	// Load and link shaders
	shader = clan::ProgramObject::load(canvas, "Resources/vertex_shader.glsl", "Resources/fragment_shader.glsl");
	shader.bind_attribute_location(0, "Position");
	shader.bind_attribute_location(1, "TexCoord0");
	shader.bind_frag_data_location(0, "cl_FragColor");
	if (!shader.link())
		throw clan::Exception("Unable to link shader program: Error:" + shader.get_info_log());
	shader.set_uniform1i("sceneBuffer", 0);
	shader.set_uniform1i("noiseTex", 1);
	shader.set_uniform1i("maskTex", 2);

	float amount = 0.0f;
	float timer = 0.0f;

	float scale = 1.0f;

	font = clan::Font("tahoma", 32);

	// Shader based on: http://www.geeks3d.com/20091009/shader-library-night-vision-post-processing-filter-glsl/

	gpu_positions = clan::VertexArrayVector<clan::Vec2f>(canvas, 6);
	gpu_tex1_coords = clan::VertexArrayVector<clan::Vec2f>(canvas, 6);
	gpu_uniforms = clan::UniformVector<ProgramUniforms>(canvas, 1);
	gpu_primitives_array = clan::PrimitivesArray(canvas);
	gpu_primitives_array.set_attributes(0, gpu_positions);
	gpu_primitives_array.set_attributes(1, gpu_tex1_coords);

	uniforms.elapsedTime = 0.0f; // seconds
	uniforms.luminanceThreshold = 0.2f;
	uniforms.colorAmplification = 4.0f;
	uniforms.effectCoverage = 0.65f;

	// Render the mask
	canvas_mask.clear();
	for (float offset=0.0f; offset<=1.0f; offset+=0.01f)
	{
		canvas_mask.fill_circle(canvas.get_width() / 2.0f,  canvas.get_height() / 2.0f, 400.0f - offset * 64.0f, clan::Colorf(offset, offset, offset, 1.0f));
	}
	canvas_mask.flush();

	startTime = clan::System::get_time();

}

bool App::update()
{
	clan::InputDevice keyboard = window.get_keyboard();
	timer = (clan::System::get_time() - startTime) / 1000.0f;

	uniforms.elapsedTime = timer;

	// Render standard image to offscreen buffer
	background.draw(canvas_offscreen, 0, 0);
	ball.draw(canvas_offscreen, canvas.get_width() / 2 + 200 * sinf(timer / 2.0f), canvas.get_height() / 2 + 200 * cosf(timer / 2.0f));
	canvas_offscreen.flush();

	render_night_vision(canvas, texture_offscreen, texture_mask, noise_texture, shader);

	const int gap = 32;
	font.draw_text(canvas, 10, 64+gap*0, std::string("luminanceThreshold : " + clan::StringHelp::float_to_text(uniforms.luminanceThreshold) + " (Press Q,W)" ));
	font.draw_text(canvas, 10, 64+gap*1, std::string("colorAmplification : " + clan::StringHelp::float_to_text(uniforms.colorAmplification) + " (Press A,S)" ));
	font.draw_text(canvas, 10, 64+gap*2, std::string("effectCoverage : " + clan::StringHelp::float_to_text(uniforms.effectCoverage) + " (Press Z,X)" ));

	window.flip();

	return !quit;
}


void App::window_close()
{
	quit = true;
}

void App::render_night_vision(clan::Canvas &canvas, clan::Texture2D &source_texture, clan::Texture2D &mask_texture, clan::Texture2D &noise_texture, clan::ProgramObject &program_object)
{
	canvas.flush();
	clan::GraphicContext gc = canvas.get_gc();

	gc.set_texture(0, source_texture);
	gc.set_texture(1, noise_texture);
	gc.set_texture(2, mask_texture);
	gc.set_program_object(program_object);

	uniforms.cl_ModelViewProjectionMatrix = canvas.get_projection() * canvas.get_transform();
	gpu_uniforms.upload_data(gc, &uniforms, 1);
	gc.set_uniform_buffer(0, gpu_uniforms);

	draw_texture(gc, clan::Rectf(0,0,canvas.get_width(),canvas.get_height()), clan::Rectf(0.0f, 0.0f, 1.0f, 1.0f));

	gc.reset_program_object();
	gc.reset_texture(2);
	gc.reset_texture(1);
	gc.reset_texture(0);
}

void App::on_input_up(const clan::InputEvent &key)
{
	if(key.id == clan::keycode_escape)
	{
		quit = true;
	}

	if (key.id == clan::keycode_q)
	{
		uniforms.luminanceThreshold -= 0.02f;
	}
	else if (key.id == clan::keycode_w)
	{
		uniforms.luminanceThreshold += 0.02f;
	}
	else if (key.id == clan::keycode_a)
	{
		uniforms.colorAmplification -= 0.5f;
	}
	else if (key.id == clan::keycode_s)
	{
		uniforms.colorAmplification += 0.5f;
	}
	else if (key.id == clan::keycode_z)
	{
		uniforms.effectCoverage -= 0.2f;
	}
	else if (key.id == clan::keycode_x)
	{
		uniforms.effectCoverage += 0.2f;
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

	gc.draw_primitives(clan::type_triangles, 6, gpu_primitives_array);
}



