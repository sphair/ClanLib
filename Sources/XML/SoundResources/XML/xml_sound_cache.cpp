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
**    Mark Page
*/

#include "XML/precomp.h"
#include "API/Sound/soundbuffer.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "API/XML/dom_element.h"
#include "API/Core/IOData/path_help.h"
#include "xml_sound_cache.h"
#include "API/XML/Resources/resource_factory.h"
#include "API/XML/Resources/xml_resource_manager.h"

namespace clan
{

	void XMLSoundCache::add_cache_factory(ResourceManager &manager, const XMLResourceDocument &doc)
	{
		SoundCache::set(manager, std::shared_ptr<SoundCache>(new XMLSoundCache(doc)));
	}

	void XMLResourceFactory::set_sound()
	{
		XMLResourceManager::add_cache_factory(std::function<void(ResourceManager &, const XMLResourceDocument &)>(&XMLSoundCache::add_cache_factory));
	}

	XMLSoundCache::XMLSoundCache(const XMLResourceDocument &doc)
		: doc(doc)
	{
	}

	XMLSoundCache::~XMLSoundCache()
	{
	}

	Resource<SoundBuffer> XMLSoundCache::get_sound(const std::string &id)
	{
		auto it = sounds.find(id);
		if (it != sounds.end())
			return it->second;

		Resource<SoundBuffer> sound = SoundBuffer::load(id, doc);
		sounds[id] = sound;
		return sound;
	}
}
