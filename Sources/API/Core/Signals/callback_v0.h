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
class Callback_Impl_v0
{
public:
	virtual ~Callback_Impl_v0()
	{
	}

	virtual void invoke() const = 0;
};

/// (Internal ClanLib Class)
class Callback_Impl_v0_static : public Callback_Impl_v0
{
public:
	Callback_Impl_v0_static(void (*static_func)())
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
template <typename UserData>
class Callback_Impl_v0_static_user : public Callback_Impl_v0
{
public:
	Callback_Impl_v0_static_user(
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
template <typename InstanceClass>
class Callback_Impl_v0_member : public Callback_Impl_v0
{
public:
	Callback_Impl_v0_member(InstanceClass *instance,
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
template <typename InstanceClass, typename UserData>
class Callback_Impl_v0_member_user : public Callback_Impl_v0
{
public:
	Callback_Impl_v0_member_user(InstanceClass *instance,
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
template <class Functor>
class Callback_Impl_v0_functor : public Callback_Impl_v0
{
public:
	Callback_Impl_v0_functor(Functor functor)
	: functor(functor)
	{
	}

	void invoke() const
	{
		functor();
	}

	Functor functor;
};

/// \brief Callback_v0
class Callback_v0
{
public:
	Callback_v0()
	{
	}

	Callback_v0(const Callback_v0 &copy)
	: impl(copy.impl)
	{
	}

	Callback_v0(Callback_Impl_v0 *impl)
	: impl(impl)
	{
	}

	Callback_v0(void (*function)())
	: impl(new Callback_Impl_v0_static(function))
	{
	}

	template<typename UserData>
	Callback_v0(void (*function)(UserData), const UserData &user_data)
	: impl(new Callback_Impl_v0_static_user<UserData>(function, user_data))
	{
	}

	template<class InstanceClass>
	Callback_v0(InstanceClass *instance, void (InstanceClass::*function)())
	: impl(new Callback_Impl_v0_member<InstanceClass>(instance, function))
	{
	}

	template<class InstanceClass, typename UserData>
	Callback_v0(InstanceClass *instance, void (InstanceClass::*function)(UserData), const UserData &user_data)
	: impl(new Callback_Impl_v0_member_user<InstanceClass, UserData>(instance, function, user_data))
	{
	}

	void set(void (*function)())
	{
		impl = std::shared_ptr< Callback_Impl_v0 >(new Callback_Impl_v0_static(function));
	}

	template<typename UserData>
	void set(void (*function)(UserData), const UserData &user_data)
	{
		impl = std::shared_ptr< Callback_Impl_v0 >(new Callback_Impl_v0_static_user<UserData>(function, user_data));
	}

	template<class InstanceClass>
	void set(InstanceClass *instance, void (InstanceClass::*function)())
	{
		impl = std::shared_ptr< Callback_Impl_v0 >(new Callback_Impl_v0_member<InstanceClass>(instance, function));
	}

	template<class InstanceClass, typename UserData>
	void set(InstanceClass *instance, void (InstanceClass::*function)(UserData), const UserData &user_data)
	{
		impl = std::shared_ptr< Callback_Impl_v0 >(new Callback_Impl_v0_member_user<InstanceClass, UserData>(instance, function, user_data));
	}

	void clear()
	{
		impl = std::shared_ptr< Callback_Impl_v0 >();
	}

	void invoke() const
	{
		impl->invoke();
	}

	bool is_null() const
	{
		return !impl;
	}

private:
	std::shared_ptr< Callback_Impl_v0 > impl;
};

/// \brief Callback_v0_functor
class Callback_v0_functor : public Callback_v0
{
public:
	Callback_v0_functor()
	{
	}

	Callback_v0_functor(const Callback_v0_functor &copy)
	: Callback_v0(copy)
	{
	}

	template<class Functor>
	Callback_v0_functor(Functor functor)
	: Callback_v0(new Callback_Impl_v0_functor<Functor>(functor))
	{
	}

};

}

/// \}
