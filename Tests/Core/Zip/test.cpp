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
*/

#include "test.h"

int main(int argc, char** argv)
{
	TestApp program;
	program.main();
}

int TestApp::main()
{
	ConsoleWindow console("Console");

	try
	{
		run_test();
		console.display_close_message();
	}
	catch(Exception error)
	{
		Console::write_line("Unhandled exception: %1", error.message);
		console.display_close_message();
		return -1;
	}

	return 0;
}

void TestApp::run_test()
{
	File file("ZipWriter.zip", File::create_always, File::access_write);
	ZipWriter zip_writer(file);
	zip_writer.begin_file("file1.txt", true);
	zip_writer.write_file_data("12345678\r\n", 10);
	zip_writer.write_file_data("12345678\r\n", 10);
	zip_writer.write_file_data("12345678\r\n", 10);
	zip_writer.write_file_data("12345678\r\n", 10);
	zip_writer.write_file_data("12345678\r\n", 10);
	zip_writer.end_file();
	std::string s = "file2";
	s.append(1, 0xc6); // silly danish char
	zip_writer.begin_file(s, false);
	zip_writer.write_file_data("ClanLib Zipping!", 16);
	zip_writer.end_file();
	zip_writer.write_toc();
	Console::write_line("Second filename: %1", s);
	file.close();

	Console::write_line("");

	file = File("ZipWriter.zip", File::open_existing, File::access_read);
	ZipReader zip_reader(file);
	while (zip_reader.read_local_file_header())
	{
		Console::write_line("File: %1", zip_reader.get_filename());
		DataBuffer buffer(zip_reader.get_uncompressed_size());
		zip_reader.read_file_data(buffer.get_data(), buffer.get_size());
		std::string str8(buffer.get_data(), buffer.get_size());
		Console::write_line("Contents: %1", StringHelp::utf8_to_text(str8));
	}
}
