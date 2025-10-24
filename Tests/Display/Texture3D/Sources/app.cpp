/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
int App::start(const std::vector<CL_String> &args)
{
	try
	{
		CL_DisplayWindowDescription win_desc;
		win_desc.set_allow_resize(true);
		win_desc.set_title("3D Texture Example");
		win_desc.set_size(CL_Size( 800, 600 ), false);

		CL_DisplayWindow window(win_desc);
		CL_Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close);
		CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

		CL_GraphicContext gc = window.get_gc();

		const int texture_width = 128;
		const int texture_height = 128;
		const int texture_depth = 64;

	// Create a mandelbrot set
		CL_PixelBuffer pixelbuffer(texture_width, texture_height*texture_depth, cl_rgba8);
		int *data = (int *) pixelbuffer.get_data();

		const int max_iteration = 1000;
		int color_buffer[max_iteration+1];
		for (int cnt=0; cnt<max_iteration; cnt++)
		{
			float hue = (float) ((cnt * 10) % 360);		
			CL_ColorHSVf hsv(hue, 0.5f, 1.0f, 1.0f);
			color_buffer[cnt] = CL_Color(hsv).color;
		}
		color_buffer[max_iteration] = 0xFF;

		double scale_counter = 1.0;
		for (int zcnt =0; zcnt < texture_depth; zcnt++)
		{
			double scale = 1.0 / scale_counter;
			for (int ycnt =0; ycnt < texture_height; ycnt++)
			{
				for (int xcnt =0; xcnt < texture_width; xcnt++)
				{
					double x0 = ((double) (xcnt - texture_width/2)) / (double) texture_width;
					double y0 = ((double) (ycnt - texture_height/2)) / (double) texture_height;

					x0 *= scale;
					y0 *= scale;

					x0 += -0.639;
					y0 += 0.41;

					double x = 0.0f;
					double y = 0.0f;

					int iteration = 0;
					while ( (x*x + y*y) <= 4.0 )
					{
						double xtemp = x*x - y*y + x0;
					    y = 2*x*y + y0;
					    x = xtemp;
						++iteration;
						if ((++iteration) == max_iteration)
							break;
					}

					*(data++) = color_buffer[iteration];
				}
			}
			scale_counter += scale_counter/8.0f;

		}
	// Create the 3D texture
		CL_Texture texture(gc, texture_width, texture_height, texture_depth);
		texture.set_image(pixelbuffer);

		CL_Font font(gc, "tahoma", 20);
		unsigned int time_last = CL_System::get_time();

		float texture_zpos = 0.0f;
		while (!quit)
		{
			unsigned int time_now = CL_System::get_time();
			float time_diff = (float) (time_now - time_last);
			time_last = time_now;

			gc.clear(CL_Colorf(0.0f,0.0f,0.4f));

			texture_zpos += time_diff/500.0f;
			if (texture_zpos > (float) texture_depth)
				texture_zpos = 0.0f;

			CL_String text( cl_format("Depth Position = %1 pixels", texture_zpos));
			font.draw_text(gc, 32, 32, text);

			gc.set_texture(0, texture);

			draw_texture(gc, 
				CL_Rectf(100.0f, 200.0f, CL_Sizef( (float) texture_width, (float) texture_height)),
				CL_Colorf::white,
				texture_zpos / (float) texture_depth);

			gc.reset_texture(0);

			gc.pop_modelview();

			window.flip(1);

			CL_KeepAlive::process();
		}

	}
	catch(CL_Exception &exception)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console", 80, 160);
		CL_Console::write_line("Exception caught: " + exception.get_message_and_stack_trace());
		console.display_close_message();

		return -1;
	}
	return 0;
}

// A key was pressed
void App::on_input_up(const CL_InputEvent &key, const CL_InputState &state)
{
	switch (key.id)
	{
		case CL_KEY_ESCAPE:
			quit = true;
			break;
	}

}

// The window was closed
void App::on_window_close()
{
	quit = true;
}


void App::draw_texture(CL_GraphicContext &gc, const CL_Rectf &rect, const CL_Colorf &color, float texture_zpos)
{
	CL_Vec2f positions[6] =
	{
		CL_Vec2f(rect.left, rect.top),
		CL_Vec2f(rect.right, rect.top),
		CL_Vec2f(rect.left, rect.bottom),
		CL_Vec2f(rect.right, rect.top),
		CL_Vec2f(rect.left, rect.bottom),
		CL_Vec2f(rect.right, rect.bottom)
	};

	CL_Vec3f tex1_coords[6] =
	{
		CL_Vec3f(0.0f, 0.0f, texture_zpos),
		CL_Vec3f(1.0f, 0.0f, texture_zpos),
		CL_Vec3f(0.0f, 1.0f, texture_zpos),
		CL_Vec3f(1.0f, 0.0f, texture_zpos),
		CL_Vec3f(0.0f, 1.0f, texture_zpos),
		CL_Vec3f(1.0f, 1.0f, texture_zpos),
	};

	CL_PrimitivesArray prim_array(gc);
	prim_array.set_attributes(0, positions);
	prim_array.set_attribute(1, color);
	prim_array.set_attributes(2, tex1_coords);
	gc.set_program_object(cl_program_single_texture);
	gc.reset_program_object();
	gc.draw_primitives(cl_triangles, 6, prim_array);
	gc.reset_program_object();
}
