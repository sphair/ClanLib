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
// Methane Brothers Object lists (Source File)
//------------------------------------------------------------------------------

#include "player.h"
#include "objlist.h"
#include "gasobj.h"
#include "global.h"
#include "bitgroup.h"
#include "objtypes.h"
#include "suck.h"
#include "game.h"
#include "goodie.h"

//------------------------------------------------------------------------------
// Offset Defines
//------------------------------------------------------------------------------
#define GUN_YOFFSET		(17)
#define GUN_LEFTXOFFSET	(-4)
#define GUN_RIGHTXOFFSET	(10)

#define PLUNGE_YOFF		(2)
#define PLUNGE_LEFTXOFF	(12)
#define PLUNGE_RIGHTXOFF	(-6)

#define PLUNGE_MAXOUT		(6)

#define GUN_PICKY1		(-4)
#define GUN_PICKY2		(38)

#define GUN_PICKOFFSET		(32)
#define GUN_PICKWIDTH		(70)

#define BAD2GUN_YOFF		(-6)
#define BAD2GUN_LXOFF		(-27)
#define BAD2GUN_RXOFF		(18)

#define ROLLENGTH		(8)

//------------------------------------------------------------------------------
// General Defines
//------------------------------------------------------------------------------
#define GUN_READY	0
#define GUN_OUT		1
#define GUN_ATMAX	2
#define GUN_IN	 	3
#define GUN_SUCK	4
#define GUN_ENTERBADDIE 5

#define DEATHPAUSE	35		// How long the player dies for
#define IMMUNE_AFTER_KILL 60		// Number of frames the player is immune for

#define PLAYERLIVES	(5)		// Number of lives the player has

#define MAX_SCORE	(999999999)

//------------------------------------------------------------------------------
// Object Details
//------------------------------------------------------------------------------
static OBJSIZE player_size = {6,12,20,24};
static OBJMOVE player_move = {400,220,6*256,9*256,300,9*256,8*256};

#define HOLDBAD_LEN	200	// How long the player is allowed to hold a baddie

#define HOLDBAD_NUMX	10
#define HOLDBAD_NUMY	15
static int holdbad_xtab[HOLDBAD_NUMX] = {0,0,1,2,3,4,4,3,2,1};
static int holdbad_ytab[HOLDBAD_NUMY] = {4,3,2,1,0,1,2,3,3,2,1,0,1,2,3};

//------------------------------------------------------------------------------
// Egg Animation
//------------------------------------------------------------------------------
#define NUM_EGG_ANIM_FRAMES	24
static int anim_egg[NUM_EGG_ANIM_FRAMES] = {
	SPR_EGG_1,SPR_EGG_1,SPR_EGG_1,SPR_EGG_1,SPR_EGG_1,SPR_EGG_1,SPR_EGG_1,SPR_EGG_1,
	SPR_EGG_2,SPR_EGG_3,SPR_EGG_4,SPR_EGG_5,SPR_EGG_6,SPR_EGG_7,SPR_EGG_8,SPR_EGG_9,
	SPR_EGG_9,SPR_EGG_9,SPR_EGG_9,SPR_EGG_9,SPR_EGG_9,SPR_EGG_9,SPR_EGG_9,SPR_EGG_9
	};

//------------------------------------------------------------------------------
// Animation
//------------------------------------------------------------------------------

static int anm_puff_standleft[] =
	{
		SPR_PUFF_STAND_L2,SPR_PUFF_STAND_L2,SPR_PUFF_STAND_L2,SPR_PUFF_STAND_L1,
		ANM_RESTART
	};

static int anm_puff_standright[] =
	{
		SPR_PUFF_STAND_R2,SPR_PUFF_STAND_R2,SPR_PUFF_STAND_R2,SPR_PUFF_STAND_R1,
		ANM_RESTART
	};

static int anm_puff_left[] =
	{
		SPR_PUFF_LEFT1,SPR_PUFF_LEFT2,SPR_PUFF_LEFT3,SPR_PUFF_LEFT4,
		SPR_PUFF_LEFT5,SPR_PUFF_LEFT6,SPR_PUFF_LEFT7,SPR_PUFF_LEFT8,
		ANM_RESTART
	};

static int anm_puff_right[] =
	{
		SPR_PUFF_RIGHT1,SPR_PUFF_RIGHT2,SPR_PUFF_RIGHT3,SPR_PUFF_RIGHT4,
		SPR_PUFF_RIGHT5,SPR_PUFF_RIGHT6,SPR_PUFF_RIGHT7,SPR_PUFF_RIGHT8,
		ANM_RESTART
	};

static int anm_blow_standleft[] =
	{
		SPR_BLOW_STAND_L2,SPR_BLOW_STAND_L2,SPR_BLOW_STAND_L2,SPR_BLOW_STAND_L1,
		ANM_RESTART
	};

static int anm_blow_standright[] =
	{
		SPR_BLOW_STAND_R2,SPR_BLOW_STAND_R2,SPR_BLOW_STAND_R2,SPR_BLOW_STAND_R1,
		ANM_RESTART
	};

static int anm_blow_left[] =
	{
		SPR_BLOW_LEFT1,SPR_BLOW_LEFT2,SPR_BLOW_LEFT3,SPR_BLOW_LEFT4,
		SPR_BLOW_LEFT5,SPR_BLOW_LEFT6,SPR_BLOW_LEFT7,SPR_BLOW_LEFT8,
		ANM_RESTART
	};

static int anm_blow_right[] =
	{
		SPR_BLOW_RIGHT1,SPR_BLOW_RIGHT2,SPR_BLOW_RIGHT3,SPR_BLOW_RIGHT4,
		SPR_BLOW_RIGHT5,SPR_BLOW_RIGHT6,SPR_BLOW_RIGHT7,SPR_BLOW_RIGHT8,
		ANM_RESTART
	};

//------------------------------------------------------------------------------
// Puff graphics
//------------------------------------------------------------------------------
PLAYER_GFX_FRAMES SMBGFX_Puff = {
		anm_puff_left,
		anm_puff_right,
		anm_puff_standleft,
		anm_puff_standright,

		SPR_PUFF_GUN_L1,
		SPR_PUFF_GUN_R1,

		SPR_PUFF_GUN_L2,
		SPR_PUFF_GUN_R2,

		SPR_PUFF_HANDLE_L,
		SPR_PUFF_HANDLE_R,

		SPR_PUFF_RIGHT1,
		SPR_PUFF_HIT,
		SPR_PUFF_OUCH,

		SPR_GRAPPLE_LEFT,
		SPR_GRAPPLE_RIGHT,

		SPR_GRAPPLE_ROPE,
		SPR_GRAPPLE_HOOK,

		0,0,0
	};
//------------------------------------------------------------------------------
// Blow graphics
//------------------------------------------------------------------------------
PLAYER_GFX_FRAMES SMBGFX_Blow = {
		anm_blow_left,
		anm_blow_right,
		anm_blow_standleft,
		anm_blow_standright,

		SPR_BLOW_GUN_L1,
		SPR_BLOW_GUN_R1,

		SPR_BLOW_GUN_L2,
		SPR_BLOW_GUN_R2,

		SPR_BLOW_HANDLE_L,
		SPR_BLOW_HANDLE_R,

		SPR_BLOW_RIGHT1,
		SPR_BLOW_HIT,
		SPR_BLOW_OUCH,

		SPR_GRAPPLE2_LEFT,
		SPR_GRAPPLE2_RIGHT,

		SPR_GRAPPLE2_ROPE,
		SPR_GRAPPLE2_HOOK,

		-2,	// Due to blows longer hair
		-1,0
	};

//------------------------------------------------------------------------------
//! \brief Initialise the player object
//------------------------------------------------------------------------------
CPlayerObj::CPlayerObj()
{
	Reset();
	SetPlayerName("");
}

//------------------------------------------------------------------------------
//! \brief Destroy the player object
//------------------------------------------------------------------------------
CPlayerObj::~CPlayerObj()
{
	DeleteBaddie();
}

//------------------------------------------------------------------------------
//! \brief Draw the player object
//------------------------------------------------------------------------------
void CPlayerObj::Draw( void )
{
	switch (m_Command)
	{
		case PCMD_NORMAL:
			DrawNormal();
			break;
		case PCMD_DIE:
			DrawDie();
			break;
		case PCMD_RISEUP:
			DrawRiseUp();
			break;
	}

}

//------------------------------------------------------------------------------
//! \brief Draw the player - During normal gameplay
//------------------------------------------------------------------------------
void CPlayerObj::DrawNormal( void )
{
	int frm;
	CBitmapGroup *sptr;

	sptr = &(m_pGame->m_Sprites);

	if (!(m_Immune&1))		// Flash the player if immune
	{
		// Wings?
		if (!m_YCentre)
		{
			sptr->Draw( SPR_POWER_WINGS, m_XPos, m_YPos-13 );
		}

		if (m_Frame) sptr->Draw( m_Frame, m_XPos, m_YPos + m_pGFX->player_y_offset );

		if (m_Mask)
		{
			frm = ((m_MaskOffset>>1) & 3) + SPR_MASK_R1;
			if (m_Dir==DIR_LEFT) frm+=4;
			sptr->Draw( frm, m_XPos, m_YPos-4 );
		}

		if (m_HandleFrame)
			sptr->Draw( m_HandleFrame, m_HandleXPos, m_HandleYPos );

		if (m_pBaddie) if (m_GunFlag!=GUN_SUCK)		// Draw inside gun
			m_pBaddie->DrawSuck();

		if (m_GunFrame)
			sptr->Draw( m_GunFrame, m_GunXPos, m_GunYPos );

		if (m_pBaddie) if (m_GunFlag==GUN_SUCK)		// Draw outside gun
			m_pBaddie->DrawSuck();

		if (m_NumFrame) m_pGame->m_Sprites.Draw( m_NumFrame, m_NumXPos, m_NumYPos );
	}
	if (m_EggFrame)	sptr->Draw( m_EggFrame, m_XPos, m_YPos-20 );
}

//------------------------------------------------------------------------------
//! \brief Load the player graphics
//------------------------------------------------------------------------------
void CPlayerObj::LoadGfx( void )
{
	m_pGame->m_Sprites.LoadRange(SPR_PUFF_LEFT1, SPR_PUFF_HANDLE_L);
	m_pGame->m_Sprites.LoadRange(SPR_BLOW_LEFT1, SPR_BLOW_HANDLE_L);
	m_pGame->m_Sprites.LoadRange(SPR_GAS_CLOUD1, SPR_GAS_RIGHT4);
	m_pGame->m_Sprites.LoadRange(SPR_STARS_1, SPR_STARS_8);
	m_pGame->m_Sprites.LoadRange(SPR_GRAPPLE_HOOK, SPR_GRAPPLE_ROPE);
	m_pGame->m_Sprites.LoadRange(SPR_GRAPPLE2_HOOK, SPR_GRAPPLE2_ROPE);
}

//------------------------------------------------------------------------------
//! \brief Reset the player object (object members)
//!
//! (Used when game restarts)\n
//! This function calls ResetAll()
//------------------------------------------------------------------------------
void CPlayerObj::Reset( void )
{
	m_Score = 0;
	m_ExtraLifeCount = 0;
	m_Lives = PLAYERLIVES;
	m_Cards = 0;
	m_NoDieCnt = 0;

	m_OrigXPos = m_XPos = 100;
	m_OrigYPos = m_YPos = 100;

	m_BlowFlag = 0;		// 0 = Use PUFF else use BLOW

	ResetAll();
}

//------------------------------------------------------------------------------
//! \brief Get the joystick depending on player one or player two
//!
//! 	\return The joystick
//------------------------------------------------------------------------------
JOYSTICK *CPlayerObj::GetJoy(void)
{
	JOYSTICK *jptr;
	if (m_Type == OBJ_PLAYER_ONE)
	{
		jptr = m_pGame->m_pJoy1;
	}else	jptr = m_pGame->m_pJoy2;
	return jptr;
}

//------------------------------------------------------------------------------
//! \brief Move the player using the joystick values
//------------------------------------------------------------------------------
void CPlayerObj::MovePlayer( void )
{
	JOYSTICK *jptr;

	int old_xpower;
	int old_xmass;
	int old_ymass;

	old_xpower = m_XPower;
	old_xmass = m_XMass;
	old_ymass = m_YMass;

	// Check for oil!
	if (CheckHitFlag( m_pGame->m_GoodieList, FLAG_STICKY ))
	{
		m_XPower = m_XPower >> 2;
		m_XMass = m_XMass >> 2;
	}

	jptr = GetJoy();

	if (!m_GiddyFlag)
	{
		if (jptr->left)
		{
			SetMoveLeft();
		}
		if (jptr->right)
		{
			SetMoveRight();
		}
	}else		// Reverse Controls
	{
		if (jptr->right)
		{
			SetMoveLeft();
		}
		if (jptr->left)
		{
			SetMoveRight();
		}
	}	

	if (!m_YCentre)
	{
		if (jptr->up)
		{
			SetMoveUp();
		}
		if (jptr->down)
		{
			SetMoveDown();
		}

	}else
	{
		if (jptr->up)
		{
			if (!m_Y_Flag)
			{
				AddScore( 2 );
				SetMoveUp();
			}

			m_YMass = m_YMass>>1;			// Control the jump height distance
		}
	}
	MoveObject();

	m_XPower = old_xpower;
	m_XMass = old_xmass;
	m_YMass = old_ymass;



}
//------------------------------------------------------------------------------
//! \brief Control the player animation
//------------------------------------------------------------------------------
void CPlayerObj::ControlAnim( void )
{
	if ((m_XInert<=-256) || (m_XInert>=256))	// Is moving?
	{
		AnimateXInert(m_pGFX->pfrm_left, m_pGFX->pfrm_right);
		m_StandPause = 2;
	}else			// Still
	{
		if (m_StandPause>0)		// To prevent frame flicking when turning quickly
		{
			m_StandPause--;
		}else
		{
			AnimateDir(m_pGFX->pfrm_stand_left, m_pGFX->pfrm_stand_right, 8);
		}
	}

}

//------------------------------------------------------------------------------
//! \brief Control the gun animation
//!
//! 	\param old_inertx = old inertia value x
//! 	\param old_inerty = old inertia value y
//------------------------------------------------------------------------------
void CPlayerObj::GunAnim( int old_inertx, int old_inerty )
{

	if (m_State&(STATE_HORIZ))			// Hit a left/right wall?
	{
		if (!m_GunInertX)
		{
			m_GunInertX = old_inertx >> 9;
			if (m_GunInertX <0) m_GunInertX++;
		}
	}

	if (m_State&(STATE_VERT))			// Hit a up/down wall?
	{
		if (!m_GunInertY)
		m_GunInertY = old_inerty >> 9;
	}

	m_GunYPos = m_YPos + GUN_YOFFSET;				// Position gun
	if (m_Dir==DIR_LEFT)
	{
		m_GunXPos = m_XPos + GUN_LEFTXOFFSET;
		m_GunFrame = m_pGFX->frm_gun_left1;

	}else
	{
		m_GunXPos = m_XPos + GUN_RIGHTXOFFSET;
		m_GunFrame = m_pGFX->frm_gun_right1;

	}

	if ((m_XInert<=-256)||(m_XInert>=256))  m_GunInertX = 0;	// If moving stop wobble
	if ((m_YInert<=-256)||(m_YInert>=256))  m_GunInertY = 0;

	m_GunXPos += m_GunInertX;			// Control the wobble
	if (m_GunInertX<0) m_GunInertX++;
	m_GunInertX = 0 - m_GunInertX;

	m_GunYPos += m_GunInertY;
	if (m_GunInertY<0) m_GunInertY++;
	m_GunInertY = 0 - m_GunInertY;


	m_HandleYPos = m_GunYPos + PLUNGE_YOFF;		// Position handle
	if (m_Dir==DIR_LEFT)
	{
		m_HandleXPos = m_GunXPos + PLUNGE_LEFTXOFF + m_PlungeOffset;
		m_HandleFrame = m_pGFX->frm_handle_left;
	}else
	{
		m_HandleXPos = m_GunXPos + PLUNGE_RIGHTXOFF - m_PlungeOffset;
		m_HandleFrame = m_pGFX->frm_handle_right;
	}

}

//------------------------------------------------------------------------------
//! \brief Eject the gas from the gun
//------------------------------------------------------------------------------
void CPlayerObj::EjectGas(void)
{
	m_pGame->m_pSound->PlaySample(SND_BLOW, m_XPos);
	AddScore( 1 );
	if (!m_GasFireFlag)
	{
		CGasObj *pobj;
		SMB_NEW(pobj,CGasObj);
		if (pobj)
		{
			m_pGame->m_GasList.Attach(pobj, OBJ_GAS, m_pGame);
			pobj->SetupEject( this );
		}
	}else
	{
		CFireLRObj *fobj;
		SMB_NEW(fobj,CFireLRObj);
		if (fobj)
		{
			m_pGame->m_GoodieList.Attach(fobj, OBJ_FIRELR, m_pGame);
			fobj->Setup( m_GunXPos, m_GunYPos+1, m_Dir );
		}
	}
}

//------------------------------------------------------------------------------
//! \brief Check to see if able to suck a baddie
//------------------------------------------------------------------------------
void CPlayerObj::CheckSuck( void )
{
	CLinkObject *pobj;
	CSuckable *psuckobj;
	CSuckable *pnearest;

	int xp1,yp1,xwidth,yheight;
	int type;

	yp1 = m_GunYPos+GUN_PICKY1;
	yheight = GUN_PICKY2;

	if (m_Dir==DIR_LEFT)
	{
		xp1 = m_GunXPos - GUN_PICKWIDTH;

	}else
	{
		xp1 = m_GunXPos;
	}

	xwidth = GUN_PICKWIDTH+GUN_PICKOFFSET;

	// Gas objects have higher priority... (the search is split up)

	pnearest = 0;
	pobj = m_pGame->m_GasList.m_pFirst;
	while (pobj)
	{
		if (pobj->m_Flags&FLAG_SUCKABLE)	// Correct object type?
		{
			type = pobj->m_Type;
			if (type == OBJ_GAS)	// Must be a gas type
			{
				if (pobj->CheckHit( xp1,yp1,xwidth,yheight ))
				{
					psuckobj = (CSuckable *) pobj;
					pnearest = CheckClose(psuckobj, pnearest, xp1);
				}
			}
		}
		pobj = pobj->m_pNext;
	}

	if (pnearest)
	{
		InitSuck( pnearest );
		return;
	}

	pobj = m_pGame->m_GasList.m_pFirst;
	while (pobj)
	{
		if (pobj->m_Flags&FLAG_SUCKABLE)	// Correct object type?
		{

			type = pobj->m_Type;
			if ((type == OBJ_TOMATO) || (type == OBJ_BOWLING)
					 || (type == OBJ_BLOCK) || (type == OBJ_SPRING))
			{
				// To improve the program responsiveness - y suck distances
				// are less for blocks and springs
				if ((type == OBJ_BLOCK) || (type == OBJ_SPRING))
				{
					yheight = GUN_PICKY2-16;
					xwidth = GUN_PICKWIDTH/3;
					if (m_Dir==DIR_LEFT)
					{
						xp1 = m_GunXPos - (GUN_PICKWIDTH/3) - 12;
					}else
					{
						xp1 = m_GunXPos + 40;
					}

				}else
				{
					xwidth = GUN_PICKWIDTH+GUN_PICKOFFSET;
					yheight = GUN_PICKY2;
					if (m_Dir==DIR_LEFT)
					{
						xp1 = m_GunXPos - GUN_PICKWIDTH;
					}else
					{
						xp1 = m_GunXPos;
					}
				}


				if (pobj->CheckHit( xp1,yp1,xwidth,yheight ))
				{
					psuckobj = (CSuckable *) pobj;
					pnearest = CheckClose(psuckobj, pnearest, xp1);
				}
			}
		}
		pobj = pobj->m_pNext;
	}
	if (pnearest)
	{
		InitSuck( pnearest );
		return;
	}

}

//------------------------------------------------------------------------------
//! \brief Check to see if an object is closer to this than another object
//!
//! 	\param pobj1 = Object 1 (MUST EXIST)
//!	\param pobj2 = Object 2 (Can be zero)
//!	\param xpos = This object xpos origin
//!
//! 	\return The closest
//------------------------------------------------------------------------------
CSuckable *CPlayerObj::CheckClose(CSuckable *pobj1,CSuckable *pobj2, int xpos)
{
	int dist1;
	int dist2;

	if (!pobj2) return pobj1;

	dist1 = pobj1->m_XPos + (pobj1->m_X2>>1) - xpos;
	dist2 = pobj2->m_XPos + (pobj2->m_X2>>1) - xpos;

	if (m_Dir==DIR_LEFT)
	{
		if (dist1<dist2) return pobj2; else return pobj1;
	}
	if (dist1<dist2) return pobj1;
	return pobj2;

}

//------------------------------------------------------------------------------
//! \brief Initialise the sucking into the suck
//!
//! 	\param pnearest = Baddie to suck
//------------------------------------------------------------------------------
void CPlayerObj::InitSuck(CSuckable *pnearest)
{
	if (pnearest->m_Type==OBJ_GAS)
	{
		CGasObj *pgasobj;
		pgasobj = (CGasObj *) pnearest;
		m_pBaddie = pgasobj->GrabBaddie(this);

	}else	// Sucking an object?
	{
		m_pBaddie = pnearest;
		pnearest->UnLink();
		pnearest->SetupCaught();

	}
	SetupSuckBaddie();
}

//------------------------------------------------------------------------------
//! \brief Setup suck a baddie to the gun
//------------------------------------------------------------------------------
void CPlayerObj::SetupSuckBaddie( void )
{
	m_GunFlag = GUN_SUCK;
	m_pBaddie->SetupSuck();
}

//------------------------------------------------------------------------------
//! \brief Calculate the offset at the gun - taking the players direction into account.
//!
//! (Used for sucking a baddie into the gun)
//!
//! 	/param gun_xpos - Where to store the coord X (REFERENCE)
//! 	/param gun_ypos - Where to store the coord Y (REFERENCE)
//------------------------------------------------------------------------------
void CPlayerObj::CalcSuckGun( int &gun_xpos, int &gun_ypos )
{
	if (m_Dir==DIR_LEFT)			// Calculate where to go to
		gun_xpos = m_GunXPos+BAD2GUN_LXOFF;
	else
		gun_xpos = m_GunXPos+BAD2GUN_RXOFF;

	gun_ypos=m_GunYPos+BAD2GUN_YOFF;

}

//------------------------------------------------------------------------------
//! \brief Suck a baddie to the gun
//------------------------------------------------------------------------------
void CPlayerObj::SuckBaddie( void )
{
	int gun_xpos,gun_ypos;
	CalcSuckGun(gun_xpos, gun_ypos);
	if (m_pBaddie->SuckBaddie(gun_xpos, gun_ypos))
	{
		InitTimer();
		m_HoldCnt = HOLDBAD_LEN;
		m_GunFlag = GUN_ENTERBADDIE;
		m_PlungeSpitFlag = 0;
	}

}

//------------------------------------------------------------------------------
//! \brief Suck a baddie inside the gun
//------------------------------------------------------------------------------
void CPlayerObj::EnterBaddie( void )
{
	int suckfrm;
	int gun_xpos,gun_ypos;

	CalcSuckGun(gun_xpos, gun_ypos);

	if (m_pBaddie->m_SuckFrmCnt>=NUMSUCKANIM)	// All frames shown?
	{
		m_pBaddie->Link();
		m_pBaddie->m_XPos = gun_xpos;
		m_pBaddie->m_YPos = m_YPos + GUN_YOFFSET + BAD2GUN_YOFF;
		m_pBaddie->SetRoll(m_Dir, ROLLENGTH);
		m_pBaddie = 0;		// Clear baddie pointer
		m_GunFlag = GUN_READY;
		return;
	}

	m_pBaddie->EnterBaddie(gun_xpos, gun_ypos, m_Dir);
	m_pBaddie->m_SuckFrmCnt+=m_PlungeSuckerRate;
	suckfrm = m_pBaddie->m_SuckFrmCnt;

	if ((suckfrm>=6) && (suckfrm<=8))		// Set filled gun graphic
	{
		if (m_Dir==DIR_LEFT)
		{
			m_GunFrame = m_pGFX->frm_gun_left2;
		}else
		{
			m_GunFrame = m_pGFX->frm_gun_right2;
		}
	}

	if ((suckfrm==7) || (suckfrm==8))	// Fully in gun
	{
		if (GetJoy()->fire)
		{
			m_HoldCnt--;
			ControlTimer();
			if (m_HoldCnt>0)	// TimeOut?
			{
				m_pBaddie->m_SuckFrmCnt-=m_PlungeSuckerRate;	// Hold baddie in gun
			}
		}
	}

	if (suckfrm>7)	// Leaving Gun?
	{
		m_PlungeOffset -= m_PlungeStepRate;	// Move back in
		if (m_PlungeOffset<=0) m_PlungeOffset = 0;
	}
	if (suckfrm>8)	// Spit Sound?
	{
		if (!m_PlungeSpitFlag)
		{
			m_PlungeSpitFlag = 1;
			m_pGame->m_pSound->PlaySample(SND_SPIT, m_XPos);
		}
	}
}

//------------------------------------------------------------------------------
//! \brief Control the players gun
//------------------------------------------------------------------------------
void CPlayerObj::ControlGun( void )
{
	switch (m_GunFlag)
	{
		case (GUN_READY):
		{
			if (!(GetJoy()->fire)) break;	// Fire not pressed

			m_PlungeOffset = m_PlungeStepRate;	// Set initial offset
			m_GunFlag = GUN_OUT;
			CheckSuck();
			break;
		}
		case (GUN_OUT):
		{
			m_PlungeOffset += m_PlungeStepRate;	// Move plunger out
			if (m_PlungeOffset < PLUNGE_MAXOUT) break;	// Not all the way out
			m_PlungeOffset = PLUNGE_MAXOUT;
			m_GunFlag = GUN_ATMAX;
			break;
		}
		case (GUN_ATMAX):
		{
			if (GetJoy()->fire) break;	// Wait until fire is released
			m_PlungeOffset -= m_PlungeStepRate;	// Move back in
			m_GunFlag = GUN_IN;
			break;
		}
		case (GUN_IN):
		{
			m_PlungeOffset -= m_PlungeStepRate;	// Move back in
			if (m_PlungeOffset>0) break;	// Wait until all the way back in
			m_PlungeOffset = 0;
			m_GunFlag = GUN_READY;
			EjectGas();
			break;
		}
		case (GUN_SUCK):
		{
			SuckBaddie();
			break;
		}
		case (GUN_ENTERBADDIE):
		{
			EnterBaddie();
			break;
		}
	}
}

//------------------------------------------------------------------------------
//! \brief Do the player object
//------------------------------------------------------------------------------
void CPlayerObj::Do( void )
{
	m_NumFrame = 0;
	switch (m_Command)
	{
		case PCMD_NORMAL:
			CheckExtraLife();
			DoNormal();
			break;
		case PCMD_DIE:
			DoDie();
			break;
		case PCMD_RISEUP:
			DoRiseUp();
			break;
	}
}

//------------------------------------------------------------------------------
//! \brief Do the player - During normal gameplay
//------------------------------------------------------------------------------
void CPlayerObj::DoNormal( void )
{
	int old_inertx;
	int old_inerty;

	// Control the egg
	if (m_EggFlag)
	{
		if (m_EggOffset<NUM_EGG_ANIM_FRAMES)
		{
			m_EggFrame = anim_egg[m_EggOffset];
			if (!(m_pGame->m_MainCounter&1)) m_EggOffset++;
		}else if (m_EggOffset<(NUM_EGG_ANIM_FRAMES+32))	// Hold Frame
		{
			m_EggOffset++;
		}
		else
		{
			m_pGame->CreateCloud( m_XPos, m_YPos-16 );
			m_EggFlag = m_EggFrame = 0;
		}
	}

	m_MaskOffset++;

	if (m_Immune)		// Control the immunity
	{
		m_PlayerFlags |= PF_IS_IMMUNE;
		m_Immune--;
		if (!m_Immune)
		{
			m_PlayerFlags &= (~PF_IS_IMMUNE);
		}
	}

	old_inertx = m_XInert;
	old_inerty = m_YInert;

	MovePlayer();
	GunAnim(old_inertx, old_inerty);
	ControlGun();
	ControlAnim();

}

//------------------------------------------------------------------------------
//! \brief Delete the baddie if it is held in the players gun
//------------------------------------------------------------------------------
void CPlayerObj::DeleteBaddie( void )
{
	if (m_pBaddie)	// Is still containing a baddie?
	{
		delete m_pBaddie;	// Then delete it
		m_pBaddie = 0;
	}
}

//------------------------------------------------------------------------------
//! \brief Release the baddie from the players gun
//!
//! (Used when the player dies)
//------------------------------------------------------------------------------
void CPlayerObj::ReleaseBaddie( void )
{
	if (m_pBaddie)	// Is still containing a baddie?
	{
		m_pBaddie->m_XPos = m_XPos;
		m_pBaddie->m_YPos = m_YPos + GUN_YOFFSET + BAD2GUN_YOFF;
		m_pBaddie->Link();
		m_pBaddie->m_YPos = m_pBaddie->m_YPos - m_pBaddie->m_Release_YOff -8;
		m_pBaddie->ReleaseFromRoll();
		m_pBaddie = 0;
	}
}

//------------------------------------------------------------------------------
//! \brief Reset the player standard settings
//!
//! (Used when progressing to the next level)\n
//! - Reset the player position and size
//------------------------------------------------------------------------------
void CPlayerObj::ResetStd( void )
{
	m_OldXPos = m_XPos;
	m_OldYPos = m_YPos;

	if (m_BlowFlag)
	{
		m_pGFX = &SMBGFX_Blow;
	}else
	{
		m_pGFX = &SMBGFX_Puff;
	}

	m_pSequence = m_pGFX->pfrm_right;
	m_Frame = m_pGFX->frm_face_right;

	m_HandleFrame = 0;
	m_GunFrame = 0;
	m_pBaddie = 0;
	m_GunFlag = 0;
	m_GunInertX = 0;
	m_GunInertY = 0;
	m_PlungeOffset = 0;

	m_XInert = m_YInert = 0;
	m_Delay = 0;
	m_State = 0;
	m_Flags = FLAG_ACCURATE;
	if (m_Type == OBJ_PLAYER_ONE)
	{
		m_Dir = DIR_RIGHT;
	}else	m_Dir = DIR_LEFT;

	m_PlayerFlags = PF_IS_ALIVE;
	InitSize(player_size);
	m_GasFireFlag = 0;
	m_Mask = 0;
	m_MaskOffset = 0;
	m_GiddyFlag = 0;
	m_Immune = 0;
	m_Command = PCMD_NORMAL;
	m_Y_Flag = -1;
	// Cancel wings!
	if (!m_YCentre) m_YCentre = 7*256;

	m_NumFrame = 0;
	m_NumXPos = 0;
	m_NumYPos = 0;
	m_NumXOff = 0;
	m_NumYOff = 0;
	m_HoldCnt = 0;
	m_EggFlag = 0;
	m_EggOffset = 0;
	m_EggFrame = 0;

	if (m_YCentre == 3*256)	// Have you got a feather?
	{
		// Give the Y power of a turbo
		m_YMass = 350;
		m_YPower = 16*256;
		m_Max_YSpeed = 10 * 256;
		m_YCentre = 10 * 256;

	}
}

//------------------------------------------------------------------------------
//! \brief Reset the player settings
//!
//! (Used when the player is killed)\n
//! This function calls ResetStd()
//------------------------------------------------------------------------------
void CPlayerObj::ResetAll( void )
{
	InitMovement(player_move);

	m_PlungeStepRate = 2;
	m_PlungeSuckerRate = 1;
	m_GasLength = 10;
	m_GasStep = 6;
	ResetStd();
}

//------------------------------------------------------------------------------
//! \brief Initialise the player when it enters a new level
//------------------------------------------------------------------------------
void CPlayerObj::NewLevel( void )
{
	DeleteBaddie();
	ResetStd();
	LoadGfx();
}

//------------------------------------------------------------------------------
//! \brief Kill the player - IE loose a live
//------------------------------------------------------------------------------
void CPlayerObj::KillPlayer( void )
{
	int sample;
	if (m_pGame->m_MainCounter&1) sample = SND_CRYING; else sample = SND_DIE2;
  	m_pGame->m_pSound->PlaySample(sample, m_XPos);

	m_pGame->RandGoodie( m_XPos + 8, m_YPos, GOODIE_FOOD, 0, -6*256 );
	ReleaseBaddie();
	ResetAll();
	m_PlayerFlags &= (~PF_IS_ALIVE);
	m_Command = PCMD_DIE;
	m_YInert = -6*256;
	m_DeadPause = 0;
	m_NoDieCnt = 0;
}

//------------------------------------------------------------------------------
//! \brief Do the player - During dying
//------------------------------------------------------------------------------
void CPlayerObj::DoDie( void )
{
	m_pGame->SetAngryBaddies(0);
	m_DeadPause++;
	if (m_DeadPause == (DEATHPAUSE/2))
	{
  		m_pGame->m_pSound->PlaySample(SND_TWINKLE, m_XPos);
	}
	if (m_DeadPause >= DEATHPAUSE)	// Finished dying?
	{
		m_Lives--;	// Loose a life
		if (m_Lives<0)	// GAME OVER
		{
			m_Lives = 0;
			if (m_DeadPause == (DEATHPAUSE+10))	// Only do once after a delay
			{
				UnLink();	// Unlink this player
				m_pGame->m_DeadPlayerList.Attach(this, m_Type, m_pGame);	// Link back in
			}
		}else
		{

			m_XPos = m_OrigXPos;
			m_YPos = m_OrigYPos;
			ResetAll();
			m_Immune = IMMUNE_AFTER_KILL;
			m_PlayerFlags |= PF_IS_IMMUNE;
		}
		return;
	}
	MoveObject();
}

//------------------------------------------------------------------------------
//! \brief Draw the player - During dying
//------------------------------------------------------------------------------
void CPlayerObj::DrawDie( void )
{
	CBitmapGroup *sptr;

	sptr = &(m_pGame->m_Sprites);

	sptr->Draw( m_pGFX->frm_hit, m_XPos, m_YPos );
	sptr->Draw( SPR_STARS_1 + (m_DeadPause & 7), m_XPos + 2, m_YPos - 4 );

	if (m_DeadPause > (DEATHPAUSE/2))	// Display Pole?
	{
		sptr->Draw( m_pGFX->frm_ouch, m_XPos + 18, m_YPos - 4 );
	}

}

//------------------------------------------------------------------------------
//! \brief Set the player rising up, out of the screen
//------------------------------------------------------------------------------
void CPlayerObj::SetRiseUp( void )
{
	m_AtTopFlag = 0;
	m_Command = PCMD_RISEUP;
	m_PlayerFlags |= PF_IS_IMMUNE;

	if (m_Dir==DIR_LEFT)
	{
		m_Frame = m_pGFX->frm_grapple_left;
		m_GrappleXPos = m_XPos - 6 + m_pGFX->grapple_left_x_offset;
	}else
	{
		m_Frame = m_pGFX->frm_grapple_right;
		m_GrappleXPos = m_XPos + 4 + m_pGFX->grapple_right_x_offset;
	}
	m_GrappleYPos = m_YPos - 30;
	m_GoingUpFlag = 0;
	m_XInert = 0;
}

//------------------------------------------------------------------------------
//! \brief Do the player rising up, out of the screen
//!
//! NOTE - THE VALUES IN THIS FUNCTION have been set to the 'best looking'
//!  graphic results - Ajust them - it looks daft (ie. a sinus table is needed)
//------------------------------------------------------------------------------
void CPlayerObj::DoRiseUp( void )
{
	int max;
	if (!m_GoingUpFlag)
	{
		m_GrappleYPos -=15;
		if (m_GrappleYPos < -30)
		{
			m_GrappleYPos = -30;
			m_GoingUpFlag = 1;
		}
		return;
	}
	max = (m_YPos - m_GrappleYPos)>>5;			// /32
	if (max<0) max = 0;

	if (m_Dir==DIR_RIGHT)	// Wobble Left or Right?
	{
		m_XInert++;
		if (m_XInert>max) m_Dir = DIR_LEFT;
	}else
	{
		m_XInert--;
		if (m_XInert<-max) m_Dir = DIR_RIGHT;
	}

	m_YPos-=8;
	if (m_YPos<-64)
	{
		m_YPos = -64;
		m_AtTopFlag = 1;
	}
}

//------------------------------------------------------------------------------
//! \brief Draw the player rising up, out of the screen
//!
//! Very yucky code!!! - but it works
//------------------------------------------------------------------------------
void CPlayerObj::DrawRiseUp( void )
{
	int seg_xpos;
	int seg_ypos;
	int xfract;
	int dest_ypos;
	int num_segs;
	int xoffset;
	int val;
	int theoffset;

	seg_xpos = m_GrappleXPos + 10;
	seg_ypos = m_GrappleYPos + 28;
	dest_ypos = m_YPos;

	// Calculate the player x offset fraction
	num_segs = (dest_ypos - seg_ypos);
	if (num_segs<0) num_segs = 0;
	num_segs = num_segs >> 4;		// Divide by 16
	if (!num_segs) num_segs = 1;	// No div by zero error

	xfract = (m_XInert<<8) / num_segs;
	xoffset = 0;

	theoffset = 0;
	for (; seg_ypos < dest_ypos; seg_ypos+=16, theoffset += val)
	{
		m_pGame->m_Sprites.Draw( m_pGFX->frm_grapple_rope, seg_xpos+theoffset, seg_ypos, GFX_NOWRAP );
		xoffset += xfract;
		val = xoffset>>8;	// Get the actual value
		xoffset &=0xff;		// Mask only the fraction
	}
	m_pGame->m_Sprites.Draw( m_pGFX->frm_grapple_rope, seg_xpos+theoffset, dest_ypos, GFX_NOWRAP );
	m_pGame->m_Sprites.Draw( m_pGFX->frm_grapple_hook, m_GrappleXPos, m_GrappleYPos, GFX_NOWRAP );
	m_pGame->m_Sprites.Draw( m_Frame, m_XPos+theoffset, m_YPos, GFX_NOWRAP );
}

//------------------------------------------------------------------------------
//! \brief Control the number above the object
//------------------------------------------------------------------------------
void CPlayerObj::ControlTimer( void )
{
	int togo;
	int cnt;

	// Control the number catching up to the player
	for (cnt=1; cnt<PNUM_DELAY; cnt++)
	{
		m_NumXList[cnt-1]=m_NumXList[cnt];
		m_NumYList[cnt-1]=m_NumYList[cnt];
	}
	m_NumXList[PNUM_DELAY-1] = m_XPos;	
	m_NumYList[PNUM_DELAY-1] = m_YPos;

	// Display the number
	togo = (10 * (m_HoldCnt)) / (HOLDBAD_LEN-16);	// 0 to 10
	if (togo<0) togo = 0;
	if (togo>9) return;		// Add a delay before number appears

	m_NumFrame = SPR_FNT_G0 + togo;
	m_NumXPos = m_NumXList[0]+holdbad_xtab[m_NumXOff]+8;
	m_NumYPos = m_NumYList[0]+holdbad_ytab[m_NumYOff]-12;

	m_NumXOff++;
	if (m_NumXOff>=HOLDBAD_NUMX) m_NumXOff=0;
	m_NumYOff++;
	if (m_NumYOff>=HOLDBAD_NUMY) m_NumYOff=0;

}

//------------------------------------------------------------------------------
//! \brief Initialise the number above the player
//------------------------------------------------------------------------------
void CPlayerObj::InitTimer( void )
{
	int cnt;
	for (cnt=0; cnt<PNUM_DELAY; cnt++)
	{
		m_NumXList[cnt] = m_XPos;
		m_NumYList[cnt] = m_YPos;
	}
}

//------------------------------------------------------------------------------
//! \brief Add score to the player
//!
//! 	\param value = Score to add
//------------------------------------------------------------------------------
void CPlayerObj::AddScore( int value )
{
	m_Score+=value;
	if (m_Score>MAX_SCORE) m_Score = MAX_SCORE;
}

//------------------------------------------------------------------------------
//! \brief Check to see if the player should gain an extra life
//------------------------------------------------------------------------------
void CPlayerObj::CheckExtraLife( void )
{

	unsigned int value;
	// Extra life after every 100,000 points
	value = m_Score / 100000;
	if (value > m_ExtraLifeCount)
	{
		m_Lives++;
		m_pGame->CreateMessage( 32, "EXTRA LIFE", 0 );
		m_ExtraLifeCount = value;
	}
}

//------------------------------------------------------------------------------
//! \brief Toggle the Puff and Blow graphics
//------------------------------------------------------------------------------
void CPlayerObj::TogglePuffBlow( void )
{
	if (m_BlowFlag)
	{
		m_BlowFlag = 0;
		m_pGFX = &SMBGFX_Puff;
	}else
	{
		m_BlowFlag = 1;
		m_pGFX = &SMBGFX_Blow;
	}
}

//------------------------------------------------------------------------------
//! \brief Set the player name
//!
//!	\param nptr = The player name
//------------------------------------------------------------------------------
void CPlayerObj::SetPlayerName(const char *nptr)
{
	int cnt;
	char let;
	for (cnt=0; cnt<4;cnt++)
	{
		let = *nptr;
		if (let == 0)	// Pad the player name with blanks
		{
			let = ' ';
		}else nptr++;
		m_Name[cnt] = let;
	}
	m_Name[cnt] = 0;
}

