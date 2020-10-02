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
**    Magnus Norddahl
*/

#include "Core/precomp.h"
#include "API/Core/IOData/file.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/System/exception.h"
#include "API/Core/Text/string_help.h"
#include "iodevice_impl.h"
#include "iodevice_provider_file.h"

namespace clan
{
	std::string File::read_text(const std::string &filename)
	{
		File file(filename);
		size_t file_size = file.get_size();
		std::vector<char> text;
		text.resize(file_size + 1);
		text[file_size] = 0;
		if (file_size)
			file.read(&text[0], file_size);
		file.close();
		if (file_size)
			return std::string(&text[0]);
		else
			return std::string();
	}

	DataBuffer File::read_bytes(const std::string &filename)
	{
		File file(filename);
		DataBuffer buffer(file.get_size());
		file.read(buffer.get_data(), buffer.get_size());
		file.close();
		return buffer;
	}

	void File::write_text(const std::string &filename, const std::string &text, bool write_bom)
	{
		File file(filename, create_always, access_write);
		if (write_bom)
		{
			unsigned char bom[3] = { 0xef, 0xbb, 0xbf };
			file.write(bom, 3);
		}
		file.write(text.data(), text.length());
		file.close();
	}

	void File::write_bytes(const std::string &filename, const DataBuffer &bytes)
	{
		File file(filename, create_always, access_write);
		file.write(bytes.get_data(), bytes.get_size());
		file.close();
	}

	File::File()
		: IODevice(new IODeviceProvider_File())
	{
	}

	File::File(
		const std::string &filename)
		: IODevice(new IODeviceProvider_File(PathHelp::normalize(filename, PathHelp::path_type_file), open_existing, access_read, share_all, 0))
	{
	}

	File::File(
		const std::string &filename,
		OpenMode open_mode,
		unsigned int access,
		unsigned int share,
		unsigned int flags)
		: IODevice(new IODeviceProvider_File(PathHelp::normalize(filename, PathHelp::path_type_file), open_mode, access, share, flags))
	{
	}
	File::~File()
	{
	}

	bool File::open(
		const std::string &filename)
	{
		IODeviceProvider_File *provider = dynamic_cast<IODeviceProvider_File*>(impl->provider);
		return provider->open(PathHelp::normalize(filename, PathHelp::path_type_file), open_existing, access_read, share_all, 0);
	}

	bool File::open(
		const std::string &filename,
		OpenMode open_mode,
		unsigned int access,
		unsigned int share,
		unsigned int flags)
	{
		IODeviceProvider_File *provider = dynamic_cast<IODeviceProvider_File*>(impl->provider);
		return provider->open(PathHelp::normalize(filename, PathHelp::path_type_file), open_mode, access, share, flags);
	}

	void File::close()
	{
		IODeviceProvider_File *provider = dynamic_cast<IODeviceProvider_File*>(impl->provider);
		provider->close();
	}
}
