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
**    Arkadiusz Kalinowski
*/

#include "precomp.h"
#include "basicphysics.h"

// The start of the Application
int BasicPhysics::start(const std::vector<std::string> &args)
{
	//Remove the need to send physic world to every object. Instead send just the description.

	//Fix having two fixtures working weirdly.
	quit = false;

	int window_x_size = 640;
	int window_y_size = 480;
	// Set the window
	DisplayWindowDescription desc;
	desc.set_title("ClanLib BasicPhysics Example");
	desc.set_size(Size(window_x_size, window_y_size), true);
	desc.set_allow_resize(false);

	DisplayWindow window(desc);
	
	// Connect the Window close event
	Slot slot_quit = window.sig_window_close().connect(this, &BasicPhysics::on_window_close);

	// Connect a keyboard handler to on_key_up()
	Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &BasicPhysics::on_input_up);

	// Create the canvas
	Canvas canvas(window);

	// Load a sprite from a png-file
	Sprite spr_logo(canvas, "tux_small.png");
	spr_logo.set_alignment(origin_center);

	//Setup physic world
	PhysicsWorldDescription phys_desc;
	phys_desc.set_gravity(0.0f,10.0f);
	phys_desc.set_sleep(true);
	phys_desc.set_physic_scale(100);

	PhysicsWorld phys_world(phys_desc);

	//Get the Physics Context
	PhysicsContext pc = phys_world.get_pc();

	//Setup ground body
	BodyDescription ground_desc(phys_world);
	ground_desc.set_position(Vec2f((float)window_x_size/2.0f,(float)window_y_size));
	ground_desc.set_type(body_static);

	Body ground(pc, ground_desc);
	//Setup ground fixture
	PolygonShape ground_shape(phys_world);
	ground_shape.set_as_box((float)window_x_size/2,20.0f);

	FixtureDescription fixture_desc(phys_world);
	fixture_desc.set_shape(ground_shape);
	
	Fixture ground_fixture(pc, ground, fixture_desc);

	//Setup box body
	BodyDescription box_desc(phys_world);
	box_desc.set_position(Vec2f(50.0f,100.0f));
	box_desc.set_type(body_dynamic);
	box_desc.set_linear_velocity(Vec2f(100.0f,0.0f));
	Body box(pc, box_desc);

	box_desc.set_angular_velocity(Angle(-120,angle_degrees));
	box_desc.set_position(Vec2f((float)window_x_size-50.0f,100.0f));
	box_desc.set_linear_velocity(Vec2f(-80.0f,0.0f));
	Body box2(pc, box_desc);

	//Setup box fixture
	PolygonShape box_shape(phys_world);
	box_shape.set_as_box((float)spr_logo.get_width()/2, (float)spr_logo.get_height()/2);

	FixtureDescription fixture_desc2(phys_world);
	fixture_desc2.set_shape(box_shape);
	fixture_desc2.set_restitution(0.6f);
	fixture_desc2.set_friction(0.0005f);

	Fixture box_fixture(pc, box, fixture_desc2);
	Fixture box_fixture2(pc, box2, fixture_desc2);

	Vec2f ground_pos = ground.get_position();

	unsigned int last_time = System::get_time();

	Vec2f pos;
	Angle angle;
	
	//Setup debug draw.
	PhysicsDebugDraw debug_draw(phys_world);
	debug_draw.set_flags(f_shape|f_aabb);

	GraphicContext gc = canvas.get_gc();

	// Run until someone presses escape
	while (!quit)
	{
		unsigned int current_time = System::get_time();
		float time_delta_ms = static_cast<float> (current_time - last_time);
		last_time = current_time;

		canvas.clear();
		canvas.fill_rect(0.0f, (float)canvas.get_height(), (float)canvas.get_width(), canvas.get_height() - 20.0f, Colorf::crimson);
		

		phys_world.step();
		pos = box.get_position();
		angle = box.get_angle();
		
		spr_logo.set_scale(1.0f,1.0f);
		spr_logo.set_angle(angle);
		spr_logo.draw(canvas,pos.x,pos.y);
		
		pos = box2.get_position();
		angle = box2.get_angle();

		spr_logo.set_scale(-1.0f,1.0f);
		spr_logo.set_angle(angle);
		spr_logo.draw(canvas,pos.x,pos.y);
		debug_draw.draw(canvas);
		
		canvas.flush();
		window.flip(1);

		// This call processes user input and other events
		KeepAlive::process(0);

		System::sleep(10);
	}

	return 0;
}

// A key was pressed
void BasicPhysics::on_input_up(const InputEvent &key)
{
	if(key.id == keycode_escape)
	{
		quit = true;
	}
}

// The window was closed
void BasicPhysics::on_window_close()
{
	quit = true;
}


