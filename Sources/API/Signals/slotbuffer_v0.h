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

//! clanSignal="System"
//! header=signals.h

#ifndef header_slotbuffer_v0
#define header_slotbuffer_v0

#if _MSC_VER > 1000
#pragma once
#endif

#include "signal_v0.h"

//: Slot Buffer V0 Class
//- !group=Signal/System!
//- !header=signals.h!
class CL_SlotBuffer_v0
{
//! Construction:
public:
	//: Slot Buffer V0 Constructor
	CL_SlotBuffer_v0() : count(0)
	{
	}

	//: Slot Buffer V0 Constructor
	CL_SlotBuffer_v0(CL_Signal_v0 &signal) : count(0)
	{
		connect(signal);
	}

//! Attributes:
public:
	//: Bool
	operator bool()
	{
		return count > 0;
	}

	//: Size
	int size()
	{
		return count;
	}

//! Operations:
public:
	//: Connect
	void connect(CL_Signal_v0 &signal)
	{
		slot = signal.connect(this, &CL_SlotBuffer_v0::callback);
	}

	//: Disconnect
	void disconnect(CL_Signal_v0 &signal)
	{
		signal.disconnect(slot);
	}

//! Implementation:
private:
	void callback()
	{
		count++;
	}

	CL_Slot slot;
	int count;
};

#endif
