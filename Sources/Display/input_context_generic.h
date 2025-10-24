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

#ifndef header_input_context_generic
#define header_input_context_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include <vector>
#include "API/Display/input_device.h"

class CL_InputEvent;

//: Input events interface.
class CL_InputContext_Generic
{
//! Construction:
public:
	CL_InputContext_Generic();

	~CL_InputContext_Generic();

//! Attributes:
public:
	std::vector<CL_InputDevice> keyboards;

	std::vector<CL_InputDevice> mice;

	std::vector<CL_InputDevice> joysticks;

//! Operations:
public:
	void add_ref();

	void release_ref();

	//: Removes all devices from this input context.
	void clear();

	//: Adds a new keyboard to the input context.
	void add_keyboard(const CL_InputDevice &keyboard);

	//: Adds a new mouse to the input context.
	void add_mouse(const CL_InputDevice &mouse);

	//: Adds a new joystick to the input context.
	void add_joystick(const CL_InputDevice &joystick);

//! Implementation:
private:
	int ref_count;
};

#endif
