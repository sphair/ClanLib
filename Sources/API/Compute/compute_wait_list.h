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
**    Magnus Norddahl
*/


#pragma once

#include "compute_event.h"

namespace clan
{
/// \addtogroup clanCompute_Compute clanCompute Compute
/// \{

/// \brief List of events that must have completed before a new operation is executed
class ComputeWaitList : public std::vector<ComputeEvent>
{
/// \name Construction
/// \{
public:
	ComputeWaitList()
	{
	}

	ComputeWaitList(const ComputeEvent &event1)
	{
		push_back(event1);
	}

	ComputeWaitList(const ComputeEvent &event1, const ComputeEvent &event2)
	{
		reserve(2);
		push_back(event1);
		push_back(event2);
	}

	ComputeWaitList(const ComputeEvent &event1, const ComputeEvent &event2, const ComputeEvent &event3)
	{
		reserve(3);
		push_back(event1);
		push_back(event2);
		push_back(event3);
	}

	ComputeWaitList(const ComputeEvent &event1, const ComputeEvent &event2, const ComputeEvent &event3, const ComputeEvent &event4)
	{
		reserve(4);
		push_back(event1);
		push_back(event2);
		push_back(event3);
		push_back(event4);
	}

	ComputeWaitList(const ComputeEvent &event1, const ComputeEvent &event2, const ComputeEvent &event3, const ComputeEvent &event4, const ComputeEvent &event5)
	{
		reserve(5);
		push_back(event1);
		push_back(event2);
		push_back(event3);
		push_back(event4);
		push_back(event5);
	}

	ComputeWaitList(const ComputeEvent &event1, const ComputeEvent &event2, const ComputeEvent &event3, const ComputeEvent &event4, const ComputeEvent &event5, const ComputeEvent &event6)
	{
		reserve(6);
		push_back(event1);
		push_back(event2);
		push_back(event3);
		push_back(event4);
		push_back(event5);
		push_back(event6);
	}

	ComputeWaitList(const ComputeEvent &event1, const ComputeEvent &event2, const ComputeEvent &event3, const ComputeEvent &event4, const ComputeEvent &event5, const ComputeEvent &event6, const ComputeEvent &event7)
	{
		reserve(7);
		push_back(event1);
		push_back(event2);
		push_back(event3);
		push_back(event4);
		push_back(event5);
		push_back(event6);
		push_back(event7);
	}
/// \}
};

}

/// \}
