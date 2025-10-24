/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

#include "Sound/precomp.h"

#ifdef _MSC_VER
#pragma warning(disable : 4786)
#endif

#include "API/Sound/SoundProviders/soundprovider_factory.h"
#include "API/Sound/SoundProviders/soundprovider_type.h"
#include "API/Core/System/exception.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/IOData/virtual_directory.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/IOData/virtual_file_system.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProviderFactory attributes:

std::map<CL_String, CL_SoundProviderType *> CL_SoundProviderFactory::types;

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProviderFactory operations:

CL_SoundProvider *CL_SoundProviderFactory::load(
	const CL_String &filename,
	bool streamed,
	const CL_VirtualDirectory &directory,
	const CL_String &type)
{
	if (!type.empty())
	{
		if (types.find(type) == types.end()) throw CL_Exception("Unknown sound provider type " + type);

		CL_SoundProviderType *factory = types[type];
		return factory->load(filename, streamed, directory);
	}

	// Determine file extension and use it to lookup type.
	CL_String ext = CL_PathHelp::get_extension(filename);
	if (ext.empty()) ext = filename;
	ext = CL_StringHelp::text_to_lower(ext);
	if (types.find(ext) == types.end()) throw CL_Exception("Unknown sound provider type " + ext);

	CL_SoundProviderType *factory = types[ext];
	return factory->load(filename, streamed, directory);
}

CL_SoundProvider *CL_SoundProviderFactory::load(
	const CL_String &fullname,
	bool streamed,
	const CL_String &type)
{
	CL_String path = CL_PathHelp::get_fullpath(fullname, CL_PathHelp::path_type_file);
	CL_String filename = CL_PathHelp::get_filename(fullname, CL_PathHelp::path_type_file);
	CL_VirtualFileSystem vfs(path);
	return CL_SoundProviderFactory::load(filename, streamed, vfs.get_root_directory(), type);
}

CL_SoundProvider *CL_SoundProviderFactory::load(
	CL_IODevice &file,
	bool streamed,
	const CL_String &type)
{
	if (types.find(type) == types.end()) throw CL_Exception("Unknown sound provider type " + type);

	CL_SoundProviderType *factory = types[type];
	return factory->load(file, streamed);
}
