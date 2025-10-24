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
#include "collision.h"
#include "teapot.h"
#include "framerate_counter.h"

// The start of the Application
int Collision::start(const std::vector<CL_String> &args)
{
	quit = false;

	// Set the window
	CL_DisplayWindowDescription desc;
	desc.set_title("ClanLib Collision Example");
	desc.set_size(CL_Size(1000, 700), true);
	desc.set_allow_resize(true);

	CL_DisplayWindow window(desc);

	// Connect the Window close event
	CL_Slot slot_quit = window.sig_window_close().connect(this, &Collision::on_window_close);

	// Connect a keyboard handler to on_key_up()
	CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &Collision::on_input_up);

	// Get the graphic context
	CL_GraphicContext gc = window.get_gc();

	CL_ResourceManager resources = CL_ResourceManager("Resources/resources.xml");
	
	CL_Font font(gc, "tahoma", 24);

	gc.clear(CL_Colorf(0.0f,0.0f,0.2f));
	font.draw_text(gc, 32, 32, "Calculating Collision Outlines");
	font.draw_text(gc, 32, 64, "Note, collision outlines can be saved to disk for faster loading times");
	window.flip();

	int num_teapots = 8;

	Teapot base_teapot;
	base_teapot.create(gc, resources);

	std::vector<Teapot> teapot_list;
	teapot_list.resize(num_teapots);

	for (int cnt=0; cnt<num_teapots; cnt++)
	{
		teapot_list[cnt].clone(base_teapot);

		int xpos = (rand() & 0xffff) % gc.get_width();
		int ypos = (rand() & 0xffff) % (gc.get_height()/2);
		teapot_list[cnt].set_position(xpos, ypos);

		int x_delta = (rand() & 0xffff);
		int y_delta = (rand() & 0xffff);
		CL_Vec2f normal((x_delta - 0x7fff) * 0.00001f, (y_delta - 0x7fff) * 0.000001f);
		normal.normalize();

		teapot_list[cnt].set_movement_delta(normal, 0.1f);

		int scale = (rand() & 0xffff);
		teapot_list[cnt].set_scale(0.5f + scale * 0.00002f, 0.5f + scale * 0.00002f);

		int frame = (rand() & 0xffff);
		teapot_list[cnt].set_frame(frame % 60);	// Assuming teapots has 60 frames

		teapot_list[cnt].set_color( CL_Colorf(
			(rand() & 0xFF) / 256.0f,
			(rand() & 0xFF) / 256.0f,
			(rand() & 0xFF) / 256.0f,
			1.0f));
	}

	FramerateCounter frameratecounter;

	unsigned int last_time = CL_System::get_time();

	// Run until someone presses escape
	while (!quit)
	{
		unsigned int current_time = CL_System::get_time();
		int time_elapsed = current_time - last_time;
		last_time = current_time;

		gc.clear(CL_Colorf(0.0f,0.0f,0.2f));

		font.draw_text(gc, 16, 32, "Sprite Image");
		font.draw_text(gc, 16, gc.get_height()/2 + 32, "Collision Outline");

		CL_String fps = cl_format("%1 fps", frameratecounter.get_framerate());
		font.draw_text(gc, gc.get_width() - 100, 32, fps);

		for (int cnt=0; cnt<num_teapots; cnt++)
		{
			teapot_list[cnt].update(gc, time_elapsed, teapot_list);
			teapot_list[cnt].draw_collision_outline(gc);
			teapot_list[cnt].draw_teapot(gc);
		}

		CL_Draw::line(gc, 0, gc.get_height()/2, gc.get_width(), gc.get_height()/2, CL_Colorf::white);
		
		window.flip(0);
		frameratecounter.frame_shown();

		CL_KeepAlive::process(0);
	}

	return 0;
}

// A key was pressed
void Collision::on_input_up(const CL_InputEvent &key, const CL_InputState &state)
{
	if(key.id == CL_KEY_ESCAPE)
	{
		quit = true;
	}
}

// The window was closed
void Collision::on_window_close()
{
	quit = true;
}


