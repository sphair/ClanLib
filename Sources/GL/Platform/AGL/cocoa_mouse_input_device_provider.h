/*
 **  ClanLib SDK
 **  Copyright (c) 1997-2016 The ClanLib Team
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

#include "API/Display/TargetProviders/input_device_provider.h"
#include "API/Core/Text/string_format.h"

namespace clan
{

class CocoaMouseInputDeviceProvider : public InputDeviceProvider
{
public:
	CocoaMouseInputDeviceProvider();
	~CocoaMouseInputDeviceProvider();

	void on_dispose() override { }
	void destroy() { delete this; }
	std::string get_name() const override { return "Mouse"; }
	std::string get_device_name() const override { return "mouse"; }
	InputDevice::Type get_type() const override { return InputDevice::pointer; }
	std::string get_key_name(int id) const override { return string_format("Button %1", id); }
	bool supports_keyid_mapping() const override { return false; }
	std::string keyid_to_string(int keycode) const override { return std::string(); }
	int string_to_keyid(const std::string &str) const override { return 0; }

	bool get_keycode(int keycode) const override;
	float get_x() const override;
	float get_y() const override;
	float get_axis(int index) const override { return 0.0f; }
	int get_axis_count() const { return 0; }
	int get_button_count() const override { return 1; }
	bool in_proximity() const override { return false; }
	void init(Signal<const InputEvent &> *new_sig_provider_event) { sig_provider_event = new_sig_provider_event; }
	void set_position(float x, float y) override { }
	bool poll(bool peek_only) { return false; }

    Signal<const InputEvent &> *sig_provider_event;

    std::vector<Vec2f> touches;
    bool down;

private:
};

}
