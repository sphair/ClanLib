
#pragma once

class FramerateCounter
{
public:
	FramerateCounter();
	void frame_rendered();
	float get_fps() const { return fps; }

private:
	unsigned int last_time;
	unsigned int frames;
	float fps;
};

inline FramerateCounter::FramerateCounter()
: last_time(0), frames(0), fps(0.0f)
{
}

inline void FramerateCounter::frame_rendered()
{
	unsigned int current_time = CL_System::get_time();
	if (last_time == 0 || current_time < last_time)
	{
		last_time = current_time;
		frames = 0;
	}

	frames++;

	unsigned int delta_time = current_time - last_time;
	if (delta_time >= 2000)
	{
		fps = frames / (delta_time / 1000.0f);
		frames = 0;
		last_time = current_time;
	}
}
