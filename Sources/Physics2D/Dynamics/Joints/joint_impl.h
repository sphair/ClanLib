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
#include "API/Physics2D/Dynamics/Joints/joint.h"
#include "API/Core/Signals/signal_v0.h"

namespace clan
{

class PhysicsWorld_Impl;

class Joint_Impl : public std::enable_shared_from_this<Joint_Impl>
{
public:
//																						_______________________
//																						C O N S T R U C T O R S

	Joint_Impl(PhysicsWorld_Impl *owner);
	virtual ~Joint_Impl();

	virtual b2Joint *get_Joint() { return joint; }

//																						___________________
//																						A T T R I B U T E S
	int get_id() const {return id;}
//																						___________________
//																						O P E R A T I O N S

	virtual void create_joint(b2JointDef &joint_def);
	virtual void remove_joint();
	void set_id(int value) { id = value;}

//																						_____________
//																						S I G N A L S

	Signal_v0 sig_joint_deletion;

//	
//																						___________________________
//																						I M P L E M E N T A T I O N

	PhysicsWorld_Impl *owner_world;
	bool joint_occupied;
	int id;
	JointType joint_type;
	b2Joint *joint;
};


}