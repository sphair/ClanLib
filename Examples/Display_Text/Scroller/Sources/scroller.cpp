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
#include "scroller.h"
#include "text.h"

// The start of the Application
int Scroller::start(const std::vector<std::string> &args)
{
	quit = false;

	// Set the window
	DisplayWindowDescription desc;
	desc.set_title("ClanLib Text Scroller Example");
	desc.set_size(Size(640, 100), true);
	desc.set_allow_resize(true);

	DisplayWindow window(desc);

	// Connect the Window close event
	Slot slot_quit = window.sig_window_close().connect(this, &Scroller::on_window_close);

	// Connect a keyboard handler to on_key_up()
	Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &Scroller::on_input_up);

	// Get the graphic context
	GraphicContext gc = window.get_gc();

	// Load the configuration
	File file("Resources/text.xml");
	DomDocument document(file);
	DomElement document_element = document.get_document_element();
	if (document_element.is_null())
		throw Exception("Cannot obtain the document element");

	Text text;
	FontDescription font_description;
	font_description.set_typeface_name("arial");
	font_description.set_height(24);
	text.init(gc, font_description, document_element, "TEXT");

	unsigned int last_time = System::get_time();

	// Run until someone presses escape
	while (!quit)
	{
		unsigned int current_time = System::get_time();
		int time_delta_ms = current_time - last_time;
		last_time = current_time;

		text.run(time_delta_ms, 20);

		gc.clear(Colorf(0.0f,0.0f,0.2f));

		Rect rect( 32, 32, gc.get_width() - 32, 64 );
		Draw::fill(gc, rect, Colorf::black);
		text.draw(gc, rect);
		rect.expand(1);
		Draw::box(gc, rect, Colorf::white);

		window.flip(1);

		KeepAlive::process(0);
	}

	return 0;
}

// A key was pressed
void Scroller::on_input_up(const InputEvent &key)
{
	if(key.id == KEY_ESCAPE)
	{
		quit = true;
	}
}

// The window was closed
void Scroller::on_window_close()
{
	quit = true;
}


