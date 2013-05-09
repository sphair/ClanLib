
#include "precomp.h"
#include "elapsed_timer.h"

ElapsedTimer::ElapsedTimer()
: last_time(0)
{
}

void ElapsedTimer::reset()
{
	last_time = 0;
}

float ElapsedTimer::update()
{
	clan::ubyte64 current_time = clan::System::get_time();
	if (last_time == 0 || current_time < last_time)
	{
		last_time = current_time;
		return 0.0f;
	}
	else
	{
		unsigned int elapsed_time = current_time - last_time;
		last_time = current_time;
		return elapsed_time / 1000.0f;
	}
}
