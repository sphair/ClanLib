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
*/

#include <ClanLib/core.h>

int main(int argc, char** argv)
{
	try 
	{
		CL_SetupCore setup_core;

		CL_ConsoleWindow console("Console");
		CL_Console::write_line( cl_format("mmx = %1", CL_System::detect_cpu_extension(CL_System::mmx)) );
		CL_Console::write_line( cl_format("mmx_ex = %1",  CL_System::detect_cpu_extension(CL_System::mmx_ex)) );
		CL_Console::write_line( cl_format("_3d_now = %1",  CL_System::detect_cpu_extension(CL_System::_3d_now)) );
		CL_Console::write_line( cl_format("_3d_now_ex = %1",  CL_System::detect_cpu_extension(CL_System::_3d_now_ex)) );
		CL_Console::write_line( cl_format("sse = %1",  CL_System::detect_cpu_extension(CL_System::sse)) );
		CL_Console::write_line( cl_format("sse2 = %1",  CL_System::detect_cpu_extension(CL_System::sse2)) );
		CL_Console::write_line( cl_format("sse3 = %1",  CL_System::detect_cpu_extension(CL_System::sse3)) );
		CL_Console::write_line( cl_format("ssse3 = %1",  CL_System::detect_cpu_extension(CL_System::ssse3)) );
		CL_Console::write_line( cl_format("sse4_a = %1",  CL_System::detect_cpu_extension(CL_System::sse4_a)) );
		CL_Console::write_line( cl_format("sse4_1 = %1",  CL_System::detect_cpu_extension(CL_System::sse4_1)) );
		CL_Console::write_line( cl_format("sse4_2 = %1",  CL_System::detect_cpu_extension(CL_System::sse4_2)) );
		CL_Console::write_line( cl_format("num cores = %1",  CL_System::get_num_cores()) );
		console.display_close_message();
	} 

	catch(CL_Exception &exception)
	{
		CL_ConsoleWindow console("Console");
		CL_Console::write_line("Exception caught: " + exception.get_message_and_stack_trace());
		console.display_close_message();
		return 1;
	}

	return 0;
}

