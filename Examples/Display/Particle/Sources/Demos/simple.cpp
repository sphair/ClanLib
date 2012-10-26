/*
////////////////////////////////////////////////////////////////////////////
// LinearParticle Example - Simple
////////////////////////////////////////////////////////////////////////////

Example of using LinearParticle to make simple trail.

Note : For simplicity, the motion of object in this exampls is frame based
hence the object moves faster in higher FPS. Motions of particles and effects
in other examples are calculated in hybrid of time based and frame based
system which is much more flexible and robust but more complicated.


 - 31/10/2006

////////////////////////////////////////////////////////////////////////////
*/

#include "precomp.h"
#include "simple.h"
#include "../LinearParticle/L_ParticleSystem.h"
#include "framerate_counter.h"

int DemoSimple::run(DisplayWindow &window)
{
	quit = false;

	// Set the window
	window.set_title("LinearParticle Example - Simple ");

	// Connect the Window close event
	Slot slot_quit = window.sig_window_close().connect(this, &DemoSimple::on_window_close);

	// Connect a keyboard handler to on_key_up()
	Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &DemoSimple::on_input_up);

	// Get the graphic context
	GraphicContext gc = window.get_gc();

	// initialize LinearParticle
	L_ParticleSystem::init();

	// create surface to be used for particle and set the alignment
	Sprite surface(gc,"Resources/light16p.png");
	surface.set_alignment(origin_center);

	// create a sample of particle with life of 5000
	L_Particle particle(&surface,5000);

	// create dropping effect with period of 16
	L_DroppingEffect dropper(0,0,16);

	// add the particle to dropper effect
	dropper.add(&particle);

	// initialize particle effect
	dropper.initialize();

	float x_pos = 320;
	float y_pos = 240;
	float x_vel = 3.0f;
	float y_vel = 3.0f;

	Font font(gc, "tahoma", 16 );

	FramerateCounter frameratecounter;

	// Run until someone presses escape
	while (!quit)
	{
		gc.clear();

		x_pos += x_vel;
		y_pos += y_vel;

		if( x_pos > 640 || x_pos < 0 )
			x_vel = -x_vel;

		if( y_pos > 480 || y_pos < 0 )
			y_vel = -y_vel;

		dropper.set_position(x_pos, y_pos);
			dropper.trigger();

		/* pass frame time to L_ParticleEffect::run(int) for time based system,
			a constant number would be a reference time unit for frame based system. */
		dropper.run(16);

		// draw dropping effect
		L_DrawParticle(gc,dropper);

		frameratecounter.show_fps(gc, font);
		window.flip(0);	// Set to "1" to lock to screen refresh rate
		frameratecounter.frame_shown();

		KeepAlive::process(0);
	}

	// deinitialize LinearParticle
	L_ParticleSystem::deinit();

	return 0;
}

// A key was pressed
void DemoSimple::on_input_up(const InputEvent &key)
{
	if(key.id == KEY_ESCAPE)
	{
		quit = true;
	}
}

// The window was closed
void DemoSimple::on_window_close()
{
	quit = true;
}
