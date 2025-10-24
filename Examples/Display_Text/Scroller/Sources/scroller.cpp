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
#include "scroller.h"
#include "text.h"

// The start of the Application
int Scroller::start(const std::vector<CL_String> &args)
{
	quit = false;

	// Set the window
	CL_DisplayWindowDescription desc;
	desc.set_title("ClanLib Text Scroller Example");
	desc.set_size(CL_Size(640, 100), true);
	desc.set_allow_resize(true);

	CL_DisplayWindow window(desc);

	// Connect the Window close event
	CL_Slot slot_quit = window.sig_window_close().connect(this, &Scroller::on_window_close);

	// Connect a keyboard handler to on_key_up()
	CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &Scroller::on_input_up);

	// Get the graphic context
	CL_GraphicContext gc = window.get_gc();

	// Load the configuration
	CL_File file("Resources/text.xml");
	CL_DomDocument document(file);
	CL_DomElement document_element = document.get_document_element();
	if (document_element.is_null())
		throw CL_Exception("Cannot obtain the document element");

	Text text;
	CL_FontDescription font_description;
	font_description.set_typeface_name("arial");
	font_description.set_height(24);
	text.init(gc, font_description, document_element, "TEXT");

	unsigned int last_time = CL_System::get_time();

	// Run until someone presses escape
	while (!quit)
	{
		unsigned int current_time = CL_System::get_time();
		int time_delta_ms = current_time - last_time;
		last_time = current_time;

		text.run(time_delta_ms, 20);

		gc.clear(CL_Colorf(0.0f,0.0f,0.2f));

		CL_Rect rect( 32, 32, gc.get_width() - 32, 64 );
		CL_Draw::fill(gc, rect, CL_Colorf::black);
		text.draw(gc, rect);
		rect.expand(1);
		CL_Draw::box(gc, rect, CL_Colorf::white);

		window.flip(1);

		CL_KeepAlive::process(0);
	}

	return 0;
}

// A key was pressed
void Scroller::on_input_up(const CL_InputEvent &key, const CL_InputState &state)
{
	if(key.id == CL_KEY_ESCAPE)
	{
		quit = true;
	}
}

// The window was closed
void Scroller::on_window_close()
{
	quit = true;
}


