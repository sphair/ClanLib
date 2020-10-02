/*
////////////////////////////////////////////////////////////////////////////
// LinearParticle Example - Shooting
////////////////////////////////////////////////////////////////////////////

This example demostrates using L_ShootingEffect to make an effect.


 - 26/09/2006

////////////////////////////////////////////////////////////////////////////
*/

#include "precomp.h"
#include "shooting.h"
#include "framerate_counter.h"

DemoShooting::DemoShooting(clan::DisplayWindow &window) : window(window)
{
	window.set_title("LinearParticle Example - Shooting ");

	sc.connect(window.sig_window_close(), clan::bind_member(this, &DemoShooting::on_window_close));
	canvas = clan::Canvas(window);
	sc.connect(window.get_keyboard().sig_key_up(), clan::bind_member(this, &DemoShooting::on_key_up));

	// initialize LinearParticle
	L_ParticleSystem::init();

	// create surface to be used for particle and set the alignment
	surface = clan::Sprite(canvas,"Resources/star.png");
	surface.set_alignment(clan::Origin::center);

	motion_ctrl.set_1d_acceleration(-0.0003);

	particle = clan::make_unique<L_Particle>(&surface, 3000);
	particle->set_color(L_Color(255,110,60,255));
	particle->coloring2(L_Color(255,110,60,255),L_Color(0,200,100,255),0.6);
	particle->rotating2(L_2PI);
	particle->set_motion_controller(&motion_ctrl);

	L_Vector shooting_vector;
	shooting_vector.set2( 0.4, L_DEGREE_TO_RADIAN(-90) );
	effect = clan::make_unique<L_ShootingEffect>(460, 360, shooting_vector, 16, 4);
	effect->add(particle.get());
	effect->set_width_interval(100);
	effect->set_angle_interval(L_DEGREE_TO_RADIAN(45));
	effect->set_life_distortion(600);
	effect->set_size_distortion(0.4f);
	effect->initialize();

	quit = false;
	show_menu = true;

	font = clan::Font("Arial", 16 );
	last_time = clan::System::get_time();
}

bool DemoShooting::update()
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


	L_DrawParticle(canvas, effect.get());

	if( show_menu )
	{
		frameratecounter.show_fps(canvas, font);

		font.draw_text(canvas, 10, 30, clan::string_format("#Particle : %1", effect->get_particle_num()));

		font.draw_text(canvas,10,425,"F1 : hide/show menu");
		font.draw_text(canvas,10,440,"Space : trigger random sleep");
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

void DemoShooting::on_key_up(const clan::InputEvent& key)
{
	if( key.id == clan::keycode_escape )
		quit = true;

	else if( key.id == clan::keycode_f1)
		show_menu = !show_menu;

	else if( key.id == clan::keycode_space)
		clan::System::sleep(rand()%200+50);
}

void DemoShooting::on_window_close()
{
	quit = true;
}

void DemoShooting::run_a_step(int time)
{
	static double rad = 0.0;
	static clan::Vec2f current_pos(460, 360);
	static clan::Vec2f prev_pos;

	rad -= 0.002*time;

	if( rad < L_2PI )
		rad += L_2PI;

	prev_pos = current_pos;
	current_pos.x = 140*cos(rad)+320;
	current_pos.y = 140*sin(rad)+360;

	L_Vector vel;
	vel.set( (current_pos.x-prev_pos.x)/time,
				(current_pos.y-prev_pos.y)/time );

	effect->trigger();

	/* it's recommended to use L_ParticleEffect::set_velocity() than just
	to use L_ParticleEffect::set_position() if the desired position of effect
	is not static or jumping. */
	effect->set_velocity(vel);
	effect->run(time);

	/* set position(although velocity has been set before) to avoid error
	being accumulated.*/
	effect->set_position(current_pos.x, current_pos.y);
}

