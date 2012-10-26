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
**    Mark Page
*/

#pragma once

#include "balls.h"

typedef struct _BallPosition
{
	float xpos;
	float ypos;
	float xspeed;
	float yspeed;
	bool xdir;
	bool ydir;
} BallPosition;

class Balls
{
public:


	Balls();

	void Init(clan::Canvas &canvas);

	void Run(clan::Canvas &canvas);

private:
	void setup_balls();
	void move_balls(float time_diff, int num_balls);

private:
	clan::Image image_ball;

	float grid_space;

	static const int max_balls = 5;
	BallPosition balls[max_balls];

	unsigned int time_last;
};
