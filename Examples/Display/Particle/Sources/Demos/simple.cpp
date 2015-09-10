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

DemoSimple::DemoSimple(clan::DisplayWindow &window) : window(window)
{
	quit = false;

	// Set the window
	window.set_title("LinearParticle Example - Simple ");

	// Connect the Window close event
	sc.connect(window.sig_window_close(), clan::bind_member(this, &DemoSimple::on_window_close));

	// Connect a keyboard handler to on_key_up()
	sc.connect(window.get_keyboard().sig_key_up(), clan::bind_member(this, &DemoSimple::on_input_up));

	// Get the graphic context
	canvas = clan::Canvas(window);

	// initialize LinearParticle
	L_ParticleSystem::init();

	// create surface to be used for particle and set the alignment
	surface = clan::Sprite(canvas,"Resources/light16p.png");
	surface.set_alignment(clan::origin_center);

	// create a sample of particle with life of 5000
	particle = clan::make_unique<L_Particle>(&surface, 5000);

	// create dropping effect with period of 16
	dropper = clan::make_unique<L_DroppingEffect>(0, 0, 16);

	// add the particle to dropper effect
	dropper->add(particle.get());

	// initialize particle effect
	dropper->initialize();

	font = clan::Font("tahoma", 16 );
}

bool DemoSimple::update()
{
	canvas.clear();

	x_pos += x_vel;
	y_pos += y_vel;

	if( x_pos > 640 || x_pos < 0 )
		x_vel = -x_vel;

	if( y_pos > 480 || y_pos < 0 )
		y_vel = -y_vel;

	dropper->set_position(x_pos, y_pos);
		dropper->trigger();

	/* pass frame time to L_ParticleEffect::run(int) for time based system,
		a constant number would be a reference time unit for frame based system. */
	dropper->run(16);

	// draw dropping effect
	L_DrawParticle(canvas, dropper.get());

	frameratecounter.show_fps(canvas, font);
	window.flip(0);	// Set to "1" to lock to screen refresh rate
	frameratecounter.frame_shown();

	if (quit)
	{
		// deinitialize LinearParticle
		L_ParticleSystem::deinit();
	}

	return !quit;
}

// A key was pressed
void DemoSimple::on_input_up(const clan::InputEvent &key)
{
	if(key.id == clan::keycode_escape)
	{
		quit = true;
	}
}

// The window was closed
void DemoSimple::on_window_close()
{
	quit = true;
}
