/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

namespace clan
{

class GameTime_Impl
{
public:
	GameTime_Impl(int ticks_per_second, int max_updates_per_second) : ticks_per_second(ticks_per_second), min_update_time_ms(0),
		start_time(0), current_time(0), time_elapsed(0.0f)
	{
		if (max_updates_per_second)
			min_update_time_ms = (1000/max_updates_per_second);
	}

	void update();
	void reset();

	int ticks_per_second;
	int min_update_time_ms;

	ubyte64 start_time;
	ubyte64 current_time;

	ubyte64 last_tick;

	float time_elapsed;
	int time_elapsed_ms;
	int ticks_elapsed;
	float tick_interpolation_time;

	ubyte64 update_frame_start_time;
	float current_fps;

private:
	void calculate_fps();
	void process_max_fps();

	int num_updates_in_2_seconds;
	int update_time_ms;

};

}
