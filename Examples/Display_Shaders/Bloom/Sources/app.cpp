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
	description.set_title("Bloom Shader");
	description.set_size(Size(1024, 768), true);

	DisplayWindow window(description);
	InputDevice keyboard = window.get_ic().get_keyboard();
	GraphicContext gc = window.get_gc();

	Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

	Slot slot_window_close = window.sig_window_close().connect(this, &App::window_close);

	// Create offscreen texture
	Texture texture_offscreen1(gc, gc.get_width(), gc.get_height());
	texture_offscreen1.set_min_filter(cl_filter_nearest);
	texture_offscreen1.set_mag_filter(cl_filter_nearest);

	Texture texture_offscreen2(gc, gc.get_width(), gc.get_height());
	texture_offscreen2.set_min_filter(cl_filter_nearest);
	texture_offscreen2.set_mag_filter(cl_filter_nearest);

	Texture texture_offscreen3(gc, gc.get_width(), gc.get_height());
	texture_offscreen3.set_min_filter(cl_filter_nearest);
	texture_offscreen3.set_mag_filter(cl_filter_nearest);

	// Create offscreen framebuffer
	FrameBuffer framebuffer_offscreen1(gc);
	framebuffer_offscreen1.attach_color_buffer(0, texture_offscreen1);

	FrameBuffer framebuffer_offscreen2(gc);
	framebuffer_offscreen2.attach_color_buffer(0, texture_offscreen2);

	FrameBuffer framebuffer_offscreen3(gc);
	framebuffer_offscreen3.attach_color_buffer(0, texture_offscreen3);

	Image background(gc, "../PostProcessing/Resources/background.png");

	// Load and link shaders
	ProgramObject gaussian_blur_shader = ProgramObject::load(gc, "Resources/gaussian_vs.glsl", "Resources/gaussian_fs.glsl");
	gaussian_blur_shader.bind_attribute_location(0, "Position");
	gaussian_blur_shader.bind_attribute_location(2, "TexCoord0");
	if (!gaussian_blur_shader.link())
		throw Exception("Unable to link gaussian shader program: Error:" + gaussian_blur_shader.get_info_log());

	ProgramObject extract_highlights_shader = ProgramObject::load(gc, "Resources/highlights_vs.glsl", "Resources/highlights_fs.glsl");
	extract_highlights_shader.bind_attribute_location(0, "Position");
	extract_highlights_shader.bind_attribute_location(2, "TexCoord0");
	if (!extract_highlights_shader.link())
		throw Exception("Unable to link hightlights shader program: Error:" + extract_highlights_shader.get_info_log());

	ProgramObject bloom_combine_shader = ProgramObject::load(gc, "Resources/bloom_vs.glsl", "Resources/bloom_fs.glsl");
	bloom_combine_shader.bind_attribute_location(0, "Position");
	bloom_combine_shader.bind_attribute_location(2, "TexCoord0");
	if (!bloom_combine_shader.link())
		throw Exception("Unable to link bloom shader program: Error:" + bloom_combine_shader.get_info_log());

	quit = false;

	float amount = 0.0f;
	float timer = 0.0f;

	float scale = 1.0f;

	Font font(gc, "tahoma", 32);

	select_text = "Default";
	highlight_threshold = 0.25f;
	blur_amount = 4;
	bloom_intensity = 1.25f;
	base_intensity = 1.0f;
	bloom_saturation = 1.0f;
	base_saturation = 1.0f;

	unsigned int startTime = System::get_time();

	while (!quit)
	{
		timer = (System::get_time() - startTime) / 1000.0f;

		// Render standard image to offscreen buffer
		gc.set_frame_buffer(framebuffer_offscreen1);
		background.set_color(Colorf(0.5f, 0.5f, 0.5f, 1.0f));	// Half brightness
		background.draw(gc, 0, 0);
		float xpos = gc.get_width() / 2 + 200 * sinf(timer / 2.0f);
		float ypos = gc.get_height() / 2 + 200 * cosf(timer / 2.0f);
		Draw::circle(gc, xpos, ypos, 64.0f, Colorf(0.8f, 0.8f, 0.0f, 1.0f));	// Draw Sun

		gc.reset_frame_buffer();

		// Render highlights
		gc.set_frame_buffer(framebuffer_offscreen2);
		render_extract_highlights(gc, texture_offscreen1, extract_highlights_shader);

		// Render horizontal blur
		gc.set_frame_buffer(framebuffer_offscreen3);
		render_gaussian_blur(gc, texture_offscreen2, gaussian_blur_shader, 1.0f / texture_offscreen2.get_width(), 0.0f);

		// Render vertical blur
		gc.set_frame_buffer(framebuffer_offscreen2);
		render_gaussian_blur(gc, texture_offscreen3, gaussian_blur_shader, 0.0f, 1.0f / texture_offscreen3.get_height());

		// Render bloom combine
		gc.reset_frame_buffer();
		render_bloom_combine(gc, texture_offscreen1, texture_offscreen2, bloom_combine_shader);

		std::string text( "Press 1 to 7 to select bloom. Currently it is :" + select_text );
		font.draw_text(gc, 10, 64, text);

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

float App::compute_gaussian(float n, float theta) // theta = Blur Amount
{
	return (float)((1.0f / sqrtf(2 * (float)PI * theta)) * expf(-(n * n) / (2.0f * theta * theta)));
}

void App::render_gaussian_blur(GraphicContext &gc, Texture &source_texture, ProgramObject &program_object, float dx, float dy)
{
	int sampleCount = 15;

	float *sampleWeights = new float[sampleCount];
	Vec2f *sampleOffsets = new Vec2f[sampleCount];

	sampleWeights[0] = compute_gaussian(0, blur_amount);
	sampleOffsets[0] = Vec2f(0.0, 0.0);

	float totalWeights = sampleWeights[0];

	for (int i = 0; i < sampleCount / 2; i++)
	{
		float weight = compute_gaussian(i + 1.0f, blur_amount);

		sampleWeights[i * 2 + 1] = weight;
		sampleWeights[i * 2 + 2] = weight;

		totalWeights += weight * 2;

		float sampleOffset = i * 2 + 1.5f;

		Vec2f delta = Vec2f(dx * sampleOffset, dy * sampleOffset);

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

	gc.set_texture(0, source_texture);
	gc.set_program_object(program_object, cl_program_matrix_modelview_projection);

	draw_texture(gc, Rectf(0,0,gc.get_width(),gc.get_height()), Colorf::white, Rectf(0.0f, 0.0f, 1.0f, 1.0f));

	gc.reset_program_object();
	gc.reset_texture(0);
}

void App::on_input_up(const InputEvent &key)
{
	if(key.id == KEY_ESCAPE)
	{
		quit = true;
	}

	if ((key.id >= KEY_0) && (key.id <= KEY_9))
	{
		blur_amount = (float) (key.id - KEY_0);
	}

	if (key.id == KEY_1)	// Default
	{
		select_text = "Default";
		highlight_threshold = 0.25f;
		blur_amount = 4;
		bloom_intensity = 1.25f;
		base_intensity = 1.0f;
		bloom_saturation = 1.0f;
		base_saturation = 1.0f;
	}
	if (key.id == KEY_2)	// Soft
	{
		select_text = "Soft";
		highlight_threshold = 0.0f;
		blur_amount = 3;
		bloom_intensity = 1.0f;
		base_intensity = 1.0f;
		bloom_saturation = 1.0f;
		base_saturation = 1.0f;
	}
	if (key.id == KEY_3) // Desaturated
	{
		select_text = "Desaturated";
		highlight_threshold = 0.5f;
		blur_amount = 8;
		bloom_intensity = 2.0f;
		base_intensity = 1.0f;
		bloom_saturation = 0.0f;
		base_saturation = 1.0f;
	}
	if (key.id == KEY_4) // Saturated
	{
		select_text = "Saturated";
		highlight_threshold = 0.25f;
		blur_amount = 4;
		bloom_intensity = 2.0f;
		base_intensity = 1.0f;
		bloom_saturation = 2.0f;
		base_saturation = 0.0f;
	}
	if (key.id == KEY_5) // Blurry
	{
		select_text = "Blurry";
		highlight_threshold = 0.0f;
		blur_amount = 2;
		bloom_intensity = 1.0f;
		base_intensity = 0.1f;
		bloom_saturation = 1.0f;
		base_saturation = 1.0f;
	}
	if (key.id == KEY_6) // Subtle
	{
		select_text = "Subtle";
		highlight_threshold = 0.5f;
		blur_amount = 2;
		bloom_intensity = 1.0f;
		base_intensity = 1.0f;
		bloom_saturation = 1.0f;
		base_saturation = 1.0f;
	}
	if (key.id == KEY_7) // Dreamlike
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

void App::render_extract_highlights(GraphicContext &gc, Texture &source_texture, ProgramObject &program_object)
{
	gc.set_texture(0, source_texture);

	gc.set_program_object(program_object);
	program_object.set_uniform1i(("SourceTexture"), 0);
	program_object.set_uniform1f(("Threshold"), highlight_threshold);

	draw_texture(gc, Rectf(0,0,gc.get_width(),gc.get_height()), Colorf::white, Rectf(0.0f, 0.0f, 1.0f, 1.0f));

	gc.reset_program_object();
	gc.reset_texture(0);
}


void App::render_bloom_combine(GraphicContext &gc, Texture &tex_base, Texture &tex_bloom, ProgramObject &program_object)
{
	gc.set_texture(0, tex_base);
	gc.set_texture(1, tex_bloom);

	gc.set_program_object(program_object);
	program_object.set_uniform1i(("BaseTexture"), 0);
	program_object.set_uniform1f(("BaseIntensity"), base_intensity);
	program_object.set_uniform1f(("BaseSaturation"), base_saturation);

	program_object.set_uniform1i(("BloomTexture"), 1);
	program_object.set_uniform1f(("BloomIntensity"), bloom_intensity);
	program_object.set_uniform1f(("BloomSaturation"), bloom_saturation);

	draw_texture(gc, Rectf(0,0,gc.get_width(),gc.get_height()), Colorf::white, Rectf(0.0f, 0.0f, 1.0f, 1.0f));

	gc.reset_program_object();
	gc.reset_texture(0);
	gc.reset_texture(1);
}

