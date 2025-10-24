/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
	description.set_title("Shockwave Shader");
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

	// Create offscreen framebuffer
	CL_FrameBuffer framebuffer_offscreen(gc);
	framebuffer_offscreen.attach_color_buffer(0, texture_offscreen);

	CL_Image background(gc, "../PostProcessing/Resources/background.png");
	CL_Image ball(gc, "../PostProcessing/Resources/ball.png");
	ball.set_alignment(origin_center);

	// Load and link shaders
	CL_ProgramObject shader = CL_ProgramObject::load(gc, "Resources/vertex_shader.glsl", "Resources/fragment_shader.glsl");
	shader.bind_attribute_location(0, "Position");
	shader.bind_attribute_location(2, "TexCoord0");
	if (!shader.link())
		throw CL_Exception("Unable to link shader program: Error:" + shader.get_info_log());

	quit = false;

	float amount = 0.0f;
	timer = 0.0f;

	float scale = 1.0f;

	CL_Font font(gc, "tahoma", 32);

	// Shader idea and code from http://www.geeks3d.com/20091116/shader-library-2d-shockwave-post-processing-filter-glsl/
	// Shader enhanced for clanlib

	shockParams = CL_Vec3f(10.0f, 0.8f, 0.1f);

	unsigned int startTime = CL_System::get_time();
	shockwave_start_time = 0.0f;
	shockwave_rate = 1.0f;
	glow = 0.1f;

	while (!quit)
	{
		timer = (CL_System::get_time() - startTime) / 1000.0f;

		shockwave_time_elapsed = (timer - shockwave_start_time) / shockwave_rate;

		// Render standard image to offscreen buffer
		gc.set_frame_buffer(framebuffer_offscreen);
		background.draw(gc, 0, 0);
		float xpos = gc.get_width() / 2 + 200 * sinf(timer / 2.0f);
		float ypos = gc.get_height() / 2 + 200 * cosf(timer / 2.0f);
		ball.draw(gc, xpos, ypos);
		gc.reset_frame_buffer();

		center.x = xpos / ((float) gc.get_width());
		center.y = ypos / ((float) gc.get_height());

		render_shockwave(gc, texture_offscreen, shader);

		const int gap = 32;
		font.draw_text(gc, 10, 64 + gap*0, "Press 'M' to emit a shockwave");
		font.draw_text(gc, 10, 64 + gap*1, "base: " + CL_StringHelp::float_to_text(shockParams.x) + " (Press Q,W)");
		font.draw_text(gc, 10, 64 + gap*2, "exponent: " + CL_StringHelp::float_to_text(shockParams.y) + " (Press A,S)");
		font.draw_text(gc, 10, 64 + gap*3, "distance: " + CL_StringHelp::float_to_text(shockParams.z) + " (Press Z,X)");
		font.draw_text(gc, 10, 64 + gap*4, "rate: " + CL_StringHelp::float_to_text(shockwave_rate) + " (Press E,R)");
		font.draw_text(gc, 10, 64 + gap*5, "glow: " + CL_StringHelp::float_to_text(glow) + " (Press D,F)");

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

void App::render_shockwave(CL_GraphicContext &gc, CL_Texture &source_texture, CL_ProgramObject &program_object)
{
	program_object.set_uniform1i("sceneTex", 0);
	program_object.set_uniform2f("center", center);
	program_object.set_uniform1f("glow", glow);
	program_object.set_uniform1f("time", shockwave_time_elapsed);
	program_object.set_uniform3f("shockParams", shockParams);

	gc.set_texture(0, source_texture);
	gc.set_program_object(program_object, cl_program_matrix_modelview_projection);

	draw_texture(gc, CL_Rectf(0,0,gc.get_width(),gc.get_height()), CL_Colorf::white, CL_Rectf(0.0f, 0.0f, 1.0f, 1.0f));

	gc.reset_program_object();
	gc.reset_texture(0);
}

void App::on_input_up(const CL_InputEvent &key, const CL_InputState &state)
{
	if(key.id == CL_KEY_ESCAPE)
	{
		quit = true;
	}

	if (key.id == CL_KEY_M)
	{
		shockwave_start_time = timer;
	}

	if (key.id == CL_KEY_Q)
	{
		shockParams.x -= 1.0f;
	}
	else if (key.id == CL_KEY_W)
	{
		shockParams.x += 1.0f;
	}
	else if (key.id == CL_KEY_A)
	{
		shockParams.y -= 0.1f;
	}
	else if (key.id == CL_KEY_S)
	{
		shockParams.y += 0.1f;
	}
	else if (key.id == CL_KEY_Z)
	{
		shockParams.z -= 0.01f;
	}
	else if (key.id == CL_KEY_X)
	{
		shockParams.z += 0.01f;
	}
	else if (key.id == CL_KEY_E)
	{
		shockwave_rate -= 0.2f;
		if (shockwave_rate < 0.2f)
			shockwave_rate = 0.2f;
	}
	else if (key.id == CL_KEY_R)
	{
		shockwave_rate += 0.2f;
	}
	else if (key.id == CL_KEY_D)
	{
		glow -= 0.02f;
	}
	else if (key.id == CL_KEY_F)
	{
		glow += 0.02f;
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

