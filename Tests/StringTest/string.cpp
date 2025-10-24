#include <ClanLib/core.h>
#include <ClanLib/application.h>

#include <iostream>

class CL_StringTest : public CL_ClanApplication
{

	int main(int argc, char** argv)
	{
		CL_SetupCore::init();
		
		double d = 55.67;
		float f = 44.7;
		int i = 23435;
		char c[] = "Werd!";

		std::string test("Hi-ho-everybody!");

		std::cout << CL_String::right(test, 3) << std::endl;
		std::cout << CL_String::left(test, 3) << std::endl;
		
		test.replace(test.begin(), test.begin()+2, "Bye-All");
		
		std::cout << CL_String::format("This %1 is %2 a %3 test! %4 %5", test, d, f, i, c) << std::endl;
		
		std::cout << test << std::endl;
	
		return 0;
	}


} app;
