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

void TestApp::test_matrix(void)
{
	Console::write_line(" Header: matrix4x4.h");
	test_matrix_mat2();
	test_matrix_mat3();
	test_matrix_mat4();
}

void TestApp::test_matrix_mat3()
{
	Console::write_line("  Class: Mat3");

	Console::write_line("   Function: inverse()");
	{
		Mat3d test_src(2, 3, 4, 2, -5, 2, -3, 6, -3);
		Mat3d test_inv;
		Mat3d test_dest;
		Mat3d test_ident = Mat3d::identity();

		test_dest = test_src;
		test_dest.inverse();
		test_dest = test_dest * test_src;

		if (test_ident != test_dest) fail();

		Mat4d test_4d(test_src);
		Mat3d test_3d(test_4d);
		if (test_3d != test_src) fail();

		test_4d =test_src;
		test_3d =test_4d;
		if (test_3d != test_src) fail();
	}

	Mat3i test_a(3, 1, 2, 4, 5 ,6, 4, 2, 1);
	Mat3i test_b(4, 7, 2, 5, 3, 5, 2, 9, 3);

	Console::write_line("   Function: multiply() and operator");
	{
		Mat3i result = test_b * test_a;
		Mat3i answer(21, 42, 17, 53, 97, 51, 28, 43, 21);
		if (result != answer) fail();

		result = Mat3i::multiply(test_b, test_a);
		if (result != answer) fail();

	}

	Console::write_line("   Function: add() and operator");
	{
		Mat3i result = test_a + test_b;
		if (result != Mat3i(7, 8, 4, 9, 8, 11, 6, 11, 4)) fail();

		result = Mat3i::add(test_a, test_b);
		if (result != Mat3i(7, 8, 4, 9, 8, 11, 6, 11, 4)) fail();

	}

	Console::write_line("   Function: subtract() and operator");
	{
		Mat3i result = test_a - test_b;
		if (result != Mat3i(-1, -6, 0, -1, 2, 1, 2, -7, -2)) fail();

		result = Mat3i::subtract(test_a, test_b);
		if (result != Mat3i(-1, -6, 0, -1, 2, 1, 2, -7, -2)) fail();
	}
}

void TestApp::test_matrix_mat4()
{
	Console::write_line("  Class: Mat4");

	Console::write_line("   Function: inverse()");
	{

		Mat4f test_src = Mat4f::rotate((Angle(30, angle_degrees)), 1.0, 0.0, 0.0, true);
		Mat4f test_inv;
		Mat4f test_dest;
		Mat4f test_ident = Mat4f::identity();

		test_dest = test_src;
		test_dest.inverse();
		test_dest = test_dest * test_src;

		if (test_ident != test_dest) fail();

	}

	static int test_a_values[] = {3, 1, 2, 4, 5 ,6, 4, 2, 1, 4, 6, 7, 6, 3, 7, 2};
	static int test_b_values[] = {4, 7, 2, 5, 3, 5, 2, 9, 3, 3, 6, 9, 2, 4, 6, 2};

	Mat4i test_a(test_a_values);
	Mat4i test_b(test_b_values);

	Mat4f test_c(test_a);
	Mat4f test_c_scaled(test_c);

	{
		float x = 2.0f;
		float y = 3.0f;
		float z = 4.0f;

		test_c_scaled[0 + 4 * 0] *= x;
		test_c_scaled[0 + 4 * 1] *= y;
		test_c_scaled[0 + 4 * 2] *= z;
		test_c_scaled[1 + 4 * 0] *= x;
		test_c_scaled[1 + 4 * 1] *= y;
		test_c_scaled[1 + 4 * 2] *= z;
		test_c_scaled[2 + 4 * 0] *= x;
		test_c_scaled[2 + 4 * 1] *= y;
		test_c_scaled[2 + 4 * 2] *= z;
		test_c_scaled[3 + 4 * 0] *= x;
		test_c_scaled[3 + 4 * 1] *= y;
		test_c_scaled[3 + 4 * 2] *= z;
	}

	Console::write_line("   Function: add() and operator");
	{
		int answer_values[] = {7, 8, 4, 9, 8, 11, 6, 11, 4, 7, 12, 16, 8, 7, 13, 4};
		Mat4i answer(answer_values);

		Mat4i result = test_a + test_b;
		if (result != answer) fail();

		result = Mat4i::add(test_a, test_b);
		if (result != answer) fail();

	}

	Console::write_line("   Function: subtract() and operator");
	{
		int answer_values[] = {-1, -6, 0, -1, 2, 1, 2, -7, -2, 1, 0, -2, 4, -1, 1, 0};
		Mat4i answer(answer_values);

		Mat4i result = test_a - test_b;
		if (result != answer) fail();

		result = Mat4i::subtract(test_a, test_b);
		if (result != answer) fail();

	}

	Console::write_line("   Function: translate()");
	{
		int answer_values[] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 2, 3, 4, 1};
		Mat4i answer(answer_values);

		Mat4i result = Mat4i::translate(2, 3, 4);
		if (result != answer) fail();
	}

	Console::write_line("   Function: translate_self() (int)");
	{
		Mat4i answer(test_a);

		Mat4i result = test_a;
		result = result * Mat4i::translate(2, 3, 4);

		Mat4i result2 = test_a;
		result2.translate_self(2,3,4);

		if (result != result2) fail();
	}

	Console::write_line("   Function: translate_self() (float)");
	{
		Mat4f answer(test_a);

		Mat4f result(test_a);
		result = result * Mat4f::translate(2, 3, 4);

		Mat4f result2(test_a);
		result2.translate_self(2, 3, 4);

		if (!result.is_equal(result2, 0.00001f))
			fail();
	}

	Console::write_line("   Function: scale_self()");
	{
		Mat4i answer(test_a);

		Mat4i result = test_a;
		result = result * Mat4i::scale(2, 3, 4);

		Mat4i result2 = test_a;
		result2.scale_self(2,3,4);

		if (result != result2) fail();

		Mat4f test = test_c;
		test.scale_self(2.0f, 3.0f, 4.0f);

		if (!test.is_equal(test_c_scaled, 0.00001f))
			fail();
	}

	Console::write_line("   Function: rotate (using euler angles)");
	{
		Mat4f mv = Mat4f::identity();
		mv = mv * Mat4f::rotate(Angle(30.0f, angle_degrees), 0.0f, 0.0f, 1.0f, false);
		mv = mv * Mat4f::rotate(Angle(10.0f, angle_degrees), 1.0f, 0.0f, 0.0f, false);
		mv = mv * Mat4f::rotate(Angle(20.0f, angle_degrees), 0.0f, 1.0f, 0.0f, false);

		Mat4f test_matrix;
		test_matrix = Mat4f::rotate(Angle(10.0f, angle_degrees), Angle(20.0f, angle_degrees), Angle(30.0f, angle_degrees), order_YXZ);
		if (!test_matrix.is_equal(mv, 0.00001f))
			fail();

	}

	Console::write_line("   Function: rotate (using euler angles) and get_euler");
	{
		test_rotate_and_get_euler(order_XYZ);
		test_rotate_and_get_euler(order_XZY);
		test_rotate_and_get_euler(order_YZX);
		test_rotate_and_get_euler(order_YXZ);
		test_rotate_and_get_euler(order_ZXY);
		test_rotate_and_get_euler(order_ZYX);
	}

	Console::write_line("   Function: transpose() (float)");
	{
		Mat4f original(test_a);

		Mat4f transposed_matrix;
	
		transposed_matrix[0] = original[0];
		transposed_matrix[1] = original[4];
		transposed_matrix[2] = original[8];
		transposed_matrix[3] = original[12];
		transposed_matrix[4] = original[1];
		transposed_matrix[5] = original[5];
		transposed_matrix[6] = original[9];
		transposed_matrix[7] = original[13];
		transposed_matrix[8] = original[2];
		transposed_matrix[9] = original[6];
		transposed_matrix[10] = original[10];
		transposed_matrix[11] = original[14];
		transposed_matrix[12] = original[3];
		transposed_matrix[13] = original[7];
		transposed_matrix[14] = original[11];
		transposed_matrix[15] = original[15];

		Mat4f test = original;
		test.transpose();

		if (!test.is_equal(transposed_matrix, 0.00001f))
			fail();
	}
}

void TestApp::test_rotate_and_get_euler(clan::EulerOrder order)
{
	for (int ax = 0; ax < 360; ax += 10)
	{
		for (int ay = 0; ay < 360; ay += 10)
		{
			for (int az = 0; az < 360; az += 10)
			{
				Angle angle_x(ax, angle_degrees);
				Angle angle_y(ay, angle_degrees);
				Angle angle_z(az, angle_degrees);

				Mat4f test_matrix;
				test_matrix = Mat4f::rotate(angle_x, angle_y, angle_z, order);

				Vec3f angles = test_matrix.get_euler(order);

				Mat4f test_matrix2;
				test_matrix2 = Mat4f::rotate(Angle(angles.x, angle_radians), Angle(angles.y, angle_radians), Angle(angles.z, angle_radians), order);

				// Note, since euler angles can have alternative forms, we compare by recreating as a rotation matrix
				if (!test_matrix.is_equal(test_matrix2, 0.00001f))
					fail();

				//check_float(angles.x, angle_x.to_radians());
				//check_float(angles.y, angle_y.to_radians());
				//check_float(angles.z, angle_z.to_radians());
			}
		}
	}
}

void TestApp::test_matrix_mat2()
{
	Console::write_line("  Class: Mat2");

	Mat2d test_a = Mat2d(3, 1, 2, 4);
	Mat2d test_b = Mat2d(-3, 7, 2, 5);

	Console::write_line("   Function: multiply() and operator");
	{
		Mat2d result = test_b * test_a;
		if (result != Mat2d(-7, 26, 2, 34)) fail();

		result = Mat2d::multiply(test_b, test_a);
		if (result != Mat2d(-7, 26, 2, 34)) fail();

	}

	Console::write_line("   Function: add() and operator");
	{
		Mat2d result = test_a + test_b;
		if (result != Mat2d(0, 8, 4, 9)) fail();

		result = Mat2d::add(test_b, test_a);
		if (result != Mat2d(0, 8, 4, 9)) fail();

	}

	Console::write_line("   Function: subtract() and operator");
	{
		Mat2d result = test_a - test_b;
		if (result != Mat2d(6, -6, 0, -1)) fail();

		result = Mat2d::subtract(test_a, test_b);
		if (result != Mat2d(6, -6, 0, -1)) fail();


	}

}

