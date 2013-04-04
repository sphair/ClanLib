/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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


#include "../api_core.h"
#include <memory>
#include <vector>

namespace clan
{

/// (Internal ClanLib Class)
class CL_API_CORE SlotCallback
{
public:
	SlotCallback() : valid(true), enabled(true) { return; }

	virtual ~SlotCallback() { return; }

	bool valid;

	bool enabled;
};

/// (Internal ClanLib Class)
class CL_API_CORE Slot_Impl
{
public:
	~Slot_Impl() { if (callback) callback->valid = false; }

	std::shared_ptr<SlotCallback> callback;
};

/// (Internal ClanLib Class)
class CL_API_CORE Signal_Impl
{
public:
	std::vector< std::shared_ptr<SlotCallback> > connected_slots;
};

}
