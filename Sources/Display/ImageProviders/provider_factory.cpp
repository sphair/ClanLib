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
**    Mark Page
**    (if your name is missing here, please add it)
*/

#include "Display/precomp.h"
#include "API/Core/IOData/file_system.h"
#include "API/Core/IOData/path_help.h"
#include "API/Display/ImageProviders/provider_factory.h"
#include "API/Display/ImageProviders/provider_type.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Core/System/exception.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/IOData/path_help.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// ProviderFactory attributes:

std::map<std::string, ImageProviderType *> ImageProviderFactory::types;

/////////////////////////////////////////////////////////////////////////////
// ImageProviderFactory operations:

PixelBuffer ImageProviderFactory::try_load(
	const std::string &filename,
	const std::string &type,
	const FileSystem &fs,
	std::string *out_failure_reason,
	bool srgb)
{
	try
	{
		return load(filename, fs, type, srgb);
	}
	catch (const Exception& e)
	{
		if (out_failure_reason)
			*out_failure_reason = e.message;
		return PixelBuffer();
	}
}

PixelBuffer ImageProviderFactory::load(
	const std::string &filename,
	const FileSystem &fs,
	const std::string &type,
	bool srgb)
{
	if (type != "")
	{
		if (types.find(type) == types.end()) throw Exception("Unknown image provider type " + type);

		ImageProviderType *factory = types[type];
		return factory->load(filename, fs, srgb);
	}

	// Determine file extension and use it to lookup type.
	std::string ext = PathHelp::get_extension(filename, PathHelp::path_type_virtual);
	ext = StringHelp::text_to_lower(ext);
	if (types.find(ext) == types.end()) throw Exception(std::string("Unknown image provider type ") + ext);

	ImageProviderType *factory = types[ext];
	return factory->load(filename, fs, srgb);
}

PixelBuffer ImageProviderFactory::load(
	IODevice &file,
	const std::string &type,
	bool srgb)
{
	if (types.find(type) == types.end()) throw Exception("Unknown image provider type " + type);

	ImageProviderType *factory = types[type];
	return factory->load(file, srgb);
}

PixelBuffer ImageProviderFactory::load(
	const std::string &fullname,
	const std::string &type,
	bool srgb)
{
	std::string path = PathHelp::get_fullpath(fullname, PathHelp::path_type_file);
	std::string filename = PathHelp::get_filename(fullname, PathHelp::path_type_file);
	FileSystem vfs(path);
	return ImageProviderFactory::load(filename, vfs, type, srgb);
}

void ImageProviderFactory::save(
	PixelBuffer buffer,
	const std::string &filename,
	FileSystem &fs,
	const std::string &type_)
{
	std::string type = type_;

	if (type.empty())
		type = PathHelp::get_extension(filename, PathHelp::path_type_virtual);
	
	if (types.find(type) == types.end()) throw Exception("Unknown image provider type " + type);
	
	ImageProviderType *factory = types[type];
	factory->save(buffer, filename, fs);
}

void ImageProviderFactory::save(
	PixelBuffer buffer,
	const std::string &fullname,
	const std::string &type)
{
	std::string path = PathHelp::get_fullpath(fullname, PathHelp::path_type_file);
	std::string filename = PathHelp::get_filename(fullname, PathHelp::path_type_file);
	FileSystem vfs(path);
	return ImageProviderFactory::save(buffer, filename, vfs, type);
}

void ImageProviderFactory::save(
	PixelBuffer buffer,
	IODevice &file,
	const std::string &type)
{
	
	if (types.find(type) == types.end()) throw Exception("Unknown image provider type " + type);

	ImageProviderType *factory = types[type];
	factory->save(buffer, file);
}

}
