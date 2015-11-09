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
**    Magnus Norddahl
*/

#pragma once

#include "API/Core/IOData/iodevice_provider.h"
#include "API/Core/IOData/file.h"
#include "API/Core/System/databuffer.h"

namespace clan
{
	class IODeviceProvider_File : public IODeviceProvider
	{
	public:
		IODeviceProvider_File();

		IODeviceProvider_File(
			const std::string &filename,
			File::OpenMode mode,
			unsigned int access,
			unsigned int share,
			unsigned int flags);

		~IODeviceProvider_File();

		int get_size() const override;
		int get_position() const override;

		bool open(
			const std::string &filename,
			File::OpenMode mode,
			unsigned int access,
			unsigned int share,
			unsigned int flags);

		void close();

		int read(void *buffer, int size, bool read_all);
		int write(const void *buffer, int size, bool write_all);
		int send(const void *data, int len, bool send_all) override;
		int receive(void *data, int len, bool receive_all) override;
		int peek(void *data, int len) override;

		bool seek(int position, IODevice::SeekMode mode) override;

		IODeviceProvider *duplicate() override;

	private:
		int lowlevel_read(void *buffer, int size, bool read_all);

		std::string filename;
		File::OpenMode open_mode;
		unsigned int access;
		unsigned int share;
		unsigned int flags;

#ifdef WIN32
		HANDLE handle;
#else
		int handle;
#endif
		DataBuffer peeked_data;
	};
}
