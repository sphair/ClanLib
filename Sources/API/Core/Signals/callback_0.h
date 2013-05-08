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

#include "../System/exception.h"
#include <memory>

namespace clan
{

/// (Internal ClanLib Class)
template <typename RetVal>
class Callback_Impl_0
{
public:
	virtual ~Callback_Impl_0()
	{
	}

	virtual RetVal invoke() const = 0;
};

/// (Internal ClanLib Class)
template <typename RetVal>
class Callback_Impl_0_static : public Callback_Impl_0<RetVal>
{
public:
	Callback_Impl_0_static(RetVal (*static_func)())
	: static_func(static_func)
	{
	}

	RetVal invoke() const
	{
		return static_func();
	}

	RetVal (*static_func)();
};

/// (Internal ClanLib Class)
template <typename RetVal, typename UserData>
class Callback_Impl_0_static_user : public Callback_Impl_0<RetVal>
{
public:
	Callback_Impl_0_static_user(
		RetVal (*static_func)(UserData), const UserData &user_data)
	: static_func(static_func), user_data(user_data)
	{
	}

	RetVal invoke() const
	{
		return static_func(user_data);
	}

	RetVal (*static_func)(UserData);

	UserData user_data;
};

/// (Internal ClanLib Class)
template <typename RetVal, typename InstanceClass>
class Callback_Impl_0_member : public Callback_Impl_0<RetVal>
{
public:
	Callback_Impl_0_member(InstanceClass *instance,
		RetVal (InstanceClass::*member_func)())
	: instance(instance), member_func(member_func)
	{
	}

	RetVal invoke() const
	{
		return (instance->*member_func)();
	}

	InstanceClass *instance;

	RetVal (InstanceClass::*member_func)();
};

/// (Internal ClanLib Class)
template <typename RetVal, typename InstanceClass, typename UserData>
class Callback_Impl_0_member_user : public Callback_Impl_0<RetVal>
{
public:
	Callback_Impl_0_member_user(InstanceClass *instance,
		RetVal (InstanceClass::*member_func)(UserData), const UserData &user_data)
	: instance(instance), member_func(member_func), user_data(user_data)
	{
	}

	RetVal invoke() const
	{
		return (instance->*member_func)(user_data);
	}

	InstanceClass *instance;

	RetVal (InstanceClass::*member_func)(UserData);

	UserData user_data;
};

/// (Internal ClanLib Class)
template <typename RetVal, class Functor>
class Callback_Impl_0_functor : public Callback_Impl_0<RetVal>
{
public:
	Callback_Impl_0_functor(Functor functor)
	: functor(functor)
	{
	}

	RetVal invoke() const
	{
		return functor();
	}

	Functor functor;
};

/// \brief Callback_0
///
template <typename RetVal>
class Callback_0
{
public:
	Callback_0()
	{
	}

	Callback_0(const Callback_0 &copy)
	: impl(copy.impl)
	{
	}

	Callback_0(Callback_Impl_0<RetVal> *impl)
	: impl(impl)
	{
	}

	Callback_0(RetVal (*function)())
	: impl(new Callback_Impl_0_static<RetVal>(function))
	{
	}

	template<typename UserData>
	Callback_0(RetVal (*function)(UserData), const UserData &user_data)
	: impl(new Callback_Impl_0_static_user<RetVal, UserData>(function, user_data))
	{
	}

	template<class InstanceClass>
	Callback_0(InstanceClass *instance, RetVal (InstanceClass::*function)())
	: impl(new Callback_Impl_0_member<RetVal, InstanceClass>(instance, function))
	{
	}

	template<class InstanceClass, typename UserData>
	Callback_0(InstanceClass *instance, RetVal (InstanceClass::*function)(UserData), const UserData &user_data)
	: impl(new Callback_Impl_0_member_user<RetVal, InstanceClass, UserData>(instance, function, user_data))
	{
	}

	void set(RetVal (*function)())
	{
		impl = std::shared_ptr< Callback_Impl_0<RetVal> >(new Callback_Impl_0_static<RetVal>(function));
	}

	template<typename UserData>
	void set(RetVal (*function)(UserData), const UserData &user_data)
	{
		impl = std::shared_ptr< Callback_Impl_0<RetVal> >(new Callback_Impl_0_static_user<RetVal, UserData>(function, user_data));
	}

	template<class InstanceClass>
	void set(InstanceClass *instance, RetVal (InstanceClass::*function)())
	{
		impl = std::shared_ptr< Callback_Impl_0<RetVal> >(new Callback_Impl_0_member<RetVal, InstanceClass>(instance, function));
	}

	template<class InstanceClass, typename UserData>
	void set(InstanceClass *instance, RetVal (InstanceClass::*function)(UserData), const UserData &user_data)
	{
		impl = std::shared_ptr< Callback_Impl_0<RetVal> >(new Callback_Impl_0_member_user<RetVal, InstanceClass, UserData>(instance, function, user_data));
	}

	void clear()
	{
		impl = std::shared_ptr< Callback_Impl_0<RetVal> >();
	}

	RetVal invoke() const
	{
		return impl->invoke();
	}

	bool is_null() const
	{
		return !impl;
	}

private:
	std::shared_ptr< Callback_Impl_0<RetVal> > impl;
};

/// \brief Callback_0_functor
///
template <typename RetVal>
class Callback_0_functor : public Callback_0<RetVal>
{
public:
	Callback_0_functor()
	{
	}

	Callback_0_functor(const Callback_0_functor &copy)
	: Callback_0<RetVal>(copy)
	{
	}

	template<class Functor>
	Callback_0_functor(Functor functor)
	: Callback_0<RetVal>(new Callback_Impl_0_functor<RetVal, Functor>(functor))
	{
	}

};

}

/// \}
