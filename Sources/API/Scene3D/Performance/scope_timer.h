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

#if defined(_MSC_VER)
#include <intrin.h>
#endif

namespace clan
{

#if defined(_MSC_VER)

static inline unsigned long long rdtsc()
{
	return __rdtsc();
}

#elif defined(__i386__)

static __inline__ unsigned long long rdtsc()
{
    unsigned long long int x;
    __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
    return x;
}

#elif defined(__x86_64__)

static __inline__ unsigned long long rdtsc()
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}

#else

static __inline__ unsigned long long rdtsc()
{
	return 0;
}

#endif

class ScopeTimerResult
{
public:
	ScopeTimerResult(const char *name, unsigned long long ticks) : name(name), ticks(ticks) { }

	const char *name;
	unsigned long long ticks;
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

	unsigned long long start_time;
	std::vector<ScopeTimerResult> results;
	unsigned long long end_time;

	unsigned long long current_start_time;
	std::vector<ScopeTimerResult> current_results;
	unsigned long long current_end_time;

	static ScopeTimerResults instance;
};

class ScopeTimer
{
public:
	ScopeTimer() : _name(), _start()
	{
	}

	ScopeTimer(const char *name) : _name(), _start()
	{
		start(name);
	}

	~ScopeTimer()
	{
		end();
	}

	void start(const char *name)
	{
		end();
		_name = name;
		_start = rdtsc();
	}

	void end()
	{
		if (_name)
		{
			ScopeTimerResults::add_result(ScopeTimerResult(_name, rdtsc() - _start));
			_name = 0;
		}
	}

private:
	ScopeTimer(const ScopeTimer &that);
	ScopeTimer &operator =(const ScopeTimer &that);

	const char *_name;
	unsigned long long _start;
};

#define ScopeTimeFunction() ScopeTimer scope_timer(__FUNCTION__);

}
