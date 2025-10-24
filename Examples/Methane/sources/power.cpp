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
// Methane Brothers PowerUp Object (Source File)
//------------------------------------------------------------------------------

#include "stdio.h"
#include "stdlib.h"
#include "misc.h"
#include "global.h"
#include "bitgroup.h"
#include "objtypes.h"
#include "game.h"
#include "player.h"
#include "goodie.h"
#include "power.h"

//------------------------------------------------------------------------------
// The PowerUp Object
//------------------------------------------------------------------------------

static OBJSIZE power_size = { 0, 0, 32, 28+3 };
static OBJMOVE power_move = {0x2a0,0x100,6*256,0xf*256,0x1d0,0xf*256,0xf*256};

#define POWERUP_LENGTH	260
#define POWERUP_DELAY	120		//Delay until powerup appears

//------------------------------------------------------------------------------
// Powerup animations
//------------------------------------------------------------------------------
static int anim_power_turbo[]=
	{
	SPR_POWER_TURBO1,
	SPR_POWER_TURBO2,
	ANM_RESTART
	};
static int anim_power_candle[]=
	{
	SPR_POWER_CANDLE1,
	SPR_POWER_CANDLE2,
	SPR_POWER_CANDLE3,
	SPR_POWER_CANDLE4,
	ANM_RESTART
	};
static int anim_power_mask[]=
	{
	SPR_MASK_R1,
	SPR_MASK_R1,
	SPR_MASK_R2,
	SPR_MASK_R2,
	SPR_MASK_R3,
	SPR_MASK_R3,
	SPR_MASK_R4,
	SPR_MASK_R4,
	ANM_RESTART
	};
static int anim_power_spin[]=
	{
	SPR_POWER_SPIN1,
	SPR_POWER_SPIN1,
	SPR_POWER_SPIN2,
	SPR_POWER_SPIN2,
	SPR_POWER_SPIN3,
	SPR_POWER_SPIN3,
	SPR_POWER_SPIN4,
	SPR_POWER_SPIN4,
	ANM_RESTART
	};

#define IMMUNE_FROM_POTION	600

//------------------------------------------------------------------------------
//! \brief Initialise the power_up
//------------------------------------------------------------------------------
CPowerUpObj::CPowerUpObj()
{
	InitSize(power_size);
	InitMovement(power_move);
	m_PowerDelay = 0;
	m_Sample = 0;
}

//------------------------------------------------------------------------------
//! \brief Draw the spring object
//------------------------------------------------------------------------------
void CPowerUpObj::Draw( void )
{
	if (!m_PowerDelay)
	{
		m_pGame->m_Sprites.Draw( m_Frame, m_XPos, m_YPos );
	}
}

//------------------------------------------------------------------------------
//! \brief Setup the power_up
//!
//! 	\param type = Power up type (PUP_xxx)
//------------------------------------------------------------------------------
void CPowerUpObj::Setup(int type)
{
	m_Frame = SPR_POWER_YELLOWSTAR;
	m_PowerUpType = type;
	m_PowerCnt = POWERUP_LENGTH;
	m_PowerDelay = POWERUP_DELAY;
	m_pSequence = 0;
	switch (type)
	{
		case PUP_OIL:
			m_Frame = SPR_POWER_OIL;
			m_pGame->m_Sprites.Load(SPR_POWER_OIL);
			m_pGame->m_Sprites.LoadRange(SPR_OIL_1, SPR_OIL_LARGE);
			m_Sample = SND_OILSOUND;
			break;
		case PUP_TAP:
			m_Frame = SPR_POWER_TAP;
			m_pGame->m_Sprites.Load(SPR_POWER_TAP);
			m_pGame->m_Sprites.LoadRange(SPR_WATER_1, SPR_WATER_LARGE);
			m_Sample = SND_TAPSOUND;
			break;
		case PUP_TURBO:
			m_Frame = SPR_POWER_TURBO1;
			m_pSequence = anim_power_turbo;
			m_pGame->m_Sprites.LoadRange(SPR_POWER_TURBO1,SPR_POWER_TURBO2);
			m_Sample = SND_TURBOSOUND;
			break;
		case PUP_SPININGTOP:
			m_Frame = SPR_POWER_SPIN1;
			m_pSequence = anim_power_spin;
			m_pGame->m_Sprites.LoadRange(SPR_POWER_SPIN1,SPR_POWER_SPIN4);
			m_Sample = SND_SPININGTOPSOUND;
			break;
		case PUP_CANDLE:
			m_Frame = SPR_POWER_CANDLE1;
			m_pSequence = anim_power_candle;
			m_pGame->m_Sprites.LoadRange(SPR_POWER_CANDLE1,SPR_POWER_CANDLE4);
			m_pGame->m_Sprites.LoadRange(SPR_FIRE_F1, SPR_FIRE_A4);
			m_Sample = SND_CANDLESOUND;
			break;
		case PUP_MARBLE:
			m_Frame = SPR_POWER_MARBLE;
			m_pGame->m_Sprites.Load(SPR_POWER_MARBLE);
			m_pGame->m_Sprites.LoadRange(SPR_MARBLE_1, SPR_MARBLE_4);
			m_Sample = SND_MARBLESOUND;
			break;
		case PUP_YELLOWSTAR:
			m_Frame = SPR_POWER_YELLOWSTAR;
			m_pGame->m_Sprites.Load(SPR_POWER_YELLOWSTAR);
			m_Sample = SND_PSTAR;
			break;
		case PUP_MOON:
			m_Frame = SPR_POWER_MOON;
			m_pGame->m_Sprites.Load(SPR_POWER_MOON);
			m_Sample = SND_MOONSOUND;
			break;
		case PUP_XYLOPHONE:
			m_Frame = SPR_POWER_XYLOPHONE;
			m_pGame->m_Sprites.Load(SPR_POWER_XYLOPHONE);
			m_Sample = SND_XYLO;
			break;
		case PUP_FEATHER:
			m_Frame = SPR_POWER_FEATHER;
			m_pGame->m_Sprites.Load(SPR_POWER_FEATHER);
			m_Sample = SND_FEATHERSOUND;
			break;
		case PUP_WHITEPOTION:
			m_Frame = SPR_POWER_WHITEPOTION;
			m_pGame->m_Sprites.Load(SPR_POWER_WHITEPOTION);
			m_Sample = SND_WPOTIONSOUND;
			break;
		case PUP_WINGS:
			m_Frame = SPR_POWER_WINGS;
			m_pGame->m_Sprites.Load(SPR_POWER_WINGS);
			m_Sample = SND_WINGSSOUND;
			break;
		case PUP_COOKIE:
			m_Frame = SPR_POWER_COOKIE;
			m_pGame->m_Sprites.Load(SPR_POWER_COOKIE);
			m_Sample = SND_COOKIESOUND;
			break;
		case PUP_CHICKEN:
			m_Frame = SPR_POWER_CHICKEN;
			m_pGame->m_Sprites.LoadRange(SPR_POWER_CHICKEN,SPR_POWER_CHICKENEGG);
			m_Sample = SND_CHICKENSOUND;
			break;
		case PUP_PURPLESTAR:
			m_Frame = SPR_POWER_PURPLESTAR;
			m_pGame->m_Sprites.Load(SPR_POWER_PURPLESTAR);
			m_Sample = SND_PSTAR;
			break;
		case PUP_GREENSTAR:
			m_Frame = SPR_POWER_GREENSTAR;
			m_pGame->m_Sprites.Load(SPR_POWER_GREENSTAR);
			m_Sample = SND_PSTAR;
			break;
		case PUP_REDSTAR:
			m_Frame = SPR_POWER_REDSTAR;
			m_pGame->m_Sprites.Load(SPR_POWER_REDSTAR);
			m_pGame->m_Sprites.LoadRange(SPR_FIREL_1, SPR_FIRER_4);
			m_Sample = SND_REDSTARSOUND;
			break;
		case PUP_MASK:
			m_Frame = SPR_MASK_R1;
			m_pSequence = anim_power_mask;
			m_pGame->m_Sprites.LoadRange(SPR_MASK_R1, SPR_MASK_L4);
			m_Sample = SND_MASKSOUND;
			break;
		case PUP_BOWLING:
			m_Frame = SPR_BOWLING_ROLL1;
			m_pGame->m_Sprites.LoadRange(SPR_BOWLING_ROLL1, SPR_BOWLING_SKITTLE);
			m_Sample = SND_BOWLINGSOUND;
			break;
		case PUP_SAUSAGE:
			m_Frame = SPR_POWER_SAUSAGE;
			m_pGame->m_Sprites.Load(SPR_POWER_SAUSAGE);
			m_pGame->m_Sprites.LoadRange(SPR_SAUSAGE_1, SPR_SAUSAGE_8);
			m_Sample = SND_SPLAT;
			break;
	}

}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CPowerUpObj::Do( void )
{
	CPlayerObj *pobj;
	int cnt;

	if (m_PowerDelay)
	{
		if (m_pGame->m_BonusLevelFlag)		// No delay in bonus rooms
		{
			m_PowerDelay = 0;

		}else
		{
			m_PowerDelay--;
			return;
		}

	}

	pobj = HitPlayer();
	if (pobj)			// Collision occured?
	{
	  	m_pGame->m_pSound->PlaySample(m_Sample, m_XPos);
		switch (m_PowerUpType)
		{
			case PUP_OIL:
				COilObj *tobj;
				for (cnt=0; cnt<16; cnt++)
				{
					SMB_NEW(tobj,COilObj);
					if (tobj)
					{
						m_pGame->m_GoodieList.Attach(tobj, OBJ_OIL, m_pGame);
						tobj->Setup(m_XPos+16, m_YPos);
					}
				}

				break;
			case PUP_MARBLE:
				CMarbleObj *marobj;
				for (cnt=0; cnt<4; cnt++)
				{
					SMB_NEW(marobj,CMarbleObj);
					if (marobj)
					{
						m_pGame->m_GoodieList.Attach(marobj, OBJ_MARBLE, m_pGame);
						marobj->Setup(m_XPos+16, m_YPos);
					}
				}

				break;
			case PUP_TAP:
				CWaterObj *wobj;
				for (cnt=0; cnt<16; cnt++)
				{
					SMB_NEW(wobj,CWaterObj);
					if (wobj)
					{
						m_pGame->m_GoodieList.Attach(wobj, OBJ_WATER, m_pGame);
						wobj->Setup(m_XPos+16, m_YPos);
					}
				}

				break;
			case PUP_CANDLE:
				CFireObj *fireobj;
				for (cnt=0; cnt<8; cnt++)
				{
					SMB_NEW(fireobj,CFireObj);
					if (fireobj)
					{
						m_pGame->m_GoodieList.Attach(fireobj, OBJ_FIRE, m_pGame);
						fireobj->Setup(m_XPos+16, m_YPos);
					}
				}

				break;
			case PUP_TURBO:
				pobj->m_YMass = 350;
				pobj->m_YPower = 16*256;
				pobj->m_Max_YSpeed = 10 * 256;
				pobj->m_YCentre = 10 * 256;
				pobj->m_XMass = 250;
				pobj->m_Max_XSpeed = 8 * 256;
				break;
			case PUP_GREENSTAR:
				pobj->m_PlungeStepRate = 3;
				pobj->m_PlungeSuckerRate = 2;
				pobj->m_GasLength = 14;
				pobj->m_GasStep = 8;
				break;
			case PUP_SPININGTOP:
				pobj->m_GiddyFlag = 1;
				break;
			case PUP_WHITEPOTION:
				pobj->m_Immune = IMMUNE_FROM_POTION;
				break;
			case PUP_PURPLESTAR:
				pobj->m_PlungeStepRate = 3;
				pobj->m_PlungeSuckerRate = 2;
				pobj->m_GasStep = 8;
				break;
			case PUP_YELLOWSTAR:
				pobj->m_GasLength = 14;
				pobj->m_PlungeSuckerRate = 2;
				break;
			case PUP_MASK:
				pobj->m_Mask = 1;
				break;
			case PUP_MOON:
				m_pGame->m_LevelSkip = 4;
				pobj->m_GasLength = 14;
				break;
			case PUP_REDSTAR:
				pobj->m_GasFireFlag = 1;
				break;
			case PUP_SAUSAGE:
				for (cnt=0; cnt<48; cnt++)
				{
					m_pGame->MakeGoodie(m_XPos+16, m_YPos,
						GOODIE_SAUSAGE, 0,
						(rand() & ((256*16)-1)) - 256*8,
						(rand() & ((256*16)-1)) - 256*8);
				}
				break;
			case PUP_FEATHER:
				pobj->m_YCentre = 3 * 256;
				break;

			case PUP_WINGS:
				pobj->m_YCentre = 0;
				break;

			case PUP_XYLOPHONE:
				m_pGame->SetBonusLevel(BLEV_MUSICROOM);
				break;

			case PUP_COOKIE:
				DestroyAllBaddies();
				break;

			case PUP_CHICKEN:
				ConvertBaddies(OBJ_CHICKENEGG);
				break;

			case PUP_BOWLING:
				CBowlingObj *bowlobj;
				SMB_NEW(bowlobj,CBowlingObj);
				if (bowlobj)
				{
					m_pGame->InitFrkObject(bowlobj, OBJ_BOWLING, m_XPos, m_YPos, &m_pGame->m_GasList);
					bowlobj->m_YInert = -6*256;
					ConvertBaddies(OBJ_SKITTLE);
				}

				break;
		}
		m_pGame->CreateCloud( m_XPos, m_YPos );
		DeleteObject();
		return ;
	}

	m_PowerCnt--;
	if (m_PowerCnt<0)
	{
		m_pGame->CreateCloud( m_XPos, m_YPos );
		DeleteObject();
		return ;
	}
	MoveObject();
	if (m_pSequence) Animate(256);
}

//------------------------------------------------------------------------------
//! \brief Convert baddies into an object type
//!
//! 	\param objtype = The OBJ_xxx type
//------------------------------------------------------------------------------
void CPowerUpObj::ConvertBaddies( int objtype )
{
	CLinkObject *pobj;
	CLinkObject *nobj;

	pobj = m_pGame->m_BaddieList.m_pFirst;			// Start address

	while (pobj)			// For all objects
	{

		nobj = pobj;
		pobj = pobj->m_pNext;

		if (nobj->m_Flags&FLAG_CATCHABLE)	// Correct object type?
		{
			if (objtype==OBJ_SKITTLE)
			{
				CSkittleObj *sobj;
				SMB_NEW(sobj,CSkittleObj);
				if (sobj)
				{
					m_pGame->InitFrkObject(sobj, OBJ_SKITTLE,
						nobj->m_XPos, nobj->m_YPos, &m_pGame->m_BaddieList);
				}
			}else
			{
				CChickenEggObj *sobj2;
				SMB_NEW(sobj2,CChickenEggObj);
				if (sobj2)
				{
					m_pGame->InitFrkObject(sobj2, OBJ_CHICKENEGG,
						nobj->m_XPos, nobj->m_YPos, &m_pGame->m_BaddieList);
				}
			}

			nobj->DeleteObject();
		}
	}

}

//------------------------------------------------------------------------------
//! \brief Cookie - Destroy All Baddies
//------------------------------------------------------------------------------
void CPowerUpObj::DestroyAllBaddies(void)
{
	CLinkObject *pobj;
	CSuckable *nobj;

	pobj = m_pGame->m_BaddieList.m_pFirst;			// Start address

	while (pobj)			// For all objects
	{

		nobj = (CSuckable *) pobj;
		pobj = pobj->m_pNext;

		if (nobj->m_Flags&FLAG_CATCHABLE)	// Correct object type?
		{
			nobj->SetExplode();
		}
	}
}

