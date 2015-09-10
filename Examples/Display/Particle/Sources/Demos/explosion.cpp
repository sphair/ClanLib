/*
////////////////////////////////////////////////////////////////////////////
// LinearParticle Example - Explosion
////////////////////////////////////////////////////////////////////////////

This example demostrates using L_EffectEmitter to easily emit multiple
explosion effects.


 - 06/06/2007

////////////////////////////////////////////////////////////////////////////
*/

#include "precomp.h"
#include "explosion.h"
#include "framerate_counter.h"

DemoExplosion::DemoExplosion(clan::DisplayWindow &window) : window(window)
{
 	window.set_title("LinearParticle Example - Explosion ");

	sc.connect(window.sig_window_close(), clan::bind_member(this, &DemoExplosion::on_window_close));
	canvas = clan::Canvas(window);

	sc.connect(window.get_keyboard().sig_key_up(), clan::bind_member(this, &DemoExplosion::on_key_up));
	sc.connect(window.get_mouse().sig_key_down(), clan::bind_member(this, &DemoExplosion::on_mouse_down));
	sc.connect(window.get_mouse().sig_key_dblclk(), clan::bind_member(this, &DemoExplosion::on_mouse_down));

	window.hide_cursor();

	// initialize LinearParticle
	L_ParticleSystem::init();

	// create surface to be used for particle and set the alignment
	surface = clan::Sprite(canvas,"Resources/explosion.png");
	surface.set_alignment(clan::origin_center);

	pointer_sur = clan::Sprite(canvas, "Resources/pointer.png");
	pointer_sur.set_alignment(clan::origin_center);

	motion_ctrl.set_1d_acceleration(-0.0004);

	particle = clan::make_unique<L_Particle>(&surface, 500);

	particle->set_color( L_Color(255,110,60,255) );
	particle->coloring2( L_Color(255,255,255,100), L_Color(0,255,60,60) );
	particle->sizing2( 1.0, 2.5 );
	particle->set_motion_controller(&motion_ctrl);

	effect = clan::make_unique<L_ExplosionEffect>(0, 0, 16, 4, 5, 0.3f);
	effect->add(particle.get());
	effect->set_life(300); //set life of this effect
	effect->set_rotation_distortion(L_2PI);
	effect->set_size_distortion(0.8);
	effect->set_life_distortion(200); //set life distortion for particles
	effect->set_speed_distortion(0.1f);
	effect->initialize();

	// add the effect sample to effect emitter.
	emitter = clan::make_unique<L_EffectEmitter>(effect.get());

	font = clan::Font("Arial", 16 );

	last_time = clan::System::get_time();

}

bool DemoExplosion::update()
{
	clan::InputDevice &mouse = window.get_mouse();

	canvas.clear();
	uint64_t current_time = clan::System::get_time();
	int time_run = current_time - last_time;
	last_time = current_time;

	emitter->run(time_run);
	L_DrawParticle(canvas, emitter.get());

	// draw cross pointer
	pointer_sur.draw(canvas, mouse.get_position().x, mouse.get_position().y);


	if( show_menu )
	{
		frameratecounter.show_fps(canvas, font);

		font.draw_text(canvas, 10, 30, clan::string_format("#Particle : %1", emitter->get_particle_num()));

		font.draw_text(canvas,10,410,"F1 : hide/show menu");
		font.draw_text(canvas,10,425,"Space : trigger random sleep");
		font.draw_text(canvas,10,440,"Left Click : Emit an explosion");
	}


	window.flip(0);	// Set to "1" to lock to screen refresh rate
	frameratecounter.frame_shown();

	if (quit)
	{
		window.show_cursor();

		// deinitialize LinearParticle
		L_ParticleSystem::deinit();
	}

	return !quit;
}

void DemoExplosion::on_key_up(const clan::InputEvent& key)
{
	if( key.id == clan::keycode_escape )
		quit = true;

	else if( key.id == clan::keycode_f1)
		show_menu = !show_menu;

	else if( key.id == clan::keycode_space)
		clan::System::sleep(rand()%200+50);
}

void DemoExplosion::on_mouse_down(const clan::InputEvent& key)
{
	// left mouse click to emit an explosion
	if( key.id == clan::mouse_left )
		emitter->emit(key.mouse_pos.x, key.mouse_pos.y);
}

void DemoExplosion::on_window_close()
{
	quit = true;
}

