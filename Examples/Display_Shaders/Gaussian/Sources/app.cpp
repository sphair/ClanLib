/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
    clan::SlotContainer cc;

	cc.connect(window.get_ic().get_keyboard().sig_key_up(), clan::bind_member(this, &App::on_input_up));

	cc.connect(window.sig_window_close(), clan::bind_member(this, &App::window_close));

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
	clan::Canvas canvas_offscreen(canvas, framebuffer_offscreen);

	clan::FrameBuffer framebuffer_offscreen2(canvas);
	framebuffer_offscreen2.attach_color(0, texture_offscreen2);
	clan::Canvas canvas_offscreen2(canvas, framebuffer_offscreen2);

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
	shader.set_uniform1i("SourceTexture", 0);

	gpu_positions = clan::VertexArrayVector<clan::Vec2f>(canvas, 6);
	gpu_tex1_coords = clan::VertexArrayVector<clan::Vec2f>(canvas, 6);
	gpu_uniforms = clan::UniformVector<ProgramUniforms>(canvas, 1);
	gpu_primitives_array = clan::PrimitivesArray(canvas);
	gpu_primitives_array.set_attributes(0, gpu_positions);
	gpu_primitives_array.set_attributes(1, gpu_tex1_coords);

	quit = false;

	float amount = 0.0f;
	float timer = 0.0f;

	float scale = 1.0f;

	clan::Font font("tahoma", 32);

	blur = 1.0f;
	uint64_t startTime = clan::System::get_time();

	while (!quit)
	{
		timer = (clan::System::get_time() - startTime) / 1000.0f;

		// Render standard image to offscreen buffer
		background.draw(canvas_offscreen, 0, 0);
		ball.draw(canvas_offscreen, canvas.get_width() / 2 + 200 * sinf(timer / 2.0f), canvas.get_height() / 2 + 200 * cosf(timer / 2.0f));
		canvas_offscreen.flush();

		render_gaussian_blur(canvas_offscreen2, blur, texture_offscreen, shader, 1.0f / texture_offscreen2.get_width(), 0.0f);
		canvas_offscreen2.flush();

		render_gaussian_blur(canvas, blur, texture_offscreen2, shader, 0.0f, 1.0f / texture_offscreen2.get_height());

		std::string text( "Press 1 to 9 to control blur amount. Currently it is :" + clan::StringHelp::float_to_text(blur) );
		font.draw_text(canvas, 10, 64, text);

		window.flip();

		clan::System::sleep(10);

		clan::RunLoop::process();
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

	if ((key.id >= clan::keycode_1) && (key.id <= clan::keycode_9))
	{
		blur = (float) (key.id - clan::keycode_0);
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

