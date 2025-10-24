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

#include "Core/precomp.h"

#include "mutex_win32.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Mutex 0.4 backward compatibility

CL_Mutex *CL_Mutex::create()
{
	return new CL_Mutex;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Mutex win32 implementation

CL_Mutex::CL_Mutex()
: impl(new CL_Mutex_Generic)
{
	InitializeCriticalSection(&impl->mutex);
	impl->event = CreateEvent(NULL, FALSE, FALSE, NULL);
}

CL_Mutex::CL_Mutex(const CL_Mutex &copy)
{
	cl_assert(false);
}

void CL_Mutex::operator =(const CL_Mutex &copy)
{
	cl_assert(false);
}

CL_Mutex::~CL_Mutex()
{
	DeleteCriticalSection(&impl->mutex);
	CloseHandle(impl->event);
	delete impl;
}

void CL_Mutex::enter()
{
	EnterCriticalSection(&impl->mutex);
}

void CL_Mutex::leave()
{
	LeaveCriticalSection(&impl->mutex);
}

void CL_Mutex::wait() 
{ 
	WaitForSingleObject(impl->event, INFINITE);
}

void CL_Mutex::notify() 
{ 
	SetEvent(impl->event);
}

void CL_Mutex::notify_all() 
{ 
	cl_assert(false); 
}
