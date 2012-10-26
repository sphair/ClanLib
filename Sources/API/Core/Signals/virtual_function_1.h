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

#include "../System/exception.h"
#include "slot.h"
#include "signals_impl.h"

namespace clan
{

template <typename RetVal, typename Param1> class Super_1;

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
template <typename RetVal, class Param1>
class VirtualCallback_1 : public SlotCallback
{
public:
	virtual RetVal invoke(Param1 param1, Super_1<RetVal, Param1> &super_func) = 0;
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
template <typename RetVal, class Param1>
class Super_1
{
public:
	RetVal invoke(Param1 param1)
	{
		std::vector< std::shared_ptr<SlotCallback> >::reverse_iterator it = it_super;
		while (it != it_end)
		{
			Super_1<RetVal, Param1> parent;
			parent.it_super = it;
			parent.it_end = it_end;
			++parent.it_super;

			if (it->get()->valid && it->get()->enabled)
			{
				return ((VirtualCallback_1<RetVal, Param1> *) it->get())->invoke(param1, parent);
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
template <typename RetVal, class Param1>
class VirtualCallback_1_static : public VirtualCallback_1<RetVal, Param1>
{
public:
	VirtualCallback_1_static(RetVal (*static_func)(Param1, Super_1<RetVal, Param1> &))
	: static_func(static_func) { return; }
	RetVal invoke(Param1 param1, Super_1<RetVal, Param1> &super_func) { return static_func(param1, super_func); }
	RetVal (*static_func)(Param1, Super_1<RetVal, Param1> &);
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
template <typename RetVal, class Param1, class UserData>
class VirtualCallback_1_static_user : public VirtualCallback_1<RetVal, Param1>
{
public:
	VirtualCallback_1_static_user(RetVal (*static_func)(Param1, UserData, Super_1<RetVal, Param1> &), const UserData &user_data)
	: static_func(static_func), user_data(user_data) { return; }
	RetVal invoke(Param1 param1, Super_1<RetVal, Param1> &super_func) { return static_func(param1, user_data, super_func); }
	RetVal (*static_func)(Param1, UserData, Super_1<RetVal, Param1> &);
	UserData user_data;
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
template <typename RetVal, class Param1, class InstanceClass>
class VirtualCallback_1_member : public VirtualCallback_1<RetVal, Param1>
{
public:
	VirtualCallback_1_member(InstanceClass *instance, RetVal (InstanceClass::*member_func)(Param1, Super_1<RetVal, Param1> &))
	: instance(instance), member_func(member_func) { return; }
	RetVal invoke(Param1 param1, Super_1<RetVal, Param1> &super_func) { return (instance->*member_func)(param1, super_func); }
	InstanceClass *instance;
	RetVal (InstanceClass::*member_func)(Param1, Super_1<RetVal, Param1> &);
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
template <typename RetVal, class Param1, class InstanceClass, class UserData>
class VirtualCallback_1_member_user : public VirtualCallback_1<RetVal, Param1>
{
public:
	VirtualCallback_1_member_user(InstanceClass *instance, RetVal (InstanceClass::*member_func)(Param1, UserData, Super_1<RetVal, Param1> &), const UserData &user_data)
	: instance(instance), member_func(member_func), user_data(user_data) { return; }
	RetVal invoke(Param1 param1, Super_1<RetVal, Param1> &super_func) { return (instance->*member_func)(param1, user_data, super_func); }
	InstanceClass *instance;
	RetVal (InstanceClass::*member_func)(Param1, UserData, Super_1<RetVal, Param1> &);
	UserData user_data;
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
template <typename RetVal, class Param1, class Functor>
class VirtualCallback_1_functor : public VirtualCallback_1<RetVal, Param1>
{
public:
	VirtualCallback_1_functor(const Functor &functor)
	: functor(functor) { return; }
	RetVal invoke(Param1 param1, Super_1<RetVal, Param1> &super_func) { return functor(param1, super_func); }
	Functor functor;
};

/// \brief VirtualFunction_1
///
/// \xmlonly !group=Core/Signals! !header=core.h! \endxmlonly
template <typename RetVal, class Param1>
class VirtualFunction_1
{
/// \name Construction
/// \{

public:
	VirtualFunction_1()
	: impl(new Signal_Impl) { return; }

	VirtualFunction_1(const VirtualFunction_1<RetVal, Param1> &copy)
	: impl(copy.impl) { return; }


/// \}
/// \name Operations
/// \{

public:
	Slot connect(RetVal (*function)(Param1, Super_1<RetVal, Param1> &))
	{
		clean_up();
		std::shared_ptr<SlotCallback> callback(
			new VirtualCallback_1_static<RetVal, Param1>(function));
		impl->connected_slots.push_back(callback);
		return Slot(callback);
	}

	template<class UserData>
	Slot connect(RetVal (*function)(Param1, UserData, Super_1<RetVal, Param1> &), const UserData &user_data)
	{
		clean_up();
		std::shared_ptr<SlotCallback> callback(
			new VirtualCallback_1_static_user<RetVal, Param1, UserData>(function, user_data));
		impl->connected_slots.push_back(callback);
		return Slot(callback);
	}

	template<class InstanceClass>
	Slot connect(InstanceClass *instance, RetVal (InstanceClass::*function)(Param1, Super_1<RetVal, Param1> &))
	{
		clean_up();
		std::shared_ptr<SlotCallback> callback(
			new VirtualCallback_1_member<RetVal, Param1, InstanceClass>(instance, function));
		impl->connected_slots.push_back(callback);
		return Slot(callback);
	}

	template<class InstanceClass, class UserData>
	Slot connect(InstanceClass *instance, RetVal (InstanceClass::*function)(Param1, UserData, Super_1<RetVal, Param1> &), const UserData &user_data)
	{
		clean_up();
		std::shared_ptr<SlotCallback> callback(
			new VirtualCallback_1_member_user<RetVal, Param1, InstanceClass, UserData>(instance, function, user_data));
		impl->connected_slots.push_back(callback);
		return Slot(callback);
	}

	template<class Functor>
	Slot connect_functor(const Functor &functor)
	{
		clean_up();
		std::shared_ptr<SlotCallback> callback(
			new VirtualCallback_1_functor<RetVal, Param1, Functor>(functor));
		impl->connected_slots.push_back(callback);
		return Slot(callback);
	}

	RetVal invoke(Param1 param1) const
	{
		std::vector< std::shared_ptr<SlotCallback> > callbacks = impl->connected_slots;
		Super_1<RetVal, Param1> s;
		s.it_super = callbacks.rbegin();
		s.it_end = callbacks.rend();
		return s.invoke(param1);
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
