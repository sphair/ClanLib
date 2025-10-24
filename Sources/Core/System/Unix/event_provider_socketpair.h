/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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


#include "API/Core/System/event_provider.h"
#include "API/Core/System/mutex.h"

class CL_EventProvider_Socketpair : public CL_EventProvider
{
/// \name Construction
/// \{

public:
	CL_EventProvider_Socketpair(bool manual_reset, bool initial_state);

	~CL_EventProvider_Socketpair();


/// \}
/// \name Attributes
/// \{

public:
	EventType get_event_type(int index);

	int get_event_handle(int index);

	int get_num_event_handles();


/// \}
/// \name Operations
/// \{

public:
	bool check_after_wait(int index);

	bool set();

	bool reset();


/// \}
/// \name Implementation
/// \{

private:
	CL_Mutex mutex;

	bool manual_reset;

	bool state;

	int wait_sockets[2];
/// \}
};


