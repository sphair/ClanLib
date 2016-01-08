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

void TestApp::test_endian(void)
{
	Console::write_line(" Header: endian.h");
	Console::write_line("  Class: Endian");

	const int testdata_size = 16;
	char testdata_in[testdata_size]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
	char testdata_out[testdata_size]={8,7,6,5,4,3,2,1,16,15,14,13,12,11,10,9};
	Console::write_line("   Function: void swap(void *data, int type_size, int total_times=1)");
	Endian::swap(testdata_in, 8, 2);
	if (memcmp(testdata_in, testdata_out, testdata_size)) fail();

	Console::write_line("   Function: bool is_system_big()");
	bool result = Endian::is_system_big();

	int value = 1;
	if (result != (0 == ((char *) &value)[0])) fail();

	Console::write_line("   Function: bool is_system_64bit()");
	result = Endian::is_system_64bit();

	if ((sizeof(&value)==8) != result) fail();

}

