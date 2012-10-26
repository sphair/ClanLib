/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

int App::start(const std::vector<std::string> &args)
{
	OpenGLWindowDescription description;
	description.set_title("NightVision Shader");
	description.set_size(Size(1024, 768), true);

	DisplayWindow window(description);
	InputDevice keyboard = window.get_ic().get_keyboard();
	GraphicContext gc = window.get_gc();

	Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

	Slot slot_window_close = window.sig_window_close().connect(this, &App::window_close);

	// Create offscreen texture
	Texture texture_offscreen(gc, gc.get_width(), gc.get_height());
	texture_offscreen.set_min_filter(cl_filter_nearest);
	texture_offscreen.set_mag_filter(cl_filter_nearest);

	Texture texture_mask(gc, gc.get_width(), gc.get_height());
	texture_mask.set_min_filter(cl_filter_nearest);
	texture_mask.set_mag_filter(cl_filter_nearest);

	// Create offscreen framebuffer
	FrameBuffer framebuffer_offscreen(gc);
	framebuffer_offscreen.attach_color_buffer(0, texture_offscreen);

	FrameBuffer framebuffer_mask(gc);
	framebuffer_mask.attach_color_buffer(0, texture_mask);

	Image background(gc, "../PostProcessing/Resources/background.png");
	Image ball(gc, "../PostProcessing/Resources/ball.png");
	ball.set_alignment(origin_center);
	Texture noise_texture(gc, "Resources/noise_texture_0001.png");
	noise_texture.set_wrap_mode(cl_wrap_repeat, cl_wrap_repeat);

	// Load and link shaders
	ProgramObject shader = ProgramObject::load(gc, "Resources/vertex_shader.glsl", "Resources/fragment_shader.glsl");
	shader.bind_attribute_location(0, "Position");
	shader.bind_attribute_location(2, "TexCoord0");
	if (!shader.link())
		throw Exception("Unable to link shader program: Error:" + shader.get_info_log());

	quit = false;

	float amount = 0.0f;
	float timer = 0.0f;

	float scale = 1.0f;

	Font font(gc, "tahoma", 32);

	// Shader based on: http://www.geeks3d.com/20091009/shader-library-night-vision-post-processing-filter-glsl/

	elapsedTime = 0.0f; // seconds
	luminanceThreshold = 0.2f;
	colorAmplification = 4.0f;
	effectCoverage = 0.65f;

	// Render the mask
	gc.set_frame_buffer(framebuffer_mask);
	gc.clear();
	for (float offset=0.0f; offset<=1.0f; offset+=0.01f)
	{
		Draw::circle(gc, gc.get_width() / 2.0f,  gc.get_height() / 2.0f, 400.0f - offset * 64.0f, Colorf(offset, offset, offset, 1.0f));
	}
	gc.reset_frame_buffer();

	unsigned int startTime = System::get_time();

	while (!quit)
	{
		timer = (System::get_time() - startTime) / 1000.0f;

		elapsedTime = timer;

		// Render standard image to offscreen buffer
		gc.set_frame_buffer(framebuffer_offscreen);
		background.draw(gc, 0, 0);
		ball.draw(gc, gc.get_width() / 2 + 200 * sinf(timer / 2.0f), gc.get_height() / 2 + 200 * cosf(timer / 2.0f));
		gc.reset_frame_buffer();

		render_night_vision(gc, texture_offscreen, texture_mask, noise_texture, shader);

		const int gap = 32;
		font.draw_text(gc, 10, 64+gap*0, std::string("luminanceThreshold : " + StringHelp::float_to_text(luminanceThreshold) + " (Press Q,W)" ));
		font.draw_text(gc, 10, 64+gap*1, std::string("colorAmplification : " + StringHelp::float_to_text(colorAmplification) + " (Press A,S)" ));
		font.draw_text(gc, 10, 64+gap*2, std::string("effectCoverage : " + StringHelp::float_to_text(effectCoverage) + " (Press Z,X)" ));

		window.flip();

		System::sleep(10);

		KeepAlive::process();
	}

	return 0;
}


void App::window_close()
{
	quit = true;
}

void App::render_night_vision(GraphicContext &gc, Texture &source_texture, Texture &mask_texture, Texture &noise_texture, ProgramObject &program_object)
{
	gc.set_texture(0, source_texture);
	gc.set_texture(1, noise_texture);
	gc.set_texture(2, mask_texture);
	
	gc.set_program_object(program_object, cl_program_matrix_modelview_projection);
	program_object.set_uniform1i("sceneBuffer", 0);
	program_object.set_uniform1i("noiseTex", 1);
	program_object.set_uniform1i("maskTex", 2);

	program_object.set_uniform1f("elapsedTime", elapsedTime);
	program_object.set_uniform1f("luminanceThreshold", luminanceThreshold);
	program_object.set_uniform1f("colorAmplification", colorAmplification);
	program_object.set_uniform1f("effectCoverage", effectCoverage);

	draw_texture(gc, Rectf(0.0f,0.0f,gc.get_width(),gc.get_height()), Colorf::white, Rectf(0.0f, 0.0f, 1.0f, 1.0f));

	gc.reset_program_object();
	gc.reset_texture(2);
	gc.reset_texture(1);
	gc.reset_texture(0);
}

void App::on_input_up(const InputEvent &key)
{
	if(key.id == KEY_ESCAPE)
	{
		quit = true;
	}

	if (key.id == KEY_Q)
	{
		luminanceThreshold -= 0.02f;
	}
	else if (key.id == KEY_W)
	{
		luminanceThreshold += 0.02f;
	}
	else if (key.id == KEY_A)
	{
		colorAmplification -= 0.5f;
	}
	else if (key.id == KEY_S)
	{
		colorAmplification += 0.5f;
	}
	else if (key.id == KEY_Z)
	{
		effectCoverage -= 0.2f;
	}
	else if (key.id == KEY_X)
	{
		effectCoverage += 0.2f;
	}

}

void App::draw_texture(GraphicContext &gc, const Rectf &rect, const Colorf &color, const Rectf &texture_unit1_coords)
{
	Vec2f positions[6] =
	{
		Vec2f(rect.left, rect.top),
		Vec2f(rect.right, rect.top),
		Vec2f(rect.left, rect.bottom),
		Vec2f(rect.right, rect.top),
		Vec2f(rect.left, rect.bottom),
		Vec2f(rect.right, rect.bottom)
	};

	Vec2f tex1_coords[6] =
	{
		Vec2f(texture_unit1_coords.left, texture_unit1_coords.top),
		Vec2f(texture_unit1_coords.right, texture_unit1_coords.top),
		Vec2f(texture_unit1_coords.left, texture_unit1_coords.bottom),
		Vec2f(texture_unit1_coords.right, texture_unit1_coords.top),
		Vec2f(texture_unit1_coords.left, texture_unit1_coords.bottom),
		Vec2f(texture_unit1_coords.right, texture_unit1_coords.bottom)
	};

	PrimitivesArray prim_array(gc);
	prim_array.set_attributes(0, positions);
	prim_array.set_attribute(1, color);
	prim_array.set_attributes(2, tex1_coords);
	gc.draw_primitives(cl_triangles, 6, prim_array);
}

