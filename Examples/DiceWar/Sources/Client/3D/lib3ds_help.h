
#pragma once

#include <lib3ds/io.h>
#include <lib3ds/file.h>

/////////////////////////////////////////////////////////////////////////////

class CL_Lib3dsIo
{
public:
	CL_Lib3dsIo(CL_IODevice device);

	~CL_Lib3dsIo();

public:
	Lib3dsIo *get_io() { return io; }

	operator Lib3dsIo *() { return get_io(); }

private:
	CL_Lib3dsIo(const CL_Lib3dsIo &src) { }
	CL_Lib3dsIo &operator =(const CL_Lib3dsIo &src) { return *this; }

	static Lib3dsBool io_error_func(void *self);
	static long io_seek_func(void *self, long offset, Lib3dsIoSeek origin);
	static long io_tell_func(void *self);
	static size_t io_read_func(void *self, void *buffer, size_t size);
	static size_t io_write_func(void *self, const void *buffer, size_t size);

	Lib3dsIo *io;
	CL_IODevice device;
};

/////////////////////////////////////////////////////////////////////////////

class CL_Lib3dsFile
{
public:
	CL_Lib3dsFile();

	~CL_Lib3dsFile();

public:
	Lib3dsFile *get_file() { return file; }

	void load(CL_IODevice device);

	void save(CL_IODevice device);

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
