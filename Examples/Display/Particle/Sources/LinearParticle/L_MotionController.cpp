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
#include "L_MotionController.h"


L_MotionController::L_MotionController()
{
	set_speed_limit(10.0f);
}


void L_MotionController::set_1d_acceleration(L_REAL acce)
{
	motion_type = L_1D_ACCELERATION;
	acceleration_1d = acce;
}


void L_MotionController::set_2d_acceleration(const L_Vector& vector)
{
	motion_type = L_2D_ACCELERATION;
	acceleration_2d = vector;
}


void L_MotionController::set_point_acceleration(L_REAL x_pos, L_REAL y_pos, L_REAL magnitude)
{
	motion_type = L_POINT_ACCELERATION;

	acceleration_point_pos.set(x_pos,y_pos);
	acceleration_point_mag = magnitude;
}


void L_MotionController::set_speed_limit(L_REAL max_speed)
{
	speed_limit = max_speed;
	speed_limit_sqr = max_speed*max_speed;
}
