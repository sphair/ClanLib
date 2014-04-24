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
#include "../Window/input_device.h"
#include "../../Core/System/disposable_object.h"
#include <memory>
#include "../../Core/Signals/signal.h"

namespace clan
{
/// \addtogroup clanDisplay_Input clanDisplay Input
/// \{

/// \brief Interface for implementing a InputDevice source.
class CL_API_DISPLAY InputDeviceProvider : public DisposableObject
{
/// \name Construction
/// \{

public:
	virtual ~InputDeviceProvider() { return; }

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns the human readable name of the device (i.e. 'Microsoft Sidewinder 3D').
	virtual std::string get_name() const = 0;

	/// \brief Return the hardware id/device for this device (i.e. /dev/input/js0)
	virtual std::string get_device_name() const = 0;

	/// \brief Returns the input device type.
	virtual InputDevice::Type get_type() const = 0;

	/// \brief Friendly key name for specified identifier (A, B, Leertaste, Backspace, Mouse Left, ...).
	/** <p>Note that this key name is localized, meaning it should only be used for menus
	    where the user view key bindings, and not configuration files and such.</p>*/
	virtual std::string get_key_name(int id) const = 0;

	/// \brief Returns true if this provider implements keyid to/from string mapping.
	virtual bool supports_keyid_mapping() const { return false; }

	/// \brief Returns a generic string name for the specified key code.
	virtual std::string keyid_to_string(int /* keycode */) const { return std::string(); }

	/// \brief Returns the key code for the specified generic string key name.
	virtual int string_to_keyid(const std::string &/* str */) const { return 0; }

	/// \brief Returns true if the passed key code is down for this device.
	/** <p>See keys.h for list of key codes.</p>*/
	virtual bool get_keycode(int keycode) const = 0;

	/// \brief Returns the x position of the device.
	/** <p>Only valid for mouse.</p>*/
	virtual int get_x() const = 0;

	/// \brief Returns the y position of the device.
	/** <p>Only valid for mouse.</p>*/
	virtual int get_y() const = 0;

	/// \brief Returns the the current position of a joystick axis.
	virtual float get_axis(int index) const = 0;

	/// \brief Returns the number of axes available on this device.
	virtual std::vector<int> get_axis_ids() const = 0;

	/// \brief Returns the current position of a joystick hat.
	/// \return Hat direction in degrees (0-360), or -1 if the hat is centered.
	virtual int get_hat(int /* index */) const { return -1; }

	/// \brief Returns the number of buttons available on this device.
	/** <p>If used on a keyboard, this function returns -1.</p>*/
	virtual int get_button_count() const = 0;

	/// \brief Returns the input device is in proximity mode. (Atm applicapble only to tablet.)
	/** <p>If used on other devices than tablet, returns false.</p>*/
	virtual bool in_proximity() const = 0;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Initialize input device provider.
	/** <p>The device field of InputEvent should not be set when emitting events.</p>
	    <p>Invoking sig_provider_event is thread safe.</p>*/
	virtual void init(Signal<const InputEvent &> *sig_provider_event) = 0;

	/// \brief Sets the position of the device.
	virtual void set_position(int x, int y) = 0;

/// \}
/// \name Implementation
/// \{

private:
/// \}
};

}

/// \}
