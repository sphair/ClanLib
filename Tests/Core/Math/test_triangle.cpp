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

void TestApp::test_triangle(void)
{
	Console::write_line(" Header: triangle_math.h");
	Console::write_line("  Class: Triangle");

	Console::write_line("   Function: point_inside()");
	{
		Trianglef triangle( Vec2f(8.0f, 8.0f), Vec2f(12.0f, 12.0f), Vec2f(-1.0f, 14.0f));
		if (!triangle.point_inside(Vec2f(6.0f, 11.0f))) fail();
		if (triangle.point_inside(Vec2f(4.0f, 9.0f))) fail();
		if (triangle.point_inside(Vec2f(10.0f, 9.0f))) fail();
		if (triangle.point_inside(Vec2f(8.0f, 13.0f))) fail();
	}
}


