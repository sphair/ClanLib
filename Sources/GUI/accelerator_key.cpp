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
**    Harry Storbacka
*/

#include "GUI/precomp.h"
#include "API/GUI/accelerator_key.h"
#include "API/GUI/gui_message_input.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/keys.h"
#include <map>

/////////////////////////////////////////////////////////////////////////////
// CL_AcceleratorKey_Impl Class:

class CL_AcceleratorKey_Impl
{
//! Construction:
public:
	CL_AcceleratorKey_Impl()
	: id(-1), shift(false), alt(false), ctrl(false)
	{
	}

//! Attributes:
public:
	CL_Callback_v2<CL_GUIMessage, CL_AcceleratorKey> func_pressed;
	int id;
	bool shift, alt, ctrl;
};


/////////////////////////////////////////////////////////////////////////////
// CL_AcceleratorKey Construction:

CL_AcceleratorKey::CL_AcceleratorKey()
{
}

CL_AcceleratorKey::CL_AcceleratorKey(int id, int mod1, int mod2, int mod3)
: impl(new CL_AcceleratorKey_Impl)
{
	impl->id = id;
	if (mod1 == CL_KEY_SHIFT || mod2 == CL_KEY_SHIFT || mod3 == CL_KEY_SHIFT)
		impl->shift = true;
	if (mod1 == CL_KEY_MENU || mod2 == CL_KEY_MENU || mod3 == CL_KEY_MENU)
		impl->alt = true;
	if (mod1 == CL_KEY_CONTROL || mod2 == CL_KEY_CONTROL || mod3 == CL_KEY_CONTROL)
		impl->ctrl = true;
}

CL_AcceleratorKey::~CL_AcceleratorKey()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_AcceleratorKey Attributes:

CL_Callback_v2<CL_GUIMessage, CL_AcceleratorKey> &CL_AcceleratorKey::func_pressed()
{
	return impl->func_pressed;
}

bool CL_AcceleratorKey::get_shift() const
{
	return impl->shift;
}

bool CL_AcceleratorKey::get_alt() const
{
	return impl->alt;
}

bool CL_AcceleratorKey::get_ctrl() const
{
	return impl->ctrl;
}

int CL_AcceleratorKey::get_id() const
{
	return impl->id;
}


void CL_AcceleratorKey::throw_if_null() const
{
	if (impl.is_null())
		throw CL_Exception("CL_AcceleratorKey is null");
}

/////////////////////////////////////////////////////////////////////////////
// CL_AcceleratorKey Operations:

void CL_AcceleratorKey::set_shift(bool value)
{
	impl->shift = value;
}

void CL_AcceleratorKey::set_alt(bool value)
{
	impl->alt = value;
}

void CL_AcceleratorKey::set_id(int value)
{
	impl->id = value;
}

void CL_AcceleratorKey::set_ctrl(bool value)
{
	impl->ctrl = value;
}

/////////////////////////////////////////////////////////////////////////////
// CL_AcceleratorKey Implementation:

