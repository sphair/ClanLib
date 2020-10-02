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
**    Magnus Norddahl
**    Mark Page
*/

#include "precomp.h"
#include "alpha.h"

clan::ApplicationInstance<Alpha> clanapp;

Alpha::Alpha()
{
#if defined(WIN32) && !defined(__MINGW32__)
	clan::D3DTarget::set_current();
#else
	clan::OpenGLTarget::set_current();
#endif

	// Set the window
	clan::DisplayWindowDescription desc;
	desc.set_title("Standard Compositing using the Alpha Channel");
	desc.set_size(clan::Size(800, 800), true);
	desc.set_allow_resize(true);

	window = clan::DisplayWindow(desc);

	// Connect the Window close event
	sc.connect(window.sig_window_close(), clan::bind_member(this, &Alpha::on_window_close));

	// Connect a keyboard handler to on_key_up()
	sc.connect(window.get_keyboard().sig_key_up(), clan::bind_member(this, &Alpha::on_input_up));

	// Get the graphic context
	canvas = clan::Canvas(window);

	clan::BlendStateDescription blend_desc;
	blend_desc.enable_blending(false);
	blend_disabled = clan::BlendState(canvas, blend_desc);
	blend_desc.enable_blending(true);
	blend_enabled = clan::BlendState(canvas, blend_desc);

	font = clan::Font("tahoma", 16);

}

bool Alpha::update()
{
	canvas.clear(clan::Colorf(1.0f,1.0f,1.0f, 1.0f));	// White background

	font.draw_text(canvas, 8, 20, "Standard Equation:", clan::Colorf::black);
	font.draw_text(canvas, 8, 40, "Destination Color    =    AlphaSource * ColorSource    +    ( 1 - AlphaSource ) * ColorDestination", clan::Colorf::black);
	font.draw_text(canvas, 8, 60, "Destination Alpha    =   AlphaSource                          +    ( 1 - AlphaSource ) * AlphaDestination", clan::Colorf::black);

	font.draw_text(canvas, 8, 100, "ColorSource = Vertex Color * Image Color", clan::Colorf::black);
	font.draw_text(canvas, 8, 120, "AlphaSource = Vertex Alpha * Image Alpha", clan::Colorf::black);
	int ypos = 160;
	int ygap = 80;
	draw_section(canvas, font, ypos, clan::Colorf(0.0f, 0.0f, 1.0f, 1.0f), clan::Colorf(1.0f, 1.0f, 1.0f, 1.0f), clan::Colorf(1.0f, 0.0f, 0.0f, 1.0f));
	ypos += ygap;
	draw_section(canvas, font, ypos, clan::Colorf(0.0f, 0.0f, 1.0f, 1.0f), clan::Colorf(1.0f, 1.0f, 1.0f, 1.0f), clan::Colorf(1.0f, 0.0f, 0.0f, 0.5f));
	ypos += ygap;
	draw_section(canvas, font, ypos, clan::Colorf(0.0f, 0.0f, 1.0f, 1.0f), clan::Colorf(1.0f, 1.0f, 1.0f, 0.5f), clan::Colorf(1.0f, 0.0f, 0.0f, 1.0f));
	ypos += ygap;
	draw_section(canvas, font, ypos, clan::Colorf(0.0f, 0.0f, 1.0f, 1.0f), clan::Colorf(1.0f, 1.0f, 1.0f, 0.5f), clan::Colorf(1.0f, 0.0f, 0.0f, 1.0f));
	ypos += ygap;
	draw_section(canvas, font, ypos, clan::Colorf(0.0f, 0.0f, 1.0f, 1.0f), clan::Colorf(0.2f, 0.0f, 0.0f, 1.0f), clan::Colorf(0.9f, 0.0f, 0.0f, 1.0f));
	ypos += ygap;
	draw_section(canvas, font, ypos, clan::Colorf(0.0f, 0.0f, 1.0f, 1.0f), clan::Colorf(1.0f, 0.0f, 0.0f, 0.2f), clan::Colorf(1.0f, 0.0f, 0.0f, 0.9f));
	ypos += ygap;
	draw_section(canvas, font, ypos, clan::Colorf(0.0f, 0.0f, 1.0f, 0.5f), clan::Colorf(1.0f, 1.0f, 1.0f, 1.0f), clan::Colorf(1.0f, 0.0f, 0.0f, 0.5f));
	ypos += ygap;
	draw_section(canvas, font, ypos, clan::Colorf(0.0f, 0.0f, 1.0f, 0.5f), clan::Colorf(1.0f, 0.5f, 0.0f, 1.0f), clan::Colorf(0.0f, 0.5f, 0.0f, 1.0f));
	ypos += ygap;

	window.flip(1);

	return !quit;
}

// A key was pressed
void Alpha::on_input_up(const clan::InputEvent &key)
{
	if(key.id == clan::keycode_escape)
	{
		quit = true;
	}
}

// The window was closed
void Alpha::on_window_close()
{
	quit = true;
}

clan::Image Alpha::create_block(clan::Canvas &canvas, const clan::Colorf &colour)
{
	const int size = 24;

	clan::Color colour_int(colour);

	clan::PixelBuffer pbuff(size, size, clan::TextureFormat::rgba8);
	uint8_t *pixels = pbuff.get_data_uint8();
	int pitch = pbuff.get_pitch();
	for (int ypos = 0; ypos < size; ypos++)
	{
		uint8_t *dest_ptr = pixels + ypos * pitch;
		for (int xpos = 0; xpos < size; xpos++)
		{
			*(dest_ptr++) = colour_int.r;
			*(dest_ptr++) = colour_int.g;
			*(dest_ptr++) = colour_int.b;
			*(dest_ptr++) = colour_int.a;
		}
	}
	return clan::Image(canvas, pbuff, pbuff.get_size());
}

std::string Alpha::get_text(float value)
{
	return clan::StringHelp::float_to_text(value, 2, false);
}

void Alpha::draw_section(clan::Canvas &canvas, clan::Font &font, int yoffset, const clan::Colorf &background, const clan::Colorf &vertex_colour, const clan::Colorf &image_colour)
{
	// Draw the background without blending to set the specified RGBA

	canvas.set_blend_state(blend_disabled);

	const int outer_area_size = 32;
	const int outer_xoffset = 8;
	canvas.fill_rect( outer_xoffset, yoffset, outer_xoffset + outer_area_size, yoffset + outer_area_size, background);

	canvas.set_blend_state(blend_enabled);

	// Create the image
	clan::Image image = create_block(canvas, image_colour);

	// Draw the image
	image.set_color(vertex_colour);
	image.draw(canvas, outer_xoffset + (outer_area_size - image.get_width())/2, yoffset + (outer_area_size - image.get_height())/2);

	clan::Colorf output;
	// Get the composited pixel buffer
	clan::Rectf rect(outer_xoffset + outer_area_size / 2, (yoffset + outer_area_size / 2), clan::Sizef(64,64));
	if (rect.is_inside(canvas.get_size()))
	{
		clan::PixelBuffer pbuff = canvas.get_pixeldata(rect, clan::TextureFormat::rgba8);
		pbuff.lock(canvas, clan::BufferAccess::read_only);

		//clan::ImageProviderFactory::save(pbuff, "test.png");

		clan::Colorf output = pbuff.get_pixel(0, 0);
		pbuff.unlock();
	}
 
	// Create the information string
	std::string info(clan::string_format("Background = %1, %2, %3, %4", get_text(background.r), get_text(background.g), get_text(background.b), get_text(background.a)));
	int xpos = outer_xoffset + outer_area_size + 8;
	int ypos = yoffset + 12;
	font.draw_text(canvas, xpos, ypos, info, clan::Colorf::black);

	info = std::string(clan::string_format("Vertex = %1, %2, %3, %4", get_text(vertex_colour.r), get_text(vertex_colour.g), get_text(vertex_colour.b), get_text(vertex_colour.a)));
	font.draw_text(canvas, xpos + 250, ypos, info, clan::Colorf::black);

	info = std::string(clan::string_format("Image = %1, %2, %3, %4", get_text(image_colour.r), get_text(image_colour.g), get_text(image_colour.b), get_text(image_colour.a)));
	font.draw_text(canvas, xpos + 500, ypos, info, clan::Colorf::black);

	ypos += 20;
	clan::Colorf source(vertex_colour * image_colour);
	clan::Colorf calculated;
	calculated.r = source.a * source.r + (1.0f - source.a) * background.r;
	calculated.g = source.a * source.g + (1.0f - source.a) * background.g;
	calculated.b = source.a * source.b + (1.0f - source.a) * background.b;
	calculated.a = source.a + (1.0f - source.a) * background.a;

	info = std::string(clan::string_format("Source = %1, %2, %3, %4", get_text(source.r), get_text(source.g), get_text(source.b), get_text(source.a)));
	font.draw_text(canvas, xpos, ypos, info, clan::Colorf::black);

	info = std::string(clan::string_format("Calculated = %1, %2, %3, %4", get_text(calculated.r), get_text(calculated.g), get_text(calculated.b), get_text(calculated.a)));
	font.draw_text(canvas, xpos +  250, ypos, info, clan::Colorf::black);

	info = std::string(clan::string_format("Actual = %1, %2, %3, %4", get_text(output.r), get_text(output.g), get_text(output.b), get_text(output.a)));
	font.draw_text(canvas, xpos + 500, ypos, info, clan::Colorf::black);
}
