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
**    Mark Page
**    (if your name is missing here, please add it)
*/

#include "test.h"

void TestApp::test_quaternion_f()
{
	Console::write_line(" Header: quaternion.h");
	Console::write_line("  Class: Quaternionf");

	{
		Quaternionf q(0.0f, 0.0f, 0.0f, angle_degrees, order_XYZ);
		check_float(q.i, 0.0f);
		check_float(q.j, 0.0f);
		check_float(q.k, 0.0f);

		check_float(q.w, 1.0f);

	}
}

void TestApp::test_quaternion_d()
{
	Console::write_line(" Header: quaternion.h");
	Console::write_line("  Class: Quaterniond");

	{
		Quaterniond q(0.0, 0.0, 0.0, angle_degrees, order_XYZ);
		check_double(q.i, 0.0);
		check_double(q.j, 0.0);
		check_double(q.k, 0.0);

		check_double(q.w, 1.0);

	}
}

void TestApp::check_float(float value, float target)
{
	const float tiny = 0.0001f;
	if ((value < (target - tiny)))
		fail();
	if ((value > (target + tiny)))
		fail();
}

void TestApp::check_double(double value, double target)
{
	const double tiny = 0.0001;
	if ((value < (target - tiny)))
		fail();
	if ((value > (target + tiny)))
		fail();
}
