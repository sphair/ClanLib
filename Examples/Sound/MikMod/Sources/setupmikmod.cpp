
#include "precomp.h"
#include "setupmikmod.h"
#include "soundprovider_mikmod.h"
#include <mikmod.h>

static int ref_count = 0;

SetupMikMod::SetupMikMod(bool register_resources_only)
{
	SetupMikMod::init(register_resources_only);
}

SetupMikMod::~SetupMikMod()
{
	SetupMikMod::deinit();
}

static clan::SoundProviderType *providertype[17] = {0};

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
 * Streamed_MikModSample_Session::clanMikMod_Update
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

void SetupMikMod::init(bool register_resources_only)
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

	providertype[0] = new clan::SoundProviderType_Register<SoundProvider_MikMod>("mikmod");
	providertype[1] = new clan::SoundProviderType_Register<SoundProvider_MikMod>("it");
	providertype[2] = new clan::SoundProviderType_Register<SoundProvider_MikMod>("xm");
	providertype[3] = new clan::SoundProviderType_Register<SoundProvider_MikMod>("s3m");
	providertype[4] = new clan::SoundProviderType_Register<SoundProvider_MikMod>("mtm");
	providertype[5] = new clan::SoundProviderType_Register<SoundProvider_MikMod>("669");
	providertype[6] = new clan::SoundProviderType_Register<SoundProvider_MikMod>("stm");
	providertype[7] = new clan::SoundProviderType_Register<SoundProvider_MikMod>("ult");
	providertype[8] = new clan::SoundProviderType_Register<SoundProvider_MikMod>("far");
	providertype[9] = new clan::SoundProviderType_Register<SoundProvider_MikMod>("med");
	providertype[10] = new clan::SoundProviderType_Register<SoundProvider_MikMod>("amf");
	providertype[11] = new clan::SoundProviderType_Register<SoundProvider_MikMod>("dsm");
	providertype[12] = new clan::SoundProviderType_Register<SoundProvider_MikMod>("imf");
	providertype[13] = new clan::SoundProviderType_Register<SoundProvider_MikMod>("gdm");
	providertype[14] = new clan::SoundProviderType_Register<SoundProvider_MikMod>("stx");
	providertype[15] = new clan::SoundProviderType_Register<SoundProvider_MikMod>("okt");
	providertype[16] = new clan::SoundProviderType_Register<SoundProvider_MikMod>("mod");

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

void SetupMikMod::deinit()
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
