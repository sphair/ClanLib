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

class MyFileSource : public FileSystemProvider
{
public:
	MyFileSource(const std::string &newpath)
	{
		path = newpath;
	}
	virtual ~MyFileSource() {return;}

	IODevice open_file(const std::string &filename,
		File::OpenMode mode = File::open_existing,
		unsigned int access = File::access_read | File::access_write,
		unsigned int share = File::share_all,
		unsigned int flags = 0)
	{
		fullname = path + filename;
		return IODevice();
	}

	//: Initiate directory listing.
	bool initialize_directory_listing(const std::string &path) { return false; }

	//: Update directory listing item.
	bool next_file(DirectoryListingEntry &entry) { return false; }

	//: Return the path of this file source.
	std::string get_path() const { return path; }

	//: Get the identifier of this file source
	std::string get_identifier() const { return path; }

private:
	std::string path;
public:
	static std::string fullname;
};
std::string MyFileSource::fullname;

void TestApp::test_virtual_directory_part2(void)
{

	Console::write_line("  (Using FileSystem)");

	std::string str;
	std::string cwd(working_dir);
	FileSystem vfs("../../Core/");

//*** testing get_directory_listing()
	Console::write_line("   Function: DirectoryListing get_directory_listing()");
	DirectoryListing listing = vfs.get_directory_listing("");
	do
	{
		if (!listing.next()) fail();
		str = listing.get_filename();
	}while(str != "IOData");

//*** testing open_file()
	Console::write_line("   Function: IODevice Directory::open_file()");
	if(true)
	{
		IODevice device = vfs.open_file("IOData/test_virtual_directory.cpp", File::open_existing, File::access_read, File::share_all, 0);
		char test_data[4];
		if (device.receive(test_data, 4, true) != 4) fail();
		if ( (test_data[0] != '/') || (test_data[1] != '*') ) fail();
	}


//*** testing mount(const std::string &mount_point, const std::string &path)
	Console::write_line("   Function: mount(const std::string &mount_point, const std::string &path)");
	if(true)
	{
		FileSystem vfs("somewhere");
		vfs.mount("ABC", "../../Core/IOData", false);
		IODevice device = vfs.open_file("ABC/test_virtual_directory.cpp", File::open_existing, File::access_read, File::share_all, 0);
	}

//*** testing mount(const std::string &mount_point, FileSystem &fs)
	Console::write_line("   Function: mount(const std::string &mount_point, FileSystem &fs)");
	if(true)
	{
		FileSystem new_vfs(new MyFileSource("Hello/"));
		IODevice device = new_vfs.open_file("ABC/World", File::open_existing, File::access_read, File::share_all, 0);
		if (MyFileSource::fullname != "Hello/ABC/World") fail();
	}

//*** testing unmount(const std::string &mount_point)
	Console::write_line("   Function: unmount(const std::string &mount_point)");
	if(true)
	{
		FileSystem vfs("./");
		vfs.mount("ABC", "../../Core/IOData", false);
		vfs.mount("DEF", "../../Core/IOData", false);
		vfs.mount("GHI", "../../Core/IOData", false);

		vfs.open_file("ABC/test_virtual_directory.cpp", File::open_existing, File::access_read, File::share_all, 0);
		vfs.open_file("DEF/test_virtual_directory.cpp", File::open_existing, File::access_read, File::share_all, 0);
		vfs.open_file("GHI/test_virtual_directory.cpp", File::open_existing, File::access_read, File::share_all, 0);
		vfs.unmount("DEF");
		vfs.unmount("WWW");
		vfs.open_file("ABC/test_virtual_directory.cpp", File::open_existing, File::access_read, File::share_all, 0);
		vfs.open_file("GHI/test_virtual_directory.cpp", File::open_existing, File::access_read, File::share_all, 0);

		try
		{
			vfs.open_file("DEF/test_virtual_directory.cpp", File::open_existing, File::access_read, File::share_all, 0);
			fail();	// Exception should have been called
		}
		catch (Exception errror)
		{
		}
		vfs.unmount("ABC");
		vfs.unmount("GHI");
		try
		{
			vfs.open_file("ABC/test_virtual_directory.cpp", File::open_existing, File::access_read, File::share_all, 0);
			vfs.open_file("GHI/test_virtual_directory.cpp", File::open_existing, File::access_read, File::share_all, 0);

			fail();	// Exception should have been called
		}
		catch (Exception errror)
		{
		}
		vfs.open_file("test_virtual_directory.cpp", File::open_existing, File::access_read, File::share_all, 0);

	}

}

