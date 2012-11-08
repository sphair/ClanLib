
#pragma once

class Program
{
public:
	static int main(const std::vector<CL_String> &args);
	static void exit_func();
	static bool exit;

	static void test1(CL_DisplayWindow &window);
	static void test2(CL_DisplayWindow &window);
	static void test3(CL_DisplayWindow &window);
};
