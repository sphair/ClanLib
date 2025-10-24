/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

class CL_Super_v0;

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
class CL_API_CORE CL_VirtualCallback_v0 : public CL_SlotCallback
{
public:
	virtual void invoke(CL_Super_v0 &super_func) = 0;
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
class CL_API_CORE CL_Super_v0
{
public:
	void invoke()
	{
		std::vector< CL_SharedPtr<CL_SlotCallback> >::reverse_iterator it = it_super;
		while (it != it_end)
		{
			CL_Super_v0 parent;
			parent.it_super = it;
			parent.it_end = it_end;
			++parent.it_super;

			if (it->get()->valid && it->get()->enabled)
			{
				((CL_VirtualCallback_v0 *) it->get())->invoke(parent);
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
class CL_API_CORE CL_VirtualCallback_v0_static : public CL_VirtualCallback_v0
{
public:
	CL_VirtualCallback_v0_static(void (*static_func)(CL_Super_v0 &))
	: static_func(static_func) { return; }
	void invoke(CL_Super_v0 &super_func) { static_func(super_func); }
	void (*static_func)(CL_Super_v0 &);
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
template <class UserData>
class CL_VirtualCallback_v0_static_user : public CL_VirtualCallback_v0
{
public:
	CL_VirtualCallback_v0_static_user(void (*static_func)(UserData, CL_Super_v0 &), const UserData &user_data)
	: static_func(static_func), user_data(user_data) { return; }
	void invoke(CL_Super_v0 &super_func) { static_func(user_data, super_func); }
	void (*static_func)(UserData, CL_Super_v0 &);
	UserData user_data;
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
template <class InstanceClass>
class CL_VirtualCallback_v0_member : public CL_VirtualCallback_v0
{
public:
	CL_VirtualCallback_v0_member(InstanceClass *instance, void (InstanceClass::*member_func)(CL_Super_v0 &))
	: instance(instance), member_func(member_func) { return; }
	void invoke(CL_Super_v0 &super_func) { (instance->*member_func)(super_func); }
	InstanceClass *instance;
	void (InstanceClass::*member_func)(CL_Super_v0 &);
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
template <class InstanceClass, class UserData>
class CL_VirtualCallback_v0_member_user : public CL_VirtualCallback_v0
{
public:
	CL_VirtualCallback_v0_member_user(InstanceClass *instance, void (InstanceClass::*member_func)(UserData, CL_Super_v0 &), const UserData &user_data)
	: instance(instance), member_func(member_func), user_data(user_data) { return; }
	void invoke(CL_Super_v0 &super_func) { (instance->*member_func)(user_data, super_func); }
	InstanceClass *instance;
	void (InstanceClass::*member_func)(UserData, CL_Super_v0 &);
	UserData user_data;
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
template <class Functor>
class CL_VirtualCallback_v0_functor : public CL_VirtualCallback_v0
{
public:
	CL_VirtualCallback_v0_functor(const Functor &functor)
	: functor(functor) { return; }
	void invoke(CL_Super_v0 &super_func) { functor(super_func); }
	Functor functor;
};

/// \brief CL_VirtualFunction_v0
///
/// \xmlonly !group=Core/Signals! !header=core.h! \endxmlonly
class CL_API_CORE CL_VirtualFunction_v0
{
/// \name Construction
/// \{

public:
	CL_VirtualFunction_v0()
	: impl(new CL_Signal_Impl) { return; }

	CL_VirtualFunction_v0(const CL_VirtualFunction_v0 &copy)
	: impl(copy.impl) { return; }


/// \}
/// \name Operations
/// \{

public:
	CL_Slot connect(void (*function)(CL_Super_v0 &))
	{
		clean_up();
		CL_SharedPtr<CL_SlotCallback> callback(
			new CL_VirtualCallback_v0_static(function));
		impl->connected_slots.push_back(callback);
		return CL_Slot(callback);
	}

	template<class UserData>
	CL_Slot connect(void (*function)(CL_Super_v0 &), const UserData &user_data)
	{
		clean_up();
		CL_SharedPtr<CL_SlotCallback> callback(
			new CL_VirtualCallback_v0_static_user<UserData>(function, user_data));
		impl->connected_slots.push_back(callback);
		return CL_Slot(callback);
	}

	template<class InstanceClass>
	CL_Slot connect(InstanceClass *instance, void (InstanceClass::*function)(CL_Super_v0 &))
	{
		clean_up();
		CL_SharedPtr<CL_SlotCallback> callback(
			new CL_VirtualCallback_v0_member<InstanceClass>(instance, function));
		impl->connected_slots.push_back(callback);
		return CL_Slot(callback);
	}

	template<class InstanceClass, class UserData>
	CL_Slot connect(InstanceClass *instance, void (InstanceClass::*function)(UserData, CL_Super_v0 &), const UserData &user_data)
	{
		clean_up();
		CL_SharedPtr<CL_SlotCallback> callback(
			new CL_VirtualCallback_v0_member_user<InstanceClass, UserData>(instance, function, user_data));
		impl->connected_slots.push_back(callback);
		return CL_Slot(callback);
	}

	template<class Functor>
	CL_Slot connect_functor(const Functor &functor)
	{
		clean_up();
		CL_SharedPtr<CL_SlotCallback> callback(
			new CL_VirtualCallback_v0_functor<Functor>(functor));
		impl->connected_slots.push_back(callback);
		return CL_Slot(callback);
	}

	void invoke() const
	{
		std::vector< CL_SharedPtr<CL_SlotCallback> > callbacks = impl->connected_slots;
		CL_Super_v0 s;
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
