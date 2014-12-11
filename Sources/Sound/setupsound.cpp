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
#include "API/Sound/setupsound.h"
#include "API/Sound/SoundProviders/soundprovider_wave.h"
#include "API/Sound/SoundProviders/soundprovider_vorbis.h"
#include "API/Sound/SoundProviders/soundprovider_type_register.h"
#include "API/Core/Resources/xml_resource_document.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/Core/Resources/xml_resource_manager.h"
#include "Sound/Resources/xml_sound_cache.h"

#define INCLUDED_FROM_SETUPVORBIS
#include "SoundProviders/stb_vorbis.h"

namespace clan
{
class ResourceManager;
class XMLResourceDocument;

/////////////////////////////////////////////////////////////////////////////
// SetupSound operations:

class SetupSound_Impl
{
public:
	static void init();
	static void deinit();

	static void add_cache_factory(ResourceManager &manager, const XMLResourceDocument &doc);

	static int ref_count;
	static SoundProviderType *providertype_wave;
	static SoundProviderType *providertype_ogg;

};

int SetupSound_Impl::ref_count = 0;
SoundProviderType *SetupSound_Impl::providertype_wave = nullptr;
SoundProviderType *SetupSound_Impl::providertype_ogg = nullptr;

SetupSound::SetupSound()
{
	SetupSound_Impl::init();
}

SetupSound::~SetupSound()
{
	SetupSound_Impl::deinit();
}

void SetupSound_Impl::init()
{
	ref_count++;
	if (ref_count > 1)
		return;
	providertype_wave = new SoundProviderType_Register<SoundProvider_Wave>("wav");
	providertype_ogg = new SoundProviderType_Register<SoundProvider_Vorbis>("ogg");
	XMLResourceManager::add_cache_factory(std::function<void(ResourceManager &, const XMLResourceDocument &)>(&SetupSound_Impl::add_cache_factory));
}

void SetupSound_Impl::deinit()
{
	ref_count--;
	if (ref_count > 0)
		return;	// Wait until final call to deinit

	delete providertype_wave;
	providertype_wave = nullptr;

	delete providertype_ogg;
	providertype_ogg = nullptr;
}

void SetupSound_Impl::add_cache_factory(ResourceManager &manager, const XMLResourceDocument &doc)
{
	SoundCache::set(manager, std::shared_ptr<SoundCache>(new XMLSoundCache(doc)));
}

}
