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

//! clanGUI="Framework"
//! header=gui.h

#ifndef header_component_move_handler
#define header_component_move_handler

#ifdef CL_API_DLL
#ifdef CL_GUI_EXPORT
#define CL_API_GUI __declspec(dllexport)
#else
#define CL_API_GUI __declspec(dllimport)
#endif
#else
#define CL_API_GUI
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "component.h"

class CL_Rect;
class CL_ComponentMoveHandler_Generic;

//: Component Move Handler Class
//- !group=GUI/Framework!
//- !header=gui.h!
class CL_API_GUI CL_ComponentMoveHandler : public CL_Component
{
//! Construction:
public:
	//: Constructor
	CL_ComponentMoveHandler(CL_Component *owner);

	//: Constructor
	CL_ComponentMoveHandler(const CL_Rect &move_area, CL_Component *owner);

	//: Destructor
	virtual ~CL_ComponentMoveHandler();

//! Implementation:
private:
	CL_ComponentMoveHandler_Generic *impl;
};

#endif
