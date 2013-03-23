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

#include "SWRender/precomp.h"
#include "swr_input_device_provider.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// SWRenderInputDeviceProvider Construction:

SWRenderInputDeviceProvider::SWRenderInputDeviceProvider()
{
}

SWRenderInputDeviceProvider::~SWRenderInputDeviceProvider()
{
}

/////////////////////////////////////////////////////////////////////////////
// SWRenderInputDeviceProvider Attributes:

std::string SWRenderInputDeviceProvider::get_name() const
{
	return std::string();
}

std::string SWRenderInputDeviceProvider::get_device_name() const
{
	return std::string();
}

InputDevice::Type SWRenderInputDeviceProvider::get_type() const
{
	return InputDevice::unknown;
}

std::string SWRenderInputDeviceProvider::get_key_name(int id) const
{
	return std::string();
}

bool SWRenderInputDeviceProvider::get_keycode(int keycode) const
{
	return false;
}

int SWRenderInputDeviceProvider::get_x() const
{
	return 0;
}

int SWRenderInputDeviceProvider::get_y() const
{
	return 0;
}

float SWRenderInputDeviceProvider::get_axis(int index) const
{
	return 0.0f;
}

int SWRenderInputDeviceProvider::get_axis_count() const
{
	return 0;
}

int SWRenderInputDeviceProvider::get_button_count() const
{
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// SWRenderInputDeviceProvider Operations:

void SWRenderInputDeviceProvider::init(Signal_v1<const InputEvent &> *sig_provider_event)
{
}

void SWRenderInputDeviceProvider::set_position(int x, int y)
{
}

bool SWRenderInputDeviceProvider::poll(bool peek_only)
{
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// SWRenderInputDeviceProvider Implementation:

}
