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


#ifndef L_PARTICLEEFFECT_H
#define L_PARTICLEEFFECT_H


#include "L_Particle.h"
#include <list>


//! Particle effect interface
class L_ParticleEffect
{

private:
	L_REAL cumulative_prob;
	int total_auto_p;


protected:
	L_Particle* fl_particle[L_PARTICLE_TYPE_LIMIT];
	L_REAL particle_prob[L_PARTICLE_TYPE_LIMIT];
	int num_particle_type;

	L_Vector velocity;

	L_REAL rotation_distortion;
	L_REAL size_distortion;
	int life_distortion;
	bool follow_shooting;

	int time_elapesed;

	bool istriggered;

	int period;
	int counter;

	int life; //L_INFINITE_LIFE for infinite life

	bool addit_vector_enabled;
	L_Vector addit_vector;


	//choose one among particles according to its probability
	inline int choose_particle(void);

	//get distortion value of size among -size_distortion and size_distortion
	L_REAL rand_size(void);

	//create a particle
	void create_particle(L_REAL in_x, L_REAL in_y, L_Vector* vec_t = NULL);

	//make the effect move
	void motion_process(L_REAL time_elapesed);

	//process for creating particle
	void creating_process(void);

	//function for inherited children to specify the particle creation method
	virtual void howto_emit_particle(void) = 0;


public:
	std::list<L_Particle*> particle_list;
	L_REAL x_pos;
	L_REAL y_pos;

	/**
	period : time(milisec) interval between two emissions, 1 <= period < infinity \n
	v_t : velocity for this effect */
	L_ParticleEffect(int period_t, int x, int y);

	/** Copy contructor.\n
	Note : this does not copy particle list. */
	L_ParticleEffect(const L_ParticleEffect& cpy);

	/** Destructor */
	virtual ~L_ParticleEffect();

	/** Add a particle type for the effect and probability for the particle to be chosen for every emission. \n
	Note : If no probabily is specified, the particle will get the remaining probability to 1 ( if more than 1 probability non-specified particle,
	remaining probability to 1 will be equally divided for each. Maximum number of particle types is limited to 8. */
	int add(L_Particle* fl_p, L_REAL prob=-1);

	/** Copy effect's velocity from "v_t". */
	void set_velocity(const L_Vector& v_t);

	/** Set effect's velocity(vector) using X length and Y length. \n
	Note : Use this function rather than set_position() if possible, internally there is calculation to have better
	motion even in jecky framerate environment. If you require to control exact position with no any accumulated
	error, call additional set_position() after run().*/
	void set_velocity(L_REAL x_length, L_REAL y_length);

	/** Set position. */
	void set_position(L_REAL x, L_REAL y);

	/** Apply distortion for particles' initial rotation (in radian). \n
	rotation_dis : 0 - 2 PI, 0 to disable the distortion */
	void set_rotation_distortion(L_REAL rotation_dis);

	/** Apply distortion for particles' size. \n
	size_dis : 0 to disable the distortion */
	void set_size_distortion(L_REAL size_dis);

	/** Apply distortion for particles' life. \n
	life_dis : 0 to disable the distortion */
	void set_life_distortion(int life_dis);

	/** Set the effect's life, L_INFINITE_LIFE for infinite life. */
	void set_life(int effect_life);

	/** Set the particles facing to the direction of its initial velocity. \n
	Note : Rotation distortion will be disabled, if you need the particles always to face its
	velocity direction even after the velocity has been changed, please use L_Particle::rotating4(). */
	void set_follow_shooting(bool flag=true);

	/** Add additional vector for emitted particle, can be used for interia effect */
	void set_addit_vector(const L_Vector& v_t);

	/** Set period for emission in milli second. */
	void set_period(int millisecond);


	//======================== Get Attributes Funtions ===============================
	L_REAL get_x_pos(void);

	L_REAL get_y_pos(void);

	int get_particle_num(void);

	int get_life(void);

	int get_period(void);
	//================================================================================

	/** Trigger particle emission. */
	void trigger(void);

	/** Initialization, must be called (once) before calling run(int). */
	void initialize(void);

	void run(int time_elapesed_t);

	void draw(clan::Canvas &canvas, int x_shift=0, int y_shift=0);

	/** Create a clone this particle effect. */
	virtual L_ParticleEffect* new_clone(void) = 0;

	/** Run all particles in the list( by calling run(int) function of particles ) with certain time. \n
	Basically this is used to resolve the inaccuracy of particle emission and movement when the frame rate is low,
	normal users should not use this function unless there is intention to write custom ParticleEffect. */
	virtual void activate_particle(int time_elapesed_t);

};


#endif
