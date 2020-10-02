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

void TestApp::test_file_help(void)
{
	FILE *fptr;
	Console::write_line(" Header: file_help.h");
	Console::write_line("  Class: FileHelp");

	Console::write_line("   Function: copy_file()");

	// Remove the files first (if they exist)
	try
	{
		FileHelp::delete_file("temporary_file.tmp");
	}
	catch(Exception)
	{
	}

	try
	{
		FileHelp::delete_file("temporary_file2.tmp");
	}
	catch(Exception)
	{
	}
//*** testing copy_file()
	FileHelp::copy_file("test.cpp", "temporary_file.tmp", false);
	fptr = fopen("temporary_file.tmp", "rb");
	if (!fptr) fail();
	fclose(fptr);

	try
	{
		FileHelp::copy_file("test.cpp", "temporary_file.tmp", false);
		// Failed because an exception should have been thrown
		fail();	// Already exists
	}
	catch(Exception)
	{
	}

	FileHelp::copy_file("test.cpp", "temporary_file.tmp", true);
	fptr = fopen("temporary_file.tmp", "rb");
	if (!fptr) fail();
	fclose(fptr);

	FileHelp::copy_file("test.cpp", "temporary_file2.tmp", true);
	fptr = fopen("temporary_file2.tmp", "rb");
	if (!fptr) fail();
	fclose(fptr);

//*** testing delete_file()
	Console::write_line("   Function: delete_file()");
	FileHelp::delete_file("temporary_file.tmp");
	FileHelp::delete_file("temporary_file2.tmp");
	fptr = fopen("temporary_file.tmp", "rb");
	if (fptr)
	{
		fclose(fptr);
		fail();
	}
}

