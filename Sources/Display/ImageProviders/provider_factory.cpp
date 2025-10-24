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
**    Mark Page
**    (if your name is missing here, please add it)
*/

#include "Display/precomp.h"
#include "API/Core/IOData/virtual_file_system.h"
#include "API/Core/IOData/path_help.h"
#include "API/Display/ImageProviders/provider_factory.h"
#include "API/Display/ImageProviders/provider_type.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Core/System/exception.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/IOData/virtual_directory.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ProviderFactory attributes:

std::map<CL_String, CL_ImageProviderType *> CL_ImageProviderFactory::types;

/////////////////////////////////////////////////////////////////////////////
// CL_ImageProviderFactory operations:

CL_PixelBuffer CL_ImageProviderFactory::try_load(
	const CL_String &filename,
	const CL_String &type,
	CL_VirtualDirectory directory,
	CL_String *out_failure_reason)
{
	try
	{
		return load(filename, directory, type);
	}
	catch (CL_Exception e)
	{
		if (out_failure_reason)
			*out_failure_reason = e.message;
		return CL_PixelBuffer();
	}
}

CL_PixelBuffer CL_ImageProviderFactory::load(
	const CL_String &filename,
	const CL_VirtualDirectory &directory,
	const CL_String &type
	)
{
	if (type != "")
	{
		if (types.find(type) == types.end()) throw CL_Exception("Unknown image provider type " + type);

		CL_ImageProviderType *factory = types[type];
		return factory->load(filename, directory);
	}

	// Determine file extension and use it to lookup type.
	CL_String ext = CL_PathHelp::get_extension(filename, CL_PathHelp::path_type_virtual);
	ext = CL_StringHelp::text_to_lower(ext);
	if (types.find(ext) == types.end()) throw CL_Exception(CL_String("Unknown image provider type ") + ext);

	CL_ImageProviderType *factory = types[ext];
	return factory->load(filename, directory);
}

CL_PixelBuffer CL_ImageProviderFactory::load(
		CL_IODevice &file,
		const CL_String &type)
{
	if (types.find(type) == types.end()) throw CL_Exception("Unknown image provider type " + type);

	CL_ImageProviderType *factory = types[type];
	return factory->load(file);
}

CL_PixelBuffer CL_ImageProviderFactory::load(
	const CL_String &fullname,
	const CL_String &type
	)
{
	CL_String path = CL_PathHelp::get_fullpath(fullname, CL_PathHelp::path_type_file);
	CL_String filename = CL_PathHelp::get_filename(fullname, CL_PathHelp::path_type_file);
	CL_VirtualFileSystem vfs(path);
	return CL_ImageProviderFactory::load(filename, vfs.get_root_directory(), type);
}

void CL_ImageProviderFactory::save(
	CL_PixelBuffer buffer,
	const CL_String &filename,
	CL_VirtualDirectory &directory,
	const CL_String &type_
	)
{
	CL_String type = type_;

	if (type.empty())
		type = CL_PathHelp::get_extension(filename, CL_PathHelp::path_type_virtual);
	
	if (types.find(type) == types.end()) throw CL_Exception("Unknown image provider type " + type);
	
	CL_ImageProviderType *factory = types[type];
	factory->save(buffer, filename, directory);
}

void CL_ImageProviderFactory::save(
	CL_PixelBuffer buffer,
	const CL_String &fullname,
	const CL_String &type
	)
{
	CL_String path = CL_PathHelp::get_fullpath(fullname, CL_PathHelp::path_type_file);
	CL_String filename = CL_PathHelp::get_filename(fullname, CL_PathHelp::path_type_file);
	CL_VirtualFileSystem vfs(path);
	CL_VirtualDirectory dir = vfs.get_root_directory();
	return CL_ImageProviderFactory::save(buffer, filename, dir, type);
}

void CL_ImageProviderFactory::save(
	CL_PixelBuffer buffer,
	CL_IODevice &file,
	const CL_String &type
	)
{
	
	if (types.find(type) == types.end()) throw CL_Exception("Unknown image provider type " + type);

	CL_ImageProviderType *factory = types[type];
	factory->save(buffer, file);
}
