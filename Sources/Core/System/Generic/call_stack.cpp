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
#include "API/Core/System/call_stack.h"
#include "call_stack_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_CallStack Construction:

CL_CallStack::CL_CallStack() : impl(new CL_CallStack_Generic)
{
}

CL_CallStack::~CL_CallStack()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_CallStack Attributes:

int CL_CallStack::get_num_frames() const
{
	return impl->frames.size();
}

const std::string &CL_CallStack::get_call_frame(int frame) const
{
	return impl->frames[frame];
}

/////////////////////////////////////////////////////////////////////////////
// CL_CallStack Operations:

void CL_CallStack::add_call_frame(const std::string &str_frame)
{
	impl->frames.push_back(str_frame);
}

/////////////////////////////////////////////////////////////////////////////
// CL_CallStack Implementation:
