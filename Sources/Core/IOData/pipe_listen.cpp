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

#include "Core/precomp.h"
#include "API/Core/IOData/pipe_listen.h"
#include "API/Core/IOData/pipe_connection.h"
#include "pipe_listen_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_PipeListen Construction:

CL_PipeListen::CL_PipeListen(const CL_String &name)
: impl(new CL_PipeListen_Impl(name))
{
}

CL_PipeListen::~CL_PipeListen()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_PipeListen Attributes:

#ifdef WIN32
HANDLE CL_PipeListen::get_handle() const
{
	return impl->handle;
}
#else
int CL_PipeListen::get_handle() const
{
	return impl->handle;
}
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_PipeListen Operations:

CL_Event CL_PipeListen::begin_accept()
{
	return impl->begin_accept();
}

CL_PipeConnection CL_PipeListen::complete_accept()
{
	return impl->complete_accept();
}

void CL_PipeListen::cancel_accept()
{
	impl->cancel_accept();
}

CL_PipeConnection CL_PipeListen::accept()
{
	return impl->accept();
}

/////////////////////////////////////////////////////////////////////////////
// CL_PipeListen Implementation:
