/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

void TestApp::test_vfs()
{
	Console::write_line(" Header: virtual_file_system.h");
	Console::write_line("  Class: VirtualFileSystem");

	test_vfs_internal("  File based #1", 1, VirtualFileSystem("../../", false));
	test_vfs_internal("  File based #2", 2, VirtualFileSystem("../../", false));
	test_vfs_internal("  Zip based #1", 1, VirtualFileSystem("../IOData/test.zip", true));
	test_vfs_internal("  Zip based #2", 2, VirtualFileSystem("test.zip", true));
}

void TestApp::test_vfs_internal(const char *message, int test_method, VirtualFileSystem vfs)
{
	Console::write_line(message);

	VirtualDirectoryListing dir;
	if (test_method==1)
	{
		dir = vfs.get_directory_listing("Core");
	}
	else
	{
		VirtualDirectory vdir = vfs.open_directory("Core");
		dir = vdir.get_directory_listing();
	}

	if (!dir.next())
		fail();

	while(true)
	{
		if (dir.get_filename() == "IOData")
			break;
		if (!dir.next())
			fail();
	}
	if (!dir.is_directory())
		fail();

	if (test_method==1)
	{
		dir = vfs.get_directory_listing("Core/IOData");
	}
	else
	{
		VirtualDirectory vdir = vfs.open_directory("Core/IOData");
		dir = vdir.get_directory_listing();
	}

	while(true)
	{
		if (dir.get_filename() == "Makefile")
			break;
		if (!dir.next())
			fail();
	}
	if (dir.is_directory())
		fail();	



}

