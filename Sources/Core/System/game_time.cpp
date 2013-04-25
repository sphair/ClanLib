/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

GameTime::GameTime(int ticks_per_second)
	: impl(new GameTime_Impl(ticks_per_second))
{
	reset();
}

float GameTime::get_time_elapsed() const
{
	return impl->time_elapsed;
}

int GameTime::get_ticks_elapsed() const
{
	return impl->ticks_elapsed;
}

float GameTime::get_tick_interpolation_time() const
{
	return impl->tick_interpolation_time;
}

void GameTime::update()
{
	long long last_time = impl->current_time;

	impl->current_time = System::get_microseconds();

	long long delta_time = impl->current_time - last_time;

	long long ticks_per_microsecond = 1000000 / impl->ticks_per_second;
	long long current_tick = delta_time / ticks_per_microsecond;

	impl->ticks_elapsed = current_tick - impl->last_tick;
	impl->time_elapsed = (float)(delta_time / (double) 1000000);
	impl->tick_interpolation_time = (float)((delta_time % ticks_per_microsecond) / (double)ticks_per_microsecond);

	impl->last_tick = current_tick;
}

void GameTime::reset()
{
	impl->start_time = System::get_microseconds();
	impl->current_time = impl->start_time;
	impl->last_tick = 0;
	impl->time_elapsed = 0.0f;
	impl->ticks_elapsed = 0;
	impl->tick_interpolation_time = 0.0f;
}

}
