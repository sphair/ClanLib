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

#ifndef header_signal_v1
#define header_signal_v1

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#pragma warning ( disable : 4355 ) // warning C4355: 'this' : used in base member initializer list
#endif

#include "signal.h"
#include "slot.h"
#include "slot_v1.h"
#include <list>
#include <algorithm>

//: CL Signal V1 class
//- !group=Signal/System!
//- !header=signals.h!
template <class PARAM1>
class CL_Signal_v1 : public CL_Signal
{
public:
	typedef CL_Slot_v1<PARAM1> *Slot;
	typedef typename std::list<Slot>::iterator SlotIterator;

	class CL_Signal_v1_Generic
	{
	public:
		CL_Signal_v1_Generic(CL_Signal_v1 *owner) : in_call(false), deleted(false), in_call_remove_slots(0), owner(owner) { return; }

		~CL_Signal_v1_Generic()
		{
			for (SlotIterator slot_it = slots.begin(); slot_it != slots.end(); slot_it++)
			{
				Slot slot = *slot_it;
				slot->release_signal_ref(owner);
			}
		}

		bool in_call, deleted;
		std::list<SlotIterator> *in_call_remove_slots;
		std::list<Slot> slots;
		CL_Signal_v1 *owner;
	};

//! Construction:
public:
	//: CL Signal V1 constructor
	CL_Signal_v1() : impl(new CL_Signal_v1_Generic(this))
	{
	}

	CL_Signal_v1(const CL_Signal_v1 &copy) : CL_Signal(copy), impl(new CL_Signal_v1_Generic(this))
	{
		impl->slots = copy.impl->slots;
		for (SlotIterator slot_it = impl->slots.begin(); slot_it != impl->slots.end(); slot_it++)
		{
			Slot slot = *slot_it;
			slot->add_signal_ref(this);
		}
	}

	//: CL Signal V1 destructor
	~CL_Signal_v1()
	{
		if (impl->in_call) impl->deleted = true;
		else delete impl;
	}

//! Attributes:
public:
	bool has_slots_connected() const
	{
		return !impl->slots.empty();
	}

//! Operations:
public:
	CL_Signal_v1 &operator =(const CL_Signal_v1 &copy)
	{
		if (impl->in_call) impl->deleted = true;
		else delete impl;

		impl = new CL_Signal_v1_Generic(this);
		impl->slots = copy.impl->slots;
		for (SlotIterator slot_it = impl->slots.begin(); slot_it != impl->slots.end(); slot_it++)
		{
			Slot slot = *slot_it;
			slot->add_signal_ref(this);
		}
		return *this;
	}

	//: Operator
	void operator() (PARAM1 param1)
	{
		call(param1);
	}

	//: Call
	void call(PARAM1 param1)
	{
		CL_Signal_v1_Generic *data = impl;
		data->in_call = true;

		std::list<SlotIterator> remove_slots;
		try
		{
			data->in_call_remove_slots = &remove_slots;
			
			// call slots connected to signal:
			for (SlotIterator slot_it = data->slots.begin(); slot_it != data->slots.end(); slot_it++)
			{
				Slot slot = *slot_it;

				// skip slot if it has no more references left in application.
				// (make it pending for removal)
				if (slot->get_slot_ref() == 0 && slot->is_persistent() == false)
				{
					if (std::find(remove_slots.begin(), remove_slots.end(), slot_it)==remove_slots.end())
						remove_slots.push_back(slot_it);
				}
				
				slot->call(param1);
				if (data->deleted) break;
			}
		}
		catch (...)
		{
			catch_final(data, remove_slots);
			throw;
		}
		catch_final(data, remove_slots);
	}

	//: Connect slot.
	CL_Slot connect(Slot slot)
	{
		slot->add_signal_ref(this);
		impl->slots.push_back(slot);
		return CL_Slot(slot);
	}

	//: Connect callback function slot.
	CL_Slot connect(void (*callback)(PARAM1))
	{
		return connect(new CL_FunctionSlot_v1<PARAM1>(callback));
	}

	//: Connect callback function with user data slot.
	template <class UserData>
	CL_Slot connect(
		void (*callback)(PARAM1, UserData),
		UserData user_data)
	{
		return connect(new CL_UserDataFunctionSlot_v1<PARAM1, UserData>(callback, user_data));
	}

	//: Connect functor slot.
	template<class Functor>
	CL_Slot connect_functor(const Functor &functor)
	{
		return connect(new CL_FunctorSlot_v1<Functor, PARAM1>(functor));
	}

	//: Connect member function slot.
	template <class CallbackClass>
	CL_Slot connect(
		CallbackClass *cb_class,
		void (CallbackClass::*callback)(PARAM1))
	{
		return connect(new CL_MethodSlot_v1<CallbackClass, PARAM1>(cb_class, callback));
	}

	//: Connect member function with user data slot.
	template <class CallbackClass, class UserData>
	CL_Slot connect(
		CallbackClass *cb_class,
		void (CallbackClass::*callback)(PARAM1, UserData),
		UserData user_data)
	{
		return connect(new CL_UserDataMethodSlot_v1<CallbackClass, PARAM1, UserData>(cb_class, callback, user_data));
	}

	//: Disconnect
	void disconnect(CL_Slot &disconnect_slot)
	{
		for (SlotIterator slot_it = impl->slots.begin(); slot_it != impl->slots.end();)
		{
			Slot slot = *slot_it;
			if (disconnect_slot.impl == slot)
			{
				if (impl->in_call)
				{
					impl->in_call_remove_slots->push_back(slot_it);
					++slot_it;
				}
				else
				{
					slot->release_signal_ref(this);
					slot_it = impl->slots.erase(slot_it);
				}
			}
			else
				++slot_it;
		}
	}

//! Implementation:
private:
	void catch_final(CL_Signal_v1_Generic *data, std::list<SlotIterator> &remove_slots)
	{
		data->in_call_remove_slots = 0;

		// remove all slots no longer connected to any CL_Slot.
		typename std::list<SlotIterator>::iterator remove_it;
		for (remove_it = remove_slots.begin(); remove_it != remove_slots.end(); remove_it++)
		{
			Slot slot = **remove_it;
			slot->release_signal_ref(this);
			data->slots.erase(*remove_it);
		}

		data->in_call = false;
		if (data->deleted) delete data;
	}

	CL_Signal_v1_Generic *impl;
};

#endif
