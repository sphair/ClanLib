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
	clan::DisplayWindowDescription description;
	description.set_title("Shockwave Shader");
	description.set_size(clan::Size(1024, 768), true);

	clan::DisplayWindow window(description);
	clan::InputDevice keyboard = window.get_ic().get_keyboard();
	clan::Canvas canvas(window);

	clan::Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

	clan::Slot slot_window_close = window.sig_window_close().connect(this, &App::window_close);

	// Create offscreen texture
	clan::Texture2D texture_offscreen(canvas, canvas.get_width(), canvas.get_height());
	texture_offscreen.set_min_filter(clan::filter_nearest);
	texture_offscreen.set_mag_filter(clan::filter_nearest);

	// Create offscreen framebuffer
	clan::FrameBuffer framebuffer_offscreen(canvas);
	framebuffer_offscreen.attach_color(0, texture_offscreen);
	clan::Canvas canvas_offscreen(canvas, framebuffer_offscreen);

	clan::Image background(canvas, "../PostProcessing/Resources/background.png");
	clan::Image ball(canvas, "../PostProcessing/Resources/ball.png");
	ball.set_alignment(clan::origin_center);

	// Load and link shaders
	clan::ProgramObject shader = clan::ProgramObject::load(canvas, "Resources/vertex_shader.glsl", "Resources/fragment_shader.glsl");
	shader.bind_attribute_location(0, "Position");
	shader.bind_attribute_location(1, "TexCoord0");
	shader.bind_frag_data_location(0, "cl_FragColor");

	if (!shader.link())
		throw clan::Exception("Unable to link shader program: Error:" + shader.get_info_log());
	shader.set_uniform1i("Texture0", 0);

	quit = false;

	float amount = 0.0f;
	timer = 0.0f;

	float scale = 1.0f;

	clan::Font font(canvas, "tahoma", 32);

	// Shader idea and code from http://www.geeks3d.com/20091116/shader-library-2d-shockwave-post-processing-filter-glsl/
	// Shader enhanced for clanlib

	shockParams = clan::Vec3f(10.0f, 0.8f, 0.1f);

	unsigned int startTime = clan::System::get_time();
	shockwave_start_time = 0.0f;
	shockwave_rate = 1.0f;
	glow = 0.1f;

	while (!quit)
	{
		timer = (clan::System::get_time() - startTime) / 1000.0f;

		shockwave_time_elapsed = (timer - shockwave_start_time) / shockwave_rate;

		// Render standard image to offscreen buffer
		background.draw(canvas_offscreen, 0, 0);
		float xpos = canvas.get_width() / 2 + 200 * sinf(timer / 2.0f);
		float ypos = canvas.get_height() / 2 + 200 * cosf(timer / 2.0f);
		ball.draw(canvas_offscreen, xpos, ypos);
		canvas_offscreen.flush();

		center.x = xpos / ((float) canvas.get_width());
		center.y = ypos / ((float) canvas.get_height());

		render_shockwave(canvas, texture_offscreen, shader);

		const int gap = 32;
		font.draw_text(canvas, 10, 64 + gap*0, "Press 'M' to emit a shockwave");
		font.draw_text(canvas, 10, 64 + gap*1, "base: " + clan::StringHelp::float_to_text(shockParams.x) + " (Press Q,W)");
		font.draw_text(canvas, 10, 64 + gap*2, "exponent: " + clan::StringHelp::float_to_text(shockParams.y) + " (Press A,S)");
		font.draw_text(canvas, 10, 64 + gap*3, "distance: " + clan::StringHelp::float_to_text(shockParams.z) + " (Press Z,X)");
		font.draw_text(canvas, 10, 64 + gap*4, "rate: " + clan::StringHelp::float_to_text(shockwave_rate) + " (Press E,R)");
		font.draw_text(canvas, 10, 64 + gap*5, "glow: " + clan::StringHelp::float_to_text(glow) + " (Press D,F)");

		canvas.flip();

		clan::System::sleep(10);

		clan::KeepAlive::process();
	}

	return 0;
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

	struct ProgramUniforms
	{
		clan::Mat4f cl_ModelViewProjectionMatrix;
		clan::Vec3f shockParams; // 10.0, 0.8, 0.1
		float time; // effect elapsed time
		clan::Vec2f center; // Mouse position
		float glow;

	};
	ProgramUniforms buffer;
	buffer.cl_ModelViewProjectionMatrix = canvas.get_projection() * canvas.get_modelview();
	buffer.center = center;
	buffer.glow = glow;
	buffer.shockParams = shockParams;
	buffer.time = shockwave_time_elapsed;
	clan::UniformVector<ProgramUniforms> uniform_vector(gc, &buffer, 1);
	gc.set_uniform_buffer(0, uniform_vector);

	draw_texture(canvas, clan::Rectf(0,0,canvas.get_width(),canvas.get_height()), clan::Rectf(0.0f, 0.0f, 1.0f, 1.0f));

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
		shockParams.x -= 1.0f;
	}
	else if (key.id == clan::keycode_w)
	{
		shockParams.x += 1.0f;
	}
	else if (key.id == clan::keycode_a)
	{
		shockParams.y -= 0.1f;
	}
	else if (key.id == clan::keycode_s)
	{
		shockParams.y += 0.1f;
	}
	else if (key.id == clan::keycode_z)
	{
		shockParams.z -= 0.01f;
	}
	else if (key.id == clan::keycode_x)
	{
		shockParams.z += 0.01f;
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
		glow -= 0.02f;
	}
	else if (key.id == clan::keycode_f)
	{
		glow += 0.02f;
	}
}

void App::draw_texture(clan::Canvas &canvas, const clan::Rectf &rect, const clan::Rectf &texture_unit1_coords)
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

	clan::GraphicContext gc = canvas.get_gc();

	clan::PrimitivesArray prim_array(gc);

	clan::VertexArrayVector<clan::Vec2f> gpu_positions = clan::VertexArrayVector<clan::Vec2f>(gc, positions, 6);
	clan::VertexArrayVector<clan::Vec2f> gpu_tex1_coords = clan::VertexArrayVector<clan::Vec2f>(gc, tex1_coords, 6);

	prim_array.set_attributes(0, gpu_positions);
	prim_array.set_attributes(1, gpu_tex1_coords);

	gc.draw_primitives(clan::type_triangles, 6, prim_array);
}

