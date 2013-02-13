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

#pragma once

#include <intrin.h>

namespace clan
{

class ScopeTimerResult
{
public:
	ScopeTimerResult(const char *name, unsigned __int64 ticks) : name(name), ticks(ticks) { }

	const char *name;
	unsigned __int64 ticks;
};

class ScopeTimerResults
{
public:
	static void start();
	static void add_result(const ScopeTimerResult &result);
	static void end();
	static int percentage(const char *name);
	static std::string timer_results();

private:
	ScopeTimerResults();

	unsigned __int64 start_time;
	std::vector<ScopeTimerResult> results;
	unsigned __int64 end_time;

	unsigned __int64 current_start_time;
	std::vector<ScopeTimerResult> current_results;
	unsigned __int64 current_end_time;

	static ScopeTimerResults instance;
};

class ScopeTimer
{
public:
	ScopeTimer(const char *name)
	: name(name), start(__rdtsc())
	{
	}

	~ScopeTimer()
	{
		ScopeTimerResults::add_result(ScopeTimerResult(name, __rdtsc() - start));
	}

private:
	const char *name;
	unsigned __int64 start;
};

#define ScopeTimeFunction() ScopeTimer scope_timer(__FUNCTION__);

}
