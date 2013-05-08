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
**    Magnus Norddahl
**    Harry Storbacka
*/

#include "Display/precomp.h"
#include "texture_impl.h"

namespace clan
{

Texture Texture_Impl::get_from_cache(const std::string &filename, const VirtualDirectory &directory, const ImageImportDescription &import_desc)
{
	if (import_desc.is_cached())	// Caching enabled
	{
		std::string hash = get_cache_hash(filename, directory, import_desc);
		return SharedGCData::get_texture(hash);
	}
	else
	{
		return Texture();
	}
}

void Texture_Impl::put_in_cache(Texture &texture, const std::string &filename, const VirtualDirectory &directory, const ImageImportDescription &import_desc)
{
	if (import_desc.is_cached())	// Caching enabled
	{
		std::string hash = get_cache_hash(filename, directory, import_desc);
		SharedGCData::add_texture(texture, hash);
		cache_used = true;
	}

}

std::string Texture_Impl::get_cache_hash(const std::string &filename, const VirtualDirectory &directory, const ImageImportDescription &import_desc)
{
	std::string alpha_code = "A";
	if (import_desc.get_premultiply_alpha())
		alpha_code = "P";

	std::string hash = directory.get_identifier() + filename + alpha_code;
	return hash;
}

}
