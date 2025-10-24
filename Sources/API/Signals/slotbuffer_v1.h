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

#ifndef header_slotbuffer_v1
#define header_slotbuffer_v1

#if _MSC_VER > 1000
#pragma once
#endif

#include <vector>
#include "slot.h"

template <class PARAM1> class CL_Signal_v1;

//: SlotBuffer V1 Class
//- !group=Signal/System!
//- !header=signals.h!
template <class PARAM1>
class CL_SlotBuffer_v1
{
public:
	struct Params
	{
		PARAM1 p1;
	};

//! Construction:
public:
	//: Slot Buffer V1 Constructor
	CL_SlotBuffer_v1()
	{
	}

	//: Slot Buffer V1 Constructor
	CL_SlotBuffer_v1(CL_Signal_v1<PARAM1> &signal)
	{
		connect(signal);
	}

//! Attributes:
public:
	//: Bool
	operator bool()
	{
		return !params.empty();
	}

	//: Size
	int size()
	{
		return params.size();
	}

	//: Operator
	Params &operator[](int index)
	{
		return params[index];
	}

//! Operations:
public:
	//: Connect
	void connect(CL_Signal_v1<PARAM1> &signal)
	{
		slot = signal.connect(this, &CL_SlotBuffer_v1::callback);
	}

	//: Disconnect
	void disconnect(CL_Signal_v1<PARAM1> &signal)
	{
		signal.disconnect(slot);
	}

//! Implementation:
private:
	void callback(PARAM1 p1)
	{
		Params p;
		p.p1 = p1;

		params.push_back(p);
	}

	std::vector<Params> params;
	CL_Slot slot;
};

#endif
