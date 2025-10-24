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
#include "API/Core/System/exception.h"
#include "API/Core/System/system.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Exception Construction:

CL_Exception::CL_Exception(const CL_String &message) : message(message)
{
	num_frames = CL_System::capture_stack_trace(1, max_frames, frames);
	for (int i = num_frames; i < max_frames; i++)
		frames[i] = 0;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Exception Attributes:

const char* CL_Exception::what() const throw()
{
	// Note, buffer is mutable
	buffer = CL_StringHelp::text_to_local8(message);
	return buffer.c_str();
}

/////////////////////////////////////////////////////////////////////////////
// CL_Exception Operations:

std::vector<CL_String> CL_Exception::get_stack_trace()
{
	return CL_System::get_stack_frames_text(frames, num_frames);
}

CL_String CL_Exception::get_message_and_stack_trace()
{
	std::vector<CL_String> stack_trace = get_stack_trace();
	CL_String text = message;
	for (size_t i = 0; i < stack_trace.size(); i++)
	{
	#ifdef WIN32
		text += cl_format("\r\n#%1 %2", (int)i, stack_trace[i]);
	#else
		text += cl_format("\n#%1 %2", (int)i, stack_trace[i]);
	#endif
	}

	return text;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Exception Implementation:
