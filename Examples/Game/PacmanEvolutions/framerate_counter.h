
#pragma once

class FramerateCounter
{
public:
	FramerateCounter()
	: current_fps(0), start_time(0), frames(0)
	{
	}

	int get_framerate() const { return current_fps; }

	void frame_shown()
	{
		frames++;
		int current_time = clan::System::get_time();
		if (start_time == 0)
		{
			start_time = current_time;
		}
		else
		{
			int delta_time = current_time - start_time;
			if (delta_time < 0 || delta_time > 2000)
			{
				if (delta_time > 0)
					current_fps = (frames*1000) / delta_time;
				frames = 0;
				start_time = current_time;
			}
		}
	}

private:
	int current_fps;
	int start_time;
	int frames;
};
