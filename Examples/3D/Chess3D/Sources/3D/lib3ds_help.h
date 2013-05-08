/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
**    Mark Page
*/
#pragma once

#include <lib3ds.h>

/////////////////////////////////////////////////////////////////////////////

class Lib3dsIo
{
public:
	Lib3dsIo(IODevice device);
	~Lib3dsIo();

public:
	Lib3dsIo *get_io() { return &io; }
	operator Lib3dsIo *() { return get_io(); }

private:
	Lib3dsIo(const Lib3dsIo &src) { }
	Lib3dsIo &operator =(const Lib3dsIo &src) { return *this; }

	static void io_log_func(void *self, Lib3dsLogLevel level, int indent, const char *msg);
	static long io_seek_func(void *self, long offset, Lib3dsIoSeek origin);
	static long io_tell_func(void *self);
	static size_t io_read_func(void *self, void *buffer, size_t size);
	static size_t io_write_func(void *self, const void *buffer, size_t size);

	Lib3dsIo io;
	IODevice device;
};

/////////////////////////////////////////////////////////////////////////////

struct Lib3dsMesh
{
	std::vector<int> face_materials; // -1 = no material
	std::vector<Vec3f> positions;
	std::vector<Vec2f> texcooords;
	std::vector<Vec3f> normals;
};

class Lib3dsFile
{
public:
	Lib3dsFile();
	~Lib3dsFile();

public:
	Lib3dsFile *get_file() { return file; }
	void load(IODevice device);
	void save(IODevice device);
	std::vector<Lib3dsMesh> export_meshes(Lib3dsNode *node = 0);

	Lib3dsFile *operator ->() { return get_file(); }
	operator Lib3dsFile *() { return get_file(); }

private:
	Lib3dsFile *file;
};

/////////////////////////////////////////////////////////////////////////////

#if defined (_MSC_VER)
	#if !defined (_DEBUG)
		#if defined(_M_X64)
			#pragma comment(lib, "lib3ds-x64-static-mt.lib")
		#else
			#pragma comment(lib, "lib3ds-static-mt.lib")
		#endif
	#else
		#if defined(_M_X64)
			#pragma comment(lib, "lib3ds-x64-static-mt-debug.lib")
		#else
			#pragma comment(lib, "lib3ds-static-mt-debug.lib")
		#endif
	#endif
#endif
