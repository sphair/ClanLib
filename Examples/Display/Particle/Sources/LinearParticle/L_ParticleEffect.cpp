//===============================================================================
//
// LinearParticle Copyright (c) 2006 Wong Chin Foo
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software in a
// product, an acknowledgment in the product documentation would be
// appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not be
// misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
//===============================================================================


#include "precomp.h"
#include "L_ParticleEffect.h"
#include "L_ParticleMem.h"

L_ParticleEffect::L_ParticleEffect(int period_t, int x, int y)
{
	x_pos = x;
	y_pos = y;

	rotation_distortion = 0;
	size_distortion = 0;
	life_distortion = 0;
	follow_shooting = false;

	num_particle_type = 0;

	cumulative_prob = 0;
	total_auto_p = 0;

	istriggered = false;
	period = period_t;
	counter = period;
	life = L_INFINITE_LIFE;
	addit_vector_enabled = false;
}


L_ParticleEffect::L_ParticleEffect(const L_ParticleEffect& cpy)
{
	num_particle_type = cpy.num_particle_type;

	int i;
	for( i=0; i<num_particle_type; i++ )
	{
		fl_particle[i] = cpy.fl_particle[i];
		particle_prob[i] = cpy.particle_prob[i];
	}

	velocity.set( cpy.velocity );

	x_pos = cpy.x_pos;
	y_pos = cpy.y_pos;

	rotation_distortion = cpy.rotation_distortion;
	size_distortion = cpy.size_distortion;
	life_distortion = cpy.life_distortion;
	follow_shooting = cpy.follow_shooting;

	cumulative_prob = cpy.cumulative_prob;
	total_auto_p = cpy.total_auto_p;

	istriggered = cpy.istriggered;
	period = cpy.period;
	counter = cpy.counter;

	life = cpy.life;

	addit_vector_enabled = cpy.addit_vector_enabled;
	addit_vector = cpy.addit_vector;
}


L_ParticleEffect::~L_ParticleEffect()
{
	std::list<L_Particle*>::iterator iter = particle_list.begin();
	while( iter != particle_list.end() )
	{
		iter = particle_list.erase(iter);
	}
}


int L_ParticleEffect::add(L_Particle* fl_p, L_REAL prob)
{
	//Particle adding fails
	if( num_particle_type >= L_PARTICLE_TYPE_LIMIT )
	{
		#ifdef L_DEBUG_MODE
			std::cout<<"LinearParticle : Could not add more than "<<L_PARTICLE_TYPE_LIMIT<<" particles for an effect."<<std::endl;
		#endif

		return 0;
	}

	else
	{
		fl_particle[num_particle_type] = fl_p;

		if( prob >= 0 )
		{
			if(cumulative_prob + prob > 1)
			{
				particle_prob[num_particle_type] = 1 - cumulative_prob;
				cumulative_prob = 1;
			}

			else
			{
				particle_prob[num_particle_type] = prob;
				cumulative_prob += particle_prob[num_particle_type];
			}
		}

		else
		{
			particle_prob[num_particle_type] = -1;
			total_auto_p++;
		}

		num_particle_type++;

		return 1;
	}
}


void L_ParticleEffect::set_velocity(const L_Vector& v_t)
{
	velocity.set(v_t);
}


void L_ParticleEffect::set_position(L_REAL x, L_REAL y)
{
	x_pos = x;
	y_pos = y;
}


void L_ParticleEffect::set_velocity(L_REAL x_length, L_REAL y_length)
{
	velocity.set( x_length, y_length );
}


void L_ParticleEffect::set_rotation_distortion(L_REAL rotation_dis)
{
	rotation_distortion = rotation_dis;
}


void L_ParticleEffect::set_size_distortion(L_REAL size_dis)
{
	size_distortion = size_dis;
}


void L_ParticleEffect::set_life_distortion(int life_dis)
{
	life_distortion = life_dis;
	if( life_distortion < 0 )
		life_distortion = -life_distortion;
}


void L_ParticleEffect::set_follow_shooting(bool flag)
{
	follow_shooting = flag;
}


void L_ParticleEffect::set_life(int effect_life)
{
	life = effect_life;
}


L_REAL L_ParticleEffect::rand_size(void)
{
	L_REAL current_size_dis = L_RAND_REAL_1() * size_distortion;
	if( rand()%2 == 0 )
		current_size_dis = -current_size_dis;

	return current_size_dis;
}


void L_ParticleEffect::set_addit_vector(const L_Vector& v_t)
{
	addit_vector_enabled = true;
	addit_vector = v_t;
}


void L_ParticleEffect::set_period(int millisecond)
{
	period = millisecond;
}


int L_ParticleEffect::choose_particle(void)
{
	L_REAL dice = L_RAND_REAL_1();
	L_REAL slider = 0;
	int i;
	for( i=0; i<num_particle_type; i++ )
	{
		slider += particle_prob[i];

		if( dice <= slider )
			break;
	}

	return i;
}


void L_ParticleEffect::create_particle(L_REAL in_x, L_REAL in_y, L_Vector* vec_t)
{
	int chosen = choose_particle();

	L_Particle* par_new;
	L_NEW_PAR( par_new, *fl_particle[chosen] );

	L_Vector vec_t2;
	if(addit_vector_enabled == true)
	{
		if(vec_t != NULL)
			vec_t2 = addit_vector + *vec_t;

		else
			vec_t2 = addit_vector;
	}

	else
	{
		if(vec_t != NULL)
			vec_t2 = *vec_t;
	}

	par_new->set_velocity(vec_t2);


	if(follow_shooting)
	{
		par_new->set_rotation(vec_t->get_angle());
	}

	else if(rotation_distortion != 0)
	{
		par_new->set_rotation2((L_RAND_REAL_1()-0.5f)*rotation_distortion);
	}

	if(size_distortion != 0)
	{
		par_new->set_size( par_new->get_ref_size()+rand_size() );
	}

	if(life_distortion != 0)
	{
		int distort = rand() % life_distortion + 1;

		if(rand() % 2 == 0)
			distort = -distort;

		par_new->set_life(par_new->get_remaininig_life()+distort);
	}

	par_new->x_pos = in_x;
	par_new->y_pos = in_y;

	par_new->initialize();

	//add to list
	particle_list.push_back(par_new);
}


void L_ParticleEffect::motion_process(L_REAL time_elapesed)
{
	x_pos += velocity.x*time_elapesed;
	y_pos += velocity.y*time_elapesed;
}


void L_ParticleEffect::creating_process(void)
{
	if( istriggered && life > 0 )
	{
		static int i;
		static int loop_counter;

		i = 0;
		while(i < time_elapesed)
		{
			loop_counter=0;

			//===================================
			int time_elapesed_minus_i = time_elapesed - i;
			int period_minus_counter = period-counter;

			if(time_elapesed_minus_i < period_minus_counter)
			{
				counter += time_elapesed_minus_i;
				i += time_elapesed_minus_i;
				loop_counter += time_elapesed_minus_i;
			}

			else
			{
				counter += period_minus_counter;
				i += period_minus_counter;
				loop_counter += period_minus_counter;
			}
			//===================================

			motion_process(loop_counter);
			activate_particle(loop_counter);

			if(counter >= period)
			{
				howto_emit_particle();
				counter -= period;
			}

		}

		istriggered = false;
		addit_vector_enabled = false;
	}


	else
	{
		motion_process(time_elapesed);
		activate_particle(time_elapesed);
	}
}


L_REAL L_ParticleEffect::get_x_pos(void)
{
	return x_pos;
}


L_REAL L_ParticleEffect::get_y_pos(void)
{
	return y_pos;
}


int L_ParticleEffect::get_particle_num(void)
{
	return particle_list.size();
}


int L_ParticleEffect::get_life(void)
{
	return life;
}


int L_ParticleEffect::get_period(void)
{
	return period;
}


void L_ParticleEffect::initialize(void)
{
	// initialize prob for each added particle
	int i;
	for( i=0; i<num_particle_type; i++ )
	{
		if(particle_prob[i] == -1)
			particle_prob[i] = (1 - cumulative_prob) / total_auto_p;
	}
}


void L_ParticleEffect::draw(clan::Canvas &canvas, int x_shift, int y_shift)
{
	std::list<L_Particle*>::iterator iter = particle_list.begin();
	while( iter != particle_list.end() )
	{
		(*iter)->draw(canvas, x_shift, y_shift);
		iter++;
	}
}


void L_ParticleEffect::activate_particle(int time_elapesed_t)
{
	std::list<L_Particle*>::iterator iter = particle_list.begin();
	while( iter != particle_list.end() )
	{
		if( (*iter)->is_alive() )
		{
			(*iter)->run(time_elapesed_t);
			iter++;
		}

		else
		{
			iter = particle_list.erase(iter);
		}
	}

	//if life is not infinite
	if(life < L_INFINITE_LIFE)
		life -= time_elapesed_t;
}


void L_ParticleEffect::trigger(void)
{
	istriggered = true;
}


void L_ParticleEffect::run(int time_elapesed_t)
{
	time_elapesed = time_elapesed_t;
	creating_process();
}
