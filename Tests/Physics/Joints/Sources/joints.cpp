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
#include "Joints.h"

int window_x_size = 640;
int window_y_size = 480;

// The start of the Application
int Joints::start(const std::vector<std::string> &args)
{
	//Remove the need to send physic world to every object. Instead send just the description.

	//Fix having two fixtures working weirdly.
	quit = false;

	// Set the window
	DisplayWindowDescription desc;
	desc.set_title("ClanLib Joints Example");
	desc.set_size(Size(window_x_size, window_y_size), true);
	desc.set_allow_resize(false);

	DisplayWindow window(desc);
	
	// Connect the Window close event
	Slot slot_quit = window.sig_window_close().connect(this, &Joints::on_window_close);

	// Connect a keyboard handler to on_key_up()
	Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &Joints::on_input_up);

	// Create the canvas
	Canvas canvas(window);

	//Setup physic world
	PhysicsWorldDescription phys_desc;
	phys_desc.set_gravity(0.0f,10.0f);
	phys_desc.set_sleep(true);
	phys_desc.set_physic_scale(100);

	PhysicsWorld phys_world(phys_desc);

	//Setup ground body
	Body ground = create_ground_body(phys_world);

	unsigned int last_time = System::get_time();

	//Setup debug draw.
	PhysicsDebugDraw debug_draw(phys_world);
	debug_draw.set_flags(f_shape|f_aabb|f_joint);

	GraphicContext gc = canvas.get_gc();
	//Setup joints

	const int number_of_joints = 2;
	std::vector<Body> bodies_A(number_of_joints);
	std::vector<Body> bodies_B(number_of_joints);
	std::vector<std::shared_ptr<Joint>> Joints(number_of_joints);
	

	for(int i=0; i<number_of_joints; i++)
	{
		bodies_A[i] = create_box_body(phys_world);
		bodies_A[i].set_position(Vec2f(80.0f+80.0f*i,280.0f));

		bodies_B[i] = create_box_body(phys_world);
		bodies_B[i].set_position(Vec2f(80.0f+80.0f*i,440.0f));

		Joints[i] = create_joint(phys_world, bodies_A[i], bodies_B[i], i);
	}

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
void Joints::on_input_up(const InputEvent &key)
{
	if(key.id == keycode_escape)
	{
		quit = true;
	}
}

// The window was closed
void Joints::on_window_close()
{
	quit = true;
}

Body Joints::create_ground_body(PhysicsWorld &phys_world)
{
	//Get the Physics Context
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

Body Joints::create_box_body(PhysicsWorld &phys_world)
{
	//Get the Physics Context
	PhysicsContext pc = phys_world.get_pc();

	BodyDescription box_desc(phys_world);
	box_desc.set_type(body_dynamic);
	Body box(pc, box_desc);

	//Setup box fixture description
	PolygonShape box_shape(phys_world);
	box_shape.set_as_box(20.0f,20.0f);

	FixtureDescription fixture_desc2(phys_world);
	fixture_desc2.set_shape(box_shape);
	fixture_desc2.set_restitution(0.6f);
	fixture_desc2.set_friction(0.001f);
	fixture_desc2.set_density(50.0f);

	Fixture box_fixture(pc, box,fixture_desc2);
	return box;
}

std::shared_ptr<Joint> Joints::create_joint(PhysicsWorld &phys_world, Body &bodyA, Body &bodyB, int type)
{
	//Get the Physics Context
	PhysicsContext pc = phys_world.get_pc();

	switch(type)
	{
	default:
	case 0: // Distance joint
		{
			DistanceJointDescription joint_desc(phys_world);

			joint_desc.set_bodies(bodyA, bodyB, bodyA.get_position(), bodyB.get_position());
			joint_desc.set_damping_ratio(1.0f);
			joint_desc.set_length(100.0f);

			std::shared_ptr<Joint> joint( static_cast<Joint *> (new DistanceJoint(pc, joint_desc)));
			return joint;
		}
	case 1: // Revolute joint
		{
			RevoluteJointDescription joint_desc(phys_world);
			joint_desc.set_bodies(bodyA, bodyB, bodyA.get_position());
			joint_desc.set_as_motor();
			joint_desc.set_motor_speed(Angle(60,angle_degrees));
			joint_desc.set_max_motor_torque(1000);
		
			std::shared_ptr<Joint> joint( static_cast<Joint *> (new RevoluteJoint(pc, joint_desc)));
			return joint;
		}
	}
}
