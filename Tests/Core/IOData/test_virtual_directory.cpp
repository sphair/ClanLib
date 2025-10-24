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
**    Mark Page
**    (if your name is missing here, please add it)
*/

#include "test.h"

class MyFileSource : public CL_VirtualFileSource
{
public:
	MyFileSource(const CL_String &newpath)
	{
		path = newpath;
	}
	virtual ~MyFileSource() {return;}

	virtual CL_IODevice open_file(const CL_String &filename,
		CL_File::OpenMode mode = CL_File::open_existing,
		unsigned int access = CL_File::access_read | CL_File::access_write,
		unsigned int share = CL_File::share_all,
		unsigned int flags = 0)
	{
		fullname = path + filename;
		return CL_IODevice();
	}

	//: Initiate directory listing.
	virtual bool initialize_directory_listing(const CL_String &path) { return false; }

	//: Update directory listing item.
	virtual bool next_file(CL_VirtualDirectoryListingEntry &entry) { return false; }

	//: Return the path of this file source.
	virtual CL_String get_path() const { return path; }

	//: Get the identifier of this file source
	virtual CL_String get_identifier() const { return path; }

private:
	CL_String path;
public:
	static CL_String fullname;
};
CL_String MyFileSource::fullname;

void TestApp::test_virtual_directory_part1(void)
{
	CL_Console::write_line(" Header: virtual_directory.h");
	CL_Console::write_line("  Class: CL_VirtualDirectory");
	CL_Console::write_line("  (Without using CL_VirtualFileSystem)");

	CL_String str;
	CL_String cwd(working_dir);
	CL_VirtualDirectory dir;

//*** testing get_file_system()
	CL_Console::write_line("   Function: CL_VirtualFileSystem get_file_system()");
	CL_VirtualFileSystem vfs = dir.get_file_system();
	if (!vfs.is_null()) return fail();

//*** testing get_path()
	CL_Console::write_line("   Function: CL_String &get_path()");
	CL_String strpath = dir.get_path();
	if (strpath != cwd) fail();

//*** testing get_directory_listing()
	CL_Console::write_line("   Function: CL_VirtualDirectoryListing get_directory_listing()");
	try
	{
		CL_VirtualDirectoryListing listing = dir.get_directory_listing();
		fail();	// Exception should have been called
	}
	catch (CL_Exception errror)
	{
	}

//*** testing open_directory()
	CL_Console::write_line("   Function: CL_VirtualDirectory CL_VirtualDirectory::open_directory(const CL_String &path)");
	CL_VirtualDirectory dir2 = dir.open_directory("ABC/DEF");
	strpath = dir2.get_path();
#ifdef WIN32
	str = cwd + "\\ABC\\DEF\\";
#else
	str = cwd + "/ABC/DEF/";
#endif
	if (str != strpath) fail();

//*** testing open_file()
	CL_Console::write_line("   Function: CL_IODevice CL_VirtualDirectory::open_file()");
	if(true)
	{
		CL_IODevice device = dir.open_file("../IOData/test_virtual_directory.cpp", CL_File::open_existing, CL_File::access_read, CL_File::share_all, 0);
		char test_data[4];
		if (device.receive(test_data, 4, true) != 4) fail();
		if ( (test_data[0] != '/') || (test_data[1] != '*') ) fail();
	}

//*** testing make_path_absolute()
	CL_Console::write_line("   Function: CL_String make_path_absolute(const CL_String &relative_path)");
	str = dir.make_path_absolute("../IOData/ABC/DEF");
	strpath = dir2.get_path();
#ifdef WIN32
	str = cwd + "\\ABC\\DEF\\";
#else
	str = cwd + "/ABC/DEF/";
#endif
	if (str != strpath) fail();

//*** testing make_path_relative()
	CL_Console::write_line("   Function: CL_String make_path_relative(const CL_String &absolute_path)");
	CL_String str2 = cwd + "/ABC/DEF";
	str = dir.make_path_relative(str2);
#ifdef WIN32
	if (str != "ABC\\DEF") fail();
#else
	if (str != "ABC/DEF") fail();
#endif

//*** testing mount(const CL_String &mount_point, CL_VirtualFileSystem &fs)
	CL_Console::write_line("   Function: mount(const CL_String &mount_point, CL_VirtualFileSystem &fs)");
	try
	{
		CL_VirtualFileSystem fs;
		dir.mount("./", fs);
		fail();	// Exception should have been called
	}
	catch (CL_Exception errror)
	{
	}

//*** testing mount(const CL_String &mount_point, const CL_String &path)
	CL_Console::write_line("   Function: mount(const CL_String &mount_point, const CL_String &path)");
	try
	{
		dir.mount("../", "../");
		fail();	// Exception should have been called
	}
	catch (CL_Exception errror)
	{
	}

//*** testing unmount(const CL_String &mount_point)
	CL_Console::write_line("   Function: unmount(const CL_String &mount_point)");
	try
	{
		dir.unmount("../");
		fail();	// Exception should have been called
	}
	catch (CL_Exception errror)
	{
	}
}

void TestApp::test_virtual_directory_part2(void)
{

	CL_Console::write_line("  (Using CL_VirtualFileSystem)");

	CL_String str;
	CL_String cwd(working_dir);
	CL_VirtualFileSystem vfs("../../");
	CL_VirtualDirectory dir(vfs, "Core/IOData");

//*** testing get_file_system()
	CL_Console::write_line("   Function: CL_VirtualFileSystem get_file_system()");
	CL_VirtualFileSystem vfs_test = dir.get_file_system();
	if (vfs_test.is_null()) fail();

//*** testing get_path()
	CL_Console::write_line("   Function: CL_String &get_path()");
	CL_String strpath = dir.get_path();
	if (strpath != "Core/IOData/") fail();

//*** testing get_directory_listing()
	CL_Console::write_line("   Function: CL_VirtualDirectoryListing get_directory_listing()");
	CL_VirtualDirectoryListing listing = dir.get_directory_listing();
	do
	{
		if (!listing.next()) fail();
		str = listing.get_filename();
	}while(str != "test_virtual_directory.cpp");

//*** testing open_directory()
	CL_Console::write_line("   Function: CL_VirtualDirectory CL_VirtualDirectory::open_directory(const CL_String &path)");
	CL_VirtualDirectory dir2 = dir.open_directory("ABC/DEF");
	strpath = dir2.get_path();
	if ("Core/IOData/ABC/DEF/" != strpath) fail();

//*** testing open_file()
	CL_Console::write_line("   Function: CL_IODevice CL_VirtualDirectory::open_file()");
	if(true)
	{
		CL_IODevice device = dir.open_file("../IOData/test_virtual_directory.cpp", CL_File::open_existing, CL_File::access_read, CL_File::share_all, 0);
		char test_data[4];
		if (device.receive(test_data, 4, true) != 4) fail();
		if ( (test_data[0] != '/') || (test_data[1] != '*') ) fail();
	}

//*** testing make_path_absolute()
	CL_Console::write_line("   Function: CL_String make_path_absolute(const CL_String &relative_path)");
	str = dir.make_path_absolute("../IOData/ABC/DEF");
	strpath = dir2.get_path();
#ifdef WIN32
	if (str != "Core\\IOData\\ABC\\DEF") fail();
#else
	if (str != "Core/IOData/ABC/DEF") fail();
#endif


//*** testing make_path_relative()
	CL_Console::write_line("   Function: CL_String make_path_relative(const CL_String &absolute_path)");
	CL_VirtualDirectory dir3(vfs, "/Core/IOData");
	CL_String str2 = "/Core/IOData/ABC/DEF";
	str = dir3.make_path_relative(str2);
#ifdef WIN32
	if (str != "ABC\\DEF") fail();
#else
	if (str != "ABC/DEF") fail();
#endif

//*** testing mount(const CL_String &mount_point, const CL_String &path)
	CL_Console::write_line("   Function: mount(const CL_String &mount_point, const CL_String &path)");
	if(true)
	{
		CL_VirtualFileSystem vfs("somewhere");
		CL_VirtualDirectory dir(vfs, "do_not_care");
		dir.mount("ABC", "../../Core/IOData");
		CL_IODevice device = dir.open_file("ABC/test_virtual_directory.cpp", CL_File::open_existing, CL_File::access_read, CL_File::share_all, 0);
	}

//*** testing mount(const CL_String &mount_point, CL_VirtualFileSystem &fs)
	CL_Console::write_line("   Function: mount(const CL_String &mount_point, CL_VirtualFileSystem &fs)");
	if(true)
	{
		CL_VirtualFileSystem vfs("somewhere");
		CL_VirtualDirectory dir(vfs, "do_not_care");
		CL_VirtualFileSystem new_vfs(new MyFileSource("Hello"));
		dir.mount("ABC", new_vfs);
		CL_IODevice device = dir.open_file("ABC/World", CL_File::open_existing, CL_File::access_read, CL_File::share_all, 0);
		if (MyFileSource::fullname != "HelloWorld") fail();
	}

//*** testing unmount(const CL_String &mount_point)
	CL_Console::write_line("   Function: unmount(const CL_String &mount_point)");
	if(true)
	{
		CL_VirtualFileSystem vfs("./");
		CL_VirtualDirectory dir(vfs, "./");
		dir.mount("ABC", "../../Core/IOData");
		dir.mount("DEF", "../../Core/IOData");
		dir.mount("GHI", "../../Core/IOData");

		dir.open_file("ABC/test_virtual_directory.cpp", CL_File::open_existing, CL_File::access_read, CL_File::share_all, 0);
		dir.open_file("DEF/test_virtual_directory.cpp", CL_File::open_existing, CL_File::access_read, CL_File::share_all, 0);
		dir.open_file("GHI/test_virtual_directory.cpp", CL_File::open_existing, CL_File::access_read, CL_File::share_all, 0);
		dir.unmount("DEF");
		dir.unmount("WWW");
		dir.open_file("ABC/test_virtual_directory.cpp", CL_File::open_existing, CL_File::access_read, CL_File::share_all, 0);
		dir.open_file("GHI/test_virtual_directory.cpp", CL_File::open_existing, CL_File::access_read, CL_File::share_all, 0);

		try
		{
			dir.open_file("DEF/test_virtual_directory.cpp", CL_File::open_existing, CL_File::access_read, CL_File::share_all, 0);
			fail();	// Exception should have been called
		}
		catch (CL_Exception errror)
		{
		}
		dir.unmount("ABC");
		dir.unmount("GHI");
		try
		{
			dir.open_file("ABC/test_virtual_directory.cpp", CL_File::open_existing, CL_File::access_read, CL_File::share_all, 0);
			dir.open_file("GHI/test_virtual_directory.cpp", CL_File::open_existing, CL_File::access_read, CL_File::share_all, 0);

			fail();	// Exception should have been called
		}
		catch (CL_Exception errror)
		{
		}
		dir.open_file("test_virtual_directory.cpp", CL_File::open_existing, CL_File::access_read, CL_File::share_all, 0);

	}

}

