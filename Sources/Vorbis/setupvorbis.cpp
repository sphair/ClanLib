/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

//! component="Vorbis"

#include "API/Vorbis/setupvorbis.h"
#include "API/Vorbis/soundprovider_vorbis.h"
#include "API/Sound/SoundProviders/soundprovider_type_register.h"

#define INCLUDED_FROM_SETUPVORBIS
#include "stb_vorbis.h"

namespace clan
{

static int cl_vorbis_init = 0;
static SoundProviderType *providertype_ogg = 0;

SetupVorbis::SetupVorbis(bool register_resources_only)
{
	SetupVorbis::init(register_resources_only);
}

SetupVorbis::~SetupVorbis()
{
	SetupVorbis::deinit();
}

void SetupVorbis::init( bool register_resources_only )
{
	cl_vorbis_init++;
	if (cl_vorbis_init == 1)
	{
		providertype_ogg = new SoundProviderType_Register<SoundProvider_Vorbis>("ogg");
	}
}

void SetupVorbis::deinit()
{
	cl_vorbis_init--;
	if (cl_vorbis_init == 0)
	{
		delete providertype_ogg;
		providertype_ogg = 0;
	}
}

}
