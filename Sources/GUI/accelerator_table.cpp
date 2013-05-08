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
#include "API/GUI/accelerator_table.h"
#include "API/GUI/accelerator_key.h"
#include "API/GUI/gui_message_input.h"
#include "API/Display/Window/input_event.h"
#include <map>

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// AcceleratorTable_Impl Class:

class AcceleratorTable_Impl
{
//! Construction:
public:
	AcceleratorTable_Impl()
	{
	}

//! Attributes:
public:
	std::vector<AcceleratorKey> entries;
};


/////////////////////////////////////////////////////////////////////////////
// AcceleratorTable Construction:

AcceleratorTable::AcceleratorTable()
: impl(new AcceleratorTable_Impl)
{
}

AcceleratorTable::~AcceleratorTable()
{
}

/////////////////////////////////////////////////////////////////////////////
// AcceleratorTable Attributes:

AcceleratorKey AcceleratorTable::get_accelerator(int id, bool shift, bool alt, bool ctrl)
{
	std::vector<AcceleratorKey>::iterator it;
	for (it = impl->entries.begin(); it != impl->entries.end(); ++it)
	{
		if ((*it).get_id() == id && (*it).get_shift() == shift && (*it).get_alt() == alt && (*it).get_ctrl() == ctrl)
		{
			return (*it);
		}
	}
	return AcceleratorKey();
}

std::vector<AcceleratorKey> AcceleratorTable::get_accelerators()
{
	return impl->entries;
}


/////////////////////////////////////////////////////////////////////////////
// AcceleratorTable Operations:

void AcceleratorTable::add_accelerator(const AcceleratorKey &accel)
{
	impl->entries.push_back(accel);
}

void AcceleratorTable::process_message(std::shared_ptr<GUIMessage> &msg)
{
	std::shared_ptr<GUIMessage_Input> input_msg = std::dynamic_pointer_cast<GUIMessage_Input>(msg);
	if (input_msg)
	{
		std::vector<AcceleratorKey>::iterator it;
		for (it = impl->entries.begin(); it != impl->entries.end(); ++it)
		{
			if ((*it).get_id() == input_msg->input_event.id && (*it).get_shift() == input_msg->input_event.shift && (*it).get_alt() == input_msg->input_event.alt && (*it).get_ctrl() == input_msg->input_event.ctrl)
			{
				if (input_msg->input_event.type == InputEvent::pressed && !(*it).func_pressed().is_null())
					(*it).func_pressed().invoke(msg, (*it));
				msg->consumed = true;
				return;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// AcceleratorTable Implementation:

}
