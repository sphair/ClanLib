/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 * Program WebSite: http://methane.sourceforge.net/index.html              *
 *                                                                         *
 ***************************************************************************/

//------------------------------------------------------------------------------
// Methane Brothers Sound Control (Source File)
//------------------------------------------------------------------------------

#include "sound.h"
#include "target.h"

//------------------------------------------------------------------------------
//! \brief Sound driver constructor
//------------------------------------------------------------------------------
CSoundDrv::CSoundDrv()
{
	m_CurrentModule = 0;
}

//------------------------------------------------------------------------------
//! \brief Destroy the sound driver
//------------------------------------------------------------------------------
CSoundDrv::~CSoundDrv()
{
}

//------------------------------------------------------------------------------
//! \brief Prepare all audio to play
//------------------------------------------------------------------------------
void CSoundDrv::PrepareAudio(void)
{
}

//------------------------------------------------------------------------------
//! \brief Stop the module from playing
//------------------------------------------------------------------------------
void CSoundDrv::StopModule(void)
{
	GLOBAL_GameTarget->StopModule();
	m_CurrentModule = 0;
}

//------------------------------------------------------------------------------
//! \brief Play the module, or restart playing the module
//!
//! 	\param id = Module id (SMOD_xxx id)
//------------------------------------------------------------------------------
void CSoundDrv::PlayModule(int id)
{
	m_CurrentModule = id;
	GLOBAL_GameTarget->PlayModule(id);
}

//------------------------------------------------------------------------------
//! \brief Play the sample
//!
//! 	\param id = SND_xxx sound id
//!	\param pos = Sample Position to use 0 to SCR_WIDTH-1. (Default = SCR_WIDTH/2)
//!	\param rate = sound frequency (Default = 0 = Use sample default)
//------------------------------------------------------------------------------
void CSoundDrv::PlaySample(int id, int pos, int rate)
{
	if (!rate)
{
		switch (id)
		{
			case SND_CAR:
				rate = SRATE_1e0;
				break;
			case SND_TRAIN:
				rate = SRATE_1e0;
				break;
			case SND_DUCK:
				rate = SRATE_1e0;
				break;
			case SND_PICKUP1:
				rate = SRATE_1e0;
				break;
			case SND_TRIBBLE:
				rate = SRATE_1e0;
				break;
			case SND_HURRY:
				rate = SRATE_1e0;
				break;
			case SND_DAY:
				rate = SRATE_300;
				break;
			case SND_CRYING:
				rate = SRATE_1e0;
				break;
			case SND_DIE2:	
				rate = SRATE_1e0;
				break;
			case SND_SPIT:
				rate = SRATE_1e0;
				break;
			case SND_SPLAT:
				rate = SRATE_1e0;
				break;
			case SND_BLOW:
				rate = SRATE_a0;
				break;
			case SND_TWINKLE:
				rate = SRATE_1e0;
				break;
			case SND_FINLEV1:
				rate = SRATE_1e0;
				break;
			case SND_PSTAR:
				rate = SRATE_150;
				break;
			case SND_XYLO:
				rate = SRATE_200;
				break;
			case SND_CARDSOUND:
				rate = SRATE_150;
				break;
			case SND_BOWLINGSOUND:
				rate = SRATE_160;
				break;
			case SND_CANDLESOUND:
				rate = SRATE_1e0;
				break;
			case SND_MARBLESOUND:
				rate = SRATE_1e0;
				break;
			case SND_TAPSOUND:
				rate = SRATE_1e0;
				break;
			case SND_OILSOUND:
				rate = SRATE_ae0;
				break;
			case SND_SPININGTOPSOUND:
				rate = SRATE_160;
				break;
			case SND_WINGSSOUND:
				rate = SRATE_1e0;
				break;
			case SND_MOONSOUND:
				rate = SRATE_1e0;
				break;
			case SND_MASKSOUND:
				rate = SRATE_170;
				break;
			case SND_REDSTARSOUND:
				rate = SRATE_a0;
				break;
			case SND_TURBOSOUND:
				rate = SRATE_150;
				break;
			case SND_CHICKENSOUND:
				rate = SRATE_1e0;
				break;
			case SND_FEATHERSOUND:
				rate = SRATE_150;
				break;
			case SND_WPOTIONSOUND:
				rate = SRATE_280;
				break;
			case SND_COOKIESOUND:
				rate = SRATE_1e0;
				break;
			default:
				rate = SRATE_1e0;
		}
	}

	// Scale the position from 0 to 255
	if (pos<0) pos = 0;
	if (pos>=SCR_WIDTH) pos = SCR_WIDTH-1;

	pos = (pos << 8) / SCR_WIDTH;

	GLOBAL_GameTarget->PlaySample(id, pos, rate);

}

//------------------------------------------------------------------------------
//! \brief Update the current module (ie restart the module if it has stopped)
//------------------------------------------------------------------------------
void CSoundDrv::UpdateModule(void)
{
	if (m_CurrentModule)	// Is something playing
	{
		GLOBAL_GameTarget->UpdateModule(m_CurrentModule);
	}
}


