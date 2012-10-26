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

/// \addtogroup clanCore_Signals clanCore Signals
/// \{

#pragma once

#include "../api_core.h"
#include "../System/exception.h"
#include "slot.h"
#include "signals_impl.h"

namespace clan
{

class Super_v0;

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
class CL_API_CORE VirtualCallback_v0 : public SlotCallback
{
public:
	virtual void invoke(Super_v0 &super_func) = 0;
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
class CL_API_CORE Super_v0
{
public:
	void invoke()
	{
		std::vector< std::shared_ptr<SlotCallback> >::reverse_iterator it = it_super;
		while (it != it_end)
		{
			Super_v0 parent;
			parent.it_super = it;
			parent.it_end = it_end;
			++parent.it_super;

			if (it->get()->valid && it->get()->enabled)
			{
				((VirtualCallback_v0 *) it->get())->invoke(parent);
				return;
			}
			++it;
		}

		throw Exception("Called non-invokable super function");
	}

	bool is_null() const
	{
		std::vector< std::shared_ptr<SlotCallback> >::reverse_iterator it = it_super;
		while (it != it_end)
		{
			if (it->get()->valid && it->get()->enabled)
				return false;
			++it;
		}
		return true;
	}

	bool is_invokable() const
	{
		return !is_null();
	}

	std::vector< std::shared_ptr<SlotCallback> >::reverse_iterator it_super;

	std::vector< std::shared_ptr<SlotCallback> >::reverse_iterator it_end;
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
class CL_API_CORE VirtualCallback_v0_static : public VirtualCallback_v0
{
public:
	VirtualCallback_v0_static(void (*static_func)(Super_v0 &))
	: static_func(static_func) { return; }
	void invoke(Super_v0 &super_func) { static_func(super_func); }
	void (*static_func)(Super_v0 &);
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
template <class UserData>
class VirtualCallback_v0_static_user : public VirtualCallback_v0
{
public:
	VirtualCallback_v0_static_user(void (*static_func)(UserData, Super_v0 &), const UserData &user_data)
	: static_func(static_func), user_data(user_data) { return; }
	void invoke(Super_v0 &super_func) { static_func(user_data, super_func); }
	void (*static_func)(UserData, Super_v0 &);
	UserData user_data;
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
template <class InstanceClass>
class VirtualCallback_v0_member : public VirtualCallback_v0
{
public:
	VirtualCallback_v0_member(InstanceClass *instance, void (InstanceClass::*member_func)(Super_v0 &))
	: instance(instance), member_func(member_func) { return; }
	void invoke(Super_v0 &super_func) { (instance->*member_func)(super_func); }
	InstanceClass *instance;
	void (InstanceClass::*member_func)(Super_v0 &);
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
template <class InstanceClass, class UserData>
class VirtualCallback_v0_member_user : public VirtualCallback_v0
{
public:
	VirtualCallback_v0_member_user(InstanceClass *instance, void (InstanceClass::*member_func)(UserData, Super_v0 &), const UserData &user_data)
	: instance(instance), member_func(member_func), user_data(user_data) { return; }
	void invoke(Super_v0 &super_func) { (instance->*member_func)(user_data, super_func); }
	InstanceClass *instance;
	void (InstanceClass::*member_func)(UserData, Super_v0 &);
	UserData user_data;
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
template <class Functor>
class VirtualCallback_v0_functor : public VirtualCallback_v0
{
public:
	VirtualCallback_v0_functor(const Functor &functor)
	: functor(functor) { return; }
	void invoke(Super_v0 &super_func) { functor(super_func); }
	Functor functor;
};

/// \brief VirtualFunction_v0
///
/// \xmlonly !group=Core/Signals! !header=core.h! \endxmlonly
class CL_API_CORE VirtualFunction_v0
{
/// \name Construction
/// \{

public:
	VirtualFunction_v0()
	: impl(new Signal_Impl) { return; }

	VirtualFunction_v0(const VirtualFunction_v0 &copy)
	: impl(copy.impl) { return; }


/// \}
/// \name Operations
/// \{

public:
	Slot connect(void (*function)(Super_v0 &))
	{
		clean_up();
		std::shared_ptr<SlotCallback> callback(
			new VirtualCallback_v0_static(function));
		impl->connected_slots.push_back(callback);
		return Slot(callback);
	}

	template<class UserData>
	Slot connect(void (*function)(Super_v0 &), const UserData &user_data)
	{
		clean_up();
		std::shared_ptr<SlotCallback> callback(
			new VirtualCallback_v0_static_user<UserData>(function, user_data));
		impl->connected_slots.push_back(callback);
		return Slot(callback);
	}

	template<class InstanceClass>
	Slot connect(InstanceClass *instance, void (InstanceClass::*function)(Super_v0 &))
	{
		clean_up();
		std::shared_ptr<SlotCallback> callback(
			new VirtualCallback_v0_member<InstanceClass>(instance, function));
		impl->connected_slots.push_back(callback);
		return Slot(callback);
	}

	template<class InstanceClass, class UserData>
	Slot connect(InstanceClass *instance, void (InstanceClass::*function)(UserData, Super_v0 &), const UserData &user_data)
	{
		clean_up();
		std::shared_ptr<SlotCallback> callback(
			new VirtualCallback_v0_member_user<InstanceClass, UserData>(instance, function, user_data));
		impl->connected_slots.push_back(callback);
		return Slot(callback);
	}

	template<class Functor>
	Slot connect_functor(const Functor &functor)
	{
		clean_up();
		std::shared_ptr<SlotCallback> callback(
			new VirtualCallback_v0_functor<Functor>(functor));
		impl->connected_slots.push_back(callback);
		return Slot(callback);
	}

	void invoke() const
	{
		std::vector< std::shared_ptr<SlotCallback> > callbacks = impl->connected_slots;
		Super_v0 s;
		s.it_super = callbacks.rbegin();
		s.it_end = callbacks.rend();
		s.invoke();
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
