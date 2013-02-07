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
	description.set_title("Postprocessing");
	description.set_size(Size(1024, 768), true);

	clan::DisplayWindow window(description);
	clan::InputDevice keyboard = window.get_ic().get_keyboard();
	clan::Canvas canvas(window);

	clan::Slot slot_window_close = window.sig_window_close().connect(this, &App::window_close);

	// Create offscreen texture
	clan::Texture2D texture_offscreen(canvas, canvas.get_width(), canvas.get_height());
	texture_offscreen.set_min_filter(clan::filter_nearest);
	texture_offscreen.set_mag_filter(clan::filter_nearest);

	// Create offscreen framebuffer
	clan::FrameBuffer framebuffer_offscreen(canvas);
	framebuffer_offscreen.attach_color(0, texture_offscreen);

	clan::Image background(canvas, "Resources/background.png");
	clan::Image ball(canvas, "Resources/ball.png");
	ball.set_alignment(origin_center);

	// Load and link shaders
	clan::ProgramObject shader = clan::ProgramObject::load(canvas, "Resources/vertex_shader.glsl", "Resources/fragment_shader.glsl");
	shader.bind_attribute_location(0, "Position");
	shader.bind_attribute_location(1, "Color0");
	shader.bind_attribute_location(2, "TexCoord0");
	if (!shader.link())
		throw Exception("Unable to link shader program: Error:" + shader.get_info_log());

	quit = false;

	float amount = 0.0f;
	float timer = 0.0f;

	float scale = 1.0f;

	unsigned int startTime = System::get_time();

	while (!keyboard.get_keycode(clan::keycode_escape) && !quit )
	{
		timer = (System::get_time() - startTime) / 1000.0f;

		// Uncomment this to make ball jump silly
		//float scale = (sinf(timer * 2.0f) + 1.1f);
		//ball.set_scale(scale, scale);

		// Render standard image to offscreen buffer
		canvas.set_frame_buffer(framebuffer_offscreen);
		background.draw(canvas, 0, 0);
		ball.draw(canvas, canvas.get_width() / 2 + 200 * sinf(timer / 2.0f), canvas.get_height() / 2 + 200 * cosf(timer / 2.0f));
		canvas.reset_frame_buffer();

		// Render offscreen buffer to screen using post process shader
		canvas.set_texture(0, texture_offscreen);
		canvas.set_program_object(shader, cl_program_matrix_modelview_projection);
		shader.set_uniform1i("SourceTexture", 0);
		shader.set_uniform1f("Amount", amount);
		shader.set_uniform1f("Timer", timer);
		draw_texture(canvas, Rect(0, 0, canvas.get_width(), canvas.get_height()));
		canvas.reset_program_object();
		canvas.reset_texture(0);

		if(timer > 2.0f)
		{
			amount += 0.005f;
			if(amount > 1.0f)
				amount = 1.0f;
		}

		canvas.flip();

		clan::System::sleep(10);

		clan::KeepAlive::process();
	}

	return 0;
}

void App::draw_texture(clan::Canvas &canvas, const clan::Rectf &rect, const clan::Colorf &color, const clan::Rectf &texture_unit1_coords)
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

	clan::PrimitivesArray prim_array(canvas);
	prim_array.set_attributes(0, positions);
	prim_array.set_attribute(1, color);
	prim_array.set_attributes(2, tex1_coords);
	canvas.draw_primitives(clan::type_triangles, 6, prim_array);
}

void App::window_close()
{
	quit = true;
}
