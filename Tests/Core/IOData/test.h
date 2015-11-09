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
**    Mark Page
**    (if your name is missing here, please add it)
*/

#include <ClanLib/core.h>
using namespace clan;

#ifndef WIN32
#include <unistd.h>
#include <stdio.h>
#ifndef MAX_PATH
#define MAX_PATH PATH_MAX
#endif
#else
#include <direct.h>
#ifndef chdir
#define _chdir chdir
#endif
#ifndef MAX_PATH
#define _MAX_PATH MAX_PATH
#endif
#include <tchar.h>
#endif


#ifndef WIN32
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

class TestApp
{
public:
	int main();
private:
	void test_endian(void);
	void test_path_help(void);
	void test_file_help(void);
	void test_datatypes(void);
	void test_directory_scanner(void);
	void test_iodevice_memory(void);
	void test_iodevice(void);
	void test_virtual_directory_part2(void);
	void fail(void);
	void test_vfs();
	void test_vfs_internal(const char *message, FileSystem vfs);
#ifdef WIN32
	TCHAR working_dir[MAX_PATH];
	char location_dir[4];
#else
	char working_dir[1024];
#endif
};

