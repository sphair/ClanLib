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
#include "alpha.h"

// The start of the Application
int Alpha::start(const std::vector<CL_String> &args)
{
	quit = false;

	// Set the window
	CL_DisplayWindowDescription desc;
	desc.set_title("Standard Compositing using the Alpha Channel");
	desc.set_size(CL_Size(800, 800), true);
	desc.set_allow_resize(true);

	CL_DisplayWindow window(desc);

	// Connect the Window close event
	CL_Slot slot_quit = window.sig_window_close().connect(this, &Alpha::on_window_close);

	// Connect a keyboard handler to on_key_up()
	CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &Alpha::on_input_up);

	// Get the graphic context
	CL_GraphicContext gc = window.get_gc();

	CL_Font font(gc, "tahoma", 16);

	// Run until someone presses escape
	while (!quit)
	{
		gc.clear(CL_Colorf(1.0f,1.0f,1.0f, 1.0f));	// White background

		font.draw_text(gc, 8, 20, "Standard Equation:", CL_Colorf::black);
		font.draw_text(gc, 8, 40, "Destination Color    =    AlphaSource * ColorSource    +    ( 1 - AlphaSource ) * ColorDestination", CL_Colorf::black);
		font.draw_text(gc, 8, 60, "Destination Alpha    =   AlphaSource                          +    ( 1 - AlphaSource ) * AlphaDestination", CL_Colorf::black);

		font.draw_text(gc, 8, 100, "ColorSource = Vertex Color * Image Color", CL_Colorf::black);
		font.draw_text(gc, 8, 120, "AlphaSource = Vertex Alpha * Image Alpha", CL_Colorf::black);
		int ypos = 160;
		int ygap = 80;
		draw_section(gc, font, ypos, CL_Colorf(0.0f, 0.0f, 1.0f, 1.0f), CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f), CL_Colorf(1.0f, 0.0f, 0.0f, 1.0f));
		ypos += ygap;
		draw_section(gc, font, ypos, CL_Colorf(0.0f, 0.0f, 1.0f, 1.0f), CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f), CL_Colorf(1.0f, 0.0f, 0.0f, 0.5f));
		ypos += ygap;
		draw_section(gc, font, ypos, CL_Colorf(0.0f, 0.0f, 1.0f, 1.0f), CL_Colorf(1.0f, 1.0f, 1.0f, 0.5f), CL_Colorf(1.0f, 0.0f, 0.0f, 1.0f));
		ypos += ygap;
		draw_section(gc, font, ypos, CL_Colorf(0.0f, 0.0f, 1.0f, 1.0f), CL_Colorf(1.0f, 1.0f, 1.0f, 0.5f), CL_Colorf(1.0f, 0.0f, 0.0f, 1.0f));
		ypos += ygap;
		draw_section(gc, font, ypos, CL_Colorf(0.0f, 0.0f, 1.0f, 1.0f), CL_Colorf(0.2f, 0.0f, 0.0f, 1.0f), CL_Colorf(0.9f, 0.0f, 0.0f, 1.0f));
		ypos += ygap;
		draw_section(gc, font, ypos, CL_Colorf(0.0f, 0.0f, 1.0f, 1.0f), CL_Colorf(1.0f, 0.0f, 0.0f, 0.2f), CL_Colorf(1.0f, 0.0f, 0.0f, 0.9f));
		ypos += ygap;
		draw_section(gc, font, ypos, CL_Colorf(0.0f, 0.0f, 1.0f, 0.5f), CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f), CL_Colorf(1.0f, 0.0f, 0.0f, 0.5f));
		ypos += ygap;
		draw_section(gc, font, ypos, CL_Colorf(0.5f, 0.5f, 1.0f, 1.0f), CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f), CL_Colorf(0.0f, 0.0f, 0.0f, 0.3f));
		ypos += ygap;

		// Flip the display, showing on the screen what we have drawed
		// since last call to flip()
		window.flip(1);

		// This call processes user input and other events
		CL_KeepAlive::process(0);
	}

	return 0;
}

// A key was pressed
void Alpha::on_input_up(const CL_InputEvent &key, const CL_InputState &state)
{
	if(key.id == CL_KEY_ESCAPE)
	{
		quit = true;
	}
}

// The window was closed
void Alpha::on_window_close()
{
	quit = true;
}

CL_Image Alpha::create_block(CL_GraphicContext &gc, const CL_Colorf &colour)
{
	const int size = 24;
	
	CL_Color colour_int(colour);
	cl_ubyte8 colour_a = colour_int.color >> 24;
	cl_ubyte8 colour_r = colour_int.color >> 16;
	cl_ubyte8 colour_g = colour_int.color >> 8;
	cl_ubyte8 colour_b = colour_int.color >> 0;

	CL_PixelBuffer pbuff(size, size, cl_rgba8);
	cl_ubyte8 *pixels = (cl_ubyte8 *) pbuff.get_data();
	int pitch = pbuff.get_pitch();
	for (int ypos = 0; ypos < size; ypos++)
	{
		cl_ubyte8 *dest_ptr = pixels + ypos * pitch;
		for (int xpos = 0; xpos < size; xpos++)
		{
			*(dest_ptr++) = colour_a;
			*(dest_ptr++) = colour_b;
			*(dest_ptr++) = colour_g;
			*(dest_ptr++) = colour_r;

		}
	}
	return CL_Image(gc, pbuff, pbuff.get_size());

}

void Alpha::draw_section(CL_GraphicContext &gc, CL_Font &font, int yoffset, const CL_Colorf &background, const CL_Colorf &vertex_colour, const CL_Colorf &image_colour)
{
	// Draw the background without blending to set the specified RGBA
	CL_BlendMode blend_mode;
	blend_mode.enable_blending(false);
	gc.set_blend_mode(blend_mode);
	const int outer_area_size = 32;
	const int outer_xoffset = 8;
	CL_Draw::fill( gc, outer_xoffset, yoffset, outer_xoffset + outer_area_size, yoffset + outer_area_size, background);
	blend_mode.enable_blending(true);
	gc.set_blend_mode(blend_mode);

	// Create the image
	CL_Image image = create_block(gc, image_colour);

	// Draw the image
	image.set_color(vertex_colour);
	image.draw(gc, outer_xoffset + (outer_area_size - image.get_width())/2, yoffset + (outer_area_size - image.get_height())/2);

	// Get the composited pixel buffer
	// ** BUG IN CLANLIB ** rect requires "gc.height() - xxxxx"
	CL_Rect rect(outer_xoffset + outer_area_size / 2, gc.get_height() - (yoffset + outer_area_size / 2), CL_Size(8,8));
	CL_PixelBuffer pbuff = gc.get_pixeldata(rect, cl_abgr8);
 	CL_Colorf output = pbuff.get_pixel(0,0);
 
	// Create the information string
	CL_String info(cl_format("Initial Destination Colour: RGBA = %1, %2, %3, %4", background.r , background.g, background.b, background.a));
	int xpos = outer_xoffset + outer_area_size + 8;
	int ypos = yoffset - 4;
	font.draw_text(gc, xpos, ypos, info, CL_Colorf::black);

	info = CL_String(cl_format("Vertex Colour: RGBA = %1, %2, %3, %4", vertex_colour.r , vertex_colour.g, vertex_colour.b, vertex_colour.a));
	ypos += 16;
	font.draw_text(gc, xpos, ypos, info, CL_Colorf::black);

	info = CL_String(cl_format("Image Colour: RGBA = %1, %2, %3, %4", image_colour.r , image_colour.g, image_colour.b, image_colour.a));
	ypos += 16;
	font.draw_text(gc, xpos, ypos, info, CL_Colorf::black);

	info = CL_String(cl_format("Destination Colour: RGBA = %1, %2, %3, %4", output.r , output.g, output.b, output.a));
	ypos += 16;
	font.draw_text(gc, xpos, ypos, info, CL_Colorf::black);

}
