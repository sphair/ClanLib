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
#include "shape_impl.h"
#include "chain_shape_impl.h"
#include "API/Physics2D/Collision/Shapes/chain_shape.h"
#include "API/Physics2D/World/physics_world.h"
#include "API/Physics2D/World/physics_context.h"
#include "API/Core/Math/angle.h"
#include "API/display.h"
#include "../../World/physics_world_impl.h"
#include "../../World/physics_context_impl.h"

namespace clan
{
//																											_______________________																											
//																											C O N S T R U C T O R S
ChainShape::ChainShape()
{
	
}

ChainShape::ChainShape(const PhysicsWorld &pw)
: impl(new ChainShape_Impl(pw.impl.get()))
{
	shape_impl->shape_type = shape_chain;
	shape_impl->shape = dynamic_cast<b2Shape*> (&impl->shape);
}

ChainShape::ChainShape(const PhysicsContext &pc)
: impl(new ChainShape_Impl(pc.impl->get_owner()))
{
	shape_impl->shape_type = shape_chain;
	shape_impl->shape = dynamic_cast<b2Shape*> (&impl->shape);
}

ChainShape::~ChainShape()
{

}

//																											___________________																											
//																											A T T R I B U T E S
void ChainShape::throw_if_null() const
{
	if (!impl)
		throw Exception("ChainShape is null");
}

//																											___________________																											
//																											O P E R A T I O N S
ChainShape &ChainShape::operator =(const ChainShape &copy)
{
	impl = copy.impl;
	return *this;
}

void 	ChainShape::create_loop (const Vec2f *vertices,const int count)
{
	b2Vec2 *b2_vertices = new b2Vec2[count]; // delete id 1
	float scale = impl->owner_world->physic_scale;

	for(int i=0; i<count; i++)
	{
		b2_vertices[i].x = vertices[i].x * scale;
		b2_vertices[i].y = vertices[i].y * scale;
	}
	
	impl->shape.CreateLoop(b2_vertices,count);

	delete b2_vertices; // delete id 1
}

void 	ChainShape::create_loop (const CollisionOutline &outline)
{

	std::vector<Contour> contours = outline.get_contours();
	int contour_count = contours.size();
	
	if(contour_count>0)
	{
		std::vector<Pointf> points = contours[0].get_points();
		int point_count = points.size();

		b2Vec2 *b2_vertices = new b2Vec2[point_count]; // delete id 4
		float scale = impl->owner_world->physic_scale;

		for(int i=0; i<point_count; i++)
		{
			b2_vertices[i].x= points[i].x / scale;
			b2_vertices[i].y= points[i].y / scale;
		}

		impl->shape.CreateLoop(b2_vertices,point_count);

		delete b2_vertices; // delete id 4
	}
	else
	throw Exception("ChainShape loop couldn't be created due to CollisionOutline having no contours.");
	
}	


void 	ChainShape::create_chain (const Vec2f *vertices,const int count)
{
	b2Vec2 *b2_vertices = new b2Vec2[count]; // delete id 3
	float scale = impl->owner_world->physic_scale;

	for(int i=0; i<count; i++)
	{
		b2_vertices[i].x = vertices[i].x * scale;
		b2_vertices[i].y = vertices[i].y * scale;
	}

	impl->shape.CreateChain(b2_vertices,count);

	delete b2_vertices; // delete id 3
}

void ChainShape::set_prev_vertex (const Vec2f &prev_vertex)
{
	impl->shape.SetPrevVertex(b2Vec2(prev_vertex.x, prev_vertex.y));
}
void ChainShape::set_next_vertex (const Vec2f &next_vertex)
{
	impl->shape.SetNextVertex(b2Vec2(next_vertex.x, next_vertex.y));
}

}