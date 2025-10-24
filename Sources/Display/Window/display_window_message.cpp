/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
**    Harry Storbacka
**    Mark Page
*/

#include "Display/precomp.h"
#include "API/Display/Window/display_window_message.h"
#include "API/Core/System/databuffer.h"

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayWindowMessage_Impl class:

class CL_DisplayWindowMessage_Impl
{
public:
	CL_DisplayWindowMessage_Impl()
	: null(false)
	{
	}

public:
	CL_DataBuffer msg_databuffer;
	CL_String msg_name;
	bool null;
};


/////////////////////////////////////////////////////////////////////////////
// CL_DisplayWindowMessage construction:

CL_DisplayWindowMessage::CL_DisplayWindowMessage()
: impl (new CL_DisplayWindowMessage_Impl)
{
}

CL_DisplayWindowMessage::~CL_DisplayWindowMessage()
{
}

CL_DisplayWindowMessage CL_DisplayWindowMessage::null()
{
	CL_DisplayWindowMessage msg;
	msg.impl->null = true;
	return msg;
}

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayWindowMessage attributes:

CL_DataBuffer CL_DisplayWindowMessage::get_msg(const CL_StringRef &name) const
{
	if (impl->null)
	{
		return CL_DataBuffer(0);
	}
	if (name == impl->msg_name)
	{
		return impl->msg_databuffer;
	}
	return CL_DataBuffer(0);
}

bool CL_DisplayWindowMessage::is_null() const
{
	return impl->null;
}

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayWindowMessage operations:

void CL_DisplayWindowMessage::set_msg(const CL_StringRef &name, CL_DataBuffer databuffer)
{
	impl->null = false;
	impl->msg_name = name;
	impl->msg_databuffer = databuffer;
}

/////////////////////////////////////////////////////////////////////////////
// CL_DisplayWindowMessage implementation:

