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

#ifndef header_input_device_directinput
#define header_input_device_directinput

#if _MSC_VER > 1000
#pragma once
#endif

#include "../input_device_generic.h"
#include "display_window_win32.h"
#include "API/Core/System/keep_alive.h"

class CL_InputDevice_DirectInput : public CL_InputDevice_Generic, public CL_KeepAlive
{
//! Construction:
public:
	CL_InputDevice_DirectInput(CL_DisplayWindow_Win32 *window, LPCDIDEVICEINSTANCE device_instance);

	virtual ~CL_InputDevice_DirectInput();

//! Attributes:
public:
	//: Returns the x position of the device.
	virtual int get_x() const;

	//: Returns the y position of the device.
	virtual int get_y() const;

	//: Returns true if the passed key code is down for this device.
	virtual bool get_keycode(int keycode) const;

	//: Key name for specified identifier (A, B, C, Space, Enter, Backspace).
	virtual std::string get_key_name(int id) const;

	//: Returns the the current position of a joystick axis.
	virtual float get_axis(int index) const;

	//: Returns the name of the device (i.e. 'Microsoft Sidewinder 3D').
	virtual std::string get_name() const;

	//: Returns the number of axes available on this device.
	virtual int get_axis_count() const;

	//: Returns the number of buttons available on this device.
	//- <p>If used on a keyboard, this function returns -1.</p>
	virtual int get_button_count() const;

//! Operations:
public:
	//: Sets the position of the device.
	virtual void set_position(int x, int y);

//! Implementation:
private:
	virtual void keep_alive();
	CL_DisplayWindow_Win32 *window;
	static BOOL CALLBACK enum_axes_callback(const DIDEVICEOBJECTINSTANCE* instance, void* context);
	static BOOL CALLBACK enum_button_callback(const DIDEVICEOBJECTINSTANCE* instance, void* context);

	DIDEVICEINSTANCE device_instance;

	LPDIRECTINPUTDEVICE8 directinput_device;
	int button_count;
	int axis_count;
};

#endif
