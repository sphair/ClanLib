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

#ifndef header_input_device_linuxjoystick
#define header_input_device_linuxjoystick

#include <vector>
#include <string>

#include "API/Core/Math/point.h"
#include "API/Core/System/keep_alive.h"
#include "../../Display/input_device_generic.h"

class CL_InputDevice_LinuxJoystick : public CL_InputDevice_Generic,
												 public CL_KeepAlive
{
private:
	int fd;
	std::string device;
	std::string name;

	std::vector<float> axes_states;
	std::vector<bool>  button_states;

public:
	CL_InputDevice_LinuxJoystick(const std::string& device);
	~CL_InputDevice_LinuxJoystick();

	std::string get_key_name(int num) const;

	float get_axis(int num)   const { return axes_states[num]; }
	bool  get_keycode(int num) const { return button_states[num]; }
	
	int get_axis_count()   const { return axes_states.size(); }
	int get_button_count() const { return button_states.size(); }

	std::string get_name() const { return name; }
	std::string get_device_name() const { return "linuxjoystick:" + device; }

	void process_event(struct js_event event);
	void keep_alive();

private:
	void send_button_event(const struct js_event& event);
	void send_axis_event(const struct js_event& event);
};

#endif

// Local Variables: ***
// mode: clanlib ***
// End: ***

