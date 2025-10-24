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

/// \addtogroup clanCore_Signals clanCore Signals
/// \{

#pragma once

#include "../System/exception.h"
#include "slot.h"
#include "signals_impl.h"

template <typename Param1, typename Param2, typename Param3, typename Param4> class CL_Super_v4;

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
template <class Param1, class Param2, class Param3, class Param4>
class CL_VirtualCallback_v4 : public CL_SlotCallback
{
public:
	virtual void invoke(Param1 param1, Param2 param2, Param3 param3, Param4 param4, CL_Super_v4<Param1, Param2, Param3, Param4> &super_func) = 0;
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
template <class Param1, class Param2, class Param3, class Param4>
class CL_Super_v4
{
public:
	void invoke(Param1 param1, Param2 param2, Param3 param3, Param4 param4)
	{
		std::vector< CL_SharedPtr<CL_SlotCallback> >::reverse_iterator it = it_super;
		while (it != it_end)
		{
			CL_Super_v4<Param1, Param2, Param3, Param4> parent;
			parent.it_super = it;
			parent.it_end = it_end;
			++parent.it_super;

			if (it->get()->valid && it->get()->enabled)
			{
				((CL_VirtualCallback_v4<Param1, Param2, Param3, Param4> *) it->get())->invoke(param1, param2, param3, param4, parent);
				return;
			}
			++it;
		}

		throw CL_Exception("Called non-invokable super function");
	}

	bool is_null() const
	{
		std::vector< CL_SharedPtr<CL_SlotCallback> >::reverse_iterator it = it_super;
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

	std::vector< CL_SharedPtr<CL_SlotCallback> >::reverse_iterator it_super;

	std::vector< CL_SharedPtr<CL_SlotCallback> >::reverse_iterator it_end;
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
template <class Param1, class Param2, class Param3, class Param4>
class CL_VirtualCallback_v4_static : public CL_VirtualCallback_v4<Param1, Param2, Param3, Param4>
{
public:
	CL_VirtualCallback_v4_static(void (*static_func)(Param1, Param2, Param3, Param4, CL_Super_v4<Param1, Param2, Param3, Param4> &))
	: static_func(static_func) { return; }
	void invoke(Param1 param1, Param2 param2, Param3 param3, Param4 param4, CL_Super_v4<Param1, Param2, Param3, Param4> &super_func) { static_func(param1, param2, param3, param4, super_func); }
	void (*static_func)(Param1, Param2, Param3, Param4, CL_Super_v4<Param1, Param2, Param3, Param4> &);
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
template <class Param1, class Param2, class Param3, class Param4, class UserData>
class CL_VirtualCallback_v4_static_user : public CL_VirtualCallback_v4<Param1, Param2, Param3, Param4>
{
public:
	CL_VirtualCallback_v4_static_user(void (*static_func)(Param1, Param2, Param3, Param4, UserData, CL_Super_v4<Param1, Param2, Param3, Param4> &), const UserData &user_data)
	: static_func(static_func), user_data(user_data) { return; }
	void invoke(Param1 param1, Param2 param2, Param3 param3, Param4 param4, CL_Super_v4<Param1, Param2, Param3, Param4> &super_func) { static_func(param1, param2, param3, param4, user_data, super_func); }
	void (*static_func)(Param1, Param2, Param3, Param4, UserData, CL_Super_v4<Param1, Param2, Param3, Param4> &);
	UserData user_data;
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
template <class Param1, class Param2, class Param3, class Param4, class InstanceClass>
class CL_VirtualCallback_v4_member : public CL_VirtualCallback_v4<Param1, Param2, Param3, Param4>
{
public:
	CL_VirtualCallback_v4_member(InstanceClass *instance, void (InstanceClass::*member_func)(Param1, Param2, Param3, Param4, CL_Super_v4<Param1, Param2, Param3, Param4> &))
	: instance(instance), member_func(member_func) { return; }
	void invoke(Param1 param1, Param2 param2, Param3 param3, Param4 param4, CL_Super_v4<Param1, Param2, Param3, Param4> &super_func) { (instance->*member_func)(param1, param2, param3, param4, super_func); }
	InstanceClass *instance;
	void (InstanceClass::*member_func)(Param1, Param2, Param3, Param4, CL_Super_v4<Param1, Param2, Param3, Param4> &);
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
template <class Param1, class Param2, class Param3, class Param4, class InstanceClass, class UserData>
class CL_VirtualCallback_v4_member_user : public CL_VirtualCallback_v4<Param1, Param2, Param3, Param4>
{
public:
	CL_VirtualCallback_v4_member_user(InstanceClass *instance, void (InstanceClass::*member_func)(Param1, Param2, Param3, Param4, UserData, CL_Super_v4<Param1, Param2, Param3, Param4> &), const UserData &user_data)
	: instance(instance), member_func(member_func), user_data(user_data) { return; }
	void invoke(Param1 param1, Param2 param2, Param3 param3, Param4 param4, CL_Super_v4<Param1, Param2, Param3, Param4> &super_func) { (instance->*member_func)(param1, param2, param3, param4, user_data, super_func); }
	InstanceClass *instance;
	void (InstanceClass::*member_func)(Param1, Param2, Param3, Param4, UserData, CL_Super_v4<Param1, Param2, Param3, Param4> &);
	UserData user_data;
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
template <class Param1, class Param2, class Param3, class Param4, class Functor>
class CL_VirtualCallback_v4_functor : public CL_VirtualCallback_v4<Param1, Param2, Param3, Param4>
{
public:
	CL_VirtualCallback_v4_functor(const Functor &functor)
	: functor(functor) { return; }
	void invoke(Param1 param1, Param2 param2, Param3 param3, Param4 param4, CL_Super_v4<Param1, Param2, Param3, Param4> &super_func) { functor(param1, param2, param3, param4, super_func); }
	Functor functor;
};

/// \brief CL_VirtualFunction_v4
///
/// \xmlonly !group=Core/Signals! !header=core.h! \endxmlonly
template <class Param1, class Param2, class Param3, class Param4>
class CL_VirtualFunction_v4
{
/// \name Construction
/// \{

public:
	CL_VirtualFunction_v4()
	: impl(new CL_Signal_Impl) { return; }

	CL_VirtualFunction_v4(const CL_VirtualFunction_v4<Param1, Param2, Param3, Param4> &copy)
	: impl(copy.impl) { return; }


/// \}
/// \name Operations
/// \{

public:
	CL_Slot connect(void (*function)(Param1, Param2, Param3, Param4, CL_Super_v4<Param1, Param2, Param3, Param4> &))
	{
		clean_up();
		CL_SharedPtr<CL_SlotCallback> callback(
			new CL_VirtualCallback_v4_static<Param1, Param2, Param3, Param4>(function));
		impl->connected_slots.push_back(callback);
		return CL_Slot(callback);
	}

	template<class UserData>
	CL_Slot connect(void (*function)(Param1, Param2, Param3, Param4, UserData, CL_Super_v4<Param1, Param2, Param3, Param4> &), const UserData &user_data)
	{
		clean_up();
		CL_SharedPtr<CL_SlotCallback> callback(
			new CL_VirtualCallback_v4_static_user<Param1, Param2, Param3, Param4, UserData>(function, user_data));
		impl->connected_slots.push_back(callback);
		return CL_Slot(callback);
	}

	template<class InstanceClass>
	CL_Slot connect(InstanceClass *instance, void (InstanceClass::*function)(Param1, Param2, Param3, Param4, CL_Super_v4<Param1, Param2, Param3, Param4> &))
	{
		clean_up();
		CL_SharedPtr<CL_SlotCallback> callback(
			new CL_VirtualCallback_v4_member<Param1, Param2, Param3, Param4, InstanceClass>(instance, function));
		impl->connected_slots.push_back(callback);
		return CL_Slot(callback);
	}

	template<class InstanceClass, class UserData>
	CL_Slot connect(InstanceClass *instance, void (InstanceClass::*function)(Param1, Param2, Param3, Param4, UserData, CL_Super_v4<Param1, Param2, Param3, Param4> &), const UserData &user_data)
	{
		clean_up();
		CL_SharedPtr<CL_SlotCallback> callback(
			new CL_VirtualCallback_v4_member_user<Param1, Param2, Param3, Param4, InstanceClass, UserData>(instance, function, user_data));
		impl->connected_slots.push_back(callback);
		return CL_Slot(callback);
	}

	template<class Functor>
	CL_Slot connect_functor(const Functor &functor)
	{
		clean_up();
		CL_SharedPtr<CL_SlotCallback> callback(
			new CL_VirtualCallback_v4_functor<Param1, Param2, Param3, Param4, Functor>(functor));
		impl->connected_slots.push_back(callback);
		return CL_Slot(callback);
	}

	void invoke(Param1 param1, Param2 param2, Param3 param3, Param4 param4) const
	{
		std::vector< CL_SharedPtr<CL_SlotCallback> > callbacks = impl->connected_slots;
		CL_Super_v4<Param1, Param2, Param3, Param4> s;
		s.it_super = callbacks.rbegin();
		s.it_end = callbacks.rend();
		s.invoke(param1, param2, param3, param4);
	}


/// \}
/// \name Implementation
/// \{

private:
	void clean_up()
	{
		std::vector< CL_SharedPtr<CL_SlotCallback> >::size_type i, size;
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

	CL_SharedPtr<CL_Signal_Impl> impl;
/// \}
};


/// \}
