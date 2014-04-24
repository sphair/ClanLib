/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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


#pragma once

#include "../api_display.h"
#include "../../Core/Signals/signal.h"
#include "../../Core/Math/point.h"
#include <memory>

namespace clan
{
/// \addtogroup clanDisplay_Input clanDisplay Input
/// \{

class InputEvent;
class InputDeviceProvider;
class InputDevice_Impl;

/// \brief InputDevice
class CL_API_DISPLAY InputDevice
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
	InputDevice();

	/// \brief Constructs a InputDevice
	///
	/// \param provider = Input Device Provider
	InputDevice(InputDeviceProvider *provider);

	/// \brief Constructs a InputDevice
	///
	/// \param impl = Weak Ptr
	InputDevice(std::weak_ptr<InputDevice_Impl> impl);

	~InputDevice();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Returns the provider for this input device
	InputDeviceProvider *get_provider() const;

	/// \brief Returns the human readable name of the device (i.e. 'Microsoft Sidewinder 3D').
	std::string get_name() const;

	/// \brief Return the hardware id/device for this device (i.e. /dev/input/js0)
	std::string get_device_name() const;

	/// \brief Returns the input device type.
	Type get_type() const;

	/// \brief Friendly key name for specified identifier (A, B, Leertaste, Backspace, Mouse Left, ...).
	/** <p>Note that this key name is localized, meaning it should only be used for menus
	    where the user view key bindings, and not configuration files and such.</p>*/
	std::string get_key_name(int id) const;

	/// \brief Returns a generic string name for the specified key code.
	std::string keyid_to_string(int keycode) const;

	/// \brief Returns the key code for the specified generic string key name.
	int string_to_keyid(const std::string &str) const;

	/// \brief Returns true if the passed key code is down for this device.
	/** <p>See keys.h for list of key codes.</p>*/
	bool get_keycode(int keycode) const;

	/// \brief Returns the position (x,y) of the device.
	/** <p>Only valid for pointer devices.</p>*/
	Point get_position() const;

	/// \brief Returns the x position of the device.
	/** <p>Only valid for pointer devices.</p>*/
	int get_x() const;

	/// \brief Returns the y position of the device.
	/** <p>Only valid for pointer devices.</p>*/
	int get_y() const;

	/// \brief Returns the the current position of a joystick axis.
	float get_axis(int axisid) const;

	/// \brief Returns the number of axes available on this device.
	std::vector<int> get_axis_ids() const;

	/// \brief Returns the current position of a joystick hat.
	/// \return Hat direction in degrees (0-360), or -1 if the hat is centered.
	int get_hat(int index) const;

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
	InputDevice &operator =(const InputDevice &copy);

	/// \brief Sets the position of the device.
	/** <p>Only valid for mouse.</p>*/
	void set_position(int x, int y);

/// \}
/// \name Signals
/// \{

public:
	/// \brief Signal emitted when key is pressed.
	Signal<const InputEvent &> &sig_key_down();

	/// \brief Signal emitted when key is released.
	Signal<const InputEvent &> &sig_key_up();

	/// \brief Signal emitted when pointer is moved (absolute movement).
	Signal<const InputEvent &> &sig_pointer_move();

	/// \brief Signal emitted when axis is moved.
	Signal<const InputEvent &> &sig_axis_move();

	/// \brief Signal emitted when the mouse is double-clicked.
	Signal<const InputEvent &> &sig_key_dblclk();

	/// \brief Signal emitted when proximity is entered or exited
	Signal<const InputEvent &> &sig_proximity_change();

/// \}
/// \name Implementation
/// \{

private:
	std::shared_ptr<InputDevice_Impl> impl;

	friend class InputContext_Impl;
/// \}
};

}

/// \}
