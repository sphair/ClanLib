/*
////////////////////////////////////////////////////////////////////////////
// LinearParticle Example - Circle
////////////////////////////////////////////////////////////////////////////

This example demostrates coloring effect and using dropping effect to make
circular light trail.


- 26/09/2006

////////////////////////////////////////////////////////////////////////////
*/

#include "precomp.h"
#include "circle2.h"
#include "framerate_counter.h"

DemoCircle2::DemoCircle2(clan::DisplayWindow &window) : window(window)
{
	window.set_title("LinearParticle Example - Circle2 ");
	sc.connect(window.sig_window_close(), clan::bind_member(this, &DemoCircle2::on_window_close));
	canvas = clan::Canvas(window);
	sc.connect(window.get_keyboard().sig_key_up(), clan::bind_member(this, &DemoCircle2::on_key_up));

	// initialize LinearParticle
	L_ParticleSystem::init();

	// create surface to be used for particle and set the alignment
	surface = clan::Sprite(canvas,"Resources/light16p.png");
	surface.set_alignment(clan::Origin::center);

	// create 1st sample of particle
	particle = std::make_unique<L_Particle>(&surface, 2000);
	particle->set_size(2.0);
	particle->coloring2( L_Color(255,130,255,20), L_Color(0,255,10,10) );

	// create 2nd sample of particle
	particle2 = std::make_unique<L_Particle>(&surface, 500);
	particle2->sizing2( 12.0, 1.0 );
	particle2->coloring2( L_Color(255,80,40,255), L_Color(0,255,40,40) );

	dropping_period = 16;
	// create dropping effect with period of 16
	dropper = std::make_unique<L_DroppingEffect>(480, 240, dropping_period);

	/* add 1st particle to dropper effect with 0.92 probability to be
	chosen for an emission. */
	dropper->add(particle.get(), 0.92);

	/* add 2nd particle, it takes the remaining probability (0.08
	in this example) if probability is not specified. */
	dropper->add(particle2.get());

	// initialize particle effect
	dropper->initialize();

	font = clan::Font("Arial", 16 );
	last_time = clan::System::get_time();

}

bool DemoCircle2::update()
{
	canvas.clear();

	uint64_t current_time = clan::System::get_time();
	int time_run = current_time - last_time;
	last_time = current_time;

	/* the maximum time step is set to 50milisecs to avoid artifacts
	and errors caused by low frame rate to be less noticeable. */
	while( time_run > 50 )
	{
		run_a_step(50);
		time_run -= 50;
	}

	if( time_run > 0 )
		run_a_step(time_run);

	L_DrawParticle(canvas, dropper.get());

	if( show_menu )
	{
		frameratecounter.show_fps(canvas, font);

		font.draw_text(canvas, 10, 30, clan::string_format("Period (millisecs) : %1", dropping_period));

		font.draw_text(canvas,10,410,"F1 : hide/show menu");
		font.draw_text(canvas,10,425,"Space : trigger random sleep");
		font.draw_text(canvas,10,440,"Up/Down : change dropping period");
	}


	window.flip(0);	// Set to "1" to lock to screen refresh rate
	frameratecounter.frame_shown();

	if (quit)
	{
		// deinitialize LinearParticle
		L_ParticleSystem::deinit();
	}
	return !quit;
}


void DemoCircle2::on_key_up(const clan::InputEvent& key)
{
	if( key.id == clan::keycode_escape )
		quit = true;

	else if( key.id == clan::keycode_up )
	{
		dropping_period++;
		if( dropping_period > 80 )
			dropping_period = 80;

		dropper->set_period(dropping_period);
	}

	else if( key.id == clan::keycode_down )
	{
		dropping_period--;
		if( dropping_period < 5 )
			dropping_period = 5;

		dropper->set_period(dropping_period);
	}

	else if( key.id == clan::keycode_f1)
		show_menu = !show_menu;

	else if( key.id == clan::keycode_space)
		clan::System::sleep(rand()%200+50);

}

void DemoCircle2::on_window_close()
{
	quit = true;
}

void DemoCircle2::run_a_step(int time)
{
	static double rad = 0.0;
	static clan::Vec2f current_pos(480, 240);
	static clan::Vec2f prev_pos;

	rad += 0.0026*time;

	if( rad > L_2PI )
		rad -= L_2PI;

	prev_pos = current_pos;
	current_pos.x = 160*cos(rad)+320;
	current_pos.y = 160*sin(rad)+240;

	L_Vector vel;
	vel.set( (current_pos.x-prev_pos.x)/time,
		(current_pos.y-prev_pos.y)/time );

	/* it's recommended to use L_ParticleEffect::set_velocity() than just
	to use L_ParticleEffect::set_position() if the desired position of effect
	is not static or jumping. */
	dropper->set_velocity(vel);

	dropper->trigger();

	dropper->run(time);

	/* set position(although velocity has been set before) to avoid error
	being accumulated.*/
	dropper->set_position(current_pos.x, current_pos.y);
}
