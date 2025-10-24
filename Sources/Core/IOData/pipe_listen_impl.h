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

#pragma once


#include "API/Core/System/event.h"
class CL_PipeConnection;

class CL_PipeListen_Impl
{
/// \name Construction
/// \{

public:
	CL_PipeListen_Impl(const CL_String &name);

	~CL_PipeListen_Impl();


/// \}
/// \name Attributes
/// \{

public:
#ifdef WIN32
	HANDLE handle;

	CL_String name;
#else
	int handle;
#endif
	CL_Event accept_event;


/// \}
/// \name Operations
/// \{

public:
	CL_Event begin_accept();

	CL_PipeConnection complete_accept();

	void cancel_accept();

	CL_PipeConnection accept();


/// \}
/// \name Implementation
/// \{

private:
#ifdef WIN32
	OVERLAPPED accept_overlapped;

	bool async_io;
#endif
/// \}
};


