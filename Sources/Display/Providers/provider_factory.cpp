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

#include "Display/display_precomp.h"
#include "API/Display/Providers/provider_factory.h"
#include "API/Display/Providers/provider_type.h"
#include "API/Display/pixel_buffer.h"
#include "API/Core/System/error.h"
#include "API/Core/System/clanstring.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ProviderFactory attributes:

std::map<std::string, CL_ProviderType *> CL_ProviderFactory::types;

/////////////////////////////////////////////////////////////////////////////
// CL_ProviderFactory operations:

CL_PixelBuffer CL_ProviderFactory::load(
	const std::string &filename,
	const std::string &type,
	CL_InputSourceProvider *input_provider)
{
	if (type != "")
	{
		if (types.find(type) == types.end()) throw CL_Error("Unknown image provider type " + type);

		CL_ProviderType *factory = types[type];
		return factory->load(filename, input_provider);
	}

	// Determine file extension and use it to lookup type.
	std::string ext = CL_String::right(filename, 4);
	if (ext.at(0) == '.') ext = CL_String::right(ext, 3);
	ext = CL_String::to_lower(ext);
	if (types.find(ext) == types.end()) throw CL_Error(std::string("Unknown image provider type ") + ext);

	CL_ProviderType *factory = types[ext];
	return factory->load(filename, input_provider);
}

void
CL_ProviderFactory::save(
	CL_PixelBuffer buffer,
	const std::string &filename,
	const std::string &type_,
	CL_OutputSourceProvider *output_provider)
{
	std::string type = type_;

	if (type.empty())
		type = CL_String::get_extension(filename);
	
	if (types.find(type) == types.end()) throw CL_Error("Unknown image provider type " + type);
	
	CL_ProviderType *factory = types[type];
	factory->save(buffer, filename, output_provider);
}


/* EOF */
