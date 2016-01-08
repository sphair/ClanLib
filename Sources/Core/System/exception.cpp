/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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

namespace clan
{
	Exception::Exception(const std::string &message) : message(message)
	{
		num_frames = System::capture_stack_trace(1, max_frames, frames);
		for (int i = num_frames; i < max_frames; i++)
			frames[i] = nullptr;
	}

	const char* Exception::what() const throw()
	{
		// Note, buffer is mutable
		buffer = StringHelp::text_to_local8(message);
		return buffer.c_str();
	}

	std::vector<std::string> Exception::get_stack_trace() const
	{
		return System::get_stack_frames_text(frames, num_frames);
	}

	std::string Exception::get_message_and_stack_trace() const
	{
		std::vector<std::string> stack_trace = get_stack_trace();
		std::string text = message;
		for (auto & elem : stack_trace)
		{
#ifdef WIN32
			text += string_format("\r\n    at %1", elem);
#else
			text += string_format("\n    at %1", elem);
#endif
		}

		return text;
	}
}
