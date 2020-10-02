/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
		Quaternionf q(0.0f, 0.0f, 0.0f, AngleUnit::degrees, EulerOrder::XYZ);
		check_float(q.i, 0.0f);
		check_float(q.j, 0.0f);
		check_float(q.k, 0.0f);

		check_float(q.w, 1.0f);

	}

	Console::write_line("   Function: set and to_matrix");
	{
		test_quaternion_euler(EulerOrder::XYZ);
		test_quaternion_euler(EulerOrder::XZY);
		test_quaternion_euler(EulerOrder::YZX);
		test_quaternion_euler(EulerOrder::YXZ);
		test_quaternion_euler(EulerOrder::ZXY);
		test_quaternion_euler(EulerOrder::ZYX);

	}
}

void TestApp::test_quaternion_euler(clan::EulerOrder order)
{
	for (int ax = 0; ax < 360; ax += 10)
	{
		for (int ay = 0; ay < 360; ay += 10)
		{
			for (int az = 0; az < 360; az += 10)
			{
				Quaternionf q;
				q.set(ax, ay, az, AngleUnit::degrees, order);
				clan::Mat4f test_matrix = q.to_matrix();

				Vec3f angles = test_matrix.get_euler(order);

				Mat4f test_matrix2;
				test_matrix2 = Mat4f::rotate(Angle(angles.x, AngleUnit::radians), Angle(angles.y, AngleUnit::radians), Angle(angles.z, AngleUnit::radians), order);

				// Note, since euler angles can have alternative forms, we compare by recreating as a rotation matrix
				if (!test_matrix.is_equal(test_matrix2, 0.00001f))
					fail();
			}
		}
	}

}

void TestApp::test_quaternion_d()
{
	Console::write_line(" Header: quaternion.h");
	Console::write_line("  Class: Quaterniond");

	{
		Quaterniond q(0.0, 0.0, 0.0, AngleUnit::degrees, EulerOrder::XYZ);
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
