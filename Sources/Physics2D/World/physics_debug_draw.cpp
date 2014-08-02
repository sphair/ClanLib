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
**    Arkadiusz Kalinowski
*/

#include "Physics2D/precomp.h"
#include "physics_debug_draw_impl.h"
#include "physics_world_impl.h"
#include "API/Physics2D/World/physics_debug_draw.h"
#include "API/Physics2D/World/physics_world.h"

namespace clan
{

//																											_______________________																											
//																											C O N S T R U C T O R S
PhysicsDebugDraw::PhysicsDebugDraw()
{

}

PhysicsDebugDraw::PhysicsDebugDraw(const PhysicsWorld &pw)
: impl(std::make_shared<PhysicsDebugDraw_Impl>(*pw.impl))
{
	
}

PhysicsDebugDraw::~PhysicsDebugDraw()
{
}

//																											___________________																											
//																											A T T R I B U T E S
void PhysicsDebugDraw::throw_if_null() const
{
	if (!impl)
		throw Exception("PhysicsDebugDraw is null");
}

unsigned int PhysicsDebugDraw::get_flags() const
{
	return impl->GetFlags();
}

//																											_____________																							
//																											S I G N A L S



//																											___________________																											
//																											O P E R A T I O N S

void PhysicsDebugDraw::draw(Canvas &canvas)
{
	impl->used_canvas = &canvas;
	impl->owner->world.DrawDebugData();
}

void PhysicsDebugDraw::set_flags(unsigned int flags)
{
	impl->SetFlags(flags);
}

}
