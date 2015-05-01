/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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

#include "precomp.h"
#include "tests.h"

void Tests::Init(std::vector<TestInfo> &testlist)
{
	testlist.push_back(TestInfo("{}", &Tests::test_empty));
/*	testlist.push_back(TestInfo("{i++;}", &Tests::test_i_plus_plus));
	testlist.push_back(TestInfo("{float_value = (float) int_value;}", &Tests::test_int_to_float));
	testlist.push_back(TestInfo("{int_value = (int) float_value;}", &Tests::test_float_to_int));
	testlist.push_back(TestInfo("{double_value = (double) int_value;}", &Tests::test_int_to_double));
	testlist.push_back(TestInfo("{int_value = (int) double_value;}", &Tests::test_double_to_int));
	testlist.push_back(TestInfo("{int_value += int_sixteen + int_seven;}", &Tests::test_int_add));
	testlist.push_back(TestInfo("{int_value += int_sixteen * int_seven;}", &Tests::test_int_multiply));
	testlist.push_back(TestInfo("{int_value += int_sixteen / int_seven;}", &Tests::test_int_divide));
	testlist.push_back(TestInfo("{float_value += float_sixteen + float_seven;}", &Tests::test_float_add));
	testlist.push_back(TestInfo("{float_value += float_sixteen * float_seven;}", &Tests::test_float_multiply));
	testlist.push_back(TestInfo("{float_value += float_sixteen / float_seven;}", &Tests::test_float_divide));
	testlist.push_back(TestInfo("{double_value += double_sixteen + double_seven;}", &Tests::test_double_add));
	testlist.push_back(TestInfo("{double_value += double_sixteen * double_seven;}", &Tests::test_double_multiply));
	testlist.push_back(TestInfo("{double_value += double_sixteen / double_seven;}", &Tests::test_double_divide));
	testlist.push_back(TestInfo("{string = std::string();}", &Tests::test_create_string));
	testlist.push_back(TestInfo("{char_value = string[0]}", &Tests::test_string_index));
	testlist.push_back(TestInfo("{char_value = string.cstr()[0]}", &Tests::test_string_cstr_index));
	testlist.push_back(TestInfo("{char_value = char_array[0];}", &Tests::test_char_array_index));
	testlist.push_back(TestInfo("{for (int cnt=0; cnt<sixteen; cnt++) char_value += string2[cnt];}", &Tests::test_string_multi_index));
	testlist.push_back(TestInfo("{for (int cnt=0; cnt<sixteen; cnt++) char_value += char_array[cnt];}", &Tests::test_char_array_multi_index));

	testlist.push_back(TestInfo("{string = \"123456789012345\";}", &Tests::test_create_string_from_15chars));
	testlist.push_back(TestInfo("{string = std::string(\"123456789012345\");}", &Tests::test_create_new_string_from_15chars));
	testlist.push_back(TestInfo("{string.assign(\"123456789012345\");}", &Tests::test_assign_string_from_15chars));
	testlist.push_back(TestInfo("{string = \"123456789012345678901234567890123456789\";}", &Tests::test_create_string_from_39chars));
	testlist.push_back(TestInfo("{string = std::string(\"123456789012345678901234567890123456789\");}", &Tests::test_create_new_string_from_39chars));
	testlist.push_back(TestInfo("{string.assign(\"123456789012345678901234567890123456789\");}", &Tests::test_assign_string_from_39chars));

	testlist.push_back(TestInfo("{int_value += *int_ptr;}", &Tests::test_pointer_index));
	testlist.push_back(TestInfo("{int_value += *int_shared_ptr;}", &Tests::test_shared_index));

	testlist.push_back(TestInfo("{for (size_t cnt=0; cnt<std_vector_int_size16.size(); cnt++) { int_value +=  std_vector_int_size16[cnt]; }}", &Tests::test_size1_vector));
	testlist.push_back(TestInfo("{for (size_t cnt=0, max = std_vector_int_size16.size(); cnt<max; cnt++) { int_value +=  std_vector_int_size16[cnt]; }}", &Tests::test_size2_vector));

	testlist.push_back(TestInfo("{for (auto it = std_vector_int_size16.begin(); it != std_vector_int_size16.end(); ++it) { int_value += *it; }}", &Tests::test_size3_vector));
	testlist.push_back(TestInfo("{for (auto it = std_vector_int_size16.begin(); it != std_vector_int_size16.end(); it++) { int_value += *it; }}", &Tests::test_size4_vector));

	testlist.push_back(TestInfo("{utils.function();}    : void function() {}", &Tests::test_calling_empty));
	testlist.push_back(TestInfo("{int_value = utils.function();}    : int function() {return five;}", &Tests::test_return_int));
	testlist.push_back(TestInfo("{string = utils.function();}    : std::string function() {return string_hello_world;}", &Tests::test_return_string_v1));
	testlist.push_back(TestInfo("{string = utils.function();}    : std::string &function() {return string_hello_world;}", &Tests::test_return_string_v2));
	testlist.push_back(TestInfo("{string = utils.function();}    : std::string function() {return \"hello world\";}", &Tests::test_return_string_v3));
	testlist.push_back(TestInfo("{utils.function(string);}    : void function(std::string &out_string) {out_string = string_hello_world;}", &Tests::test_get_string));
*/
	testlist.push_back(TestInfo("{int_value += utils.function(\"~15chars~\");}    : int function(const std::string &s) {return s.length();}", &Tests::test_string_v8));
	testlist.push_back(TestInfo("{int_value += utils.function(string_15);}    : int function(const std::string &s) {return s.length();}", &Tests::test_string_v8b));

}

Tests::Tests()
{
	for (int cnt=0; cnt<16; cnt++)
		string2.push_back('X');
	int_sixteen = 16;
	int_seven = 7;
	float_sixteen = 16.0f;
	float_seven = 7.0f;
	double_sixteen = 16.0;
	double_seven = 7.0;
	int_ptr = &int_value;
	int_shared_ptr = std::shared_ptr<int>(new int);
	*int_shared_ptr = 123;

	std_vector_int_size16.resize(16, 0);
	string_15 = "123456789012345";
}

void Tests::test_empty()
{
}

void Tests::test_i_plus_plus()
{
	int_value++;
}

void Tests::test_create_string()
{
	string = std::string();
}

void Tests::test_create_string_from_15chars()
{
	string = "123456789012345";
}

void Tests::test_create_new_string_from_15chars()
{
	string = std::string("123456789012345");
}

void Tests::test_assign_string_from_15chars()
{
	string.assign("123456789012345");
}

void Tests::test_create_string_from_39chars()
{
	string = "123456789012345678901234567890123456789";
}

void Tests::test_create_new_string_from_39chars()
{
	string = std::string("123456789012345678901234567890123456789");
}

void Tests::test_assign_string_from_39chars()
{
	string.assign("123456789012345678901234567890123456789");
}

void Tests::test_string_index()
{
	char_value = string2[0];
}

void Tests::test_string_cstr_index()
{
	char_value = string2.c_str()[0];
}

void Tests::test_string_multi_index()
{
	for (int cnt=0; cnt<int_sixteen; cnt++) char_value += string2[cnt];
}

void Tests::test_char_array_index()
{
	char_value = char_array[0];
}

void Tests::test_char_array_multi_index()
{
	for (int cnt=0; cnt<int_sixteen; cnt++) char_value += char_array[cnt];
}

void Tests::test_int_to_float()
{
	float_value = (float) int_value;
}

void Tests::test_float_to_int()
{
	int_value = (int) float_value;
}

void Tests::test_int_to_double()
{
	double_value = (double) int_value;
}

void Tests::test_double_to_int()
{
	int_value = (int) double_value;
}

void Tests::test_int_add()
{
	int_value += int_sixteen + int_seven;
}

void Tests::test_int_multiply()
{
	int_value += int_sixteen * int_seven;
}

void Tests::test_int_divide()
{
	int_value += int_sixteen / int_seven;
}

void Tests::test_float_add()
{
	float_value += float_sixteen + float_seven;
}

void Tests::test_float_multiply()
{
	float_value += float_sixteen * float_seven;
}

void Tests::test_float_divide()
{
	float_value += float_sixteen / float_seven;
}

void Tests::test_double_add()
{
	double_value += double_sixteen + double_seven;
}

void Tests::test_double_multiply()
{
	double_value += double_sixteen * double_seven;
}

void Tests::test_double_divide()
{
	double_value += double_sixteen / double_seven;
}

void Tests::test_pointer_index()
{
	int_value += *int_ptr;
}

void Tests::test_shared_index()
{
	int_value += *int_shared_ptr;
}

void Tests::test_size1_vector()
{
	for (size_t cnt=0; cnt<std_vector_int_size16.size(); cnt++) { int_value +=  std_vector_int_size16[cnt]; }
}

void Tests::test_size2_vector()
{
	for (size_t cnt=0, max = std_vector_int_size16.size(); cnt<max; cnt++) { int_value +=  std_vector_int_size16[cnt]; }
}

void Tests::test_size3_vector()
{
	for (auto it = std_vector_int_size16.begin(); it != std_vector_int_size16.end(); ++it) { int_value += *it; }
}

void Tests::test_size4_vector()
{
	for (auto it = std_vector_int_size16.begin(); it != std_vector_int_size16.end(); it++) { int_value += *it; }
}


void Tests::test_calling_empty()
{
	utils.test_empty();
}

void Tests::test_return_int()
{
	int_value = utils.test_return_int();
}

void Tests::test_return_string_v1()
{
	string = utils.test_return_string();
}

void Tests::test_return_string_v2()
{
	string = utils.test_return_string2();
}

void Tests::test_return_string_v3()
{
	string = utils.test_return_string3();
}

void Tests::test_get_string()
{
	utils.test_get_string(string);
}

void Tests::test_string_v8()
{
	int_value += utils.test_string_v8("123456789012345");
}

void Tests::test_string_v8b()
{
	int_value += utils.test_string_v8(string_15);
}
