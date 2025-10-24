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
#include "framerate_counter.h"

App::App() : quit(false)
{
}

// The start of the Application
int App::start(const std::vector<CL_String> &args)
{
	CL_DisplayWindowDescription win_desc;
	win_desc.set_allow_resize(true);
	win_desc.set_title("PixelBuffer Example");
	win_desc.set_size(CL_Size( 600, 630 ), false);

	CL_DisplayWindow window(win_desc);
	CL_Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close);
	CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

	CL_GraphicContext gc = window.get_gc();

	CL_PixelBuffer tux("../../3D/Clan3D/Resources/tux.png");

	CL_PixelBuffer cpu_buffer = tux.copy();

	const int num_gpu_buffers = 2;
	CL_PixelBuffer gpu_buffer[num_gpu_buffers];
	for (int cnt=0; cnt < num_gpu_buffers; cnt++)
	{
		// Note - This example only uses CL_PIXEL_UNPACK_BUFFER_ARB, it does not use CL_PIXEL_PACK_BUFFER_ARB
		gpu_buffer[cnt] = CL_PixelBuffer(gc, tux);
	}

	const int num_textures = 2;
	CL_Texture textures[num_textures];
	for (int cnt=0; cnt < num_textures; cnt++)
	{
		textures[cnt] = CL_Texture(gc, tux.get_width(), tux.get_height(), cl_rgb);
	}

	CL_Font font(gc, "Tahoma", 24);

	FramerateCounter framerate_counter;

	int gpu_buffer_cycle = 0;
	int texture_cycle = 0;
	cpu_active = true;

	while (!quit)
	{
		framerate_counter.frame_shown();
		gc.clear(CL_Colorf(0.0f,0.0f,0.2f));

		// Modify the pixel buffer
		read_write_pixel_buffer(tux);

		// Control the texture buffering
		int texture_cycle_first = texture_cycle;
		int texture_cycle_second = texture_cycle + 1;
		if (texture_cycle_second >= num_textures)
			texture_cycle_second -= num_textures;

		// Control the gpu buffering
		int gpu_buffer_cycle_first = gpu_buffer_cycle;
		int gpu_buffer_cycle_second = gpu_buffer_cycle + 1;
		if (gpu_buffer_cycle_second >= num_gpu_buffers)
			gpu_buffer_cycle_second -= num_gpu_buffers;

		// Always draw the same text, to ensure matching speed calculation
		if (cpu_active)
		{
			font.draw_text(gc, 8, 24, "GPU Pixel Buffer Object", CL_Colorf(0.0f, 0.0f, 0.2f, 1.0f));
			font.draw_text(gc, 8, 24, "CPU Pixel Buffer", CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f));
			draw_cpu(gc, cpu_buffer, tux, textures[texture_cycle_first], textures[texture_cycle_second]);
		}
		else
		{
			font.draw_text(gc, 8, 24, "CPU Pixel Buffer", CL_Colorf(0.0f, 0.0f, 0.2f, 1.0f));
			font.draw_text(gc, 8, 24, "GPU Pixel Buffer Object", CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f));
			draw_gpu(gc, gpu_buffer[gpu_buffer_cycle_first], gpu_buffer[gpu_buffer_cycle_second], tux, textures[texture_cycle_first], textures[texture_cycle_second]);
		}

		font.draw_text(gc, 8, gc.get_height() - 16, "Press any key to toggle method");

		texture_cycle = texture_cycle_second;
		gpu_buffer_cycle = gpu_buffer_cycle_second;

		CL_String fps(cl_format("%1 fps", framerate_counter.get_framerate()));
		font.draw_text(gc, gc.get_width() - 100, gc.get_height()-16, fps, CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f));

		CL_KeepAlive::process(0);
		// Use flip(1) to lock the fps
		window.flip(0);
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

	cpu_active = !cpu_active;
}

// The window was closed
void App::on_window_close()
{
	quit = true;
}

void App::upload_pixel_buffer(CL_PixelBuffer &pbuff_source, CL_PixelBuffer &pbuff_dest)
{
	pbuff_source.lock(cl_access_read_only);
	pbuff_dest.upload_data(CL_Rect(0,0, pbuff_source.get_size()), pbuff_source.get_data());
	pbuff_source.unlock();
}

void App::draw_cpu(CL_GraphicContext &gc, CL_PixelBuffer &cpu_buffer, CL_PixelBuffer &tux, CL_Texture &texture_to_write_into, CL_Texture &texture_to_draw)
{
	upload_pixel_buffer(tux, cpu_buffer);
	texture_to_write_into.set_subimage(0, 0, cpu_buffer, cpu_buffer.get_size());

	draw_texture(gc, texture_to_draw, 32, 32);
}

void App::draw_gpu(CL_GraphicContext &gc, CL_PixelBuffer &gpu_buffer_to_write_into, CL_PixelBuffer &gpu_buffer_to_draw, CL_PixelBuffer &tux, CL_Texture &texture_to_write_into, CL_Texture &texture_to_draw)
{
	upload_pixel_buffer(tux, gpu_buffer_to_write_into);
	texture_to_write_into.set_subimage(0, 0, gpu_buffer_to_draw, gpu_buffer_to_draw.get_size());
	draw_texture(gc, texture_to_draw, 32, 32);
}

void App::draw_texture(CL_GraphicContext &gc, CL_Texture &texture, int xpos, int ypos)
{
	gc.set_texture(0, texture);
	CL_Draw::texture(gc, CL_Rectf( (float) xpos, (float) ypos, CL_Sizef(512.0f, 512.0f)));
	gc.reset_texture(0);
}

void App::read_write_pixel_buffer(CL_PixelBuffer &pbuff)
{
	unsigned int time_now = CL_System::get_time();
	pbuff.lock(cl_access_read_write);
	if (pbuff.get_format() != cl_rgb8)
		throw CL_Exception("Expected the format to be RGB8");
	unsigned char *dptr = (unsigned char *) pbuff.get_data();

	// Only write to a portion of the pixel buffer - To keep up the example speed
	const int write_fraction = 2;

	const int box_width = pbuff.get_width() / write_fraction;
	const int box_height = pbuff.get_height() / write_fraction;
	int next_line = (pbuff.get_width() - box_width) * pbuff.get_bytes_per_pixel();

	dptr += pbuff.get_bytes_per_pixel() * pbuff.get_width() * ( (pbuff.get_height() - box_height) / 2);
	dptr += pbuff.get_bytes_per_pixel() * (pbuff.get_width() - box_width) / 2;

	for (int ycnt = 0; ycnt < box_height; ycnt++)
	{
		for (int xcnt = 0; xcnt < box_width; xcnt++)
		{
			unsigned char blue = dptr[0];
			unsigned char red = dptr[1];
			unsigned char green = dptr[2];
			blue =  red + xcnt + (time_now / 10);
			green = red + xcnt*ycnt/512 + (time_now / 20);
			*(dptr++) = blue;
			*(dptr++) = red;
			*(dptr++) = green;
		}
		dptr += next_line;
	}
	pbuff.unlock();
}
