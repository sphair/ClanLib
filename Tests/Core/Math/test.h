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

#include <ClanLib/core.h>

using namespace clan;

#ifndef WIN32
#include <unistd.h>
#include <stdio.h>
#ifndef MAX_PATH
#define MAX_PATH PATH_MAX
#endif
#else
#include <direct.h>
#ifndef chdir
#define _chdir chdir
#endif
#ifndef MAX_PATH
#define _MAX_PATH MAX_PATH
#endif
#include <tchar.h>
#endif


#ifndef WIN32
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

extern int g_bConstructor;
extern int g_bDestructor;

class MyClass
{
public:
	MyClass() { g_bConstructor++; };
	~MyClass() { g_bDestructor++; };	
};

class TestApp
{
public:
	int main();
private:
	void check_normalize_180(float input_angle, float output_angle);
	void check_float(float value, float target);
	void check_double(double value, double target);
	void test_vector2();
	void test_vector3();
	void test_vector4();
	void test_matrix();
	void test_angle();
	void test_quaternion_f();
	void test_quaternion_d();
	void test_line2();
	void test_line3();
	void test_line_ray2();
	void test_line_ray3();
	void test_line_segment2();
	void test_line_segment3();
	void test_triangle();
	void test_matrix_mat2();
	void test_matrix_mat3();
	void test_matrix_mat4();
	void test_rect();
	void test_bigint();
	void test_rotate_and_get_euler(clan::EulerOrder order);
	void fail();
	void test_quaternion_euler(clan::EulerOrder order);

};

