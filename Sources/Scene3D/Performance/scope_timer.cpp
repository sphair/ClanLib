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

#include "Scene3D/precomp.h"
#include "API/Scene3D/Performance/scope_timer.h"

namespace clan
{


ScopeTimerResults ScopeTimerResults::instance;

ScopeTimerResults::ScopeTimerResults()
: start_time(0), end_time(0), current_start_time(0), current_end_time(0)
{
}

void ScopeTimerResults::start()
{
	instance.current_results.clear();
	instance.current_start_time = rdtsc();
	instance.current_end_time = instance.current_start_time;
}

void ScopeTimerResults::add_result(const ScopeTimerResult &result)
{
	for (size_t i = 0; i < instance.current_results.size(); i++)
	{
		if (instance.current_results[i].name == result.name)
		{
			instance.current_results[i].ticks += result.ticks;
			return;
		}
	}
	instance.current_results.push_back(result);
}

void ScopeTimerResults::end()
{
	instance.current_end_time = rdtsc();
	instance.results = instance.current_results;
	instance.start_time = instance.current_start_time;
	instance.end_time = instance.current_end_time;
}

int ScopeTimerResults::percentage(const char *name)
{
	unsigned long long elapsed = instance.end_time - instance.start_time;
	if (elapsed != 0)
	{
		unsigned long long ticks = 0;
		for (size_t i = 0; i < instance.results.size(); i++)
		{
			if (instance.results[i].name == name)
				ticks += instance.results[i].ticks;
		}
		return (int)(ticks * 100 / elapsed);
	}
	else
	{
		return 0;
	}
}

std::string ScopeTimerResults::timer_results()
{
	ScopeTimeFunction();
	std::map<const char *, bool> result_names;
	for (size_t i = 0; i < instance.results.size(); i++)
		result_names[instance.results[i].name] = true;

	std::string final_results;
	for (std::map<const char *, bool>::iterator it = result_names.begin(); it != result_names.end(); ++it)
	{
		int time_spent = percentage(it->first);
		if (time_spent > 0)
		{
			if (!final_results.empty())
				final_results += "\n";
			final_results += string_format("%1: %2%%", it->first, time_spent);
		}
	}
	return final_results;
}

}
