/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
**    Arkadiusz Kalinowski
*/

/// \brief <p>ClanLib physic library.</p>
//! Global=Physic

#pragma once

#ifdef __cplusplus_cli
#pragma managed(push, off)
#endif

#include "Physics2D/api_physics2d.h"
#include "Physics2D/World/physics_world.h"
#include "Physics2D/World/physics_world_description.h"
#include "Physics2D/World/physics_debug_draw.h"
#include "Physics2D/World/physics_context.h"
#include "Physics2D/World/physics_query_assistant.h"
#include "Physics2D/World/query_result.h"
#include "Physics2D/Dynamics/body.h"
#include "Physics2D/Dynamics/body_description.h"
#include "Physics2D/Dynamics/fixture.h"
#include "Physics2D/Dynamics/fixture_description.h"
#include "Physics2D/Dynamics/Joints/joint.h"
#include "Physics2D/Dynamics/Joints/distance_joint_description.h"
#include "Physics2D/Dynamics/Joints/distance_joint.h"
#include "Physics2D/Dynamics/Joints/revolute_joint_description.h"
#include "Physics2D/Dynamics/Joints/revolute_joint.h"
#include "Physics2D/Dynamics/Joints/prismatic_joint_description.h"
#include "Physics2D/Dynamics/Joints/prismatic_joint.h"
#include "Physics2D/Dynamics/Joints/mouse_joint_description.h"
#include "Physics2D/Dynamics/Joints/mouse_joint.h"
#include "Physics2D/Collision/physics_object.h"
#include "Physics2D/Collision/Shapes/shape.h"
#include "Physics2D/Collision/Shapes/polygon_shape.h"
#include "Physics2D/Collision/Shapes/chain_shape.h"
#include "Physics2D/Collision/Shapes/circle_shape.h"
#include "Physics2D/Collision/Shapes/edge_shape.h"

#ifdef __cplusplus_cli
#pragma managed(pop)
#endif

#if defined(_MSC_VER)
	#if !defined(_MT)
		#error Your application is set to link with the single-threaded version of the run-time library. Go to project settings, in the C++ section, and change it to multi-threaded.
	#endif
	#if !defined(_DEBUG)
		#if defined(DLL)
			#pragma comment(lib, "clanPhysics2D-dll.lib")
		#elif defined(_DLL)
			#pragma comment(lib, "clanPhysics2D-static-mtdll.lib")
		#else
			#pragma comment(lib, "clanPhysics2D-static-mt.lib")
		#endif
	#else
		#if defined(DLL)
			#pragma comment(lib, "clanPhysics2D-dll-debug.lib")
		#elif defined(_DLL)
			#pragma comment(lib, "clanPhysics2D-static-mtdll-debug.lib")
		#else
			#pragma comment(lib, "clanPhysics2D-static-mt-debug.lib")

		#endif
	#endif
#endif
