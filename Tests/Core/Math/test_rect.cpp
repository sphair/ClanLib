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

void TestApp::test_rect(void)
{
	Console::write_line(" Header: rect.h");
	Console::write_line("  Class: Rect");

//XXXXXXXXXX
//XX####XXX
//XX####XXX
//XX####XXX
//XXXXXXXXXX

	Rect base_rect(2, 1, 6, 4);
	if (base_rect.get_width() != 4)
		fail();
	if (base_rect.get_height() != 3)
		fail();

	if (base_rect.left != 2)
		fail();

	if (base_rect.top != 1)
		fail();

	if (base_rect.contains(Point(base_rect.left - 1, base_rect.top)))
		fail();
	if (!base_rect.contains(Point(base_rect.left, base_rect.top)))
		fail();
	if (!base_rect.contains(Point(base_rect.left + 1, base_rect.top)))
		fail();
	if (base_rect.contains(Point(base_rect.left, base_rect.top-1)))
		fail();
	if (!base_rect.contains(Point(base_rect.left, base_rect.top+1)))
		fail();

	if (!base_rect.contains(Point(base_rect.right - 1, base_rect.top)))
		fail();
	if (base_rect.contains(Point(base_rect.right, base_rect.top)))
		fail();
	if (base_rect.contains(Point(base_rect.right + 1, base_rect.top)))
		fail();

	if (!base_rect.contains(Point(base_rect.right - 1, base_rect.bottom - 1)))
		fail();
	if (base_rect.contains(Point(base_rect.right - 1, base_rect.bottom)))
		fail();
	if (base_rect.contains(Point(base_rect.right - 1, base_rect.bottom + 1)))
		fail();

	if (!base_rect.contains(Point(base_rect.left, base_rect.bottom - 1)))
		fail();
	if (base_rect.contains(Point(base_rect.left, base_rect.bottom)))
		fail();
	if (base_rect.contains(Point(base_rect.left, base_rect.bottom + 1)))
		fail();

	Rect test_rect;
	
	test_rect = base_rect;
	if (!test_rect.is_overlapped(base_rect)) 
		fail();
	if (!base_rect.is_overlapped(test_rect)) 
		fail();

	test_rect = base_rect;
	test_rect.translate(Point(test_rect.get_width(), 0));
	if (test_rect.is_overlapped(base_rect)) 
		fail();
	if (base_rect.is_overlapped(test_rect)) 
		fail();

	test_rect = base_rect;
	test_rect.translate(Point(test_rect.get_width() - 1, 0));
	if (!test_rect.is_overlapped(base_rect)) 
		fail();
	if (!base_rect.is_overlapped(test_rect)) 
		fail();

	test_rect = base_rect;
	test_rect.translate(Point(0, test_rect.get_height()));
	if (test_rect.is_overlapped(base_rect)) 
		fail();
	if (base_rect.is_overlapped(test_rect)) 
		fail();

	test_rect = base_rect;
	test_rect.translate(Point(0, test_rect.get_height() - 1));
	if (!test_rect.is_overlapped(base_rect)) 
		fail();
	if (!base_rect.is_overlapped(test_rect)) 
		fail();

	
	test_rect = base_rect;
	if (!test_rect.is_inside(base_rect)) 
		fail();
	if (!base_rect.is_inside(test_rect)) 
		fail();

	test_rect = base_rect;
	test_rect.translate(Point(1, 0));
	if (test_rect.is_inside(base_rect)) 
		fail();
	if (base_rect.is_inside(test_rect)) 
		fail();
	test_rect = base_rect;
	test_rect.translate(Point(-1, 0));
	if (test_rect.is_inside(base_rect)) 
		fail();
	if (base_rect.is_inside(test_rect)) 
		fail();
	test_rect = base_rect;
	test_rect.translate(Point(0, 1));
	if (test_rect.is_inside(base_rect)) 
		fail();
	if (base_rect.is_inside(test_rect)) 
		fail();
	test_rect = base_rect;
	test_rect.translate(Point(0, -1));
	if (test_rect.is_inside(base_rect)) 
		fail();
	if (base_rect.is_inside(test_rect)) 
		fail();
}


