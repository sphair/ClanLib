#include <ClanLib/core.h>

int main(int argc, char** argv)
{
	try
	{
		std::cout << "The binary '" << argv[0] << "' is located in '" 
			<< CL_System::get_exe_path() << "'" << std::endl;
	}
	catch (CL_Error& err) 
	{
		std::cout << "CL_Error: " << err.message << std::endl;
	}
	return 0;
}
