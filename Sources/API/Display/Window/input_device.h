/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
**    Harry Storbacka
*/

/// \addtogroup clanDisplay_Input clanDisplay Input
/// \{

#pragma once

#include "../api_display.h"
#include "../../Core/System/sharedptr.h"
#include "../../Core/System/weakptr.h"
#include "../../Core/Text/string_types.h"
#include "../../Core/Signals/signal_v2.h"
#include "../../Core/Math/point.h"

class CL_InputEvent;
class CL_InputState;
class CL_InputDeviceProvider;
class CL_InputDevice_Impl;

/// \brief CL_InputDevice
///
/// \xmlonly !group=Display/Input! !header=display.h! \endxmlonly
class CL_API_DISPLAY CL_InputDevice
{
public:
	/// \brief Input device types.
	enum Type
	{
		keyboard,
		pointer,
		joystick,
		tablet,
		unknown
	};

/// \name Construction
/// \{

public:
	/// \brief Constructs a null instance.
	CL_InputDevice();

	/// \brief Constructs a InputDevice
	///
	/// \param provider = Input Device Provider
	CL_InputDevice(CL_InputDeviceProvider *provider);

	/// \brief Constructs a InputDevice
	///
	/// \param impl = Weak Ptr
	CL_InputDevice(CL_WeakPtr<CL_InputDevice_Impl> impl);

	~CL_InputDevice();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Returns the provider for this input device
	CL_InputDeviceProvider *get_provider() const;

	/// \brief Returns the human readable name of the device (i.e. 'Microsoft Sidewinder 3D').
	CL_String get_name() const;

	/// \brief Return the hardware id/device for this device (i.e. /dev/input/js0)
	CL_String get_device_name() const;

	/// \brief Returns the input device type.
	Type get_type() const;

	/// \brief Friendly key name for specified identifier (A, B, Leertaste, Backspace, Mouse Left, ...).
	/** <p>Note that this key name is localized, meaning it should only be used for menus
	    where the user view key bindings, and not configuration files and such.</p>*/
	CL_String get_key_name(int id) const;

	/// \brief Returns a generic string name for the specified key code.
	CL_String keyid_to_string(int keycode) const;

	/// \brief Returns the key code for the specified generic string key name.
	int string_to_keyid(const CL_String &str) const;

	/// \brief Returns true if the passed key code is down for this device.
	/** <p>See keys.h for list of key codes.</p>*/
	bool get_keycode(int keycode) const;

	/// \brief Returns the position (x,y) of the device.
	/** <p>Only valid for pointer devices.</p>*/
	CL_Point get_position() const;

	/// \brief Returns the x position of the device.
	/** <p>Only valid for pointer devices.</p>*/
	int get_x() const;

	/// \brief Returns the y position of the device.
	/** <p>Only valid for pointer devices.</p>*/
	int get_y() const;

	/// \brief Returns the the current position of a joystick axis.
	double get_axis(int index) const;

	/// \brief Returns the number of axes available on this device.
	int get_axis_count() const;

	/// \brief Returns the number of buttons available on this device.
	/** <p>If used on a keyboard, this function returns -1.</p>*/
	int get_button_count() const;

	/// \brief Returns true if the input device is in proximity (applicable for tablets).
	/** <p>Always returns false for non-tablet devices</p>*/
	bool in_proximity() const;

/// \}
/// \name Operations
/// \{

public:
	CL_InputDevice &operator =(const CL_InputDevice &copy);

	/// \brief Sets the position of the device.
	/** <p>Only valid for mouse.</p>*/
	void set_position(int x, int y);

	/// \brief Update device
	///
	/// \param peek_only Treat as a request to see if an event would occur
	///
	/// \return true when the device event has occurred
	bool poll(bool peek_only);

/// \}
/// \name Signals
/// \{

public:
	/// \brief Signal emitted when key is pressed.
	CL_Signal_v2<const CL_InputEvent &, const CL_InputState &> &sig_key_down();

	/// \brief Signal emitted when key is released.
	CL_Signal_v2<const CL_InputEvent &, const CL_InputState &> &sig_key_up();

	/// \brief Signal emitted when pointer is moved (absolute movement).
	CL_Signal_v2<const CL_InputEvent &, const CL_InputState &> &sig_pointer_move();

	/// \brief Signal emitted when axis is moved.
	CL_Signal_v2<const CL_InputEvent &, const CL_InputState &> &sig_axis_move();

	/// \brief Signal emitted when the mouse is double-clicked.
	CL_Signal_v2<const CL_InputEvent &, const CL_InputState &> &sig_key_dblclk();

	/// \brief Signal emitted when proximity is entered or exited
	CL_Signal_v2<const CL_InputEvent &, const CL_InputState &> &sig_proximity_change();

/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_InputDevice_Impl> impl;

	friend class CL_InputContext_Impl;
/// \}
};

/// \}
