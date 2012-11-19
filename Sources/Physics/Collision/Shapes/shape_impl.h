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

#pragma once
#include "../../Box2D/Box2D.h"
#include "API/Physics/Collision/Shapes/shape.h"

namespace clan
{


class Shape_Impl
{
public:
//																						_______________________
//																						C O N S T R U C T O R S

	Shape_Impl() {};
	virtual ~Shape_Impl() { return; }

	b2Shape *get_shape();

//																						___________________
//																						A T T R I B U T E S

//																						___________________
//																						O P E R A T I O N S

//	void Shape_Impl::set_shape (const b2Shape &shape);

//																						___________________________
//																						I M P L E M E N T A T I O N

	ShapeType shape_type;
	b2Shape *shape; //should this be a plain pointer?
};


}