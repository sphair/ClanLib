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

#include "test.h"

void TestApp::test_path_help(void)
{
	Console::write_line(" Header: path_help.h");
	Console::write_line("  Class: PathHelp");

//*** testing make_absolute()

	Console::write_line("   Function: std::string make_absolute()");

	std::string str;
	std::string str2;
	std::string str_dir(working_dir);

	str = PathHelp::make_absolute("ABC/DEF\\GHI", "JKL/MNO\\PQR", PathHelp::path_type_file);
#ifdef WIN32
	str2 = str_dir + "\\ABC\\DEF\\GHI\\JKL\\MNO\\PQR"; 
#else
	str2 = str_dir + "/ABC/DEF/GHI/JKL/MNO/PQR"; 
#endif
	if (str != str2) fail();

	str = PathHelp::make_absolute("ABC/DEF\\GHI\\", "JKL/MNO\\PQR", PathHelp::path_type_file);
#ifdef WIN32
	str2 = str_dir + "\\ABC\\DEF\\GHI\\JKL\\MNO\\PQR"; 
#else
	str2 = str_dir + "/ABC/DEF/GHI/JKL/MNO/PQR"; 
#endif
	if (str != str2) fail();

	str = PathHelp::make_absolute("ABC/DEF\\GHI/", "JKL/MNO\\PQR", PathHelp::path_type_file);
#ifdef WIN32
	str2 = str_dir + "\\ABC\\DEF\\GHI\\JKL\\MNO\\PQR"; 
#else
	str2 = str_dir + "/ABC/DEF/GHI/JKL/MNO/PQR"; 
#endif
	if (str != str2) fail();

	str = PathHelp::make_absolute("ABC/DEF\\GHI", "JKL/MNO\\PQR", PathHelp::path_type_file);
#ifdef WIN32
	str2 = str_dir + "\\ABC\\DEF\\GHI\\JKL\\MNO\\PQR"; 
#else
	str2 = str_dir + "/ABC/DEF/GHI/JKL/MNO/PQR"; 
#endif
	if (str != str2) fail();

	str = PathHelp::make_absolute("/ABC/DEF\\GHI", "JKL/MNO\\PQR", PathHelp::path_type_file);
#ifdef WIN32
	std::string str_location(location_dir);
	str2 = str_location + "\\ABC\\DEF\\GHI\\JKL\\MNO\\PQR"; 
#else
	str2 =  "/ABC/DEF/GHI/JKL/MNO/PQR"; 
#endif
	if (str != str2) fail();

#ifdef WIN32
	// Test not valid on WIn32
#else
	str = PathHelp::make_absolute("ABC/DEF\\GHI", "/JKL/MNO\\PQR", PathHelp::path_type_file);
	str2 = "/JKL/MNO/PQR"; 
	if (str != str2) fail();
#endif

	str = PathHelp::make_absolute("ABC/DEF\\GHI", "JKL/MNO\\PQR", PathHelp::path_type_virtual);
	str2 = "ABC/DEF/GHI/JKL/MNO/PQR"; 
	if (str != str2) fail();

	str = PathHelp::make_absolute("ABC/DEF\\GHI\\", "JKL/MNO\\PQR", PathHelp::path_type_virtual);
	str2 = "ABC/DEF/GHI/JKL/MNO/PQR"; 
	if (str != str2) fail();

	str = PathHelp::make_absolute("ABC/DEF\\GHI/", "JKL/MNO\\PQR", PathHelp::path_type_virtual);
	str2 = "ABC/DEF/GHI/JKL/MNO/PQR"; 
	if (str != str2) fail();

	str = PathHelp::make_absolute("ABC/DEF\\GHI", "JKL/MNO\\PQR", PathHelp::path_type_virtual);
	str2 = "ABC/DEF/GHI/JKL/MNO/PQR"; 
	if (str != str2) fail();

	str = PathHelp::make_absolute("/ABC/DEF\\GHI", "JKL/MNO\\PQR", PathHelp::path_type_virtual);
	str2 =  "/ABC/DEF/GHI/JKL/MNO/PQR"; 
	if (str != str2) fail();

	str = PathHelp::make_absolute("ABC/DEF\\GHI", "/JKL/MNO\\PQR", PathHelp::path_type_virtual);
	str2 = "/JKL/MNO/PQR"; 
	if (str != str2) fail();

//*** testing make_relative()

	Console::write_line("   Function: std::string make_relative()");

	str = PathHelp::make_relative("ABC/DEF\\GHI", str_dir + "/ABC\\DEF/GHI\\JKL/MNO/PQR", PathHelp::path_type_file);
#ifdef WIN32
	str2 = "JKL\\MNO\\PQR"; 
#else
	str2 = "JKL/MNO/PQR"; 
#endif
	if (str != str2) fail();

	str = PathHelp::make_relative("ABC/DEF\\GHI\\", str_dir + "/ABC/DEF/GHI\\JKL/MNO/PQR", PathHelp::path_type_file);
#ifdef WIN32
	str2 = "JKL\\MNO\\PQR"; 
#else
	str2 = "JKL/MNO/PQR"; 
#endif
	if (str != str2) fail();

	str = PathHelp::make_relative("ABC/DEF\\GHI/", str_dir + "/ABC/DEF\\GHI/JKL/MNO/PQR", PathHelp::path_type_file);
#ifdef WIN32
	str2 = "JKL\\MNO\\PQR"; 
#else
	str2 = "JKL/MNO/PQR"; 
#endif
	if (str != str2) fail();

	str = PathHelp::make_relative("ABC/DEF\\GHI", str_dir + "/ABC/DEF\\GHI/JKL/MNO/PQR", PathHelp::path_type_file);
#ifdef WIN32
	str2 = "JKL\\MNO\\PQR"; 
#else
	str2 = "JKL/MNO/PQR"; 
#endif
	if (str != str2) fail();

	str = PathHelp::make_relative("ABC/DEF\\GHI", str_dir + "/ABC/DEF\\JKL/MNO/PQR", PathHelp::path_type_file);
#ifdef WIN32
	str2 = "../JKL\\MNO\\PQR"; 
#else
	str2 = "../JKL/MNO/PQR"; 
#endif
	if (str != str2) fail();

	str = PathHelp::make_relative("/ABC/DEF\\GHI", "/ABC/DEF\\GHI/JKL/MNO/PQR", PathHelp::path_type_file);
#ifdef WIN32
	str2 =  "JKL\\MNO\\PQR"; 
#else
	str2 =  "JKL/MNO/PQR"; 
#endif
	if (str != str2) fail();

	str = PathHelp::make_relative("/ABC/DEF\\GHI\\", "/ABC/DEF/GHI\\JKL/MNO/PQR", PathHelp::path_type_virtual);
	str2 = "JKL/MNO/PQR"; 
	if (str != str2) fail();

	str = PathHelp::make_relative("/ABC/DEF\\GHI/", "/ABC/DEF/GHI\\JKL/MNO/PQR", PathHelp::path_type_virtual);
	str2 = "JKL/MNO/PQR"; 
	if (str != str2) fail();

	str = PathHelp::make_relative("/ABC/DEF\\GHI", "/ABC/DEF/GHI\\JKL/MNO/PQR", PathHelp::path_type_virtual);
	str2 = "JKL/MNO/PQR"; 
	if (str != str2) fail();

	str = PathHelp::make_relative("/ABC/DEF\\GHI", "/ABC/DEF\\GHI/JKL/MNO/PQR", PathHelp::path_type_virtual);
	str2 =  "JKL/MNO/PQR"; 
	if (str != str2) fail();

	str = PathHelp::make_relative("/ABC/DEF\\GHI", "/ABC/DEF\\JKL/MNO/PQR", PathHelp::path_type_virtual);
	str2 =  "../JKL/MNO/PQR"; 
	if (str != str2) fail();

//*** testing is_absolute()

	Console::write_line("   Function: bool is_absolute()");
#ifdef WIN32
	bool result = PathHelp::is_absolute("c:/ABC/DEF\\GHI", PathHelp::path_type_file);
	if (!result) fail();
	result = PathHelp::is_absolute("c:\\ABC/DEF\\GHI", PathHelp::path_type_file);
	if (!result) fail();
#else
	bool result = PathHelp::is_absolute("/ABC/DEF\\GHI", PathHelp::path_type_file);
	if (!result) fail();
	result = PathHelp::is_absolute("\\ABC/DEF\\GHI", PathHelp::path_type_file);
	if (!result) fail();
#endif
	result = PathHelp::is_absolute("ABC/DEF\\GHI", PathHelp::path_type_file);
	if (result) fail();
	result = PathHelp::is_absolute("./ABC/DEF\\GHI", PathHelp::path_type_file);
	if (result) fail();
	result = PathHelp::is_absolute("", PathHelp::path_type_file);
	if (result) fail();

	result = PathHelp::is_absolute("/ABC/DEF\\GHI", PathHelp::path_type_virtual);
	if (!result) fail();
	result = PathHelp::is_absolute("\\ABC/DEF\\GHI", PathHelp::path_type_virtual);
	if (result) fail();	// (As '\' is not allowed)
	result = PathHelp::is_absolute("ABC/DEF\\GHI", PathHelp::path_type_virtual);
	if (result) fail();
	result = PathHelp::is_absolute("./ABC/DEF\\GHI", PathHelp::path_type_virtual);
	if (result) fail();
	result = PathHelp::is_absolute("", PathHelp::path_type_virtual);
	if (result) fail();

//*** testing is_relative()

	Console::write_line("   Function: bool is_relative()");

#ifdef WIN32
	result = PathHelp::is_relative("C:/ABC/DEF\\GHI", PathHelp::path_type_file);
	if (result) fail();
	result = PathHelp::is_relative("c:\\ABC/DEF\\GHI", PathHelp::path_type_file);
	if (result) fail();
#else
	result = PathHelp::is_relative("/ABC/DEF\\GHI", PathHelp::path_type_file);
	if (result) fail();
	result = PathHelp::is_relative("\\ABC/DEF\\GHI", PathHelp::path_type_file);
	if (result) fail();
#endif
	result = PathHelp::is_relative("ABC/DEF\\GHI", PathHelp::path_type_file);
	if (!result) fail();
	result = PathHelp::is_relative("./ABC/DEF\\GHI", PathHelp::path_type_file);
	if (!result) fail();
	result = PathHelp::is_relative("", PathHelp::path_type_file);
	if (!result) fail();

	result = PathHelp::is_relative("/ABC/DEF\\GHI", PathHelp::path_type_virtual);
	if (result) fail();
	result = PathHelp::is_relative("\\ABC/DEF\\GHI", PathHelp::path_type_virtual);
	if (!result) fail();	// (As '\' is not allowed)
	result = PathHelp::is_relative("ABC/DEF\\GHI", PathHelp::path_type_virtual);
	if (!result) fail();
	result = PathHelp::is_relative("./ABC/DEF\\GHI", PathHelp::path_type_virtual);
	if (!result) fail();
	result = PathHelp::is_relative("", PathHelp::path_type_virtual);
	if (!result) fail();

//*** testing normalize()

	Console::write_line("   Function: std::string normalize()");

	str = PathHelp::normalize("ABC/DEF\\GHI", PathHelp::path_type_file);
#ifdef WIN32
	str2 = "ABC\\DEF\\GHI"; 
#else
	str2 = "ABC/DEF/GHI"; 
#endif
	if (str != str2) fail();

	str = PathHelp::normalize("/ABC/DEF\\GHI", PathHelp::path_type_file);
#ifdef WIN32
	str2 = "\\ABC\\DEF\\GHI"; 
#else
	str2 = "/ABC/DEF/GHI"; 
#endif
	if (str != str2) fail();

	str = PathHelp::normalize("\\ABC/DEF\\GHI", PathHelp::path_type_file);
#ifdef WIN32
	str2 = "\\ABC\\DEF\\GHI"; 
#else
	str2 = "/ABC/DEF/GHI"; 
#endif
	if (str != str2) fail();

	str = PathHelp::normalize("././ABC/DEF\\GHI/..\\../JKL", PathHelp::path_type_file);
#ifdef WIN32
	str2 = "ABC\\JKL"; 
#else
	str2 = "ABC/JKL"; 
#endif
	if (str != str2) fail();

	str = PathHelp::normalize("ABC/DEF\\GHI", PathHelp::path_type_virtual);
	str2 = "ABC/DEF/GHI"; 
	if (str != str2) fail();

	str = PathHelp::normalize("/ABC/DEF\\GHI", PathHelp::path_type_virtual);
	str2 = "/ABC/DEF/GHI"; 
	if (str != str2) fail();

	str = PathHelp::normalize("\\ABC/DEF\\GHI", PathHelp::path_type_virtual);
	str2 = "/ABC/DEF/GHI"; 
	if (str != str2) fail();

	str = PathHelp::normalize("././ABC/DEF\\GHI/..\\../JKL", PathHelp::path_type_virtual);
	str2 = "ABC/JKL"; 
	if (str != str2) fail();

//*** testing add_trailing_slash()
	Console::write_line("   Function: std::string add_trailing_slash()");

	str = PathHelp::add_trailing_slash("ABC", PathHelp::path_type_file);
#ifdef WIN32
	str2 = "ABC\\"; 
#else
	str2 = "ABC/"; 
#endif
	if (str != str2) fail();

	str = PathHelp::add_trailing_slash("ABC/", PathHelp::path_type_file);
#ifdef WIN32
	str2 = "ABC/"; 
#else
	str2 = "ABC/"; 
#endif
	if (str != str2) fail();

	str = PathHelp::add_trailing_slash("ABC\\", PathHelp::path_type_file);
#ifdef WIN32
	str2 = "ABC\\"; 
#else
	str2 = "ABC\\"; 
#endif
	if (str != str2) fail();

	str = PathHelp::add_trailing_slash("ABC", PathHelp::path_type_virtual);
	str2 = "ABC/"; 
	if (str != str2) fail();

	str = PathHelp::add_trailing_slash("ABC/", PathHelp::path_type_virtual);
	str2 = "ABC/"; 
	if (str != str2) fail();

	str = PathHelp::add_trailing_slash("ABC\\", PathHelp::path_type_virtual);
	str2 = "ABC\\/"; 
	if (str != str2) fail();

//*** testing remove_trailing_slash()
	Console::write_line("   Function: std::string remove_trailing_slash()");

	str = PathHelp::remove_trailing_slash("ABC");
#ifdef WIN32
	str2 = "ABC"; 
#else
	str2 = "ABC"; 
#endif
	if (str != str2) fail();

	str = PathHelp::remove_trailing_slash("ABC/");
#ifdef WIN32
	str2 = "ABC"; 
#else
	str2 = "ABC"; 
#endif
	if (str != str2) fail();

	str = PathHelp::remove_trailing_slash("ABC\\");
#ifdef WIN32
	str2 = "ABC"; 
#else
	str2 = "ABC"; 
#endif
	if (str != str2) fail();

	str = PathHelp::remove_trailing_slash("ABC");
	str2 = "ABC"; 
	if (str != str2) fail();

	str = PathHelp::remove_trailing_slash("ABC/");
	str2 = "ABC"; 
	if (str != str2) fail();

	str = PathHelp::remove_trailing_slash("ABC\\");
	str2 = "ABC"; 
	if (str != str2) fail();

//*** testing get_location()
	Console::write_line("   Function: std::string get_location()");

	str = PathHelp::get_location("ABC", PathHelp::path_type_file);
	str2 = ""; 
	if (str != str2) fail();

	str = PathHelp::get_location("ABC", PathHelp::path_type_virtual);
	str2 = ""; 
	if (str != str2) fail();

//*** testing get_basepath()
	Console::write_line("   Function: std::string get_basepath()");

	str = PathHelp::get_basepath("ABC", PathHelp::path_type_file);
	str2 = ""; 
	if (str != str2) fail();

#ifdef WIN32	// WIN32 only tests
	str = PathHelp::get_basepath("c:\\ABC\\DEF", PathHelp::path_type_file);
	str2 = "\\ABC\\"; 
	if (str != str2) fail();
	str = PathHelp::get_basepath("\\\\ABC\\DEF\\GHI", PathHelp::path_type_file);
	str2 = "\\DEF\\"; 
	if (str != str2) fail();

#endif

	str = PathHelp::get_basepath("ABC/DEF", PathHelp::path_type_file);
#ifdef WIN32
	str2 = "ABC/"; 
#else
	str2 = "ABC/"; 
#endif
	if (str != str2) fail();

	str = PathHelp::get_basepath("/ABC\\DEF", PathHelp::path_type_file);
#ifdef WIN32
	str2 = "/ABC\\"; 
#else
	str2 = "/ABC\\"; 
#endif
	if (str != str2) fail();

	str = PathHelp::get_basepath("ABC", PathHelp::path_type_virtual);
	str2 = ""; 
	if (str != str2) fail();

	str = PathHelp::get_basepath("ABC/DEF", PathHelp::path_type_virtual);
#ifdef WIN32
	str2 = "ABC/"; 
#else
	str2 = "ABC/"; 
#endif
	if (str != str2) fail();

	str = PathHelp::get_basepath("/ABC\\DEF", PathHelp::path_type_virtual);
#ifdef WIN32
	str2 = "/"; 
#else
	str2 = "/"; 
#endif
	if (str != str2) fail();

//*** testing  split_basepath()
	Console::write_line("   Function: std::vector<std::string> split_basepath()");

	std::vector<std::string> vstr = PathHelp::split_basepath("ABC/DEF\\GHI/JKL", PathHelp::path_type_file);
	if (vstr.size() != 3) fail();
	if (vstr[0] != "ABC") fail();
	if (vstr[1] != "DEF") fail();
	if (vstr[2] != "GHI") fail();

	vstr = PathHelp::split_basepath("ABC/DEF\\GHI/JKL", PathHelp::path_type_virtual);
	if (vstr.size() != 2) fail();
	if (vstr[0] != "ABC") fail();
	if (vstr[1] != "DEF\\GHI") fail();

//*** testing get_fullpath()
	Console::write_line("   Function: std::string get_fullpath()");

	str = PathHelp::get_fullpath("ABC", PathHelp::path_type_file);
	str2 = ""; 
	if (str != str2) fail();

	str = PathHelp::get_fullpath("ABC/DEF", PathHelp::path_type_file);
#ifdef WIN32
	str2 = "ABC/"; 
#else
	str2 = "ABC/"; 
#endif
	if (str != str2) fail();

	str = PathHelp::get_fullpath("/ABC\\DEF", PathHelp::path_type_file);
#ifdef WIN32
	str2 = "/ABC\\"; 
#else
	str2 = "/ABC\\"; 
#endif
	if (str != str2) fail();

	str = PathHelp::get_fullpath("ABC", PathHelp::path_type_virtual);
	str2 = ""; 
	if (str != str2) fail();

	str = PathHelp::get_fullpath("ABC/DEF", PathHelp::path_type_virtual);
	str2 = "ABC/"; 
	if (str != str2) fail();

	str = PathHelp::get_fullpath("/ABC\\DEF", PathHelp::path_type_virtual);
	str2 = "/"; 
	if (str != str2) fail();

//*** testing get_filename()
	Console::write_line("   Function: std::string get_filename()");

	str = PathHelp::get_filename("ABC/DEF\\HIJ.KLM", PathHelp::path_type_file);
	str2 = "HIJ.KLM"; 
	if (str != str2) fail();

	str = PathHelp::get_filename("ABC/DEF\\HIJ.KLM", PathHelp::path_type_virtual);
	str2 = "DEF\\HIJ.KLM"; 
	if (str != str2) fail();

//*** testing get_basename()
	Console::write_line("   Function: std::string get_basename()");

	str = PathHelp::get_basename("ABC/DEF\\HIJ.KLM", PathHelp::path_type_file);
	str2 = "HIJ"; 
	if (str != str2) fail();

	str = PathHelp::get_basename("ABC/DEF\\.KLM", PathHelp::path_type_file);
#ifdef WIN32
	str2 = ""; 
#else
	str2 = ".KLM"; 
#endif
	if (str != str2) fail();

	str = PathHelp::get_basename("ABC/DEF\\HIJ.KLM", PathHelp::path_type_virtual);
	str2 = "DEF\\HIJ"; 
	if (str != str2) fail();

	str = PathHelp::get_basename("ABC/.DEF\\X", PathHelp::path_type_virtual);
#ifdef WIN32
	str2 = ".DEF\\X"; 
#else
	str2 = ".DEF\\X"; 
#endif
	if (str != str2) fail();

//*** testing get_extension()
	Console::write_line("   Function: std::string get_extension()");

	str = PathHelp::get_extension("ABC/DEF\\HIJ.KLM", PathHelp::path_type_file);
	str2 = "KLM"; 
	if (str != str2) fail();

	str = PathHelp::get_extension("ABC/DEF\\.KLM", PathHelp::path_type_file);
#ifdef WIN32
	str2 = "KLM"; 
#else
	str2 = ""; 
#endif
	if (str != str2) fail();

	str = PathHelp::get_extension("ABC/DEF\\HIJ.KLM", PathHelp::path_type_virtual);
	str2 = "KLM"; 
	if (str != str2) fail();

	str = PathHelp::get_extension("ABC/.DEF\\X", PathHelp::path_type_virtual);
	str2 = ""; 
	if (str != str2) fail();

//*** testing get_fullname(#1)
	Console::write_line("   Function: std::string get_fullname(#1)");

	str = PathHelp::get_fullname("ABC", "DEF", PathHelp::path_type_file);
#ifdef WIN32
	str2 = "ABC\\DEF"; 
#else
	str2 = "ABC/DEF"; 
#endif
	if (str != str2) fail();

	str = PathHelp::get_fullname("ABC", "DEF", PathHelp::path_type_virtual);
	str2 = "ABC/DEF"; 
	if (str != str2) fail();

//*** testing get_fullname(#2)
	Console::write_line("   Function: std::string get_fullname(#2)");

	str = PathHelp::get_fullname("ABC", "DEF", "GHI", PathHelp::path_type_file);
#ifdef WIN32
	str2 = "ABC\\DEF.GHI"; 
#else
	str2 = "ABC/DEF.GHI"; 
#endif
	if (str != str2) fail();

	str = PathHelp::get_fullname("ABC", "DEF", "GHI", PathHelp::path_type_virtual);
	str2 = "ABC/DEF.GHI"; 
	if (str != str2) fail();

//*** testing get_fullname(#3)
	Console::write_line("   Function: std::string get_fullname(#3)");

	str = PathHelp::get_fullname("ABC", "DEF", "GHI", "JKL", PathHelp::path_type_file);
#ifdef WIN32
	str2 = "ABCDEF\\GHI.JKL"; 
#else
	str2 = "ABCDEF/GHI.JKL"; 
#endif
	if (str != str2) fail();

	str = PathHelp::get_fullname("ABC", "DEF", "GHI", "JKL", PathHelp::path_type_virtual);
	str2 = "ABCDEF/GHI.JKL"; 
	if (str != str2) fail();

}


