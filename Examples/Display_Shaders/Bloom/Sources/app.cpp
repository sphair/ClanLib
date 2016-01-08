/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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

clan::ApplicationInstance<App> clanapp;

App::App()
{
	clan::OpenGLTarget::set_current();
	clan::DisplayWindowDescription description;
	description.set_title("Bloom Shader");
	description.set_size(clan::Size(1024, 768), true);

	window = clan::DisplayWindow(description);
	canvas = clan::Canvas(window);

	sc.connect(window.get_keyboard().sig_key_up(), clan::bind_member(this, &App::on_input_up));

	sc.connect(window.sig_window_close(), clan::bind_member(this, &App::window_close));

	// Create offscreen texture
	texture_offscreen1 = clan::Texture2D(canvas, canvas.get_width(), canvas.get_height());
	texture_offscreen1.set_min_filter(clan::filter_nearest);
	texture_offscreen1.set_mag_filter(clan::filter_nearest);

	texture_offscreen2 = clan::Texture2D(canvas, canvas.get_width(), canvas.get_height());
	texture_offscreen2.set_min_filter(clan::filter_nearest);
	texture_offscreen2.set_mag_filter(clan::filter_nearest);

	texture_offscreen3 = clan::Texture2D(canvas, canvas.get_width(), canvas.get_height());
	texture_offscreen3.set_min_filter(clan::filter_nearest);
	texture_offscreen3.set_mag_filter(clan::filter_nearest);

	// Create offscreen framebuffer
	clan::FrameBuffer framebuffer_offscreen1(canvas);
	framebuffer_offscreen1.attach_color(0, texture_offscreen1);
	canvas_offscreen1 = clan::Canvas(canvas, framebuffer_offscreen1);

	clan::FrameBuffer framebuffer_offscreen2(canvas);
	framebuffer_offscreen2.attach_color(0, texture_offscreen2);
	canvas_offscreen2 = clan::Canvas(canvas, framebuffer_offscreen2);

	clan::FrameBuffer framebuffer_offscreen3(canvas);
	framebuffer_offscreen3.attach_color(0, texture_offscreen3);
	canvas_offscreen3 = clan::Canvas(canvas, framebuffer_offscreen3);

	background = clan::Image(canvas, "../PostProcessing/Resources/background.png");

	// Load and link shaders
	gaussian_blur_shader = clan::ProgramObject::load(canvas, "Resources/gaussian_vs.glsl", "Resources/gaussian_fs.glsl");
	gaussian_blur_shader.bind_attribute_location(0, "Position");
	gaussian_blur_shader.bind_attribute_location(1, "TexCoord0");
	gaussian_blur_shader.bind_frag_data_location(0, "cl_FragColor");
	if (!gaussian_blur_shader.link())
		throw clan::Exception("Unable to link gaussian shader program: Error:" + gaussian_blur_shader.get_info_log());

	extract_highlights_shader = clan::ProgramObject::load(canvas, "Resources/highlights_vs.glsl", "Resources/highlights_fs.glsl");
	extract_highlights_shader.bind_attribute_location(0, "Position");
	extract_highlights_shader.bind_attribute_location(1, "TexCoord0");
	//extract_highlights_shader.bind_frag_data_location(0, "cl_FragColor");
	if (!extract_highlights_shader.link())
		throw clan::Exception("Unable to link hightlights shader program: Error:" + extract_highlights_shader.get_info_log());

	bloom_combine_shader = clan::ProgramObject::load(canvas, "Resources/bloom_vs.glsl", "Resources/bloom_fs.glsl");
	bloom_combine_shader.bind_attribute_location(0, "Position");
	bloom_combine_shader.bind_attribute_location(1, "TexCoord0");
	//bloom_combine_shader.bind_frag_data_location(0, "cl_FragColor");
	if (!bloom_combine_shader.link())
		throw clan::Exception("Unable to link bloom shader program: Error:" + bloom_combine_shader.get_info_log());

	gpu_positions = clan::VertexArrayVector<clan::Vec2f>(canvas, 6);
	gpu_tex1_coords = clan::VertexArrayVector<clan::Vec2f>(canvas, 6);
	gpu_uniforms = clan::UniformVector<ProgramUniforms>(canvas, 1);
	gpu_primitives_array = clan::PrimitivesArray(canvas);
	gpu_primitives_array.set_attributes(0, gpu_positions);
	gpu_primitives_array.set_attributes(1, gpu_tex1_coords);

	font = clan::Font("tahoma", 32);

	select_text = "Default";

	startTime = clan::System::get_time();
}

bool App::update()
{
	clan::InputDevice keyboard = window.get_keyboard();
	timer = (clan::System::get_time() - startTime) / 1000.0f;

	// Render standard image to offscreen buffer
	background.set_color(clan::Colorf(0.5f, 0.5f, 0.5f, 1.0f));	// Half brightness
	background.draw(canvas_offscreen1, 0, 0);
	float xpos = canvas.get_width() / 2 + 200 * sinf(timer / 2.0f);
	float ypos = canvas.get_height() / 2 + 200 * cosf(timer / 2.0f);
	canvas_offscreen1.fill_circle(xpos, ypos, 64.0f, clan::Colorf(0.8f, 0.8f, 0.0f, 1.0f));	// Draw Sun

	canvas_offscreen1.flush();
	// Render highlights
	render_extract_highlights(canvas_offscreen2, texture_offscreen1, extract_highlights_shader);
	canvas_offscreen2.flush();

	// Render horizontal blur
	render_gaussian_blur(canvas_offscreen3, blur_amount, texture_offscreen2, gaussian_blur_shader, 1.0f / texture_offscreen2.get_width(), 0.0f);
	canvas_offscreen3.flush();

	// Render vertical blur
	render_gaussian_blur(canvas_offscreen2, blur_amount, texture_offscreen3, gaussian_blur_shader, 0.0f, 1.0f / texture_offscreen3.get_height());
	canvas_offscreen2.flush();

	// Render bloom combine
	render_bloom_combine(canvas, texture_offscreen1, texture_offscreen2, bloom_combine_shader);

	std::string text( "Press 1 to 7 to select bloom. Currently it is :" + select_text );
	font.draw_text(canvas, 10, 64, text);

	window.flip(1);


	return !quit;
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
	uniforms.sample[0].weight = compute_gaussian(0, blur_amount);
	uniforms.sample[0].offset_x = 0.0f;
	uniforms.sample[0].offset_y = 0.0f;

	float totalWeights = uniforms.sample[0].weight;

	for (int i = 0; i < sampleCount / 2; i++)
	{
		float weight = compute_gaussian(i + 1.0f, blur_amount);

		uniforms.sample[i * 2 + 1].weight = weight;
		uniforms.sample[i * 2 + 2].weight = weight;

		totalWeights += weight * 2;

		float sampleOffset = i * 2 + 1.5f;

		clan::Vec2f delta(dx * sampleOffset, dy * sampleOffset);

		uniforms.sample[i * 2 + 1].offset_x = delta.x;
		uniforms.sample[i * 2 + 1].offset_y = delta.y;
		uniforms.sample[i * 2 + 2].offset_x = -delta.x;
		uniforms.sample[i * 2 + 2].offset_y = -delta.y;
	}

	for (int i = 0; i < sampleCount; i++)
	{
		uniforms.sample[i].weight /= totalWeights;
	}

	canvas.flush();
	clan::GraphicContext gc = canvas.get_gc();

	gc.set_texture(0, source_texture);
	gc.set_program_object(program_object);

	uniforms.cl_ModelViewProjectionMatrix = canvas.get_projection() * canvas.get_transform();
	gpu_uniforms.upload_data(gc, &uniforms, 1);
	gc.set_uniform_buffer(0, gpu_uniforms);

	draw_texture(gc, clan::Rectf(0,0,canvas.get_width(),canvas.get_height()), clan::Rectf(0.0f, 0.0f, 1.0f, 1.0f));

	gc.reset_program_object();
	gc.reset_texture(0);

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



void App::render_extract_highlights(clan::Canvas &canvas, clan::Texture2D &source_texture, clan::ProgramObject &program_object)
{
	canvas.flush();
	clan::GraphicContext gc = canvas.get_gc();

	gc.set_texture(0, source_texture);

	gc.set_program_object(program_object);
	program_object.set_uniform1i(("SourceTexture"), 0);
	program_object.set_uniform1f(("Threshold"), highlight_threshold);

	program_object.set_uniform_matrix("cl_ModelViewProjectionMatrix", canvas.get_projection() * canvas.get_transform());

	draw_texture(gc, clan::Rectf(0,0,canvas.get_width(),canvas.get_height()), clan::Rectf(0.0f, 0.0f, 1.0f, 1.0f));

	gc.reset_program_object();
	gc.reset_texture(0);

}


void App::render_bloom_combine(clan::Canvas &canvas, clan::Texture2D &tex_base, clan::Texture2D &tex_bloom, clan::ProgramObject &program_object)
{
	canvas.flush();
	clan::GraphicContext gc = canvas.get_gc();

	gc.set_texture(0, tex_base);
	gc.set_texture(1, tex_bloom);

	gc.set_program_object(program_object);
	program_object.set_uniform1i(("BaseTexture"), 0);
	program_object.set_uniform1f(("BaseIntensity"), base_intensity);
	program_object.set_uniform1f(("BaseSaturation"), base_saturation);

	program_object.set_uniform1i(("BloomTexture"), 1);
	program_object.set_uniform1f(("BloomIntensity"), bloom_intensity);
	program_object.set_uniform1f(("BloomSaturation"), bloom_saturation);

	program_object.set_uniform_matrix("cl_ModelViewProjectionMatrix", canvas.get_projection() * canvas.get_transform());

	draw_texture(gc, clan::Rectf(0,0,canvas.get_width(),canvas.get_height()), clan::Rectf(0.0f, 0.0f, 1.0f, 1.0f));

	gc.reset_program_object();
	gc.reset_texture(0);
	gc.reset_texture(1);

}
