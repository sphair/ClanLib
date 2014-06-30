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
#include "scroller.h"
#include "text.h"

// The start of the Application
int Scroller::start(const std::vector<std::string> &args)
{
	quit = false;

	// Set the window
	clan::DisplayWindowDescription desc;
	desc.set_title("ClanLib Text Scroller Example");
	desc.set_size(clan::Size(640, 100), true);
	desc.set_allow_resize(true);

	clan::DisplayWindow window(desc);
    clan::SlotContainer cc;

	// Connect the Window close event
	cc.connect(window.sig_window_close(), std::function<void()>(this, &Scroller::on_window_close));

	// Connect a keyboard handler to on_key_up()
	cc.connect(window.get_ic().get_keyboard().sig_key_up(), std::function<void(const clan::InputEvent&)>(this, &Scroller::on_input_up));

	// Get the canvas
	clan::Canvas canvas(window);

	// Load the configuration
	clan::File file("Resources/text.xml");
	clan::DomDocument document(file);
	clan::DomElement document_element = document.get_document_element();
	if (document_element.is_null())
		throw clan::Exception("Cannot obtain the document element");

	Text text;
	clan::FontDescription font_description;
	font_description.set_typeface_name("arial");
	font_description.set_height(24);
	text.init(canvas, font_description, document_element, "TEXT");

	clan::GameTime game_time;
	// Run until someone presses escape
	while (!quit)
	{
		game_time.update();
		text.run(game_time.get_time_elapsed_ms(), 20);

		canvas.clear(clan::Colorf(0.0f,0.0f,0.2f));

		clan::Rect rect( 32, 32, canvas.get_width() - 32, 64 );
		canvas.fill_rect(rect, clan::Colorf::black);
		text.draw(canvas, rect);
		rect.expand(1);
		canvas.draw_box(rect, clan::Colorf::white);

		window.flip(1);

		clan::KeepAlive::process(0);
	}

	return 0;
}

// A key was pressed
void Scroller::on_input_up(const clan::InputEvent &key)
{
	if(key.id == clan::keycode_escape)
	{
		quit = true;
	}
}

// The window was closed
void Scroller::on_window_close()
{
	quit = true;
}


