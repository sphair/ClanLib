/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#ifndef header_threadfunc_v1
#define header_threadfunc_v1

#if _MSC_VER > 1000
#pragma once
#endif

#include "thread.h"

template<class Class, class Param>
class CL_ThreadFunc_Runnable_v1 : public CL_Runnable
{
public:
	typedef void (Class::*MemberFunc)(Param &param);

	CL_ThreadFunc_Runnable_v1(Class *self, MemberFunc func, const Param &param)
	: self(self), func(func), param(param)
	{
	}

private:
	virtual void run()
	{
		(*self.*func)(param);
	}

	Class *self;
	MemberFunc func;
	Param param;
};

//: Member function based thread callback interface.
//- <p>The CL_ThreadFunc_v1<MyClass, MyParam> is an interface used to call a member function
//- in a new thread.</p>
//-
//- <p>Following code demonstrates how it is used:
//- <code>
//- class MyClass
//- {
//-   CL_Thread thread;
//-
//-   MyClass() : thread(CL_ThreadFunc_v1(this, MyClass::worker_function, 15))
//-   {
//-     thread.start();
//-   }
//-
//-   void worker_function(int value) { assert(value == 15); }
//- };
//- </code></p>
#define CL_ThreadFunc_v1(a, b, c) CL_Thread(new CL_ThreadFunc_Runnable_v1(a, b, c), true)

/* Following code procudes internal compiler error with msvc++
template<class Class, class Param>
CL_Thread CL_ThreadFunc_v1(
	Class *self,
	CL_ThreadFunc_Runnable_v1<Class, Param>::MemberFunc func,
	const Param &param)
{
	return CL_Thread(new CL_ThreadFunc_Runnable_v1<Class, Param>(self, func, param), true);
}
*/

#endif
