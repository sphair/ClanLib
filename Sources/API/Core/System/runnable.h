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

/// \addtogroup clanCore_System clanCore System
/// \{


#pragma once


#include "../api_core.h"
#include <typeinfo>

/// \brief Thread runnable object.
///
/// \xmlonly !group=Core/System! !header=core.h! \endxmlonly
class CL_API_CORE CL_Runnable
{
/// \name Construction
/// \{

public:
	CL_Runnable();

	virtual ~CL_Runnable();


/// \}
/// \name Attributes
/// \{

public:


/// \}
/// \name Operations
/// \{

public:
	virtual void run() = 0;

	/// \brief Sets the name (displayed in debuggers) of the calling thread.
	/** <p>Currently this only works with the Visual Studio compiler and debugger.</p>*/
	static void set_thread_name(const char *name);


/// \}
/// \name Implementation
/// \{

private:
/// \}
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/System! !header=core.h! !hide! \endxmlonly
template<class C>
class CL_RunnableMember_v0 : public CL_Runnable
{
/// \name Construction
/// \{

public:
	typedef void (C::*PtrMemberFunc)();

	CL_RunnableMember_v0(C *instance, PtrMemberFunc func)
	: instance(instance), func(func)
	{
	}


/// \}
/// \name Operations
/// \{

public:
	virtual void run()
	{
		set_thread_name(typeid(func).name());
		C *local_instance = instance;
		PtrMemberFunc local_func = func;
		delete this;
		(local_instance->*local_func)();
	}


/// \}
/// \name Implementation
/// \{

private:
	C *instance;

	PtrMemberFunc func;
/// \}
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/System! !header=core.h! !hide! \endxmlonly
template<class C, class P1>
class CL_RunnableMember_v1 : public CL_Runnable
{
/// \name Construction
/// \{

public:
	typedef void (C::*PtrMemberFunc)(P1 p1);

	CL_RunnableMember_v1(C *instance, PtrMemberFunc func, P1 p1)
	: instance(instance), func(func), p1(p1)
	{
	}


/// \}
/// \name Operations
/// \{

public:
	virtual void run()
	{
		set_thread_name(typeid(func).name());
		C *local_instance = instance;
		PtrMemberFunc local_func = func;
		P1 local_p1 = p1;
		delete this;
		(local_instance->*local_func)(local_p1);
	}


/// \}
/// \name Implementation
/// \{

private:
	C *instance;

	PtrMemberFunc func;

	P1 p1;
/// \}
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/System! !header=core.h! !hide! \endxmlonly
template<class C, class P1, class P2>
class CL_RunnableMember_v2 : public CL_Runnable
{
/// \name Construction
/// \{

public:
	typedef void (C::*PtrMemberFunc)(P1 p1, P2 p2);

	CL_RunnableMember_v2(C *instance, PtrMemberFunc func, P1 p1, P2 p2)
	: instance(instance), func(func), p1(p1), p2(p2)
	{
	}


/// \}
/// \name Operations
/// \{

public:
	virtual void run()
	{
		set_thread_name(typeid(func).name());
		C *local_instance = instance;
		PtrMemberFunc local_func = func;
		P1 local_p1 = p1;
		P2 local_p2 = p2;
		delete this;
		(local_instance->*local_func)(local_p1, local_p2);
	}


/// \}
/// \name Implementation
/// \{

private:
	C *instance;

	PtrMemberFunc func;

	P1 p1;

	P2 p2;
/// \}
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/System! !header=core.h! !hide! \endxmlonly
template<class C, class P1, class P2, class P3>
class CL_RunnableMember_v3 : public CL_Runnable
{
/// \name Construction
/// \{

public:
	typedef void (C::*PtrMemberFunc)(P1 p1, P2 p2, P3 p3);

	CL_RunnableMember_v3(C *instance, PtrMemberFunc func, P1 p1, P2 p2, P3 p3)
	: instance(instance), func(func), p1(p1), p2(p2), p3(p3)
	{
	}


/// \}
/// \name Operations
/// \{

public:
	virtual void run()
	{
		set_thread_name(typeid(func).name());
		C *local_instance = instance;
		PtrMemberFunc local_func = func;
		P1 local_p1 = p1;
		P2 local_p2 = p2;
		P3 local_p3 = p3;
		delete this;
		(local_instance->*local_func)(local_p1, local_p2, local_p3);
	}


/// \}
/// \name Implementation
/// \{

private:
	C *instance;

	PtrMemberFunc func;

	P1 p1;

	P2 p2;

	P3 p3;
/// \}
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/System! !header=core.h! !hide! \endxmlonly
template<class C, class P1, class P2, class P3, class P4>
class CL_RunnableMember_v4 : public CL_Runnable
{
/// \name Construction
/// \{

public:
	typedef void (C::*PtrMemberFunc)(P1 p1, P2 p2, P3 p3, P4 p4);

	CL_RunnableMember_v4(C *instance, PtrMemberFunc func, P1 p1, P2 p2, P3 p3, P4 p4)
	: instance(instance), func(func), p1(p1), p2(p2), p3(p3), p4(p4)
	{
	}


/// \}
/// \name Operations
/// \{

public:
	virtual void run()
	{
		set_thread_name(typeid(func).name());
		C *local_instance = instance;
		PtrMemberFunc local_func = func;
		P1 local_p1 = p1;
		P2 local_p2 = p2;
		P3 local_p3 = p3;
		P4 local_p4 = p4;
		delete this;
		(local_instance->*local_func)(local_p1, local_p2, local_p3, local_p4);
	}


/// \}
/// \name Implementation
/// \{

private:
	C *instance;

	PtrMemberFunc func;

	P1 p1;

	P2 p2;

	P3 p3;

	P4 p4;
/// \}
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/System! !header=core.h! !hide! \endxmlonly
template<class C, class P1, class P2, class P3, class P4, class P5>
class CL_RunnableMember_v5 : public CL_Runnable
{
/// \name Construction
/// \{

public:
	typedef void (C::*PtrMemberFunc)(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5);

	CL_RunnableMember_v5(C *instance, PtrMemberFunc func, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
	: instance(instance), func(func), p1(p1), p2(p2), p3(p3), p4(p4), p5(p5)
	{
	}


/// \}
/// \name Operations
/// \{

public:
	virtual void run()
	{
		set_thread_name(typeid(func).name());
		C *local_instance = instance;
		PtrMemberFunc local_func = func;
		P1 local_p1 = p1;
		P2 local_p2 = p2;
		P3 local_p3 = p3;
		P4 local_p4 = p4;
		P5 local_p5 = p5;
		delete this;
		(local_instance->*local_func)(local_p1, local_p2, local_p3, local_p4, local_p5);
	}


/// \}
/// \name Implementation
/// \{

private:
	C *instance;

	PtrMemberFunc func;

	P1 p1;

	P2 p2;

	P3 p3;

	P4 p4;

	P5 p5;
/// \}
};


/// \}
