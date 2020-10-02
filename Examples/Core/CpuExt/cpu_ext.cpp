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
*/

#include <ClanLib/core.h>
using namespace clan;

int main(int argc, char** argv)
{
	try 
	{
		

		ConsoleWindow console("Console");
		Console::write_line( string_format("mmx = %1", System::detect_cpu_extension(System::mmx)) );
		Console::write_line( string_format("mmx_ex = %1",  System::detect_cpu_extension(System::mmx_ex)) );
		Console::write_line( string_format("_3d_now = %1",  System::detect_cpu_extension(System::_3d_now)) );
		Console::write_line( string_format("_3d_now_ex = %1",  System::detect_cpu_extension(System::_3d_now_ex)) );
		Console::write_line( string_format("sse = %1",  System::detect_cpu_extension(System::sse)) );
		Console::write_line( string_format("sse2 = %1",  System::detect_cpu_extension(System::sse2)) );
		Console::write_line( string_format("sse3 = %1",  System::detect_cpu_extension(System::sse3)) );
		Console::write_line( string_format("ssse3 = %1",  System::detect_cpu_extension(System::ssse3)) );
		Console::write_line( string_format("sse4_a = %1",  System::detect_cpu_extension(System::sse4_a)) );
		Console::write_line( string_format("sse4_1 = %1",  System::detect_cpu_extension(System::sse4_1)) );
		Console::write_line( string_format("sse4_2 = %1",  System::detect_cpu_extension(System::sse4_2)) );
		Console::write_line( string_format("xop = %1",  System::detect_cpu_extension(System::xop)) );
		Console::write_line( string_format("avx = %1",  System::detect_cpu_extension(System::avx)) );
		Console::write_line( string_format("aes = %1",  System::detect_cpu_extension(System::aes)) );
		Console::write_line( string_format("fma3 = %1",  System::detect_cpu_extension(System::fma3)) );
		Console::write_line( string_format("fma4 = %1",  System::detect_cpu_extension(System::fma4)) );
		Console::write_line( string_format("num cores = %1",  System::get_num_cores()) );
		console.display_close_message();
	} 

	catch(Exception &exception)
	{
		ConsoleWindow console("Console");
		Console::write_line("Exception caught: " + exception.get_message_and_stack_trace());
		console.display_close_message();
		return 1;
	}

	return 0;
}

