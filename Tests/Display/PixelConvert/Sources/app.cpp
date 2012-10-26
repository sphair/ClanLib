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
**    Magnus Norddahl
**    Mark Page
*/

#include "precomp.h"
#include "app.h"

// The start of the Application
int App::start(const std::vector<CL_String> &args)
{
	quit = false;

	// Set the window
	CL_DisplayWindowDescription desc;
	desc.set_title("ClanLib App Example");
	desc.set_size(CL_Size(1200, 500), true);
	desc.set_allow_resize(true);

	CL_DisplayWindow window(desc);

	// Connect the Window close event
	CL_Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close);

	// Connect a keyboard handler to on_key_up()
	CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

	// Get the graphic context
	CL_GraphicContext gc = window.get_gc();

	// Load a sprite from a png-file
	tux = CL_PixelBuffer("Resources/tux.png");

	tux_original = CL_Image(gc, tux, tux.get_size());
	current_format = cl_rgba8;

	//current_format = cl_rgba8;
	//current_format = cl_rgb8;
	//current_format = cl_bgra8;
	//current_format = cl_bgr8;
	//current_format = cl_r8;
	//current_format = cl_r8_snorm;
	//current_format = cl_r16;
	//current_format = cl_r16_snorm;
	//current_format = cl_rg8;
	//current_format = cl_rg8_snorm;
	//current_format = cl_rg16;
	//current_format = cl_rg16_snorm;
	//current_format = cl_r3_g3_b2;
	//current_format = cl_rgb4;
	//current_format = cl_rgb5;
	//current_format = cl_rgb8_snorm;
	//current_format = cl_rgb10;
	//current_format = cl_rgb12;		// XX
	//current_format = cl_rgb16;
	//current_format = cl_rgb16_snorm;
	//current_format = cl_rgba2;		// XX
	//current_format = cl_rgba4;
	//current_format = cl_rgb5_a1;
	//current_format = cl_rgba8_snorm;
	//current_format = cl_rgb10_a2;
	//current_format = cl_rgba12;		// X
	//current_format = cl_rgba16;
	//current_format = cl_rgba16_snorm;
	//current_format = cl_srgb8;
	//current_format = cl_srgb8_alpha8;
	//current_format = cl_r16f;
	//current_format = cl_rg16f;
	//current_format = cl_rgb16f;
	//current_format = cl_rgba16f;
	//current_format = cl_r32f;
	current_format = cl_rg32f;
	//current_format = cl_rgb32f;
	//current_format = cl_rgba32f;
	//current_format = cl_r11f_g11f_b10f;	// X
	//current_format = cl_rgb9_e5;	// X
	//current_format = cl_r8i;
	//current_format = cl_r8ui;
	//current_format = cl_r16i;
	//current_format = cl_r16ui;
	//current_format = cl_r32i;
	//current_format = cl_r32ui;
	//current_format = cl_rg8i;
	//current_format = cl_rg8ui;
	//current_format = cl_rg16i;
	//current_format = cl_rg16ui;
	//current_format = cl_rg32i;
	//current_format = cl_rg32ui;
	//current_format = cl_rgb8i;
	//current_format = cl_rgb8ui;
	//current_format = cl_rgb16i;
	//current_format = cl_rgb16ui;
	//current_format = cl_rgb32i;
	//current_format = cl_rgb32ui;
	//current_format = cl_rgba8i;
	//current_format = cl_rgba8ui;
	//current_format = cl_rgba16i;
	//current_format = cl_rgba16ui;
	//current_format = cl_rgba32i;
	//current_format = cl_rgba32ui;
	//current_format = cl_depth_component16;
	//current_format = cl_depth_component24;
	//current_format = cl_depth_component32;
	//current_format = cl_depth_component32f;
	//current_format = cl_depth24_stencil8;
	//current_format = cl_depth32f_stencil8;
	//current_format = cl_compressed_red;
	//current_format = cl_compressed_rg;
	//current_format = cl_compressed_rgb;
	//current_format = cl_compressed_rgba;
	//current_format = cl_compressed_srgb;
	//current_format = cl_compressed_srgb_alpha;
	//current_format = cl_compressed_red_rgtc1;
	//current_format = cl_compressed_signed_red_rgtc1;
	//current_format = cl_compressed_rg_rgtc2;
	//current_format = cl_compressed_signed_rg_rgtc2;
	//current_format = cl_compressed_rgb_s3tc_dxt1;
	//current_format = cl_compressed_rgba_s3tc_dxt1;
	//current_format = cl_compressed_rgba_s3tc_dxt3;
	//current_format = cl_compressed_rgba_s3tc_dxt5;
	//current_format = cl_compressed_srgb_s3tc_dxt1;
	//current_format = cl_compressed_srgb_alpha_s3tc_dxt1;
	//current_format = cl_compressed_srgb_alpha_s3tc_dxt3;
	//current_format = cl_compressed_srgb_alpha_s3tc_dxt5;


	display_gc = &gc;
	update_images();

	CL_Font font(gc, "tahoma", 24);

	// Run until someone presses escape
	while (!quit)
	{

		gc.clear(CL_Colorf(0.0f,0.0f,0.2f));

		tux_original.draw(gc, 0, 0);
		tux_convert.draw(gc, 400, 0);
		tux_unconvert.draw(gc, 800, 0);

		font.draw_text(gc, 10, 400, "cl_rgba8");

		if (gl_supported)
		{
			font.draw_text(gc, 410, 400, "converted");
		}
		else
		{
			font.draw_text(gc, 410, 400, "NOT SUPPORTED BY clanGL");
		}

		font.draw_text(gc, 810, 400, "converted back to cl_rgba8");

		window.flip(1);

		// This call processes user input and other events
		CL_KeepAlive::process(0);
	}

	return 0;
}

// A key was pressed
void App::on_input_up(const CL_InputEvent &key)
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

void App::update_images()
{

	CL_PixelConverter converter;
	switch (current_format)
	{
		case cl_srgb8:
		case cl_srgb8_alpha8:
			converter.set_gamma(1.0f / 2.2f);
	}

	CL_PixelBuffer pb_tux_convert = tux.to_format(current_format, converter);

	switch (current_format)
	{
		case cl_srgb8:
		case cl_srgb8_alpha8:
			converter.set_gamma(2.2f);
	}
	CL_PixelBuffer pb_tux_unconvert = pb_tux_convert.to_format(cl_rgba8, converter);

	tux_convert = CL_Image(*display_gc, pb_tux_convert, pb_tux_convert.get_size());
	tux_unconvert = CL_Image(*display_gc, pb_tux_unconvert, pb_tux_unconvert.get_size());

	gl_supported = CL_OpenGL::get_textureformat(current_format).valid;

}

