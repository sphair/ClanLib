/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
**    Sean Heber
*/

#pragma once


#include "API/Display/input_device.h"
#include "API/Display/input_device_provider.h"

class CL_OpenGLWindowProvider_AGL;

class CL_InputDeviceProvider_MacKeyboard : public CL_InputDeviceProvider
{
/// \name Construction
/// \{

public:
	CL_InputDeviceProvider_MacKeyboard(CL_OpenGLWindowProvider_AGL *window);

	~CL_InputDeviceProvider_MacKeyboard();

	void destroy() { /* do not delete this here. Handled by CL_OpenGLWindowProvider_AGL. */ }


/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns the name of the device (i.e. 'Microsoft Sidewinder 3D').
	CL_String get_name() const;

	/// \brief Return the hardware id/device for this device (i.e. /dev/input/js0)
	CL_String get_device_name() const;

	/// \brief Returns the input device type.
	CL_InputDevice::Type get_type() const { return CL_InputDevice::keyboard; }

	/// \brief Key name for specified identifier (A, B, C, Space, Enter, Backspace).
	CL_String get_key_name(int id) const;

	/// \brief Returns true if the passed key code is down for this device.
	bool get_keycode(int keycode) const;

	/// \brief Returns the x position of the device.
	/** <p>Only valid for mouse.</p>*/
	int get_x() const { return 0; }

	/// \brief Returns the y position of the device.
	/** <p>Only valid for mouse.</p>*/
	int get_y() const { return 0; }

	/// \brief Returns the the current position of a joystick axis.
	float get_axis(int index) const;

	/// \brief Returns the number of axes available on this device.
	int get_axis_count() const;

	/// \brief Returns the number of buttons available on this device.
	/** <p>If used on a keyboard, this function returns -1.</p>*/
	int get_button_count() const;


/// \}
/// \name Operations
/// \{

public:
	/// \brief Initialize input device provider.
	/** <p>The device field of CL_InputEvent should not be set when emitting events.</p>*/
	void init(CL_Signal_v1<const CL_InputEvent &> *new_sig_provider_event)
	{
		sig_provider_event = new_sig_provider_event;
	}

	/// \brief Sets the position of the device.
	void set_position(int x, int y) { return; }


/// \}
/// \name Implementation
/// \{

private:
	CL_Signal_v1<const CL_InputEvent &> *sig_provider_event;

	CL_OpenGLWindowProvider_AGL *window;

	friend class CL_OpenGLWindowProvider_AGL;
/// \}
};


