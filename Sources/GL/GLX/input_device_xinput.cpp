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

#include <iostream>
#include <cstring>
#include <cstdio>
#include "API/Core/System/log.h"
#include "API/Display/input_device.h"
#include "API/Display/input_event.h"
#include "API/Core/System/clanstring.h"
#include "API/Core/System/error.h"
#include "input_device_xinput.h"
#include "display_window_opengl.h"

#define INVALID_EVENT_TYPE	-1

CL_InputDevice_XInput::CL_InputDevice_XInput(CL_DisplayWindow_OpenGL *owner_, 
															XDeviceInfo* info)
	: owner(owner_),
	  name(info->name),
	  absolute(false),
	  num_keys   (0),
	  motion_type        (INVALID_EVENT_TYPE),
	  button_press_type  (INVALID_EVENT_TYPE),
	  button_release_type(INVALID_EVENT_TYPE),
	  key_press_type     (INVALID_EVENT_TYPE),
	  key_release_type   (INVALID_EVENT_TYPE),
	  proximity_in_type  (INVALID_EVENT_TYPE),
	  proximity_out_type (INVALID_EVENT_TYPE)

{
  type = CL_InputDevice::tablet;  

  //info = find_device_info(owner->get_display(), name.c_str(), True);
  //if (!info)
  // throw CL_Error("CL_InputDeviceXInput Error: Couldn't find device: " + name);

  get_info(info);

  if (!register_events(owner->get_display(), info, name.c_str(), True))
  {
	  CL_Log::log("debug", "CL_InputDeviceXInput: Couldn't find device: " + name);
  }
  else
  {
	  slot_xevent = owner->sig_unknown_xevent.connect(this, &CL_InputDevice_XInput::on_xevent);
  }
}

CL_InputDevice_XInput::~CL_InputDevice_XInput()
{
	
}

void
CL_InputDevice_XInput::get_info(XDeviceInfo	*info)
{
	int			i,j;
	XAnyClassPtr	any;
	XKeyInfoPtr		k;
	XButtonInfoPtr	b;
	XValuatorInfoPtr	v;
	XAxisInfoPtr	a;

	//printf("\"%s\"\tid=%ld\t[%s]\n", info->name, info->id,
	//		 (info->use == IsXExtensionDevice) ? "XExtensionDevice" :
//			 ((info->use == IsXPointer) ? "XPointer" : "XKeyboard"));

	if (info->num_classes > 0) {
		any = (XAnyClassPtr) (info->inputclassinfo);
		for (i=0; i<info->num_classes; i++) {
			switch (any->c_class) {
			case KeyClass:
				k = (XKeyInfoPtr) any;
				//printf("\tNum_keys is %d\n", k->num_keys);
				//printf("\tMin_keycode is %d\n", k->min_keycode);
				//printf("\tMax_keycode is %d\n", k->max_keycode);

				num_keys = k->num_keys;
				break;

			case ButtonClass:
				b = (XButtonInfoPtr) any;
				//printf("\tNum_buttons is %d\n", b->num_buttons);

				buttons.resize(b->num_buttons, false);
				break;

			case ValuatorClass:
				v = (XValuatorInfoPtr) any;
				a = (XAxisInfoPtr) ((char *) v + 
										  sizeof (XValuatorInfo));

				//printf("\tNum_axes is %d\n", v->num_axes);
				//printf("\tMode is %s\n", (v->mode == Absolute) ? "Absolute" : "Relative");
				//printf("\tMotion_buffer is %ld\n", v->motion_buffer);

				absolute = (v->mode == Absolute);

				for (j=0; j<v->num_axes; j++, a++) 
				{
					//printf("\tAxis %d :\n", j);
					//printf("\t\tMin_value is %d\n", a->min_value);
					//printf("\t\tMax_value is %d\n", a->max_value);
					//printf ("\t\tResolution is %d\n", a->resolution);
					
					axis.push_back(AxisInfo(a->min_value, a->max_value, a->resolution));
				}

				break;
		
			default:
				printf ("unknown class\n");
			}
			any = (XAnyClassPtr) ((char *) any + any->length);
		}
	}
}

XDeviceInfo*
CL_InputDevice_XInput::find_device_info(Display	*display,
													 const char		*name,
													 Bool		only_extended)
{
	// FIXME: Not really needed could simply pass XDeviceInfo to the
	// constructor, might however make a nicer interface
	XDeviceInfo	*devices;
	int		loop;
	int		num_devices;
	int		len = strlen(name);
	Bool     is_id = True;
	XID		id = 0;
    
	for(loop=0; loop<len; loop++) {
		if (!isdigit(name[loop])) {
			is_id = False;
			break;
		}
	}

	if (is_id) {
		id = atoi(name);
	}
    
	devices = XListInputDevices(display, &num_devices);

	for(loop=0; loop<num_devices; loop++) {
		if ((!only_extended || (devices[loop].use == IsXExtensionDevice)) &&
			 ((!is_id && strcmp(devices[loop].name, name) == 0) ||
			  (is_id && devices[loop].id == id))) {
			return &devices[loop];
		}
	}
	return NULL;
}

int
CL_InputDevice_XInput::register_events(Display		*dpy,
													XDeviceInfo	*info,
													const char		*dev_name,
													Bool		handle_proximity)
{
	int             number = 0;	/* number of events registered */
	XEventClass     event_list[7];
	int             i;
	XDevice         *device;
	Window          root_win;
	unsigned long   screen;
	XInputClassInfo *ip;

	screen   = DefaultScreen(dpy);
	root_win = RootWindow(dpy, screen);

	device = XOpenDevice(dpy, info->id);

	if (!device) {
		fprintf(stderr, "unable to open device %s\n", dev_name);
		return 0;
	}
    
	if (device->num_classes > 0) {
		for (ip = device->classes, i=0; i<info->num_classes; ip++, i++) {
			switch (ip->input_class) {
			case KeyClass:
				DeviceKeyPress  (device, key_press_type,   event_list[number]); number++;
				DeviceKeyRelease(device, key_release_type, event_list[number]); number++;
				break;

			case ButtonClass:
				DeviceButtonPress  (device, button_press_type,   event_list[number]); number++;
				DeviceButtonRelease(device, button_release_type, event_list[number]); number++;
				break;

			case ValuatorClass:
				DeviceMotionNotify(device, motion_type, event_list[number]); number++;
				if (handle_proximity) {
					ProximityIn (device, proximity_in_type,  event_list[number]); number++;
					ProximityOut(device, proximity_out_type, event_list[number]); number++;
				}
				break;
		
			default:
				fprintf(stderr, "unknown class\n");
				break;
			}
		}

		if (XSelectExtensionEvent(dpy, root_win, event_list, number)) {
			fprintf(stderr, "error selecting extended events\n");
			return 0;
		}
	}

	//std::cout << "### Registered events: " << number << std::endl;
	return number;
}

void
CL_InputDevice_XInput::on_xevent(XEvent &event)
{
	if (0)
		std::cout << this << " event: " 
					 << event.type << " Defs: "
					 << motion_type << " " 
					 << button_press_type << " " 
					 << button_release_type << " " 
					 << key_press_type << " " 
					 << key_release_type << " " 
					 << proximity_out_type << " " 
					 << proximity_in_type << " " 
					 << std::endl;

	std::vector<AxisInfo> old_axis = axis;
	std::vector<bool>     old_buttons = buttons;

  	if (event.type == motion_type) 
	{
		XDeviceMotionEvent *motion = (XDeviceMotionEvent *) &event;

		//printf("motion ");
	    
		for(int loop=0; loop<motion->axes_count; loop++) {
			//printf("a[%d]=%d ", motion->first_axis + loop, motion->axis_data[loop]);
			axis[loop + motion->first_axis].pos = motion->axis_data[loop];
		}
		//printf("\n");
	} 
	else if ((event.type == button_press_type) ||
				(event.type == button_release_type)) 
	{
		int	loop;
		XDeviceButtonEvent *button = (XDeviceButtonEvent *) &event;
	    
		//printf("button %s %d ", (event.type == button_release_type) ? "release" : "press  ", button->button);

		buttons[button->button] = (event.type == button_press_type);
	    
		for(loop=0; loop<button->axes_count; loop++) {
			//printf("a[%d]=%d ", button->first_axis + loop, button->axis_data[loop]);
			axis[loop + button->first_axis].pos = button->axis_data[loop];
		}
		//printf("\n");
	} 
	else if ((event.type == key_press_type) ||
				(event.type == key_release_type)) 
	{
		int	loop;
		XDeviceKeyEvent *key = (XDeviceKeyEvent *) &event;
	    
		//printf("key %s %d ", (event.type == key_release_type) ? "release" : "press  ", key->keycode);
	    
		for(loop=0; loop<key->axes_count; loop++) {
			//printf("a[%d]=%d ", key->first_axis + loop, key->axis_data[loop]);
			axis[loop + key->first_axis].pos = key->axis_data[loop];
		}
		//printf("\n");
	}
	else if ((event.type == proximity_out_type) ||
				(event.type == proximity_in_type))
	{
		int	loop;
		XProximityNotifyEvent *prox = (XProximityNotifyEvent *) &event;
	    
		//printf("proximity %s ", (event.type == proximity_in_type) ? "in " : "out");
	    
		for(loop=0; loop<prox->axes_count; loop++) {
			//printf("a[%d]=%d ", prox->first_axis + loop, prox->axis_data[loop]);
			axis[loop + prox->first_axis].pos = prox->axis_data[loop];
		}
		//printf("\n");
	}
	else 
	{
		//printf("CL_InputDevice_XInput: what's that %d\n", event.type);
	}


	for (std::vector<bool>::size_type i = 0; i < buttons.size(); ++i)
	{
		if (buttons[i] != old_buttons[i])
		{
			CL_InputEvent ie;
			
			ie.id           = i;
			ie.type         = CL_InputEvent::pressed;
			ie.device       = CL_InputDevice(this);
			ie.mouse_pos    = CL_Point(0, 0);
			ie.axis_pos     = 0;
			ie.repeat_count = false;

			sig_axis_move(ie);
		}
	}

	for (std::vector<AxisInfo>::size_type i = 0; i < axis.size(); ++i)
	{
		if (axis[i].pos != old_axis[i].pos)
		{
			CL_InputEvent ie;
			
			ie.id           = i;
			ie.type         = CL_InputEvent::axis_moved;
			ie.device       = CL_InputDevice(this);
			ie.mouse_pos    = CL_Point(0, 0);
			ie.axis_pos     = get_axis(i);
			ie.repeat_count = false;

			sig_axis_move(ie);
		}
	}
}

int
CL_InputDevice_XInput::get_x() const
{
	return 0;
}

int
CL_InputDevice_XInput::get_y() const
{
	return 0;
}

bool
CL_InputDevice_XInput::get_keycode(int keycode) const
{
	if (keycode >=0 && keycode < int(buttons.size()))
	{
		return buttons[keycode];
	}
	else
	{
		return false;
	}
}

std::string
CL_InputDevice_XInput::get_key_name(int id) const
{
	return "button" + CL_String::from_int(id);
}

float
CL_InputDevice_XInput::get_axis(int index) const
{
	if (index >= 0 && index < get_axis_count())
	{
		return float(axis[index].pos)/(axis[index].max_value - axis[index].min_value);
	}
	else
	{
		return 0.0f;
	}
}

int
CL_InputDevice_XInput::get_axis_count() const
{
	return axis.size();
}

std::string
CL_InputDevice_XInput::get_name() const
{
	return name;
}

std::string
CL_InputDevice_XInput::get_device_name() const
{
	return "xinput:" + name;
}

int
CL_InputDevice_XInput::get_button_count() const
{
	return buttons.size();
}

// Local Variables: ***
// mode: clanlib ***
// End: ***
