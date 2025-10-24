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
#include "API/GUI/accelerator_table.h"
#include "API/GUI/accelerator_key.h"
#include "API/GUI/gui_message_input.h"
#include "API/Display/Window/input_event.h"
#include <map>

/////////////////////////////////////////////////////////////////////////////
// CL_AcceleratorTable_Impl Class:

class CL_AcceleratorTable_Impl
{
//! Construction:
public:
	CL_AcceleratorTable_Impl()
	{
	}

//! Attributes:
public:
	std::vector<CL_AcceleratorKey> entries;
};


/////////////////////////////////////////////////////////////////////////////
// CL_AcceleratorTable Construction:

CL_AcceleratorTable::CL_AcceleratorTable()
: impl(new CL_AcceleratorTable_Impl)
{
}

CL_AcceleratorTable::~CL_AcceleratorTable()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_AcceleratorTable Attributes:

CL_AcceleratorKey CL_AcceleratorTable::get_accelerator(int id, bool shift, bool alt, bool ctrl)
{
	std::vector<CL_AcceleratorKey>::iterator it;
	for (it = impl->entries.begin(); it != impl->entries.end(); ++it)
	{
		if ((*it).get_id() == id && (*it).get_shift() == shift && (*it).get_alt() == alt && (*it).get_ctrl() == ctrl)
		{
			return (*it);
		}
	}
	return CL_AcceleratorKey();
}

std::vector<CL_AcceleratorKey> CL_AcceleratorTable::get_accelerators()
{
	return impl->entries;
}


/////////////////////////////////////////////////////////////////////////////
// CL_AcceleratorTable Operations:

void CL_AcceleratorTable::add_accelerator(const CL_AcceleratorKey &accel)
{
	impl->entries.push_back(accel);
}

void CL_AcceleratorTable::process_message(CL_GUIMessage &msg)
{
	if (msg.is_type(CL_GUIMessage_Input::get_type_name()))
	{
		CL_GUIMessage_Input input_msg = msg;
		CL_InputEvent e = input_msg.get_event();

		std::vector<CL_AcceleratorKey>::iterator it;
		for (it = impl->entries.begin(); it != impl->entries.end(); ++it)
		{
			if ((*it).get_id() == e.id && (*it).get_shift() == e.shift && (*it).get_alt() == e.alt && (*it).get_ctrl() == e.ctrl)
			{
				if (e.type == CL_InputEvent::pressed && !(*it).func_pressed().is_null())
					(*it).func_pressed().invoke(msg, (*it));
				msg.set_consumed();
				return;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_AcceleratorTable Implementation:

