/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
*/

#include "test.h"

// This is the Program class that is called by CL_ClanApplication
class Program
{
public:
	static int main(const std::vector<CL_String> &args)
	{
		// Initialize ClanLib base components
		CL_SetupCore setup_core;

		// Start the Application
		TestApp app;
		int retval = app.main(args);
		return retval;
	}
};

// Instantiate CL_ClanApplication, informing it where the Program is located
CL_ClanApplication app(&Program::main);

int TestApp::main(const std::vector<CL_String> &args)
{
	CL_ConsoleWindow console("Console");

	try
	{
		run_test();
		console.display_close_message();
	}
	catch(CL_Exception error)
	{
		CL_Console::write_line("Unhandled exception: %1", error.message);
		console.display_close_message();
		return -1;
	}

	return 0;
}

void TestApp::run_test()
{
	CL_File file("ZipWriter.zip", CL_File::create_always, CL_File::access_write);
	CL_ZipWriter zip_writer(file);
	zip_writer.begin_file("file1.txt", true);
	zip_writer.write_file_data("12345678\r\n", 10);
	zip_writer.write_file_data("12345678\r\n", 10);
	zip_writer.write_file_data("12345678\r\n", 10);
	zip_writer.write_file_data("12345678\r\n", 10);
	zip_writer.write_file_data("12345678\r\n", 10);
	zip_writer.end_file();
	CL_String s = L"file2";
	s.append(1, 0xc6); // silly danish char
	zip_writer.begin_file(s, false);
	zip_writer.write_file_data("ClanLib Zipping!", 16);
	zip_writer.end_file();
	zip_writer.write_toc();
	CL_Console::write_line("Second filename: %1", s);
	file.close();

	CL_Console::write_line("");

	file = CL_File("ZipWriter.zip", CL_File::open_existing, CL_File::access_read);
	CL_ZipReader zip_reader(file);
	while (zip_reader.read_local_file_header())
	{
		CL_Console::write_line("File: %1", zip_reader.get_filename());
		CL_DataBuffer buffer(zip_reader.get_uncompressed_size());
		zip_reader.read_file_data(buffer.get_data(), buffer.get_size());
		CL_StringRef8 str8(buffer.get_data(), buffer.get_size(), false);
		CL_Console::write_line("Contents: %1", CL_StringHelp::utf8_to_text(str8));
	}
}
