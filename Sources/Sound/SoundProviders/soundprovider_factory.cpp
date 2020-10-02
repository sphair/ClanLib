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

#include "Sound/precomp.h"

#ifdef _MSC_VER
#pragma warning(disable : 4786)
#endif

#include "API/Sound/SoundProviders/soundprovider_factory.h"
#include "API/Sound/SoundProviders/soundprovider_type.h"
#include "API/Core/System/exception.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/IOData/file_system.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/IOData/file_system.h"
#include "../setupsound.h"

namespace clan
{
	SoundProvider *SoundProviderFactory::load(
		const std::string &filename,
		bool streamed,
		const FileSystem &fs,
		const std::string &type)
	{
		SetupSound::start();
		auto &types = *SetupSound::get_sound_provider_factory_types();

		if (!type.empty())
		{
			if (types.find(type) == types.end()) throw Exception("Unknown sound provider type " + type);

			SoundProviderType *factory = types[type];
			return factory->load(filename, streamed, fs);
		}

		// Determine file extension and use it to lookup type.
		std::string ext = PathHelp::get_extension(filename);
		if (ext.empty()) ext = filename;
		ext = StringHelp::text_to_lower(ext);
		if (types.find(ext) == types.end()) throw Exception("Unknown sound provider type " + ext);

		SoundProviderType *factory = types[ext];
		return factory->load(filename, streamed, fs);
	}

	SoundProvider *SoundProviderFactory::load(
		const std::string &fullname,
		bool streamed,
		const std::string &type)
	{
		SetupSound::start();
		std::string path = PathHelp::get_fullpath(fullname, PathHelp::path_type_file);
		std::string filename = PathHelp::get_filename(fullname, PathHelp::path_type_file);
		FileSystem vfs(path);
		return SoundProviderFactory::load(filename, streamed, vfs, type);
	}

	SoundProvider *SoundProviderFactory::load(
		IODevice &file,
		bool streamed,
		const std::string &type)
	{
		SetupSound::start();
		auto &types = *SetupSound::get_sound_provider_factory_types();

		if (types.find(type) == types.end()) throw Exception("Unknown sound provider type " + type);

		SoundProviderType *factory = types[type];
		return factory->load(file, streamed);
	}
}
