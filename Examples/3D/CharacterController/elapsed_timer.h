
#pragma once

class ElapsedTimer
{
public:
	ElapsedTimer() : last_time(clan::System::get_microseconds()) { }

	float seconds_elapsed()
	{
		long long current_time = clan::System::get_microseconds();

		double seconds = (current_time - last_time) * 1e-6;

		last_time = current_time;

		return static_cast<float>(seconds);
	}

private:
	long long last_time;
};
