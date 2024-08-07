/*
////////////////////////////////////////////////////////////////////////////
// LinearParticle Example - UserCollision
////////////////////////////////////////////////////////////////////////////

This example demostrates using user callback function to implement collision
detection for particles.


 - 06/06/2007

////////////////////////////////////////////////////////////////////////////
*/

#include "precomp.h"
#include "usercollision.h"
#include "framerate_counter.h"

DemoUserCollision::DemoUserCollision(clan::DisplayWindow &window) : window(window)
{
	window.set_title("LinearParticle Example - UserCollision ");

	sc.connect(window.sig_window_close(), clan::bind_member(this, &DemoUserCollision::on_window_close));
	canvas = clan::Canvas(window);

	sc.connect(window.get_keyboard().sig_key_up(), clan::bind_member(this, &DemoUserCollision::on_key_up));

	// initialize LinearParticle
	srand(clan::System::get_time());
	L_ParticleSystem::init();

	// create surface to be used for particle and set the alignment
	particle_1_sur = clan::Sprite(canvas, "Resources/sketch.png");
	particle_1_sur.set_alignment(clan::Origin::center);

	particle_2_sur = clan::Sprite(canvas, "Resources/explosion.png");
	particle_2_sur.set_alignment(clan::Origin::center);

	/* If L_INFINITE_LIFE is used, the system will not remove
	the particle automatically. */
	particle_1 = std::make_unique<L_Particle>(&particle_1_sur, L_INFINITE_LIFE);
	particle_1->set_color(L_Color(255,255,150,70));

	// Set callback function.
	particle_1->set_prerun_callback(&check_collision, (void*)this);


	// Setup shooting effect.
	L_Vector shooting_vector;
	shooting_vector.set2( 0.4, L_DEGREE_TO_RADIAN(-90) );
	shooting_eff = std::make_unique<L_ShootingEffect>(320, 400, shooting_vector, 40, 1);
	shooting_eff->add(particle_1.get());
	shooting_eff->set_angle_interval(L_DEGREE_TO_RADIAN(25));
	shooting_eff->set_follow_shooting();
	shooting_eff->initialize();


	// Setup explosion effect
	motion_ctrl.set_1d_acceleration(-0.0004);
	particle_2 = std::make_unique<L_Particle>(&particle_2_sur, 500);
	particle_2->set_color( L_Color(255,110,60,255) );
	particle_2->coloring2( L_Color(255,255,255,100), L_Color(0,255,60,60) );
	particle_2->sizing2( 0.8, 1.5 );
	particle_2->set_motion_controller(&motion_ctrl);

	exp_effect = std::make_unique<L_ExplosionEffect>(0, 0, 16, 4, 5, 0.15f);
	exp_effect->add(particle_2.get());
	exp_effect->set_life(100); //set life of this effect
	exp_effect->set_rotation_distortion(L_2PI);
	exp_effect->set_size_distortion(0.7);
	exp_effect->set_life_distortion(100); //set life distortion for particles
	exp_effect->set_speed_distortion(0.1f);
	exp_effect->initialize();

	exp_emitter = std::make_unique<L_EffectEmitter>(exp_effect.get());

	circle_pos.set( 320, 100 );
	circle_surface = clan::Image(canvas, "Resources/circle.png");
	circle_surface.set_alignment(clan::Origin::center);

	cflight_surface = clan::Image(canvas, "Resources/cflight.png");
	cflight_surface.set_color( clan::Color(255,255,255,220) );
	cflight_surface.set_alignment(clan::Origin::center);

	font = clan::Font("Arial", 16 );
	last_time = clan::System::get_time();

}

bool DemoUserCollision::update()
{
	clan::InputDevice &keyboard = window.get_keyboard();
	triggered = false;

	canvas.clear();

	L_Vector vel;
	if( keyboard.get_keycode(clan::keycode_up) )
		vel.y = -0.2;

	else if( keyboard.get_keycode(clan::keycode_down))
		vel.y  = 0.2;

	if( keyboard.get_keycode(clan::keycode_left))
		vel.x = -0.2;

	else if( keyboard.get_keycode(clan::keycode_right))
		vel.x = 0.2;

	if( keyboard.get_keycode(clan::keycode_z))
		triggered = true;

	shooting_eff->set_velocity(vel);

	uint64_t current_time = clan::System::get_time();
	int time_run = current_time - last_time;
	last_time = current_time;

	while( time_run > 30 )
	{
		run_a_step(30);
		time_run -= 30;
	}

	if( time_run > 0 )
		run_a_step(time_run);

	circle_surface.draw(canvas, circle_pos.x, circle_pos.y);
	L_DrawParticle(canvas, shooting_eff.get());
	L_DrawParticle(canvas, exp_emitter.get());
	cflight_surface.draw(canvas, shooting_eff->x_pos, shooting_eff->y_pos);

	if( show_menu )
	{
		frameratecounter.show_fps(canvas, font);

		font.draw_text(canvas, 10,395,"F1 : hide/show menu");
		font.draw_text(canvas, 10,410,"Space : trigger random sleep");
		font.draw_text(canvas, 10,425,"Z Key : trigger shooting");
		font.draw_text(canvas, 10,440,"Arrow keys : move the shooting source");
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

void DemoUserCollision::on_key_up(const clan::InputEvent& key)
{
	if( key.id == clan::keycode_escape )
		quit = true;

	else if( key.id == clan::keycode_f1)
		show_menu = !show_menu;

	else if( key.id == clan::keycode_space)
		clan::System::sleep(rand()%200+50);
}

void DemoUserCollision::on_window_close()
{
	quit = true;
}

void DemoUserCollision::run_a_step(int time)
{
	if( triggered )
		shooting_eff->trigger();

	shooting_eff->run(time);
	exp_emitter->run(time);
}

void DemoUserCollision::check_collision(L_Particle& particle, const void* user_data)
{
	DemoUserCollision* app_oject = (DemoUserCollision*)user_data;

	// Remove the particle when its position is less than -8
	if( particle.y_pos < -8 )
		particle.remove();

	else
	{
		float circle_radius_sqr = 4096;
		float x_d = particle.x_pos - app_oject->circle_pos.x;
		float y_d = particle.y_pos - app_oject->circle_pos.y;

		/* Remove the projectile particle when it's collided with the big
		circle. Point-circle collision detection is used in this example
		for simplicity reason. */
		if( x_d*x_d + y_d*y_d < circle_radius_sqr )
		{
			app_oject->exp_emitter->emit(particle.x_pos, particle.y_pos);
			particle.remove();
		}
	}
}

