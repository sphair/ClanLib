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

#include "precomp.h"
#include "framerate_counter.h"

FramerateCounter::FramerateCounter()
: current_fps(0), start_time(0), frames(0)
{
}

void FramerateCounter::frame_shown()
{
	frames++;
	uint64_t current_time = clan::System::get_time();
	if (start_time == 0)
	{
		start_time = current_time;
	}
	else
	{
		int delta_time = current_time - start_time;
		if (delta_time < 0 || delta_time > 2000)
		{
			if (delta_time > 0)
				current_fps = (frames*1000) / delta_time;
			frames = 0;
			start_time = current_time;
		}
	}
}

void FramerateCounter::show_fps(clan::Canvas &canvas, clan::Font &fps_font)
{
	std::string fps = clan::string_format("%1 fps", get_framerate());
	fps_font.draw_text(canvas, canvas.get_width() - 100, 30, fps);
}
