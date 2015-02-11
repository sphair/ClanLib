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

#pragma once

class App
{
public:
	~App();
	int start(const std::vector<std::string> &args);
	void window_close();

private:
	void render_mandelbrot(float mandelbrot_scale, unsigned char *dest);
	void on_input_up(const clan::InputEvent &key);
	void worker_thread();

	bool quit;

	static const int texture_size = 256;

	clan::Texture2D texture_buffers[2];
	clan::PixelBuffer pixel_buffers[2];

	int texture_buffers_offset;
	int pixel_buffers_offset;

	clan::PixelBuffer *pixelbuffer_write;	// Pixelbuffer that is written to
	clan::PixelBuffer *pixelbuffer_completed;	// Completed pixelbuffer
	clan::Texture2D *texture_write;	// Texture that is written to
	clan::Texture2D *texture_completed;	// Completed texture

	std::thread thread;

	bool thread_exit_flag = false;
	bool thread_start_flag = false;
	bool thread_complete_flag = false;
	bool thread_crashed_flag = false;
	std::mutex thread_mutex;
	std::condition_variable thread_worker_event;

	// Worker thead variables
	float scale;
	unsigned char *dest_pixels;	// Used by the worker thread to contain where to write the pixels to
};

