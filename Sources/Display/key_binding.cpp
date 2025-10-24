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

#include "Display/display_precomp.h"
#include "API/Display/key_binding.h"
#include "key_binding_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_KeyBinding construction:

CL_KeyBinding::CL_KeyBinding()
: impl(new CL_KeyBinding_Generic)
{
}

CL_KeyBinding::CL_KeyBinding(int keycode, const CL_InputDevice &device)
: impl(new CL_KeyBinding_Generic)
{
	set_keycode(keycode);
	set_device(device);
}

CL_KeyBinding::CL_KeyBinding(const CL_KeyBinding &copy)
: impl(new CL_KeyBinding_Generic)
{
	set_keycode(copy.get_keycode());
	set_device(copy.get_device());
}

/////////////////////////////////////////////////////////////////////////////
// CL_KeyBinding attributes:

bool CL_KeyBinding::operator ==(const CL_KeyBinding &other) const
{
	return ((other.get_device().impl == get_device().impl) && (other.get_keycode() == get_keycode()));
}

bool CL_KeyBinding::operator <(const CL_KeyBinding &other) const
{
	return ((other.get_device().impl == get_device().impl) && (other.get_keycode() < get_keycode()));
}

bool CL_KeyBinding::operator >(const CL_KeyBinding &other) const
{
	return ((other.get_device().impl == get_device().impl) && (other.get_keycode() > get_keycode()));
}

CL_InputDevice &CL_KeyBinding::get_device() const
{
	return impl->device;
}

int CL_KeyBinding::get_keycode() const
{
	return impl->keycode;
}

std::string CL_KeyBinding::get_key_name() const
{
	return "TODO";
}

/////////////////////////////////////////////////////////////////////////////
// CL_KeyBinding operations:

CL_KeyBinding &CL_KeyBinding::operator =(const CL_KeyBinding &copy)
{
	set_keycode(copy.get_keycode());
	set_device(copy.get_device());
	return *this;
}

void CL_KeyBinding::set_keycode(int keycode)
{
	impl->keycode = keycode;
}

void CL_KeyBinding::set_device(const CL_InputDevice &device)
{
	impl->device = device;
}

/////////////////////////////////////////////////////////////////////////////
// CL_KeyBinding implementation:
