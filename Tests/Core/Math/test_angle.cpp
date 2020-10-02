/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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

void TestApp::test_angle()
{
	Console::write_line(" Header: angle.h");
	Console::write_line("  Class: Angle");

	Console::write_line("   Function: Angle(float value, AngleUnit unit)");
	{
		Angle angle(45.0f, AngleUnit::degrees);
		float f_angle = angle.to_degrees();
		if ( (f_angle < 44.9f) || (f_angle > 45.1f) )
			fail();

		Angle angle2(PI/4.0f, AngleUnit::radians);
		float f_angle2 = angle2.to_radians();
		if ( (f_angle2 < (PI/4.0f - 0.001f)) || (f_angle2 > (PI/4.0f + 0.001f) ) )
			fail();

	}

	Console::write_line("   Function: void normalize()");
	{
		for (float turns = -16.0f; turns < 16.0f; turns += 1.0f)
		{
			Angle angle(45.0f + turns * 360.0f, AngleUnit::degrees);
			angle.normalize();
			float f_angle = angle.to_degrees();
			if ( (f_angle < 44.9f) || (f_angle > 45.1f) )
				fail();
		}

	}

	Console::write_line("   Function: void normalize_180()");
	{
		check_normalize_180(0.0f, 0.0f);
		check_normalize_180(45.0f, 45.0f);
		check_normalize_180(90.0f, 90.0f);
		check_normalize_180(135.0f, 135.0f);
		check_normalize_180(180.0f, 180.0f);

		check_normalize_180(315.0f, -45.0f);
		check_normalize_180(270.0f, -90.0f);
		check_normalize_180(225.0f, -135.0f);
	}

}

void TestApp::check_normalize_180(float input_angle, float output_angle)
{
	Angle angle;
	angle.set_degrees(input_angle);
	angle.normalize_180();
	float f_angle = angle.to_degrees();
	if ( (f_angle < (output_angle - 0.001f)) || (f_angle > (output_angle + 0.001f) ) )
		fail();
}
