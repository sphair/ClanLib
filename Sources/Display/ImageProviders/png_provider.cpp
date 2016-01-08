/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
#include "API/Core/System/exception.h"
#include "API/Core/IOData/file_system.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/Text/string_help.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/ImageProviders/png_provider.h"
#include "Display/ImageProviders/PNGLoader/png_loader.h"
#include "Display/ImageProviders/PNGWriter/png_writer.h"
#include "Core/Zip/miniz.h"
#include <stdlib.h>

namespace clan
{
	PixelBuffer PNGProvider::load(
		const std::string &filename,
		const FileSystem &fs,
		bool srgb)
	{
		return PNGLoader::load(fs.open_file(filename), srgb);
	}

	PixelBuffer PNGProvider::load(
		const std::string &fullname,
		bool srgb)
	{
		File file(fullname);
		return PNGLoader::load(file, srgb);
	}

	PixelBuffer PNGProvider::load(IODevice &file, bool srgb)
	{
		return PNGLoader::load(file, srgb);
	}

	void PNGProvider::save(
		PixelBuffer buffer,
		const std::string &filename,
		FileSystem &fs)
	{
		IODevice file = fs.open_file(filename, File::create_always, File::access_read_write);
		save(buffer, file);
	}

	void PNGProvider::save(
		PixelBuffer buffer,
		const std::string &fullname)
	{
		std::string path = PathHelp::get_fullpath(fullname, PathHelp::path_type_file);
		std::string filename = PathHelp::get_filename(fullname, PathHelp::path_type_file);
		FileSystem vfs(path);
		PNGProvider::save(buffer, filename, vfs);
	}

	void PNGProvider::save(PixelBuffer buffer, IODevice &iodev)
	{
		PNGWriter::save(iodev, buffer);
		/*
		if (buffer.get_format() != tf_rgba8)
		{
			PixelBuffer newbuf(
			buffer.get_width(),
			buffer.get_height(),
			tf_rgba8);
			newbuf.set_image(buffer);
			buffer = newbuf;
		}

		size_t size = 0;
		void *data = tdefl_write_image_to_png_file_in_memory(buffer.get_data(), buffer.get_width(), buffer.get_height(), 4, &size);
		try
		{
			iodev.write(data, size);
			if (data)
			free(data);
		}
		catch (...)
		{
			if (data)
			free(data);
			throw;
		}
		*/
	}
}
