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
	description.set_title("Postprocessing");
	description.set_size(Size(1024, 768), true);

	DisplayWindow window(description);
	InputDevice keyboard = window.get_ic().get_keyboard();
	GraphicContext gc = window.get_gc();

	Slot slot_window_close = window.sig_window_close().connect(this, &App::window_close);

	// Create offscreen texture
	Texture texture_offscreen(gc, gc.get_width(), gc.get_height());
	texture_offscreen.set_min_filter(cl_filter_nearest);
	texture_offscreen.set_mag_filter(cl_filter_nearest);

	// Create offscreen framebuffer
	FrameBuffer framebuffer_offscreen(gc);
	framebuffer_offscreen.attach_color_buffer(0, texture_offscreen);

	Image background(gc, "Resources/background.png");
	Image ball(gc, "Resources/ball.png");
	ball.set_alignment(origin_center);

	// Load and link shaders
	ProgramObject shader = ProgramObject::load(gc, "Resources/vertex_shader.glsl", "Resources/fragment_shader.glsl");
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

	while (!keyboard.get_keycode(KEY_ESCAPE) && !quit )
	{
		timer = (System::get_time() - startTime) / 1000.0f;

		// Uncomment this to make ball jump silly
		//float scale = (sinf(timer * 2.0f) + 1.1f);
		//ball.set_scale(scale, scale);

		// Render standard image to offscreen buffer
		gc.set_frame_buffer(framebuffer_offscreen);
		background.draw(gc, 0, 0);
		ball.draw(gc, gc.get_width() / 2 + 200 * sinf(timer / 2.0f), gc.get_height() / 2 + 200 * cosf(timer / 2.0f));
		gc.reset_frame_buffer();

		// Render offscreen buffer to screen using post process shader
		gc.set_texture(0, texture_offscreen);
		gc.set_program_object(shader, cl_program_matrix_modelview_projection);
		shader.set_uniform1i("SourceTexture", 0);
		shader.set_uniform1f("Amount", amount);
		shader.set_uniform1f("Timer", timer);
		draw_texture(gc, Rect(0, 0, gc.get_width(), gc.get_height()));
		gc.reset_program_object();
		gc.reset_texture(0);

		if(timer > 2.0f)
		{
			amount += 0.005f;
			if(amount > 1.0f)
				amount = 1.0f;
		}

		window.flip();

		System::sleep(10);

		KeepAlive::process();
	}

	return 0;
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

void App::window_close()
{
	quit = true;
}
