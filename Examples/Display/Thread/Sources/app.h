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
*/

#pragma once

// Choose the target renderer
//#define USE_OPENGL_2
//#define USE_OPENGL_1
#define USE_SOFTWARE_RENDERER

class App
{
public:
	int start(const std::vector<CL_String> &args);
	void window_close();

private:
	void render_mandelbrot(float mandelbrot_scale, unsigned char *dest);
	void on_input_up(const CL_InputEvent &key, const CL_InputState &state);
	void worker_thread();

	bool quit;
	volatile bool crashed_flag;

	static const int texture_size = 256;

	CL_Texture texture_buffers[2];
	CL_PixelBuffer pixel_buffers[2];

	int texture_buffers_offset;
	int pixel_buffers_offset;
	volatile bool worker_thread_complete;

	CL_PixelBuffer *pixelbuffer_write;	// Pixelbuffer that is written to
	CL_PixelBuffer *pixelbuffer_completed;	// Completed pixelbuffer
	CL_Texture *texture_write;	// Texture that is written to
	CL_Texture *texture_completed;	// Completed texture

	CL_Event worker_thread_activate_event;
	CL_Event worker_thread_stop_event;

	// Worker thead variables
	CL_Mutex worker_thread_mutex;
	float scale;
	unsigned char *dest_pixels;	// Used by the worker thread to contain where to write the pixels to
};

