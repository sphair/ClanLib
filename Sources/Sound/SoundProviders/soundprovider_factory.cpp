/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include "Sound/precomp.h"

#ifdef _MSC_VER
#pragma warning(disable : 4786)
#endif

#include "API/Sound/SoundProviders/soundprovider_factory.h"
#include "API/Sound/SoundProviders/soundprovider_type.h"
#include "API/Core/System/error.h"
#include "API/Core/System/clanstring.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProviderFactory attributes:

std::map<std::string, CL_SoundProviderType *> CL_SoundProviderFactory::types;

/////////////////////////////////////////////////////////////////////////////
// CL_SoundProviderFactory operations:

CL_SoundProvider *CL_SoundProviderFactory::load(
	const std::string &filename,
	bool streamed,
	const std::string &type,
	CL_InputSourceProvider *input_provider)
{
	if (type != "")
	{
		if (types.find(type) == types.end()) throw CL_Error("Unknown sound provider type " + type);

		CL_SoundProviderType *factory = types[type];
		return factory->load(filename, input_provider, streamed);
	}

	// Determine file extension and use it to lookup type.
	std::string ext = CL_String::get_extension(filename);
	if (ext.empty()) ext = filename;
	ext = CL_String::to_lower(ext);
	if (types.find(ext) == types.end()) throw CL_Error(std::string("Unknown sound provider type ") + ext.c_str());

	CL_SoundProviderType *factory = types[ext];
	return factory->load(filename, input_provider, streamed);
}
