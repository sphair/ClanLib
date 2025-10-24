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

#include "API/MikMod/setupmikmod.h"
#include "API/MikMod/soundprovider_mikmod.h"
#include "API/Sound/SoundProviders/soundprovider_type_register.h"
#include "API/Core/Resources/resource_manager.h"

#ifdef __APPLE__
#include <mikmod/mikmod.h>
#else
#include <mikmod.h>
#endif

static int ref_count = 0;

CL_SetupMikMod::CL_SetupMikMod(bool register_resources_only)
{
	CL_SetupMikMod::init(register_resources_only);
}

CL_SetupMikMod::~CL_SetupMikMod()
{
	CL_SetupMikMod::deinit();
}

static CL_SoundProviderType *providertype[17] = {0};

static BOOL clanMikMod_IsPresent()
{
	return 1;
}

static void clanMikMod_Update()
{
	/* do nothing */
}

/*
 * This structure is the ClanLib driver for MikMod. It's a "standard" driver
 * since it re-uses lots of functions from libMikMod, indeed the big
 * ClanLib-specific stuff is the update function, which is mapped to
 * CL_Streamed_MikModSample_Session::clanMikMod_Update
 */
MDRIVER drv_clanlib = {
	NULL,
	"clanMikMod driver",
	"0.1",
	255,
	255, 
	"clanmikmod",
#if LIBMIKMOD_VERSION >= 0x030200
	NULL,
#endif
	NULL, // Command line
	clanMikMod_IsPresent, // IsPresent
	VC_SampleLoad,
	VC_SampleUnload,
	VC_SampleSpace,
	VC_SampleLength,
	VC_Init,
	VC_Exit,
	NULL, // Reset
	VC_SetNumVoices,
	VC_PlayStart,
	VC_PlayStop,
	clanMikMod_Update,
	NULL, // Pause
	VC_VoiceSetVolume,
	VC_VoiceGetVolume,
	VC_VoiceSetFrequency,
	VC_VoiceGetFrequency,
	VC_VoiceSetPanning,
	VC_VoiceGetPanning,
	VC_VoicePlay,
	VC_VoiceStop,
	VC_VoiceStopped,
	VC_VoiceGetPosition,
	VC_VoiceRealVolume
};

void CL_SetupMikMod::init(bool register_resources_only)
{
	ref_count++;
	if (ref_count > 1) return;

	/* Register the loaders we want to use:  */
	MikMod_RegisterAllLoaders();
	
	/* Register the drivers we want to use: */
	MikMod_RegisterDriver(&drv_clanlib);

#ifdef WIN32
	MikMod_Init("");
#else
#if LIBMIKMOD_VERSION >= ((3 << 16) | (1 << 8) | (7))
	MikMod_Init("");
#else
	MikMod_Init();
#endif
#endif

	md_mode |= DMODE_INTERP;
	
	// disable the reverb
	md_reverb = 0;

	providertype[0] = new CL_SoundProviderType_Register<CL_SoundProvider_MikMod>("mikmod");
	providertype[1] = new CL_SoundProviderType_Register<CL_SoundProvider_MikMod>("it");
	providertype[2] = new CL_SoundProviderType_Register<CL_SoundProvider_MikMod>("xm");
	providertype[3] = new CL_SoundProviderType_Register<CL_SoundProvider_MikMod>("s3m");
	providertype[4] = new CL_SoundProviderType_Register<CL_SoundProvider_MikMod>("mtm");
	providertype[5] = new CL_SoundProviderType_Register<CL_SoundProvider_MikMod>("669");
	providertype[6] = new CL_SoundProviderType_Register<CL_SoundProvider_MikMod>("stm");
	providertype[7] = new CL_SoundProviderType_Register<CL_SoundProvider_MikMod>("ult");
	providertype[8] = new CL_SoundProviderType_Register<CL_SoundProvider_MikMod>("far");
	providertype[9] = new CL_SoundProviderType_Register<CL_SoundProvider_MikMod>("med");
	providertype[10] = new CL_SoundProviderType_Register<CL_SoundProvider_MikMod>("amf");
	providertype[11] = new CL_SoundProviderType_Register<CL_SoundProvider_MikMod>("dsm");
	providertype[12] = new CL_SoundProviderType_Register<CL_SoundProvider_MikMod>("imf");
	providertype[13] = new CL_SoundProviderType_Register<CL_SoundProvider_MikMod>("gdm");
	providertype[14] = new CL_SoundProviderType_Register<CL_SoundProvider_MikMod>("stx");
	providertype[15] = new CL_SoundProviderType_Register<CL_SoundProvider_MikMod>("okt");
	providertype[16] = new CL_SoundProviderType_Register<CL_SoundProvider_MikMod>("mod");

	/*
	 * OK, now we kind of ignore the "register_resources_only" 
	 * parameter. Indeed, MikMod_Init seems to be required for
	 * the module loader to be able to work. However, it's not
	 * a big problem since MikMod_Init() does not require any
	 * peculiar hardware and/or settings, the hardware being
	 * accessed through clanSound anyway. In fact, MikMod is
	 * only used to read modules and transform them into raw
	 * sound. Then clanSound handles everything else. So
	 * register_resources_only is of no use in this function,
	 * but is kept for API consistency.
	 */
	// if (register_resources_only) return;
}

void CL_SetupMikMod::deinit()
{
	ref_count--;
	if (ref_count > 0) return;

	for(int i=0; i<17; i++)
	{
		delete providertype[i];
		providertype[i] = 0;
	}
	
	MikMod_Exit();
}
