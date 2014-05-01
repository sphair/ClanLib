/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
	description.set_title("Postprocessing");
	description.set_size(Size(1024, 768), true);

	clan::DisplayWindow window(description);
	clan::InputDevice keyboard = window.get_ic().get_keyboard();
	clan::Canvas canvas(window);
    clan::CallbackContainer cc;

	cc.connect(window.sig_window_close(), {this, &App::window_close});

	// Create offscreen texture
	clan::Texture2D texture_offscreen(canvas, canvas.get_width(), canvas.get_height());
	texture_offscreen.set_min_filter(clan::filter_nearest);
	texture_offscreen.set_mag_filter(clan::filter_nearest);

	// Create offscreen framebuffer
	clan::FrameBuffer framebuffer_offscreen(canvas);
	framebuffer_offscreen.attach_color(0, texture_offscreen);
	clan::Canvas canvas_offscreen(canvas, framebuffer_offscreen);

	clan::Image background(canvas, "Resources/background.png");
	clan::Image ball(canvas, "Resources/ball.png");
	ball.set_alignment(origin_center);

	// Load and link shaders
	clan::ProgramObject shader = clan::ProgramObject::load(canvas, "Resources/vertex_shader.glsl", "Resources/fragment_shader.glsl");
	shader.bind_attribute_location(0, "Position");
	shader.bind_attribute_location(1, "TexCoord0");
	shader.bind_frag_data_location(0, "cl_FragColor");
	if (!shader.link())
		throw Exception("Unable to link shader program: Error:" + shader.get_info_log());
	shader.set_uniform1i("Texture", 0);

	gpu_positions = clan::VertexArrayVector<clan::Vec2f>(canvas, 6);
	gpu_tex1_coords = clan::VertexArrayVector<clan::Vec2f>(canvas, 6);
	gpu_uniforms = clan::UniformVector<ProgramUniforms>(canvas, 1);
	gpu_primitives_array = clan::PrimitivesArray(canvas);
	gpu_primitives_array.set_attributes(0, gpu_positions);
	gpu_primitives_array.set_attributes(1, gpu_tex1_coords);

	quit = false;

	uniforms.amount = 0.0f;
	uniforms.timer = 0.0f;

	float scale = 1.0f;

	ubyte64 startTime = System::get_time();

	while (!keyboard.get_keycode(clan::keycode_escape) && !quit )
	{
		uniforms.timer = (System::get_time() - startTime) / 1000.0f;

		// Uncomment this to make ball jump silly
		//float scale = (sinf(timer * 2.0f) + 1.1f);
		//ball.set_scale(scale, scale);

		// Render standard image to offscreen buffer

		background.draw(canvas_offscreen, 0, 0);
		ball.draw(canvas_offscreen, canvas.get_width() / 2 + 200 * sinf(uniforms.timer / 2.0f), canvas.get_height() / 2 + 200 * cosf(uniforms.timer / 2.0f));
		canvas_offscreen.flush();

		// Render offscreen buffer to screen using post process shader

		canvas.flush();
		clan::GraphicContext gc = canvas.get_gc();

		gc.set_texture(0, texture_offscreen);
		gc.set_program_object(shader);

		uniforms.cl_ModelViewProjectionMatrix = canvas.get_projection() * canvas.get_modelview();
		gpu_uniforms.upload_data(gc, &uniforms, 1);
		gc.set_uniform_buffer(0, gpu_uniforms);

		draw_texture(gc, clan::Rectf(0,0,canvas.get_width(),canvas.get_height()), clan::Rectf(0.0f, 0.0f, 1.0f, 1.0f));

		gc.reset_program_object();
		gc.reset_texture(0);


		if(uniforms.timer > 2.0f)
		{
			uniforms.amount += 0.005f;
			if(uniforms.amount > 1.0f)
				uniforms.amount = 1.0f;
		}

		window.flip();

		clan::System::sleep(10);

		clan::KeepAlive::process();
	}

	return 0;
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


void App::window_close()
{
	quit = true;
}
