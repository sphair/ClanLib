/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
#include "textfade.h"

// The start of the Application
int TextFade::start(const std::vector<std::string> &args)
{
	quit = false;

	// Set the window
	clan::DisplayWindowDescription desc;
	desc.set_title("ClanLib TextFade Example");
	desc.set_size(clan::Size(640, 480), true);
	desc.set_allow_resize(true);

	clan::DisplayWindow window(desc);

	// Connect the Window close event
	clan::Slot slot_quit = window.sig_window_close().connect(this, &TextFade::on_window_close);

	// Connect a keyboard handler to on_key_up()
	clan::Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &TextFade::on_input_up);

	// Get the graphic context
	clan::Canvas canvas(window);

	clan::Image background(canvas, "../../Game/DiceWar/Resources/lobby_background2.png");

	clan::FontDescription font_description;
	font_description.set_typeface_name("arial");
	font_description.set_height(48);
	font_description.set_subpixel(false);	// Fading only works with sub pixel off
	clan::Font standard_font(canvas, font_description);

	clan::DisplayCache app_resources("../Font/Resources/resources.xml");
	clan::Font_Sprite sprite_font(canvas, "ClanFont", &app_resources);

	// Run until someone presses escape
	while (!quit)
	{
		background.draw(canvas, canvas.get_size());

		draw_text(canvas, standard_font, 100, "ABCDEFGHIJKLMNOPQRSTUVWXZ");

		draw_text(canvas, sprite_font, 300, "ABCDEFGHIJKLMNOPQRSTUVWXZ");

		canvas.flip(1);

		// This call processes user input and other events
		clan::KeepAlive::process(0);
	}

	return 0;
}

void TextFade::draw_text(clan::Canvas &canvas, clan::Font &font, int ypos, const char *text)
{
	// For this example, to keep things simple, we only handle ASCII characters

	int gc_width = canvas.get_width();

	clan::Size text_size = font.get_text_size(canvas, text);

	int xpos = (gc_width - text_size.width) / 2;

	while(*text)
	{
		char let = *(text++);
		char buffer[2];
		buffer[0] = let;
		buffer[1] = 0;

		int position_off_centre = (gc_width/2) - xpos;
		if (position_off_centre < 0)
			position_off_centre = -position_off_centre;

		float alpha = 1.0f - ( (float) position_off_centre * 2.0f / gc_width );
		if (alpha > 0.0f)
			font.draw_text(canvas, xpos, ypos,  buffer, clan::Colorf(0.4f, 0.4f, 1.0f, alpha));
		xpos += font.get_glyph_size(canvas, let).width;
	}

}


// A key was pressed
void TextFade::on_input_up(const clan::InputEvent &key)
{
	if(key.id == clan::keycode_escape)
	{
		quit = true;
	}
}

// The window was closed
void TextFade::on_window_close()
{
	quit = true;
}


