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
#include "sinescroll.h"

// The start of the Application
int SineScroll::start(const std::vector<std::string> &args)
{
	sin_offset = 0.0f;
	quit = false;

	// Set the window
	DisplayWindowDescription desc;
	desc.set_title("ClanLib SineScroll Example");
	desc.set_size(Size(800, 700), true);
	desc.set_allow_resize(true);

	DisplayWindow window(desc);

	// Connect the Window close event
	Slot slot_quit = window.sig_window_close().connect(this, &SineScroll::on_window_close);

	// Connect a keyboard handler to on_key_up()
	Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &SineScroll::on_input_up);

	// Get the graphic context
	GraphicContext gc = window.get_gc();

	texture = Texture(gc, "../../Game/DiceWar/Resources/lobby_background2.png");

	unsigned int last_time = System::get_time();

	// Run until someone presses escape
	while (!quit)
	{
		unsigned int current_time = System::get_time();
		int delta_ms = current_time - last_time;
		last_time = current_time;

		gc.clear(Colorf(0.0f,0.0f,0.2f));
	
		draw_demo(gc, delta_ms);

		window.flip(1);

		KeepAlive::process(0);
	}

	return 0;
}

// A key was pressed
void SineScroll::on_input_up(const InputEvent &key)
{
	if(key.id == KEY_ESCAPE)
	{
		quit = true;
	}
}

// The window was closed
void SineScroll::on_window_close()
{
	quit = true;
}

void SineScroll::draw_demo(GraphicContext &gc, int delta_ms)
{
	Rectf rect(0.0f, 0.0f, Sizef(300.0f, 300.0f));
	Rectf texture_unit1_coords(0.0f, 0.0f, 1.0f, 1.0f);

	std::vector<Vec2f> dest_position;
	std::vector<Vec2f> texture_position;

	int dest_width = gc.get_width();
	if (dest_width <=0)
		return;

	int dest_xoffset = 0;

	int gc_height = gc.get_height();

	int dest_height = 128;
	int dest_start_y = (gc_height - dest_height)  / 2;

	float texture_y_start = 0.15f;	// Set to 0.0f for start and 1.0f for end to use the entire texture
	float texture_y_end = 0.5f;

	dest_position.reserve(dest_width * 2);
	texture_position.reserve(dest_width * 2);

	float sin_amplitude = (  (float) gc_height / 4.0f);

	if (delta_ms > 1000)	// Limit to 1 second to frame
		delta_ms = 1000;

	sin_offset += ((float) delta_ms / 1000.0f);
	if (sin_offset > (2.0f * PI))
		sin_offset -= PI * 2.0f;

	for (int cnt=0; cnt < dest_width; cnt++)
	{
		float y_offset = sin_amplitude * sin( sin_offset + (float) cnt / 100.0f ) ;

		dest_position.push_back( Vec2f( cnt, dest_start_y + y_offset ) );
		dest_position.push_back( Vec2f( cnt, dest_start_y + dest_height + y_offset) );

		texture_position.push_back( Vec2f( (float) cnt / dest_width, texture_y_start ) );
		texture_position.push_back( Vec2f( (float) cnt / dest_width, texture_y_end ) );

	}

	gc.set_texture(0, texture);
	PrimitivesArray prim_array(gc);
	prim_array.set_attributes(0, &dest_position[0]);
	prim_array.set_attribute(1, Colorf(1.0f, 1.0f, 1.0f, 1.0f));
	prim_array.set_attributes(2, &texture_position[0]);
	gc.set_program_object(cl_program_single_texture);
	gc.draw_primitives(cl_lines, dest_position.size(), prim_array);
	gc.reset_program_object();
	gc.reset_texture(0);
}
