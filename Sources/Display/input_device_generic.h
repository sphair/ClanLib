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

#ifndef header_input_device_generic
#define header_input_device_generic

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

#include "API/Display/input_device.h"

class CL_API_DISPLAY CL_InputDevice_Generic
{
//! Construction:
public:
	CL_InputDevice_Generic();

	virtual ~CL_InputDevice_Generic();

//! Attributes:
public:
	//: The input device id.
	int id;

	//: The input device type.
	CL_InputDevice::Type type;

	//: Returns the x position of the device.
	virtual int get_x() const { return 0; }

	//: Returns the y position of the device.
	virtual int get_y() const { return 0; }

	//: Returns true if the passed key code is down for this device.
	virtual bool get_keycode(int keycode) const { return false; }

	//: Key name for specified identifier (A, B, C, Space, Enter, Backspace).
	virtual std::string get_key_name(int id) const = 0;

	//: Returns a generic string name for the specified key code. 
	virtual std::string keyid_to_string(int keycode) const;

	//: Returns the key code for the specified generic string key name.
	virtual int string_to_keyid(const std::string &str) const;

	//: Returns the the current position of a joystick axis.
	virtual float get_axis(int index) const = 0;

	//: Returns the human-readable name of the device (i.e. 'Microsoft Sidewinder 3D').
	virtual std::string get_name() const = 0;

	//: Return the hardware id/device for this device (i.e. '/dev/input/js0')
	virtual std::string get_device_name() const { return ""; }

	//: Returns the number of axes available on this device.
	virtual int get_axis_count() const = 0;

	//: Returns the number of buttons available on this device.
	//- <p>If used on a keyboard, this function returns -1.</p>
	virtual int get_button_count() const = 0;

//! Operations:
public:
	//: Sets the position of the device.
	//- <p>Only valid for mouse.</p>
	virtual void set_position(int x, int y) {};

	void add_ref();

	void release_ref();

//! Signals:
public:
	CL_Signal_v1<const CL_InputEvent &> sig_key_down;

	CL_Signal_v1<const CL_InputEvent &> sig_key_up;

	CL_Signal_v1<const CL_InputEvent &> sig_key_dblclk;

	CL_Signal_v1<const CL_InputEvent &> sig_pointer_move;

	CL_Signal_v1<const CL_InputEvent &> sig_axis_move;

	CL_Signal_v1<const CL_InputEvent &> sig_ball_move;

//! Implementation:
private:
	int ref_count;
};

#endif
