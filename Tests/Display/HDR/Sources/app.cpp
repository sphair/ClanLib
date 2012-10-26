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
**    Mark Page
*/

#include "precomp.h"
#include "app.h"

App::App() : quit(false)
{
}

// The start of the Application
int App::start(const std::vector<std::string> &args)
{
	try
	{
		DisplayWindowDescription win_desc;
		win_desc.set_allow_resize(true);
		win_desc.set_title("HDR Example");
		win_desc.set_size(Size( 600, 630 ), false);

		DisplayWindow window(win_desc);
		Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close);
		Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

		GraphicContext gc = window.get_gc();

		// Load and link shaders
		ProgramObject shader = ProgramObject::load(gc, "Resources/vertex_shader.glsl", "Resources/fragment_shader.glsl");
		shader.bind_attribute_location(0, "Position");
		shader.bind_attribute_location(2, "TexCoord0");
		if (!shader.link())
			throw Exception("Unable to link shader program: Error:" + shader.get_info_log());

		const float dynamic_range_start = -1.5f;
		const float dynamic_range_end = 1.5f;
		const Size image_size(gc.get_width() - 64, 128);

		Texture image_rgb32f = create_rgb32f(gc, image_size, dynamic_range_start, dynamic_range_end);

		Font font(gc, "Tahoma", 20);

		unsigned int time_last = System::get_time();

		bool direction = false;

		while (!quit)
		{
			unsigned int time_now = System::get_time();
			time_delta = ((float) time_now - time_last) / 1000.0f;
			time_last = time_now;

			if (direction)
			{
				color_offset.r += time_delta * 1.0f;
				if (color_offset.r > 2.0f)
				{
					color_offset.r = 2.0f;
					direction = false;
				}
			}
			else
			{
				color_offset.r -= time_delta * 1.0f;
				if (color_offset.r < -2.0f)
				{
					color_offset.r = -2.0f;
					direction = true;
				}
			}
			color_offset.g = color_offset.r;
			color_offset.b = color_offset.r;

			gc.clear(Colorf(0.2f,0.2f,0.5f));

			font.draw_text(gc, 32, 50, "Showing Texture RGB values from " + StringHelp::float_to_text(dynamic_range_start) + " to " + StringHelp::float_to_text(dynamic_range_end));
			draw_image(gc, image_rgb32f, 32.0f, 100.0f, shader, Vec4f(0.0f, 0.0f, 0.0f, 0.0f));

			font.draw_text(gc, 32, 350, "Showing Texture with an offset to the floating point color component");
			draw_image(gc, image_rgb32f, 32.0f, 400.0f, shader, color_offset);

			KeepAlive::process(0);
			window.flip(1);
		}

	}
	catch(Exception &exception)
	{
		// Create a console window for text-output if not available
		ConsoleWindow console("Console", 80, 160);
		Console::write_line("Exception caught: " + exception.get_message_and_stack_trace());
		console.display_close_message();

		return -1;
	}
	return 0;
}

// A key was pressed
void App::on_input_up(const InputEvent &key, const InputState &state)
{
	if(key.id == KEY_ESCAPE)
	{
		quit = true;
	}

}

// The window was closed
void App::on_window_close()
{
	quit = true;
}

Texture App::create_rgb32f(GraphicContext &gc, const Size &image_size, float dynamic_range_start, float dynamic_range_end)
{
	PixelBuffer buffer(image_size.width, image_size.height, cl_rgb32f);
	float *data = (float *) buffer.get_data();

	for (int ycnt = 0; ycnt < image_size.height; ycnt++)
	{
		ColorHSVf color_hsv( (float) ycnt * 360.0f / (float) image_size.height, 1.0f, 1.0f, 1.0f );
		Colorf color(color_hsv);

		for (int xcnt = 0; xcnt < image_size.width; xcnt++)
		{
			float scale = dynamic_range_start + ( ((float) xcnt) * (dynamic_range_end - dynamic_range_start) ) / ( (float) image_size.width );
			float red = color.r * scale;
			float green = color.g * scale;
			float blue = color.b * scale;
			*(data++) = blue;
			*(data++) = green;
			*(data++) = red;

		}
	}

	Texture texture(gc, image_size.width, image_size.height, cl_rgb32f);
	texture.set_image(buffer);

	return texture;
}

void App::draw_image(GraphicContext &gc, Texture &image, float xpos, float ypos, ProgramObject &program_object, Vec4f &draw_color_offset)
{
	program_object.set_uniform1i("SourceTexture", 0);
	program_object.set_uniform4f("color_offset", draw_color_offset);

	gc.set_texture(0, image);
	gc.set_program_object(program_object, cl_program_matrix_modelview_projection);

	draw_texture(gc, Rectf(xpos, ypos, Sizef(image.get_width(), image.get_height())), Colorf::white, Rectf(0.0f, 0.0f, 1.0f, 1.0f));

	gc.reset_program_object();
	gc.reset_texture(0);

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

