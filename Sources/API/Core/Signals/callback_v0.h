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
#include "../System/sharedptr.h"

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
class CL_Callback_Impl_v0
{
public:
	virtual ~CL_Callback_Impl_v0()
	{
	}

	virtual void invoke() const = 0;
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
class CL_Callback_Impl_v0_static : public CL_Callback_Impl_v0
{
public:
	CL_Callback_Impl_v0_static(void (*static_func)())
	: static_func(static_func)
	{
	}

	void invoke() const
	{
		static_func();
	}

	void (*static_func)();
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
template <typename UserData>
class CL_Callback_Impl_v0_static_user : public CL_Callback_Impl_v0
{
public:
	CL_Callback_Impl_v0_static_user(
		void (*static_func)(UserData), const UserData &user_data)
	: static_func(static_func), user_data(user_data)
	{
	}

	void invoke() const
	{
		static_func(user_data);
	}

	void (*static_func)(UserData);

	UserData user_data;
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
template <typename InstanceClass>
class CL_Callback_Impl_v0_member : public CL_Callback_Impl_v0
{
public:
	CL_Callback_Impl_v0_member(InstanceClass *instance,
		void (InstanceClass::*member_func)())
	: instance(instance), member_func(member_func)
	{
	}

	void invoke() const
	{
		(instance->*member_func)();
	}

	InstanceClass *instance;

	void (InstanceClass::*member_func)();
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
template <typename InstanceClass, typename UserData>
class CL_Callback_Impl_v0_member_user : public CL_Callback_Impl_v0
{
public:
	CL_Callback_Impl_v0_member_user(InstanceClass *instance,
		void (InstanceClass::*member_func)(UserData), const UserData &user_data)
	: instance(instance), member_func(member_func), user_data(user_data)
	{
	}

	void invoke() const
	{
		(instance->*member_func)(user_data);
	}

	InstanceClass *instance;

	void (InstanceClass::*member_func)(UserData);

	UserData user_data;
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
template <class Functor>
class CL_Callback_Impl_v0_functor : public CL_Callback_Impl_v0
{
public:
	CL_Callback_Impl_v0_functor(Functor functor)
	: functor(functor)
	{
	}

	void invoke() const
	{
		functor();
	}

	Functor functor;
};

/// \brief CL_Callback_v0
///
/// \xmlonly !group=Core/Signals! !header=core.h! \endxmlonly
class CL_Callback_v0
{
public:
	CL_Callback_v0()
	{
	}

	CL_Callback_v0(const CL_Callback_v0 &copy)
	: impl(copy.impl)
	{
	}

	CL_Callback_v0(CL_Callback_Impl_v0 *impl)
	: impl(impl)
	{
	}

	CL_Callback_v0(void (*function)())
	: impl(new CL_Callback_Impl_v0_static(function))
	{
	}

	template<typename UserData>
	CL_Callback_v0(void (*function)(UserData), const UserData &user_data)
	: impl(new CL_Callback_Impl_v0_static_user<UserData>(function, user_data))
	{
	}

	template<class InstanceClass>
	CL_Callback_v0(InstanceClass *instance, void (InstanceClass::*function)())
	: impl(new CL_Callback_Impl_v0_member<InstanceClass>(instance, function))
	{
	}

	template<class InstanceClass, typename UserData>
	CL_Callback_v0(InstanceClass *instance, void (InstanceClass::*function)(UserData), const UserData &user_data)
	: impl(new CL_Callback_Impl_v0_member_user<InstanceClass, UserData>(instance, function, user_data))
	{
	}

	void set(void (*function)())
	{
		impl = CL_SharedPtr< CL_Callback_Impl_v0 >(new CL_Callback_Impl_v0_static(function));
	}

	template<typename UserData>
	void set(void (*function)(UserData), const UserData &user_data)
	{
		impl = CL_SharedPtr< CL_Callback_Impl_v0 >(new CL_Callback_Impl_v0_static_user<UserData>(function, user_data));
	}

	template<class InstanceClass>
	void set(InstanceClass *instance, void (InstanceClass::*function)())
	{
		impl = CL_SharedPtr< CL_Callback_Impl_v0 >(new CL_Callback_Impl_v0_member<InstanceClass>(instance, function));
	}

	template<class InstanceClass, typename UserData>
	void set(InstanceClass *instance, void (InstanceClass::*function)(UserData), const UserData &user_data)
	{
		impl = CL_SharedPtr< CL_Callback_Impl_v0 >(new CL_Callback_Impl_v0_member_user<InstanceClass, UserData>(instance, function, user_data));
	}

	void clear()
	{
		impl = CL_SharedPtr< CL_Callback_Impl_v0 >();
	}

	void invoke() const
	{
		impl->invoke();
	}

	bool is_null() const
	{
		return impl.is_null();
	}

private:
	CL_SharedPtr< CL_Callback_Impl_v0 > impl;
};

/// \brief CL_Callback_v0_functor
///
/// \xmlonly !group=Core/Signals! !header=core.h! \endxmlonly
class CL_Callback_v0_functor : public CL_Callback_v0
{
public:
	CL_Callback_v0_functor()
	{
	}

	CL_Callback_v0_functor(const CL_Callback_v0_functor &copy)
	: CL_Callback_v0(copy)
	{
	}

	template<class Functor>
	CL_Callback_v0_functor(Functor functor)
	: CL_Callback_v0(new CL_Callback_Impl_v0_functor<Functor>(functor))
	{
	}

};


/// \}
