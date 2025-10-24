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

#include "API/Signals/slot_generic.h"
#include "API/Signals/slot.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Slot_Generic construction:

CL_Slot_Generic::CL_Slot_Generic() : persistent(false), slot_ref_count(0), signal_ref_count(0)
{
}

CL_Slot_Generic::~CL_Slot_Generic()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_Slot_Generic attributes:

int CL_Slot_Generic::get_slot_ref() const
{
	return slot_ref_count;
}

int CL_Slot_Generic::get_signal_ref() const
{
	return signal_ref_count;
}

bool CL_Slot_Generic::is_persistent() const
{
	return persistent;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Slot_Generic operations:

void CL_Slot_Generic::add_slot_ref()
{
	slot_ref_count++;
}

void CL_Slot_Generic::release_slot_ref()
{
	slot_ref_count--;
	check_delete();
}

void CL_Slot_Generic::add_signal_ref(CL_Signal *signal)
{
	connected_signals.push_back(signal);
	signal_ref_count++;
}

void CL_Slot_Generic::release_signal_ref(CL_Signal *signal)
{
	connected_signals.remove(signal);
	signal_ref_count--;
	check_delete();
}

void CL_Slot_Generic::set_persistent(bool flag)
{
	persistent = flag;
}

/////////////////////////////////////////////////////////////////////////////
//! Implementation:

void CL_Slot_Generic::check_delete()
{
	// Try remove from signals immidiately, if possible:
	if (slot_ref_count == 0 && signal_ref_count > 0)
	{
		// Prevent recursive looping (and allows us to create a helper CL_Slot object):
		slot_ref_count++;

		std::list<CL_Signal *> signals = connected_signals;
		connected_signals.clear(); // clear member list to avoid recursive loop problems

		std::list<CL_Signal *>::iterator it;
		for (it = signals.begin(); it != signals.end(); ++it)
		{
			CL_Signal *cur = (*it);
			CL_Slot slot(this);
			cur->disconnect(slot);
		}

		slot_ref_count--;
	}

	// Delete ourselves if nobody reference us anymore
	if (slot_ref_count == 0 && signal_ref_count == 0) delete this;
}
