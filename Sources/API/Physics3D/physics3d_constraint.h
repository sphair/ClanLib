/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
*/


#pragma once

#include "../Core/Math/vec3.h"
#include "../Core/Math/mat4.h"
#include "../Core/Math/quaternion.h"
#include <string>
#include <memory>

namespace clan
{
/// \addtogroup clanPhysics3D_World clanPhysics3D World
/// \{

class Physics3DWorld;
class Physics3DObject;
class Physics3DConstraint_Impl;

class Physics3DConstraint
{
public:
	Physics3DConstraint();

	bool is_null() const;

	static Physics3DConstraint point_to_point(Physics3DWorld &world, const Physics3DObject &body_a, const Vec3f &pivot_in_a);
	static Physics3DConstraint point_to_point(Physics3DWorld &world, const Physics3DObject &body_a, const Physics3DObject &body_b, const Vec3f &pivot_in_a, const Vec3f &pivot_in_b);
	static Physics3DConstraint hinge(Physics3DWorld &world, const Physics3DObject &body_a, const Vec3f &pivot_in_a, const Quaternionf &axis_in_a, bool use_reference_frame_a = false);
	static Physics3DConstraint hinge(Physics3DWorld &world, const Physics3DObject &body_a, const Physics3DObject &body_b, const Vec3f &pivot_in_a, const Vec3f &pivot_in_b, const Quaternionf &axis_in_a, const Quaternionf &axis_in_b, bool use_reference_frame_a = false);
	static Physics3DConstraint slider(Physics3DWorld &world, const Physics3DObject &body_a, const Physics3DObject &body_b, const Vec3f &pivot_in_a, const Vec3f &pivot_in_b, const Quaternionf &axis_in_a, const Quaternionf &axis_in_b, bool use_reference_frame_a = false);
	static Physics3DConstraint cone_twist(Physics3DWorld &world, const Physics3DObject &body_a, const Vec3f &pivot_in_a, const Quaternionf &axis_in_a);
	static Physics3DConstraint cone_twist(Physics3DWorld &world, const Physics3DObject &body_a, const Physics3DObject &body_b, const Vec3f &pivot_in_a, const Vec3f &pivot_in_b, const Quaternionf &axis_in_a, const Quaternionf &axis_in_b);
	static Physics3DConstraint six_degrees_of_freedom(Physics3DWorld &world, const Physics3DObject &body_a, const Physics3DObject &body_b, const Vec3f &pivot_in_a, const Vec3f &pivot_in_b, const Quaternionf &axis_in_a, const Quaternionf &axis_in_b, bool use_reference_frame_a = false);

	// Six degrees of freedom only:
	void set_linear_lower_limit(const Vec3f &limit);
	void set_linear_upper_limit(const Vec3f &limit);

private:
	std::shared_ptr<Physics3DConstraint_Impl> impl;

	friend class Physics3DObject;
};

}

/// \}
