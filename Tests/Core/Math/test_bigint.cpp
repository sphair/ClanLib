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

void TestApp::test_bigint(void)
{
	Console::write_line(" Header: bigint.h");
	Console::write_line("  Class: BigInt");


	Console::write_line("   Function: BigInt constructors");
	{
		BigInt value;
		value.set(1234);
		BigInt value2(value);
		BigInt value3;
		value3 = value;
		value.set(33);
		ubyte32 result;
		value.get(result);
		if (result != 33)
			fail();
		value2.get(result);
		if (result != 1234)
			fail();
		value3.get(result);
		if (result != 1234)
			fail();

		value.set(111);
		value2.set(222);
		value3.set(333);
		value.get(result);
		if (result != 111)
			fail();
		value2.get(result);
		if (result != 222)
			fail();
		value3.get(result);
		if (result != 333)
			fail();
		
	}

	Console::write_line("   Function: BigInt set() and get()");
	{
		BigInt value;
		value.set(1234);
		ubyte32 result;
		value.get(result);
		if (result != 1234)
			fail();
	}
}


