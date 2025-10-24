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
template <typename RetVal, typename P1, typename P2>
class CL_Callback_Impl_2
{
public:
	virtual ~CL_Callback_Impl_2()
	{
	}

	virtual RetVal invoke(P1 p1, P2 p2) const = 0;
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
template <typename RetVal, typename P1, typename P2>
class CL_Callback_Impl_2_static : public CL_Callback_Impl_2<RetVal, P1, P2>
{
public:
	CL_Callback_Impl_2_static(RetVal (*static_func)(P1, P2))
	: static_func(static_func)
	{
	}

	RetVal invoke(P1 p1, P2 p2) const
	{
		return static_func(p1, p2);
	}

	RetVal (*static_func)(P1, P2);
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
template <typename RetVal, typename P1, typename P2, typename UserData>
class CL_Callback_Impl_2_static_user : public CL_Callback_Impl_2<RetVal, P1, P2>
{
public:
	CL_Callback_Impl_2_static_user(
		RetVal (*static_func)(P1, P2, UserData), const UserData &user_data)
	: static_func(static_func), user_data(user_data)
	{
	}

	RetVal invoke(P1 p1, P2 p2) const
	{
		return static_func(p1, p2, user_data);
	}

	RetVal (*static_func)(P1, P2, UserData);

	UserData user_data;
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
template <typename RetVal, typename P1, typename P2, typename InstanceClass>
class CL_Callback_Impl_2_member : public CL_Callback_Impl_2<RetVal, P1, P2>
{
public:
	CL_Callback_Impl_2_member(InstanceClass *instance,
		RetVal (InstanceClass::*member_func)(P1, P2))
	: instance(instance), member_func(member_func)
	{
	}

	RetVal invoke(P1 p1, P2 p2) const
	{
		return (instance->*member_func)(p1, p2);
	}

	InstanceClass *instance;

	RetVal (InstanceClass::*member_func)(P1, P2);
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
template <typename RetVal, typename P1, typename P2, typename InstanceClass, typename UserData>
class CL_Callback_Impl_2_member_user : public CL_Callback_Impl_2<RetVal, P1, P2>
{
public:
	CL_Callback_Impl_2_member_user(InstanceClass *instance,
		RetVal (InstanceClass::*member_func)(P1, P2, UserData), const UserData &user_data)
	: instance(instance), member_func(member_func), user_data(user_data)
	{
	}

	RetVal invoke(P1 p1, P2 p2) const
	{
		return (instance->*member_func)(p1, p2, user_data);
	}

	InstanceClass *instance;

	RetVal (InstanceClass::*member_func)(P1, P2, UserData);

	UserData user_data;
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/Signals! !header=core.h! !hide! \endxmlonly
template <typename RetVal, class P1, class P2, class Functor>
class CL_Callback_Impl_2_functor : public CL_Callback_Impl_2<RetVal, P1, P2>
{
public:
	CL_Callback_Impl_2_functor(Functor functor)
	: functor(functor)
	{
	}

	RetVal invoke(P1 p1, P2 p2) const
	{
		return functor(p1, p2);
	}

	Functor functor;
};

/// \brief CL_Callback_2
///
/// \xmlonly !group=Core/Signals! !header=core.h! \endxmlonly
template <typename RetVal, typename P1, typename P2>
class CL_Callback_2
{
public:
	CL_Callback_2()
	{
	}

	CL_Callback_2(const CL_Callback_2 &copy)
	: impl(copy.impl)
	{
	}

	CL_Callback_2(CL_Callback_Impl_2<RetVal, P1, P2> *impl)
	: impl(impl)
	{
	}

	CL_Callback_2(RetVal (*function)(P1, P2))
	: impl(new CL_Callback_Impl_2_static<RetVal, P1, P2>(function))
	{
	}

	template<typename UserData>
	CL_Callback_2(RetVal (*function)(P1, P2, UserData), const UserData &user_data)
	: impl(new CL_Callback_Impl_2_static_user<RetVal, P1, P2, UserData>(function, user_data))
	{
	}

	template<class InstanceClass>
	CL_Callback_2(InstanceClass *instance, RetVal (InstanceClass::*function)(P1, P2))
	: impl(new CL_Callback_Impl_2_member<RetVal, P1, P2, InstanceClass>(instance, function))
	{
	}

	template<class InstanceClass, typename UserData>
	CL_Callback_2(InstanceClass *instance, RetVal (InstanceClass::*function)(P1, P2, UserData), const UserData &user_data)
	: impl(new CL_Callback_Impl_2_member_user<RetVal, P1, P2, InstanceClass, UserData>(instance, function, user_data))
	{
	}

	void set(RetVal (*function)(P1, P2))
	{
		impl = CL_SharedPtr< CL_Callback_Impl_2<RetVal, P1, P2> >(new CL_Callback_Impl_2_static<RetVal, P1, P2>(function));
	}

	template<typename UserData>
	void set(RetVal (*function)(P1, P2, UserData), const UserData &user_data)
	{
		impl = CL_SharedPtr< CL_Callback_Impl_2<RetVal, P1, P2> >(new CL_Callback_Impl_2_static_user<RetVal, P1, P2, UserData>(function, user_data));
	}

	template<class InstanceClass>
	void set(InstanceClass *instance, RetVal (InstanceClass::*function)(P1, P2))
	{
		impl = CL_SharedPtr< CL_Callback_Impl_2<RetVal, P1, P2> >(new CL_Callback_Impl_2_member<RetVal, P1, P2, InstanceClass>(instance, function));
	}

	template<class InstanceClass, typename UserData>
	void set(InstanceClass *instance, RetVal (InstanceClass::*function)(P1, P2, UserData), const UserData &user_data)
	{
		impl = CL_SharedPtr< CL_Callback_Impl_2<RetVal, P1, P2> >(new CL_Callback_Impl_2_member_user<RetVal, P1, P2, InstanceClass, UserData>(instance, function, user_data));
	}

	void clear()
	{
		impl = CL_SharedPtr< CL_Callback_Impl_2<RetVal, P1, P2> >();
	}

	RetVal invoke(P1 p1, P2 p2) const
	{
		return impl->invoke(p1, p2);
	}

	bool is_null() const
	{
		return impl.is_null();
	}

private:
	CL_SharedPtr< CL_Callback_Impl_2<RetVal, P1, P2> > impl;
};

/// \brief CL_Callback_2_functor
///
/// \xmlonly !group=Core/Signals! !header=core.h! \endxmlonly
template <typename RetVal, typename P1, typename P2>
class CL_Callback_2_functor : public CL_Callback_2<RetVal, P1, P2>
{
public:
	CL_Callback_2_functor()
	{
	}

	CL_Callback_2_functor(const CL_Callback_2_functor &copy)
	: CL_Callback_2<RetVal, P1, P2>(copy)
	{
	}

	template<class Functor>
	CL_Callback_2_functor(Functor functor)
	: CL_Callback_2<RetVal, P1, P2>(new CL_Callback_Impl_2_functor<RetVal, P1, P2, Functor>(functor))
	{
	}

};


/// \}
