/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

void TestApp::test_vector4(void)
{
	CL_Console::write_line(" Header: cl_vector.h");
	CL_Console::write_line("  Class: CL_Vec4");

	CL_Console::write_line("   Function: distance3()");
	{
		CL_Vec4d test_a(2.0,3.0,4.0,5.0);
		CL_Vec4d test_b(3.0,4.0,5.0,6.0);

		if (test_a.distance3(test_b) != sqrt(1.0 + 1.0 + 1.0 ))  fail();
	}

	CL_Console::write_line("   Function: distance4()");
	{
		CL_Vec4d test_a(2.0,3.0,4.0,5.0);
		CL_Vec4d test_b(3.0,4.0,5.0,6.0);

		if (test_a.distance4(test_b) != sqrt(1.0 + 1.0 + 1.0 + 1.0 ))  fail();
	}

	CL_Console::write_line("   Function: length3()");
	{
		CL_Vec4d testi(3.0,4.0,5.0,6.0);
		if (testi.length3() != sqrt(50.0 ))  fail();
	}

	CL_Console::write_line("   Function: length4()");
	{
		CL_Vec4d testi(3.0,4.0,5.0,6.0);
		if (testi.length4() != sqrt(86.0 ))  fail();
	}

	CL_Console::write_line("   Function: normalize3()");
	{
		CL_Vec4d testi(3.0,4.0,5.0,6.0);
		testi.normalize3();
		if (testi !=  CL_Vec4d(3.0/sqrt(50.0), 4.0/sqrt(50.0), 5.0/sqrt(50.0), 6.0))  fail();
	}

	CL_Console::write_line("   Function: static normalize3()");
	{
		CL_Vec4d testi(3.0,4.0,5.0,6.0);
		if (CL_Vec4d::normalize3(testi) !=  CL_Vec4d(3.0/sqrt(50.0), 4.0/sqrt(50.0), 5.0/sqrt(50.0), 6.0))  fail();
	}

	CL_Console::write_line("   Function: dot3()");
	{
		CL_Vec4d test_a(3.0,4.0,5.0,6.0);
		CL_Vec4d test_b(13.0,14.0,15.0,16.0);
		if (test_a.dot3(test_b) != ((3.0 * 13.0)+ (4.0*14.0) + (5.0 * 15.0)))  fail();
	}

	CL_Console::write_line("   Function: dot4()");
	{
		CL_Vec4d test_a(3.0,4.0,5.0,6.0);
		CL_Vec4d test_b(13.0,14.0,15.0,16.0);
		if (test_a.dot4(test_b) != ((3.0 * 13.0)+ (4.0*14.0) + (5.0 * 15.0) + (6.0 * 16.0)))  fail();
	}

	CL_Console::write_line("   Function: normalize4()");
	{
		CL_Vec4d testi(3.0,4.0,5.0,6.0);
		testi.normalize4();
		if (testi !=  CL_Vec4d(3.0/sqrt(86.0), 4.0/sqrt(86.0), 5.0/sqrt(86.0), 6.0/sqrt(86.0)))  fail();
	}

	CL_Console::write_line("   Function: static normalize4()");
	{
		CL_Vec4d testi(3.0,4.0,5.0,6.0);
		if (CL_Vec4d::normalize4(testi) !=  CL_Vec4d(3.0/sqrt(86.0), 4.0/sqrt(86.0), 5.0/sqrt(86.0), 6.0/sqrt(86.0)))  fail();
	}

	CL_Console::write_line("   Function: operator += (const CL_Vec4<Type>& vector)");
	{
		CL_Vec4d testd(2.5, 3.5, 4.5, 5.5);
		testd += CL_Vec4d(1.0, 2.0, 3.0, 4.0);

		if (testd.x != 3.5) fail();
		if (testd.y != 5.5) fail();
		if (testd.z != 7.5) fail();
		if (testd.w != 9.5) fail();

		CL_Vec4i testi(2, 3, 4, 5);
		testi += CL_Vec4i(1, 2, 3, 4);
		if (testi.x != 3) fail();
		if (testi.y != 5) fail();
		if (testi.z != 7) fail();
		if (testi.w != 9) fail();

	}

	CL_Console::write_line("   Function: operator += ( Type value)");
	{
		CL_Vec4d testd(2.5, 3.5, 4.5, 5.5);
		double valued = 2.0;
		testd += valued;

		if (testd.x != 4.5) fail();
		if (testd.y != 5.5) fail();
		if (testd.z != 6.5) fail();
		if (testd.w != 7.5) fail();

		CL_Vec4i testi(2, 3, 4, 5);
		int valuei = 2;
		testi += valuei;
		if (testi.x != 4) fail();
		if (testi.y != 5) fail();
		if (testi.z != 6) fail();
		if (testi.w != 7) fail();

	}

	CL_Console::write_line("   Function: operator + (Type value)");
	{
		CL_Vec4d testd(2.5, 3.5, 4.5, 5.5);
		double valued = 2.0;
		testd = testd + valued;

		if (testd.x != 4.5) fail();
		if (testd.y != 5.5) fail();
		if (testd.z != 6.5) fail();
		if (testd.w != 7.5) fail();

		CL_Vec4i testi(2, 3, 4, 5);
		int valuei = 2;
		testi = testi + valuei;
		if (testi.x != 4) fail();
		if (testi.y != 5) fail();
		if (testi.z != 6) fail();
		if (testi.w != 7) fail();

	}

	CL_Console::write_line("   Function: operator + (const CL_Vec4<Type>& vector)");
	{
		CL_Vec4d testd(2.5, 3.5, 4.5, 5.5);
		testd = testd + CL_Vec4d(1.5, 2.5, 3.5, 4.5);

		if (testd.x != 4.0) fail();
		if (testd.y != 6.0) fail();
		if (testd.z != 8.0) fail();
		if (testd.w != 10.0) fail();

		CL_Vec4i testi(2, 3, 4, 5);
		testi = testi + CL_Vec4i(1, 2, 3, 4);
		if (testi.x != 3) fail();
		if (testi.y != 5) fail();
		if (testi.z != 7) fail();
		if (testi.w != 9) fail();

	}

	CL_Console::write_line("   Function: operator -= (const CL_Vec4<Type>& vector)");
	{
		CL_Vec4d testd(2.5, 3.5, 4.5, 5.5);
		testd -= CL_Vec4d(1.0, 2.0, 3.0, 4.0);

		if (testd.x != 1.5) fail();
		if (testd.y != 1.5) fail();
		if (testd.z != 1.5) fail();
		if (testd.w != 1.5) fail();

		CL_Vec4i testi(2, 3, 4, 5);
		testi -= CL_Vec4i(1, 2, 3, 4);
		if (testi.x != 1) fail();
		if (testi.y != 1) fail();
		if (testi.z != 1) fail();
		if (testi.w != 1) fail();

	}

	CL_Console::write_line("   Function: operator -= ( Type value)");
	{
		CL_Vec4d testd(2.5, 3.5, 4.5, 5.5);
		double valued = 2.0;
		testd -= valued;

		if (testd.x != 0.5) fail();
		if (testd.y != 1.5) fail();
		if (testd.z != 2.5) fail();
		if (testd.w != 3.5) fail();

		CL_Vec4i testi(2, 3, 4, 5);
		int valuei = 2;
		testi -= valuei;
		if (testi.x != 0) fail();
		if (testi.y != 1) fail();
		if (testi.z != 2) fail();
		if (testi.w != 3) fail();

	}

	CL_Console::write_line("   Function: operator - (Type value)");
	{
		CL_Vec4d testd(2.5, 3.5, 4.5, 5.5);
		double valued = 2.0;
		testd = testd - valued;

		if (testd.x != 0.5) fail();
		if (testd.y != 1.5) fail();
		if (testd.z != 2.5) fail();
		if (testd.w != 3.5) fail();

		CL_Vec4i testi(2, 3, 4, 5);
		int valuei = 2;
		testi = testi - valuei;
		if (testi.x != 0) fail();
		if (testi.y != 1) fail();
		if (testi.z != 2) fail();
		if (testi.w != 3) fail();

	}

	CL_Console::write_line("   Function: operator - (const CL_Vec4<Type>& vector)");
	{
		CL_Vec4d testd(2.5, 3.5, 4.5, 5.5);
		testd = testd - CL_Vec4d(1.5, 2.5, 3.5, 4.5);

		if (testd.x != 1.0) fail();
		if (testd.y != 1.0) fail();
		if (testd.z != 1.0) fail();
		if (testd.w != 1.0) fail();

		CL_Vec4i testi(2, 3, 4, 5);
		testi = testi - CL_Vec4i(1, 2, 3, 4);
		if (testi.x != 1) fail();
		if (testi.y != 1) fail();
		if (testi.z != 1) fail();
		if (testi.w != 1) fail();

	}

	CL_Console::write_line("   Function: operator *= (const CL_Vec4<Type>& vector)");
	{
		CL_Vec4d testd(2.5, 3.5, 4.5, 5.5);
		testd *= CL_Vec4d(1.0, 2.0, 3.0, 4.0);

		if (testd.x != 2.5) fail();
		if (testd.y != 7.0) fail();
		if (testd.z != 13.5) fail();
		if (testd.w != 22.0) fail();

		CL_Vec4i testi(2, 3, 4, 5);
		testi *= CL_Vec4i(1, 2, 3, 4);
		if (testi.x != 2) fail();
		if (testi.y != 6) fail();
		if (testi.z != 12) fail();
		if (testi.w != 20) fail();

	}

	CL_Console::write_line("   Function: operator *= ( Type value)");
	{
		CL_Vec4d testd(2.5, 3.5, 4.5, 5.5);
		double valued = 2.0;
		testd *= valued;

		if (testd.x != 5.0) fail();
		if (testd.y != 7.0) fail();
		if (testd.z != 9.0) fail();
		if (testd.w != 11.0) fail();

		CL_Vec4i testi(2, 3, 4, 5);
		int valuei = 2;
		testi *= valuei;
		if (testi.x != 4) fail();
		if (testi.y != 6) fail();
		if (testi.z != 8) fail();
		if (testi.w != 10) fail();

	}

	CL_Console::write_line("   Function: operator * (Type value)");
	{
		CL_Vec4d testd(2.5, 3.5, 4.5, 5.5);
		double valued = 2.0;
		testd = testd * valued;

		if (testd.x != 5.0) fail();
		if (testd.y != 7.0) fail();
		if (testd.z != 9.0) fail();
		if (testd.w != 11.0) fail();

		CL_Vec4i testi(2, 3, 4, 5);
		int valuei = 2;
		testi = testi * valuei;
		if (testi.x != 4) fail();
		if (testi.y != 6) fail();
		if (testi.z != 8) fail();
		if (testi.w != 10) fail();

	}

	CL_Console::write_line("   Function: operator * (const CL_Vec4<Type>& vector)");
	{
		CL_Vec4d testd(2.5, 3.5, 4.5, 5.5);
		testd = testd * CL_Vec4d(1.5, 2.5, 3.5, 4.5);

		if (testd.x != 3.75) fail();
		if (testd.y != 8.75) fail();
		if (testd.z != 15.75) fail();
		if (testd.w != 24.75) fail();

		CL_Vec4i testi(2, 3, 4, 5);
		testi = testi * CL_Vec4i(1, 2, 3, 4);
		if (testi.x != 2) fail();
		if (testi.y != 6) fail();
		if (testi.z != 12) fail();
		if (testi.w != 20) fail();

	}

	CL_Console::write_line("   Function: operator /= (const CL_Vec4<Type>& vector)");
	{
		CL_Vec4d testd(2.5, 3.5, 4.5, 5.5);
		testd /= CL_Vec4d(1.0, 2.0, 3.0, 4.0);

		if (testd.x != 2.5) fail();
		if (testd.y != 1.75) fail();
		if (testd.z != 1.5) fail();
		if (testd.w != 1.375) fail();

		CL_Vec4i testi(2, 10, 20, 5);
		testi /= CL_Vec4i(1, 2, 3, 4);
		if (testi.x != 2) fail();
		if (testi.y != 5) fail();
		if (testi.z != 6) fail();
		if (testi.w != 1) fail();

	}

	CL_Console::write_line("   Function: operator /= ( Type value)");
	{
		CL_Vec4d testd(2.5, 3.5, 4.5, 5.5);
		double valued = 2.0;
		testd /= valued;

		if (testd.x != 1.25) fail();
		if (testd.y != 1.75) fail();
		if (testd.z != 2.25) fail();
		if (testd.w != 2.75) fail();

		CL_Vec4i testi(2, 10, 20, 5);
		int valuei = 2;
		testi /= valuei;
		if (testi.x != 1) fail();
		if (testi.y != 5) fail();
		if (testi.z != 10) fail();
		if (testi.w != 2) fail();

	}

	CL_Console::write_line("   Function: operator / (Type value)");
	{
		CL_Vec4d testd(2.5, 3.5, 4.5, 5.5);
		double valued = 2.0;
		testd = testd / valued;

		if (testd.x != 1.25) fail();
		if (testd.y != 1.75) fail();
		if (testd.z != 2.25) fail();
		if (testd.w != 2.75) fail();

		CL_Vec4i testi(2, 10, 20, 5);
		int valuei = 2;
		testi = testi / valuei;
		if (testi.x != 1) fail();
		if (testi.y != 5) fail();
		if (testi.z != 10) fail();
		if (testi.w != 2) fail();

	}

	CL_Console::write_line("   Function: operator / (const CL_Vec4<Type>& vector)");
	{
		CL_Vec4d testd(2.5, 3.5, 4.5, 10.0);
		testd = testd / CL_Vec4d(1.0, 2.5, 4.5, 2.0);

		if (testd.x != 2.5) fail();
		if (testd.y != 1.4) fail();
		if (testd.z != 1.0) fail();
		if (testd.w != 5.0) fail();

		CL_Vec4i testi(2, 10, 20, 5);
		testi = testi / CL_Vec4i(1, 2, 3, 4);
		if (testi.x != 2) fail();
		if (testi.y != 5) fail();
		if (testi.z != 6) fail();
		if (testi.w != 1) fail();

	}

	CL_Console::write_line("   Function: operator = (const CL_Vec4<Type>& vector)");
	{
		CL_Vec4d testd(2.5, 3.5, 4.5, 5.5);
		testd = CL_Vec4d(1.0, 2.0, 3.0, 4.0);

		if (testd.x != 1.0) fail();
		if (testd.y != 2.0) fail();
		if (testd.z != 3.0) fail();
		if (testd.w != 4.0) fail();

		CL_Vec4i testi(2, 3, 4, 5);
		testi = CL_Vec4i(1, 2, 3, 4);
		if (testi.x != 1) fail();
		if (testi.y != 2) fail();
		if (testi.z != 3) fail();
		if (testi.w != 4) fail();

	}

	CL_Console::write_line("   Function: operator == (const CL_Vec4<Type>& vector)");
	{
		CL_Vec4d testd(2.5, 3.5, 4.5, 5.5);
		if (testd == CL_Vec4d(1.0, 2.0, 3.0, 4.0)) fail();
		if (testd == CL_Vec4d(2.5, 2.0, 3.0, 4.0)) fail();
		if (testd == CL_Vec4d(2.5, 3.5, 3.0, 4.0)) fail();
		if (testd == CL_Vec4d(2.5, 3.5, 4.5, 4.0)) fail();
		if (!(testd == CL_Vec4d(2.5, 3.5, 4.5, 5.5))) fail();

		CL_Vec4i testi(2, 3, 4, 5);
		if (testi == CL_Vec4i(1, 2, 3, 4)) fail();
		if (testi == CL_Vec4i(2, 2, 3, 4)) fail();
		if (testi == CL_Vec4i(2, 3, 3, 4)) fail();
		if (testi == CL_Vec4i(2, 3, 4, 4)) fail();
		if (!(testi == CL_Vec4i(2, 3, 4, 5))) fail();
	}

	CL_Console::write_line("   Function: operator != (const CL_Vec4<Type>& vector)");
	{
		CL_Vec4d testd(2.5, 3.5, 4.5, 5.5);
		if (!(testd != CL_Vec4d(1.0, 2.0, 3.0, 4.0))) fail();
		if (!(testd != CL_Vec4d(2.5, 2.0, 3.0, 4.0))) fail();
		if (!(testd != CL_Vec4d(2.5, 3.5, 3.0, 4.0))) fail();
		if (!(testd != CL_Vec4d(2.5, 3.5, 4.5, 4.0))) fail();
		if ((testd != CL_Vec4d(2.5, 3.5, 4.5, 5.5))) fail();

		CL_Vec4i testi(2, 3, 4, 5);
		if (!(testi != CL_Vec4i(1, 2, 3, 4))) fail();
		if (!(testi != CL_Vec4i(2, 2, 3, 4))) fail();
		if (!(testi != CL_Vec4i(2, 3, 3, 4))) fail();
		if (!(testi != CL_Vec4i(2, 3, 4, 4))) fail();
		if ((testi != CL_Vec4i(2, 3, 4, 5))) fail();
	}

	CL_Console::write_line("   Function: round()");
	{
		CL_Vec4d testd(2.0, 2.5, -2.0, -2.5);
		testd.round();

		if (testd.x != 2.0) fail();
		if (testd.y != 3.0) fail();
		if (testd.z != -2.0) fail();
		if (testd.w != -2.0) fail();

		CL_Vec4f testf(2.0f, 2.5f, -2.0f, -2.9f);
		testf.round();

		if (testf.x != 2.0f) fail();
		if (testf.y != 3.0f) fail();
		if (testf.z != -2.0f) fail();
		if (testf.w != -3.0f) fail();
	}

	CL_Console::write_line("   Function: static round()");
	{
		CL_Vec4d testd(2.0, 2.5, -2.0, -2.5);
		CL_Vec4d destd = CL_Vec4d::round(testd);

		if (destd.x != 2.0) fail();
		if (destd.y != 3.0) fail();
		if (destd.z != -2.0) fail();
		if (destd.w != -2.0) fail();

		CL_Vec4f testf(2.0f, 2.5f, -2.0f, -2.9f);
		CL_Vec4f destf = CL_Vec4f::round(testf);

		if (destf.x != 2.0f) fail();
		if (destf.y != 3.0f) fail();
		if (destf.z != -2.0f) fail();
		if (destf.w != -3.0f) fail();
	}
}

void TestApp::test_vector3(void)
{
	CL_Console::write_line(" Header: cl_vector.h");
	CL_Console::write_line("  Class: CL_Vec3");

	CL_Console::write_line("   Function: distance()");
	{
		CL_Vec3d test_a(2.0,3.0,4.0);
		CL_Vec3d test_b(3.0,4.0,5.0);

		if (test_a.distance(test_b) != sqrt(1.0 + 1.0 + 1.0 ))  fail();
	}

	CL_Console::write_line("   Function: normalize()");
	{
		CL_Vec3d testi(3.0,4.0,5.0);
		testi.normalize();
		if (testi !=  CL_Vec4d(3.0/sqrt(50.0), 4.0/sqrt(50.0), 5.0/sqrt(50.0)))  fail();
	}

	CL_Console::write_line("   Function: static normalize()");
	{
		CL_Vec3d testi(3.0,4.0,5.0);
		if (CL_Vec3d::normalize(testi) !=  CL_Vec4d(3.0/sqrt(50.0), 4.0/sqrt(50.0), 5.0/sqrt(50.0)))  fail();
	}

	CL_Console::write_line("   Function: length()");
	{
		CL_Vec3d testi(3.0,4.0,5.0);
		if (testi.length() != sqrt(50.0 ))  fail();
	}

	CL_Console::write_line("   Function: dot()");
	{
		CL_Vec3d test_a(3.0,4.0,5.0);
		CL_Vec3d test_b(13.0,14.0,15.0);
		if (test_a.dot(test_b) != ((3.0 * 13.0)+ (4.0*14.0) + (5.0 * 15.0)))  fail();
	}

	CL_Console::write_line("   Function: operator += (const CL_Vec3<Type>& vector)");
	{
		CL_Vec3d testd(2.5, 3.5, 4.5);
		testd += CL_Vec3d(1.0, 2.0, 3.0);

		if (testd.x != 3.5) fail();
		if (testd.y != 5.5) fail();
		if (testd.z != 7.5) fail();

		CL_Vec3i testi(2, 3, 4);
		testi += CL_Vec3i(1, 2, 3);
		if (testi.x != 3) fail();
		if (testi.y != 5) fail();
		if (testi.z != 7) fail();

	}

	CL_Console::write_line("   Function: operator += ( Type value)");
	{
		CL_Vec3d testd(2.5, 3.5, 4.5);
		double valued = 2.0;
		testd += valued;

		if (testd.x != 4.5) fail();
		if (testd.y != 5.5) fail();
		if (testd.z != 6.5) fail();

		CL_Vec3i testi(2, 3, 4);
		int valuei = 2;
		testi += valuei;
		if (testi.x != 4) fail();
		if (testi.y != 5) fail();
		if (testi.z != 6) fail();

	}

	CL_Console::write_line("   Function: operator + (Type value)");
	{
		CL_Vec3d testd(2.5, 3.5, 4.5);
		double valued = 2.0;
		testd = testd + valued;

		if (testd.x != 4.5) fail();
		if (testd.y != 5.5) fail();
		if (testd.z != 6.5) fail();

		CL_Vec3i testi(2, 3, 4);
		int valuei = 2;
		testi = testi + valuei;
		if (testi.x != 4) fail();
		if (testi.y != 5) fail();
		if (testi.z != 6) fail();

	}

	CL_Console::write_line("   Function: operator + (const CL_Vec3<Type>& vector)");
	{
		CL_Vec3d testd(2.5, 3.5, 4.5);
		testd = testd + CL_Vec3d(1.5, 2.5, 3.5);

		if (testd.x != 4.0) fail();
		if (testd.y != 6.0) fail();
		if (testd.z != 8.0) fail();

		CL_Vec3i testi(2, 3, 4);
		testi = testi + CL_Vec3i(1, 2, 3);
		if (testi.x != 3) fail();
		if (testi.y != 5) fail();
		if (testi.z != 7) fail();

	}

	CL_Console::write_line("   Function: operator -= (const CL_Vec3<Type>& vector)");
	{
		CL_Vec3d testd(2.5, 3.5, 4.5);
		testd -= CL_Vec3d(1.0, 2.0, 3.0);

		if (testd.x != 1.5) fail();
		if (testd.y != 1.5) fail();
		if (testd.z != 1.5) fail();

		CL_Vec3i testi(2, 3, 4);
		testi -= CL_Vec3i(1, 2, 3);
		if (testi.x != 1) fail();
		if (testi.y != 1) fail();
		if (testi.z != 1) fail();

	}

	CL_Console::write_line("   Function: operator -= ( Type value)");
	{
		CL_Vec3d testd(2.5, 3.5, 4.5);
		double valued = 2.0;
		testd -= valued;

		if (testd.x != 0.5) fail();
		if (testd.y != 1.5) fail();
		if (testd.z != 2.5) fail();

		CL_Vec3i testi(2, 3, 4);
		int valuei = 2;
		testi -= valuei;
		if (testi.x != 0) fail();
		if (testi.y != 1) fail();
		if (testi.z != 2) fail();

	}

	CL_Console::write_line("   Function: operator - (Type value)");
	{
		CL_Vec3d testd(2.5, 3.5, 4.5);
		double valued = 2.0;
		testd = testd - valued;

		if (testd.x != 0.5) fail();
		if (testd.y != 1.5) fail();
		if (testd.z != 2.5) fail();

		CL_Vec3i testi(2, 3, 4);
		int valuei = 2;
		testi = testi - valuei;
		if (testi.x != 0) fail();
		if (testi.y != 1) fail();
		if (testi.z != 2) fail();

	}

	CL_Console::write_line("   Function: operator - (const CL_Vec3<Type>& vector)");
	{
		CL_Vec3d testd(2.5, 3.5, 4.5);
		testd = testd - CL_Vec3d(1.5, 2.5, 3.5);

		if (testd.x != 1.0) fail();
		if (testd.y != 1.0) fail();
		if (testd.z != 1.0) fail();

		CL_Vec3i testi(2, 3, 4);
		testi = testi - CL_Vec3i(1, 2, 3);
		if (testi.x != 1) fail();
		if (testi.y != 1) fail();
		if (testi.z != 1) fail();

	}

	CL_Console::write_line("   Function: operator *= (const CL_Vec3<Type>& vector)");
	{
		CL_Vec3d testd(2.5, 3.5, 4.5);
		testd *= CL_Vec3d(1.0, 2.0, 3.0);

		if (testd.x != 2.5) fail();
		if (testd.y != 7.0) fail();
		if (testd.z != 13.5) fail();

		CL_Vec3i testi(2, 3, 4);
		testi *= CL_Vec3i(1, 2, 3);
		if (testi.x != 2) fail();
		if (testi.y != 6) fail();
		if (testi.z != 12) fail();

	}

	CL_Console::write_line("   Function: operator *= ( Type value)");
	{
		CL_Vec3d testd(2.5, 3.5, 4.5);
		double valued = 2.0;
		testd *= valued;

		if (testd.x != 5.0) fail();
		if (testd.y != 7.0) fail();
		if (testd.z != 9.0) fail();

		CL_Vec3i testi(2, 3, 4);
		int valuei = 2;
		testi *= valuei;
		if (testi.x != 4) fail();
		if (testi.y != 6) fail();
		if (testi.z != 8) fail();

	}

	CL_Console::write_line("   Function: operator * (Type value)");
	{
		CL_Vec3d testd(2.5, 3.5, 4.5);
		double valued = 2.0;
		testd = testd * valued;

		if (testd.x != 5.0) fail();
		if (testd.y != 7.0) fail();
		if (testd.z != 9.0) fail();

		CL_Vec3i testi(2, 3, 4);
		int valuei = 2;
		testi = testi * valuei;
		if (testi.x != 4) fail();
		if (testi.y != 6) fail();
		if (testi.z != 8) fail();

	}

	CL_Console::write_line("   Function: operator * (const CL_Vec3<Type>& vector)");
	{
		CL_Vec3d testd(2.5, 3.5, 4.5);
		testd = testd * CL_Vec3d(1.5, 2.5, 3.5);

		if (testd.x != 3.75) fail();
		if (testd.y != 8.75) fail();
		if (testd.z != 15.75) fail();

		CL_Vec3i testi(2, 3, 4);
		testi = testi * CL_Vec3i(1, 2, 3);
		if (testi.x != 2) fail();
		if (testi.y != 6) fail();
		if (testi.z != 12) fail();

	}

	CL_Console::write_line("   Function: operator /= (const CL_Vec3<Type>& vector)");
	{
		CL_Vec3d testd(2.5, 3.5, 4.5);
		testd /= CL_Vec3d(1.0, 2.0, 3.0);

		if (testd.x != 2.5) fail();
		if (testd.y != 1.75) fail();
		if (testd.z != 1.5) fail();

		CL_Vec3i testi(2, 10, 20);
		testi /= CL_Vec3i(1, 2, 3);
		if (testi.x != 2) fail();
		if (testi.y != 5) fail();
		if (testi.z != 6) fail();

	}

	CL_Console::write_line("   Function: operator /= ( Type value)");
	{
		CL_Vec3d testd(2.5, 3.5, 4.5);
		double valued = 2.0;
		testd /= valued;

		if (testd.x != 1.25) fail();
		if (testd.y != 1.75) fail();
		if (testd.z != 2.25) fail();

		CL_Vec3i testi(2, 10, 20);
		int valuei = 2;
		testi /= valuei;
		if (testi.x != 1) fail();
		if (testi.y != 5) fail();
		if (testi.z != 10) fail();

	}

	CL_Console::write_line("   Function: operator / (Type value)");
	{
		CL_Vec3d testd(2.5, 3.5, 4.5);
		double valued = 2.0;
		testd = testd / valued;

		if (testd.x != 1.25) fail();
		if (testd.y != 1.75) fail();
		if (testd.z != 2.25) fail();

		CL_Vec3i testi(2, 10, 20);
		int valuei = 2;
		testi = testi / valuei;
		if (testi.x != 1) fail();
		if (testi.y != 5) fail();
		if (testi.z != 10) fail();

	}

	CL_Console::write_line("   Function: operator / (const CL_Vec3<Type>& vector)");
	{
		CL_Vec3d testd(2.5, 3.5, 4.5);
		testd = testd / CL_Vec3d(1.0, 2.5, 4.5);

		if (testd.x != 2.5) fail();
		if (testd.y != 1.4) fail();
		if (testd.z != 1.0) fail();

		CL_Vec3i testi(2, 10, 20);
		testi = testi / CL_Vec3i(1, 2, 3);
		if (testi.x != 2) fail();
		if (testi.y != 5) fail();
		if (testi.z != 6) fail();

	}

	CL_Console::write_line("   Function: operator = (const CL_Vec3<Type>& vector)");
	{
		CL_Vec3d testd(2.5, 3.5, 4.5);
		testd = CL_Vec3d(1.0, 2.0, 3.0);

		if (testd.x != 1.0) fail();
		if (testd.y != 2.0) fail();
		if (testd.z != 3.0) fail();

		CL_Vec3i testi(2, 3, 4);
		testi = CL_Vec3i(1, 2, 3);
		if (testi.x != 1) fail();
		if (testi.y != 2) fail();
		if (testi.z != 3) fail();

	}

	CL_Console::write_line("   Function: operator == (const CL_Vec3<Type>& vector)");
	{
		CL_Vec3d testd(2.5, 3.5, 4.5);
		if (testd == CL_Vec3d(1.0, 2.0, 3.0)) fail();
		if (testd == CL_Vec3d(2.5, 2.0, 3.0)) fail();
		if (testd == CL_Vec3d(2.5, 3.5, 3.0)) fail();
		if (!(testd == CL_Vec3d(2.5, 3.5, 4.5))) fail();

		CL_Vec3i testi(2, 3, 4);
		if (testi == CL_Vec3i(1, 2, 3)) fail();
		if (testi == CL_Vec3i(2, 2, 3)) fail();
		if (testi == CL_Vec3i(2, 3, 3)) fail();
		if (!(testi == CL_Vec3i(2, 3, 4))) fail();
	}

	CL_Console::write_line("   Function: operator != (const CL_Vec3<Type>& vector)");
	{
		CL_Vec3d testd(2.5, 3.5, 4.5);
		if (!(testd != CL_Vec3d(1.0, 2.0, 3.0))) fail();
		if (!(testd != CL_Vec3d(2.5, 2.0, 3.0))) fail();
		if (!(testd != CL_Vec3d(2.5, 3.5, 3.0))) fail();
		if ((testd != CL_Vec3d(2.5, 3.5, 4.5))) fail();

		CL_Vec3i testi(2, 3, 4);
		if (!(testi != CL_Vec3i(1, 2, 3))) fail();
		if (!(testi != CL_Vec3i(2, 2, 3))) fail();
		if (!(testi != CL_Vec3i(2, 3, 3))) fail();
		if ((testi != CL_Vec3i(2, 3, 4))) fail();
	}

	CL_Console::write_line("   Function: round()");
	{
		CL_Vec3d testd(2.0, 2.5, -2.0);
		testd.round();

		if (testd.x != 2.0) fail();
		if (testd.y != 3.0) fail();
		if (testd.z != -2.0) fail();

		CL_Vec3f testf(2.0f, 2.5f, -2.0f);
		testf.round();

		if (testf.x != 2.0f) fail();
		if (testf.y != 3.0f) fail();
		if (testf.z != -2.0f) fail();
	}

	CL_Console::write_line("   Function: static round()");
	{
		CL_Vec3d testd(2.0, 2.5, -2.0);
		CL_Vec3d destd = CL_Vec3d::round(testd);

		if (destd.x != 2.0) fail();
		if (destd.y != 3.0) fail();
		if (destd.z != -2.0) fail();

		CL_Vec3f testf(2.0f, 2.5f, -2.0f);
		CL_Vec3f destf = CL_Vec3f::round(testf);

		if (destf.x != 2.0f) fail();
		if (destf.y != 3.0f) fail();
		if (destf.z != -2.0f) fail();
	}

	CL_Console::write_line("   Function: CL_Vec3<Type> operator * (const CL_Mat3<Type>& matrix, const CL_Vec3<Type>& v)");
	{
		/// Matrix is assumed to be in the Column-Major matrix format (opengl native)\n
		CL_Mat3f matrix(1.0f, 2.0f, 3.0f, 
						4.0f, 5.0f, 6.0f,
						7.0f, 8.0f, 9.0f);

		CL_Vec3f vector(1.0f, 2.0f, 3.0f);
		CL_Vec3f result = matrix * vector;

		if (result.x != ( (matrix[0 + 3*0] * vector.x) + (matrix[0 + 3*1] * vector.y) + (matrix[0 + 3*2] * vector.z) ) )
			fail();
		if (result.y != ( (matrix[1 + 3*0] * vector.x) + (matrix[1 + 3*1] * vector.y) + (matrix[1 + 3*2] * vector.z) ) )
			fail();
		if (result.z != ( (matrix[2 + 3*0] * vector.x) + (matrix[2 + 3*1] * vector.y) + (matrix[2 + 3*2] * vector.z) ) )
			fail();
	
	}
}


void TestApp::test_vector2(void)
{
	CL_Console::write_line(" Header: cl_vector.h");
	CL_Console::write_line("  Class: CL_Vec2");

	CL_Console::write_line("   Function: rotate()");
	{
		CL_Vec2i test_a;
		CL_Vec2i hotspot(1,3);

		test_a = CL_Vec2i(4,5);
		test_a.rotate(hotspot, CL_Angle(0, cl_degrees));
		if (test_a != CL_Vec2i(4, 5))  fail();

		test_a = CL_Vec2i(4,5);
		test_a.rotate(hotspot, CL_Angle(90, cl_degrees));
		if (test_a != CL_Vec2i(-1, 6))  fail();

		test_a = CL_Vec2i(4,5);
		test_a.rotate(hotspot, CL_Angle(180, cl_degrees));
		if (test_a != CL_Vec2i(-2, 1))  fail();

		test_a = CL_Vec2i(4,5);
		test_a.rotate(hotspot, CL_Angle(270, cl_degrees));
		if (test_a != CL_Vec2i(3, 0))  fail();

		test_a = CL_Vec2i(4,5);
		test_a.rotate(hotspot, CL_Angle(360, cl_degrees));
		if (test_a != CL_Vec2i(4, 5))  fail();

		test_a = CL_Vec2i(4,5);
		test_a.rotate(CL_Vec2i(0,0), CL_Angle(180, cl_degrees));
		if (test_a != CL_Vec2i(-4, -5))  fail();

	}

	CL_Console::write_line("   Function: distance()");
	{
		CL_Vec2d test_a(2.0,3.0);
		CL_Vec2d test_b(3.0,4.0);

		if (test_a.distance(test_b) != sqrt(1.0 + 1.0 ))  fail();
	}
	CL_Console::write_line("   Function: normalize()");
	{
		CL_Vec2d testi(3.0,4.0);
		testi.normalize();
		if (testi !=  CL_Vec2d(3.0/sqrt(25.0), 4.0/sqrt(25.0)))  fail();
	}

	CL_Console::write_line("   Function: static normalize()");
	{
		CL_Vec2d testi(3.0,4.0);
		if (CL_Vec2d::normalize(testi) !=  CL_Vec2d(3.0/sqrt(25.0), 4.0/sqrt(25.0)))  fail();
	}

	CL_Console::write_line("   Function: length()");
	{
		CL_Vec2d testi(3.0,4.0);
		if (testi.length() != sqrt(25.0 ))  fail();
	}

	CL_Console::write_line("   Function: dot()");
	{
		CL_Vec2d test_a(3.0,4.0);
		CL_Vec2d test_b(13.0,14.0);
		if (test_a.dot(test_b) != ((3.0 * 13.0)+ (4.0*14.0)))  fail();
	}

	CL_Console::write_line("   Function: operator += (const CL_Vec2<Type>& vector)");
	{
		CL_Vec2d testd(2.5, 3.5);
		testd += CL_Vec2d(1.0, 2.0);

		if (testd.x != 3.5) fail();
		if (testd.y != 5.5) fail();

		CL_Vec2i testi(2, 3);
		testi += CL_Vec2i(1, 2);
		if (testi.x != 3) fail();
		if (testi.y != 5) fail();

	}

	CL_Console::write_line("   Function: operator += ( Type value)");
	{
		CL_Vec2d testd(2.5, 3.5);
		double valued = 2.0;
		testd += valued;

		if (testd.x != 4.5) fail();
		if (testd.y != 5.5) fail();

		CL_Vec2i testi(2, 3);
		int valuei = 2;
		testi += valuei;
		if (testi.x != 4) fail();
		if (testi.y != 5) fail();

	}

	CL_Console::write_line("   Function: operator + (Type value)");
	{
		CL_Vec2d testd(2.5, 3.5);
		double valued = 2.0;
		testd = testd + valued;

		if (testd.x != 4.5) fail();
		if (testd.y != 5.5) fail();

		CL_Vec2i testi(2, 3);
		int valuei = 2;
		testi = testi + valuei;
		if (testi.x != 4) fail();
		if (testi.y != 5) fail();

	}

	CL_Console::write_line("   Function: operator + (const CL_Vec2<Type>& vector)");
	{
		CL_Vec2d testd(2.5, 3.5);
		testd = testd + CL_Vec2d(1.5, 2.5);

		if (testd.x != 4.0) fail();
		if (testd.y != 6.0) fail();

		CL_Vec2i testi(2, 3);
		testi = testi + CL_Vec2i(1, 2);
		if (testi.x != 3) fail();
		if (testi.y != 5) fail();

	}

	CL_Console::write_line("   Function: operator -= (const CL_Vec2<Type>& vector)");
	{
		CL_Vec2d testd(2.5, 3.5);
		testd -= CL_Vec2d(1.0, 2.0);

		if (testd.x != 1.5) fail();
		if (testd.y != 1.5) fail();

		CL_Vec2i testi(2, 3);
		testi -= CL_Vec2i(1, 2);
		if (testi.x != 1) fail();
		if (testi.y != 1) fail();

	}

	CL_Console::write_line("   Function: operator -= ( Type value)");
	{
		CL_Vec2d testd(2.5, 3.5);
		double valued = 2.0;
		testd -= valued;

		if (testd.x != 0.5) fail();
		if (testd.y != 1.5) fail();

		CL_Vec2i testi(2, 3);
		int valuei = 2;
		testi -= valuei;
		if (testi.x != 0) fail();
		if (testi.y != 1) fail();

	}

	CL_Console::write_line("   Function: operator - (Type value)");
	{
		CL_Vec2d testd(2.5, 3.5);
		double valued = 2.0;
		testd = testd - valued;

		if (testd.x != 0.5) fail();
		if (testd.y != 1.5) fail();

		CL_Vec2i testi(2, 3);
		int valuei = 2;
		testi = testi - valuei;
		if (testi.x != 0) fail();
		if (testi.y != 1) fail();

	}

	CL_Console::write_line("   Function: operator - (const CL_Vec2<Type>& vector)");
	{
		CL_Vec2d testd(2.5, 3.5);
		testd = testd - CL_Vec2d(1.5, 2.5);

		if (testd.x != 1.0) fail();
		if (testd.y != 1.0) fail();

		CL_Vec2i testi(2, 3);
		testi = testi - CL_Vec2i(1, 2);
		if (testi.x != 1) fail();
		if (testi.y != 1) fail();

	}

	CL_Console::write_line("   Function: operator *= (const CL_Vec2<Type>& vector)");
	{
		CL_Vec2d testd(2.5, 3.5);
		testd *= CL_Vec2d(1.0, 2.0);

		if (testd.x != 2.5) fail();
		if (testd.y != 7.0) fail();

		CL_Vec2i testi(2, 3);
		testi *= CL_Vec2i(1, 2);
		if (testi.x != 2) fail();
		if (testi.y != 6) fail();

	}

	CL_Console::write_line("   Function: operator *= ( Type value)");
	{
		CL_Vec2d testd(2.5, 3.5);
		double valued = 2.0;
		testd *= valued;

		if (testd.x != 5.0) fail();
		if (testd.y != 7.0) fail();

		CL_Vec2i testi(2, 3);
		int valuei = 2;
		testi *= valuei;
		if (testi.x != 4) fail();
		if (testi.y != 6) fail();
	}

	CL_Console::write_line("   Function: operator * (Type value)");
	{
		CL_Vec2d testd(2.5, 3.5);
		double valued = 2.0;
		testd = testd * valued;

		if (testd.x != 5.0) fail();
		if (testd.y != 7.0) fail();

		CL_Vec2i testi(2, 3);
		int valuei = 2;
		testi = testi * valuei;
		if (testi.x != 4) fail();
		if (testi.y != 6) fail();

	}

	CL_Console::write_line("   Function: operator * (const CL_Vec2<Type>& vector)");
	{
		CL_Vec2d testd(2.5, 3.5);
		testd = testd * CL_Vec2d(1.5, 2.5);

		if (testd.x != 3.75) fail();
		if (testd.y != 8.75) fail();

		CL_Vec2i testi(2, 3);
		testi = testi * CL_Vec2i(1, 2);
		if (testi.x != 2) fail();
		if (testi.y != 6) fail();

	}

	CL_Console::write_line("   Function: operator /= (const CL_Vec2<Type>& vector)");
	{
		CL_Vec2d testd(2.5, 3.5);
		testd /= CL_Vec2d(1.0, 2.0);

		if (testd.x != 2.5) fail();
		if (testd.y != 1.75) fail();

		CL_Vec2i testi(2, 10);
		testi /= CL_Vec2i(1, 2);
		if (testi.x != 2) fail();
		if (testi.y != 5) fail();

	}

	CL_Console::write_line("   Function: operator /= ( Type value)");
	{
		CL_Vec2d testd(2.5, 3.5);
		double valued = 2.0;
		testd /= valued;

		if (testd.x != 1.25) fail();
		if (testd.y != 1.75) fail();

		CL_Vec2i testi(2, 10);
		int valuei = 2;
		testi /= valuei;
		if (testi.x != 1) fail();
		if (testi.y != 5) fail();
	}

	CL_Console::write_line("   Function: operator / (Type value)");
	{
		CL_Vec2d testd(2.5, 3.5);
		double valued = 2.0;
		testd = testd / valued;

		if (testd.x != 1.25) fail();
		if (testd.y != 1.75) fail();

		CL_Vec2i testi(2, 10);
		int valuei = 2;
		testi = testi / valuei;
		if (testi.x != 1) fail();
		if (testi.y != 5) fail();

	}

	CL_Console::write_line("   Function: operator / (const CL_Vec2<Type>& vector)");
	{
		CL_Vec2d testd(2.5, 3.5);
		testd = testd / CL_Vec2d(1.0, 2.5);

		if (testd.x != 2.5) fail();
		if (testd.y != 1.4) fail();

		CL_Vec2i testi(2, 10);
		testi = testi / CL_Vec2i(1, 2);
		if (testi.x != 2) fail();
		if (testi.y != 5) fail();

	}

	CL_Console::write_line("   Function: operator = (const CL_Vec2<Type>& vector)");
	{
		CL_Vec2d testd(2.5, 3.5);
		testd = CL_Vec2d(1.0, 2.0);

		if (testd.x != 1.0) fail();
		if (testd.y != 2.0) fail();

		CL_Vec2i testi(2, 3);
		testi = CL_Vec2i(1, 2);
		if (testi.x != 1) fail();
		if (testi.y != 2) fail();

	}

	CL_Console::write_line("   Function: operator == (const CL_Vec2<Type>& vector)");
	{
		CL_Vec2d testd(2.5, 3.5);
		if (testd == CL_Vec2d(1.0, 2.0)) fail();
		if (testd == CL_Vec2d(2.5, 2.0)) fail();
		if (!(testd == CL_Vec2d(2.5, 3.5))) fail();

		CL_Vec2i testi(2, 3);
		if (testi == CL_Vec2i(1, 2)) fail();
		if (testi == CL_Vec2i(2, 2)) fail();
		if (!(testi == CL_Vec2i(2, 3))) fail();
	}

	CL_Console::write_line("   Function: operator != (const CL_Vec2<Type>& vector)");
	{
		CL_Vec2d testd(2.5, 3.5);
		if (!(testd != CL_Vec2d(1.0, 2.0))) fail();
		if (!(testd != CL_Vec2d(2.5, 2.0))) fail();
		if ((testd != CL_Vec2d(2.5, 3.5))) fail();

		CL_Vec2i testi(2, 3);
		if (!(testi != CL_Vec2i(1, 2))) fail();
		if (!(testi != CL_Vec2i(2, 2))) fail();
		if ((testi != CL_Vec2i(2, 3))) fail();
	}

	CL_Console::write_line("   Function: round()");
	{
		CL_Vec2d testd(2.0, 2.5);
		testd.round();

		if (testd.x != 2.0) fail();
		if (testd.y != 3.0) fail();

		CL_Vec2f testf(2.0f, 2.5f);
		testf.round();

		if (testf.x != 2.0f) fail();
		if (testf.y != 3.0f) fail();
	}

	CL_Console::write_line("   Function: static round()");
	{
		CL_Vec2d testd(2.0, 2.5);
		CL_Vec2d destd = CL_Vec2d::round(testd);

		if (destd.x != 2.0) fail();
		if (destd.y != 3.0) fail();

		CL_Vec2f testf(2.0f, 2.5f);
		CL_Vec2f destf = CL_Vec2f::round(testf);

		if (destf.x != 2.0f) fail();
		if (destf.y != 3.0f) fail();
	}
}


void TestApp::test_vector1(void)
{
	CL_Console::write_line(" Header: cl_vector.h");
	CL_Console::write_line("  Class: CL_Vec1");


	CL_Console::write_line("   Function: operator += (const CL_Vec1<Type>& vector)");
	{
		CL_Vec1d testd(2.5);
		testd += CL_Vec1d(1.0);

		if (testd.x != 3.5) fail();

		CL_Vec1i testi(2);
		testi += CL_Vec1i(1);
		if (testi.x != 3) fail();

	}

	CL_Console::write_line("   Function: operator += ( Type value)");
	{
		CL_Vec1d testd(2.5);
		double valued = 2.0;
		testd += valued;

		if (testd.x != 4.5) fail();

		CL_Vec1i testi(2);
		int valuei = 2;
		testi += valuei;
		if (testi.x != 4) fail();

	}

	CL_Console::write_line("   Function: operator + (Type value)");
	{
		CL_Vec1d testd(2.5);
		double valued = 2.0;
		testd = testd + valued;

		if (testd.x != 4.5) fail();

		CL_Vec1i testi(2);
		int valuei = 2;
		testi = testi + valuei;
		if (testi.x != 4) fail();

	}

	CL_Console::write_line("   Function: operator + (const CL_Vec1<Type>& vector)");
	{
		CL_Vec1d testd(2.5);
		testd = testd + CL_Vec1d(1.5);

		if (testd.x != 4.0) fail();

		CL_Vec1i testi(2);
		testi = testi + CL_Vec1i(1);
		if (testi.x != 3) fail();
	}

	CL_Console::write_line("   Function: operator -= (const CL_Vec1<Type>& vector)");
	{
		CL_Vec1d testd(2.5);
		testd -= CL_Vec1d(1.0);

		if (testd.x != 1.5) fail();

		CL_Vec1i testi(2);
		testi -= CL_Vec1i(1);
		if (testi.x != 1) fail();

	}

	CL_Console::write_line("   Function: operator -= ( Type value)");
	{
		CL_Vec1d testd(2.5);
		double valued = 2.0;
		testd -= valued;

		if (testd.x != 0.5) fail();

		CL_Vec1i testi(2);
		int valuei = 2;
		testi -= valuei;
		if (testi.x != 0) fail();

	}

	CL_Console::write_line("   Function: operator - (Type value)");
	{
		CL_Vec1d testd(2.5);
		double valued = 2.0;
		testd = testd - valued;

		if (testd.x != 0.5) fail();

		CL_Vec1i testi(2);
		int valuei = 2;
		testi = testi - valuei;
		if (testi.x != 0) fail();

	}

	CL_Console::write_line("   Function: operator - (const CL_Vec1<Type>& vector)");
	{
		CL_Vec1d testd(2.5);
		testd = testd - CL_Vec1d(1.5);

		if (testd.x != 1.0) fail();

		CL_Vec1i testi(2);
		testi = testi - CL_Vec1i(1);
		if (testi.x != 1) fail();

	}

	CL_Console::write_line("   Function: operator *= (const CL_Vec1<Type>& vector)");
	{
		CL_Vec1d testd(2.5);
		testd *= CL_Vec1d(1.0);

		if (testd.x != 2.5) fail();

		CL_Vec1i testi(2);
		testi *= CL_Vec1i(1);
		if (testi.x != 2) fail();

	}

	CL_Console::write_line("   Function: operator *= ( Type value)");
	{
		CL_Vec1d testd(2.5);
		double valued = 2.0;
		testd *= valued;

		if (testd.x != 5.0) fail();

		CL_Vec1i testi(2);
		int valuei = 2;
		testi *= valuei;
		if (testi.x != 4) fail();

	}

	CL_Console::write_line("   Function: operator * (Type value)");
	{
		CL_Vec1d testd(2.5);
		double valued = 2.0;
		testd = testd * valued;

		if (testd.x != 5.0) fail();

		CL_Vec1i testi(2);
		int valuei = 2;
		testi = testi * valuei;
		if (testi.x != 4) fail();

	}

	CL_Console::write_line("   Function: operator * (const CL_Vec1<Type>& vector)");
	{
		CL_Vec1d testd(2.5);
		testd = testd * CL_Vec1d(1.5);

		if (testd.x != 3.75) fail();

		CL_Vec1i testi(2);
		testi = testi * CL_Vec1i(1);
		if (testi.x != 2) fail();

	}

	CL_Console::write_line("   Function: operator /= (const CL_Vec1<Type>& vector)");
	{
		CL_Vec1d testd(2.5);
		testd /= CL_Vec1d(1.0);

		if (testd.x != 2.5) fail();

		CL_Vec1i testi(2);
		testi /= CL_Vec1i(1);
		if (testi.x != 2) fail();

	}

	CL_Console::write_line("   Function: operator /= ( Type value)");
	{
		CL_Vec1d testd(2.5);
		double valued = 2.0;
		testd /= valued;

		if (testd.x != 1.25) fail();

		CL_Vec1i testi(2);
		int valuei = 2;
		testi /= valuei;
		if (testi.x != 1) fail();

	}

	CL_Console::write_line("   Function: operator / (Type value)");
	{
		CL_Vec1d testd(2.5);
		double valued = 2.0;
		testd = testd / valued;

		if (testd.x != 1.25) fail();

		CL_Vec1i testi(2);
		int valuei = 2;
		testi = testi / valuei;
		if (testi.x != 1) fail();

	}

	CL_Console::write_line("   Function: operator / (const CL_Vec1<Type>& vector)");
	{
		CL_Vec1d testd(2.5);
		testd = testd / CL_Vec1d(1.0);

		if (testd.x != 2.5) fail();

		CL_Vec1i testi(2);
		testi = testi / CL_Vec1i(1);
		if (testi.x != 2) fail();

	}

	CL_Console::write_line("   Function: operator = (const CL_Vec1<Type>& vector)");
	{
		CL_Vec1d testd(2.5);
		testd = CL_Vec1d(1.0);

		if (testd.x != 1.0) fail();

		CL_Vec1i testi(2);
		testi = CL_Vec1i(1);
		if (testi.x != 1) fail();

	}

	CL_Console::write_line("   Function: operator == (const CL_Vec1<Type>& vector)");
	{
		CL_Vec1d testd(2.5);
		if (testd == CL_Vec1d(1.0)) fail();
		if (!(testd == CL_Vec1d(2.5))) fail();

		CL_Vec1i testi(2);
		if (testi == CL_Vec1i(1)) fail();
		if (!(testi == CL_Vec1i(2))) fail();
	}

	CL_Console::write_line("   Function: operator != (const CL_Vec1<Type>& vector)");
	{
		CL_Vec1d testd(2.5);
		if (!(testd != CL_Vec1d(1.0))) fail();
		if ((testd != CL_Vec1d(2.5))) fail();

		CL_Vec1i testi(2);
		if (!(testi != CL_Vec1i(1))) fail();
		if ((testi != CL_Vec1i(2))) fail();
	}

	CL_Console::write_line("   Function: round()");
	{
		CL_Vec1d testd(2.0);
		testd.round();

		if (testd.x != 2.0) fail();

		CL_Vec1f testf(2.0f);
		testf.round();

		if (testf.x != 2.0f) fail();
	}

	CL_Console::write_line("   Function: static round()");
	{
		CL_Vec1d testd(2.0);
		CL_Vec1d dest = CL_Vec1d::round(testd);

		if (dest.x != 2.0) fail();

		CL_Vec1f testf(2.0f);
		CL_Vec1f dest2 = CL_Vec1f::round(testf);

		if (dest2.x != 2.0f) fail();
	}
}



