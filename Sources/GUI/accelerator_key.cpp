/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
**    Harry Storbacka
*/

#include "GUI/precomp.h"
#include "API/GUI/accelerator_key.h"
#include "API/GUI/gui_message_input.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"
#include <map>

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// AcceleratorKey_Impl Class:

class AcceleratorKey_Impl
{
//! Construction:
public:
	AcceleratorKey_Impl()
	: id(-1), shift(false), alt(false), ctrl(false)
	{
	}

//! Attributes:
public:
	std::function<void(std::shared_ptr<GUIMessage> &, AcceleratorKey)> func_pressed;
	int id;
	bool shift, alt, ctrl;
};


/////////////////////////////////////////////////////////////////////////////
// AcceleratorKey Construction:

AcceleratorKey::AcceleratorKey()
{
}

AcceleratorKey::AcceleratorKey(int id, int mod1, int mod2, int mod3)
: impl(std::make_shared<AcceleratorKey_Impl>())
{
	impl->id = id;
	if (mod1 == keycode_shift || mod2 == keycode_shift || mod3 == keycode_shift)
		impl->shift = true;
	if (mod1 == keycode_menu || mod2 == keycode_menu || mod3 == keycode_menu)
		impl->alt = true;
	if (mod1 == keycode_control || mod2 == keycode_control || mod3 == keycode_control)
		impl->ctrl = true;
}

AcceleratorKey::~AcceleratorKey()
{
}

/////////////////////////////////////////////////////////////////////////////
// AcceleratorKey Attributes:

std::function<void(std::shared_ptr<GUIMessage> &, AcceleratorKey)> &AcceleratorKey::func_pressed()
{
	return impl->func_pressed;
}

bool AcceleratorKey::get_shift() const
{
	return impl->shift;
}

bool AcceleratorKey::get_alt() const
{
	return impl->alt;
}

bool AcceleratorKey::get_ctrl() const
{
	return impl->ctrl;
}

int AcceleratorKey::get_id() const
{
	return impl->id;
}


void AcceleratorKey::throw_if_null() const
{
	if (!impl)
		throw Exception("AcceleratorKey is null");
}

/////////////////////////////////////////////////////////////////////////////
// AcceleratorKey Operations:

void AcceleratorKey::set_shift(bool value)
{
	impl->shift = value;
}

void AcceleratorKey::set_alt(bool value)
{
	impl->alt = value;
}

void AcceleratorKey::set_id(int value)
{
	impl->id = value;
}

void AcceleratorKey::set_ctrl(bool value)
{
	impl->ctrl = value;
}

/////////////////////////////////////////////////////////////////////////////
// AcceleratorKey Implementation:

}
