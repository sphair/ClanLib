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

//! clanCore="System"
//! header=core.h

#ifndef header_keepalive
#define header_keepalive

#include "thread.h"

#ifdef CL_API_DLL
#ifdef CL_CORE_EXPORT
#define CL_API_CORE __declspec(dllexport)
#else
#define CL_API_CORE __declspec(dllimport)
#endif
#else
#define CL_API_CORE
#endif

#if _MSC_VER > 1000
#pragma once
#endif

//: Keep alive callback interface.
//- !group=Core/System!
//- !header=core.h!
//- <p>If you want to add something that gets called in a
//- CL_System::keep_alive() call, inherit this class and create an instance
//- of it.</p>
class CL_API_CORE CL_KeepAlive
{
//! Construction:
public:
	//: Constructs and registers the object as a keep_alive listener.
	CL_KeepAlive();
	
	//: Unregisters the object as a keep_alive listener.
	virtual ~CL_KeepAlive();

//! Operations:
public:
	//: Called when CL_System::keep_alive() is called.
	virtual void keep_alive()=0;

//! Implementation:
private:
	//: Thread ID of thread this keep alive object was created in.
	CL_ThreadId create_thread_id;
};

#endif
