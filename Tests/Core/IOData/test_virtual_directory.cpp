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

class MyFileSource : public VirtualFileSource
{
public:
	MyFileSource(const std::string &newpath)
	{
		path = newpath;
	}
	virtual ~MyFileSource() {return;}

	virtual IODevice open_file(const std::string &filename,
		File::OpenMode mode = File::open_existing,
		unsigned int access = File::access_read | File::access_write,
		unsigned int share = File::share_all,
		unsigned int flags = 0)
	{
		fullname = path + filename;
		return IODevice();
	}

	//: Initiate directory listing.
	virtual bool initialize_directory_listing(const std::string &path) { return false; }

	//: Update directory listing item.
	virtual bool next_file(VirtualDirectoryListingEntry &entry) { return false; }

	//: Return the path of this file source.
	virtual std::string get_path() const { return path; }

	//: Get the identifier of this file source
	virtual std::string get_identifier() const { return path; }

private:
	std::string path;
public:
	static std::string fullname;
};
std::string MyFileSource::fullname;

void TestApp::test_virtual_directory_part1(void)
{
	Console::write_line(" Header: virtual_directory.h");
	Console::write_line("  Class: VirtualDirectory");
	Console::write_line("  (Without using VirtualFileSystem)");

	std::string str;
	std::string cwd(working_dir);
	VirtualDirectory dir;

//*** testing get_file_system()
	Console::write_line("   Function: VirtualFileSystem get_file_system()");
	VirtualFileSystem vfs = dir.get_file_system();
	if (!vfs.is_null()) return fail();

//*** testing get_path()
	Console::write_line("   Function: std::string &get_path()");
	std::string strpath = dir.get_path();
	if (strpath != cwd) fail();

//*** testing get_directory_listing()
	Console::write_line("   Function: VirtualDirectoryListing get_directory_listing()");
	try
	{
		VirtualDirectoryListing listing = dir.get_directory_listing();
		fail();	// Exception should have been called
	}
	catch (Exception errror)
	{
	}

//*** testing open_directory()
	Console::write_line("   Function: VirtualDirectory VirtualDirectory::open_directory(const std::string &path)");
	VirtualDirectory dir2 = dir.open_directory("ABC/DEF");
	strpath = dir2.get_path();
#ifdef WIN32
	str = cwd + "\\ABC\\DEF\\";
#else
	str = cwd + "/ABC/DEF/";
#endif
	if (str != strpath) fail();

//*** testing open_file()
	Console::write_line("   Function: IODevice VirtualDirectory::open_file()");
	if(true)
	{
		IODevice device = dir.open_file("../IOData/test_virtual_directory.cpp", File::open_existing, File::access_read, File::share_all, 0);
		char test_data[4];
		if (device.receive(test_data, 4, true) != 4) fail();
		if ( (test_data[0] != '/') || (test_data[1] != '*') ) fail();
	}

//*** testing make_path_absolute()
	Console::write_line("   Function: std::string make_path_absolute(const std::string &relative_path)");
	str = dir.make_path_absolute("../IOData/ABC/DEF");
	strpath = dir2.get_path();
#ifdef WIN32
	str = cwd + "\\ABC\\DEF\\";
#else
	str = cwd + "/ABC/DEF/";
#endif
	if (str != strpath) fail();

//*** testing make_path_relative()
	Console::write_line("   Function: std::string make_path_relative(const std::string &absolute_path)");
	std::string str2 = cwd + "/ABC/DEF";
	str = dir.make_path_relative(str2);
#ifdef WIN32
	if (str != "ABC\\DEF") fail();
#else
	if (str != "ABC/DEF") fail();
#endif

//*** testing mount(const std::string &mount_point, VirtualFileSystem &fs)
	Console::write_line("   Function: mount(const std::string &mount_point, VirtualFileSystem &fs)");
	try
	{
		VirtualFileSystem fs;
		dir.mount("./", fs);
		fail();	// Exception should have been called
	}
	catch (Exception errror)
	{
	}

//*** testing mount(const std::string &mount_point, const std::string &path)
	Console::write_line("   Function: mount(const std::string &mount_point, const std::string &path)");
	try
	{
		dir.mount("../", "../");
		fail();	// Exception should have been called
	}
	catch (Exception errror)
	{
	}

//*** testing unmount(const std::string &mount_point)
	Console::write_line("   Function: unmount(const std::string &mount_point)");
	try
	{
		dir.unmount("../");
		fail();	// Exception should have been called
	}
	catch (Exception errror)
	{
	}
}

void TestApp::test_virtual_directory_part2(void)
{

	Console::write_line("  (Using VirtualFileSystem)");

	std::string str;
	std::string cwd(working_dir);
	VirtualFileSystem vfs("../../");
	VirtualDirectory dir(vfs, "Core/IOData");

//*** testing get_file_system()
	Console::write_line("   Function: VirtualFileSystem get_file_system()");
	VirtualFileSystem vfs_test = dir.get_file_system();
	if (vfs_test.is_null()) fail();

//*** testing get_path()
	Console::write_line("   Function: std::string &get_path()");
	std::string strpath = dir.get_path();
	if (strpath != "Core/IOData/") fail();

//*** testing get_directory_listing()
	Console::write_line("   Function: VirtualDirectoryListing get_directory_listing()");
	VirtualDirectoryListing listing = dir.get_directory_listing();
	do
	{
		if (!listing.next()) fail();
		str = listing.get_filename();
	}while(str != "test_virtual_directory.cpp");

//*** testing open_directory()
	Console::write_line("   Function: VirtualDirectory VirtualDirectory::open_directory(const std::string &path)");
	VirtualDirectory dir2 = dir.open_directory("ABC/DEF");
	strpath = dir2.get_path();
	if ("Core/IOData/ABC/DEF/" != strpath) fail();

//*** testing open_file()
	Console::write_line("   Function: IODevice VirtualDirectory::open_file()");
	if(true)
	{
		IODevice device = dir.open_file("../IOData/test_virtual_directory.cpp", File::open_existing, File::access_read, File::share_all, 0);
		char test_data[4];
		if (device.receive(test_data, 4, true) != 4) fail();
		if ( (test_data[0] != '/') || (test_data[1] != '*') ) fail();
	}

//*** testing make_path_absolute()
	Console::write_line("   Function: std::string make_path_absolute(const std::string &relative_path)");
	str = dir.make_path_absolute("../IOData/ABC/DEF");
	strpath = dir2.get_path();
#ifdef WIN32
	if (str != "Core/IOData/ABC/DEF") fail();
#else
	if (str != "Core/IOData/ABC/DEF") fail();
#endif


//*** testing make_path_relative()
	Console::write_line("   Function: std::string make_path_relative(const std::string &absolute_path)");
	VirtualDirectory dir3(vfs, "/Core/IOData");
	std::string str2 = "/Core/IOData/ABC/DEF";
	str = dir3.make_path_relative(str2);
#ifdef WIN32
	if (str != "ABC/DEF") fail();
#else
	if (str != "ABC/DEF") fail();
#endif

//*** testing mount(const std::string &mount_point, const std::string &path)
	Console::write_line("   Function: mount(const std::string &mount_point, const std::string &path)");
	if(true)
	{
		VirtualFileSystem vfs("somewhere");
		VirtualDirectory dir(vfs, "do_not_care");
		dir.mount("ABC", "../../Core/IOData");
		IODevice device = dir.open_file("ABC/test_virtual_directory.cpp", File::open_existing, File::access_read, File::share_all, 0);
	}

//*** testing mount(const std::string &mount_point, VirtualFileSystem &fs)
	Console::write_line("   Function: mount(const std::string &mount_point, VirtualFileSystem &fs)");
	if(true)
	{
		VirtualFileSystem vfs("somewhere");
		VirtualDirectory dir(vfs, "do_not_care");
		VirtualFileSystem new_vfs(new MyFileSource("Hello"));
		dir.mount("ABC", new_vfs);
		IODevice device = dir.open_file("ABC/World", File::open_existing, File::access_read, File::share_all, 0);
		if (MyFileSource::fullname != "HelloWorld") fail();
	}

//*** testing unmount(const std::string &mount_point)
	Console::write_line("   Function: unmount(const std::string &mount_point)");
	if(true)
	{
		VirtualFileSystem vfs("./");
		VirtualDirectory dir(vfs, "./");
		dir.mount("ABC", "../../Core/IOData");
		dir.mount("DEF", "../../Core/IOData");
		dir.mount("GHI", "../../Core/IOData");

		dir.open_file("ABC/test_virtual_directory.cpp", File::open_existing, File::access_read, File::share_all, 0);
		dir.open_file("DEF/test_virtual_directory.cpp", File::open_existing, File::access_read, File::share_all, 0);
		dir.open_file("GHI/test_virtual_directory.cpp", File::open_existing, File::access_read, File::share_all, 0);
		dir.unmount("DEF");
		dir.unmount("WWW");
		dir.open_file("ABC/test_virtual_directory.cpp", File::open_existing, File::access_read, File::share_all, 0);
		dir.open_file("GHI/test_virtual_directory.cpp", File::open_existing, File::access_read, File::share_all, 0);

		try
		{
			dir.open_file("DEF/test_virtual_directory.cpp", File::open_existing, File::access_read, File::share_all, 0);
			fail();	// Exception should have been called
		}
		catch (Exception errror)
		{
		}
		dir.unmount("ABC");
		dir.unmount("GHI");
		try
		{
			dir.open_file("ABC/test_virtual_directory.cpp", File::open_existing, File::access_read, File::share_all, 0);
			dir.open_file("GHI/test_virtual_directory.cpp", File::open_existing, File::access_read, File::share_all, 0);

			fail();	// Exception should have been called
		}
		catch (Exception errror)
		{
		}
		dir.open_file("test_virtual_directory.cpp", File::open_existing, File::access_read, File::share_all, 0);

	}

}

