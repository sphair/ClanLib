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

#ifndef header_input_device_usbmouse
#define header_input_device_usbmouse

#include "API/Core/Math/point.h"
#include "API/Core/System/keep_alive.h"
#include "../../Display/input_device_generic.h"

class CL_DisplayWindow_Generic;

class CL_InputDevice_LinuxUSBMouse : public CL_InputDevice_Generic,
												 public CL_KeepAlive
{
private:
	CL_DisplayWindow_Generic* parent;
	std::string device;
	int fd;

	std::vector<bool> buttons;
	CL_Point mouse_pos;
public:
	CL_InputDevice_LinuxUSBMouse(CL_DisplayWindow_Generic* parent, 
										  const std::string& dev);
	~CL_InputDevice_LinuxUSBMouse();

	//: Returns the x position of the device.
	int get_x() const { return mouse_pos.x; }

	//: Returns the y position of the device.
	int get_y() const { return mouse_pos.y; }

	//: Returns true if the passed key code is down for this device.
	bool get_keycode(int keycode) const;
	
	std::string get_key_name(int id) const { return "mouse-button"; };

	virtual float get_axis(int index) const { return 0; }

	//: Returns the name of the device (i.e. 'Microsoft Sidewinder 3D').
	std::string get_name() const { return "Linux USB Mouse"; }

	//: Return the hardware id/device for this device (i.e. '/dev/input/js0')
	std::string get_device_name() const { return "linuxmouse:" + device; }

	//: Returns the number of axes available on this device.
	int get_axis_count() const { return 0; }

	//: Returns the number of buttons available on this device.
	//- <p>If used on a keyboard, this function returns -1.</p>
	int get_button_count() const { return 0; }

private:
        void send_pointer_move(const CL_Point& pos);
        void send_ball_move(int delta_x, int delta_y);
        void send_key_event(int key, bool press);
        
	//: Init device to Microsoft Explorer so that all buttons are accessible
	void init_explorer_ps2();

	void keep_alive();
};

#endif

// Local Variables: ***
// mode: clanlib ***
// End: ***
