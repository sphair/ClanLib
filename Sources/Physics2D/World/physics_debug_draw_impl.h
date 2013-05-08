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

#pragma once

#include "../Box2D/Box2D.h"
#include "API/Physics2D/World/physics_debug_draw.h"

namespace clan
{

class Canvas;
class PhysicsWorld_Impl;

class PhysicsDebugDraw_Impl : public b2Draw
{
public:

//																						_______________________
//																						C O N S T R U C T O R S
/// \name Construction
/// \{
	PhysicsDebugDraw_Impl(PhysicsWorld_Impl &pw_impl);

	virtual ~PhysicsDebugDraw_Impl() { return; }
//																						___________________
//																						O P E R A T I O N S
/// \name Operations
/// \{

	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);

	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);

	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);

	void DrawTransform(const b2Transform& xf);

    void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);

    void DrawString(int x, int y, const char* string, ...); 

    void DrawAABB(b2AABB* aabb, const b2Color& color);

//																						_____________
//																						S I G N A L S
	

//																						___________________
//																						A T T R I B U T E S
/// \}
/// \name Attributes
/// \{
public:
	PhysicsWorld_Impl *owner;
	float physic_scale;
	Canvas *used_canvas;
/// \}
};

}
