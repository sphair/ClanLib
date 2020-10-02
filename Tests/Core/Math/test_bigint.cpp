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

void TestApp::test_bigint(void)
{
	Console::write_line(" Header: bigint.h");
	Console::write_line("  Class: BigInt");


	Console::write_line("   Function: BigInt constructors");
	{
		BigInt value(1234);
		BigInt value2(value);
		BigInt value3;
		value3 = value;
		value.set(33);
		uint32_t result;
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

	Console::write_line("   Function: BigInt set(uint32_t) and get(uint32_t)");
	{
		BigInt value(1234);
		uint32_t result;
		value.get(result);
		if (result != 1234)
			fail();
		value.set(0xffffffff);
		value.get(result);
		if (result != 0xffffffff)
			fail();
		value.set(0x12345678);
		value.get(result);
		if (result != 0x12345678)
			fail();
	}

	Console::write_line("   Function: BigInt set(uint64_t) and get(uint64_t)");
	{
		BigInt value(1234);
		uint64_t result;
		value.get(result);
		if (result != 1234ULL)
			fail();
		value.set(0xffffffffffffffffULL);
		value.get(result);
		if (result != 0xffffffffffffffffULL)
			fail();
		value.set(0x12345678ABCD6543ULL);
		value.get(result);
		if (result != 0x12345678ABCD6543ULL)
			fail();
	}


	Console::write_line("   Function: BigInt set(int32_t) and get(int32_t)");
	{
		BigInt value(1234);
		int32_t result;
		value.get(result);
		if (result != 1234)
			fail();

		value.set(-1234);
		value.get(result);
		if (result != -1234)
			fail();
		value.set(0x7fffffff);
		value.get(result);
		if (result != 0x7fffffff)
			fail();
		value.set(-0x7fffffff);
		value.get(result);
		if (result != -0x7fffffff)
			fail();
	}

	Console::write_line("   Function: BigInt set(int64_t) and get(int64_t)");
	{
		BigInt value(1234LL);
		int64_t result;
		value.get(result);
		if (result != 1234LL)
			fail();

		value.set(-1234LL);
		value.get(result);
		if (result != -1234LL)
			fail();
		value.set(0x7fffffffffffffffLL);
		value.get(result);
		if (result != 0x7fffffffffffffffLL)
			fail();
		value.set(-0x7fffffffffffffffLL);
		value.get(result);
		if (result != -0x7fffffffffffffffLL)
			fail();
	}

	Console::write_line("   Function: operator + ");
	{
		BigInt value1(5);
		BigInt value2(3);
		BigInt value3;

		value3 = value1 + value2;
		int32_t result;
		value3.get(result);
		if (result != 8)
			fail();


	}
	Console::write_line("   Function: operator += ");
	{
		BigInt value1(5);
		BigInt value2(3);

		value1 += value2;
		int32_t result;
		value1.get(result);
		if (result != 8)
			fail();

		value1 += 1;
		value1.get(result);
		if (result != 9)
			fail();

	}
	Console::write_line("   Function: operator - ");
	{
		BigInt value1(5);
		BigInt value2(3);
		BigInt value3;

		value3 = value1 - value2;
		int32_t result;
		value3.get(result);
		if (result != 2)
			fail();
	}
	Console::write_line("   Function: operator -= ");
	{
		BigInt value1(5);
		BigInt value2(3);
	
		value1 -= value2;
		int32_t result;
		value1.get(result);
		if (result != 2)
			fail();
	}

	Console::write_line("   Function: operator * ");
	{
		BigInt value1(5);
		BigInt value2(3);
		BigInt value3;

		value3 = value1 * value2;
		int32_t result;
		value3.get(result);
		if (result != 15)
			fail();

	}
	Console::write_line("   Function: operator *= ");
	{
		BigInt value1(5);
		BigInt value2(3);

		value1 *= value2;
		int32_t result;
		value1.get(result);
		if (result != 15)
			fail();

		value1.set(-5);
		value2.set(3);
		value1 *= value2;
	
		value1.get(result);
		if (result != -15)
			fail();

		value1.set(5);
		value2.set(-3);
		value1 *= value2;
	
		value1.get(result);
		if (result != -15)
			fail();

		value1.set(-5);
		value2.set(-3);
		value1 *= value2;
	
		value1.get(result);
		if (result != 15)
			fail();

		value1.set(-5);
		value1 *= 3;
	
		value1.get(result);
		if (result != -15)
			fail();
	}
	Console::write_line("   Function: operator / ");
	{
		BigInt value1(9);
		BigInt value2(4);
		BigInt value3;

		value3 = value1 / value2;
		int32_t result;
		value3.get(result);
		if (result != 2)
			fail();


	}
	Console::write_line("   Function: operator /= ");
	{
		BigInt value1(9);
		BigInt value2(4);

		value1 /= value2;
		int32_t result;
		value1.get(result);
		if (result != 2)
			fail();
	}
	Console::write_line("   Function: operator % ");
	{
		BigInt value1(9);
		BigInt value2(4);
		BigInt value3;

		value3 = value1 % value2;
		int32_t result;
		value3.get(result);
		if (result != 1)
			fail();


	}
	Console::write_line("   Function: operator %= ");
	{
		BigInt value1(9);
		BigInt value2(4);

		value1 %= value2;
		int32_t result;
		value1.get(result);
		if (result != 1)
			fail();
	}

	Console::write_line("   Function: is_odd() ");
	{
		BigInt value;
		value.set(130);
		if (value.is_odd())
			fail();
		value.set(131);
		if (!value.is_odd())
			fail();
	}


	Console::write_line("   Function: is_even() ");
	{
		BigInt value;
		value.set(131);
		if (value.is_even())
			fail();
		value.set(130);
		if (!value.is_even())
			fail();
	}
}
