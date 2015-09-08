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
**    Mark Page
*/

#include "precomp.h"
#include "app.h"

clan::ApplicationInstance<App> clanapp;

App::App()
{
	// We support all display targets, in order listed here
#ifdef WIN32
	//FIXME
	//clan::D3DTarget::set_current();
#endif
	clan::OpenGLTarget::set_current();

	clan::DisplayWindowDescription win_desc;
	win_desc.set_allow_resize(true);
	win_desc.set_title("PixelBuffer Example");
	win_desc.set_size(clan::Size( 600, 630 ), false);

	window = clan::DisplayWindow(win_desc);
	sc.connect(window.sig_window_close(), clan::bind_member(this, &App::on_window_close));
	sc.connect(window.get_ic().get_keyboard().sig_key_up(), clan::bind_member(this, &App::on_input_up));
	canvas = clan::Canvas(window);

	tux = clan::PixelBuffer("../../3D/Object3D/Resources/tux.png");

	cpu_buffer = tux.copy();

	for (int cnt=0; cnt < num_gpu_buffers; cnt++)
	{
		// Note - This example only uses PIXEL_UNPACK_BUFFER_ARB, it does not use PIXEL_PACK_BUFFER_ARB
		gpu_buffer[cnt] = clan::TransferTexture(canvas, tux);
	}

	for (int cnt=0; cnt < num_textures; cnt++)
	{
		textures[cnt] = clan::Texture2D(canvas, tux.get_width(), tux.get_height(), clan::tf_rgba8);
	}

	font = clan::Font("Tahoma", 24);

	game_time.reset();
}

bool App::update()
{
	game_time.update();
	canvas.clear(clan::Colorf(0.0f,0.0f,0.2f));

	// Modify the pixel buffer
	read_write_pixel_buffer(canvas, tux);

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
		font.draw_text(canvas, 8, 24, "GPU Transfer Texture Object", clan::Colorf(0.0f, 0.0f, 0.2f, 1.0f));
		font.draw_text(canvas, 8, 24, "CPU Pixel Buffer", clan::Colorf(1.0f, 1.0f, 1.0f, 1.0f));
		draw_cpu(canvas, cpu_buffer, tux, textures[texture_cycle_first], textures[texture_cycle_second]);
	}
	else
	{
		font.draw_text(canvas, 8, 24, "CPU Pixel Buffer", clan::Colorf(0.0f, 0.0f, 0.2f, 1.0f));
		font.draw_text(canvas, 8, 24, "GPU Transfer Texture Object", clan::Colorf(1.0f, 1.0f, 1.0f, 1.0f));
		draw_gpu(canvas, gpu_buffer[gpu_buffer_cycle_first], gpu_buffer[gpu_buffer_cycle_second], tux, textures[texture_cycle_first], textures[texture_cycle_second]);
	}

	font.draw_text(canvas, 8, canvas.get_height() - 16, "Press any key to toggle method");

	texture_cycle = texture_cycle_second;
	gpu_buffer_cycle = gpu_buffer_cycle_second;

	std::string fps(clan::string_format("%1 fps", clan::StringHelp::float_to_text(game_time.get_updates_per_second(), 1)));
	font.draw_text(canvas, canvas.get_width() - 100, canvas.get_height()-16, fps, clan::Colorf(1.0f, 1.0f, 1.0f, 1.0f));

	// Use flip(1) to lock the fps
	window.flip(0);

	return !quit;
}

// A key was pressed
void App::on_input_up(const clan::InputEvent &key)
{
	if(key.id == clan::keycode_escape)
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

void App::upload_pixel_buffer(clan::Canvas &canvas, clan::PixelBuffer &pbuff_source, clan::PixelBuffer &pbuff_dest)
{
	pbuff_source.lock(canvas, clan::access_read_only);
	pbuff_dest.upload_data(canvas, clan::Rect(0,0, pbuff_source.get_size()), pbuff_source.get_data());
	pbuff_source.unlock();
}

void App::draw_cpu(clan::Canvas &canvas, clan::PixelBuffer &cpu_buffer, clan::PixelBuffer &tux, clan::Texture2D &texture_to_write_into, clan::Texture2D &texture_to_draw)
{
	upload_pixel_buffer(canvas, tux, cpu_buffer);
	texture_to_write_into.set_subimage(canvas, 0, 0, cpu_buffer, cpu_buffer.get_size());

	draw_texture(canvas, texture_to_draw, 32, 32);
}

void App::draw_gpu(clan::Canvas &canvas, clan::PixelBuffer &gpu_buffer_to_write_into, clan::PixelBuffer &gpu_buffer_to_draw, clan::PixelBuffer &tux, clan::Texture2D &texture_to_write_into, clan::Texture2D &texture_to_draw)
{
	upload_pixel_buffer(canvas, tux, gpu_buffer_to_write_into);
	texture_to_write_into.set_subimage(canvas, 0, 0, gpu_buffer_to_draw, gpu_buffer_to_draw.get_size());
	draw_texture(canvas, texture_to_draw, 32, 32);
}

void App::draw_texture(clan::Canvas &canvas, clan::Texture2D &texture, int xpos, int ypos)
{
	clan::Image image(texture, texture.get_size());
	image.draw(canvas, clan::Rectf( (float) xpos, (float) ypos, clan::Sizef(512.0f, 512.0f)));
}

void App::read_write_pixel_buffer(clan::Canvas &canvas, clan::PixelBuffer &pbuff)
{
	uint64_t time_now = clan::System::get_time();
	pbuff.lock(canvas, clan::access_read_write);
	if (pbuff.get_format() != clan::tf_rgba8)
		throw clan::Exception("Expected the format to be RGBA8");
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
			*(dptr++) = 0xFF;
		}
		dptr += next_line;
	}
	pbuff.unlock();
}
