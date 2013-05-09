
#pragma once

class ElapsedTimer
{
public:
	ElapsedTimer();
	void reset();
	float update();

private:
	clan::ubyte64 last_time;
};
