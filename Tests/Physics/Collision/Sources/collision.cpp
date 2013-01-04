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
#include "collision.h"

int window_x_size = 640;
int window_y_size = 480;

// The start of the Application
int Collision::start(const std::vector<std::string> &args)
{
	//Remove the need to send physic world to every object. Instead send just the description.

	//Fix having two fixtures working weirdly.
	quit = false;

	// Set the window
	DisplayWindowDescription desc;
	desc.set_title("ClanLib Collision Example");
	desc.set_size(Size(window_x_size, window_y_size), true);
	desc.set_allow_resize(false);

	DisplayWindow window(desc);
	
	// Connect the Window close event
	Slot slot_quit = window.sig_window_close().connect(this, &Collision::on_window_close);

	// Connect a keyboard handler to on_key_up()
	Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &Collision::on_input_up);

	// Create the canvas
	Canvas canvas(window);

	//Setup physic world
	PhysicsWorldDescription phys_desc;
	phys_desc.set_gravity(0.0f,10.0f);
	phys_desc.set_sleep(true);
	phys_desc.set_physic_scale(100);
	phys_desc.set_timestep(1.0f/60.0f);
	phys_desc.set_velocity_iterations(8);
	phys_desc.set_position_iterations(3);

	PhysicsWorld phys_world(phys_desc);

	//Setup ground body
	Body ground = create_ground_body(phys_world);

	unsigned int last_time = System::get_time();

	//Setup outline body
	Body outline_body = create_outline_body(phys_world);
	outline_body.set_position(Vec2f(200.0f,200.0f));

	//Setup debug draw.
	PhysicsDebugDraw debug_draw(phys_world);
	debug_draw.set_flags(f_shape);

	GraphicContext gc = canvas.get_gc();
	
	// Run until someone presses escape
	while (!quit)
	{
		unsigned int current_time = System::get_time();
		float time_delta_ms = static_cast<float> (current_time - last_time);
		last_time = current_time;

		canvas.clear();
		
		phys_world.step();
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
void Collision::on_input_up(const InputEvent &key)
{
	if(key.id == keycode_escape)
	{
		quit = true;
	}
}

// The window was closed
void Collision::on_window_close()
{
	quit = true;
}

Body Collision::create_ground_body(PhysicsWorld &phys_world)
{
	PhysicsContext pc = phys_world.get_pc();
	BodyDescription ground_desc(phys_world);
	ground_desc.set_position(Vec2f((float)window_x_size/2.0f,(float)window_y_size));
	ground_desc.set_type(body_static);

	Body ground(pc, ground_desc);

	//Setup ground fixture
	PolygonShape ground_shape(phys_world);
	ground_shape.set_as_box((float)window_x_size/2,20.0f);

	FixtureDescription fixture_desc(phys_world);
	fixture_desc.set_shape(ground_shape);
	fixture_desc.set_friction(1.0f);
	fixture_desc.set_density(1000.0f);

	Fixture ground_fixture(pc, ground, fixture_desc);
	return ground;
}

Body Collision::create_outline_body(PhysicsWorld &phys_world)
{
	PhysicsContext pc = phys_world.get_pc();

	BodyDescription box_desc(phys_world);
	box_desc.set_type(body_dynamic);
	Body box(pc, box_desc);

	//Setup box fixture description.
	//
	//Outlines shouldn't be too complex. Keep it Under 100 points.
	//If it has more, think about splitting it into couple smaller groups.
	//Then create separate fixtures for the same body.
	CollisionOutline outline("test_outline.out");

	ChainShape outline_shape(phys_world);
	outline_shape.create_loop(outline);

	FixtureDescription fixture_desc2(phys_world);
	fixture_desc2.set_shape(outline_shape);
	fixture_desc2.set_restitution(0.6f);
	fixture_desc2.set_friction(0.001f);
	fixture_desc2.set_density(50.0f);

	Fixture box_fixture(pc, box,fixture_desc2);
	return box;
}

