/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
*/

#pragma once

#include "API/Core/Signals/slot_container.h"
#include "API/Display/Window/input_device.h"
#include "API/Display/TargetProviders/input_device_provider.h"
#include "hid.h"
#include <map>

namespace clan
{

class DataBuffer;

class InputDeviceProvider_Win32Hid : public InputDeviceProvider
{
/// \name Construction
/// \{
public:
	InputDeviceProvider_Win32Hid(HANDLE rawinput_device);
	~InputDeviceProvider_Win32Hid();
/// \}

/// \name Attributes
/// \{
public:
	/// \brief Returns the human readable name of the device (i.e. 'Microsoft Sidewinder 3D').
	std::string get_name() const;

	/// \brief Return the hardware id/device for this device (i.e. /dev/input/js0)
	std::string get_device_name() const;

	/// \brief Returns the input device type.
	InputDevice::Type get_type() const;

	/// \brief Friendly key name for specified identifier (A, B, Leertaste, Backspace, Mouse Left, ...).
	/** <p>Note that this key name is localized, meaning it should only be used for menus
	    where the user view key bindings, and not configuration files and such.</p>*/
	std::string get_key_name(int id) const;

	/// \brief Returns true if the passed key code is down for this device.
	/** <p>See keys.h for list of key codes.</p>*/
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
	std::vector<int> get_axis_ids() const;

	/// \brief Returns the current position of a joystick hat.
	/// \return Hat direction in degrees (0-360), or -1 if the hat is centered.
	int get_hat(int index) const;

	/// \brief Returns the number of buttons available on this device.
	/** <p>If used on a keyboard, this function returns -1.</p>*/
	int get_button_count() const;

	/// \brief Returns the input device is in proximity mode. (Atm applicapble only to tablet.)
	/** <p>If used on other devices than tablet, returns false.</p>*/
	bool in_proximity() const { return false; }
/// \}

/// \name Operations
/// \{
public:
	/// \brief Initialize input device provider.
	/** <p>The device field of InputEvent should not be set when emitting events.</p>*/
	void init(Signal_v1<const InputEvent &> *new_sig_provider_event)
	{
		sig_provider_event = new_sig_provider_event;
	}

	/// \brief Sets the position of the device.
	void set_position(int x, int y) { }

	/// \brief Update device
	///
	/// \param peek_only Treat as a request to see if an event would occur
	///
	/// \return true when the device event has occurred
	bool poll(bool peek_only) { return false; }	// Uses automatic updating and event handling via message system

	void update(RAWINPUT *raw_input);

/// \}

/// \name Implementation
/// \{
private:
	void on_dispose();

	HANDLE open_device();
	DataBuffer get_preparse_data();

	void find_names(HANDLE device);
	void find_button_names(HANDLE device, void *preparse_data);
	void find_value_names(HANDLE device, void *preparse_data);

	void update(void *preparse_data, void *report, int report_size);
	void update_buttons(void *preparse_data, void *report, int report_size);
	void update_values(void *preparse_data, void *report, int report_size);

	Hid hid;
	HANDLE rawinput_device;

	Signal_v1<const InputEvent &> *sig_provider_event;

	std::string product_name;
	std::string manufacturer_name;
	std::string serial_number;

	std::vector<bool> buttons;
	std::vector<float> axis_values;
	std::vector<int> hat_values;

	std::vector<int> axis_ids;

	std::vector<std::string> button_names;
	std::vector<std::string> axis_names;
	std::vector<std::string> hat_names;

	std::map<Hid::USAGE, int> usage_to_button_index;
	std::map<Hid::USAGE, int> usage_to_axis_index;
	std::map<Hid::USAGE, int> usage_to_hat_index;

	friend class Win32Window;
/// \}
};

}
