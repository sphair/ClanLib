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
**    Magnus Norddahl
**    Mark Page
*/

#pragma once

class Tests;

#include "utils.h"

class TestInfo
{
public:
	TestInfo(const std::string &name, void (Tests::*func)()) : name(name), func(func), result(0.0f){}

	std::string name;
	void (Tests::*func)();
	float result;

};

class Tests
{
public:
	Tests();

	static void Init(std::vector<TestInfo> &testlist);

	void test_empty();
	void test_i_plus_plus();
	void test_create_string();
	void test_string_index();
	void test_char_array_index();
	void test_string_cstr_index();
	void test_string_multi_index();
	void test_char_array_multi_index();

	void test_int_to_float();
	void test_float_to_int();
	void test_int_to_double();
	void test_double_to_int();
	void test_int_add();
	void test_int_multiply();
	void test_int_divide();
	void test_float_add();
	void test_float_multiply();
	void test_float_divide();
	void test_double_add();
	void test_double_multiply();
	void test_double_divide();

	void Tests::test_pointer_index();
	void Tests::test_shared_index();

	void test_create_string_from_15chars();
	void test_create_new_string_from_15chars();
	void test_assign_string_from_15chars();
	void test_create_string_from_39chars();
	void test_create_new_string_from_39chars();
	void test_assign_string_from_39chars();
	void test_size1_vector();
	void test_size2_vector();
	void test_size3_vector();
	void test_size4_vector();
	void test_calling_empty();
	void test_return_int();
	void test_return_string_v1();
	void test_return_string_v2();
	void test_return_string_v3();
	void test_get_string();
	void test_string_v8();
	void test_string_v8b();

	Utils utils;
	std::string string;
	std::string string2;
	int int_sixteen;
	int int_seven;
	float float_sixteen;
	float float_seven;
	double double_sixteen;
	double double_seven;
	char char_array[16];
	char char_value;
	int int_value;
	float float_value;
	double double_value;
	int *int_ptr;
	std::shared_ptr<int> int_shared_ptr;
	std::vector<int> std_vector_int_size16;
	std::string string_15;
};
