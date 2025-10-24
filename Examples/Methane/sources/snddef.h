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
// Audio Header File
//------------------------------------------------------------------------------

#ifndef _SDF_H
#define _SDF_H 1

#include "global.h"

//------------------------------------------------------------------------------
// Sound channel codes
//------------------------------------------------------------------------------
#define CHANNEL_LEFT	0
#define CHANNEL_RIGHT	1

//------------------------------------------------------------------------------
// A fudge to insert a wave header (for MikMod)
//------------------------------------------------------------------------------
#define WAVE_HEADER_SIZE	58
#define WAVE_TAIL_SIZE	1

//------------------------------------------------------------------------------
// Amiga sample rate conversions
// PLEASE NOTE - THE VARIABLE NAMES NO LONGER MATCH THE AMIGA NOTE VALUE
//------------------------------------------------------------------------------
#define SRATE_1e0	8000
#define SRATE_300	6000
#define SRATE_a0	12000
#define SRATE_150	10000
#define SRATE_160	11000
#define SRATE_170	11000
#define SRATE_280	5000
#define SRATE_200	7800
#define SRATE_ae0	4000

//------------------------------------------------------------------------------
// Modules
//------------------------------------------------------------------------------
#define MODULE_START_NUMBER	4000

#define SMOD_TUNE1		4000
#define SMOD_TUNE2		4001
#define SMOD_BOSS		4002
#define SMOD_COMPLETE		4003
#define SMOD_TITLE		4004
#define SMOD_EMPTY		4005

#define MODULE_END_NUMBER	4005
#define MODULE_COUNT		(1 + MODULE_END_NUMBER - MODULE_START_NUMBER)

//------------------------------------------------------------------------------
// Samples
//------------------------------------------------------------------------------

#define SND_START_NUMBER	(5005)

#define SND_CAR			5005
#define SND_TRAIN		5006
#define SND_DUCK		5007
#define SND_PICKUP1		5008
#define SND_TRIBBLE		5009
#define SND_HURRY		5010
#define SND_DAY			5011
#define SND_CRYING		5012
#define SND_DIE2		5013
#define SND_SPIT		5014
#define SND_SPLAT		5015
#define SND_BLOW		5016
#define SND_TWINKLE		5017
#define SND_FINLEV1		5018
#define SND_PSTAR		5019
#define SND_XYLO		5020
#define SND_CARDSOUND		5021
#define SND_BOWLINGSOUND	5022
#define SND_CANDLESOUND		5023
#define SND_MARBLESOUND		5024
#define SND_TAPSOUND		5025
#define SND_OILSOUND		5026
#define SND_SPININGTOPSOUND	5027
#define SND_WINGSSOUND		5028
#define SND_MOONSOUND		5029
#define SND_MASKSOUND		5030
#define SND_REDSTARSOUND	5031
#define SND_TURBOSOUND		5032
#define SND_CHICKENSOUND	5033
#define SND_FEATHERSOUND	5034
#define SND_WPOTIONSOUND	5035
#define SND_COOKIESOUND		5036

#define SND_END_NUMBER  	(5036)

#define SND_COUNT		(1 + SND_END_NUMBER - SND_START_NUMBER)

extern MODULE_RESOURCE_DATA MethaneModuleData[MODULE_COUNT];
extern SAMPLE_RESOURCE_DATA MethaneSampleData[SND_COUNT];

#endif

