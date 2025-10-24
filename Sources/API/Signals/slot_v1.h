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

//! clanSignal="System"
//! header=signals.h

#ifndef header_slot_v1
#define header_slot_v1

#if _MSC_VER > 1000
#pragma once
#endif

#include "slot_generic.h"

//: Slot V1 class
//- !group=Signal/System!
//- !header=signals.h!
template <class PARAM1>
class CL_Slot_v1 : public CL_Slot_Generic
{
public:
	//: Call
	virtual void call(PARAM1 param1)=0;
};

//: Funtion Slot V1 class
//- !group=Signal/System!
//- !header=signals.h!
template <class PARAM1>
class CL_FunctionSlot_v1 : public CL_Slot_v1<PARAM1>
{
public:
	typedef void (*Callback)(PARAM1);

	//: Function Slot V1 constructor
	CL_FunctionSlot_v1(Callback callback) : callback(callback) { return; }

	//: Call
	virtual void call(PARAM1 param1) { callback(param1); }

private:
	Callback callback;
};

//: User Data Function Slot V1 class
//- !group=Signal/System!
//- !header=signals.h!
template <class PARAM1, class UserData>
class CL_UserDataFunctionSlot_v1 : public CL_Slot_v1<PARAM1>
{
public:
	typedef void (*Callback)(PARAM1, UserData);

	//: Funtion Slot V1 constructor
	CL_UserDataFunctionSlot_v1(Callback callback, UserData user_data) : callback(callback), user_data(user_data)  { return; }

	//: Call
	virtual void call(PARAM1 param1) { callback(param1, user_data); }

private:
	Callback callback;
	UserData user_data;
};



//: FunctorSlot V1 Class
//- !group=Signal/System!
//- !header=signals.h!
template<class Functor, class PARAM1>
class CL_FunctorSlot_v1 : public CL_Slot_v1<PARAM1>
{
public:
	//: FunctorSlot V1 Constructor
	CL_FunctorSlot_v1(const Functor &functor)
		: functor(functor) { return; }

	//: Call
	virtual void call(PARAM1 param1) { functor(param1); }

private:
	Functor functor;
};

//: Method Slot V1
//- !group=Signal/System!
//- !header=signals.h!
template <class CallbackClass, class PARAM1>
class CL_MethodSlot_v1 : public CL_Slot_v1<PARAM1>
{
public:
	typedef void (CallbackClass::*Callback)(PARAM1);

	//: Method Slot V1 constructor
	CL_MethodSlot_v1(CallbackClass *cb_class, Callback callback)
		: cb_class(cb_class), callback(callback) { return; }

	//: Call
	virtual void call(PARAM1 param1) { (cb_class->*callback)(param1); }

private:
	CallbackClass *cb_class;
	Callback callback;
};

//: User Data Method Slot V1 class
//- !group=Signal/System!
//- !header=signals.h!
template <class CallbackClass, class PARAM1, class UserData>
class CL_UserDataMethodSlot_v1 : public CL_Slot_v1<PARAM1>
{
public:
	typedef void (CallbackClass::*Callback)(PARAM1, UserData);

	//: User Data Method Slot V1 constructor
	CL_UserDataMethodSlot_v1(CallbackClass *cb_class, Callback callback, UserData user_data)
		: cb_class(cb_class), callback(callback), user_data(user_data) { return; }

	//: Call
	virtual void call(PARAM1 param1) { (cb_class->*callback)(param1, user_data); }

private:
	CallbackClass *cb_class;
	Callback callback;
	UserData user_data;
};

#endif
