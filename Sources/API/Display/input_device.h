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

#ifndef header_inputdevice
#define header_inputdevice

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

#ifdef _MSC_VER
#pragma warning( disable : 4786)
#endif

#include <string>

#include "../signals.h"

class CL_InputDevice_Generic;
class CL_InputEvent;

//: General input device.
//- !group=Display/Input!
//- !header=display.h!
class CL_API_DISPLAY CL_InputDevice
{
public:
	//: Input device types.
	enum Type
	{
		keyboard,
		mouse,
		joystick,
                tablet,
                unknown
	};

//! Construction:
public:
	//: Constructs an input device.
	CL_InputDevice();

	CL_InputDevice(const CL_InputDevice &copy);

	~CL_InputDevice();

//! Attributes:
public:
	//: Returns the human readable name of the device (i.e. 'Microsoft Sidewinder 3D').
	std::string get_name() const;

	//: Return the hardware id/device for this device (i.e. /dev/input/js0)
	std::string get_device_name() const;

	//: Returns the input device id.
	int get_id() const;

	//: Returns the input device type.
	Type get_type() const;

	//: Friendly key name for specified identifier (A, B, Leertaste, Backspace, Mouse Left, ...).
	//- <p>Note that this key name is localized, meaning it should only be used for menus
	//- where the user view key bindings, and not configuration files and such.</p>
	std::string get_key_name(int id) const;

	//: Returns a generic string name for the specified key code. 
	std::string keyid_to_string(int keycode) const;

	//: Returns the key code for the specified generic string key name.
	int string_to_keyid(const std::string &str) const;

	//: Returns true if the passed key code is down for this device.
	//- <p>See keys.h for list of key codes.</p>
	bool get_keycode(int keycode) const;
	
	//: Returns the x position of the device.
	//- <p>Only valid for mouse.</p>
	int get_x() const;

	//: Returns the y position of the device.
	//- <p>Only valid for mouse.</p>
	int get_y() const;

	//: Returns the the current position of a joystick axis.
	float get_axis(int index) const;

	//: Returns the number of axes available on this device.
	int get_axis_count() const;

	//: Returns the number of buttons available on this device.
	//- <p>If used on a keyboard, this function returns -1.</p>
	int get_button_count() const;

//! Operations:
public:
	CL_InputDevice &operator =(const CL_InputDevice &copy);

	//: Sets the position of the device.
	//- <p>Only valid for mouse.</p>
	void set_position(int x, int y);

//! Signals:
public:
	//: Signal emitted when key is pressed.
	CL_Signal_v1<const CL_InputEvent &> &sig_key_down();

	//: Signal emitted when key is released.
	CL_Signal_v1<const CL_InputEvent &> &sig_key_up();

	//: Signal emitted when the mouse is double-clicked.
	CL_Signal_v1<const CL_InputEvent &> &sig_key_dblclk();

	//: Signal emitted when pointer is moved (absolute movement).
	CL_Signal_v1<const CL_InputEvent &> &sig_pointer_move();

	//: Signal emitted when pointer's ball is moved (relative movement).
	CL_Signal_v1<const CL_InputEvent &> &sig_ball_move();

	//: Signal emitted when axis is moved.
	CL_Signal_v1<const CL_InputEvent &> &sig_axis_move();

	//: Deprecated. Use sig_pointer_move() instead.
	CL_Signal_v1<const CL_InputEvent &> &sig_move();

//! Implementation:
public:
	CL_InputDevice(CL_InputDevice_Generic *impl);

	CL_InputDevice_Generic *impl;
};

#endif
