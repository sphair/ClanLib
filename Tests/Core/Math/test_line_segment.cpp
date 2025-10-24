/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

void TestApp::test_line_segment2()
{
	CL_Console::write_line(" Header: line_segment.h");
	CL_Console::write_line("  Class: CL_LineSegment2");

	CL_Console::write_line("   Function: get_midpoint()");
	{
		CL_LineSegment2f line_a(CL_Vec2f(2.0f, 8.0f), CL_Vec2f(4.0f, 20.0f));
		if (line_a.get_midpoint() != CL_Vec2f(3.0f, 14.0f) ) fail();
	}

	CL_Console::write_line("   Function: point_right_of_line()");
	{
		CL_LineSegment2f line_a(CL_Vec2f(2.0f, 8.0f), CL_Vec2f(16.0f, 15.0f));
		float value = line_a.point_right_of_line(CL_Vec2f(11.0, 7.0f));
		if (value >= 0.0f) fail();
		value = line_a.point_right_of_line(CL_Vec2f(3.0, 16.0f));
		if (value <= 0.0f) fail();
		value = line_a.point_right_of_line(CL_Vec2f(21.0, 17.5f));
		if (value != 0.0f) fail();
		value = line_a.point_right_of_line(CL_Vec2f(7.0, 10.0f));
		if (value >= 0.0f) fail();
		value = line_a.point_right_of_line(CL_Vec2f(7.0, 11.0f));
		if (value <= 0.0f) fail();
	}

	CL_Console::write_line("   Function: normal()");
	{
		CL_LineSegment2f line_a(CL_Vec2f(0.0f, 10.0f), CL_Vec2f(0.0f, 20.0f));
		CL_Vec2f normal = line_a.normal();
		if (normal != CL_Vec2f(-1.0f, 0.0f)) fail();

		line_a = CL_LineSegment2f(CL_Vec2f(0.0f, 20.0f), CL_Vec2f(0.0f, 10.0f));
		normal = line_a.normal();
		if (normal != CL_Vec2f(1.0f, 0.0f)) fail();

		line_a = CL_LineSegment2f(CL_Vec2f(10.0f, 0.0f), CL_Vec2f(20.0f, 0.0f));
		normal = line_a.normal();
		if (normal != CL_Vec2f(0.0f, 1.0f)) fail();

		line_a = CL_LineSegment2f(CL_Vec2f(20.0f, 0.0f), CL_Vec2f(10.0f, 0.0f));
		normal = line_a.normal();
		if (normal != CL_Vec2f(0.0f, -1.0f)) fail();

		line_a = CL_LineSegment2f(CL_Vec2f(10.0f, 10.0f), CL_Vec2f(20.0f, 20.0f));
		normal = line_a.normal();
		if ( ! ( (normal.x >= -0.8f) && (normal.x <= -0.7f) ) ) fail();
		if ( ! ( (normal.y >= 0.7f) && (normal.y <= 0.8f) ) ) fail();

	}

	CL_Console::write_line("   Function: collinear()");
	{
		CL_LineSegment2f line_a(CL_Vec2f(2.0f, 8.0f), CL_Vec2f(4.0f, 12.0f));
		CL_LineSegment2f line_b(CL_Vec2f(4.0f, 12.0f), CL_Vec2f(6.0f, 16.0f));
		if (!line_a.collinear(line_b) ) fail();

		line_b = CL_LineSegment2f(CL_Vec2f(5.0f, 12.0f), CL_Vec2f(6.0f, 16.0f));
		if (line_a.collinear(line_b) ) fail();
	
		line_b = CL_LineSegment2f(CL_Vec2f(5.0f, 14.0f), CL_Vec2f(6.0f, 16.0f));
		if (!line_a.collinear(line_b) ) fail();

		line_b = CL_LineSegment2f(CL_Vec2f(2.0f, 8.0f), CL_Vec2f(4.0f, 12.0f));
		if (!line_a.collinear(line_b) ) fail();

	}
	CL_Console::write_line("   Function: intersects()");
	{
		CL_LineSegment2f line_a(CL_Vec2f(2.0f, 8.0f), CL_Vec2f(16.0f, 15.0f));
		CL_LineSegment2f line_b(CL_Vec2f(3.0f, 16.0f), CL_Vec2f(11.0f, 7.0f));
		if (!line_a.intersects(line_b, false) ) fail();

		line_b = CL_LineSegment2f(CL_Vec2f(-3.0f, 9.0f), CL_Vec2f(2.0f, 4.0f));
		if (line_a.intersects(line_b, false) ) fail();

		line_b = CL_LineSegment2f(CL_Vec2f(16.0f, 18.0f), CL_Vec2f(18.0f, 13.0f));
		if (line_a.intersects(line_b, false) ) fail();

		line_b = CL_LineSegment2f(CL_Vec2f(-2.0f, -8.0f), CL_Vec2f(-16.0f, -15.0f));
		if (line_a.intersects(line_b, false) ) fail();

		line_b = CL_LineSegment2f(CL_Vec2f(16.0f, 15.0f), CL_Vec2f(18.0f, 20.0f));
		if (!line_a.intersects(line_b, false) ) fail();

		line_b = CL_LineSegment2f(CL_Vec2f(2.0f, 6.0f), CL_Vec2f(16.0f, 13.0f));
		if (line_a.intersects(line_b, false) ) fail();

		line_b = CL_LineSegment2f(CL_Vec2f(2.0f, 8.0f), CL_Vec2f(16.0f + 14.0f, 15.0f + 7.0f));
		if (line_a.intersects(line_b, false) ) fail();

		line_b = CL_LineSegment2f(CL_Vec2f(2.0f, 8.0f), CL_Vec2f(16.0f + 14.0f, 15.0f + 7.0f));
		if (!line_a.intersects(line_b, true) ) fail();

		line_a = CL_LineSegment2f(CL_Vec2f(50.0f,75.0f),CL_Vec2f(75.0f,50.0f));
		line_b = CL_LineSegment2f(CL_Vec2f(73.4f,98.0f),CL_Vec2f(73.4f,73.4f));
		if (line_a.intersects(line_b, false)) fail();
	}

	CL_Console::write_line("   Function: get_intersection()");
	{
		bool did_intersect;

		CL_LineSegment2f line_a(CL_Vec2f(2.0f, 8.0f), CL_Vec2f(16.0f, 15.0f));
		CL_LineSegment2f line_b(CL_Vec2f(3.0f, 16.0f), CL_Vec2f(11.0f, 7.0f));
		CL_Vec2f dest_intercept = line_a.get_intersection(line_b, did_intersect);
		if (!did_intersect) fail();
		if ((dest_intercept.x <= 7.0f ) || (dest_intercept.x >= 8.0f )) fail();
		if ((dest_intercept.y <= 10.0f ) || (dest_intercept.y >= 11.0f )) fail();

		line_b = CL_LineSegment2f(CL_Vec2f(-3.0f, 9.0f), CL_Vec2f(2.0f, 4.0f));
		line_a.get_intersection(line_b, did_intersect);
		if (did_intersect) fail();

		line_b = CL_LineSegment2f(CL_Vec2f(16.0f, 20.0f), CL_Vec2f(26.0f, 15.0f));
		line_a.get_intersection(line_b, did_intersect);
		if (did_intersect) fail();

		// Parallel
		line_b = CL_LineSegment2f(CL_Vec2f(-2.0f, -8.0f), CL_Vec2f(-16.0f, -15.0f));
		line_a.get_intersection(line_b, did_intersect);
		if (did_intersect) fail();

		line_b = CL_LineSegment2f(CL_Vec2f(16.0f, 15.0f), CL_Vec2f(18.0f, 20.0f));
		dest_intercept = line_a.get_intersection(line_b, did_intersect);
		if (!did_intersect ) fail();
		if (dest_intercept != line_b.p) fail();

		line_b = CL_LineSegment2f(CL_Vec2f(2.0f, 8.0f), CL_Vec2f(18.0f, 20.0f));
		dest_intercept = line_a.get_intersection(line_b, did_intersect);
		if (!did_intersect) fail();
		if (dest_intercept != line_b.p) fail();

		// Parallel
		line_b = CL_LineSegment2f(CL_Vec2f(2.0f, 6.0f), CL_Vec2f(16.0f, 13.0f));
		line_a.get_intersection(line_b, did_intersect);
		if (did_intersect) fail();

		// Parallel
		line_b = CL_LineSegment2f(CL_Vec2f(2.0f, 8.0f), CL_Vec2f(16.0f + 14.0f, 15.0f + 7.0f));
		line_a.get_intersection(line_b, did_intersect);
		if (did_intersect) fail();
	}

	CL_Console::write_line("   Function: point_distance()");
	{
		CL_LineSegment2f line_a(CL_Vec2f(1.0f, 0.0f), CL_Vec2f(9.0f, 0.0f));
		CL_Vec2f point;
		float distance;

		point = CL_Vec2f(-1.0f, 0.0f);
		distance = line_a.point_distance(point);

		if (distance != 2.0f ) fail();

		point = CL_Vec2f(6.0f, 0.0f);
		distance = line_a.point_distance(point);

		if (distance != 0.0f ) fail();

		point = CL_Vec2f(11.0f, 0.0f);
		distance = line_a.point_distance(point);

		if (distance != 2.0f ) fail();

		line_a = CL_LineSegment2f(CL_Vec2f(0.0f, 1.0f), CL_Vec2f(0.0f, 9.0f));

		point = CL_Vec2f(0.0f, -1.0f);
		distance = line_a.point_distance(point);

		if (distance != 2.0f ) fail();

		point = CL_Vec2f(0.0f, 6.0f);
		distance = line_a.point_distance(point);

		if (distance != 0.0f ) fail();

		point = CL_Vec2f(0.0f, 11.0f);
		distance = line_a.point_distance(point);

		if (distance != 2.0f ) fail();

		line_a = CL_LineSegment2f(CL_Vec2f(0.0f, 1.0f), CL_Vec2f(0.0f, 9.0f));

		point = CL_Vec2f(25.0f, 5.0f);
		distance = line_a.point_distance(point);

		if (distance != 25.0f ) fail();

	}
}

void TestApp::test_line_segment3()
{
	CL_Console::write_line(" Header: line_segment.h");
	CL_Console::write_line("  Class: CL_LineSegment3");

	CL_Console::write_line("   Function: get_midpoint()");
	{
		CL_LineSegment3f line_a(CL_Vec3f(2.0f, 8.0f, 1.0f), CL_Vec3f(4.0f, 20.0f, 3.0f));
		if (line_a.get_midpoint() != CL_Vec3f(3.0f, 14.0f, 2.0f) ) fail();
	}

	CL_Console::write_line("   Function: point_distance()");
	{
		CL_LineSegment3f line_a(CL_Vec3f(1.0f, 0.0f, 0.0f), CL_Vec3f(9.0f, 0.0f, 0.0f));
		CL_Vec3f point;
		CL_Vec3f intercept;
		float distance;

		point = CL_Vec3f(-1.0f, 0.0f, 0.0f);
		distance = line_a.point_distance(point, intercept);

		if (distance != 2.0f ) fail();
		if (intercept != CL_Vec3f(1.0f, 0.0f, 0.0f) ) fail();

		point = CL_Vec3f(6.0f, 0.0f, 0.0f);
		distance = line_a.point_distance(point, intercept);

		if (distance != 0.0f ) fail();
		if (intercept != CL_Vec3f(6.0f, 0.0f, 0.0f) ) fail();

		point = CL_Vec3f(11.0f, 0.0f, 0.0f);
		distance = line_a.point_distance(point, intercept);

		if (distance != 2.0f ) fail();
		if (intercept != CL_Vec3f(9.0f, 0.0f, 0.0f) ) fail();

		line_a = CL_LineSegment3f(CL_Vec3f(0.0f, 1.0f, 0.0f), CL_Vec3f(0.0f, 9.0f, 0.0f));

		point = CL_Vec3f(0.0f, -1.0f, 0.0f);
		distance = line_a.point_distance(point, intercept);

		if (distance != 2.0f ) fail();
		if (intercept != CL_Vec3f(0.0f, 1.0f, 0.0f) ) fail();

		point = CL_Vec3f(0.0f, 6.0f, 0.0f);
		distance = line_a.point_distance(point, intercept);

		if (distance != 0.0f ) fail();
		if (intercept != CL_Vec3f(0.0f, 6.0f, 0.0f) ) fail();

		point = CL_Vec3f(0.0f, 11.0f, 0.0f);
		distance = line_a.point_distance(point, intercept);

		if (distance != 2.0f ) fail();
		if (intercept != CL_Vec3f(0.0f, 9.0f, 0.0f) ) fail();

		line_a = CL_LineSegment3f(CL_Vec3f(0.0f, 0.0f, 1.0f), CL_Vec3f(0.0f, 0.0f, 9.0f));

		point = CL_Vec3f(0.0f, 0.0f, -1.0f);
		distance = line_a.point_distance(point, intercept);

		if (distance != 2.0f ) fail();
		if (intercept != CL_Vec3f(0.0f, 0.0f, 1.0f) ) fail();

		point = CL_Vec3f(0.0f, 0.0f, 6.0f);
		distance = line_a.point_distance(point, intercept);

		if (distance != 0.0f ) fail();
		if (intercept != CL_Vec3f(0.0f, 0.0f, 6.0f) ) fail();

		point = CL_Vec3f(0.0f, 0.0f, 11.0f);
		distance = line_a.point_distance(point, intercept);

		if (distance != 2.0f ) fail();
		if (intercept != CL_Vec3f(0.0f, 0.0f, 9.0f) ) fail();

		line_a = CL_LineSegment3f(CL_Vec3f(0.0f, 1.0f, 0.0f), CL_Vec3f(0.0f, 9.0f, 0.0f));

		point = CL_Vec3f(25.0f, 5.0f, 0.0f);
		distance = line_a.point_distance(point, intercept);

		if (distance != 25.0f ) fail();
		if (intercept != CL_Vec3f(0.0f, 5.0f, 0.0f) ) fail();
	}

}
