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

#include "../api_core.h"
#include "../System/exception.h"
#include "slot.h"
#include "signals_impl.h"

template<typename RetVal> class CL_Super_0;

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
template<typename RetVal>
class CL_VirtualCallback_0 : public CL_SlotCallback
{
public:
	virtual RetVal invoke(CL_Super_0<RetVal> &super_func) = 0;
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
template<typename RetVal>
class CL_Super_0
{
public:
	RetVal invoke()
	{
		std::vector< CL_SharedPtr<CL_SlotCallback> >::reverse_iterator it = it_super;
		while (it != it_end)
		{
			CL_Super_0<RetVal> parent;
			parent.it_super = it;
			parent.it_end = it_end;
			++parent.it_super;

			if (it->get()->valid && it->get()->enabled)
			{
				return ((CL_VirtualCallback_0<RetVal> *) it->get())->invoke(parent);
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
template<typename RetVal>
class CL_VirtualCallback_0_static : public CL_VirtualCallback_0<RetVal>
{
public:
	CL_VirtualCallback_0_static(RetVal (*static_func)(CL_Super_0<RetVal> &))
	: static_func(static_func) { return; }
	RetVal invoke(CL_Super_0<RetVal> &super_func) { return static_func(super_func); }
	RetVal (*static_func)(CL_Super_0<RetVal> &);
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
template <typename RetVal, class UserData>
class CL_VirtualCallback_0_static_user : public CL_VirtualCallback_0<RetVal>
{
public:
	CL_VirtualCallback_0_static_user(void (*static_func)(UserData, CL_Super_0<RetVal> &), const UserData &user_data)
	: static_func(static_func), user_data(user_data) { return; }
	RetVal invoke(CL_Super_0<RetVal> &super_func) { return static_func(user_data, super_func); }
	RetVal (*static_func)(UserData, CL_Super_0<RetVal> &);
	UserData user_data;
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
template <typename RetVal, class InstanceClass>
class CL_VirtualCallback_0_member : public CL_VirtualCallback_0<RetVal>
{
public:
	CL_VirtualCallback_0_member(InstanceClass *instance, void (InstanceClass::*member_func)(CL_Super_0<RetVal> &))
	: instance(instance), member_func(member_func) { return; }
	RetVal invoke(CL_Super_0<RetVal> &super_func) { return (instance->*member_func)(super_func); }
	InstanceClass *instance;
	RetVal (InstanceClass::*member_func)(CL_Super_0<RetVal> &);
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
template <typename RetVal, class InstanceClass, class UserData>
class CL_VirtualCallback_0_member_user : public CL_VirtualCallback_0<RetVal>
{
public:
	CL_VirtualCallback_0_member_user(InstanceClass *instance, void (InstanceClass::*member_func)(UserData, CL_Super_0<RetVal> &), const UserData &user_data)
	: instance(instance), member_func(member_func), user_data(user_data) { return; }
	RetVal invoke(CL_Super_0<RetVal> &super_func) { return (instance->*member_func)(user_data, super_func); }
	InstanceClass *instance;
	RetVal (InstanceClass::*member_func)(UserData, CL_Super_0<RetVal> &);
	UserData user_data;
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
template <typename RetVal, class Functor>
class CL_VirtualCallback_0_functor : public CL_VirtualCallback_0<RetVal>
{
public:
	CL_VirtualCallback_0_functor(const Functor &functor)
	: functor(functor) { return; }
	RetVal invoke(CL_Super_0<RetVal> &super_func) { return functor(super_func); }
	Functor functor;
};

/// \brief CL_VirtualFunction_0
///
/// \xmlonly !group=Core/Signals! !header=core.h! \endxmlonly
template <typename RetVal>
class CL_VirtualFunction_0
{
/// \name Construction
/// \{

public:
	CL_VirtualFunction_0()
	: impl(new CL_Signal_Impl) { return; }

	CL_VirtualFunction_0(const CL_VirtualFunction_0<RetVal> &copy)
	: impl(copy.impl) { return; }


/// \}
/// \name Operations
/// \{

public:
	CL_Slot connect(void (*function)(CL_Super_0<RetVal> &))
	{
		clean_up();
		CL_SharedPtr<CL_SlotCallback> callback(
			new CL_VirtualCallback_0_static<RetVal>(function));
		impl->connected_slots.push_back(callback);
		return CL_Slot(callback);
	}

	template<class UserData>
	CL_Slot connect(RetVal (*function)(CL_Super_0<RetVal> &), const UserData &user_data)
	{
		clean_up();
		CL_SharedPtr<CL_SlotCallback> callback(
			new CL_VirtualCallback_0_static_user<RetVal, UserData>(function, user_data));
		impl->connected_slots.push_back(callback);
		return CL_Slot(callback);
	}

	template<class InstanceClass>
	CL_Slot connect(InstanceClass *instance, RetVal (InstanceClass::*function)(CL_Super_0<RetVal> &))
	{
		clean_up();
		CL_SharedPtr<CL_SlotCallback> callback(
			new CL_VirtualCallback_0_member<RetVal, InstanceClass>(instance, function));
		impl->connected_slots.push_back(callback);
		return CL_Slot(callback);
	}

	template<class InstanceClass, class UserData>
	CL_Slot connect(InstanceClass *instance, RetVal (InstanceClass::*function)(UserData, CL_Super_0<RetVal> &), const UserData &user_data)
	{
		clean_up();
		CL_SharedPtr<CL_SlotCallback> callback(
			new CL_VirtualCallback_0_member_user<RetVal, InstanceClass, UserData>(instance, function, user_data));
		impl->connected_slots.push_back(callback);
		return CL_Slot(callback);
	}

	template<class Functor>
	CL_Slot connect_functor(const Functor &functor)
	{
		clean_up();
		CL_SharedPtr<CL_SlotCallback> callback(
			new CL_VirtualCallback_0_functor<RetVal, Functor>(functor));
		impl->connected_slots.push_back(callback);
		return CL_Slot(callback);
	}

	RetVal invoke() const
	{
		std::vector< CL_SharedPtr<CL_SlotCallback> > callbacks = impl->connected_slots;
		CL_Super_0<RetVal> s;
		s.it_super = callbacks.rbegin();
		s.it_end = callbacks.rend();
		return s.invoke();
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
