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
**    Harry Storbacka
*/

#include "Display/precomp.h"
#include <iostream>
#include "API/Core/IOData/file_system.h"
#include "API/Core/IOData/path_help.h"
#include "API/Display/ImageProviders/jpeg_provider.h"
#include "API/Core/System/exception.h"
#include "API/Core/Text/string_help.h"
#include "JPEGLoader/jpeg_loader.h"
#include "JPEGWriter/jpge.h"

namespace clan
{
	PixelBuffer JPEGProvider::load(
		const std::string &filename,
		const FileSystem &fs,
		bool srgb)
	{
		return JPEGLoader::load(fs.open_file(filename), srgb);
	}

	PixelBuffer JPEGProvider::load(
		IODevice &file,
		bool srgb)
	{
		return JPEGLoader::load(file, srgb);
	}

	PixelBuffer JPEGProvider::load(
		const std::string &fullname,
		bool srgb)
	{
		std::string path = PathHelp::get_fullpath(fullname, PathHelp::path_type_file);
		std::string filename = PathHelp::get_filename(fullname, PathHelp::path_type_file);
		FileSystem vfs(path);
		return JPEGProvider::load(filename, vfs, srgb);
	}

	void JPEGProvider::save(
		PixelBuffer buffer,
		const std::string &fullname,
		int quality)
	{
		std::string path = PathHelp::get_fullpath(fullname, PathHelp::path_type_file);
		std::string filename = PathHelp::get_filename(fullname, PathHelp::path_type_file);
		FileSystem vfs(path);
		return JPEGProvider::save(buffer, filename, vfs, quality);
	}

	void JPEGProvider::save(
		PixelBuffer buffer,
		IODevice &file,
		int quality)
	{
		if (buffer.get_format() != tf_rgb8)
		{
			PixelBuffer newbuf(buffer.get_width(), buffer.get_height(), tf_rgb8);
			newbuf.set_image(buffer);
			buffer = newbuf;
		}

		DataBuffer output(buffer.get_width() * buffer.get_height() * 5);
		int size = output.get_size();

		clan_jpge::params desc;
		desc.m_quality = quality;
		bool result = clan_jpge::compress_image_to_jpeg_file_in_memory(output.get_data(), size, buffer.get_width(), buffer.get_height(), 3, output.get_data<clan_jpge::uint8>(), desc);
		if (!result)
			throw Exception("Unable to compress JPEG image");

		file.write(output.get_data(), size);
	}

	void JPEGProvider::save(
		PixelBuffer buffer,
		const std::string &filename,
		FileSystem &fs,
		int quality)
	{
		IODevice iodev = fs.open_file(filename, File::create_always, File::access_read_write);
		save(buffer, iodev, quality);
	}
}
