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
*/

#pragma once


#include "API/Core/IOData/iodevice_provider.h"
#include "API/Core/IOData/file.h"
#include "API/Core/System/databuffer.h"

class CL_SecurityDescriptor;

class CL_IODeviceProvider_File : public CL_IODeviceProvider
{
/// \name Construction
/// \{

public:
	CL_IODeviceProvider_File();

	CL_IODeviceProvider_File(
		const CL_String &filename,
		CL_File::OpenMode mode,
		unsigned int access,
		unsigned int share,
		unsigned int flags);

	CL_IODeviceProvider_File(
		const CL_String &filename,
		CL_File::OpenMode mode,
		const CL_SecurityDescriptor &permissions,
		unsigned int access,
		unsigned int share,
		unsigned int flags);

	~CL_IODeviceProvider_File();


/// \}
/// \name Attributes
/// \{

public:
	int get_size() const;

	int get_position() const;

	CL_SecurityDescriptor get_permissions() const;


/// \}
/// \name Operations
/// \{

public:
	bool open(
		const CL_String &filename,
		CL_File::OpenMode mode,
		unsigned int access,
		unsigned int share,
		unsigned int flags);

	bool open(
		const CL_String &filename,
		CL_File::OpenMode mode,
		const CL_SecurityDescriptor &permissions,
		unsigned int access,
		unsigned int share,
		unsigned int flags);

	void close();

	bool set_permissions(const CL_SecurityDescriptor &permissions);

	int read(void *buffer, int size, bool read_all);

	int write(const void *buffer, int size, bool write_all);

	int send(const void *data, int len, bool send_all);

	int receive(void *data, int len, bool receive_all);

	int peek(void *data, int len);

	bool seek(int position, CL_IODevice::SeekMode mode);

	CL_IODeviceProvider *duplicate();


/// \}
/// \name Implementation
/// \{

private:
	int lowlevel_read(void *buffer, int size, bool read_all);

	CL_String filename;
	CL_File::OpenMode open_mode;
	unsigned int access;
	unsigned int share;
	unsigned int flags;

#ifdef WIN32
	HANDLE handle;
#else
	int handle;
#endif
	CL_DataBuffer peeked_data;
/// \}
};


