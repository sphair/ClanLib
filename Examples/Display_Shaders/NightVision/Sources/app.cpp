/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

int App::start(const std::vector<CL_String> &args)
{
	CL_OpenGLWindowDescription description;
	description.set_title("NightVision Shader");
	description.set_size(CL_Size(1024, 768), true);

	CL_DisplayWindow window(description);
	CL_InputDevice keyboard = window.get_ic().get_keyboard();
	CL_GraphicContext gc = window.get_gc();

	CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

	CL_Slot slot_window_close = window.sig_window_close().connect(this, &App::window_close);

	// Create offscreen texture
	CL_Texture texture_offscreen(gc, gc.get_width(), gc.get_height());
	texture_offscreen.set_min_filter(cl_filter_nearest);
	texture_offscreen.set_mag_filter(cl_filter_nearest);

	CL_Texture texture_mask(gc, gc.get_width(), gc.get_height());
	texture_mask.set_min_filter(cl_filter_nearest);
	texture_mask.set_mag_filter(cl_filter_nearest);

	// Create offscreen framebuffer
	CL_FrameBuffer framebuffer_offscreen(gc);
	framebuffer_offscreen.attach_color_buffer(0, texture_offscreen);

	CL_FrameBuffer framebuffer_mask(gc);
	framebuffer_mask.attach_color_buffer(0, texture_mask);

	CL_Image background(gc, "../PostProcessing/Resources/background.png");
	CL_Image ball(gc, "../PostProcessing/Resources/ball.png");
	ball.set_alignment(origin_center);
	CL_Texture noise_texture(gc, "Resources/noise_texture_0001.png");
	noise_texture.set_wrap_mode(cl_wrap_repeat, cl_wrap_repeat);

	// Load and link shaders
	CL_ProgramObject shader = CL_ProgramObject::load(gc, "Resources/vertex_shader.glsl", "Resources/fragment_shader.glsl");
	shader.bind_attribute_location(0, "Position");
	shader.bind_attribute_location(2, "TexCoord0");
	if (!shader.link())
		throw CL_Exception("Unable to link shader program: Error:" + shader.get_info_log());

	quit = false;

	float amount = 0.0f;
	float timer = 0.0f;

	float scale = 1.0f;

	CL_Font font(gc, "tahoma", 32);

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
		CL_Draw::circle(gc, gc.get_width() / 2.0f,  gc.get_height() / 2.0f, 400.0f - offset * 64.0f, CL_Colorf(offset, offset, offset, 1.0f));
	}
	gc.reset_frame_buffer();

	unsigned int startTime = CL_System::get_time();

	while (!quit)
	{
		timer = (CL_System::get_time() - startTime) / 1000.0f;

		elapsedTime = timer;

		// Render standard image to offscreen buffer
		gc.set_frame_buffer(framebuffer_offscreen);
		background.draw(gc, 0, 0);
		ball.draw(gc, gc.get_width() / 2 + 200 * sinf(timer / 2.0f), gc.get_height() / 2 + 200 * cosf(timer / 2.0f));
		gc.reset_frame_buffer();

		render_night_vision(gc, texture_offscreen, texture_mask, noise_texture, shader);

		const int gap = 32;
		font.draw_text(gc, 10, 64+gap*0, CL_String("luminanceThreshold : " + CL_StringHelp::float_to_text(luminanceThreshold) + " (Press Q,W)" ));
		font.draw_text(gc, 10, 64+gap*1, CL_String("colorAmplification : " + CL_StringHelp::float_to_text(colorAmplification) + " (Press A,S)" ));
		font.draw_text(gc, 10, 64+gap*2, CL_String("effectCoverage : " + CL_StringHelp::float_to_text(effectCoverage) + " (Press Z,X)" ));

		window.flip();

		CL_System::sleep(10);

		CL_KeepAlive::process();
	}

	return 0;
}


void App::window_close()
{
	quit = true;
}

void App::render_night_vision(CL_GraphicContext &gc, CL_Texture &source_texture, CL_Texture &mask_texture, CL_Texture &noise_texture, CL_ProgramObject &program_object)
{
	program_object.set_uniform1i("sceneBuffer", 0);
	program_object.set_uniform1i("noiseTex", 1);
	program_object.set_uniform1i("maskTex", 2);

	program_object.set_uniform1f("elapsedTime", elapsedTime);
	program_object.set_uniform1f("luminanceThreshold", luminanceThreshold);
	program_object.set_uniform1f("colorAmplification", colorAmplification);
	program_object.set_uniform1f("effectCoverage", effectCoverage);

	gc.set_texture(0, source_texture);
	gc.set_texture(1, noise_texture);
	gc.set_texture(2, mask_texture);
	
	gc.set_program_object(program_object, cl_program_matrix_modelview_projection);

	draw_texture(gc, CL_Rectf(0,0,gc.get_width(),gc.get_height()), CL_Colorf::white, CL_Rectf(0.0f, 0.0f, 1.0f, 1.0f));

	gc.reset_program_object();
	gc.reset_texture(2);
	gc.reset_texture(1);
	gc.reset_texture(0);
}

void App::on_input_up(const CL_InputEvent &key, const CL_InputState &state)
{
	if(key.id == CL_KEY_ESCAPE)
	{
		quit = true;
	}

	if (key.id == CL_KEY_Q)
	{
		luminanceThreshold -= 0.02f;
	}
	else if (key.id == CL_KEY_W)
	{
		luminanceThreshold += 0.02f;
	}
	else if (key.id == CL_KEY_A)
	{
		colorAmplification -= 0.5f;
	}
	else if (key.id == CL_KEY_S)
	{
		colorAmplification += 0.5f;
	}
	else if (key.id == CL_KEY_Z)
	{
		effectCoverage -= 0.2f;
	}
	else if (key.id == CL_KEY_X)
	{
		effectCoverage += 0.2f;
	}

}

void App::draw_texture(CL_GraphicContext &gc, const CL_Rectf &rect, const CL_Colorf &color, const CL_Rectf &texture_unit1_coords)
{
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

	CL_PrimitivesArray prim_array(gc);
	prim_array.set_attributes(0, positions);
	prim_array.set_attribute(1, color);
	prim_array.set_attributes(2, tex1_coords);
	gc.draw_primitives(cl_triangles, 6, prim_array);
}

