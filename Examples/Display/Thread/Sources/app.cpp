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
#if defined(WIN32) && !defined(__MINGW32__)
	clan::D3DTarget::set_current();
#else
	clan::OpenGLTarget::set_current();
#endif

	clan::DisplayWindowDescription description;
	description.set_title("Thread Example");
	description.set_size(clan::Size(1024, 768), true);

	window = clan::DisplayWindow(description);
	keyboard = window.get_keyboard();
	canvas = clan::Canvas(window);

	sc.connect(window.get_keyboard().sig_key_up(), clan::bind_member(this, &App::on_input_up));
	sc.connect(window.sig_window_close(), clan::bind_member(this, &App::window_close));

	// Load the font
	font = clan::Font("tahoma", 32);

	// Create the initial textures
	texture_buffers[0] = clan::Texture2D(canvas, texture_size, texture_size);
	texture_buffers[1] = clan::Texture2D(canvas, texture_size, texture_size);

	// Create the initial pixelbuffers
	pixel_buffers[0] = clan::PixelBuffer(texture_size, texture_size, clan::tf_rgba8);
	pixel_buffers[1] = clan::PixelBuffer(texture_size, texture_size, clan::tf_rgba8);

	// Initially clear the textures, so they are filled with a "Calculating..." message
	clan::FrameBuffer framebuffer(canvas);
	framebuffer.attach_color(0, texture_buffers[0]);
	clan::Canvas canvas_fb( canvas, framebuffer );
	canvas_fb.clear();
	font.draw_text(canvas_fb, 32, 96, "Calculating...");
	canvas_fb.flush();
	framebuffer.attach_color(0, texture_buffers[1]);
	canvas_fb = clan::Canvas( canvas, framebuffer );
	canvas_fb.clear();
	font.draw_text(canvas_fb, 32, 96, "Calculating...");
	canvas_fb.flush();

	// Setup the initial texture double buffering variables

	texture_buffers_offset = 0;
	pixel_buffers_offset = 0;
	
	texture_write = &texture_buffers[0];
	texture_completed = &texture_buffers[1];
	pixelbuffer_write = &pixel_buffers[0];
	pixelbuffer_completed = &pixel_buffers[1];

	thread = std::thread(&App::worker_thread, this);

	// Main loop
	last_time = clan::System::get_time();
	last_mandelbrot_time = clan::System::get_time();

}

bool App::update()
{
	framerate_counter.frame_shown();

	// Calculate timings
	uint64_t current_time = clan::System::get_time();
	float time_delta_ms = (float) (current_time - last_time);
	last_time = current_time;

	angle += time_delta_ms / 50.0f;
	while(angle > 360.0f)
		angle-=360.0f;

	canvas.clear();
			
	// If the pixel buffer was uploaded on the last frame, double buffer it
	if (texture_write_active)
	{
		texture_write_active = false;
		if (texture_buffers_offset == 0)
		{
			texture_buffers_offset = 1;
			texture_write = &texture_buffers[1];
			texture_completed = &texture_buffers[0];
		}
		else
		{
			texture_buffers_offset = 0;
			texture_write = &texture_buffers[0];
			texture_completed = &texture_buffers[1];
		}
	}

	// Wait for pixel buffer completion
	std::unique_lock<std::mutex> lock(thread_mutex);
	if (thread_complete_flag == true)
	{
		thread_complete_flag = false;
		pixelbuffer_write->unlock();

		texture_write->set_subimage(canvas, 0, 0, *pixelbuffer_write, pixelbuffer_write->get_size());
		texture_write_active = true;
		// Note the worker thread will start on the other pixelbuffer straight away, in the next "if" statement
	}

	// Start a new transfer when required
	if ((thread_start_flag == false))
	{
		worker_thread_framerate_counter.frame_shown();

		// Swap the pixelbuffer's
		if (pixel_buffers_offset == 0)
		{
			pixel_buffers_offset = 1;
			pixelbuffer_write = &pixel_buffers[1];
			pixelbuffer_completed = &pixel_buffers[0];
		}
		else
		{
			pixel_buffers_offset = 0;
			pixelbuffer_write = &pixel_buffers[0];
			pixelbuffer_completed = &pixel_buffers[1];
		}

		pixelbuffer_write->lock(canvas, clan::access_write_only);
		dest_pixels = (unsigned char *) pixelbuffer_write->get_data();
		thread_start_flag = true;
		thread_complete_flag = false;

		// Adjust the mandelbrot scale
		float mandelbrot_time_delta_ms = (float) (current_time - last_mandelbrot_time);
		last_mandelbrot_time = current_time;
		scale -= scale * mandelbrot_time_delta_ms / 1000.0f;
		if (scale <= 0.001f)
			scale = 4.0f;

		thread_worker_event.notify_all();
	}
	pixelbuffer_write->unlock();

	// Draw rotating mandelbrot
	canvas.set_transform(clan::Mat4f::translate(canvas.get_width()/2, canvas.get_height()/2, 0.0f) * clan::Mat4f::rotate(clan::Angle(angle, clan::angle_degrees), 0.0f, 0.0f, 1.0f));
	clan::Image image(*texture_completed, clan::Size(texture_size, texture_size));
	image.draw( canvas, -texture_size/2, -texture_size/2 );

	canvas.set_transform(clan::Mat4f::identity());
		
	// Draw FPS
	std::string fps;
	fps = std::string(clan::string_format("Main Loop %1 fps", framerate_counter.get_framerate()));
	font.draw_text(canvas, 16, canvas.get_height()-16-2, fps, clan::Colorf(1.0f, 1.0f, 0.0f, 1.0f));
	fps = std::string(clan::string_format("Worker Thread %1 fps", worker_thread_framerate_counter.get_framerate()));
	font.draw_text(canvas, 16, canvas.get_height()-64-2, fps, clan::Colorf(1.0f, 1.0f, 0.0f, 1.0f));

	// Draw worker thread crashed message
	if (thread_crashed_flag)
		font.draw_text(canvas, 16, 32, "WORKER THREAD CRASHED");
	
	window.flip(0);

	return !quit;
}

App::~App()
{
	std::unique_lock<std::mutex> lock(thread_mutex);
	thread_exit_flag = true;
	lock.unlock();
	thread_worker_event.notify_all();
	thread.join();
}

void App::window_close()
{
	quit = true;
}

void App::render_mandelbrot(float mandelbrot_scale, unsigned char *dest)
{
	unsigned char *pptr = dest;

	const int max_iteration = 254;

	for (int ycnt =0; ycnt < texture_size; ycnt++)
	{
		for (int xcnt =0; xcnt < texture_size; xcnt++)
		{
			double x0 = ((double) (xcnt - texture_size/2)) / (double) texture_size;
			double y0 = ((double) (ycnt - texture_size/2)) / (double) texture_size;

			x0 *= mandelbrot_scale;
			y0 *= mandelbrot_scale;

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

			*(pptr++) = iteration;
			*(pptr++) = iteration/2;
			*(pptr++) = iteration/2;
			*(pptr++) = 0xFF;
		}
	}

}

void App::on_input_up(const clan::InputEvent &key)
{
	if(key.id == clan::keycode_escape)
	{
		quit = true;
	}

}

void App::worker_thread()
{
	try
	{
		while(true)
		{
		
			std::unique_lock<std::mutex> lock(thread_mutex);
			thread_worker_event.wait(lock, [&]() { return thread_start_flag || thread_exit_flag; });
			if (thread_exit_flag)
				break;
			thread_start_flag = false;
			lock.unlock();

			// Write to texture
			if (dest_pixels)
				render_mandelbrot(scale, dest_pixels);

			//throw clan::Exception("Bang!");	// <--- Use this to test the application handles exceptions in threads

			lock.lock();
			thread_complete_flag = true;
		}
	}
	catch(clan::Exception &)
	{
		std::unique_lock<std::mutex> lock(thread_mutex);
		thread_crashed_flag = true;
	}


}

