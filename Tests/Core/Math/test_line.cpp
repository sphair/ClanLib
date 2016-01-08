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

void TestApp::test_line2()
{
	Console::write_line(" Header: line.h");
	Console::write_line("  Class: Line2");

	Console::write_line("   Function: Line2(const Vec2<Type> &point_p, const Vec2<Type> &point_q)");
	{
		Line2f line(Vec2f(2.0f, 4.0f), Vec2f(4.0f, 8.0f));
		if (line.p != Vec2f(2.0f, 4.0f)) fail();
		if (line.q != Vec2f(4.0f, 8.0f)) fail();
	}

	Console::write_line("   Function: point_right_of_line()");
	{
		Line2f line_a(Vec2f(2.0f, 8.0f), Vec2f(16.0f, 15.0f));
		float value = line_a.point_right_of_line(Vec2f(11.0, 7.0f));
		if (value >= 0.0f) fail();
		value = line_a.point_right_of_line(Vec2f(3.0, 16.0f));
		if (value <= 0.0f) fail();
		value = line_a.point_right_of_line(Vec2f(21.0, 17.5f));
		if (value != 0.0f) fail();
		value = line_a.point_right_of_line(Vec2f(7.0, 10.0f));
		if (value >= 0.0f) fail();
		value = line_a.point_right_of_line(Vec2f(7.0, 11.0f));
		if (value <= 0.0f) fail();
	}

	Console::write_line("   Function: Line2(const Vec2<Type> &point_p, Type gradient)");
	{
		Line2f line(Vec2f(0.0f, 2.0f), 0.25f);
		if (line.p != Vec2f(0.0f, 2.0f)) fail();
		if (line.q != Vec2f(1.0f, 0.25f)) fail();
	}

	Console::write_line("   Function: get_intersection()");
	{
		Line2f line_a(Vec2f(1.0f, 6.0f), Vec2f(4.0f, 0.0f));
		Line2f line_b(Vec2f(1.0f, 1.0f), Vec2f(5.0f, 3.0f));
		bool did_intersect;
		Vec2f intersect = line_a.get_intersection(line_b, did_intersect);
		if (!did_intersect) fail();
		if (intersect != Vec2f(3.0f, 2.0f) ) fail();

		line_a = Line2f(Vec2f(2.0f, 1.0f), Vec2f(3.0f, 1.0f));
		line_b = Line2f(Vec2f(3.0f, 0.0f), Vec2f(4.0f, 1.0f));
		intersect = line_a.get_intersection(line_b, did_intersect);
		if (!did_intersect) fail();
		if (intersect != Vec2f(4.0f, 1.0f) ) fail();

		line_a = Line2f(Vec2f(1.0f, 6.0f), Vec2f(4.0f, 0.0f));
		line_b = Line2f(Vec2f(1.0f, 9.0f), Vec2f(4.0f, 3.0f));
		intersect = line_a.get_intersection(line_b, did_intersect);
		if (did_intersect) fail();
	}
}

void TestApp::test_line3()
{
	Console::write_line(" Header: line.h");
	Console::write_line("  Class: Line3");

	Console::write_line("   Function: get_intersection()");
	{
		Line3f line_a(Vec3f(6.0f, -4.0f, 2.0f), Vec3f(7.0f, -6.0f, 7.0f));
		Line3f line_b(Vec3f(1.0f, 4.0f, -17.0f), Vec3f(2.0f, 3.0f, -15.0f));
		bool did_intersect;
		Vec3f intersect = line_a.get_intersection(line_b, did_intersect, 0.5f);
		if (!did_intersect) fail();
		if (intersect != Vec3f(3.0f, 2.0f, -13.0f) ) fail();

		line_a = Line3f(Vec3f(-1.0f, 2.0f, 4.0f), Vec3f(1.0f, 2.0f, 7.0f));
		line_b = Line3f(Vec3f(-4.0f, 4.0f, 6.0f), Vec3f(1.0f, 2.0f, 7.0f));
		intersect = line_a.get_intersection(line_b, did_intersect, 0.5f);
		if (!did_intersect) fail();
		if (intersect != Vec3f(1.0f, 2.0f, 7.0f) ) fail();

		line_a = Line3f(Vec3f(0.0f, -1.0f, 4.0f), Vec3f(5.0f, 2.0f, 1.0f));
		line_b = Line3f(Vec3f(2.0f, 5.0f, 6.0f), Vec3f(6.0f, 2.0f, 8.0f));
		intersect = line_a.get_intersection(line_b, did_intersect, 0.5f);
		if (did_intersect) fail();
	}
}
