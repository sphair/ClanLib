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

/// \addtogroup clanCore_Signals clanCore Signals
/// \{

#pragma once

#include "../api_core.h"
#include "slot.h"
#include "signals_impl.h"

namespace clan
{

/// (Internal ClanLib Class)
class CL_API_CORE SlotCallback_v0 : public SlotCallback
{
public:
	virtual void invoke() = 0;
};

/// (Internal ClanLib Class)
class CL_API_CORE SlotCallback_v0_static : public SlotCallback_v0
{
public:
	SlotCallback_v0_static(void (*static_func)())
	: static_func(static_func) { return; }
	void invoke() { static_func(); }
	void (*static_func)();
};

/// (Internal ClanLib Class)
template <class UserData>
class SlotCallback_v0_static_user : public SlotCallback_v0
{
public:
	SlotCallback_v0_static_user(void (*static_func)(UserData), const UserData &user_data)
	: static_func(static_func), user_data(user_data) { return; }
	void invoke() { static_func(user_data); }
	void (*static_func)(UserData);
	UserData user_data;
};

/// (Internal ClanLib Class)
template <class InstanceClass>
class SlotCallback_v0_member : public SlotCallback_v0
{
public:
	SlotCallback_v0_member(InstanceClass *instance, void (InstanceClass::*member_func)())
	: instance(instance), member_func(member_func) { return; }
	void invoke() { (instance->*member_func)(); }
	InstanceClass *instance;
	void (InstanceClass::*member_func)();
};

/// (Internal ClanLib Class)
template <class InstanceClass, class UserData>
class SlotCallback_v0_member_user : public SlotCallback_v0
{
public:
	SlotCallback_v0_member_user(InstanceClass *instance, void (InstanceClass::*member_func)(UserData), const UserData &user_data)
	: instance(instance), member_func(member_func), user_data(user_data) { return; }
	void invoke() { (instance->*member_func)(user_data); }
	InstanceClass *instance;
	void (InstanceClass::*member_func)(UserData);
	UserData user_data;
};

/// (Internal ClanLib Class)
template <class Functor>
class SlotCallback_v0_functor : public SlotCallback_v0
{
public:
	SlotCallback_v0_functor(const Functor &functor)
	: functor(functor) { return; }
	void invoke() { functor(); }
	Functor functor;
};

/// \brief Signal_v0
class CL_API_CORE Signal_v0
{
/// \name Construction
/// \{

public:
	Signal_v0()
	: impl(new Signal_Impl) { return; }

	Signal_v0(const Signal_v0 &copy)
	: impl(copy.impl) { return; }


/// \}
/// \name Operations
/// \{

public:
	Slot connect(void (*function)())
	{
		clean_up();
		std::shared_ptr<SlotCallback> callback(
			new SlotCallback_v0_static(function));
		impl->connected_slots.push_back(callback);
		return Slot(callback);
	}

	template<class UserData>
	Slot connect(void (*function)(UserData), const UserData &user_data)
	{
		clean_up();
		std::shared_ptr<SlotCallback> callback(
			new SlotCallback_v0_static_user<UserData>(function, user_data));
		impl->connected_slots.push_back(callback);
		return Slot(callback);
	}

	template<class InstanceClass>
	Slot connect(InstanceClass *instance, void (InstanceClass::*function)())
	{
		clean_up();
		std::shared_ptr<SlotCallback> callback(
			new SlotCallback_v0_member<InstanceClass>(instance, function));
		impl->connected_slots.push_back(callback);
		return Slot(callback);
	}

	template<class InstanceClass, class UserData>
	Slot connect(InstanceClass *instance, void (InstanceClass::*function)(UserData), const UserData &user_data)
	{
		clean_up();
		std::shared_ptr<SlotCallback> callback(
			new SlotCallback_v0_member_user<InstanceClass, UserData>(instance, function, user_data));
		impl->connected_slots.push_back(callback);
		return Slot(callback);
	}

	template<class Functor>
	Slot connect_functor(const Functor &functor)
	{
		clean_up();
		std::shared_ptr<SlotCallback> callback(
			new SlotCallback_v0_functor<Functor>(functor));
		impl->connected_slots.push_back(callback);
		return Slot(callback);
	}

	void invoke() const
	{
		std::vector< std::shared_ptr<SlotCallback> > callbacks = impl->connected_slots;
		std::vector< std::shared_ptr<SlotCallback> >::size_type i, size;
		size = callbacks.size();
		for (i = 0; i < size; i++)
			if (callbacks[i]->valid && callbacks[i]->enabled)
				((SlotCallback_v0 *) callbacks[i].get())->invoke();
	}


/// \}
/// \name Implementation
/// \{

private:
	void clean_up()
	{
		std::vector< std::shared_ptr<SlotCallback> >::size_type i, size;
		size = impl->connected_slots.size();
		for (i = 0; i < size; i++)
		{
			if (!impl->connected_slots[i]->valid)
			{
				impl->connected_slots.erase(impl->connected_slots.begin()+i);
				i--;
				size--;
			}
		}
	}

	std::shared_ptr<Signal_Impl> impl;
/// \}
};

}

/// \}
