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
	description.set_title("Guassian Blur Shader");
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

	clan::Texture2D texture_offscreen2(canvas, canvas.get_width(), canvas.get_height());
	texture_offscreen2.set_min_filter(clan::filter_nearest);
	texture_offscreen2.set_mag_filter(clan::filter_nearest);

	// Create offscreen framebuffer
	clan::FrameBuffer framebuffer_offscreen(canvas);
	framebuffer_offscreen.attach_color(0, texture_offscreen);

	clan::FrameBuffer framebuffer_offscreen2(canvas);
	framebuffer_offscreen2.attach_color(0, texture_offscreen2);

	clan::Image background(canvas, "../PostProcessing/Resources/background.png");
	clan::Image ball(canvas, "../PostProcessing/Resources/ball.png");
	ball.set_alignment(clan::origin_center);

	// Load and link shaders
	clan::ProgramObject shader = clan::ProgramObject::load(canvas, "Resources/vertex_shader.glsl", "Resources/fragment_shader.glsl");
	shader.bind_attribute_location(0, "Position");
	shader.bind_attribute_location(2, "TexCoord0");
	if (!shader.link())
		throw clan::Exception("Unable to link shader program: Error:" + shader.get_info_log());

	quit = false;

	float amount = 0.0f;
	float timer = 0.0f;

	float scale = 1.0f;

	clan::Font font(canvas, "tahoma", 32);

	blur = 1.0f;
	unsigned int startTime = clan::System::get_time();

	while (!quit)
	{
		timer = (clan::System::get_time() - startTime) / 1000.0f;

		// Render standard image to offscreen buffer
		canvas.set_frame_buffer(framebuffer_offscreen);
		background.draw(canvas, 0, 0);
		ball.draw(canvas, canvas.get_width() / 2 + 200 * sinf(timer / 2.0f), canvas.get_height() / 2 + 200 * cosf(timer / 2.0f));
		canvas.reset_frame_buffer();

		canvas.set_frame_buffer(framebuffer_offscreen2);
		render_gaussian_blur(canvas, blur, texture_offscreen, shader, 1.0f / texture_offscreen2.get_width(), 0.0f);
		canvas.reset_frame_buffer();

		render_gaussian_blur(canvas, blur, texture_offscreen2, shader, 0.0f, 1.0f / texture_offscreen2.get_height());

		std::string text( "Press 1 to 9 to control blur amount. Currently it is :" + clan::StringHelp::float_to_text(blur) );
		font.draw_text(canvas, 10, 64, text);

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

float App::compute_gaussian(float n, float theta) // theta = Blur Amount
{
	return (float)((1.0f / sqrtf(2 * (float)clan::PI * theta)) * expf(-(n * n) / (2.0f * theta * theta)));
}

void App::render_gaussian_blur(clan::Canvas &canvas, float blur_amount, clan::Texture2D &source_texture, clan::ProgramObject &program_object, float dx, float dy)
{
	int sampleCount = 15;

	float *sampleWeights = new float[sampleCount];
	clan::Vec2f *sampleOffsets = new clan::Vec2f[sampleCount];

	sampleWeights[0] = compute_gaussian(0, blur_amount);
	sampleOffsets[0] = clan::Vec2f(0.0, 0.0);

	float totalWeights = sampleWeights[0];

	for (int i = 0; i < sampleCount / 2; i++)
	{
		float weight = compute_gaussian(i + 1.0f, blur_amount);

		sampleWeights[i * 2 + 1] = weight;
		sampleWeights[i * 2 + 2] = weight;

		totalWeights += weight * 2;

		float sampleOffset = i * 2 + 1.5f;

		clan::Vec2f delta = Vec2f(dx * sampleOffset, dy * sampleOffset);

		sampleOffsets[i * 2 + 1] = delta;
		sampleOffsets[i * 2 + 2] = Vec2f(-delta.x, -delta.y);
	}

	for (int i = 0; i < sampleCount; i++)
	{
		sampleWeights[i] /= totalWeights;
	}

	program_object.set_uniform1i("SourceTexture", 0);
	program_object.set_uniformfv("SampleOffsets", 2, sampleCount, (float *)sampleOffsets);
	program_object.set_uniformfv("SampleWeights", 1, sampleCount, sampleWeights);

	canvas.set_texture(0, source_texture);
	canvas.set_program_object(program_object, cl_program_matrix_modelview_projection);

	draw_texture(canvas, clan::Rectf(0,0,canvas.get_width(),canvas.get_height()), clan::Colorf::white, clan::Rectf(0.0f, 0.0f, 1.0f, 1.0f));

	canvas.reset_program_object();
	canvas.reset_texture(0);

	delete sampleWeights;
	delete sampleOffsets;
}

void App::on_input_up(const clan::InputEvent &key)
{
	if(key.id == clan::keycode_escape)
	{
		quit = true;
	}

	if ((key.id >= clan::keycode_1) && (key.id <= clan::keycode_9))
	{
		blur = (float) (key.id - clan::keycode_0);
	}

}

void App::draw_texture(clan::Canvas &canvas, const clan::Rectf &rect, const clan::Colorf &color, const clan::Rectf &texture_unit1_coords)
{
	clan::Vec2f positions[6] =
	{
		Vec2f(rect.left, rect.top),
		Vec2f(rect.right, rect.top),
		Vec2f(rect.left, rect.bottom),
		Vec2f(rect.right, rect.top),
		Vec2f(rect.left, rect.bottom),
		Vec2f(rect.right, rect.bottom)
	};

	clan::Vec2f tex1_coords[6] =
	{
		Vec2f(texture_unit1_coords.left, texture_unit1_coords.top),
		Vec2f(texture_unit1_coords.right, texture_unit1_coords.top),
		Vec2f(texture_unit1_coords.left, texture_unit1_coords.bottom),
		Vec2f(texture_unit1_coords.right, texture_unit1_coords.top),
		Vec2f(texture_unit1_coords.left, texture_unit1_coords.bottom),
		Vec2f(texture_unit1_coords.right, texture_unit1_coords.bottom)
	};

	clan::PrimitivesArray prim_array(canvas);
	prim_array.set_attributes(0, positions);
	prim_array.set_attribute(1, color);
	prim_array.set_attributes(2, tex1_coords);
	canvas.draw_primitives(clan::type_triangles, 6, prim_array);
}

