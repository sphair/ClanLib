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


#ifndef L_PARTICLE_H
#define L_PARTICLE_H


#include "L_Vector.h"
#include "L_MotionController.h"


//! clan::Color class
class L_Color
{
public:
	unsigned char a;
	unsigned char r;
    unsigned char g;
	unsigned char b;

	L_Color()
	{}

	L_Color(const L_Color& color)
	{
		a = color.a;
		r = color.r;
		g = color.g;
		b = color.b;
	}

	L_Color(const clan::Color& color)
	{
		a = color.get_alpha();
		r = color.get_red();
		g = color.get_green();
		b = color.get_blue();
	}

	L_Color(unsigned char a,unsigned char r,unsigned char g,unsigned char b)
	{
		this->a = a;
		this->r = r;
		this->g = g;
		this->b = b;
	}

	void set(const L_Color& color)
	{
		a = color.a;
		r = color.r;
		g = color.g;
		b = color.b;
	}
};


typedef void L_PRERUN_USERFUNC(L_Particle&, const void*);


//! Particle entity for being attached to ParticleEffect.
class L_Particle
{

private:
   //GraphicContext canvas;
	clan::Sprite* surface;
	L_Vector vec;
	L_Vector accel_vec_1d_to_2d;
	L_MotionController* motion_controller;

	L_REAL ref_size;
	L_REAL current_size;
	L_REAL current_radian;
	L_Color current_color;
	L_REAL time_elapesed;
	int life;
	int remaining_life;	//remaining life of particle

	//===== coloring type =====
	char coloring_type;
	int switch_time;
	int last_time_e;
	L_Color color1;
	L_Color color2;
	//========================

	//==== sizing varibles ====
	//0 : off, 1 : changes each frame, 2 : surface's size gradually closes to size_r
	char sizing_type;
	L_REAL size_ratio;
	L_REAL size_ratio2;
	//=========================

	//=== rotating variables ===
	char rotating_type;
	L_REAL radian;
	L_REAL radian_rm;
	//==========================

	//=====stating time========
	int coloring_starting_time;
	int sizing_starting_time;
	int rotating_starting_time;
	L_REAL coloring_starting_f;
	L_REAL sizing_starting_f;
	L_REAL rotating_starting_f;
	//==========================

	char coloring_switch;

	L_PRERUN_USERFUNC* prerun_user_func;
	void* prerun_user_data;


	inline void coloring_process(void);
	inline void sizing_process(void);
	inline void rotating_process(void);
	inline void motion_process(void);


	inline void set_coloring_sp(L_REAL begin_at);
	inline void set_sizing_sp(L_REAL begin_at);
	inline void set_rotating_sp(L_REAL begin_at);

public:
	L_REAL x_pos;
	L_REAL y_pos;

	L_Particle();

	/** Copy contructor ( it does not copy all variables ) */
	L_Particle(const L_Particle& par);

	/** Contructor \n
	life_t : pass L_INFINITE_LIFE to have infinite life particle */
	L_Particle(clan::Sprite* surface_t, int life_t);


	void copy_from(const L_Particle& par);


	//======================== Set Attributes Funtions ===============================

	/** Set the current size and reference size of particle, the sizing effect is relative to reference size. */
	void set_size(L_REAL size);

	/** Set the life in miliseconds, pass L_INFINITE_LIFE for infinite life. */
	void set_life(int life_t);

	void set_color(const L_Color& color);

	void set_color(const char a, const char r, const char g, const char b);

	void set_velocity(const L_Vector& v_t);

	void set_position(L_REAL x, L_REAL y);

	void set_rotation(L_REAL radian);

	/** Only use this when rotating2() is used. This function would reset the initial rotation \n
	set by rotating2(). */
	void set_rotation2(L_REAL radian);

	/** Assign a motion controller for complex motion. */
	void set_motion_controller(L_MotionController* motion_ctr);

	/** Set blending mode of particle ( particle is in L_ADDITIVE_BLENDING blending mode \n
	by default ). Available blending mode : L_NORMAL_BLENDING, L_ADDITIVE_BLENDING */
	void set_blending_mode(int mode);

	/** Set user call back function.
	The user function is called at least once per frame before calling run(int). \n
	<small>L_PRERUN_USERFUNC : \n
	&nbsp;&nbsp; void callback_function(L_Particle& particle, const void* user_data)</small> */
	void set_prerun_callback(L_PRERUN_USERFUNC* user_func, void* user_data);

	/** This particle will be removed in run(int) after this function has been called. */
	void remove(void);

	//coloring_type = 1a
	/** Set coloring effect, the particle changes from current color(at "begin_at") to "color1_t"(at the end). */
	void coloring1(const L_Color& color1_t, L_REAL begin_at=0);

	//coloring_type = 1b
	/** Set coloring effect, the color of particle changes from color1(at "begin_at") to "color2"(at the end). */
	void coloring2(const L_Color& color1_t, const L_Color& color2_t, L_REAL begin_at=0);

	//coloring_type = 2
	/** Set coloring effect, the particle keeps switching colors with duration of "switch_time_t"(in milisecs). */
	void coloring3(const L_Color& color1_t, const L_Color& color2_t, unsigned int switch_time_t, L_REAL begin_at=0);

	//sizing_type = 1a
	/** Set sizing effect, the particle changes from current size(at "begin_at") to "size_r"(at the end). */
	void sizing1(L_REAL size_r, L_REAL begin_at=0);

	//sizing_type = 1b
	/** Set sizing effect, the size of particle changes from "size_r1"(at "begin_at") to "size_r2"(at the end). */
	void sizing2(L_REAL size_r1, L_REAL size_r2, L_REAL begin_at=0);

	//sizing_type = 2
	/** Set sizing effect, the size of particle changes in amount of "size_r" value every millisec. */
	void sizing3(L_REAL size_r, L_REAL begin_at=0);

	//rotating type = 1
	/** Set rotating effect, the particle rotates in amount of "radian_t" every millisec. */
	void rotating1(L_REAL radian_t, L_REAL begin_at=0);

	//rotating type = 2
	/** Set rotating effect, the particle rotates in total amount of "radian_t" in its life. */
	void rotating2(L_REAL radian_t, L_REAL begin_at=0);

	//rotating type = 3
	/** Set rotating effect, the particle rotates randomly. */
	void rotating3(L_REAL begin_at=0);

	//rotating type = 4
	/** Set rotating effect, the particle always face to the direction of its velocity. */
	void rotating4(L_REAL begin_at=0);

	/** Disable coloring effect. */
	void disable_coloring(void);

	/** Disable sizing effect. */
	void disable_sizing(void);

	/** Disable rotating effect. */
	void disable_rotating(void);

	//================================== End =========================================



	//======================== Get Attributes Funtions ===============================
	L_REAL get_size(void);

	L_REAL get_ref_size(void);

	L_REAL get_radian(void);

	L_Vector get_velocity(void);

	int get_remaininig_life(void);

	bool is_alive(void);
	//=================================== End ========================================



	// initialization before running, called by particle effect in the creation process
	void initialize(void);

	/** Pass the frame time for this function to activate the particle. */
	void run(int time_elapesed_t);

	/** Draw the particle with screen position shift option. */
	void draw(clan::Canvas &canvas, int x_shift=0, int y_shift=0);

};

#endif
