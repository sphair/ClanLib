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

#ifndef header_slot_v5
#define header_slot_v5

#if _MSC_VER > 1000
#pragma once
#endif

#include "slot_generic.h"

//: Slot V5 Class
//- !group=Signal/System!
//- !header=signals.h!
template <class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5>
class CL_Slot_v5 : public CL_Slot_Generic
{
public:
	//: Call
	virtual void call(PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4, PARAM5 param5)=0;
};

//: FunctionSlot V5 Class
//- !group=Signal/System!
//- !header=signals.h!
template <class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5>
class CL_FunctionSlot_v5 : public CL_Slot_v5<PARAM1, PARAM2, PARAM3, PARAM4, PARAM5>
{
public:
	typedef void (*Callback)(PARAM1, PARAM2, PARAM3, PARAM4, PARAM5);

	//: FunctionSlot V5 Constructor
	CL_FunctionSlot_v5(Callback callback) : callback(callback) { return; }

	//: Call
	virtual void call(PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4, PARAM5 param5) { callback(param1, param2, param3, param4, param5); }

private:
	Callback callback;
};

//: FunctorSlot V5 Class
//- !group=Signal/System!
//- !header=signals.h!
template<class Functor, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5>
class CL_FunctorSlot_v5 : public CL_Slot_v5<PARAM1, PARAM2, PARAM3, PARAM4, PARAM5>
{
public:
	//: FunctorSlot V5 Constructor
	CL_FunctorSlot_v5(const Functor &functor)
	: functor(functor) { return; }

	//: Call
	virtual void call(PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4, PARAM5 param5) { functor(param1, param2, param3, param4, param5); }

private:
	Functor functor;
};

//: Methane Slot V5 class
//- !group=Signal/System!
//- !header=signals.h!
template <class CallbackClass, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5>
class CL_MethodSlot_v5 : public CL_Slot_v5<PARAM1, PARAM2, PARAM3, PARAM4, PARAM5>
{
public:
	typedef void (CallbackClass::*Callback)(PARAM1, PARAM2, PARAM3, PARAM4, PARAM5);

	//: Methane Slot V5 constructor
	CL_MethodSlot_v5(CallbackClass *cb_class, Callback callback)
	: cb_class(cb_class), callback(callback) { return; }

	//: Call
	virtual void call(PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4, PARAM5 param5) { (cb_class->*callback)(param1, param2, param3, param4, param5); }

private:
	CallbackClass *cb_class;
	Callback callback;
};

//: User Data Methane Slot V5 Class
//- !group=Signal/System!
//- !header=signals.h!
template <class CallbackClass, class PARAM1, class PARAM2, class PARAM3, class PARAM4, class PARAM5, class UserData>
class CL_UserDataMethodSlot_v5 : public CL_Slot_v5<PARAM1, PARAM2, PARAM3, PARAM4, PARAM5>
{
public:
	typedef void (CallbackClass::*Callback)(PARAM1, PARAM2, PARAM3, PARAM4, PARAM5, UserData);

	//: User Data Methane Slot V5 Constructor
	CL_UserDataMethodSlot_v5(CallbackClass *cb_class, Callback callback, UserData user_data)
	: cb_class(cb_class), callback(callback), user_data(user_data) { return; }

	//: Call
	virtual void call(PARAM1 param1, PARAM2 param2, PARAM3 param3, PARAM4 param4, PARAM5 param5) { (cb_class->*callback)(param1, param2, param3, param4, param5, user_data); }

private:
	CallbackClass *cb_class;
	Callback callback;
	UserData user_data;
};

#endif
