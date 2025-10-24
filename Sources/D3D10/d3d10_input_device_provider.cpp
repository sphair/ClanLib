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
*/

#include "D3D10/precomp.h"
#include "d3d10_input_device_provider.h"

/////////////////////////////////////////////////////////////////////////////
// CL_D3D10InputDeviceProvider Construction:

CL_D3D10InputDeviceProvider::CL_D3D10InputDeviceProvider()
{
}

CL_D3D10InputDeviceProvider::~CL_D3D10InputDeviceProvider()
{
}

void CL_D3D10InputDeviceProvider::destroy()
{
	delete this;
}

/////////////////////////////////////////////////////////////////////////////
// CL_D3D10InputDeviceProvider Attributes:

CL_String CL_D3D10InputDeviceProvider::get_name() const
{
	return CL_String();
}

CL_String CL_D3D10InputDeviceProvider::get_device_name() const
{
	return CL_String();
}

CL_InputDevice::Type CL_D3D10InputDeviceProvider::get_type() const
{
	return CL_InputDevice::unknown;
}

CL_String CL_D3D10InputDeviceProvider::get_key_name(int id) const
{
	return CL_String();
}

bool CL_D3D10InputDeviceProvider::get_keycode(int keycode) const
{
	return false;
}

int CL_D3D10InputDeviceProvider::get_x() const
{
	return 0;
}

int CL_D3D10InputDeviceProvider::get_y() const
{
	return 0;
}

float CL_D3D10InputDeviceProvider::get_axis(int index) const
{
	return 0.0f;
}

int CL_D3D10InputDeviceProvider::get_axis_count() const
{
	return 0;
}

int CL_D3D10InputDeviceProvider::get_button_count() const
{
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CL_D3D10InputDeviceProvider Operations:

void CL_D3D10InputDeviceProvider::init(CL_Signal_v1<const CL_InputEvent &> *sig_provider_event)
{
}

void CL_D3D10InputDeviceProvider::set_position(int x, int y)
{
}

bool CL_D3D10InputDeviceProvider::poll(bool peek_only)
{
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// CL_D3D10InputDeviceProvider Implementation:
