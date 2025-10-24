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
#include "API/Display/frameratecounter.h"
#include "API/Core/System/system.h"
#include "frameratecounter_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_FramerateCounter construction:

CL_FramerateCounter::CL_FramerateCounter()
: impl(new CL_FramerateCounter_Generic)
{
	impl->start_time = CL_System::get_time();
	impl->total_time = 0;
	impl->frame_time = 0;
	impl->fps_result = 0;
	impl->fps_counter = 0;
}

CL_FramerateCounter::~CL_FramerateCounter ()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// CL_FramerateCounter attributes:

int CL_FramerateCounter::get_fps()
{
	return impl->fps_result;
}

void CL_FramerateCounter::set_fps_limit(int fps) 
{
	if (fps > 0)
		impl->frame_time = 1000 / fps;
	else
		impl->frame_time = 0;
}


/////////////////////////////////////////////////////////////////////////////
// CL_FramerateCounter implementation:

void CL_FramerateCounter::keep_alive()
{
	int cur_time = CL_System::get_time();
	int delta_time = cur_time - impl->start_time;
	impl->start_time = cur_time;
	
	if (delta_time < impl->frame_time)
	{
		CL_System::sleep(impl->frame_time - delta_time);
		impl->total_time += impl->frame_time;
	}
	else
	{
		impl->total_time += delta_time;
	}
		
	if(impl->total_time >= 1000)	// One second has passed
	{
		impl->fps_result = impl->fps_counter + 1;
		impl->fps_counter = impl->total_time = 0;
	}
	impl->fps_counter++;	// Increase fps
}
