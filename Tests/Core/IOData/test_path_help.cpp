/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
	CL_Console::write_line(" Header: path_help.h");
	CL_Console::write_line("  Class: CL_PathHelp");

//*** testing make_absolute()

	CL_Console::write_line("   Function: CL_String make_absolute()");

	CL_String str;
	CL_String str2;
	CL_String str_dir(working_dir);

	str = CL_PathHelp::make_absolute(cl_text("ABC/DEF\\GHI"), cl_text("JKL/MNO\\PQR"), CL_PathHelp::path_type_file);
#ifdef WIN32
	str2 = str_dir + cl_text("\\ABC\\DEF\\GHI\\JKL\\MNO\\PQR"); 
#else
	str2 = str_dir + cl_text("/ABC/DEF/GHI/JKL/MNO/PQR"); 
#endif
	if (str != str2) fail();

	str = CL_PathHelp::make_absolute(cl_text("ABC/DEF\\GHI\\"), cl_text("JKL/MNO\\PQR"), CL_PathHelp::path_type_file);
#ifdef WIN32
	str2 = str_dir + cl_text("\\ABC\\DEF\\GHI\\JKL\\MNO\\PQR"); 
#else
	str2 = str_dir + cl_text("/ABC/DEF/GHI/JKL/MNO/PQR"); 
#endif
	if (str != str2) fail();

	str = CL_PathHelp::make_absolute(cl_text("ABC/DEF\\GHI/"), cl_text("JKL/MNO\\PQR"), CL_PathHelp::path_type_file);
#ifdef WIN32
	str2 = str_dir + cl_text("\\ABC\\DEF\\GHI\\JKL\\MNO\\PQR"); 
#else
	str2 = str_dir + cl_text("/ABC/DEF/GHI/JKL/MNO/PQR"); 
#endif
	if (str != str2) fail();

	str = CL_PathHelp::make_absolute(cl_text("ABC/DEF\\GHI"), cl_text("JKL/MNO\\PQR"), CL_PathHelp::path_type_file);
#ifdef WIN32
	str2 = str_dir + cl_text("\\ABC\\DEF\\GHI\\JKL\\MNO\\PQR"); 
#else
	str2 = str_dir + cl_text("/ABC/DEF/GHI/JKL/MNO/PQR"); 
#endif
	if (str != str2) fail();

	str = CL_PathHelp::make_absolute(cl_text("/ABC/DEF\\GHI"), cl_text("JKL/MNO\\PQR"), CL_PathHelp::path_type_file);
#ifdef WIN32
	CL_String str_location(location_dir);
	str2 = str_location + cl_text("\\ABC\\DEF\\GHI\\JKL\\MNO\\PQR"); 
#else
	str2 =  cl_text("/ABC/DEF/GHI/JKL/MNO/PQR"); 
#endif
	if (str != str2) fail();

#ifdef WIN32
	// Test not valid on WIn32
#else
	str = CL_PathHelp::make_absolute(cl_text("ABC/DEF\\GHI"), cl_text("/JKL/MNO\\PQR"), CL_PathHelp::path_type_file);
	str2 = cl_text("/JKL/MNO/PQR"); 
	if (str != str2) fail();
#endif

	str = CL_PathHelp::make_absolute(cl_text("ABC/DEF\\GHI"), cl_text("JKL/MNO\\PQR"), CL_PathHelp::path_type_virtual);
	str2 = cl_text("ABC/DEF/GHI/JKL/MNO/PQR"); 
	if (str != str2) fail();

	str = CL_PathHelp::make_absolute(cl_text("ABC/DEF\\GHI\\"), cl_text("JKL/MNO\\PQR"), CL_PathHelp::path_type_virtual);
	str2 = cl_text("ABC/DEF/GHI/JKL/MNO/PQR"); 
	if (str != str2) fail();

	str = CL_PathHelp::make_absolute(cl_text("ABC/DEF\\GHI/"), cl_text("JKL/MNO\\PQR"), CL_PathHelp::path_type_virtual);
	str2 = cl_text("ABC/DEF/GHI/JKL/MNO/PQR"); 
	if (str != str2) fail();

	str = CL_PathHelp::make_absolute(cl_text("ABC/DEF\\GHI"), cl_text("JKL/MNO\\PQR"), CL_PathHelp::path_type_virtual);
	str2 = cl_text("ABC/DEF/GHI/JKL/MNO/PQR"); 
	if (str != str2) fail();

	str = CL_PathHelp::make_absolute(cl_text("/ABC/DEF\\GHI"), cl_text("JKL/MNO\\PQR"), CL_PathHelp::path_type_virtual);
	str2 =  cl_text("/ABC/DEF/GHI/JKL/MNO/PQR"); 
	if (str != str2) fail();

	str = CL_PathHelp::make_absolute(cl_text("ABC/DEF\\GHI"), cl_text("/JKL/MNO\\PQR"), CL_PathHelp::path_type_virtual);
	str2 = cl_text("/JKL/MNO/PQR"); 
	if (str != str2) fail();

//*** testing make_relative()

	CL_Console::write_line("   Function: CL_String make_relative()");

	str = CL_PathHelp::make_relative(cl_text("ABC/DEF\\GHI"), str_dir + cl_text("/ABC\\DEF/GHI\\JKL/MNO/PQR"), CL_PathHelp::path_type_file);
#ifdef WIN32
	str2 = cl_text("JKL\\MNO\\PQR"); 
#else
	str2 = cl_text("JKL/MNO/PQR"); 
#endif
	if (str != str2) fail();

	str = CL_PathHelp::make_relative(cl_text("ABC/DEF\\GHI\\"), str_dir + cl_text("/ABC/DEF/GHI\\JKL/MNO/PQR"), CL_PathHelp::path_type_file);
#ifdef WIN32
	str2 = cl_text("JKL\\MNO\\PQR"); 
#else
	str2 = cl_text("JKL/MNO/PQR"); 
#endif
	if (str != str2) fail();

	str = CL_PathHelp::make_relative(cl_text("ABC/DEF\\GHI/"), str_dir + cl_text("/ABC/DEF\\GHI/JKL/MNO/PQR"), CL_PathHelp::path_type_file);
#ifdef WIN32
	str2 = cl_text("JKL\\MNO\\PQR"); 
#else
	str2 = cl_text("JKL/MNO/PQR"); 
#endif
	if (str != str2) fail();

	str = CL_PathHelp::make_relative(cl_text("ABC/DEF\\GHI"), str_dir + cl_text("/ABC/DEF\\GHI/JKL/MNO/PQR"), CL_PathHelp::path_type_file);
#ifdef WIN32
	str2 = cl_text("JKL\\MNO\\PQR"); 
#else
	str2 = cl_text("JKL/MNO/PQR"); 
#endif
	if (str != str2) fail();

	str = CL_PathHelp::make_relative(cl_text("ABC/DEF\\GHI"), str_dir + cl_text("/ABC/DEF\\JKL/MNO/PQR"), CL_PathHelp::path_type_file);
#ifdef WIN32
	str2 = cl_text("../JKL\\MNO\\PQR"); 
#else
	str2 = cl_text("../JKL/MNO/PQR"); 
#endif
	if (str != str2) fail();

	str = CL_PathHelp::make_relative(cl_text("/ABC/DEF\\GHI"), cl_text("/ABC/DEF\\GHI/JKL/MNO/PQR"), CL_PathHelp::path_type_file);
#ifdef WIN32
	str2 =  cl_text("JKL\\MNO\\PQR"); 
#else
	str2 =  cl_text("JKL/MNO/PQR"); 
#endif
	if (str != str2) fail();

	str = CL_PathHelp::make_relative(cl_text("/ABC/DEF\\GHI\\"), cl_text("/ABC/DEF/GHI\\JKL/MNO/PQR"), CL_PathHelp::path_type_virtual);
	str2 = cl_text("JKL/MNO/PQR"); 
	if (str != str2) fail();

	str = CL_PathHelp::make_relative(cl_text("/ABC/DEF\\GHI/"), cl_text("/ABC/DEF/GHI\\JKL/MNO/PQR"), CL_PathHelp::path_type_virtual);
	str2 = cl_text("JKL/MNO/PQR"); 
	if (str != str2) fail();

	str = CL_PathHelp::make_relative(cl_text("/ABC/DEF\\GHI"), cl_text("/ABC/DEF/GHI\\JKL/MNO/PQR"), CL_PathHelp::path_type_virtual);
	str2 = cl_text("JKL/MNO/PQR"); 
	if (str != str2) fail();

	str = CL_PathHelp::make_relative(cl_text("/ABC/DEF\\GHI"), cl_text("/ABC/DEF\\GHI/JKL/MNO/PQR"), CL_PathHelp::path_type_virtual);
	str2 =  cl_text("JKL/MNO/PQR"); 
	if (str != str2) fail();

	str = CL_PathHelp::make_relative(cl_text("/ABC/DEF\\GHI"), cl_text("/ABC/DEF\\JKL/MNO/PQR"), CL_PathHelp::path_type_virtual);
	str2 =  cl_text("../JKL/MNO/PQR"); 
	if (str != str2) fail();

//*** testing is_absolute()

	CL_Console::write_line("   Function: bool is_absolute()");
#ifdef WIN32
	bool result = CL_PathHelp::is_absolute(cl_text("c:/ABC/DEF\\GHI"), CL_PathHelp::path_type_file);
	if (!result) fail();
	result = CL_PathHelp::is_absolute(cl_text("c:\\ABC/DEF\\GHI"), CL_PathHelp::path_type_file);
	if (!result) fail();
#else
	bool result = CL_PathHelp::is_absolute(cl_text("/ABC/DEF\\GHI"), CL_PathHelp::path_type_file);
	if (!result) fail();
	result = CL_PathHelp::is_absolute(cl_text("\\ABC/DEF\\GHI"), CL_PathHelp::path_type_file);
	if (!result) fail();
#endif
	result = CL_PathHelp::is_absolute(cl_text("ABC/DEF\\GHI"), CL_PathHelp::path_type_file);
	if (result) fail();
	result = CL_PathHelp::is_absolute(cl_text("./ABC/DEF\\GHI"), CL_PathHelp::path_type_file);
	if (result) fail();
	result = CL_PathHelp::is_absolute(cl_text(""), CL_PathHelp::path_type_file);
	if (result) fail();

	result = CL_PathHelp::is_absolute(cl_text("/ABC/DEF\\GHI"), CL_PathHelp::path_type_virtual);
	if (!result) fail();
	result = CL_PathHelp::is_absolute(cl_text("\\ABC/DEF\\GHI"), CL_PathHelp::path_type_virtual);
	if (result) fail();	// (As '\' is not allowed)
	result = CL_PathHelp::is_absolute(cl_text("ABC/DEF\\GHI"), CL_PathHelp::path_type_virtual);
	if (result) fail();
	result = CL_PathHelp::is_absolute(cl_text("./ABC/DEF\\GHI"), CL_PathHelp::path_type_virtual);
	if (result) fail();
	result = CL_PathHelp::is_absolute(cl_text(""), CL_PathHelp::path_type_virtual);
	if (result) fail();

//*** testing is_relative()

	CL_Console::write_line("   Function: bool is_relative()");

#ifdef WIN32
	result = CL_PathHelp::is_relative(cl_text("C:/ABC/DEF\\GHI"), CL_PathHelp::path_type_file);
	if (result) fail();
	result = CL_PathHelp::is_relative(cl_text("c:\\ABC/DEF\\GHI"), CL_PathHelp::path_type_file);
	if (result) fail();
#else
	result = CL_PathHelp::is_relative(cl_text("/ABC/DEF\\GHI"), CL_PathHelp::path_type_file);
	if (result) fail();
	result = CL_PathHelp::is_relative(cl_text("\\ABC/DEF\\GHI"), CL_PathHelp::path_type_file);
	if (result) fail();
#endif
	result = CL_PathHelp::is_relative(cl_text("ABC/DEF\\GHI"), CL_PathHelp::path_type_file);
	if (!result) fail();
	result = CL_PathHelp::is_relative(cl_text("./ABC/DEF\\GHI"), CL_PathHelp::path_type_file);
	if (!result) fail();
	result = CL_PathHelp::is_relative(cl_text(""), CL_PathHelp::path_type_file);
	if (!result) fail();

	result = CL_PathHelp::is_relative(cl_text("/ABC/DEF\\GHI"), CL_PathHelp::path_type_virtual);
	if (result) fail();
	result = CL_PathHelp::is_relative(cl_text("\\ABC/DEF\\GHI"), CL_PathHelp::path_type_virtual);
	if (!result) fail();	// (As '\' is not allowed)
	result = CL_PathHelp::is_relative(cl_text("ABC/DEF\\GHI"), CL_PathHelp::path_type_virtual);
	if (!result) fail();
	result = CL_PathHelp::is_relative(cl_text("./ABC/DEF\\GHI"), CL_PathHelp::path_type_virtual);
	if (!result) fail();
	result = CL_PathHelp::is_relative(cl_text(""), CL_PathHelp::path_type_virtual);
	if (!result) fail();

//*** testing normalize()

	CL_Console::write_line("   Function: CL_String normalize()");

	str = CL_PathHelp::normalize(cl_text("ABC/DEF\\GHI"), CL_PathHelp::path_type_file);
#ifdef WIN32
	str2 = cl_text("ABC\\DEF\\GHI"); 
#else
	str2 = cl_text("ABC/DEF/GHI"); 
#endif
	if (str != str2) fail();

	str = CL_PathHelp::normalize(cl_text("/ABC/DEF\\GHI"), CL_PathHelp::path_type_file);
#ifdef WIN32
	str2 = cl_text("\\ABC\\DEF\\GHI"); 
#else
	str2 = cl_text("/ABC/DEF/GHI"); 
#endif
	if (str != str2) fail();

	str = CL_PathHelp::normalize(cl_text("\\ABC/DEF\\GHI"), CL_PathHelp::path_type_file);
#ifdef WIN32
	str2 = cl_text("\\ABC\\DEF\\GHI"); 
#else
	str2 = cl_text("/ABC/DEF/GHI"); 
#endif
	if (str != str2) fail();

	str = CL_PathHelp::normalize(cl_text("././ABC/DEF\\GHI/..\\../JKL"), CL_PathHelp::path_type_file);
#ifdef WIN32
	str2 = cl_text("ABC\\JKL"); 
#else
	str2 = cl_text("ABC/JKL"); 
#endif
	if (str != str2) fail();

	str = CL_PathHelp::normalize(cl_text("ABC/DEF\\GHI"), CL_PathHelp::path_type_virtual);
	str2 = cl_text("ABC/DEF/GHI"); 
	if (str != str2) fail();

	str = CL_PathHelp::normalize(cl_text("/ABC/DEF\\GHI"), CL_PathHelp::path_type_virtual);
	str2 = cl_text("/ABC/DEF/GHI"); 
	if (str != str2) fail();

	str = CL_PathHelp::normalize(cl_text("\\ABC/DEF\\GHI"), CL_PathHelp::path_type_virtual);
	str2 = cl_text("/ABC/DEF/GHI"); 
	if (str != str2) fail();

	str = CL_PathHelp::normalize(cl_text("././ABC/DEF\\GHI/..\\../JKL"), CL_PathHelp::path_type_virtual);
	str2 = cl_text("ABC/JKL"); 
	if (str != str2) fail();

//*** testing add_trailing_slash()
	CL_Console::write_line("   Function: CL_String add_trailing_slash()");

	str = CL_PathHelp::add_trailing_slash(cl_text("ABC"), CL_PathHelp::path_type_file);
#ifdef WIN32
	str2 = cl_text("ABC\\"); 
#else
	str2 = cl_text("ABC/"); 
#endif
	if (str != str2) fail();

	str = CL_PathHelp::add_trailing_slash(cl_text("ABC/"), CL_PathHelp::path_type_file);
#ifdef WIN32
	str2 = cl_text("ABC/"); 
#else
	str2 = cl_text("ABC/"); 
#endif
	if (str != str2) fail();

	str = CL_PathHelp::add_trailing_slash(cl_text("ABC\\"), CL_PathHelp::path_type_file);
#ifdef WIN32
	str2 = cl_text("ABC\\"); 
#else
	str2 = cl_text("ABC\\"); 
#endif
	if (str != str2) fail();

	str = CL_PathHelp::add_trailing_slash(cl_text("ABC"), CL_PathHelp::path_type_virtual);
	str2 = cl_text("ABC/"); 
	if (str != str2) fail();

	str = CL_PathHelp::add_trailing_slash(cl_text("ABC/"), CL_PathHelp::path_type_virtual);
	str2 = cl_text("ABC/"); 
	if (str != str2) fail();

	str = CL_PathHelp::add_trailing_slash(cl_text("ABC\\"), CL_PathHelp::path_type_virtual);
	str2 = cl_text("ABC\\/"); 
	if (str != str2) fail();

//*** testing remove_trailing_slash()
	CL_Console::write_line("   Function: CL_String remove_trailing_slash()");

	str = CL_PathHelp::remove_trailing_slash(cl_text("ABC"), CL_PathHelp::path_type_file);
#ifdef WIN32
	str2 = cl_text("ABC"); 
#else
	str2 = cl_text("ABC"); 
#endif
	if (str != str2) fail();

	str = CL_PathHelp::remove_trailing_slash(cl_text("ABC/"), CL_PathHelp::path_type_file);
#ifdef WIN32
	str2 = cl_text("ABC"); 
#else
	str2 = cl_text("ABC"); 
#endif
	if (str != str2) fail();

	str = CL_PathHelp::remove_trailing_slash(cl_text("ABC\\"), CL_PathHelp::path_type_file);
#ifdef WIN32
	str2 = cl_text("ABC"); 
#else
	str2 = cl_text("ABC"); 
#endif
	if (str != str2) fail();

	str = CL_PathHelp::remove_trailing_slash(cl_text("ABC"), CL_PathHelp::path_type_virtual);
	str2 = cl_text("ABC"); 
	if (str != str2) fail();

	str = CL_PathHelp::remove_trailing_slash(cl_text("ABC/"), CL_PathHelp::path_type_virtual);
	str2 = cl_text("ABC"); 
	if (str != str2) fail();

	str = CL_PathHelp::remove_trailing_slash(cl_text("ABC\\"), CL_PathHelp::path_type_virtual);
	str2 = cl_text("ABC"); 
	if (str != str2) fail();

//*** testing get_location()
	CL_Console::write_line("   Function: CL_String get_location()");

	str = CL_PathHelp::get_location(cl_text("ABC"), CL_PathHelp::path_type_file);
	str2 = cl_text(""); 
	if (str != str2) fail();

	str = CL_PathHelp::get_location(cl_text("ABC"), CL_PathHelp::path_type_virtual);
	str2 = cl_text(""); 
	if (str != str2) fail();

//*** testing get_basepath()
	CL_Console::write_line("   Function: CL_String get_basepath()");

	str = CL_PathHelp::get_basepath(cl_text("ABC"), CL_PathHelp::path_type_file);
	str2 = cl_text(""); 
	if (str != str2) fail();

#ifdef WIN32	// WIN32 only tests
	str = CL_PathHelp::get_basepath(cl_text("c:\\ABC\\DEF"), CL_PathHelp::path_type_file);
	str2 = cl_text("\\ABC\\"); 
	if (str != str2) fail();
	str = CL_PathHelp::get_basepath(cl_text("\\\\ABC\\DEF\\GHI"), CL_PathHelp::path_type_file);
	str2 = cl_text("\\DEF\\"); 
	if (str != str2) fail();

#endif

	str = CL_PathHelp::get_basepath(cl_text("ABC/DEF"), CL_PathHelp::path_type_file);
#ifdef WIN32
	str2 = cl_text("ABC/"); 
#else
	str2 = cl_text("ABC/"); 
#endif
	if (str != str2) fail();

	str = CL_PathHelp::get_basepath(cl_text("/ABC\\DEF"), CL_PathHelp::path_type_file);
#ifdef WIN32
	str2 = cl_text("/ABC\\"); 
#else
	str2 = cl_text("/ABC\\"); 
#endif
	if (str != str2) fail();

	str = CL_PathHelp::get_basepath(cl_text("ABC"), CL_PathHelp::path_type_virtual);
	str2 = cl_text(""); 
	if (str != str2) fail();

	str = CL_PathHelp::get_basepath(cl_text("ABC/DEF"), CL_PathHelp::path_type_virtual);
#ifdef WIN32
	str2 = cl_text("ABC/"); 
#else
	str2 = cl_text("ABC/"); 
#endif
	if (str != str2) fail();

	str = CL_PathHelp::get_basepath(cl_text("/ABC\\DEF"), CL_PathHelp::path_type_virtual);
#ifdef WIN32
	str2 = cl_text("/"); 
#else
	str2 = cl_text("/"); 
#endif
	if (str != str2) fail();

//*** testing  split_basepath()
	CL_Console::write_line("   Function: std::vector<CL_String> split_basepath()");

	std::vector<CL_String> vstr = CL_PathHelp::split_basepath(cl_text("ABC/DEF\\GHI/JKL"), CL_PathHelp::path_type_file);
	if (vstr.size() != 3) fail();
	if (vstr[0] != cl_text("ABC")) fail();
	if (vstr[1] != cl_text("DEF")) fail();
	if (vstr[2] != cl_text("GHI")) fail();

	vstr = CL_PathHelp::split_basepath(cl_text("ABC/DEF\\GHI/JKL"), CL_PathHelp::path_type_virtual);
	if (vstr.size() != 2) fail();
	if (vstr[0] != cl_text("ABC")) fail();
	if (vstr[1] != cl_text("DEF\\GHI")) fail();

//*** testing get_fullpath()
	CL_Console::write_line("   Function: CL_String get_fullpath()");

	str = CL_PathHelp::get_fullpath(cl_text("ABC"), CL_PathHelp::path_type_file);
	str2 = cl_text(""); 
	if (str != str2) fail();

	str = CL_PathHelp::get_fullpath(cl_text("ABC/DEF"), CL_PathHelp::path_type_file);
#ifdef WIN32
	str2 = cl_text("ABC/"); 
#else
	str2 = cl_text("ABC/"); 
#endif
	if (str != str2) fail();

	str = CL_PathHelp::get_fullpath(cl_text("/ABC\\DEF"), CL_PathHelp::path_type_file);
#ifdef WIN32
	str2 = cl_text("/ABC\\"); 
#else
	str2 = cl_text("/ABC\\"); 
#endif
	if (str != str2) fail();

	str = CL_PathHelp::get_fullpath(cl_text("ABC"), CL_PathHelp::path_type_virtual);
	str2 = cl_text(""); 
	if (str != str2) fail();

	str = CL_PathHelp::get_fullpath(cl_text("ABC/DEF"), CL_PathHelp::path_type_virtual);
	str2 = cl_text("ABC/"); 
	if (str != str2) fail();

	str = CL_PathHelp::get_fullpath(cl_text("/ABC\\DEF"), CL_PathHelp::path_type_virtual);
	str2 = cl_text("/"); 
	if (str != str2) fail();

//*** testing get_filename()
	CL_Console::write_line("   Function: CL_String get_filename()");

	str = CL_PathHelp::get_filename(cl_text("ABC/DEF\\HIJ.KLM"), CL_PathHelp::path_type_file);
	str2 = cl_text("HIJ.KLM"); 
	if (str != str2) fail();

	str = CL_PathHelp::get_filename(cl_text("ABC/DEF\\HIJ.KLM"), CL_PathHelp::path_type_virtual);
	str2 = cl_text("DEF\\HIJ.KLM"); 
	if (str != str2) fail();

//*** testing get_basename()
	CL_Console::write_line("   Function: CL_String get_basename()");

	str = CL_PathHelp::get_basename(cl_text("ABC/DEF\\HIJ.KLM"), CL_PathHelp::path_type_file);
	str2 = cl_text("HIJ"); 
	if (str != str2) fail();

	str = CL_PathHelp::get_basename(cl_text("ABC/DEF\\.KLM"), CL_PathHelp::path_type_file);
#ifdef WIN32
	str2 = cl_text(""); 
#else
	str2 = cl_text(".KLM"); 
#endif
	if (str != str2) fail();

	str = CL_PathHelp::get_basename(cl_text("ABC/DEF\\HIJ.KLM"), CL_PathHelp::path_type_virtual);
	str2 = cl_text("DEF\\HIJ"); 
	if (str != str2) fail();

	str = CL_PathHelp::get_basename(cl_text("ABC/.DEF\\X"), CL_PathHelp::path_type_virtual);
#ifdef WIN32
	str2 = cl_text(".DEF\\X"); 
#else
	str2 = cl_text(".DEF\\X"); 
#endif
	if (str != str2) fail();

//*** testing get_extension()
	CL_Console::write_line("   Function: CL_String get_extension()");

	str = CL_PathHelp::get_extension(cl_text("ABC/DEF\\HIJ.KLM"), CL_PathHelp::path_type_file);
	str2 = cl_text("KLM"); 
	if (str != str2) fail();

	str = CL_PathHelp::get_extension(cl_text("ABC/DEF\\.KLM"), CL_PathHelp::path_type_file);
#ifdef WIN32
	str2 = cl_text("KLM"); 
#else
	str2 = cl_text(""); 
#endif
	if (str != str2) fail();

	str = CL_PathHelp::get_extension(cl_text("ABC/DEF\\HIJ.KLM"), CL_PathHelp::path_type_virtual);
	str2 = cl_text("KLM"); 
	if (str != str2) fail();

	str = CL_PathHelp::get_extension(cl_text("ABC/.DEF\\X"), CL_PathHelp::path_type_virtual);
	str2 = cl_text(""); 
	if (str != str2) fail();

//*** testing get_fullname(#1)
	CL_Console::write_line("   Function: CL_String get_fullname(#1)");

	str = CL_PathHelp::get_fullname(cl_text("ABC"), cl_text("DEF"), CL_PathHelp::path_type_file);
#ifdef WIN32
	str2 = cl_text("ABC\\DEF"); 
#else
	str2 = cl_text("ABC/DEF"); 
#endif
	if (str != str2) fail();

	str = CL_PathHelp::get_fullname(cl_text("ABC"), cl_text("DEF"), CL_PathHelp::path_type_virtual);
	str2 = cl_text("ABC/DEF"); 
	if (str != str2) fail();

//*** testing get_fullname(#2)
	CL_Console::write_line("   Function: CL_String get_fullname(#2)");

	str = CL_PathHelp::get_fullname(cl_text("ABC"), cl_text("DEF"), cl_text("GHI"), CL_PathHelp::path_type_file);
#ifdef WIN32
	str2 = cl_text("ABC\\DEF.GHI"); 
#else
	str2 = cl_text("ABC/DEF.GHI"); 
#endif
	if (str != str2) fail();

	str = CL_PathHelp::get_fullname(cl_text("ABC"), cl_text("DEF"), cl_text("GHI"), CL_PathHelp::path_type_virtual);
	str2 = cl_text("ABC/DEF.GHI"); 
	if (str != str2) fail();

//*** testing get_fullname(#3)
	CL_Console::write_line("   Function: CL_String get_fullname(#3)");

	str = CL_PathHelp::get_fullname(cl_text("ABC"), cl_text("DEF"), cl_text("GHI"), cl_text("JKL"), CL_PathHelp::path_type_file);
#ifdef WIN32
	str2 = cl_text("ABCDEF\\GHI.JKL"); 
#else
	str2 = cl_text("ABCDEF/GHI.JKL"); 
#endif
	if (str != str2) fail();

	str = CL_PathHelp::get_fullname(cl_text("ABC"), cl_text("DEF"), cl_text("GHI"), cl_text("JKL"), CL_PathHelp::path_type_virtual);
	str2 = cl_text("ABCDEF/GHI.JKL"); 
	if (str != str2) fail();

}


