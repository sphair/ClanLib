/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
		GameTime_Impl(int ticks_per_second, int max_updates_per_second) : ticks_per_second(ticks_per_second)
		{
			if (max_updates_per_second)
				min_update_time_ms = (1000 / max_updates_per_second);
		}

		void update();
		void reset();

		int ticks_per_second = 0;
		int min_update_time_ms = 0;

		uint64_t start_time = 0;
		uint64_t current_time = 0;

		uint64_t last_tick = 0;

		float time_elapsed = 0.0f;
		int time_elapsed_ms = 0;
		int time_elapsed_ms_microsecond_adjustment = 0;		// Amount of Microseconds lost due to time_elapsed_ms rounding (to add on for next time)

		int ticks_elapsed = 0;
		float tick_interpolation_time = 0.0f;

		uint64_t update_frame_start_time = 0;
		float current_fps = 0.0f;

	private:
		void calculate_fps();
		void process_max_fps();

		int num_updates_in_2_seconds = 0;
		int update_time_ms = 0;
	};
}
