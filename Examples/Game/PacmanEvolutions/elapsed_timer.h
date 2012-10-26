
#pragma once

class ElapsedTimer
{
public:
	ElapsedTimer();
	void reset();
	float update();

private:
	unsigned int last_time;
};
