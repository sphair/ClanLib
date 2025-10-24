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

#ifndef header_slot_container
#define header_slot_container

#if _MSC_VER > 1000
#pragma once
#endif

#include "slot.h"
#include "signal_v0.h"
#include "signal_v1.h"
#include "signal_v2.h"
#include "signal_v3.h"
#include "signal_v4.h"
#include "signal_v5.h"
#include <list>

//: Slot Generic Container
//- !group=Signal/System!
//- !header=signals.h!
//- <p>Convience signal/slot connector class.</p>
//- <p>The slot container class is a convience class. It simplifies the
//- connection between signals and slots by keeping all connections made
//- until the entire class is destroyed. This saves the developer from having
//- to keep a CL_Slot reference around for each connection needed.</p>
//- <p>In order to use this class, simply make it a member variable and call
//- connect_signal() for each function you want hooked up.</p>
//- <p>See the login_view.cpp/h in the CTalk example for an example of how to use
//- this class.</p>
class CL_SlotContainer
{
public:
//! Operations:
	//: Connect a slot to a function CL_Signal_v0 signal.
	template<class SigClass>
	void connect(SigClass &sig, void (*func)())
	{
		slots.push_back(sig.connect(func));
	}

	//: Connect a slot to a function CL_Signal_v0 signal with a user data parameter
	//: passed along.
	template<class SigClass, class UserData>
	void connect(SigClass &sig, void (*func)(UserData), UserData user_data)
	{
		slots.push_back(sig.connect(func, user_data));
	}

	//: Connect a slot to a function CL_Signal_v1 signal.
	template<class SigClass, class Param1>
	void connect(SigClass &sig, void(func)(Param1))
	{
		slots.push_back(sig.connect(func));
	}

	//NOTE: I added support for a user_data parm for global functions for only what I needed, most of the other permutations needed are still missing.
	//To add more, you need to edit the signal_v* and slot_v* to add function-user-data versions. -SAR  2-13-2008

	//: Connect a slot to a function CL_Signal_v1 signal with an user data parameter
	//: passed along.
	template<class SigClass, class Param1, class UserData>
	void connect(SigClass &sig, void(*func)(Param1, UserData), UserData user_data)
	{
		slots.push_back(sig.connect(func, user_data));
	}


	//: Connect a slot to a CL_Signal_v0 signal.
	template<class SigClass, class Class>
	void connect(SigClass &sig, Class *self, void(Class::*func)())
	{
		slots.push_back(sig.connect(self, func));
	}

	//: Connect a slot to a CL_Signal_v0 signal with a user data parameter
	//: passed along.
	template<class SigClass, class Class, class UserData>
	void connect(SigClass &sig, Class *self, void(Class::*func)(UserData), UserData user_data)
	{
		slots.push_back(sig.connect(self, func, user_data));
	}

	//: Connect a slot to a CL_Signal_v1 signal.
	template<class SigClass, class Class, class Param1>
	void connect(SigClass &sig, Class *self, void(Class::*func)(Param1))
	{
		slots.push_back(sig.connect(self, func));
	}

	//: Connect a slot to a CL_Signal_v1 signal with an user data parameter
	//: passed along.
	template<class SigClass, class Class, class Param1, class UserData>
	void connect(SigClass &sig, Class *self, void(Class::*func)(Param1, UserData), UserData user_data)
	{
		slots.push_back(sig.connect(self, func, user_data));
	}

	//: Connect a slot to a CL_Signal_v2 signal.
	template<class SigClass, class Class, class Param1, class Param2>
	void connect(SigClass &sig, Class *self, void(Class::*func)(Param1, Param2))
	{
		slots.push_back(sig.connect(self, func));
	}

	//: Connect a slot to a CL_Signal_v2 signal with an user data parameter
	//: passed along.
	template<class SigClass, class Class, class Param1, class Param2, class UserData>
	void connect(SigClass &sig, Class *self, void(Class::*func)(Param1, Param2, UserData), UserData user_data)
	{
		slots.push_back(sig.connect(self, func, user_data));
	}

	//: Connect a slot to a CL_Signal_v3 signal.
	template<class SigClass, class Class, class Param1, class Param2, class Param3>
	void connect(SigClass &sig, Class *self, void(Class::*func)(Param1, Param2, Param3))
	{
		slots.push_back(sig.connect(self, func));
	}

	//: Connect a slot to a CL_Signal_v3 signal with an user data parameter
	//: passed along.
	template<class SigClass, class Class, class Param1, class Param2, class Param3, class UserData>
	void connect(SigClass &sig, Class *self, void(Class::*func)(Param1, Param2, Param3, UserData), UserData user_data)
	{
		slots.push_back(sig.connect(self, func, user_data));
	}

	//: Connect a slot to a CL_Signal_v4 signal.
	template<class SigClass, class Class, class Param1, class Param2, class Param3, class Param4>
	void connect(SigClass &sig, Class *self, void(Class::*func)(Param1, Param2, Param3, Param4))
	{
		slots.push_back(sig.connect(self, func));
	}

	//: Connect a slot to a CL_Signal_v4 signal with an user data parameter
	//: passed along.
	template<class SigClass, class Class, class Param1, class Param2, class Param3, class Param4, class UserData>
	void connect(SigClass &sig, Class *self, void(Class::*func)(Param1, Param2, Param3, Param4, UserData user_data), UserData user_data)
	{
		slots.push_back(sig.connect(self, func, user_data));
	}

	//: Connect a slot to a CL_Signal_v5 signal.
	template<class SigClass, class Class, class Param1, class Param2, class Param3, class Param4, class Param5>
	void connect(SigClass &sig, Class *self, void(Class::*func)(Param1, Param2, Param3, Param4, Param5))
	{
		slots.push_back(sig.connect(self, func));
	}

	//: Connect a slot to a CL_Signal_v5 signal with an user data parameter
	//: passed along.
	template<class SigClass, class Class, class Param1, class Param2, class Param3, class Param4, class Param5, class UserData>
	void connect(SigClass &sig, Class *self, void(Class::*func)(Param1, Param2, Param3, Param4, Param5, UserData user_data), UserData user_data)
	{
		slots.push_back(sig.connect(self, func, user_data));
	}

	//: Connect a functor slot to a signal.
	template<class SigClass, class Functor>
	void connect_functor(SigClass &sig, const Functor &functor)
	{
		slots.push_back(sig.connect_functor(functor));
	}

private:
	std::list<CL_Slot> slots;
};

#endif
