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
**    Magnus Norddahl
*/

#include "Display/precomp.h"
#include "API/Display/Window/input_event.h"
#include "API/Core/System/exception.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/logger.h"
#include "input_device_provider_directinput.h"
#include "win32_window.h"

/////////////////////////////////////////////////////////////////////////////
// CL_InputDeviceProvider_DirectInput construction:

CL_InputDeviceProvider_DirectInput::CL_InputDeviceProvider_DirectInput(
	CL_Win32Window *window,
	LPCDIDEVICEINSTANCE ptr_device_instance)
: sig_provider_event(0), window(window), device_instance(*ptr_device_instance), directinput_device(0)
{
	HRESULT result = window->get_directinput()->CreateDevice(
		device_instance.guidInstance,
		&directinput_device,
		0);

	if (FAILED(result))
	{
		throw CL_Exception(
			cl_format(
				"Unable to open device %1 (%2)",
				device_instance.tszInstanceName,
				device_instance.tszProductName));
	}

	result = directinput_device->SetCooperativeLevel(window->get_hwnd(), DISCL_FOREGROUND|DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		directinput_device->Release();

		throw CL_Exception(
			cl_format(
				"Unable to the set cooperative level for %1 (%2)",
				device_instance.tszInstanceName,
				device_instance.tszProductName));
	}

	result = directinput_device->SetDataFormat(&c_dfDIJoystick2);
	if (FAILED(result))
	{
		directinput_device->Release();

		throw CL_Exception(
			cl_format(
				"Unable to set device dataformat for %1 (%2)",
				device_instance.tszInstanceName,
				device_instance.tszProductName));
	}

	// Enable buffering of input events:
	DIPROPDWORD value;
	memset(&value, 0, sizeof(DIPROPDWORD));
	value.diph.dwSize = sizeof(DIPROPDWORD);
	value.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	value.diph.dwObj = 0;
	value.diph.dwHow = DIPH_DEVICE;
	value.dwData = 128;
	result = directinput_device->SetProperty(DIPROP_BUFFERSIZE, &value.diph);
	if (FAILED(result))
	{
		directinput_device->Release();

		throw CL_Exception(
			cl_format(
				"Unable to set buffer size attribute on device %1 (%2)",
				device_instance.tszInstanceName,
				device_instance.tszProductName));
	}

	/*
	*** This is not required, and may fail with DIERR_OTHERAPPHASPRIO.
	*** The function update() performs the Acquire().

	result = directinput_device->Acquire();
	if (FAILED(result))
	{
		directinput_device->Release();

		throw CL_Exception(
			cl_format(
				"Unable to acquire device %1 (%2)",
				device_instance.tszInstanceName,
				device_instance.tszProductName));
	}
	*/
}

CL_InputDeviceProvider_DirectInput::~CL_InputDeviceProvider_DirectInput()
{
	dispose();
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDeviceProvider_DirectInput attributes:

int CL_InputDeviceProvider_DirectInput::get_x() const
{
	throw_if_disposed();
	return 0;
}

int CL_InputDeviceProvider_DirectInput::get_y() const
{
	throw_if_disposed();
	return 0;
}

bool CL_InputDeviceProvider_DirectInput::get_keycode(int keycode) const
{
	throw_if_disposed();
	if (keycode < 0 || keycode >= 128) return false;

	DIJOYSTATE2 joystate2;
	HRESULT result = directinput_device->GetDeviceState(sizeof(DIJOYSTATE2), &joystate2);
	if (FAILED(result)) return false;

	return LOBYTE(joystate2.rgbButtons[keycode]) != 0;
}

CL_String CL_InputDeviceProvider_DirectInput::get_key_name(int id) const
{
	throw_if_disposed();
	return cl_format("Joystick button %1", id);
}

float CL_InputDeviceProvider_DirectInput::get_axis(int index) const
{
	throw_if_disposed();
	DIJOYSTATE2 joystate2;
	HRESULT result = directinput_device->GetDeviceState(sizeof(DIJOYSTATE2), &joystate2);
	if (FAILED(result)) return 0;

	switch (index)
	{
	case 0:
		return float(joystate2.lX)/(0xffff/2)-1.0f;

	case 1:
		return float(joystate2.lY)/(0xffff/2)-1.0f;

	case 2:
		return float(joystate2.lZ)/(0xffff/2)-1.0f;

	case 3:
		return float(joystate2.lRx)/(0xffff/2)-1.0f;

	case 4:
		return float(joystate2.lRy)/(0xffff/2)-1.0f;

	case 5:
		return float(joystate2.lRz)/(0xffff/2)-1.0f;

	case 6:
		return float(joystate2.rglSlider[0])/(0xffff/2)-1.0f;

	case 7:
		return float(joystate2.rglSlider[1])/(0xffff/2)-1.0f;

	case 8:
		return float(joystate2.lVX)/(0xffff/2)-1.0f;

	case 9:
		return float(joystate2.lVY)/(0xffff/2)-1.0f;

	case 10:
		return float(joystate2.lVZ)/(0xffff/2)-1.0f;

	case 11:
		return float(joystate2.lVRx)/(0xffff/2)-1.0f;

	case 12:
		return float(joystate2.lVRy)/(0xffff/2)-1.0f;

	case 13:
		return float(joystate2.lVRz)/(0xffff/2)-1.0f;

	case 14:
		return float(joystate2.rglVSlider[0])/(0xffff/2)-1.0f;

	case 15:
		return float(joystate2.rglVSlider[1])/(0xffff/2)-1.0f;

	case 16:
		return float(joystate2.lAX)/(0xffff/2)-1.0f;

	case 17:
		return float(joystate2.lAY)/(0xffff/2)-1.0f;

	case 18:
		return float(joystate2.lAZ)/(0xffff/2)-1.0f;

	case 19:
		return float(joystate2.lARx)/(0xffff/2)-1.0f;

	case 20:
		return float(joystate2.lARy)/(0xffff/2)-1.0f;

	case 21:
		return float(joystate2.lARz)/(0xffff/2)-1.0f;

	case 22:
		return float(joystate2.rglASlider[0])/(0xffff/2)-1.0f;

	case 23:
		return float(joystate2.rglASlider[1])/(0xffff/2)-1.0f;

	case 24:
		return float(joystate2.lFX)/(0xffff/2)-1.0f;

	case 25:
		return float(joystate2.lFY)/(0xffff/2)-1.0f;

	case 26:
		return float(joystate2.lFZ)/(0xffff/2)-1.0f;

	case 27:
		return float(joystate2.lFRx)/(0xffff/2)-1.0f;

	case 28:
		return float(joystate2.lFRy)/(0xffff/2)-1.0f;

	case 29:
		return float(joystate2.lFRz)/(0xffff/2)-1.0f;

	case 30:
		return float(joystate2.rglFSlider[0])/(0xffff/2)-1.0f;

	case 31:
		return float(joystate2.rglFSlider[1])/(0xffff/2)-1.0f;

	case 32:
		return float(joystate2.rgdwPOV[0]);

	case 33:
		return float(joystate2.rgdwPOV[1]);

	case 34:
		return float(joystate2.rgdwPOV[2]);

	case 35:
		return float(joystate2.rgdwPOV[3]);
	}

	return 0.0f;
}

CL_String CL_InputDeviceProvider_DirectInput::get_name() const
{
	throw_if_disposed();
	return device_instance.tszInstanceName;
}

CL_String CL_InputDeviceProvider_DirectInput::get_device_name() const
{
	throw_if_disposed();
	return device_instance.tszInstanceName;
}

int CL_InputDeviceProvider_DirectInput::check_axis(DWORD dwOfs) const
{
	throw_if_disposed();
	DIDEVICEOBJECTINSTANCE didoi;
	HRESULT hr;

	didoi.dwSize = sizeof( didoi );
	hr=directinput_device->GetObjectInfo( &didoi, dwOfs, DIPH_BYOFFSET );
	if ( SUCCEEDED( hr ) )
	{
		return 1;
	}
	return 0;
}

int CL_InputDeviceProvider_DirectInput::get_axis_count() const
{
	throw_if_disposed();
	int count=0;

	// Struct DIJOYSTATE contains maximums
	count += check_axis(DIJOFS_SLIDER(0)); 
	count += check_axis(DIJOFS_SLIDER(1)); 

	// Should the hat be classified as an axis?
	count += check_axis(DIJOFS_POV(0)); 
	count += check_axis(DIJOFS_POV(1)); 
	count += check_axis(DIJOFS_POV(2)); 
	count += check_axis(DIJOFS_POV(3)); 

	count += check_axis(DIJOFS_RX); 
	count += check_axis(DIJOFS_RY); 
	count += check_axis(DIJOFS_RZ); 
	count += check_axis(DIJOFS_X); 
	count += check_axis(DIJOFS_Y); 
	count += check_axis(DIJOFS_Z); 

	return count;
}

int CL_InputDeviceProvider_DirectInput::get_button_count() const
{
	throw_if_disposed();
	int count=0;

	for ( int x = 0; x < 32; x++ )
	{
		count += check_axis(DIJOFS_BUTTON( x )); 
	}
	return count;
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDeviceProvider_DirectInput operations:

void CL_InputDeviceProvider_DirectInput::set_position(int x, int y)
{
	throw_if_disposed();
		//TODO: Fixme
}

void CL_InputDeviceProvider_DirectInput::update()
{
	throw_if_disposed();
	directinput_device->Poll();

	// Get events:
	while (true)
	{
		DIDEVICEOBJECTDATA buffer[16];
		DWORD num_events = 16;

		HRESULT result = directinput_device->GetDeviceData(
			sizeof(DIDEVICEOBJECTDATA),
			buffer,
			&num_events,
			0);

		// Try to reacquire joystick if we lost it.
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED) directinput_device->Acquire();

		if (FAILED(result) && result != DI_BUFFEROVERFLOW) break;
		if (num_events == 0) break;

		// Ok here follows the decoding of events.

		for (unsigned int i=0; i<num_events; i++)
		{
			CL_InputEvent event;
			event.mouse_pos = window->mouse_pos;
			event.repeat_count = 0;

			if (buffer[i].dwOfs == FIELD_OFFSET(DIJOYSTATE2, lX))
			{
				// X Axis position event
				event.type = CL_InputEvent::axis_moved;
				event.id = 0;
				event.axis_pos = float(buffer[i].dwData)/(0xffff/2)-1.0f;
				if (sig_provider_event)
					sig_provider_event->invoke(event);
			}
			else if (buffer[i].dwOfs == FIELD_OFFSET(DIJOYSTATE2, lY))
			{
				// Y Axis position event
				event.type = CL_InputEvent::axis_moved;
				event.id = 1;
				event.axis_pos = float(buffer[i].dwData)/(0xffff/2)-1.0f;
				if (sig_provider_event)
					sig_provider_event->invoke(event);
			}
			else if (buffer[i].dwOfs == FIELD_OFFSET(DIJOYSTATE2, lZ))
			{
				// Z Axis position event
				event.type = CL_InputEvent::axis_moved;
				event.id = 2;
				event.axis_pos = float(buffer[i].dwData)/(0xffff/2)-1.0f;
				if (sig_provider_event)
					sig_provider_event->invoke(event);
			}
			else if (buffer[i].dwOfs == FIELD_OFFSET(DIJOYSTATE2, rglSlider[0]))
			{
				// extra 1 axis position event
				event.type = CL_InputEvent::axis_moved;
				event.id = 3;
				event.axis_pos = float(buffer[i].dwData)/(0xffff/2)-1.0f;
				if (sig_provider_event)
					sig_provider_event->invoke(event);
			}
			else if (buffer[i].dwOfs == FIELD_OFFSET(DIJOYSTATE2, rglSlider[1]))
			{
				// extra 2 axis position event
				event.type = CL_InputEvent::axis_moved;
				event.id = 4;
				event.axis_pos = float(buffer[i].dwData)/(0xffff/2)-1.0f;
				if (sig_provider_event)
					sig_provider_event->invoke(event);
			}
			else if (buffer[i].dwOfs >= FIELD_OFFSET(DIJOYSTATE2, rgbButtons) && buffer[i].dwOfs < FIELD_OFFSET(DIJOYSTATE2, rgbButtons)+128)
			{
				// Button event
				int button_index = buffer[i].dwOfs - FIELD_OFFSET(DIJOYSTATE2, rgbButtons);
				event.id = button_index;

				if (LOBYTE(buffer[i].dwData)) // If high bit of lower byte is set, key is down
				{
					event.type = CL_InputEvent::pressed;
				}
				else
				{
					event.type = CL_InputEvent::released;
				}
				if (sig_provider_event)
					sig_provider_event->invoke(event);
			}
			else if (buffer[i].dwOfs >= FIELD_OFFSET(DIJOYSTATE2, rgdwPOV) && buffer[i].dwOfs < FIELD_OFFSET(DIJOYSTATE2, rgdwPOV)+4*sizeof(DWORD))
			{
				// Hat event:
				int hat_index = (buffer[i].dwOfs - FIELD_OFFSET(DIJOYSTATE2, rgdwPOV)) / sizeof(DWORD);
				bool centered = (LOWORD(buffer[i].dwData) == 0xFFFF);
				int direction = buffer[i].dwData / DI_DEGREES;
				if (centered) direction = -1;

				event.type = CL_InputEvent::axis_moved;
				event.id = 32 + hat_index;
				event.axis_pos = float(direction);
				if (sig_provider_event)
					sig_provider_event->invoke(event);
			}
			else if (buffer[i].dwOfs == FIELD_OFFSET(DIJOYSTATE2, lRx))
			{
				// X Axis rotation event
				event.type = CL_InputEvent::axis_moved;
				event.id = 5;
				event.axis_pos = float(buffer[i].dwData)/(0xffff/2)-1.0f;
				if (sig_provider_event)
					sig_provider_event->invoke(event);
			}
			else if (buffer[i].dwOfs == FIELD_OFFSET(DIJOYSTATE2, lRy))
			{
				// Y Axis rotation event
				event.type = CL_InputEvent::axis_moved;
				event.id = 6;
				event.axis_pos = float(buffer[i].dwData)/(0xffff/2)-1.0f;
				if (sig_provider_event)
					sig_provider_event->invoke(event);
			}
			else if (buffer[i].dwOfs == FIELD_OFFSET(DIJOYSTATE2, lRz))
			{
				// Z Axis rotation event
				event.type = CL_InputEvent::axis_moved;
				event.id = 7;
				event.axis_pos = float(buffer[i].dwData)/(0xffff/2)-1.0f;
				if (sig_provider_event)
					sig_provider_event->invoke(event);
			}
			else if (buffer[i].dwOfs == FIELD_OFFSET(DIJOYSTATE2, lVX))
			{
				// X Axis velocity event
				event.type = CL_InputEvent::axis_moved;
				event.id = 8;
				event.axis_pos = float(buffer[i].dwData)/(0xffff/2)-1.0f;
				if (sig_provider_event)
					sig_provider_event->invoke(event);
			}
			else if (buffer[i].dwOfs == FIELD_OFFSET(DIJOYSTATE2, lVY))
			{
				// Y Axis velocity event
				event.type = CL_InputEvent::axis_moved;
				event.id = 9;
				event.axis_pos = float(buffer[i].dwData)/(0xffff/2)-1.0f;
				if (sig_provider_event)
					sig_provider_event->invoke(event);
			}
			else if (buffer[i].dwOfs == FIELD_OFFSET(DIJOYSTATE2, lVZ))
			{
				// Z Axis velocity event
				event.type = CL_InputEvent::axis_moved;
				event.id = 10;
				event.axis_pos = float(buffer[i].dwData)/(0xffff/2)-1.0f;
				if (sig_provider_event)
					sig_provider_event->invoke(event);
			}
			else if (buffer[i].dwOfs == FIELD_OFFSET(DIJOYSTATE2, lVRx))
			{
				// X Axis angular velocity event
				event.type = CL_InputEvent::axis_moved;
				event.id = 11;
				event.axis_pos = float(buffer[i].dwData)/(0xffff/2)-1.0f;
				if (sig_provider_event)
					sig_provider_event->invoke(event);
			}
			else if (buffer[i].dwOfs == FIELD_OFFSET(DIJOYSTATE2, lVRy))
			{
				// Y Axis angular velocity event
				event.type = CL_InputEvent::axis_moved;
				event.id = 12;
				event.axis_pos = float(buffer[i].dwData)/(0xffff/2)-1.0f;
				if (sig_provider_event)
					sig_provider_event->invoke(event);
			}
			else if (buffer[i].dwOfs == FIELD_OFFSET(DIJOYSTATE2, lVRz))
			{
				// Z Axis angular velocity event
				event.type = CL_InputEvent::axis_moved;
				event.id = 13;
				event.axis_pos = float(buffer[i].dwData)/(0xffff/2)-1.0f;
				if (sig_provider_event)
					sig_provider_event->invoke(event);
			}
			else if (buffer[i].dwOfs == FIELD_OFFSET(DIJOYSTATE2, rglVSlider[0]))
			{
				// extra 1 axis velocity event
				event.type = CL_InputEvent::axis_moved;
				event.id = 14;
				event.axis_pos = float(buffer[i].dwData)/(0xffff/2)-1.0f;
				if (sig_provider_event)
					sig_provider_event->invoke(event);
			}
			else if (buffer[i].dwOfs == FIELD_OFFSET(DIJOYSTATE2, rglVSlider[1]))
			{
				// extra 2 axis velocity event
				event.type = CL_InputEvent::axis_moved;
				event.id = 15;
				event.axis_pos = float(buffer[i].dwData)/(0xffff/2)-1.0f;
				if (sig_provider_event)
					sig_provider_event->invoke(event);
			}
			else if (buffer[i].dwOfs == FIELD_OFFSET(DIJOYSTATE2, lAX))
			{
				// X Axis acceleration event
				event.type = CL_InputEvent::axis_moved;
				event.id = 16;
				event.axis_pos = float(buffer[i].dwData)/(0xffff/2)-1.0f;
				if (sig_provider_event)
					sig_provider_event->invoke(event);
			}
			else if (buffer[i].dwOfs == FIELD_OFFSET(DIJOYSTATE2, lAY))
			{
				// Y Axis acceleration event
				event.type = CL_InputEvent::axis_moved;
				event.id = 17;
				event.axis_pos = float(buffer[i].dwData)/(0xffff/2)-1.0f;
				if (sig_provider_event)
					sig_provider_event->invoke(event);
			}
			else if (buffer[i].dwOfs == FIELD_OFFSET(DIJOYSTATE2, lAZ))
			{
				// Z Axis acceleration event
				event.type = CL_InputEvent::axis_moved;
				event.id = 18;
				event.axis_pos = float(buffer[i].dwData)/(0xffff/2)-1.0f;
				if (sig_provider_event)
					sig_provider_event->invoke(event);
			}
			else if (buffer[i].dwOfs == FIELD_OFFSET(DIJOYSTATE2, lARx))
			{
				// X Axis angular acceleration event
				event.type = CL_InputEvent::axis_moved;
				event.id = 19;
				event.axis_pos = float(buffer[i].dwData)/(0xffff/2)-1.0f;
				if (sig_provider_event)
					sig_provider_event->invoke(event);
			}
			else if (buffer[i].dwOfs == FIELD_OFFSET(DIJOYSTATE2, lARy))
			{
				// Y Axis angular acceleration event
				event.type = CL_InputEvent::axis_moved;
				event.id = 20;
				event.axis_pos = float(buffer[i].dwData)/(0xffff/2)-1.0f;
				if (sig_provider_event)
					sig_provider_event->invoke(event);
			}
			else if (buffer[i].dwOfs == FIELD_OFFSET(DIJOYSTATE2, lARz))
			{
				// Z Axis angular acceleration event
				event.type = CL_InputEvent::axis_moved;
				event.id = 21;
				event.axis_pos = float(buffer[i].dwData)/(0xffff/2)-1.0f;
				if (sig_provider_event)
					sig_provider_event->invoke(event);
			}
			else if (buffer[i].dwOfs == FIELD_OFFSET(DIJOYSTATE2, rglASlider[0]))
			{
				// extra 1 axis acceleration event
				event.type = CL_InputEvent::axis_moved;
				event.id = 22;
				event.axis_pos = float(buffer[i].dwData)/(0xffff/2)-1.0f;
				if (sig_provider_event)
					sig_provider_event->invoke(event);
			}
			else if (buffer[i].dwOfs == FIELD_OFFSET(DIJOYSTATE2, rglASlider[1]))
			{
				// extra 2 axis acceleration event
				event.type = CL_InputEvent::axis_moved;
				event.id = 23;
				event.axis_pos = float(buffer[i].dwData)/(0xffff/2)-1.0f;
				if (sig_provider_event)
					sig_provider_event->invoke(event);
			}
			else if (buffer[i].dwOfs == FIELD_OFFSET(DIJOYSTATE2, lFX))
			{
				// X Axis force event
				event.type = CL_InputEvent::axis_moved;
				event.id = 24;
				event.axis_pos = float(buffer[i].dwData)/(0xffff/2)-1.0f;
				if (sig_provider_event)
					sig_provider_event->invoke(event);
			}
			else if (buffer[i].dwOfs == FIELD_OFFSET(DIJOYSTATE2, lFY))
			{
				// Y Axis force event
				event.type = CL_InputEvent::axis_moved;
				event.id = 25;
				event.axis_pos = float(buffer[i].dwData)/(0xffff/2)-1.0f;
				if (sig_provider_event)
					sig_provider_event->invoke(event);
			}
			else if (buffer[i].dwOfs == FIELD_OFFSET(DIJOYSTATE2, lFZ))
			{
				// Z Axis force event
				event.type = CL_InputEvent::axis_moved;
				event.id = 26;
				event.axis_pos = float(buffer[i].dwData)/(0xffff/2)-1.0f;
				if (sig_provider_event)
					sig_provider_event->invoke(event);
			}
			else if (buffer[i].dwOfs == FIELD_OFFSET(DIJOYSTATE2, lFRx))
			{
				// X Axis torque event
				event.type = CL_InputEvent::axis_moved;
				event.id = 27;
				event.axis_pos = float(buffer[i].dwData)/(0xffff/2)-1.0f;
				if (sig_provider_event)
					sig_provider_event->invoke(event);
			}
			else if (buffer[i].dwOfs == FIELD_OFFSET(DIJOYSTATE2, lFRy))
			{
				// Y Axis torque event
				event.type = CL_InputEvent::axis_moved;
				event.id = 28;
				event.axis_pos = float(buffer[i].dwData)/(0xffff/2)-1.0f;
				if (sig_provider_event)
					sig_provider_event->invoke(event);
			}
			else if (buffer[i].dwOfs == FIELD_OFFSET(DIJOYSTATE2, lFRz))
			{
				// Z Axis torque event
				event.type = CL_InputEvent::axis_moved;
				event.id = 29;
				event.axis_pos = float(buffer[i].dwData)/(0xffff/2)-1.0f;
				if (sig_provider_event)
					sig_provider_event->invoke(event);
			}
			else if (buffer[i].dwOfs == FIELD_OFFSET(DIJOYSTATE2, rglFSlider[0]))
			{
				// extra 1 axis force event
				event.type = CL_InputEvent::axis_moved;
				event.id = 30;
				event.axis_pos = float(buffer[i].dwData)/(0xffff/2)-1.0f;
				if (sig_provider_event)
					sig_provider_event->invoke(event);
			}
			else if (buffer[i].dwOfs == FIELD_OFFSET(DIJOYSTATE2, rglFSlider[1]))
			{
				// extra 2 axis force event
				event.type = CL_InputEvent::axis_moved;
				event.id = 31;
				event.axis_pos = float(buffer[i].dwData)/(0xffff/2)-1.0f;
				if (sig_provider_event)
					sig_provider_event->invoke(event);
			}
		}
	}
}

bool CL_InputDeviceProvider_DirectInput::poll(bool peek_only)
{
	throw_if_disposed();
	// peek_only is not supported, as WM_INPUT is used as the event trigger
	update();
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// CL_InputDeviceProvider_DirectInput implementation:

void CL_InputDeviceProvider_DirectInput::on_dispose()
{
	if (directinput_device)
		directinput_device->Release();
}
