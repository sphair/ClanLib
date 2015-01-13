/*
////////////////////////////////////////////////////////////////////////////
// LinearParticle Example - CMotion
////////////////////////////////////////////////////////////////////////////

This example demostrates the effect made by using L_MotionController and
point gravitation (point acceleration).


 - 06/06/2007

////////////////////////////////////////////////////////////////////////////
*/

#include "precomp.h"
#include "cmotion.h"
#include "framerate_counter.h"

DemoCMotion::DemoCMotion() : fontColor(255.0f,255.0f,255.0f), blendingMode(L_ADDITIVE_BLENDING)
{
}

int DemoCMotion::run(clan::DisplayWindow &window)
{
    clan::SlotContainer cc;
	window.set_title("LinearParticle Example - CMotion ");
	cc.connect(window.sig_window_close(), clan::bind_member(this, &DemoCMotion::on_window_close));
	clan::Canvas canvas(window);

	cc.connect(window.get_ic().get_keyboard().sig_key_up(), [&](const clan::InputEvent &input){on_key_up(input, canvas); });

	// initialize LinearParticle
	L_ParticleSystem::init();

	// create surface to be used for particle and set the alignment
	clan::Sprite surface(canvas,"Resources/sketch.png");
	surface.set_alignment(clan::origin_center);
	font = new clan::Font("Arial", 16 );

	motion_ctrl.set_speed_limit(0.65f);
	motion_ctrl.set_point_acceleration( 320, 240, 0.001f );

	particle = new L_Particle(&surface,3000);
	particle->rotating4();
	particle->set_motion_controller(&motion_ctrl);


	L_Vector shooting_vector;
	shooting_vector.set2( 0.4, L_DEGREE_TO_RADIAN(-90) );
	effect = new L_ShootingEffect(460,360,shooting_vector,16,6);
	effect->add(particle);
	effect->set_width_interval(20);
	effect->set_angle_interval(L_DEGREE_TO_RADIAN(40));
	effect->set_life_distortion(600);
	effect->set_size_distortion(0.4f);
	effect->set_speed_distortion(0.06f);
	effect->initialize();

	bg_color = clan::Colorf(0.0f,0.0f,0.0f);


	char str[32];
	quit = false;
	show_menu = true;


	random_ini_rotation = false;
	current_style = 0;
	set_style(canvas);

	FramerateCounter frameratecounter;
	uint64_t last_time = clan::System::get_time();

	while(!quit)
	{
		canvas.clear(bg_color);

		uint64_t current_time = clan::System::get_time();
		int time_run = current_time - last_time;
		last_time = current_time;

		/* the maximum time step is set to 50 milisecs to avoid artifacts
		and errors caused by low frame rate to be less noticeable. */
		while( time_run > 50 )
		{
			run_a_step(50);
			time_run -= 50;
		}

		if( time_run > 0 )
			run_a_step(time_run);

		
		if(blendingMode == L_ADDITIVE_BLENDING)
			L_DrawParticle(canvas,effect);
		else
			L_DrawParticleMinusAlpha(canvas,effect);

		if( show_menu )
		{
			frameratecounter.show_fps(canvas, *font);

			sprintf(str,"#Particle : %d", effect->get_particle_num());
			font->draw_text(canvas,10,30,str,fontColor);

			font->draw_text(canvas,10,395,"F1 : hide/show menu",fontColor);
			font->draw_text(canvas,10,410,"Space : trigger random sleep",fontColor);
			font->draw_text(canvas,10,425,"Z key : change style",fontColor);
			font->draw_text(canvas,10,440,"X Key : toggle randomization for initial rotation",fontColor);
		}


		window.flip(0);	// Set to "1" to lock to screen refresh rate
		frameratecounter.frame_shown();

		clan::RunLoop::process(0);
	}

	delete font;
	delete effect;
	delete particle;

	// deinitialize LinearParticle
	L_ParticleSystem::deinit();
	return 0;
}

void DemoCMotion::set_style(clan::Canvas &canvas)
{
	if( current_style == 0 )
	{
		bg_color = clan::Colorf(0.0f,0.0f,0.0f);

		particle->set_color(L_Color(255,110,60,255));
		particle->coloring2(L_Color(255,110,60,255),L_Color(0,200,100,255),0.8);
		blendingMode = L_ADDITIVE_BLENDING;

		fontColor = clan::Colorf(255.0f,255.0f,255.0f);
	}

	else
	{
		bg_color = clan::Colorf(255.0f,255.0f,255.0f);

		particle->set_color(L_Color(100,0,0,0));
		particle->coloring2(L_Color(100,0,0,0),L_Color(0,0,0,0),0.8);
		blendingMode = L_NORMAL_BLENDING;

		fontColor = clan::Colorf(0.0f,0.0f,0.0f);
	}

}

void DemoCMotion::on_key_up(const clan::InputEvent& key, clan::Canvas canvas)
{
	if( key.id == clan::keycode_escape )
		quit = true;

	else if( key.id == clan::keycode_z)
	{
		current_style = (current_style+1)%2;
		set_style(canvas);
	}

	else if( key.id == clan::keycode_x)
	{
		random_ini_rotation = !random_ini_rotation;
		if( random_ini_rotation )
			effect->set_rotation_distortion(L_DEGREE_TO_RADIAN(200));

		else
			effect->set_rotation_distortion(0);
	}

	else if( key.id == clan::keycode_f1)
		show_menu = !show_menu;

	else if( key.id == clan::keycode_space)
		clan::System::sleep(rand()%200+50);
}

void DemoCMotion::on_window_close()
{
	quit = true;
}

void DemoCMotion::run_a_step(int time)
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

