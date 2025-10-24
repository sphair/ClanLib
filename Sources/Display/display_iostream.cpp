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

#include "Display/display_precomp.h"
#include <iostream>
#include "API/Display/display_window.h"
#include "API/Display/input_event.h"
#include "API/Display/input_device.h"
#include "API/Display/display_iostream.h"
#include "API/Core/core_iostream.h"

std::ostream& operator<<(std::ostream& s, const CL_DisplayWindow& window)
{
	s << "#<CL_DisplayWindow: " << window.get_width() << "x" << window.get_height() << ">";
	return s;
}

std::ostream& operator<<(std::ostream& s, const CL_InputDevice& device)
{
	s << "#<CL_InputDevice: '" << device.get_name() << "'>";
	return s;
}

std::ostream& operator<<(std::ostream& s, const CL_InputEvent& event)
{
	s << "#<CL_InputEvent: ";

	switch (event.type)
	{
	case CL_InputEvent::no_key:
      s << "no_key";
      break;
	case CL_InputEvent::pressed:
      s << "pressed";
		s << ", '" << event.device.get_key_name(event.id) << "'";
		s << ", " << event.mouse_pos; 
      break;
	case CL_InputEvent::released:
      s << "released";
		s << ", '" << event.device.get_key_name(event.id) << "'";
		s << ", " << event.mouse_pos; 
      break;
	case CL_InputEvent::pointer_moved:
      s << "pointer_moved";
		s << ", pos: " << event.mouse_pos; 
      break;
	case CL_InputEvent::axis_moved:
      s << "axis_moved";
		s << ", id: " << event.id;
		s << ", pos: " << event.axis_pos;
      break;
	case CL_InputEvent::ball_moved:
      s << "ball_moved";
		s << ", " << event.mouse_pos; 
      break;
	default:
      s << "unknown";
	}

	s << ", " << event.device;
	s << ">";

	return s;
}

// EOF //
