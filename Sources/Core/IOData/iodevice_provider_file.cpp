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
**    Magnus Norddahl
**    Harry Storbacka
*/

#include "Core/precomp.h"
#include "API/Core/IOData/file.h"
#include "API/Core/IOData/security_descriptor.h"
#include "API/Core/System/exception.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "iodevice_provider_file.h"
#ifndef WIN32
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

#ifdef WIN32
#define invalid_handle INVALID_HANDLE_VALUE
#else
#define invalid_handle -1
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_IODeviceProvider_File Construction:

CL_IODeviceProvider_File::CL_IODeviceProvider_File()
: handle(invalid_handle), peeked_data(0)
{
}

CL_IODeviceProvider_File::CL_IODeviceProvider_File(
	const CL_String &filename,
	CL_File::OpenMode open_mode,
	unsigned int access,
	unsigned int share,
	unsigned int flags)
: handle(invalid_handle), peeked_data(0)
{
	bool result = open(filename, open_mode, access, share, flags);
	if (result == false)
		throw CL_Exception(cl_format("CL_IODeviceProvider_File::CL_IODeviceProvider_File(): Unable to open file '%1'", filename));
}

CL_IODeviceProvider_File::CL_IODeviceProvider_File(
	const CL_String &filename,
	CL_File::OpenMode mode,
	const CL_SecurityDescriptor &permissions,
	unsigned int access,
	unsigned int share,
	unsigned int flags)
: handle(invalid_handle), peeked_data(0)
{
	bool result = open(filename, mode, permissions, access, share, flags);
	if (result == false)
		throw CL_Exception("CL_IODeviceProvider_File::CL_IODeviceProvider_File(): Unable to open file");
}

CL_IODeviceProvider_File::~CL_IODeviceProvider_File()
{
	close();
}

/////////////////////////////////////////////////////////////////////////////
// CL_IODeviceProvider_File Attributes:

int CL_IODeviceProvider_File::get_size() const
{
#ifdef WIN32
	if (handle == invalid_handle)
		throw CL_Exception("CL_IODeviceProvider_File::get_size(): Unable to get file size, no file open");

	DWORD size = GetFileSize(handle, 0);
	if (size == INVALID_FILE_SIZE)
		throw CL_Exception("CL_IODeviceProvider_File::get_size(): Unable to get file size");

	return size;
#else
	if (handle == invalid_handle)
		throw CL_Exception("CL_IODeviceProvider_File::get_size(): Unable to get file size, no file open");

	off_t old_pos = lseek(handle, 0, SEEK_CUR);
	if (old_pos == (off_t) -1)
		throw CL_Exception("CL_IODeviceProvider_File::get_size(): Unable to get file size");
	off_t size = lseek(handle, 0, SEEK_END);
	lseek(handle, old_pos, SEEK_SET);

	if (size == (off_t) -1)
		throw CL_Exception("CL_IODeviceProvider_File::get_size(): Unable to get file size");
		
	return (int) size;
#endif
}

int CL_IODeviceProvider_File::get_position() const
{
#ifdef WIN32
	if (handle == invalid_handle)
		throw CL_Exception("CL_IODeviceProvider_File::get_position(): Unable to get file position pointer, no file open");

	DWORD pos = SetFilePointer(handle, 0, 0, FILE_CURRENT);
	if (pos == INVALID_SET_FILE_POINTER)
		throw CL_Exception("CL_IODeviceProvider_File::get_position(): Unable to get file position pointer");

	return (int) pos;
#else
	if (handle == invalid_handle)
		throw CL_Exception("Unable to get file position pointer, no file open");

	off_t pos = lseek(handle, 0, SEEK_CUR);
	if (pos == (off_t) -1)
		throw CL_Exception("Unable to get file position pointer");

	return pos;
#endif
}

CL_SecurityDescriptor CL_IODeviceProvider_File::get_permissions() const
{
	CL_SecurityDescriptor security_descriptor;
	return security_descriptor;
}

/////////////////////////////////////////////////////////////////////////////
// CL_IODeviceProvider_File Operations:

bool CL_IODeviceProvider_File::open(
	const CL_String &filename,
	CL_File::OpenMode open_mode,
	unsigned int access,
	unsigned int share,
	unsigned int flags)
{
	if (handle != invalid_handle)
		close();

	// save parameters for duplicate()
	this->filename = filename;
	this->open_mode = open_mode;
	this->access = access;
	this->share = share;
	this->flags = flags;
		
#ifdef WIN32
	DWORD win32_desired_access = 0;
	DWORD win32_share_mode = 0;
	DWORD win32_create_mode = 0;
	DWORD win32_flags = 0;
	LPSECURITY_ATTRIBUTES win32_security_attributes = 0;

	switch (open_mode)
	{
	case CL_File::create_always: win32_create_mode = CREATE_ALWAYS; break;
	case CL_File::create_new: win32_create_mode = CREATE_NEW; break;
	case CL_File::open_always: win32_create_mode = OPEN_ALWAYS; break;
	case CL_File::open_existing: win32_create_mode = OPEN_EXISTING; break;
	case CL_File::open_existing_truncate: win32_create_mode = TRUNCATE_EXISTING; break;
	}

	if (access & CL_File::access_read)
		win32_desired_access |= GENERIC_READ;
	if (access & CL_File::access_write)
		win32_desired_access |= GENERIC_WRITE;

	if (share & CL_File::share_read)
		win32_share_mode |= FILE_SHARE_READ;
	if (share & CL_File::share_write)
		win32_share_mode |= FILE_SHARE_WRITE;
	if (share & CL_File::share_delete)
		win32_share_mode |= FILE_SHARE_DELETE;

	if (flags & CL_File::flag_write_through)
		win32_flags |= FILE_FLAG_WRITE_THROUGH;
	if (flags & CL_File::flag_no_buffering)
		win32_flags |= FILE_FLAG_NO_BUFFERING;
	if (flags & CL_File::flag_random_access)
		win32_flags |= FILE_FLAG_RANDOM_ACCESS;
	if (flags & CL_File::flag_sequential_scan)
		win32_flags |= FILE_FLAG_SEQUENTIAL_SCAN;

	handle = CreateFile(
		CL_StringHelp::utf8_to_ucs2(filename).c_str(),
		win32_desired_access,
		win32_share_mode,
		win32_security_attributes,
		win32_create_mode,
		win32_flags,
		0);

	return (handle != invalid_handle);
#else
	int unix_flags = 0;
	mode_t mode = S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH; // ug+rx, o+r

	if ((access & CL_File::access_read) && (access & CL_File::access_write))
		unix_flags |= O_RDWR;
	else if (access & CL_File::access_read)
		unix_flags |= O_RDONLY;
	else if (access & CL_File::access_write)
		unix_flags |= O_WRONLY;

	if (open_mode == CL_File::create_always)
		unix_flags |= O_CREAT | O_TRUNC;
	else if (open_mode == CL_File::create_new)
		unix_flags |= O_CREAT | O_EXCL;
	else if (open_mode == CL_File::open_always)
		unix_flags |= O_CREAT;
	else if (open_mode == CL_File::open_existing)
		unix_flags |= 0;
	else if (open_mode == CL_File::open_existing_truncate)
		unix_flags |= O_TRUNC;

	CL_String8 filename_a = CL_StringHelp::text_to_local8(filename);
	handle = ::open(filename_a.c_str(), unix_flags, mode);
	if (handle == -1)
		return false;
	
	return true;
#endif
}

bool CL_IODeviceProvider_File::open(
	const CL_String &filename,
	CL_File::OpenMode mode,
	const CL_SecurityDescriptor &permissions,
	unsigned int access,
	unsigned int share,
	unsigned int flags)
{
	return open(filename, mode, access, share, flags);
}
	
void CL_IODeviceProvider_File::close()
{
#ifdef WIN32
	if (handle != invalid_handle)
		CloseHandle(handle);
#else
	if (handle != invalid_handle)
		::close(handle);
#endif
	handle = invalid_handle;
}

bool CL_IODeviceProvider_File::set_permissions(const CL_SecurityDescriptor &permissions)
{
	return false;
}

int CL_IODeviceProvider_File::read(void *buffer, int size, bool read_all)
{
	if (size == 0)
		return 0;
	if (peeked_data.get_size() > 0)
	{
		#define cl_min(a,b) ((a)<(b)?(a):(b))
		int peek_amount = cl_min(size, peeked_data.get_size());
		memcpy(buffer, peeked_data.get_data(), peek_amount);
		memmove(peeked_data.get_data(), peeked_data.get_data()+peek_amount, peeked_data.get_size()-peek_amount);
		peeked_data.set_size(peeked_data.get_size()-peek_amount);
		if (peek_amount <= size)
			return peek_amount + read((char*) buffer+peek_amount, size-peek_amount, read_all);
	}

	return lowlevel_read(buffer, size, read_all);
}

int CL_IODeviceProvider_File::write(const void *buffer, int size, bool write_all)
{
	if (handle == invalid_handle)
		throw CL_Exception("CL_IODeviceProvider_File::write(): Unable to write to file, no file open");

#ifdef WIN32
	DWORD written = 0;
	BOOL result = WriteFile(handle, buffer, size, &written, 0);
	if (result == TRUE)
		return written;
	else
		throw CL_Exception("CL_IODeviceProvider_File::write(): WriteFile failed");
#else
	int result = ::write(handle, buffer, size);
	return result;
#endif
}

int CL_IODeviceProvider_File::send(const void *data, int len, bool send_all)
{
	return write(data, len, send_all);
}

int CL_IODeviceProvider_File::receive(void *data, int len, bool receive_all)
{
	return read(data, len, receive_all);
}

int CL_IODeviceProvider_File::peek(void *data, int len)
{
	if (peeked_data.get_size() >= len)
	{
		memcpy(data, peeked_data.get_data(), len);
		return len;
	}
	else
	{
		int old_size = peeked_data.get_size();
		try
		{
			peeked_data.set_size(len);
			int bytes_read = lowlevel_read(peeked_data.get_data()+old_size, len-old_size, false);
			peeked_data.set_size(old_size+bytes_read);
			memcpy(data, peeked_data.get_data(), peeked_data.get_size());
			return peeked_data.get_size();
		}
		catch (CL_Exception e)
		{
			peeked_data.set_size(old_size);
			throw e;
		}
	}
}

bool CL_IODeviceProvider_File::seek(int position, CL_IODevice::SeekMode seek_mode)
{
	if (handle == invalid_handle)
		throw CL_Exception("CL_IODeviceProvider_File::seek(): Unable to get file position pointer, no file open");

#ifdef WIN32
	DWORD moveMethod = FILE_BEGIN;
	switch (seek_mode)
	{
	case CL_IODevice::seek_set: moveMethod = FILE_BEGIN; break;
	case CL_IODevice::seek_cur: moveMethod = FILE_CURRENT; break;
	case CL_IODevice::seek_end: moveMethod = FILE_END; break;
	}

	DWORD new_pos = SetFilePointer(handle, position, 0, moveMethod);
	return (new_pos != INVALID_FILE_SIZE);
#else
	int mode = SEEK_SET;
	if (seek_mode == CL_File::seek_set)
		mode = SEEK_SET;
	else if (seek_mode == CL_File::seek_cur)
		mode = SEEK_CUR;
	else if (seek_mode == CL_File::seek_end)
		mode = SEEK_END;
	
	off_t new_pos = lseek(handle, position, mode);
	if (new_pos == (off_t) -1)
		return false;
	else
		return true;
#endif
}

CL_IODeviceProvider *CL_IODeviceProvider_File::duplicate()
{
	return new CL_IODeviceProvider_File(filename, open_mode, access, share, flags);
}

/////////////////////////////////////////////////////////////////////////////
// CL_IODeviceProvider_File Implementation:

int CL_IODeviceProvider_File::lowlevel_read(void *buffer, int size, bool read_all)
{
	if (handle == invalid_handle)
		throw CL_Exception("CL_IODeviceProvider_File::lowlevel_read(): Unable to read from file, no file open");

#ifdef WIN32
	DWORD bytes_read = 0;
	BOOL result = ReadFile(handle, buffer, size, &bytes_read, 0);
	if (result == TRUE)
		return bytes_read;
	else
		throw CL_Exception("CL_IODeviceProvider_File::lowlevel_read(): ReadFile failed");
#else
	int result = ::read(handle, buffer, size);
	return result;
#endif
}
