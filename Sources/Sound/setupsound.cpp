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
**    Mark Page
*/

#include "Sound/precomp.h"
#include "setupsound.h"
#include "API/Sound/SoundProviders/soundprovider_wave.h"
#include "API/Sound/SoundProviders/soundprovider_vorbis.h"
#include "API/Sound/SoundProviders/soundprovider_type_register.h"
#include "API/Core/Resources/xml_resource_document.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/Core/Resources/xml_resource_manager.h"
#include "Sound/Resources/XML/xml_sound_cache.h"
#include "../Core/System/setup_core.h"
#include "API/Core/Math/cl_math.h"

#define INCLUDED_FROM_SETUPVORBIS
#include "SoundProviders/stb_vorbis.h"

namespace clan
{
class ResourceManager;
class XMLResourceDocument;

/////////////////////////////////////////////////////////////////////////////
// SetupSound operations:

class SetupSound_Impl : public SetupModule
{
public:
	SetupSound_Impl();
	virtual ~SetupSound_Impl();

	static void add_cache_factory(ResourceManager &manager, const XMLResourceDocument &doc);

	SoundProviderType *providertype_wave = nullptr;;
	SoundProviderType *providertype_ogg = nullptr;;

};


void SetupSound::start()
{
	std::lock_guard<std::recursive_mutex> lock(SetupCore::instance.mutex);

	if (SetupCore::instance.module_sound)
		return;

	SetupCore::start();	// Display depends on core.
	SetupCore::instance.module_sound = clan::make_unique<SetupSound_Impl>();
}

SetupSound_Impl::SetupSound_Impl()
{
	providertype_wave = new SoundProviderType_Register<SoundProvider_Wave>("wav");
	providertype_ogg = new SoundProviderType_Register<SoundProvider_Vorbis>("ogg");
	XMLResourceManager::add_cache_factory(std::function<void(ResourceManager &, const XMLResourceDocument &)>(&SetupSound_Impl::add_cache_factory));
}

SetupSound_Impl::~SetupSound_Impl()
{
	delete providertype_wave;
	delete providertype_ogg;
}

void SetupSound_Impl::add_cache_factory(ResourceManager &manager, const XMLResourceDocument &doc)
{
	SoundCache::set(manager, std::shared_ptr<SoundCache>(new XMLSoundCache(doc)));
}

}
