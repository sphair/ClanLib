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
#include "API/Core/System/game_time.h"
#include "API/Core/System/system.h"
#include "game_time_impl.h"

namespace clan
{
	GameTime::GameTime(int ticks_per_second, int max_updates_per_second)
		: impl(std::make_shared<GameTime_Impl>(ticks_per_second, max_updates_per_second))
	{
		reset();
	}

	float GameTime::get_time_elapsed() const
	{
		return impl->time_elapsed;
	}

	int GameTime::get_time_elapsed_ms() const
	{
		return impl->time_elapsed_ms;
	}

	int GameTime::get_ticks_elapsed() const
	{
		return impl->ticks_elapsed;
	}

	float GameTime::get_tick_time_elapsed() const
	{
		return 1.0f / impl->ticks_per_second;
	}

	int GameTime::get_tick_time_elapsed_ms() const
	{
		return 1000 / impl->ticks_per_second;
	}

	float GameTime::get_tick_interpolation_time() const
	{
		return impl->tick_interpolation_time;
	}

	float GameTime::get_updates_per_second() const
	{
		return impl->current_fps;
	}

	float GameTime::get_current_time() const
	{
		double timer = ((double)(impl->current_time - impl->start_time)) / 1000000.0;
		return (float)timer;
	}

	uint64_t GameTime::get_current_time_microseconds() const
	{
		return (impl->current_time - impl->start_time);
	}

	uint64_t GameTime::get_current_time_ms() const
	{
		return (impl->current_time - impl->start_time) / 1000;
	}

	void GameTime::update()
	{
		impl->update();
	}

	void GameTime_Impl::update()
	{
		uint64_t last_time = current_time;

		current_time = System::get_microseconds();

		if (current_time < last_time)		// Old cpu's may report time travelling on early multicore processors (iirc)
			last_time = current_time;

		uint64_t ticks_per_microsecond = 1000000 / ticks_per_second;
		uint64_t current_tick = (current_time - start_time) / ticks_per_microsecond;

		ticks_elapsed = current_tick - last_tick;
		time_elapsed_ms = (int)((time_elapsed_ms_microsecond_adjustment + current_time - last_time) / 1000);
		time_elapsed_ms_microsecond_adjustment = (current_time - last_time) % 1000;
		time_elapsed = (float)((current_time - last_time) / (double)1000000);
		tick_interpolation_time = (float)(((current_time - start_time) % ticks_per_microsecond) / (double)ticks_per_microsecond);

		last_tick = current_tick;

		if (min_update_time_ms)	// Handle max fps
			process_max_fps();

		calculate_fps();
	}

	void GameTime_Impl::process_max_fps()
	{
		update_time_ms += time_elapsed_ms;

		int diff = min_update_time_ms - update_time_ms;
		if (diff > 0)	// We have spare cpu cycles
		{
			System::sleep(diff);
		}

		update_time_ms -= min_update_time_ms;		// Subtract by expected time for next iteration

		if (update_time_ms > min_update_time_ms)		// Limit slow updates, to give a chance to catch up
			update_time_ms = min_update_time_ms;
	}

	void GameTime_Impl::calculate_fps()
	{
		num_updates_in_2_seconds++;
		int delta_time_ms = (current_time - update_frame_start_time) / 1000;

		if ((delta_time_ms < 0) || (delta_time_ms > 2000))		// Sample FPS every 2 seconds
		{
			if (delta_time_ms > 0)
			{
				current_fps = (num_updates_in_2_seconds*1000.0f) / (float)delta_time_ms;
			}
			num_updates_in_2_seconds = 0;
			update_frame_start_time = current_time;
		}

	}

	void GameTime::reset()
	{
		impl->reset();
	}

	void GameTime_Impl::reset()
	{
		start_time = System::get_microseconds();
		current_time = start_time;
		last_tick = 0;
		time_elapsed = 0.0f;
		ticks_elapsed = 0;
		tick_interpolation_time = 0.0f;
		time_elapsed_ms = 0;
		time_elapsed_ms_microsecond_adjustment = 0;
		num_updates_in_2_seconds = 0;
		update_frame_start_time = current_time;
		current_fps = 0;
		num_updates_in_2_seconds = 0;
		update_time_ms = 0;

	}
}
