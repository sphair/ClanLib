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

void TestApp::test_matrix(void)
{
	CL_Console::write_line(" Header: matrix4x4.h");
	test_matrix_mat2();
	test_matrix_mat3();
	test_matrix_mat4();
}

void TestApp::test_matrix_mat3()
{
	CL_Console::write_line("  Class: CL_Mat3");

	CL_Console::write_line("   Function: inverse()");
	{
		CL_Mat3d test_src(2, 3, 4, 2, -5, 2, -3, 6, -3);
		CL_Mat3d test_inv;
		CL_Mat3d test_dest;
		CL_Mat3d test_ident = CL_Mat3d::identity();

		test_dest = test_src;
		test_dest.inverse();
		test_dest.multiply(test_src);

		if (test_ident != test_dest) fail();

		CL_Mat4d test_4d(test_src);
		CL_Mat3d test_3d(test_4d);
		if (test_3d != test_src) fail();

		test_4d =test_src;
		test_3d =test_4d;
		if (test_3d != test_src) fail();
	}

	CL_Mat3i test_a(3, 1, 2, 4, 5 ,6, 4, 2, 1);
	CL_Mat3i test_b(4, 7, 2, 5, 3, 5, 2, 9, 3);

	CL_Console::write_line("   Function: multiply() and operator");
	{
		CL_Mat3i result = test_a * test_b;
		CL_Mat3i answer(21, 42, 17, 53, 97, 51, 28, 43, 21);
		if (result != answer) fail();

		result = CL_Mat3i::multiply(test_a, test_b);
		if (result != answer) fail();

		result = test_b;
		result.multiply(test_a);
		if (result != answer) fail();

	}

	CL_Console::write_line("   Function: add() and operator");
	{
		CL_Mat3i result = test_a + test_b;
		if (result != CL_Mat3i(7, 8, 4, 9, 8, 11, 6, 11, 4)) fail();

		result = CL_Mat3i::add(test_a, test_b);
		if (result != CL_Mat3i(7, 8, 4, 9, 8, 11, 6, 11, 4)) fail();

		result = test_b;
		result.add(test_a);
		if (result != CL_Mat3i(7, 8, 4, 9, 8, 11, 6, 11, 4)) fail();

	}

	CL_Console::write_line("   Function: subtract() and operator");
	{
		CL_Mat3i result = test_a - test_b;
		if (result != CL_Mat3i(-1, -6, 0, -1, 2, 1, 2, -7, -2)) fail();

		result = CL_Mat3i::subtract(test_a, test_b);
		if (result != CL_Mat3i(-1, -6, 0, -1, 2, 1, 2, -7, -2)) fail();

		result = test_b;
		result.subtract(test_a);
		if (result != CL_Mat3i(-1, -6, 0, -1, 2, 1, 2, -7, -2)) fail();

	}
}

void TestApp::test_matrix_mat4()
{
	CL_Console::write_line("  Class: CL_Mat4");

	CL_Console::write_line("   Function: inverse()");
	{

		CL_Mat4f test_src = CL_Mat4f::rotate((CL_Angle(30, cl_degrees)), 1.0, 0.0, 0.0, true);
		CL_Mat4f test_inv;
		CL_Mat4f test_dest;
		CL_Mat4f test_ident = CL_Mat4f::identity();

		test_dest = test_src;
		test_dest.inverse();
		test_dest.multiply(test_src);

		if (test_ident != test_dest) fail();

	}

	static int test_a_values[] = {3, 1, 2, 4, 5 ,6, 4, 2, 1, 4, 6, 7, 6, 3, 7, 2};
	static int test_b_values[] = {4, 7, 2, 5, 3, 5, 2, 9, 3, 3, 6, 9, 2, 4, 6, 2};

	CL_Mat4i test_a(test_a_values);
	CL_Mat4i test_b(test_b_values);

	CL_Console::write_line("   Function: add() and operator");
	{
		int answer_values[] = {7, 8, 4, 9, 8, 11, 6, 11, 4, 7, 12, 16, 8, 7, 13, 4};
		CL_Mat4i answer(answer_values);

		CL_Mat4i result = test_a + test_b;
		if (result != answer) fail();

		result = CL_Mat4i::add(test_a, test_b);
		if (result != answer) fail();

		result = test_b;
		result.add(test_a);
		if (result != answer) fail();

	}

	CL_Console::write_line("   Function: subtract() and operator");
	{
		int answer_values[] = {-1, -6, 0, -1, 2, 1, 2, -7, -2, 1, 0, -2, 4, -1, 1, 0};
		CL_Mat4i answer(answer_values);

		CL_Mat4i result = test_a - test_b;
		if (result != answer) fail();

		result = CL_Mat4i::subtract(test_a, test_b);
		if (result != answer) fail();

		result = test_b;
		result.subtract(test_a);
		if (result != answer) fail();

	}

	CL_Console::write_line("   Function: translate()");
	{
		int answer_values[] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 2, 3, 4, 1};
		CL_Mat4i answer(answer_values);

		CL_Mat4i result = CL_Mat4i::translate(2, 3, 4);
		if (result != answer) fail();
	}

	CL_Console::write_line("   Function: translate_self()");
	{
		CL_Mat4i answer(test_a);

		CL_Mat4i result = test_a;
		result.multiply(CL_Mat4i::translate(2, 3, 4));

		CL_Mat4i result2 = test_a;
		result2.translate_self(2,3,4);

		if (result != result2) fail();
	}

	CL_Console::write_line("   Function: scale_self()");
	{
		CL_Mat4i answer(test_a);

		CL_Mat4i result = test_a;
		result.multiply(CL_Mat4i::scale(2, 3, 4));

		CL_Mat4i result2 = test_a;
		result2.scale_self(2,3,4);

		if (result != result2) fail();
	}

	CL_Console::write_line("   Function: rotate (using euler angles) and get_euler");
	{
		CL_Angle angle_x(20, cl_degrees);
		CL_Angle angle_y(30, cl_degrees);
		CL_Angle angle_z(40, cl_degrees);

		CL_Mat4f test_matrix;
		test_matrix = CL_Mat4f::rotate(angle_x, angle_y, angle_z, cl_YXZ);

		CL_Vec3f angles = test_matrix.get_euler(cl_YXZ);

		check_float(angles.x, angle_x.to_radians());
		check_float(angles.y, angle_y.to_radians());
		check_float(angles.z, angle_z.to_radians());
	}
}

void TestApp::test_matrix_mat2()
{
	CL_Console::write_line("  Class: CL_Mat2");

	CL_Mat2d test_a = CL_Mat2d(3, 1, 2, 4);
	CL_Mat2d test_b = CL_Mat2d(-3, 7, 2, 5);

	CL_Console::write_line("   Function: multiply() and operator");
	{
		CL_Mat2d result = test_a * test_b;
		if (result != CL_Mat2d(-7, 26, 2, 34)) fail();

		result = CL_Mat2d::multiply(test_a, test_b);
		if (result != CL_Mat2d(-7, 26, 2, 34)) fail();

		result = test_b;
		result.multiply(test_a);
		if (result != CL_Mat2d(-7, 26, 2, 34)) fail();

	}

	CL_Console::write_line("   Function: add() and operator");
	{
		CL_Mat2d result = test_a + test_b;
		if (result != CL_Mat2d(0, 8, 4, 9)) fail();

		result = CL_Mat2d::add(test_a, test_b);
		if (result != CL_Mat2d(0, 8, 4, 9)) fail();

		result = test_b;
		result.add(test_a);
		if (result != CL_Mat2d(0, 8, 4, 9)) fail();

	}

	CL_Console::write_line("   Function: subtract() and operator");
	{
		CL_Mat2d result = test_a - test_b;
		if (result != CL_Mat2d(6, -6, 0, -1)) fail();

		result = CL_Mat2d::subtract(test_a, test_b);
		if (result != CL_Mat2d(6, -6, 0, -1)) fail();

		result = test_b;
		result.subtract(test_a);
		if (result != CL_Mat2d(6, -6, 0, -1)) fail();

	}

}

