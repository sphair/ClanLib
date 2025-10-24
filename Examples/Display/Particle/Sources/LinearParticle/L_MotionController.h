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


#ifndef L_MOTIONCONTROLLER
#define L_MOTIONCONTROLLER


#include "L_Vector.h"


class L_Particle;


enum
{
	L_1D_ACCELERATION,
	L_2D_ACCELERATION,
	L_POINT_ACCELERATION
};


//! Motion controller for complex motions of particle(s).
class L_MotionController
{

friend class L_Particle;

private:
	char motion_type;

	L_REAL acceleration_1d;
	L_Vector acceleration_2d;

	L_Vector acceleration_point_pos;
	L_REAL acceleration_point_mag;

	L_REAL speed_limit;
	L_REAL speed_limit_sqr;


public:
	L_MotionController();

	/** Set 1D accleration for particle(s), negative value for deceleration. \n
	Note : The change of 1d acceleration only affects new particles. */
	void set_1d_acceleration(L_REAL acce);

	/** Set 2D accleration for particle(s). */
	void set_2d_acceleration(const L_Vector& vector);

	/** Set point of gravity to attract particle(s). \n
	Note : For time based system, manual sub-stepping is required in order to run
	precisely when this gravitation point acceleration is used (refer to cmotion
	example). */
	void set_point_acceleration(L_REAL x_pos, L_REAL y_pos, L_REAL magnitude);

	/** Set maximum speed limit for particle(s). */
	void set_speed_limit(L_REAL max_speed);

};

#endif
