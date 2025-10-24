/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
		win_desc.set_title("HDR Example");
		win_desc.set_size(CL_Size( 600, 630 ), false);

		CL_DisplayWindow window(win_desc);
		CL_Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close);
		CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

		CL_GraphicContext gc = window.get_gc();

		const float dynamic_range_start = 0.0f;
		const float dynamic_range_end = 2.0f;
		const CL_Size image_size(gc.get_width() - 64, 128);

		CL_Image image_rgb8 = create_rgb8(gc, image_size, dynamic_range_start, dynamic_range_end);
		CL_Image image_rgb32f = create_rgb32f(gc, image_size, dynamic_range_start, dynamic_range_end);

		CL_Font font(gc, "Tahoma", 24);

		float ball_offset_a = 0.0f;
		float ball_offset_b = 0.0f;
		unsigned int time_last = CL_System::get_time();

		while (!quit)
		{
			unsigned int time_now = CL_System::get_time();
			time_delta = ((float) time_now - time_last) / 1000.0f;
			time_last = time_now;

			gc.clear(CL_Colorf(0.5f,0.5f,0.5f));

			draw_image(gc, image_rgb8, 32.0f, 32.0f, ball_offset_a);
			draw_image(gc, image_rgb32f, 32.0f, 200.0f, ball_offset_b);

			CL_KeepAlive::process(0);
			// Use flip(1) to lock the fps
			window.flip(1);
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
	if(key.id == CL_KEY_ESCAPE)
	{
		quit = true;
	}

}

// The window was closed
void App::on_window_close()
{
	quit = true;
}

CL_Image App::create_rgb8(CL_GraphicContext &gc, const CL_Size &image_size, float dynamic_range_start, float dynamic_range_end)
{
	CL_PixelBuffer buffer(image_size.width, image_size.height, cl_rgb8);
	unsigned char *data = (unsigned char *) buffer.get_data();

	for (int ycnt = 0; ycnt < image_size.height; ycnt++)
	{
		CL_ColorHSVf color_hsv( (float) ycnt * 360.0f / (float) image_size.height, 1.0f, 1.0f, 1.0f );
		CL_Colorf color(color_hsv);

		for (int xcnt = 0; xcnt < image_size.width; xcnt++)
		{
			float scale = dynamic_range_start + ( ((float) xcnt) * (dynamic_range_end - dynamic_range_start) ) / ( (float) image_size.width );
			float red = color.r * scale;
			float green = color.g * scale;
			float blue = color.b * scale;
			if (red < 0.0) red = 0.0f;
			if (red > 1.0) red = 1.0f;
			if (green < 0.0) green = 0.0f;
			if (green > 1.0) green = 1.0f;
			if (blue < 0.0) blue = 0.0f;
			if (blue > 1.0) blue = 1.0f;

			*(data++) = (int) (blue * 255.0f);
			*(data++) = (int) (green * 255.0f);
			*(data++) = (int) (red * 255.0f);

		}
	}

	return CL_Image(gc, buffer, image_size);
}

CL_Image App::create_rgb32f(CL_GraphicContext &gc, const CL_Size &image_size, float dynamic_range_start, float dynamic_range_end)
{
	CL_PixelBuffer buffer(image_size.width, image_size.height, cl_rgb32f);
	float *data = (float *) buffer.get_data();

	for (int ycnt = 0; ycnt < image_size.height; ycnt++)
	{
		CL_ColorHSVf color_hsv( (float) ycnt * 360.0f / (float) image_size.height, 1.0f, 1.0f, 1.0f );
		CL_Colorf color(color_hsv);

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

	return CL_Image(gc, buffer, image_size);
}

void App::draw_image(CL_GraphicContext &gc, CL_Image &image, float xpos, float ypos, float &offset_x)
{
	float range_x = image.get_width() * 2.0f;
	offset_x += time_delta * 200.0f;
	if (offset_x > range_x)
	{
		offset_x -= range_x;
		if (offset_x > range_x)
			offset_x = 0;
	}

	image.draw(gc, xpos, ypos);

	//float adjust_xpos = (offset_x + (image.get_width() / 2.0f)) / 2.0f;
	float adjust_xpos = offset_x - image.get_width();
	if (adjust_xpos < 0.0f)
		adjust_xpos = -adjust_xpos;

	CL_Draw::circle(gc, xpos + adjust_xpos, ypos + 32.0f, 32.0f, CL_Colorf(0.0f, 0.0f, 0.0f, 0.5f));


}
