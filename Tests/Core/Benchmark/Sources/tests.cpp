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
**    Magnus Norddahl
**    Mark Page
*/

#include "precomp.h"
#include "tests.h"

void Tests::Init(std::vector<TestInfo> &testlist)
{
	testlist.push_back(TestInfo("", &Tests::test_empty));
	testlist.push_back(TestInfo("i++;", &Tests::test_i_plus_plus));
	testlist.push_back(TestInfo("string = std::string();", &Tests::test_create_string));
	testlist.push_back(TestInfo("char_value = string[0]", &Tests::test_string_index));
	testlist.push_back(TestInfo("char_value = char_array[0];", &Tests::test_char_array_index));
	testlist.push_back(TestInfo("for (int cnt=0; cnt<sixteen; cnt++) char_value += string2[cnt];", &Tests::test_string_multi_index));
	testlist.push_back(TestInfo("for (int cnt=0; cnt<sixteen; cnt++) char_value += char_array[cnt];", &Tests::test_char_array_multi_index));
	testlist.push_back(TestInfo("float_value = (float) int_value;", &Tests::test_int_to_float));
	testlist.push_back(TestInfo("int_value = (int) float_value;", &Tests::test_float_to_int));
	testlist.push_back(TestInfo("double_value = (double) int_value;", &Tests::test_int_to_double));
	testlist.push_back(TestInfo("int_value = (int) double_value;", &Tests::test_double_to_int));

}

Tests::Tests()
{
	for (int cnt=0; cnt<16; cnt++)
		string2.push_back('X');
	sixteen = 16;
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

void Tests::test_string_index()
{
	char_value = string2[0];
}

void Tests::test_string_multi_index()
{
	for (int cnt=0; cnt<sixteen; cnt++) char_value += string2[cnt];
}

void Tests::test_char_array_index()
{
	char_value = char_array[0];
}

void Tests::test_char_array_multi_index()
{
	for (int cnt=0; cnt<sixteen; cnt++) char_value += char_array[cnt];
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

