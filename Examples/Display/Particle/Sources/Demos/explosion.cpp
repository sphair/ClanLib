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

int DemoExplosion::run(DisplayWindow &window)
{
	window.set_title("LinearParticle Example - Explosion ");

	Slot slot_quit = window.sig_window_close().connect(this, &DemoExplosion::on_window_close);
	GraphicContext gc = window.get_gc();

	Slot slot_key_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &DemoExplosion::on_key_up);
	Slot slot_mouse_up = (window.get_ic().get_mouse()).sig_key_down().connect(this, &DemoExplosion::on_mouse_down);
	Slot slot_mouse_dblclk = (window.get_ic().get_mouse()).sig_key_dblclk().connect(this, &DemoExplosion::on_mouse_down);

	window.hide_cursor();

	// initialize LinearParticle
	L_ParticleSystem::init();

	// create surface to be used for particle and set the alignment
	Sprite surface(gc,"Resources/explosion.png");
	surface.set_alignment(origin_center);

	Sprite pointer_sur(gc,"Resources/pointer.png");
	pointer_sur.set_alignment(origin_center);

	motion_ctrl.set_1d_acceleration(-0.0004);

	L_Particle particle(&surface,500);
	particle.set_color( L_Color(255,110,60,255) );
	particle.coloring2( L_Color(255,255,255,100), L_Color(0,255,60,60) );
	particle.sizing2( 1.0, 2.5 );
	particle.set_motion_controller(&motion_ctrl);

	effect = new L_ExplosionEffect(0,0,16,4,5,0.3f);
	effect->add(&particle);
	effect->set_life(300); //set life of this effect
	effect->set_rotation_distortion(L_2PI);
	effect->set_size_distortion(0.8);
	effect->set_life_distortion(200); //set life distortion for particles
	effect->set_speed_distortion(0.1f);
	effect->initialize();

	// add the effect sample to effect emitter.
	emitter  = new L_EffectEmitter(effect);


	char str[32];
	quit = false;
	show_menu = true;

	Font font(gc, "Arial", 16 );

	FramerateCounter frameratecounter;
	unsigned int last_time = System::get_time();

	InputDevice &mouse = window.get_ic().get_mouse();
	while(!quit)
	{
		gc.clear();
		unsigned int current_time = System::get_time();
		int time_run = current_time - last_time;
		last_time = current_time;

		emitter->run(time_run);
		L_DrawParticle(gc,emitter);

		// draw cross pointer
		pointer_sur.draw(gc, mouse.get_x(), mouse.get_y());


		if( show_menu )
		{
			frameratecounter.show_fps(gc, font);

			sprintf(str,"#Particle : %d", emitter->get_particle_num());
			font.draw_text(gc,10,30,str);

			font.draw_text(gc,10,410,"F1 : hide/show menu");
			font.draw_text(gc,10,425,"Space : trigger random sleep");
			font.draw_text(gc,10,440,"Left Click : Emit an explosion");
		}


		window.flip(0);	// Set to "1" to lock to screen refresh rate
		frameratecounter.frame_shown();

		KeepAlive::process(0);
	}

	window.show_cursor();

	delete emitter;
	delete effect;

	// deinitialize LinearParticle
	L_ParticleSystem::deinit();

	return 0;
}

void DemoExplosion::on_key_up(const InputEvent& key, const InputState&)
{
	if( key.id == KEY_ESCAPE )
		quit = true;

	else if( key.id == KEY_F1)
		show_menu = !show_menu;

	else if( key.id == KEY_SPACE)
		System::sleep(rand()%200+50);
}

void DemoExplosion::on_mouse_down(const InputEvent& key, const InputState&)
{
	// left mouse click to emit an explosion
	if( key.id == MOUSE_LEFT )
		emitter->emit(key.mouse_pos.x, key.mouse_pos.y);
}

void DemoExplosion::on_window_close()
{
	quit = true;
}

