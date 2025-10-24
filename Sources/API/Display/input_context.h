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

//! clanDisplay="Input"
//! header=display.h

#ifndef header_input_context
#define header_input_context

#ifdef CL_API_DLL
#ifdef CL_DISPLAY_EXPORT
#define CL_API_DISPLAY __declspec(dllexport)
#else
#define CL_API_DISPLAY __declspec(dllimport)
#endif
#else
#define CL_API_DISPLAY
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "../signals.h"

class CL_InputDevice;
class CL_InputEvent;
class CL_InputContext_Generic;

//: Input events interface.
//- !group=Display/Input!
//- !header=display.h!
//- <p>An input context is a collection of inputdevices available in
//- a displaywindow.</p>
class CL_API_DISPLAY CL_InputContext
{
//! Construction:
public:
	//: Constructs an input context.
	CL_InputContext();

	CL_InputContext(const CL_InputContext &copy);

	~CL_InputContext();

//! Attributes:
public:
	//: Returns the number of keyboards available.
	int get_keyboard_count() const;

	//: Returns the number of mice available.
	int get_mouse_count() const;

	//: Returns the number of joysticks available.
	int get_joystick_count() const;

	//: Returns the input device for the specified keyboard.
	CL_InputDevice &get_keyboard(int keyboard = 0);

	//: Returns the input device for the specified mouse.
	CL_InputDevice &get_mouse(int mouse = 0);

	//: Returns the input device for the specified joystick.
	CL_InputDevice &get_joystick(int joystick = 0);

	//: Returns the input device with the given devicename
	CL_InputDevice &get_device(const std::string& devicename);

//! Operations:
public:
	//: Copy assignment operator.
	CL_InputContext &operator =(CL_InputContext &copy);

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
	CL_InputContext_Generic *impl;
};

#endif
