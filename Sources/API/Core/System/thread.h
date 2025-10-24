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
#include "sharedptr.h"
#include "runnable.h"
#include "exception.h"

class CL_Thread_Impl;

/// \brief Thread class.
///
/// \xmlonly !group=Core/System! !header=core.h! \endxmlonly
class CL_API_CORE CL_Thread
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a thread object.
	CL_Thread();

	~CL_Thread();


/// \}
/// \name Attributes
/// \{

public:


/// \}
/// \name Operations
/// \{

public:
	/// \brief Starts a thread.
	void start(CL_Runnable *runnable);

	template<class C>
	void start(C *instance, void (C::*member)())
	{
		CL_Runnable *r = new CL_RunnableMember_v0<C>(instance, member);
		try
		{
			start(r);
		}
		catch (CL_Exception)
		{
			delete r;
			throw;
		}
	}

	template<class C, class P1>
	void start(C *instance, void (C::*member)(P1 p1), P1 p1)
	{
		CL_Runnable *r = new CL_RunnableMember_v1<C, P1>(instance, member, p1);
		try
		{
			start(r);
		}
		catch (CL_Exception)
		{
			delete r;
			throw;
		}
	}

	template<class C, class P1, class P2>
	void start(C *instance, void (C::*member)(P1 p1, P2 p2), P1 p1, P2 p2)
	{
		CL_Runnable *r = new CL_RunnableMember_v2<C, P1, P2>(instance, member, p1, p2);
		try
		{
			start(r);
		}
		catch (CL_Exception)
		{
			delete r;
			throw;
		}
	}

	template<class C, class P1, class P2, class P3>
	void start(C *instance, void (C::*member)(P1 p1, P2 p2, P3 p3), P1 p1, P2 p2, P3 p3)
	{
		CL_Runnable *r = new CL_RunnableMember_v3<C, P1, P2, P3>(instance, member, p1, p2, p3);
		try
		{
			start(r);
		}
		catch (CL_Exception)
		{
			delete r;
			throw;
		}
	}

	template<class C, class P1, class P2, class P3, class P4>
	void start(C *instance, void (C::*member)(P1 p1, P2 p2, P3 p3, P4 p4), P1 p1, P2 p2, P3 p3, P4 p4)
	{
		CL_Runnable *r = new CL_RunnableMember_v4<C, P1, P2, P3, P4>(instance, member, p1, p2, p3, p4);
		try
		{
			start(r);
		}
		catch (CL_Exception)
		{
			delete r;
			throw;
		}
	}

	template<class C, class P1, class P2, class P3, class P4, class P5>
	void start(C *instance, void (C::*member)(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5), P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
	{
		CL_Runnable *r = new CL_RunnableMember_v5<C, P1, P2, P3, P4, P5>(instance, member, p1, p2, p3, p4, p5);
		try
		{
			start(r);
		}
		catch (CL_Exception)
		{
			delete r;
			throw;
		}
	}

	/// \brief Blocks until thread has completed its execution.
	void join();

	/// \brief Sets the name (displayed in debuggers) of the calling thread.
	/** <p>Currently this only works with the Visual Studio compiler and debugger.</p>*/
	static void set_thread_name(const char *name);


/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_Thread_Impl> impl;
/// \}
};


/// \}
