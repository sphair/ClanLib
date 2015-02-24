/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
**    Mark Page
*/

#pragma once

#include "API/Display/Window/input_device.h"
#include "API/Display/TargetProviders/input_device_provider.h"

struct js_event;

namespace clan
{

class X11Window;

class InputDeviceProvider_LinuxJoystick : public InputDeviceProvider
{
/// \name Construction
/// \{

public:
	InputDeviceProvider_LinuxJoystick(X11Window *window, const std::string &device);

	~InputDeviceProvider_LinuxJoystick();

	void destroy() { delete this; }

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Get the file descriptor of the joystick
	int get_fd() const;

	/// \brief Returns the input device type.
	InputDevice::Type get_type() const override { return InputDevice::joystick; }

	/// \brief Returns the x position of the device.
	float get_x() const override;

	/// \brief Returns the y position of the device.
	float get_y() const override;

	/// \brief Returns true if the passed key code is down for this device.
	bool get_keycode(int keycode) const override;

	/// \brief Key name for specified identifier (A, B, C, Space, Enter, Backspace).
	std::string get_key_name(int id) const override;

	/// \brief Returns the the current position of a joystick axis.
	float get_axis(int index) const override;

	/// \brief Returns the name of the device (i.e. 'Microsoft Sidewinder 3D').
	std::string get_name() const override;

	/// \brief Return the hardware id/device for this device (i.e. /dev/input/js0)
	std::string get_device_name() const override;

	/// \brief Returns the number of axes available on this device.
	std::vector<int> get_axis_ids() const override;

	/// \brief Returns the number of buttons available on this device.
	/** <p>If used on a keyboard, this function returns -1.</p>*/
	int get_button_count() const override;

	/// \brief Tablet specific functionality.
	bool in_proximity() const override { return false; }

/// \}
/// \name Operations
/// \{

public:
	/// \brief Initialize input device provider.
	/** <p>The device field of InputEvent should not be set when emitting events.</p>*/
	void init(Signal<void(const InputEvent &)> *new_sig_provider_event) override
	{
		sig_provider_event = new_sig_provider_event;
	}

	/// \brief Sets the position of the device.
	void set_position(float x, float y) override;

	/// \brief Update device
	///
	/// \param peek_only Treat as a request to see if an event would occur
	///
	/// \return true when the device event has occurred
	bool poll(bool peek_only);

/// \}
/// \name Implementation
/// \{

private:
	void on_dispose() override;
	void process_event(js_event event) const;
	void update_states() const;

	Signal<void(const InputEvent &)> *sig_provider_event;

	X11Window *window;

	std::string device;

	int fd;

	mutable std::vector<float> axis_states;
	mutable std::vector<bool>  button_states;

	std::string name;

	mutable bool new_event;

/// \}
};

}
