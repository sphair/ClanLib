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
#include "API/Sound/setupsound.h"
#include "API/Sound/SoundProviders/soundprovider_wave.h"
#include "API/Sound/SoundProviders/soundprovider_type_register.h"
#include "API/Core/Resources/resource_manager.h"
#include "resourcetype_sample.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SetupSound operations:

static int ref_count = 0;
static CL_Slot slot_resource_added;
static CL_SoundProviderType *providertype_wave = 0;

static void clansound_resource_added(CL_Resource &resource)
{
	std::string type = resource.get_type();

	if (type == "sample") new CL_ResourceData_Sample(resource);
}

CL_SetupSound::CL_SetupSound(bool register_resources_only)
{
	CL_SetupSound::init(register_resources_only);
}

CL_SetupSound::~CL_SetupSound()
{
	CL_SetupSound::deinit();
}

void CL_SetupSound::init(bool register_resources_only)
{
	ref_count++;
	if (ref_count > 1) return;

	slot_resource_added = CL_ResourceManager::sig_resource_added().connect(&clansound_resource_added);
	providertype_wave = new CL_SoundProviderType_Register<CL_SoundProvider_Wave>("wav");

	if (register_resources_only) return;
}

void CL_SetupSound::deinit()
{
	ref_count--;
	if (ref_count > 0) return;	// Wait until final call to deinit

	delete providertype_wave;
	providertype_wave = 0;
	slot_resource_added = CL_Slot();
}
