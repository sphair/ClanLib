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
// Methane Brothers Main Game (Source File)
//------------------------------------------------------------------------------

#include "global.h"
#include "gfxdef.h"
#include "game.h"
#include "player.h"
#include "objtypes.h"
#include "baddie.h"
#include "goodie.h"
#include "boss.h"
#include "misc.h"
#include "gasobj.h"
#include "power.h"
#include "weapon.h"
#include "target.h"
#include <stdlib.h>

//------------------------------------------------------------------------------
// The Game Version Number
//------------------------------------------------------------------------------
static char GameVersionNumber[] = "VERSION 1.5.0";

#define DELAY_BEFORE_NEXT_LEVEL 140

#define DELAY_BEFORE_DOODAHDAY	1200

#define JEWEL_DIE_COUNT	13	// Number of times the player has to
								// stay alive before enterering the jewel room
#define POWERROOM_LENGTH	120	// Length (in frames) of the power room

#define GAMEOVER_DELAY		96	// How long the gameover message is on screen for

#define HISCREEN_SHOW_DELAY 100

//------------------------------------------------------------------------------
//! \brief Game Constructor
//------------------------------------------------------------------------------
CGame::CGame()
{
	int cnt;
	HISCORES *hs;

	hs = m_HiScores;
	for (cnt=0; cnt<MAX_HISCORES; cnt++, hs++)
	{
		hs->score = 0;
		hs->name[0] = ' ';
		hs->name[1] = ' ';
		hs->name[2] = ' ';
		hs->name[3] = ' ';
	}
	m_HiOffset = 0;
}

//------------------------------------------------------------------------------
//! \brief Initialise the main game (setup the pointers)
//!
//! (This function MUST be called before other CGame functions)
//!
//! 	\param tptr = The main target
//!	\param jptr1 = Joystick 1
//!	\param jptr2 = Joystick 2
//------------------------------------------------------------------------------
void CGame::Init(CGameTarget *tptr, JOYSTICK *jptr1, JOYSTICK *jptr2)
{
	m_pGameTarget = tptr;
	m_pJoy1 = jptr1;
	m_pJoy2 = jptr2;
	m_Sprites.m_pGame = this;
	m_pSound = &m_Sound;
	m_PUP_Cnt = 0;
	m_MainCommand = MC_TITLE;

}

//------------------------------------------------------------------------------
//! \brief Start the main game 
//------------------------------------------------------------------------------
void CGame::StartGame(void)
{
	m_LevelNumber = 1;
	m_CurrentTune = -1;

	InitTitleScreen();
}

//------------------------------------------------------------------------------
//! \brief The Program Main Loop 
//------------------------------------------------------------------------------
void CGame::MainLoop(void)
{
	m_pSound->UpdateModule();

	if (ControlFade())
	{
		switch (m_MainCommand)
		{
			case (MC_GAME):
				GameLoop();
				break;
			case (MC_COMPLETED):
				CompletedLoop();
				break;
			case (MC_TITLE):
				TitleScreenLoop();
				return;
			case (MC_HIGHSCREEN):
				HighScreenLoop();
				return;
			case (MC_GETPLAYER):
				GetPlayerNameLoop();
				return;
		}
	}

	m_pGameTarget->RedrawScreen();
}

//------------------------------------------------------------------------------
//! \brief The Game Loop
//------------------------------------------------------------------------------
void CGame::GameLoop(void)
{
	// Cheat Mode Controller

	if ( (m_pJoy1->next_level) || (m_pJoy2->next_level) )
	{
		m_pJoy1->next_level = 0;
		m_pJoy2->next_level = 0;
		m_Map.GetMap(++m_LevelNumber);
		InitSpriteList();
	}

	// -------

	m_MainCounter++;

	CheckComplete();
	CheckDooDahDay();
	CheckExtras();
	CheckForGameOver();
	if (m_MainCommand!=MC_GAME) return;	// Exit now when completed

	m_FontList.DoAll();
	m_PlayerList.DoAll();
	m_GasList.DoAll();
	m_BaddieList.DoAll();
	m_GoodieList.DoAll();
	m_ExtraList.DoAll();

	// Do not redraw map etc. when fading has started
	if (m_FadeType!=FADE_COMPLETE)
	{
		if (m_FadeFlag==FADE_FLAG_WAIT) return;
	}

	m_Map.Draw();

	m_ExtraList.DrawAll();
	m_GoodieList.DrawAll();
	m_BaddieList.DrawAll();
	m_GasList.DrawAll();
	m_PlayerList.DrawAll();
	m_FontList.DrawAll();

	DrawPlayersInfo();
	CheckExtras2();
}

//------------------------------------------------------------------------------
//! \brief Start a link object (PRIVATE - USED BY ::StartFRKObject())
//!
//!	\param ptr = Pointer to the object
//! 	\param type = object type (OBJ_xxx) - Define in objtypes.h
//!	\param xpos = X Position
//!	\param ypos = Y Position
//!	\param objlist = Object list base to attach to
//------------------------------------------------------------------------------
void CGame::InitFrkObject(CLinkObject *ptr, int type, int xpos, int ypos, CObjectList *objlist)
{
	ptr->m_OldXPos = ptr->m_XPos = xpos;
	ptr->m_OldYPos = ptr->m_YPos = ypos;
	objlist->Attach(ptr, type, this);
}

//------------------------------------------------------------------------------
//! \brief Start a FRK (sprite) Object
//!
//! 	\param type = FRK type (as FRK_xxx) - Defined in objtypes.h
//!	\param xpos = X Position
//!	\param ypos = Y Position
//------------------------------------------------------------------------------
void CGame::StartFRKObject(int type, int xpos, int ypos)
{
	switch (type)
	{
		case (FRK_BUG):
		{
			CBugObj *pobj;
			SMB_NEW(pobj,CBugObj);
			if (pobj)
			{
				InitFrkObject(pobj, OBJ_BUG, xpos, ypos, &m_BaddieList);
				pobj->LoadGfx();
				pobj->PrepareGlide();
			
			}
			break;
		}
		case (FRK_SUCKER):
		{
			CSuckerObj *pobj;
			SMB_NEW(pobj,CSuckerObj);
			if (pobj)
			{
				ypos = ypos - 7;
				ypos = ypos & (~7);
				xpos = xpos & (~7);
				InitFrkObject(pobj, OBJ_SUCKER, xpos, ypos, &m_BaddieList);
				pobj->LoadGfx();
				pobj->PrepareGlide();

			}
			break;
		}
		case (FRK_B_BLOCK):
		{
			CBlockObj *tobj;
			SMB_NEW(tobj,CBlockObj);
			if (tobj)
			{
				ypos = ypos - 15;
				ypos = ypos & (~7);
				InitFrkObject(tobj, OBJ_BLOCK, xpos, ypos, &m_GasList);
				tobj->LoadGfx();
			}
			break;
		}

		case (FRK_HOTSPOT):
		{
			CHotSpotObj *pobj;
			SMB_NEW(pobj,CHotSpotObj);
			if (pobj)
			{
				InitFrkObject(pobj, OBJ_HOTSPOT, xpos, ypos, &m_GoodieList);
			}
			break;
		}
		case (FRK_DWARF):
		{
			CDwarfObj *pobj;
			SMB_NEW(pobj,CDwarfObj);
			if (pobj)
			{
				InitFrkObject(pobj, OBJ_DWARF, xpos, ypos, &m_BaddieList);
				pobj->LoadGfx();
				pobj->PrepareGlide();

			}
			break;
		}
		case (FRK_WHIRLIGIG):
		{
			CWhirlyObj *pobj;
			SMB_NEW(pobj,CWhirlyObj);
			if (pobj)
			{
				InitFrkObject(pobj, OBJ_WHIRLY, xpos, ypos, &m_BaddieList);
				pobj->LoadGfx();
				pobj->PrepareGlide();

			}
			break;
		}
		case (FRK_CLOWN):
		{
			CClownObj *pobj;
			SMB_NEW(pobj,CClownObj);
			if (pobj)
			{
				InitFrkObject(pobj, OBJ_CLOWN, xpos, ypos, &m_BaddieList);
				pobj->LoadGfx();
				pobj->PrepareGlide();

			}
			break;
		}
		case (FRK_MECHOBUG):
		{
			CMBugObj *pobj;
			SMB_NEW(pobj,CMBugObj);
			if (pobj)
			{
				InitFrkObject(pobj, OBJ_MBUG, xpos, ypos, &m_BaddieList);
				pobj->LoadGfx();
				pobj->PrepareGlide();

			}
			break;
		}
		case (FRK_ZOOM):
		{
			CZoomObj *pobj;
			SMB_NEW(pobj,CZoomObj);
			if (pobj)
			{
				InitFrkObject(pobj, OBJ_ZOOM, xpos, ypos, &m_BaddieList);
				pobj->LoadGfx();
				pobj->PrepareGlide();

			}
			break;
		}
		case (FRK_JUMP):
		{
			CJumpObj *pobj;
			SMB_NEW(pobj,CJumpObj);
			if (pobj)
			{
				InitFrkObject(pobj, OBJ_JUMP, xpos, ypos, &m_BaddieList);
				pobj->LoadGfx();
				pobj->PrepareGlide();

			}
			break;
		}
		case (FRK_DOOFUS):
		{
			CDoofusObj *pobj;
			SMB_NEW(pobj,CDoofusObj);
			if (pobj)
			{
				InitFrkObject(pobj, OBJ_DOOFUS, xpos, ypos, &m_BaddieList);
				pobj->LoadGfx();
				pobj->PrepareGlide();

			}
			break;
		}
		case (FRK_SPIKE):
		{
			CSpikeObj *pobj;
			SMB_NEW(pobj,CSpikeObj);
			if (pobj)
			{
				InitFrkObject(pobj, OBJ_SPIKE, xpos, ypos, &m_BaddieList);
				pobj->LoadGfx();
				pobj->PrepareGlide();

			}
			break;
		}
		case (FRK_PLAYER1):
		{
			CPlayerObj *play1;
			play1 = GetPlayer(OBJ_PLAYER_ONE);
			if (play1)
			{
				ypos -=5;		// As in the amiga source!
				play1->m_OrigXPos = play1->m_OldXPos = play1->m_XPos = xpos;
				play1->m_OrigYPos = play1->m_OldYPos = play1->m_YPos = ypos;
			}
			break;
		}
		case (FRK_PLAYER2):
		{
			CPlayerObj *play2;
			play2 = GetPlayer(OBJ_PLAYER_TWO);
			if (play2)
			{
				ypos -=5;		// As in the amiga source!
				play2->m_OrigXPos = play2->m_OldXPos = play2->m_XPos = xpos;
				play2->m_OrigYPos = play2->m_OldYPos = play2->m_YPos = ypos;
			}
			break;
		}
		case (FRK_CLOWNBOSS):
		{
			CClownBoss *pobj;
			SMB_NEW(pobj,CClownBoss);
			if (pobj)
			{
				InitFrkObject(pobj, OBJ_CLOWNBOSS, xpos, ypos, &m_BaddieList);
				pobj->LoadGfx();
				m_BossLevel = 1;
			}
			PlayModule(SMOD_BOSS);
			break;
		}

		case (FRK_CRABBOSS):
		{
			CCrabBoss *pobj;
			SMB_NEW(pobj,CCrabBoss);
			if (pobj)
			{
				InitFrkObject(pobj, OBJ_CRABBOSS, xpos, ypos, &m_BaddieList);
				pobj->LoadGfx();
				m_BossLevel = 2;
			}
			PlayModule(SMOD_BOSS);
			break;
		}

		case (FRK_CLOCKBOSS):
		{
			CClockBoss *pobj;
			SMB_NEW(pobj,CClockBoss);
			if (pobj)
			{
				InitFrkObject(pobj, OBJ_CLOCKBOSS, xpos, ypos, &m_BaddieList);
				pobj->LoadGfx();
				m_BossLevel = 3;
			}
			PlayModule(SMOD_BOSS);
			CBowlingObj *tobj;
			SMB_NEW(tobj,CBowlingObj);
			if (tobj)
			{
				xpos = 140;
				ypos = 20;
				InitFrkObject(tobj, OBJ_BOWLING, xpos, ypos, &m_GasList);
				tobj->LoadGfx();
			}
			break;
		}

		case (FRK_TOM):
		{
			CTomatoObj *tobj;
			SMB_NEW(tobj,CTomatoObj);
			if (tobj)
			{
				InitFrkObject(tobj, OBJ_TOMATO, xpos, ypos, &m_GasList);
				tobj->m_OrigXPos = tobj->m_XPos;
				tobj->m_OrigYPos = tobj->m_YPos;
				tobj->LoadGfx();
			}
			CTubeObj *pobj;
			SMB_NEW(pobj,CTubeObj);
			if (pobj)
			{
				InitFrkObject(pobj, OBJ_TUBE, xpos, 0, &m_GasList);
				pobj->LoadGfx();
			}
			break;
		}
		case (FRK_B_SPRING):
		{
			CSpringObj *tobj;
			SMB_NEW(tobj,CSpringObj);
			if (tobj)
			{
				ypos = ypos - 7;
				ypos = ypos & (~7);
				InitFrkObject(tobj, OBJ_SPRING, xpos, ypos, &m_GasList);
				tobj->LoadGfx();
			}
			break;
		}

		case (FRK_FLOWER):
		{
			m_FlowerFlag = 1;
			m_Sprites.LoadRange(SPR_FLOWERS_SEED, SPR_FLOWERS_D3);
			break;
		}

		case (FRK_CHEST):
		{
			CChestObj *cobj;
			SMB_NEW(cobj,CChestObj);
			if (cobj)
			{
				InitFrkObject(cobj, OBJ_CHEST, xpos , ypos, &m_ExtraList);
				cobj->LoadGfx();
			}
			break;
		}

		case (FRK_ENDBOSS):
		{
			m_BossLevel = 4;
			PlayModule(SMOD_BOSS);
			CEndBoss *cobj;
			CEndBoss *cobj_last = 0;
			// The last boss is made up of 8 segments, numbered 0-7
			for (int cnt=7; cnt>=0; cnt--)
			{
				SMB_NEW(cobj,CEndBoss);
				if (cobj)
				{
					InitFrkObject(cobj, OBJ_ENDBOSS, xpos , ypos, &m_BaddieList);
					if (!cnt) cobj->LoadGfx();
					cobj->m_pNextSeg = cobj_last;
					if (cobj_last) cobj_last->m_pLastSeg = cobj;
					cobj->Setup(cnt);
					xpos-=32;
					if (xpos<0) xpos+=SCR_WIDTH;
				}
				cobj_last = cobj;
			}
			break;
		}

		case (FRK_B_HARDHAT):
		case (FRK_B_TAG):
		case (FRK_B_PINKFROG):
		case (FRK_B_YELLOWSTAR):
		{
			MakePowerUp(PUP_YELLOWSTAR, xpos, ypos);
			break;
		}
		case (FRK_B_TURBO):
		{
			MakePowerUp(PUP_TURBO, xpos, ypos);
			break;
		}
		case (FRK_B_PURPLESTAR):
		{
			MakePowerUp(PUP_PURPLESTAR, xpos, ypos);
			break;
		}
		case (FRK_B_GREENSTAR):
		{
			MakePowerUp(PUP_GREENSTAR, xpos, ypos);
			break;
		}
		case (FRK_B_SAUSAGE):
		{
			MakePowerUp(PUP_SAUSAGE, xpos, ypos);
			break;
		}
		case (FRK_B_OIL):
		{
			MakePowerUp(PUP_OIL, xpos, ypos);
			break;
		}
		case (FRK_B_TAP):
		{
			MakePowerUp(PUP_TAP, xpos, ypos);
			break;
		}
		case (FRK_B_REDSTAR):
		{
			MakePowerUp(PUP_REDSTAR, xpos, ypos);
			break;
		}
		case (FRK_B_BOWLINGBALL):
		{
			MakePowerUp(PUP_BOWLING, xpos, ypos);
			break;
		}
		case (FRK_B_FEATHER):
		{
			MakePowerUp(PUP_FEATHER, xpos, ypos);
			break;
		}
		case (FRK_B_WINGS):
		{
			MakePowerUp(PUP_WINGS, xpos, ypos);
			break;
		}
		case (FRK_B_COOKIE):
		{
			MakePowerUp(PUP_COOKIE, xpos, ypos);
			break;
		}
		case (FRK_B_CHICKEN):
		{
			MakePowerUp(PUP_CHICKEN, xpos, ypos);
			break;
		}
		case (FRK_B_SPININGTOP):
		{
			MakePowerUp(PUP_SPININGTOP, xpos, ypos);
			break;
		}
		case (FRK_B_CANDLE):
		{
			MakePowerUp(PUP_CANDLE, xpos, ypos);
			break;
		}
		case (FRK_B_WHITEPOTION):
		{
			MakePowerUp(PUP_WHITEPOTION, xpos, ypos);
			break;
		}
		case (FRK_B_MOON):
		{
			MakePowerUp(PUP_MOON, xpos, ypos);
			break;
		}
		case (FRK_B_MARBLE):
		{
			MakePowerUp(PUP_MARBLE, xpos, ypos);
			break;
		}
		case (FRK_B_MASK):
		{
			MakePowerUp(PUP_MASK, xpos, ypos);
			break;
		}
		case (FRK_B_GENERATOR):
		{
			CGeneratorObj *pobj;
			SMB_NEW(pobj,CGeneratorObj);
			if (pobj)
			{
				InitFrkObject(pobj, OBJ_GENERATOR, xpos, ypos-14, &m_BaddieList);
				pobj->Setup();
				pobj->LoadGfx();
			}
			break;
		}

		case (FRK_B_XYLOPHONE):
		{
			MakePowerUp(PUP_XYLOPHONE, xpos, ypos);
			break;
		}
		case (FRK_B_CARRYDOOR):
		{
			MakePowerUp(PUP_CARRYDOOR, xpos, ypos);
			break;
		}
		case (FRK_TREASSPOT):
		{
			CTreasSpotObj *hsobj;
			SMB_NEW(hsobj,CTreasSpotObj);
			if (hsobj)
			{
				InitFrkObject(hsobj, OBJ_TREASSPOT, xpos, ypos, &m_GoodieList);
				m_TreasSpotCnt++;
			}
			break;
		}
		case (FRK_SWITCH):
		{
			CSwitchObj *pobj;
			SMB_NEW(pobj,CSwitchObj);
			if (pobj)
			{
				InitFrkObject(pobj, OBJ_SWITCH, xpos, ypos, &m_ExtraList);
			}
			break;
		}
	}

}

//------------------------------------------------------------------------------
//! \brief Initialise the powerup list (used by MakePowerUp and UsePowerUp)
//------------------------------------------------------------------------------
void CGame::InitPowerUp(void)
{
	m_PUP_Cnt = 0;
}

//------------------------------------------------------------------------------
//! \brief Make a powerup (called from startfrkobject())
//!
//!	\param type = PUP_xxx power up types
//!	\param xpos = X Position
//!	\param ypos = Y Position
//------------------------------------------------------------------------------
void CGame::MakePowerUp(int type, int xpos, int ypos)
{
	PUPTYPE *pptr;
	// Too many powerups?
	if (m_PUP_Cnt >= MAX_PUP) return;

	pptr = &m_PUP_Data[m_PUP_Cnt];
	m_PUP_Cnt++;

	pptr->type = type;
	pptr->xpos = xpos;
	pptr->ypos = ypos;

}

//------------------------------------------------------------------------------
//! \brief Choose which powerup to use in the level
//------------------------------------------------------------------------------
void CGame::UsePowerUp(void)
{
	int power_num;
	PUPTYPE *pptr;

	if (m_PUP_Cnt)		// Power Exists?
	{
		power_num = (rand() & 1023) % m_PUP_Cnt;	// Which powerup to use?
		pptr = &m_PUP_Data[power_num];

		power_num = pptr->type;

		// Door type is treated differently
		if (power_num==PUP_CARRYDOOR)
		{
			CCarryDoorObj *dobj;
			SMB_NEW(dobj,CCarryDoorObj);
			if (dobj)
			{
				InitFrkObject(dobj, OBJ_CARRYDOOR, pptr->xpos, pptr->ypos, &m_ExtraList);
				dobj->LoadGfx();
			}

		}else
		{
			CPowerUpObj *tobj;
			SMB_NEW(tobj,CPowerUpObj);
			if (tobj)
			{
				InitFrkObject(tobj, OBJ_POWERUP, pptr->xpos, pptr->ypos, &m_GoodieList);
				tobj->Setup(power_num);
			}
		}
	}
}

//------------------------------------------------------------------------------
//! \brief Create a new number rise object
//------------------------------------------------------------------------------
void CGame::MakeNumRise(int xpos, int ypos, int frame)
{
	CNumRiseObj *pobj;

	SMB_NEW(pobj,CNumRiseObj);
	if (pobj)
	{
		pobj->m_XPos = xpos;
		pobj->m_YPos = ypos;
		pobj->m_Frame = frame;
		m_GoodieList.Attach(pobj, OBJ_NUMRISE, this);
	}

}


//------------------------------------------------------------------------------
//! \brief Load the goodie graphics
//------------------------------------------------------------------------------
void CGame::LoadGoodieGfx( void )
{
	m_Sprites.LoadRange(SPR_GOOD_BOAT, SPR_GOOD_KITE);
	m_Sprites.LoadRange(SPR_FOOD_1, SPR_FOOD_13);
	m_Sprites.LoadRange(SPR_FRUIT_1, SPR_FRUIT_10);
	m_Sprites.LoadRange(SPR_EXP_1, SPR_EXP_13);
	m_Sprites.LoadRange(SPR_NUM_100, SPR_NUM_100000);
	m_Sprites.LoadRange(SPR_RFONT_0, SPR_RFONT_CURSOR);
	m_Sprites.LoadRange(SPR_DAY_1, SPR_DAY_4);
	m_Sprites.LoadRange(SPR_CARD_BACK1, SPR_CARD_SDMOND);
}

//------------------------------------------------------------------------------
//! \brief Make a random goodie (VERSION 1)
//!
//! (Note: gtype is not validated)
//!
//! 	\param xpos = X Initial Coord
//! 	\param ypos = Y Initial Coord
//!	\param gtype = Goodie Type (IE GOODIE_TOYS)
//!	\param xinert = X Initial Inertia
//!	\param yinert = Y Initial Inertia
//------------------------------------------------------------------------------
void CGame::RandGoodie(int xpos, int ypos, int gtype, int xinert, int yinert)
{
	int gid;

	int max;

	max = maxvals[gtype];

	gid = rand() % max;

	MakeGoodie(xpos, ypos, gtype, gid, xinert, yinert);
}

//------------------------------------------------------------------------------
//! \brief Make a random goodie (VERSION 2)
//!
//! (Note: gtype is not validated)
//!
//! 	\param xpos = X Initial Coord
//! 	\param ypos = Y Initial Coord
//!	\param gtype = Goodie Type (IE GOODIE_TOYS)
//!	\param dir = Direction to fly off to (0=RIGHT, Else LEFT) (NOT DIR_LEFT!)
//------------------------------------------------------------------------------
void CGame::RandGoodie(int xpos, int ypos, int gtype, int dir)
{
	int xinert;
	int yinert;

	xinert = ( rand() & 0x7ff );
	yinert = -( rand() & 0xfff );

	xinert += 0x300;
	yinert += 0x1ff;

	if (dir)
	{
		xinert = -xinert;
	}

	RandGoodie(xpos, ypos, gtype, xinert, yinert);
}

//------------------------------------------------------------------------------
//! \brief Make a goodie
//!
//! (Note: gtype, gid variables are not validated)
//!
//! 	\param xpos = X Initial Coord
//! 	\param ypos = Y Initial Coord
//!	\param gtype = Goodie Type (IE GOODIE_TOYS)
//!	\param gid = Goodie ID (IE Less than MAX_TOYS)
//!	\param xinert = X Initial Inertia
//!	\param yinert = Y Initial Inertia
//------------------------------------------------------------------------------
void CGame::MakeGoodie(int xpos, int ypos, int gtype, int gid, int xinert, int yinert)
{
	CGoodieObj *pgood;
	GOODIE_TYPE *group;

	SMB_NEW(pgood,CGoodieObj);
	if (pgood)
	{
		m_GoodieList.Attach(pgood,OBJ_GOODIE, this);
		pgood->m_OldXPos = pgood->m_XPos = xpos;
		pgood->m_OldYPos = pgood->m_YPos = ypos;
		pgood->m_XInert = xinert;
		pgood->m_YInert = yinert;

		if (pgood->m_XInert>=0) pgood->m_Dir=DIR_RIGHT;	// Object moving right

		group = goodie_group[gtype];
		group = group + gid;				// Point to the correct goodie
		pgood->m_GID = gid;
		pgood->SetStruct(group);

	}

}

//------------------------------------------------------------------------------
//! \brief Create a small 'Jump'
//!
//! 	\param xpos = X start position
//!	\param ypos = Y start position
//!	\param xinert = X initital inertia 
//!	\param yinert = Y initial inertia
//------------------------------------------------------------------------------
void CGame::CreateSmallJump(int xpos, int ypos, int xinert, int yinert)
{
	CJumpObj *pjump;

	SMB_NEW(pjump,CJumpObj);
	if (pjump)
	{
		pjump->m_OldXPos = pjump->m_XPos = xpos;
		pjump->m_OldYPos = pjump->m_YPos = ypos;
		pjump->m_XInert = xinert;
		pjump->m_YInert = yinert;
		pjump->InitSize(jump_smallsize);
		pjump->m_JumpSize = 1;
		pjump->m_Frame = SPR_JUMP_SJUMP1;
		pjump->m_pSuckFrames = &suck_jump_smallframes;
		m_BaddieList.Attach(pjump,OBJ_SMALLJUMP, this);
	}
}

//------------------------------------------------------------------------------
//! \brief Set a large 'Jump' to explode another smaller 'Jumps'
//!
//! 	\param xpos = X start position
//!	\param ypos = Y start position
//!	\param dir = Direction to fly off to (0=RIGHT, Else LEFT) (NOT DIR_LEFT!)
//------------------------------------------------------------------------------
void CGame::SetJumpExplode(int xpos, int ypos, int dir)
{
	int xinert1;
	int xinert2;

	xinert1 = 12*256 - (rand()&0x3ff);
	xinert2 = 6*256 + (rand()&0x3ff);

	if (dir)	// Fly off to the left?
	{
		xinert1 = -xinert1;
		xinert2 = -xinert2;
	}

	CreateSmallJump(xpos, ypos,xinert1, -256*4 );
	CreateSmallJump(xpos, ypos,xinert2, -256*8 );

}

//------------------------------------------------------------------------------
//! \brief Gets the address of a player
//!
//!	\param player_object_id = The player OBJ_PLAYER_xxx id
//!
//!	\return The Player. 0 = Not Found
//------------------------------------------------------------------------------
CPlayerObj *CGame::GetPlayer( int player_object_id)
{
	CPlayerObj *pobj;

	pobj = (CPlayerObj *) m_PlayerList.m_pFirst;
	while (pobj)
	{
		if (pobj->m_Type == player_object_id)
		{
			return pobj;
		}
		pobj = (CPlayerObj *) pobj->m_pNext;
	}
	return 0;
}

//------------------------------------------------------------------------------
//! \brief Initialise the sprites for the map
//!
//! NOTE: The Player Objects must already exist.\n
//! The sprite graphics are also loaded
//------------------------------------------------------------------------------
void CGame::InitSpriteList(void)
{
	int tune;
	int xpos,ypos,type;
	int cnt;
	short *frkptr;

	CPlayerObj *pobj;

	m_EnterBonusLevelFlag = 0;
	m_BonusLevelFlag = 0;
	m_BonusCompleteFlag = 0;
	m_CountDown = -1;
	m_TreasSpotCnt = 0;
	m_BossLevel = 0;
	m_BonusDelay = 0;

	m_GoodieCollectCnt = 0;
	m_GoodieCollectFlag = 0;

	m_FadeType = FADE_LEVELFADE;
	m_FadeFlag = FADE_FLAG_WAIT;

	if (m_LevelNumber<=MAX_LEVELS)	// Don't change music on bonus screens
	{
		if (m_LevelNumber<50) tune = SMOD_TUNE1; else tune = SMOD_TUNE2;
		PlayModule(tune);
	}


	m_Sprites.InitPurge();		// Initialise the graphic purge

	m_GasList.DeleteAll();		// Delete the object lists
	m_BaddieList.DeleteAll();
	m_GoodieList.DeleteAll();
	m_ExtraList.DeleteAll();
	m_FontList.DeleteAll();

	pobj = (CPlayerObj *) m_PlayerList.m_pFirst;
	while (pobj)
	{
		pobj->NewLevel();
		if (m_EggFlag)	// Egg the player?
		{
			pobj->m_EggFlag = 1;
			m_Sprites.LoadRange(SPR_EGG_1, SPR_EGG_9);
		}
		pobj = (CPlayerObj *) pobj->m_pNext;
	}
	m_EggFlag = 0;

	// Standard Sprites
	m_Sprites.LoadRange(SPR_P1UP, SPR_P2UP);
	m_Sprites.LoadRange(SPR_FNT_R0, SPR_FNT_Y9);
	m_Sprites.LoadRange(SPR_CARD_SSPADE, SPR_CARD_SDMOND);

	LoadGoodieGfx();

	cnt = m_Map.m_FrkData[0];		// Get the number of entries
	frkptr = &m_Map.m_FrkData[3];	// To the sprite data

	m_FlowerFlag = 0;
	InitPowerUp();
	for (;cnt>0; cnt--)		// For all the objects
	{
		type = *(frkptr++);
		xpos = *(frkptr++);
		ypos = *(frkptr++);
		StartFRKObject(type, xpos, ypos);
	}
	UsePowerUp();
	m_Sprites.DoPurge();		// Do the graphic purge

	SetLevelName(m_LevelNumber);

	m_MainCounter=0;

	m_CompleteFlag = 0;
  	m_PanelOrigin = 16;
	m_DayDelay = 0;
	m_DisableCard = 0;

}

//------------------------------------------------------------------------------
//! \brief Draw a player information (ie score, lives)
//!
//! 	\param pobj = player to show. (0 = No Player)
//!	\param play = player status offset structure
//------------------------------------------------------------------------------
void CGame::DrawPlayerInfo( CPlayerObj *pobj, PLAYER_STATUS *play )
{
	char txtbuffer[16];
	char *txtptr;
	int cnt;
	int xpos;
	int delta;
	int score;
	int lives;
	int frame;
	int cards;

	if (pobj) score=pobj->m_Score; else score = 0;
	if (pobj) lives=pobj->m_Lives; else lives = 0;
	if (lives>99) lives = 99;	// Limit lives to 99

	txtbuffer[0]=0;
	sprintf(txtbuffer, "%u", score);

	frame=play->scorefrm;
	xpos=play->scorexoff;

	txtptr = txtbuffer;
	while (*txtptr)
	{
		m_Sprites.Draw( frame + (*(txtptr++)) - '0', xpos, PLAY_YOFF - m_PanelOrigin, GFX_NOWRAP );
		xpos+=8;
	}

	m_Sprites.Draw( play->pxup, play->pxoff, PLAY_YOFF - m_PanelOrigin, GFX_NOWRAP );
	xpos=play->livesxoff;
	m_Sprites.Draw( frame + 10 , xpos, PLAY_YOFF - m_PanelOrigin, GFX_NOWRAP );
	m_Sprites.Draw( frame + (lives/10) , xpos+8, PLAY_YOFF - m_PanelOrigin, GFX_NOWRAP );
	m_Sprites.Draw( frame + (lives%10) , xpos+16, PLAY_YOFF - m_PanelOrigin, GFX_NOWRAP );

	xpos = play->cardxoff;
	delta = play->cardxdelta;
	if (pobj)
	{
		cards = pobj->m_Cards;
		for (cnt=SPR_CARD_SSPADE; cnt<(SPR_CARD_SSPADE+4); cnt++)	// 4 Cards
		{
			
			if (cards&1)
			{
				m_Sprites.Draw( cnt, xpos, CARD_YOFF + m_PanelOrigin, GFX_NOWRAP );
				xpos +=delta;
			}
			cards = cards>>1;
		}
	}

}

//------------------------------------------------------------------------------
//! \brief Draw the players information (ie score, lives)
//------------------------------------------------------------------------------
void CGame::DrawPlayersInfo(void)
{
	CPlayerObj *play;
	int lower;
	int upper;

	play = GetPlayer(OBJ_PLAYER_ONE);
	DrawPlayerInfo( play,&pstat1 );

	play = GetPlayer(OBJ_PLAYER_TWO);
	DrawPlayerInfo( play,&pstat2 );

	// Draw level number
	if ((!m_BonusLevelFlag) && (m_LevelNumber < 100))
	{
		upper = m_LevelNumber / 10;
		lower = m_LevelNumber % 10;
		if (upper == 0)	// Single digit level number
		{
			m_Sprites.Draw( SPR_FNT_Y1 + lower - 1 , CREDIT_XOFF, PLAY_YOFF - m_PanelOrigin, GFX_NOWRAP );
		}else
		{
			m_Sprites.Draw( SPR_FNT_Y1 + upper - 1 , CREDIT_XOFF - 4, PLAY_YOFF - m_PanelOrigin, GFX_NOWRAP );
			m_Sprites.Draw( SPR_FNT_Y1 + lower - 1 , CREDIT_XOFF + 4, PLAY_YOFF - m_PanelOrigin, GFX_NOWRAP );
		}
	}
}

//------------------------------------------------------------------------------
//! \brief Display a rainbox font message onto the screen
//!
//! 	\param yoffset = Y offset of the font
//!	\param txt = Text to display
//!	\param delay = Message delay (in frames)
//!
//! 	\return the next charactor delay
//------------------------------------------------------------------------------
int CGame::CreateMessage(int yoffset, const char *txt, int delay)
{
	char let;
	int txtlen;
	int xoffset;
	int xwidth;
	int offset;
	CRFontObj *robj;

	txtlen = (int) ::strlen(txt);

	xwidth = txtlen << 3;		// (*16, /2)
	xoffset = (SCR_WIDTH/2) - xwidth;
	while(*txt)
	{
		let = *(txt++);

		offset = let - ' ';
		if ( (offset>0) && (offset<NUM_FONT_TABLE) )
		{
			SMB_NEW(robj,CRFontObj);
			if (robj)
			{
				m_FontList.Attach(robj,OBJ_RFONT, this);
				robj->SetRiser( font_table[offset], xoffset, yoffset, delay );
			}
		}
		xoffset+=16;
		delay += 2;
	}
	return delay;
}

//------------------------------------------------------------------------------
//! \brief Display a level message (Called when a new level starts)
//!
//! 	\param id = Level ID (1-100)
//------------------------------------------------------------------------------
void CGame::SetLevelName( int id )
{
	int delay;
	char txtbuffer[16];

	if ( (id<=0) || (id>NUM_STD_LEVELS) )
	{
		CreateMessage(0, "ERROR", 0);
	}else
	{
		sprintf( txtbuffer, "%d", id );
		if (id<=MAX_LEVELS)
		{
			delay = CreateMessage(0, txtbuffer, 0);
			CreateMessage(32, level_text_names[id-1], delay);
		}
	}
}

//------------------------------------------------------------------------------
//! \brief Check to see if the level has been completed
//------------------------------------------------------------------------------
void CGame::CheckComplete(void)
{
	int nextlev;
	CPlayerObj *pobj;

	if (m_GameOverFlag)
	{
		DoGameOverLoop();
		return;
	}

	if (m_EnterBonusLevelFlag)
	{
		EnterBonusLevel();
		return;
	}
	if (m_BonusLevelFlag)
	{
		if (m_BonusCompleteFlag)
		{
			NextLevel();
		}
		return;
	}

	if (!m_CompleteFlag)
	{
		if (m_LevelSkip>0)	// Moon Collected?
		{
			m_CompleteFlag = 1;
			m_CompleteCnt = 0;
			m_LevelSkip--;
		}
		if (IsComplete())	// Is Completed
		{
			m_CompleteFlag = 1;
			m_CompleteCnt = DELAY_BEFORE_NEXT_LEVEL;
			m_pSound->PlaySample(SND_FINLEV1);
			CreateMessage( -32, "COMPLETED", 0 );
			if ((!m_BossLevel)&&(!m_DisableCard))	// Display the card?
			{
				CCardObj *pobj;
				SMB_NEW(pobj,CCardObj);
				if (pobj)
				{
					m_GoodieList.Attach(pobj, OBJ_CARD, this);
				}
			}
		}else
		{
		  	if (m_PanelOrigin>0) m_PanelOrigin--;
			
			return;
		}
	}
	if (m_CompleteCnt)		// Delay before the end of the level
	{
		// Calculate the panel Y offset
		m_PanelOrigin = m_CompleteCnt;
		m_PanelOrigin = 32 - m_PanelOrigin;
		if (m_PanelOrigin<0) m_PanelOrigin = 0;
		if (m_PanelOrigin>32) m_PanelOrigin = 32;

		m_CompleteCnt--;
		return;
	}

	nextlev = 0;

	pobj = (CPlayerObj *) m_PlayerList.m_pFirst;
	if (!pobj) nextlev = 1;	// No players exist
	while (pobj)
	{
		if (pobj->m_Command!=PCMD_RISEUP)
		{
			if ((pobj->m_PlayerFlags) & PF_IS_ALIVE)
			{
				pobj->SetRiseUp();
			}
		}else
		{
			if (pobj->m_AtTopFlag)
			{
				if (nextlev == 0) nextlev = 1;
			}else
			{
				nextlev = 2;	// Not at top (note, the other player may be at the top)
			}
		}

		pobj = (CPlayerObj *) pobj->m_pNext;

	}

	if (nextlev == 1)
	{
		CheckForEgg();
		NextLevel();
	}
}

//------------------------------------------------------------------------------
//! \brief Go to the next level
//------------------------------------------------------------------------------
void CGame::NextLevel(void)
{
	int num;
	CPlayerObj *pobj;

	// Check for key rooms
	if (m_BossLevel)
	{
		SetBonusLevel(BLEV_KEYROOM);
		EnterBonusLevel();
		return;
	}

	// Check for the bonus rooms
	if (!m_BonusLevelFlag)
	{

		pobj = (CPlayerObj *) m_PlayerList.m_pFirst;
		while (pobj)
		{
			if (pobj->m_PlayerFlags & PF_IS_ALIVE)
			{
				pobj->m_NoDieCnt++;

				// Coin Room Test
				num = pobj->m_Score&255;
				if ( (num==222) )
				{
					SetBonusLevel(BLEV_COINROOM);
					EnterBonusLevel();
					return;
				}

				// Jewel Room Test
				if (pobj->m_NoDieCnt >= JEWEL_DIE_COUNT)
				{
					pobj->m_NoDieCnt = 0;
					SetBonusLevel(BLEV_JEWELROOM);
					EnterBonusLevel();
					return;
				}
			}	
			pobj = (CPlayerObj *) pobj->m_pNext;
		}

		// Check for power rooms
		if (!m_GoodieCollectFlag)
		{
			if (m_GoodieCollectCnt>10)
			{
				SetBonusLevel(BLEV_POWERROOM);
				EnterBonusLevel();
				return;
			}
		}
	}
	m_LevelNumber++;
	if (m_LevelNumber>100)		// Completed Game?
	{
		m_Map.LoadBlockSet(SPR_ENDBLOX_DATA);
		SetBonusLevel(BLEV_COMPLETED);
		EnterBonusLevel();
		m_Sprites.LoadRange(SPR_ENDSPR_1, SPR_ENDSPR_30);
		m_Sprites.LoadRange(SPR_BALLOON_1, SPR_BALLOON_5);
		m_Sprites.LoadRange(SPR_RFONT_0, SPR_RFONT_CURSOR);
		m_BonusDelay = 0;
		m_EndYOffset = 0;
		m_MainCommand = MC_COMPLETED;
		PlayModule(SMOD_COMPLETE);
		return;
	}
	m_Map.GetMap(m_LevelNumber);
	InitSpriteList();

}

//------------------------------------------------------------------------------
//! \brief Has the level has been completed (all baddies removed)
//!
//! 	\return 0 = Not Completed. Else = Has been Completed
//------------------------------------------------------------------------------
int CGame::IsComplete(void)
{
	CGasObj *gasobj;
	CLinkObject *linkobj;
	CPlayerObj *pobj;

	// First, check to see if player is holding a baddie

	pobj = (CPlayerObj *) m_PlayerList.m_pFirst;
	while (pobj)
	{
		if (pobj->m_pBaddie)
		{
			if (pobj->m_pBaddie->m_Flags & FLAG_BADDIE) return 0;	// Gun containing baddie
		}
		pobj = (CPlayerObj *) pobj->m_pNext;
	}

	// Find any baddies
	linkobj = m_BaddieList.m_pFirst;
	while (linkobj)
	{
		if (linkobj->m_Flags & FLAG_BADDIE) return 0;	// Found a baddie
		linkobj = linkobj->m_pNext;
	}

	// Find any baddies inside a gas cloud

	gasobj = (CGasObj *) m_GasList.m_pFirst;
	while (gasobj)
	{
		if (gasobj->m_Type == OBJ_GAS)
		{
			if (gasobj->m_pBaddie) return 0;	// Found a baddie
		}
		gasobj = (CGasObj *) gasobj->m_pNext;
	}


	// Is completed
	return 1;
}

//------------------------------------------------------------------------------
//! \brief Create the stuff when a boss is defeated
//!
//! 	\param xpos = X Position of "doo dah day"
//! 	\param ypos = Y Position of "doo dah day"
//------------------------------------------------------------------------------
void CGame::SetBossDie( int xpos, int ypos )
{
	int cnt;
	CRealDayObj *pobj;
	CPlayerObj *playobj;

	playobj = (CPlayerObj *) m_PlayerList.m_pFirst;
	while (playobj)
	{
		playobj->AddScore( 10000 );
		playobj = (CPlayerObj *) playobj->m_pNext;
	}
	MakeNumRise( xpos, ypos + 96, SPR_NUM_10000 );

	SMB_NEW(pobj,CRealDayObj);
	if (pobj)
	{
		pobj->Setup(xpos,ypos);
		m_GoodieList.Attach(pobj, OBJ_REALDAY, this);
	}
	for (cnt=0; cnt<16; cnt++)
	{
		RandGoodie( xpos, ypos + (6*cnt), GOODIE_COMP, cnt&1 );
	}

}

//------------------------------------------------------------------------------
//! \brief Create a gas cloud to appear then disappear
//!
//! 	\param xpos = X where to display it
//! 	\param ypos = Y Where to display it
//------------------------------------------------------------------------------
void CGame::CreateCloud( int xpos, int ypos )
{
	CCloudObj *pobj;

	SMB_NEW(pobj,CCloudObj);
	if (pobj)
	{
		m_GasList.Attach(pobj, OBJ_CLOUD, this);
		pobj->Setup(xpos,ypos);
	}

}

//------------------------------------------------------------------------------
//! \brief Check to see if doo dah day should appear if the player takes too long
//------------------------------------------------------------------------------
void CGame::CheckDooDahDay(void)
{
	if (m_BonusLevelFlag || m_BossLevel) return;

	if (m_CompleteFlag)		// No doo dah day when the level has been completed
	{
		m_DayDelay = 0;
		return;
	}
	m_DayDelay++;
	if (m_DayDelay>DELAY_BEFORE_DOODAHDAY)
	{
		// Create Doo Dah Day
		CDayObj *pobj;
		SMB_NEW(pobj,CDayObj);
		if (pobj)
		{
			// Put into the goodie list, so it is in front of normal baddies
			m_GoodieList.Attach(pobj, OBJ_DAY, this);
			pobj->Setup((SCR_WIDTH/2)-6,(SCR_HEIGHT/2)-4);
		}
		m_DayDelay = 0;
		return;
	}

	if (m_DayDelay>(DELAY_BEFORE_DOODAHDAY-30))
	{
		// Create Doo Dah Day clouds
		int xp,yp;
		xp = rand();
		yp = rand();
		xp = xp & 63;
		yp = yp & 63;
		xp = xp + (SCR_WIDTH / 2) - 32;
		yp = yp + (SCR_HEIGHT / 2) - 32;
		CreateCloud( xp, yp );
		return;
	}
	if (m_DayDelay==(DELAY_BEFORE_DOODAHDAY-30))
	{
		CreateMessage( 0, "LOOK OUT!", 0 );
		m_pSound->PlaySample(SND_DAY);
		return;
	}

	if (m_DayDelay==(DELAY_BEFORE_DOODAHDAY/2))
	{
		m_DisableCard = 1;
		CreateMessage( 0, "HURRY UP!", 0 );
		SetAngryBaddies(1);
		m_pSound->PlaySample(SND_HURRY);
		return;
	}
}

//------------------------------------------------------------------------------
//! \brief Check for extra things
//------------------------------------------------------------------------------
void CGame::CheckExtras(void)
{
	if (m_FlowerFlag)
	{
		if (!(m_MainCounter&15))
		{
			MakeGoodie(rand()%SCR_WIDTH, rand()%SCR_HEIGHT,
				GOODIE_FLOWER, rand()&3, 0, -4*256);
		}
	}

	// Control the power room (should be done using an object...)
	if (m_BonusLevelFlag)
	{
		if (m_BonusLevelID==BLEV_POWERROOM)
		{
			m_BonusDelay++;
			if (m_BonusDelay>=POWERROOM_LENGTH)
			{
				m_BonusCompleteFlag = 1;
			}else
			{
				m_CountDown = (POWERROOM_LENGTH-m_BonusDelay)>>3;
			}
		}
	}
}

//------------------------------------------------------------------------------
//! \brief Draw a rainbow font directly to the screen
//!
//! 	\param ypos = Y offset
//!	\param text = Text to draw
//------------------------------------------------------------------------------
void CGame::DrawFont(int ypos, const char *text)
{
	int	len;
	int	xoffset;
	int	cnt;
	int	offset;

	len = (int) ::strlen(text);
	xoffset = (SCR_WIDTH/2) - (len*8);
	for (cnt=0; cnt<len; cnt++)
	{
		offset = text[cnt] - ' ';
		if ( (offset>0) && (offset<NUM_FONT_TABLE) )
		{
			m_Sprites.Draw( font_table[offset], xoffset, ypos, GFX_NOWRAP );
		}
		xoffset+=16;

	}
}

//------------------------------------------------------------------------------
//! \brief Draw a screen title font directly to the screen
//!
//! 	\param ypos = Y offset
//!	\param text = Text to draw
//!	\param xpos = X offset. If 0 = Centre text
//------------------------------------------------------------------------------
void CGame::DrawScrFont(int ypos, const char *text, int xpos)
{
	int	len;
	int	xoffset;
	int	cnt;
	int	sprid;
	char	let;


	len = (int) ::strlen(text);
	if (xpos)
	{
		xoffset = xpos;
	}else
	{
		xoffset = (SCR_WIDTH/2) - (len*4);
	}
	for (cnt=0; cnt<len; cnt++)
	{
		let = *(text)++;

		sprid = 0;

		if ((let >= 'A' ) && (let <= 'Z')) sprid = SPR_SCRFONT_A + (let - 'A');
		if ((let >= '0' ) && (let <= '9')) sprid = SPR_SCRFONT_0 + (let - '0');
		if (let == '.') sprid = SPR_SCRFONT_DOT;
		if (let == '/') sprid = SPR_SCRFONT_SLASH;
		if (let == ':') sprid = SPR_SCRFONT_COLON;

		if ( sprid )
		{
			m_Sprites.Draw( sprid, xoffset, ypos, GFX_NOWRAP );
		}
		xoffset+=8;

	}
}

//------------------------------------------------------------------------------
//! \brief Check for extra things version2 (called in a different place)
//------------------------------------------------------------------------------
void CGame::CheckExtras2(void)
{
	if (m_CountDown>=0)		// Number countdown wanted?
	{
		char text[16];
		sprintf(text,"%d", m_CountDown);
		DrawFont( 48, text );
	}
}

//------------------------------------------------------------------------------
//! \brief Set all the baddies to be angry / normal
//!
//! 	\param flag = 0 = NOT angry. Else Angry
//------------------------------------------------------------------------------
void CGame::SetAngryBaddies(int flag)
{
	CLinkObject *linkobj;

	// Find all baddies
	linkobj = m_BaddieList.m_pFirst;
	while (linkobj)
	{
		if (flag)
		{
			linkobj->m_State |= STATE_ANGRY;
		}else
		{
			linkobj->m_State &= (~STATE_ANGRY);
		}

		linkobj = linkobj->m_pNext;
	}

}

//------------------------------------------------------------------------------
//! \brief Set the player to go to a bonus level
//!
//! 	\param lvl_id = BLEV_xxx level id
//------------------------------------------------------------------------------
void CGame::SetBonusLevel( int lvl_id )
{
	m_BonusLevelID = lvl_id;
	m_EnterBonusLevelFlag = 1;
}

//------------------------------------------------------------------------------
//! \brief Enter a bonus level
//------------------------------------------------------------------------------
void CGame::EnterBonusLevel(void)
{
	int oldlevel;
	int old_boss_flag;

	old_boss_flag = m_BossLevel;
	oldlevel = m_LevelNumber;
	m_Map.GetMap(m_BonusLevelID);
	m_LevelNumber = m_BonusLevelID;
	InitSpriteList();
	m_LevelNumber = oldlevel;
	m_BonusLevelFlag = 1;
	if (m_BonusLevelID==BLEV_MUSICROOM)
	{
		// As per amiga source - Hard code the man
		CNoteManObj *pobj;
		SMB_NEW(pobj,CNoteManObj);
		if (pobj)
		{
			InitFrkObject(pobj, OBJ_NOTEMAN, 176, 60, &m_BaddieList);
			pobj->LoadGfx();
		}
	}
	if (m_BonusLevelID==BLEV_CHEESEROOM)
	{
		// As per amiga source - Hard code the cheese
		CBigCheeseObj *cobj;
		SMB_NEW(cobj,CBigCheeseObj);
		if (cobj)
		{
			InitFrkObject(cobj, OBJ_BIGCHEESE, 0 , 50, &m_GoodieList);
			cobj->LoadGfx();
			cobj->m_MainCheeseFlag = 1;
		}
		SMB_NEW(cobj,CBigCheeseObj);
		if (cobj)
		{
			InitFrkObject(cobj, OBJ_BIGCHEESE, 320-48 , 50, &m_GoodieList);
			cobj->LoadGfx();
		}
	}
	if (m_BonusLevelID==BLEV_TREASROOM)
	{
		// As per amiga source - Hard code the treasure
		SetTreasure(136,50, GOODIE_TRES);
	}

	if (m_BonusLevelID==BLEV_JEWELROOM)
	{
		// As per amiga source - Hard code the treasure
		SetTreasure(136,50, GOODIE_JEWEL);
	}

	if (m_BonusLevelID==BLEV_COINROOM)
	{
		// As per amiga source - Hard code the treasure
		SetTreasure(180,192, GOODIE_COIN);
	}

	if (m_BonusLevelID==BLEV_CARDROOM)
	{
		CCardRoomObj *apobj;
		SMB_NEW(apobj,CCardRoomObj);
		if (apobj)
		{
			InitFrkObject(apobj, OBJ_CARDROOM, 0, 0, &m_ExtraList);
			apobj->LoadGfx();
		}
	}

	if (m_BonusLevelID==BLEV_KEYROOM)
	{
		CKeyRoomObj *bpobj;
		SMB_NEW(bpobj,CKeyRoomObj);
		if (bpobj)
		{
			InitFrkObject(bpobj, OBJ_KEYROOM, 0, 0, &m_ExtraList);
			bpobj->Setup(old_boss_flag);
			bpobj->LoadGfx();
		}
	}

}

//------------------------------------------------------------------------------
//! \brief Make a treasure box (chest) - Used by bonus rooms
//!
//! 	\param xpos = xpos
//! 	\param ypos = ypos
//!	\param rtype = GOODIE_xxx to release from the treasure box
//------------------------------------------------------------------------------
void CGame::SetTreasure(int xpos, int ypos, int rtype)
{
	CChestObj *cobj;
	SMB_NEW(cobj,CChestObj);
	if (cobj)
	{
		InitFrkObject(cobj, OBJ_CHEST, xpos , ypos, &m_ExtraList);
		cobj->m_BonusRoomFlag = 1;
		cobj->m_ReleaseType = rtype;
		cobj->LoadGfx();
	}
}

//------------------------------------------------------------------------------
//! \brief Count the number of baddies on the level
//!
//! 	\return The number of baddies
//------------------------------------------------------------------------------
int CGame::CountBaddies(void)
{
	CGasObj *gasobj;
	CLinkObject *linkobj;
	
	int counter;

	counter = 0;

	// Find any baddies
	linkobj = m_BaddieList.m_pFirst;
	while (linkobj)
	{
		if (linkobj->m_Flags & FLAG_BADDIE) counter++;
		linkobj = linkobj->m_pNext;
	}

	// Find any baddies inside a gas cloud

	gasobj = (CGasObj *) m_GasList.m_pFirst;
	while (gasobj)
	{
		if (gasobj->m_Type == OBJ_GAS)
		{
			if (gasobj->m_pBaddie) counter++;
		}
		gasobj = (CGasObj *) gasobj->m_pNext;
	}

	return counter;
}

//------------------------------------------------------------------------------
//! \brief Check to see if the players should be egged (ie player missed the card)
//------------------------------------------------------------------------------
void CGame::CheckForEgg(void)
{
	int cards;
	int thecard;
	CCardObj *cptr;
	CPlayerObj *pobj;

	m_EggFlag = 0;
	pobj = (CPlayerObj *) m_PlayerList.m_pFirst;
	while (pobj)
	{
		cards = pobj->m_Cards;
		cptr = (CCardObj *) m_GoodieList.FindFirst(OBJ_CARD);
		if (cptr)	// Card not picked up?
		{
			thecard = cptr->GetCardMask();	// Get the card type
			cards = cards & thecard;	// Mask the card
			if (!cards)	// Card was needed
			{
				m_EggFlag = 1;
			}
		}
		pobj = (CPlayerObj *) pobj->m_pNext;
	}
}

//------------------------------------------------------------------------------
//! \brief Play a tune. If it is already playing - continue
//!
//! 	\param id = SMOD_xxx id
//------------------------------------------------------------------------------
void CGame::PlayModule(int id)
{
	if (id!=m_CurrentTune)
	{
		m_CurrentTune = id;
		m_pSound->PlayModule(id);
	}

}

//------------------------------------------------------------------------------
//! \brief Control the colour fading
//!
//! 	\return 0 = Do not run the game main loop
//------------------------------------------------------------------------------
int CGame::ControlFade(void)
{
	if (m_FadeType==FADE_COMPLETE) return 1;
	switch (m_FadeType)
	{
		case FADE_NORMAL:
			if (Fade( 0.0f, 32 ))
			{
				m_FadeType = FADE_COMPLETE;
			}
			break;
		case FADE_BLACK:
			if (Fade( -1.0f,32 ))
			{
				m_FadeType = FADE_COMPLETE;
			}
			break;
		case FADE_WHITE:
			if (Fade( 1.0f, 64 ))
			{
				m_FadeType = FADE_COMPLETE;
			}
			break;
		case FADE_LEVELFADE:
			if (Fade( -1.0f, 32 ))
			{
				m_FadeType = FADE_NORMAL;
				m_FadeFlag = FADE_FLAG_CONTINUE;
			}
			break;
		case FADE_TITLESCREEN:
			if (Fade( 0.0f, 32 ))
			{
				m_FadeType = FADE_COMPLETE;
			}
			break;
		default:
			m_FadeType = FADE_COMPLETE;
	}
	if (m_FadeType==FADE_COMPLETE) return 1;
	switch (m_FadeFlag)
	{
		case FADE_FLAG_ONCE:
			m_FadeFlag = FADE_FLAG_WAIT;
			return 1;
		case FADE_FLAG_CONTINUE:
			return 1;
	}
	return 0;
}

//------------------------------------------------------------------------------
//! \brief The Completed game screen - main loop
//------------------------------------------------------------------------------
void CGame::CompletedLoop(void)
{
	// -------

	m_MainCounter++;

	if (m_GameOverFlag)
	{
		DoGameOverLoop();
		return;
	}


	// Test for return to high score
	if (m_MainCounter>100)
	{
		if ( (m_pJoy1->fire) || (m_pJoy2->fire) )
		{
			m_GameOverFlag = GAMEOVER_DELAY-1;	// Immediate fade
			m_pJoy1->fire = 0;
			m_pJoy2->fire = 0;
			return;
		}

	}

	m_ExtraList.DoAll();

	// Do not redraw map etc. when fading has started
	if (m_FadeType!=FADE_COMPLETE)
	{
		if (m_FadeFlag==FADE_FLAG_WAIT) return;
	}

	m_Map.Draw();

	CreateBalloons();

	m_ExtraList.DrawAll();
	DrawEndCredits();
}


//------------------------------------------------------------------------------
//! \brief Create the balloons for the completed screen
//------------------------------------------------------------------------------
void CGame::CreateBalloons(void)
{
	m_BonusDelay--;
	if (m_BonusDelay>0) return;
	m_BonusDelay = (rand() & 15);

	CBalloonObj *cobj;
	SMB_NEW(cobj,CBalloonObj);
	if (cobj)
	{
		int xpos = (rand() % (SCR_WIDTH-32));
		InitFrkObject(cobj, OBJ_BALLOON, xpos , SCR_HEIGHT, &m_ExtraList);
		cobj->m_Frame = SPR_BALLOON_1 + (rand() % 5);
		cobj->m_Rate = (rand() &3) + 3;
	}
}

//------------------------------------------------------------------------------
//! \brief Draw the graphic items in the end credits
//!
//! 	\param xpos = X Original Offset
//! 	\param ypos = Y Original Offset
//!	\param party = party group
//------------------------------------------------------------------------------
void CGame::DrawEndGfxItems(int xpos, int ypos, PARTYOFFS **party)
{
	PARTYOFFS *pptr;
	for (; *party; party++)
	{
		pptr = *party;
		m_Sprites.Draw( pptr->frame, pptr->xoffset + xpos,
			pptr->yoffset + ypos, GFX_NOWRAP );
	}

}

//------------------------------------------------------------------------------
//! \brief Draw the end credits
//------------------------------------------------------------------------------
void CGame::DrawEndCredits(void)
{
	int xpos;
	int ypos;
	ENDGROUP **items;
	ENDGROUP *group;
	const char *txt;
	char let;

	ypos = (m_EndYOffset-=2) + SCR_HEIGHT;

	if (m_EndYOffset<-0xa00) m_EndYOffset = 0;

	for (items = end_items; *items; items++)
	{
		group = *items;
		DrawEndGfxItems( group->x_gfxoffset, ypos, group->party );
		ypos += group->y_textoffset + 32;
		txt = group->text;
		while (*txt)
		{
			xpos = (int) (unsigned char) *(txt++);
			while (*txt)	// If null, the other while loop will also exit (I hope!)
			{
				let = *(txt++);
				if (let==1)	// Newline?
				{
					ypos+=32;
					break;
				}
				if (let!=' ')
				{
					let = let - ' ';
					if ( (let>0) && (let<NUM_FONT_TABLE) )
					{
						m_Sprites.Draw( font_table[let], xpos, ypos, GFX_NOWRAP );
					}
				}
				xpos+=16;
			}
		}
		ypos+=32;
	}
 
}

//------------------------------------------------------------------------------
//! \brief Initialise the title screen
//------------------------------------------------------------------------------
void CGame::InitTitleScreen(void)
{
	m_Map.LoadBlockSet(SPR_TITLE_DATA);
	m_MainCommand = MC_TITLE;
	m_Map.GetMap(BLEV_TITLE);
	m_FadeType = FADE_TITLESCREEN;
	m_FadeFlag = FADE_FLAG_CONTINUE;

	PlayModule(SMOD_TITLE);
	m_MainCounter = 0;
	m_pJoy1->fire = 0;
	m_pJoy2->fire = 0;

	m_Sprites.LoadRange(SPR_SCRFONT_A, SPR_SCRFONT_COLON);


}

//------------------------------------------------------------------------------
//! \brief The title screen main loop
//------------------------------------------------------------------------------
void CGame::TitleScreenLoop(void)
{
	m_MainCounter++;
	if ( (m_pJoy1->fire) || (m_pJoy2->fire) )	// Game start?
	{
		InitGetPlayerNameScreen(m_pJoy2->fire);
		return;
	}

	m_Map.Draw();
	DrawScrFont( 116, GameVersionNumber, 200);
	DrawScrFont( 200, "GAME LICENSE:");
	DrawScrFont( 200+12, "GNU GENERAL PUBLIC LICENSE VERSION 2");

	DrawScrFont( 232, "HTTP://METHANE.SOURCEFORGE.NET" );
	m_pGameTarget->RedrawScreen();

	if (m_MainCounter == HISCREEN_SHOW_DELAY)	// Finished showing hiscores
	{
		m_FadeType = FADE_BLACK;
		m_FadeFlag = FADE_FLAG_WAIT;
	}
	if (m_MainCounter > HISCREEN_SHOW_DELAY)	// Finished showing hiscores
	{
		InitHighScreen();
	}

}

//------------------------------------------------------------------------------
//! \brief Initialise the game over (Called from the player object)
//------------------------------------------------------------------------------
void CGame::InitGameOver(void)
{
	m_GameOverFlag = 1;
	m_FontList.DeleteAll();	// Delete all the fonts
	CreateMessage(0, "GAME OVER", 0);

}

//------------------------------------------------------------------------------
//! \brief Do the game over loop
//------------------------------------------------------------------------------
void CGame::DoGameOverLoop(void)
{
	m_GameOverFlag++;

	if (m_GameOverFlag == GAMEOVER_DELAY)	// Fade to black?
	{
		m_FadeType = FADE_BLACK;
		m_FadeFlag = FADE_FLAG_WAIT;
	}

	if (m_GameOverFlag > GAMEOVER_DELAY)
	{
		CPlayerObj *pobj;
		pobj = (CPlayerObj *) m_DeadPlayerList.m_pFirst;
		while (pobj)
		{
			InsertHiScore(pobj->m_Score,pobj->m_Name);
			pobj = (CPlayerObj *) pobj->m_pNext;
		}
		InitHighScreen();
	}
}

//------------------------------------------------------------------------------
//! \brief A new game is about to start
//------------------------------------------------------------------------------
void CGame::InitNewGame(void)
{
	CPlayerObj *pobj;

	m_LevelNumber = 1;
	m_GameOverFlag = 0;
	m_TrainCnt = m_DuckCnt = m_CarCnt = 0;
	m_LevelSkip = 0;
	m_EggFlag = 0;
	m_PUP_Cnt = 0;

	m_Map.LoadBlockSet(SPR_BLOX_DATA);
	m_MainCommand = MC_GAME;
	m_Map.GetMap(m_LevelNumber);

	// Reset the player
	m_PlayerList.DeleteAll();
	m_DeadPlayerList.DeleteAll();
	SMB_NEW(pobj,CPlayerObj);
	if (pobj)
	{
		m_PlayerList.Attach(pobj,OBJ_PLAYER_ONE, this);
		pobj->SetPlayerName(m_PlayerNameBuff1);
	}

	if (m_TwoPlayerModeFlag)
	{
		SMB_NEW(pobj,CPlayerObj);
		if (pobj)
		{
			m_PlayerList.Attach(pobj,OBJ_PLAYER_TWO, this);
			pobj->SetPlayerName(m_PlayerNameBuff2);
			pobj->TogglePuffBlow();
		}
	}

	InitSpriteList();

}

//------------------------------------------------------------------------------
//! \brief Initialise the high score table screen
//------------------------------------------------------------------------------
void CGame::InitHighScreen(void)
{
	m_Map.LoadBlockSet(SPR_ENDBLOX_DATA);
	m_MainCommand = MC_HIGHSCREEN;
	m_Map.GetMap(BLEV_HIGHMAP);
	m_FadeType = FADE_NORMAL;
	m_FadeFlag = FADE_FLAG_CONTINUE;

	PlayModule(SMOD_TITLE);

	m_Sprites.LoadRange(SPR_RFONT_0, SPR_RFONT_CURSOR);

	m_HiOffset = 0;
	m_ScrChgFlag = 1;
	m_pJoy1->key = 0;
	m_pJoy2->key = 0;
	m_MainCounter = 0;
	m_pJoy1->fire = 0;
	m_pJoy2->fire = 0;
}

//------------------------------------------------------------------------------
//! \brief Insert the hiscore into the highscore table
//!
//! 	\param score = The score
//!	\param name = Persons name (null terminated)
//!
//! 	\return The hiscore. 0 = Score was too low to go into the hiscore table
//------------------------------------------------------------------------------
HISCORES *CGame::InsertHiScore(int score, char *name)
{
	int cnt;
	int cnt2;
	HISCORES *hs;
	char *txt;
	char let;

	hs = m_HiScores;
	for (cnt=0; cnt<MAX_HISCORES; cnt++, hs++)
	{
		if (score>=hs->score)	// Insert score here?
		{
			// Shift the scores down a place
			for (cnt2=(MAX_HISCORES-1); cnt<cnt2; cnt2--)
			{
				memcpy( &m_HiScores[cnt2], &m_HiScores[cnt2-1], sizeof(HISCORES) );
			}
			hs->score = score;	// Insert the score
			txt = hs->name;
			txt[0] = txt[1] = txt[2] = txt[3] = ' ';
			for (cnt2=0; cnt2<4; cnt2++)
			{
				let = *(name++);
				if (let<=' ') break;
				*(txt++) = let;
			}
			return hs;
		}
	}
	return 0;
}

//------------------------------------------------------------------------------
//! \brief Draw the high score table
//------------------------------------------------------------------------------
void CGame::DrawHighTable(void)
{
	char text[32];
	int cnt;
	HISCORES *hs;

	DrawFont( 16*1, "HIGH SCORES" );

	hs = m_HiScores;
	for (cnt=1; cnt<=MAX_HISCORES; cnt++, hs++)
	{
		sprintf( text, "%d %d %c%c%c%c", cnt, hs->score,
			hs->name[0],hs->name[1],hs->name[2],hs->name[3] );
		DrawFont( 18*(cnt+4), text );
	}

}

//------------------------------------------------------------------------------
//! \brief Redraw the screen only when needed
//------------------------------------------------------------------------------
void CGame::RedrawScrIfNeeded(void)
{
	if ( (m_FadeType!=FADE_COMPLETE) || (m_ScrChgFlag) )
	{
		if (m_ScrChgFlag)
		{
			m_ScrChgFlag--;
		}
		m_pGameTarget->RedrawScreen();
	}

}

//------------------------------------------------------------------------------
//! \brief The high score screen main loop
//------------------------------------------------------------------------------
void CGame::HighScreenLoop(void)
{
	m_MainCounter++;

	m_Map.Draw();

	if ( (m_pJoy1->fire) || (m_pJoy2->fire) )	// Game start?
	{
		InitGetPlayerNameScreen(m_pJoy2->fire);
		return;
	}

	if (m_MainCounter == HISCREEN_SHOW_DELAY)	// Finished showing hiscores
	{
		m_FadeType = FADE_BLACK;
		m_FadeFlag = FADE_FLAG_WAIT;
	}
	if (m_MainCounter > HISCREEN_SHOW_DELAY)	// Finished showing hiscores
	{
		InitTitleScreen();
	}

	DrawHighTable();

	RedrawScrIfNeeded();

}


//------------------------------------------------------------------------------
//! \brief Control the global lighting
//!
//! 	\param desired_light = Colour to fade to (-1.0 = Black. 0 = Normal. 1.0 = White)
//!	\param speed = Fade speed 1 (slow) to 256 (fast)
//!
//! 	\return 0 = Still fading. Else Fade Completed
//------------------------------------------------------------------------------
int CGame::Fade( float desired_light, int speed )
{
	int changeflag;

	changeflag = 1;

	if (m_pGameTarget->m_Lighting < desired_light)
	{
		m_pGameTarget->m_Lighting += (speed/256.0f);
		if (m_pGameTarget->m_Lighting >=desired_light)
		{
			m_pGameTarget->m_Lighting = desired_light;
		}else
		{
			changeflag = 0;
		}
	}else	if (m_pGameTarget->m_Lighting > desired_light)
	{
		m_pGameTarget->m_Lighting -= (speed/256.0f);
		if (m_pGameTarget->m_Lighting <=desired_light)
		{
			m_pGameTarget->m_Lighting = desired_light;
		}else
		{
			changeflag = 0;
		}
	}

	if (changeflag) m_pGameTarget->m_FadeChangeFlag = 0; else m_pGameTarget->m_FadeChangeFlag = 1;
	return changeflag;
}

//------------------------------------------------------------------------------
//! \brief Toggle the Puff and Blow graphics.
//------------------------------------------------------------------------------
void CGame::TogglePuffBlow(void)
{
	CPlayerObj *pobj;

	pobj = (CPlayerObj *) m_PlayerList.m_pFirst;
	while (pobj)
	{
		pobj->TogglePuffBlow();
		pobj = (CPlayerObj *) pobj->m_pNext;
	}
}

//------------------------------------------------------------------------------
//! \brief Initialise the get player name screen
//!
//!	\param player_two_flag = 0 for one player, else two players
//------------------------------------------------------------------------------
void CGame::InitGetPlayerNameScreen(int player_two_flag)
{
	int cnt;

	m_TwoPlayerModeFlag = player_two_flag;
	m_Map.LoadBlockSet(SPR_ENDBLOX_DATA);
	m_MainCommand = MC_GETPLAYER;
	m_Map.GetMap(BLEV_HIGHMAP);
	m_FadeType = FADE_BLACK;
	m_FadeFlag = FADE_FLAG_WAIT;
	m_NameEditFadeUpFlag = 1;
	
	PlayModule(SMOD_TITLE);

	m_Sprites.LoadRange(SPR_RFONT_0, SPR_RFONT_CURSOR);

	m_ScrChgFlag = 1;
	m_MainCounter = 0;
	m_EditPlayerOneNameFlag = 1;

	for (cnt=0; cnt<4; cnt++)
	{
		m_PlayerNameBuff1[cnt] = '?';
		m_PlayerNameBuff2[cnt] = '?';
	}
	m_PlayerNameBuff1[cnt] = 0;
	m_PlayerNameBuff2[cnt] = 0;
	PrepareEditName();
}

//------------------------------------------------------------------------------
//! \brief Prepare editing name
//------------------------------------------------------------------------------
void CGame::PrepareEditName(void)
{
	m_HiOffset = 0;
	m_pJoy1->key = 0;
	m_pJoy2->key = 0;
	m_pJoy1->fire = 0;
	m_pJoy2->fire = 0;


}

//------------------------------------------------------------------------------
//! \brief The get player name screen main loop
//------------------------------------------------------------------------------
void CGame::GetPlayerNameLoop(void)
{
	char *nptr;
	char let;
	m_MainCounter++;

	m_Map.Draw();

	if (m_NameEditFadeUpFlag)
	{
		m_FadeType = FADE_NORMAL;
		m_FadeFlag = FADE_FLAG_CONTINUE;
		m_NameEditFadeUpFlag = 0;
	}

	if (m_pJoy2->fire)
	{
		m_TwoPlayerModeFlag = 1;
	}

	if (!m_TwoPlayerModeFlag)
	{
		DrawFont( 16*1, "ONE PLAYER MODE" );
	}else
	{
		DrawFont( 16*1, "TWO PLAYER MODE" );
	}

	if (m_EditPlayerOneNameFlag)
	{
		DrawFont( 16*4, "PLAYER ONE");
		nptr = m_PlayerNameBuff1;
	}else
	{
		DrawFont( 16*4, "PLAYER TWO");
		nptr = m_PlayerNameBuff2;
	}
	DrawFont( 16*5, "ENTER NAME" );

	// Flash the cursor
	let = nptr[m_HiOffset];
	if (m_MainCounter&4)
	{
		nptr[m_HiOffset] = '#';
	}
	if (!(m_MainCounter&3))
	{
		m_ScrChgFlag = 1;
	}

	DrawFont( 16*9, nptr );
	nptr[m_HiOffset] = let;

	RedrawScrIfNeeded();

	EditName(m_pJoy1, nptr);	// Easier to edit name using player 1
}

//------------------------------------------------------------------------------
//! \brief Edit the player high score name
//!
//! 	\param pjoy = The joystick (eg m_pjoy)
//!	\param nptr = The player name
//------------------------------------------------------------------------------
void CGame::EditName(JOYSTICK *pjoy, char *nptr)
{
	char key;
	key = pjoy->key;

	if (key)
	{
		m_ScrChgFlag = 1;
		pjoy->key = 0;
		if ( ((key>='A') && (key<='Z')) ||
			((key>='a') && (key<='z')) ||
			((key>='0') && (key<='9')) ||
			(key==' ') )
		{
			nptr[m_HiOffset] = toupper(key);
			m_HiOffset++;
			if (m_HiOffset>=4) m_HiOffset = 0;
		}else
		{
			if (pjoy->left)
			{
				m_HiOffset--;
				if (m_HiOffset<0) m_HiOffset = 3;
			}
			if (pjoy->right)
			{
				m_HiOffset++;
				if (m_HiOffset>=4) m_HiOffset = 0;
			}

		}
		m_MainCounter = 4;

		if ( (key==10) || (key==13) )	// Finished editing?
		{
			m_MainCounter = HISCREEN_SHOW_DELAY;
			pjoy->fire = 0;

			// Is the second player name required
			if ( (m_EditPlayerOneNameFlag) && (m_TwoPlayerModeFlag) )
			{
				// Copy the name
				m_EditPlayerOneNameFlag = 0;	// Edit player 2 name
				PrepareEditName();
				m_FadeType = FADE_BLACK;
				m_FadeFlag = FADE_FLAG_WAIT;
				m_NameEditFadeUpFlag = 1;
			}else
			{
				InitNewGame();
			}
		}
	}
}

//------------------------------------------------------------------------------
//! \brief Test to see if "Game Over" needs to be displayed
//------------------------------------------------------------------------------
void CGame::CheckForGameOver( void )
{
	if (m_GameOverFlag) return;
	if (m_PlayerList.m_pFirst) return;	// Player still alive

	InitGameOver();
}


