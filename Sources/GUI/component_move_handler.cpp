/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include "precomp.h"
#include "API/GUI/component_move_handler.h"
#include "API/GUI/component.h"
#include "API/Display/keys.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ComponentMoveHandler_Generic class:

class CL_ComponentMoveHandler_Generic
{
public:
	CL_ComponentMoveHandler_Generic(CL_ComponentMoveHandler *move_handler);

	void on_mouse_down(const CL_InputEvent &key);
	void on_mouse_up(const CL_InputEvent &key);
	void on_mouse_move(const CL_InputEvent &key);

	CL_Slot slot_mouse_down;
	CL_Slot slot_mouse_up;
	CL_Slot slot_mouse_move;

	bool is_active;
	CL_Point mouse_origin;

	CL_ComponentMoveHandler *move_handler;
};

/////////////////////////////////////////////////////////////////////////////
// CL_ComponentMoveHandler:

CL_ComponentMoveHandler::CL_ComponentMoveHandler(CL_Component *owner)
: CL_Component(owner)
{
	impl = new CL_ComponentMoveHandler_Generic(this);
}

CL_ComponentMoveHandler::CL_ComponentMoveHandler(const CL_Rect &move_area, CL_Component *owner)
: CL_Component(move_area, owner)
{
	impl = new CL_ComponentMoveHandler_Generic(this);
}

CL_ComponentMoveHandler::~CL_ComponentMoveHandler()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ComponentMoveHandler_Generic implementation:

CL_ComponentMoveHandler_Generic::CL_ComponentMoveHandler_Generic(CL_ComponentMoveHandler *move_handler)
: move_handler(move_handler)
{
	is_active = false;

	slot_mouse_down = move_handler->sig_mouse_down().connect(
		this, &CL_ComponentMoveHandler_Generic::on_mouse_down);

	slot_mouse_up = move_handler->sig_mouse_up().connect(
		this, &CL_ComponentMoveHandler_Generic::on_mouse_up);

	slot_mouse_move = move_handler->sig_mouse_move().connect(
		this, &CL_ComponentMoveHandler_Generic::on_mouse_move);
}

void CL_ComponentMoveHandler_Generic::on_mouse_down(const CL_InputEvent &key)
{
	if(key.id == CL_MOUSE_LEFT)
	{
		mouse_origin = CL_Point((int)key.mouse_pos.x, (int)key.mouse_pos.y);
		is_active = true;
		move_handler->capture_mouse();
	}
}

void CL_ComponentMoveHandler_Generic::on_mouse_up(const CL_InputEvent &key)
{
	if(key.id == CL_MOUSE_LEFT)
	{
		is_active = false;
		move_handler->release_mouse();
	}
}

void CL_ComponentMoveHandler_Generic::on_mouse_move(const CL_InputEvent &key)
{
	if(is_active == false)
		return;

	CL_Component *owner = move_handler->get_parent();
	
	const CL_Rect &pos = owner->get_position();

	int delta_x = key.mouse_pos.x - mouse_origin.x;
	int delta_y = key.mouse_pos.y - mouse_origin.y;

	owner->set_position(pos.left + delta_x, pos.top + delta_y);
}
