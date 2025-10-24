
#include "precomp.h"
#include "lib3ds_help.h"

/////////////////////////////////////////////////////////////////////////////

CL_Lib3dsIo::CL_Lib3dsIo(CL_IODevice device)
: io(0), device(device)
{
	io = lib3ds_io_new(
		this,
		&CL_Lib3dsIo::io_error_func,
		&CL_Lib3dsIo::io_seek_func,
		&CL_Lib3dsIo::io_tell_func,
		&CL_Lib3dsIo::io_read_func,
		&CL_Lib3dsIo::io_write_func);
}

CL_Lib3dsIo::~CL_Lib3dsIo()
{
	lib3ds_io_free(io);
}

Lib3dsBool CL_Lib3dsIo::io_error_func(void *self)
{
	return LIB3DS_FALSE;
}

long CL_Lib3dsIo::io_seek_func(void *_self, long offset, Lib3dsIoSeek origin)
{
	CL_Lib3dsIo *self = (CL_Lib3dsIo *) _self;
	switch (origin)
	{
	case LIB3DS_SEEK_SET:
		return self->device.seek(offset, CL_IODevice::seek_set);
	case LIB3DS_SEEK_CUR:
		return self->device.seek(offset, CL_IODevice::seek_cur);
	case LIB3DS_SEEK_END:
		return self->device.seek(offset, CL_IODevice::seek_end);
	}
	return 0;
}

long CL_Lib3dsIo::io_tell_func(void *_self)
{
	CL_Lib3dsIo *self = (CL_Lib3dsIo *) _self;
	return self->device.get_position();
}

size_t CL_Lib3dsIo::io_read_func(void *_self, void *buffer, size_t size)
{
	CL_Lib3dsIo *self = (CL_Lib3dsIo *) _self;
	return self->device.read(buffer, size, false);
}

size_t CL_Lib3dsIo::io_write_func(void *_self, const void *buffer, size_t size)
{
	CL_Lib3dsIo *self = (CL_Lib3dsIo *) _self;
	return self->device.write(buffer, size, false);
}

/////////////////////////////////////////////////////////////////////////////

CL_Lib3dsFile::CL_Lib3dsFile()
: file(0)
{
	file = lib3ds_file_new();
}

CL_Lib3dsFile::~CL_Lib3dsFile()
{
	lib3ds_file_free(file);
}

void CL_Lib3dsFile::load(CL_IODevice device)
{
	CL_Lib3dsIo io(device);
	Lib3dsBool result = lib3ds_file_read(file, io);
	if (result != LIB3DS_TRUE)
		throw CL_Exception(cl_text("Unable to load 3ds file"));
}

void CL_Lib3dsFile::save(CL_IODevice device)
{
	CL_Lib3dsIo io(device);
	Lib3dsBool result = lib3ds_file_write(file, io);
	if (result != LIB3DS_TRUE)
		throw CL_Exception(cl_text("Unable to save 3ds file"));
}
