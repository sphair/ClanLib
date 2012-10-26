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
	description.set_title("Guassian Blur Shader");
	description.set_size(Size(1024, 768), true);

	DisplayWindow window(description);
	InputDevice keyboard = window.get_ic().get_keyboard();
	GraphicContext gc = window.get_gc();

	Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

	Slot slot_window_close = window.sig_window_close().connect(this, &App::window_close);

	// Create offscreen texture
	Texture texture_offscreen(gc, gc.get_width(), gc.get_height());
	texture_offscreen.set_min_filter(cl_filter_nearest);
	texture_offscreen.set_mag_filter(cl_filter_nearest);

	Texture texture_offscreen2(gc, gc.get_width(), gc.get_height());
	texture_offscreen2.set_min_filter(cl_filter_nearest);
	texture_offscreen2.set_mag_filter(cl_filter_nearest);

	// Create offscreen framebuffer
	FrameBuffer framebuffer_offscreen(gc);
	framebuffer_offscreen.attach_color_buffer(0, texture_offscreen);

	FrameBuffer framebuffer_offscreen2(gc);
	framebuffer_offscreen2.attach_color_buffer(0, texture_offscreen2);

	Image background(gc, "../PostProcessing/Resources/background.png");
	Image ball(gc, "../PostProcessing/Resources/ball.png");
	ball.set_alignment(origin_center);

	// Load and link shaders
	ProgramObject shader = ProgramObject::load(gc, "Resources/vertex_shader.glsl", "Resources/fragment_shader.glsl");
	shader.bind_attribute_location(0, "Position");
	shader.bind_attribute_location(2, "TexCoord0");
	if (!shader.link())
		throw Exception("Unable to link shader program: Error:" + shader.get_info_log());

	quit = false;

	float amount = 0.0f;
	float timer = 0.0f;

	float scale = 1.0f;

	Font font(gc, "tahoma", 32);

	blur = 1.0f;
	unsigned int startTime = System::get_time();

	while (!quit)
	{
		timer = (System::get_time() - startTime) / 1000.0f;

		// Render standard image to offscreen buffer
		gc.set_frame_buffer(framebuffer_offscreen);
		background.draw(gc, 0, 0);
		ball.draw(gc, gc.get_width() / 2 + 200 * sinf(timer / 2.0f), gc.get_height() / 2 + 200 * cosf(timer / 2.0f));
		gc.reset_frame_buffer();

		gc.set_frame_buffer(framebuffer_offscreen2);
		render_gaussian_blur(gc, blur, texture_offscreen, shader, 1.0f / texture_offscreen2.get_width(), 0.0f);
		gc.reset_frame_buffer();

		render_gaussian_blur(gc, blur, texture_offscreen2, shader, 0.0f, 1.0f / texture_offscreen2.get_height());

		std::string text( "Press 1 to 9 to control blur amount. Currently it is :" + StringHelp::float_to_text(blur) );
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

void App::render_gaussian_blur(GraphicContext &gc, float blur_amount, Texture &source_texture, ProgramObject &program_object, float dx, float dy)
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

	if ((key.id >= KEY_1) && (key.id <= KEY_9))
	{
		blur = (float) (key.id - KEY_0);
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

