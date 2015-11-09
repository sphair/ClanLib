/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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

clan::ApplicationInstance<SineScroll> clanapp;

SineScroll::SineScroll()
{
#ifdef WIN32
	clan::D3DTarget::set_current();
#else
	clan::OpenGLTarget::set_current();
#endif

	// Set the window
	clan::DisplayWindowDescription desc;
	desc.set_title("ClanLib SineScroll Example");
	desc.set_size(clan::Size(800, 700), true);
	desc.set_allow_resize(true);

	window = clan::DisplayWindow(desc);

	// Connect the Window close event
	sc.connect(window.sig_window_close(), clan::bind_member(this, &SineScroll::on_window_close));

	// Connect a keyboard handler to on_key_up()
	sc.connect(window.get_keyboard().sig_key_up(), clan::bind_member(this, &SineScroll::on_input_up));

	canvas = clan::Canvas(window);

	texture = clan::Texture2D(canvas, "../../Display/Path/Resources/lobby_background2.png");

	game_time.reset();
}

bool SineScroll::update()
{
	game_time.update();
	
	canvas.clear(clan::Colorf(0.0f,0.0f,0.2f));
	
	draw_demo(canvas, game_time.get_time_elapsed_ms());

	window.flip(1);

	return !quit;
}

// A key was pressed
void SineScroll::on_input_up(const clan::InputEvent &key)
{
	if(key.id == clan::keycode_escape)
	{
		quit = true;
	}
}

// The window was closed
void SineScroll::on_window_close()
{
	quit = true;
}

void SineScroll::draw_demo(clan::Canvas &canvas, int delta_ms)
{
	clan::Rectf rect(0.0f, 0.0f, clan::Sizef(300.0f, 300.0f));
	clan::Rectf texture_unit1_coords(0.0f, 0.0f, 1.0f, 1.0f);

	std::vector<clan::Vec2f> dest_position;
	std::vector<clan::Vec2f> texture_position;

	int dest_width = canvas.get_width();
	if (dest_width <=0)
		return;

	int dest_xoffset = 0;

	int gc_height = canvas.get_height();

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
	if (sin_offset > (2.0f * clan::PI))
		sin_offset -= clan::PI * 2.0f;

	for (int cnt=0; cnt < dest_width; cnt++)
	{
		float y_offset = sin_amplitude * sin( sin_offset + (float) cnt / 100.0f ) ;

		dest_position.push_back( clan::Vec2f( cnt, dest_start_y + y_offset ) );
		dest_position.push_back( clan::Vec2f( cnt, dest_start_y + dest_height + y_offset) );

		texture_position.push_back( clan::Vec2f( (float) cnt / dest_width, texture_y_start ) );
		texture_position.push_back( clan::Vec2f( (float) cnt / dest_width, texture_y_end ) );

	}

	canvas.draw_lines(&dest_position[0], &texture_position[0], dest_position.size(), texture);

}
