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
	description.set_title("Bloom Shader");
	description.set_size(clan::Size(1024, 768), true);

	clan::DisplayWindow window(description);
	clan::InputDevice keyboard = window.get_ic().get_keyboard();
	clan::Canvas canvas(window);

	clan::Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

	clan::Slot slot_window_close = window.sig_window_close().connect(this, &App::window_close);

	// Create offscreen texture
	clan::Texture2D texture_offscreen1(canvas, canvas.get_width(), canvas.get_height());
	texture_offscreen1.set_min_filter(clan::filter_nearest);
	texture_offscreen1.set_mag_filter(clan::filter_nearest);

	clan::Texture2D texture_offscreen2(canvas, canvas.get_width(), canvas.get_height());
	texture_offscreen2.set_min_filter(clan::filter_nearest);
	texture_offscreen2.set_mag_filter(clan::filter_nearest);

	clan::Texture2D texture_offscreen3(canvas, canvas.get_width(), canvas.get_height());
	texture_offscreen3.set_min_filter(clan::filter_nearest);
	texture_offscreen3.set_mag_filter(clan::filter_nearest);

	// Create offscreen framebuffer
	clan::FrameBuffer framebuffer_offscreen1(canvas);
	framebuffer_offscreen1.attach_color(0, texture_offscreen1);

	clan::FrameBuffer framebuffer_offscreen2(canvas);
	framebuffer_offscreen2.attach_color(0, texture_offscreen2);

	clan::FrameBuffer framebuffer_offscreen3(canvas);
	framebuffer_offscreen3.attach_color(0, texture_offscreen3);

	clan::Image background(canvas, "../PostProcessing/Resources/background.png");

	// Load and link shaders
	clan::ProgramObject gaussian_blur_shader = clan::ProgramObject::load(canvas, "Resources/gaussian_vs.glsl", "Resources/gaussian_fs.glsl");
	gaussian_blur_shader.bind_attribute_location(0, "Position");
	gaussian_blur_shader.bind_attribute_location(2, "TexCoord0");
	if (!gaussian_blur_shader.link())
		throw clan::Exception("Unable to link gaussian shader program: Error:" + gaussian_blur_shader.get_info_log());

	clan::ProgramObject extract_highlights_shader = clan::ProgramObject::load(canvas, "Resources/highlights_vs.glsl", "Resources/highlights_fs.glsl");
	extract_highlights_shader.bind_attribute_location(0, "Position");
	extract_highlights_shader.bind_attribute_location(2, "TexCoord0");
	if (!extract_highlights_shader.link())
		throw clan::Exception("Unable to link hightlights shader program: Error:" + extract_highlights_shader.get_info_log());

	clan::ProgramObject bloom_combine_shader = clan::ProgramObject::load(canvas, "Resources/bloom_vs.glsl", "Resources/bloom_fs.glsl");
	bloom_combine_shader.bind_attribute_location(0, "Position");
	bloom_combine_shader.bind_attribute_location(2, "TexCoord0");
	if (!bloom_combine_shader.link())
		throw clan::Exception("Unable to link bloom shader program: Error:" + bloom_combine_shader.get_info_log());

	quit = false;

	float amount = 0.0f;
	float timer = 0.0f;

	float scale = 1.0f;

	clan::Font font(canvas, "tahoma", 32);

	select_text = "Default";
	highlight_threshold = 0.25f;
	blur_amount = 4;
	bloom_intensity = 1.25f;
	base_intensity = 1.0f;
	bloom_saturation = 1.0f;
	base_saturation = 1.0f;

	clan::ubyte64 startTime = clan::System::get_time();

	while (!quit)
	{
		timer = (clan::System::get_time() - startTime) / 1000.0f;

		// Render standard image to offscreen buffer
		canvas.set_frame_buffer(framebuffer_offscreen1);
		background.set_color(clan::Colorf(0.5f, 0.5f, 0.5f, 1.0f));	// Half brightness
		background.draw(canvas, 0, 0);
		float xpos = canvas.get_width() / 2 + 200 * sinf(timer / 2.0f);
		float ypos = canvas.get_height() / 2 + 200 * cosf(timer / 2.0f);
		canvas.fill_circle(xpos, ypos, 64.0f, clan::Colorf(0.8f, 0.8f, 0.0f, 1.0f));	// Draw Sun

		canvas.reset_frame_buffer();

		// Render highlights
		canvas.set_frame_buffer(framebuffer_offscreen2);
		render_extract_highlights(canvas, texture_offscreen1, extract_highlights_shader);

		// Render horizontal blur
		canvas.set_frame_buffer(framebuffer_offscreen3);
		render_gaussian_blur(canvas, texture_offscreen2, gaussian_blur_shader, 1.0f / texture_offscreen2.get_width(), 0.0f);

		// Render vertical blur
		canvas.set_frame_buffer(framebuffer_offscreen2);
		render_gaussian_blur(canvas, texture_offscreen3, gaussian_blur_shader, 0.0f, 1.0f / texture_offscreen3.get_height());

		// Render bloom combine
		canvas.reset_frame_buffer();
		render_bloom_combine(canvas, texture_offscreen1, texture_offscreen2, bloom_combine_shader);

		std::string text( "Press 1 to 7 to select bloom. Currently it is :" + select_text );
		font.draw_text(canvas, 10, 64, text);

		window.flip();

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

void App::render_gaussian_blur(clan::Canvas &canvas, clan::Texture2D &source_texture, clan::ProgramObject &program_object, float dx, float dy)
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

		clan::Vec2f delta = clan::Vec2f(dx * sampleOffset, dy * sampleOffset);

		sampleOffsets[i * 2 + 1] = delta;
		sampleOffsets[i * 2 + 2] = clan::Vec2f(-delta.x, -delta.y);
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

	if ((key.id >= clan::keycode_0) && (key.id <= clan::keycode_9))
	{
		blur_amount = (float) (key.id - clan::keycode_0);
	}

	if (key.id == clan::keycode_1)	// Default
	{
		select_text = "Default";
		highlight_threshold = 0.25f;
		blur_amount = 4;
		bloom_intensity = 1.25f;
		base_intensity = 1.0f;
		bloom_saturation = 1.0f;
		base_saturation = 1.0f;
	}
	if (key.id == clan::keycode_2)	// Soft
	{
		select_text = "Soft";
		highlight_threshold = 0.0f;
		blur_amount = 3;
		bloom_intensity = 1.0f;
		base_intensity = 1.0f;
		bloom_saturation = 1.0f;
		base_saturation = 1.0f;
	}
	if (key.id == clan::keycode_3) // Desaturated
	{
		select_text = "Desaturated";
		highlight_threshold = 0.5f;
		blur_amount = 8;
		bloom_intensity = 2.0f;
		base_intensity = 1.0f;
		bloom_saturation = 0.0f;
		base_saturation = 1.0f;
	}
	if (key.id == clan::keycode_4) // Saturated
	{
		select_text = "Saturated";
		highlight_threshold = 0.25f;
		blur_amount = 4;
		bloom_intensity = 2.0f;
		base_intensity = 1.0f;
		bloom_saturation = 2.0f;
		base_saturation = 0.0f;
	}
	if (key.id == clan::keycode_5) // Blurry
	{
		select_text = "Blurry";
		highlight_threshold = 0.0f;
		blur_amount = 2;
		bloom_intensity = 1.0f;
		base_intensity = 0.1f;
		bloom_saturation = 1.0f;
		base_saturation = 1.0f;
	}
	if (key.id == clan::keycode_6) // Subtle
	{
		select_text = "Subtle";
		highlight_threshold = 0.5f;
		blur_amount = 2;
		bloom_intensity = 1.0f;
		base_intensity = 1.0f;
		bloom_saturation = 1.0f;
		base_saturation = 1.0f;
	}
	if (key.id == clan::keycode_7) // Dreamlike
	{
		select_text = "Dreamlike";
		highlight_threshold = 0.25f;
		blur_amount = 8;
		bloom_intensity = 2.0f;
		base_intensity = 1.0f;
		bloom_saturation = 0.0f;
		base_saturation = 1.0f;
	}

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

void App::render_extract_highlights(clan::Canvas &canvas, clan::Texture2D &source_texture, clan::ProgramObject &program_object)
{
	canvas.set_texture(0, source_texture);

	canvas.set_program_object(program_object);
	program_object.set_uniform1i(("SourceTexture"), 0);
	program_object.set_uniform1f(("Threshold"), highlight_threshold);

	draw_texture(canvas, clan::Rectf(0,0,canvas.get_width(),canvas.get_height()), clan::Colorf::white, clan::Rectf(0.0f, 0.0f, 1.0f, 1.0f));

	canvas.reset_program_object();
	canvas.reset_texture(0);
}


void App::render_bloom_combine(clan::Canvas &canvas, clan::Texture2D &tex_base, clan::Texture2D &tex_bloom, clan::ProgramObject &program_object)
{
	canvas.set_texture(0, tex_base);
	canvas.set_texture(1, tex_bloom);

	canvas.set_program_object(program_object);
	program_object.set_uniform1i(("BaseTexture"), 0);
	program_object.set_uniform1f(("BaseIntensity"), base_intensity);
	program_object.set_uniform1f(("BaseSaturation"), base_saturation);

	program_object.set_uniform1i(("BloomTexture"), 1);
	program_object.set_uniform1f(("BloomIntensity"), bloom_intensity);
	program_object.set_uniform1f(("BloomSaturation"), bloom_saturation);

	draw_texture(canvas, clan::Rectf(0,0,canvas.get_width(),canvas.get_height()), clan::Colorf::white, clan::Rectf(0.0f, 0.0f, 1.0f, 1.0f));

	canvas.reset_program_object();
	canvas.reset_texture(0);
	canvas.reset_texture(1);
}

