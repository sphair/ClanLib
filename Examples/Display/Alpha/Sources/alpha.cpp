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
**    Magnus Norddahl
**    Mark Page
*/

#include "precomp.h"
#include "alpha.h"

// The start of the Application
int Alpha::start(const std::vector<std::string> &args)
{
	quit = false;

	// Set the window
	DisplayWindowDescription desc;
	desc.set_title("Standard Compositing using the Alpha Channel");
	desc.set_size(Size(800, 800), true);
	desc.set_allow_resize(true);

	DisplayWindow window(desc);

	// Connect the Window close event
	Slot slot_quit = window.sig_window_close().connect(this, &Alpha::on_window_close);

	// Connect a keyboard handler to on_key_up()
	Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &Alpha::on_input_up);

	// Get the graphic context
	Canvas canvas(window);

	BlendStateDescription blend_desc;
	blend_desc.enable_blending(false);
	blend_disabled = BlendState(canvas, blend_desc);
	blend_desc.enable_blending(true);
	blend_enabled = BlendState(canvas, blend_desc);

	Font font(canvas, "tahoma", 16);

	// Run until someone presses escape
	while (!quit)
	{
		canvas.clear(Colorf(1.0f,1.0f,1.0f, 1.0f));	// White background

		font.draw_text(canvas, 8, 20, "Standard Equation:", Colorf::black);
		font.draw_text(canvas, 8, 40, "Destination Color    =    AlphaSource * ColorSource    +    ( 1 - AlphaSource ) * ColorDestination", Colorf::black);
		font.draw_text(canvas, 8, 60, "Destination Alpha    =   AlphaSource                          +    ( 1 - AlphaSource ) * AlphaDestination", Colorf::black);

		font.draw_text(canvas, 8, 100, "ColorSource = Vertex Color * Image Color", Colorf::black);
		font.draw_text(canvas, 8, 120, "AlphaSource = Vertex Alpha * Image Alpha", Colorf::black);
		int ypos = 160;
		int ygap = 80;
		draw_section(canvas, font, ypos, Colorf(0.0f, 0.0f, 1.0f, 1.0f), Colorf(1.0f, 1.0f, 1.0f, 1.0f), Colorf(1.0f, 0.0f, 0.0f, 1.0f));
		ypos += ygap;
		draw_section(canvas, font, ypos, Colorf(0.0f, 0.0f, 1.0f, 1.0f), Colorf(1.0f, 1.0f, 1.0f, 1.0f), Colorf(1.0f, 0.0f, 0.0f, 0.5f));
		ypos += ygap;
		draw_section(canvas, font, ypos, Colorf(0.0f, 0.0f, 1.0f, 1.0f), Colorf(1.0f, 1.0f, 1.0f, 0.5f), Colorf(1.0f, 0.0f, 0.0f, 1.0f));
		ypos += ygap;
		draw_section(canvas, font, ypos, Colorf(0.0f, 0.0f, 1.0f, 1.0f), Colorf(1.0f, 1.0f, 1.0f, 0.5f), Colorf(1.0f, 0.0f, 0.0f, 1.0f));
		ypos += ygap;
		draw_section(canvas, font, ypos, Colorf(0.0f, 0.0f, 1.0f, 1.0f), Colorf(0.2f, 0.0f, 0.0f, 1.0f), Colorf(0.9f, 0.0f, 0.0f, 1.0f));
		ypos += ygap;
		draw_section(canvas, font, ypos, Colorf(0.0f, 0.0f, 1.0f, 1.0f), Colorf(1.0f, 0.0f, 0.0f, 0.2f), Colorf(1.0f, 0.0f, 0.0f, 0.9f));
		ypos += ygap;
		draw_section(canvas, font, ypos, Colorf(0.0f, 0.0f, 1.0f, 0.5f), Colorf(1.0f, 1.0f, 1.0f, 1.0f), Colorf(1.0f, 0.0f, 0.0f, 0.5f));
		ypos += ygap;
		draw_section(canvas, font, ypos, Colorf(0.5f, 0.5f, 1.0f, 1.0f), Colorf(1.0f, 1.0f, 1.0f, 1.0f), Colorf(0.0f, 0.0f, 0.0f, 0.3f));
		ypos += ygap;

		canvas.flush();

		// Flip the display, showing on the screen what we have drawed
		// since last call to flip()
		window.flip(1);

		// This call processes user input and other events
		KeepAlive::process(0);
	}

	return 0;
}

// A key was pressed
void Alpha::on_input_up(const InputEvent &key)
{
	if(key.id == keycode_escape)
	{
		quit = true;
	}
}

// The window was closed
void Alpha::on_window_close()
{
	quit = true;
}

Image Alpha::create_block(Canvas &canvas, const Colorf &colour)
{
	const int size = 24;
	
	Color colour_int(colour);
	ubyte8 colour_a = colour_int.a;
	ubyte8 colour_r = colour_int.r;
	ubyte8 colour_g = colour_int.g;
	ubyte8 colour_b = colour_int.b;

	PixelBuffer pbuff(size, size, tf_rgba8);
	ubyte8 *pixels = (ubyte8 *) pbuff.get_data();
	int pitch = pbuff.get_pitch();
	for (int ypos = 0; ypos < size; ypos++)
	{
		ubyte8 *dest_ptr = pixels + ypos * pitch;
		for (int xpos = 0; xpos < size; xpos++)
		{
			*(dest_ptr++) = colour_a;
			*(dest_ptr++) = colour_b;
			*(dest_ptr++) = colour_g;
			*(dest_ptr++) = colour_r;

		}
	}
	return Image(canvas, pbuff, pbuff.get_size());

}

void Alpha::draw_section(Canvas &canvas, Font &font, int yoffset, const Colorf &background, const Colorf &vertex_colour, const Colorf &image_colour)
{
	// Draw the background without blending to set the specified RGBA

	canvas.set_blend_state(blend_disabled);

	const int outer_area_size = 32;
	const int outer_xoffset = 8;
	canvas.fill( outer_xoffset, yoffset, outer_xoffset + outer_area_size, yoffset + outer_area_size, background);

	canvas.set_blend_state(blend_enabled);

	// Create the image
	Image image = create_block(canvas, image_colour);

	// Draw the image
	image.set_color(vertex_colour);
	image.draw(canvas, outer_xoffset + (outer_area_size - image.get_width())/2, yoffset + (outer_area_size - image.get_height())/2);

	// Get the composited pixel buffer
	Rect rect(outer_xoffset + outer_area_size / 2, (yoffset + outer_area_size / 2), Size(64,64));
	PixelBuffer pbuff = canvas.get_pixeldata(rect, tf_rgba8);
	pbuff.lock(canvas, access_read_only);

	ImageProviderFactory::save(pbuff, "test.png");

 	Colorf output = pbuff.get_pixel(0,0);
	pbuff.unlock();
 
	// Create the information string
	std::string info(string_format("Initial Destination Colour: RGBA = %1, %2, %3, %4", background.r , background.g, background.b, background.a));
	int xpos = outer_xoffset + outer_area_size + 8;
	int ypos = yoffset - 4;
	font.draw_text(canvas, xpos, ypos, info, Colorf::black);

	info = std::string(string_format("Vertex Colour: RGBA = %1, %2, %3, %4", vertex_colour.r , vertex_colour.g, vertex_colour.b, vertex_colour.a));
	ypos += 16;
	font.draw_text(canvas, xpos, ypos, info, Colorf::black);

	info = std::string(string_format("Image Colour: RGBA = %1, %2, %3, %4", image_colour.r , image_colour.g, image_colour.b, image_colour.a));
	ypos += 16;
	font.draw_text(canvas, xpos, ypos, info, Colorf::black);

	info = std::string(string_format("Destination Colour: RGBA = %1, %2, %3, %4", output.r , output.g, output.b, output.a));
	ypos += 16;
	font.draw_text(canvas, xpos, ypos, info, Colorf::black);

}
