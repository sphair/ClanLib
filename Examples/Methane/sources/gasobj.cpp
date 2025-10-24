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
// Methane Brothers Gas Object lists (Source File)
//------------------------------------------------------------------------------

#include "stdio.h"
#include "stdlib.h"
#include "gasobj.h"
#include "global.h"
#include "bitgroup.h"
#include "player.h"
#include "maps.h"
#include "objtypes.h"
#include "suck.h"
#include "game.h"  

//------------------------------------------------------------------------------
// Offset Defines
//------------------------------------------------------------------------------
#define GAS_FROM_GUN_LX		(30)
#define GAS_FROM_GUN_RX		(24)
#define GAS_FROM_GUN_Y		(-8)
#define HATCH_OFFSETY		(5)

#define GAS_FLASHPOINT		(240)
#define GAS_RELEASEPOINT	(GAS_FLASHPOINT+60)

//------------------------------------------------------------------------------
// Animation
//------------------------------------------------------------------------------

static int anm_gas_anim[] =
	{
		SPR_GAS_CLOUD1,SPR_GAS_CLOUD2,SPR_GAS_CLOUD3,SPR_GAS_CLOUD4,
		ANM_RESTART
	};
static int anm_gas_fade[] =
	{
		SPR_GAS_FADE1,SPR_GAS_FADE2,SPR_GAS_FADE3,SPR_GAS_FADE4,SPR_GAS_FADE5,
		ANM_RESTART
	};

static int anm_gas_left[] =
	{
		SPR_GAS_LEFT1,SPR_GAS_LEFT2,SPR_GAS_LEFT3,SPR_GAS_LEFT4,
		ANM_RESTART
	};
static int anm_gas_right[] =
	{
		SPR_GAS_RIGHT1,SPR_GAS_RIGHT2,SPR_GAS_RIGHT3,SPR_GAS_RIGHT4,
		ANM_RESTART
	};

//------------------------------------------------------------------------------
// Object Details
//------------------------------------------------------------------------------
static OBJSIZE gas_size = {6,8,22,24};
static OBJMOVE gas_move = {256,100,4*256,256,100,4*256,0};

//------------------------------------------------------------------------------
//! \brief Initialise the gas object
//------------------------------------------------------------------------------
CGasObj::CGasObj()
{
	Reset();
}

//------------------------------------------------------------------------------
//! \brief Destroy the gas object
//------------------------------------------------------------------------------
CGasObj::~CGasObj()
{
	if (m_pBaddie)	// Is still containing a baddie?
	{
		delete m_pBaddie;	// Then delete it
		m_pBaddie = 0;
	}
}

//------------------------------------------------------------------------------
//! \brief Draw the gas object
//------------------------------------------------------------------------------
void CGasObj::Draw( void )
{
	int toggle;
	int frame;
	if (m_Frame) m_pGame->m_Sprites.Draw( m_Frame, m_XPos, m_YPos );
	if (m_GasCmd==GAS_FLOAT)	// Draw the baddie inside the gas?
	{
		toggle = m_pGame->m_MainCounter&1;
		if (m_HoldTimer>=GAS_FLASHPOINT)	// Baddie about to escape?
		{
			if (toggle) return;		// Flash baddie
		}
		if (toggle) frame = m_pBaddie->m_pSuckFrames->hatch1;
		else frame = m_pBaddie->m_pSuckFrames->hatch2;

		m_pGame->m_Sprites.Draw( frame, m_XPos, m_YPos+HATCH_OFFSETY );
	}
}

//------------------------------------------------------------------------------
//! \brief Reset the object (object members)
//------------------------------------------------------------------------------
void CGasObj::Reset( void )
{
	m_pPlayer = 0;
	m_GasCmd = GAS_START;
	m_Timer = 0;
	m_pBaddie = 0;
	m_HoldTimer = 0;

	InitMovement(gas_move);
	InitSize(gas_size);
}

//------------------------------------------------------------------------------
//! \brief Setup the object object ejecting
//!
//! 	\param player = player to attach to gun
//------------------------------------------------------------------------------
void CGasObj::SetupEject( CPlayerObj *player )
{
	SetAnim(anm_gas_anim);
	m_pPlayer = player;

	m_Dir = m_pPlayer->m_Dir;	// Copy direction facing
	m_XPos = m_pPlayer->m_GunXPos;	// Copy xpos
	m_YPos = m_pPlayer->m_GunYPos;	// Copy ypos

	m_YPos += GAS_FROM_GUN_Y;
	if (HitWall())				// Gas hit a wall?
	{
		SetFade();
		return;
	}
}

//------------------------------------------------------------------------------
//! \brief Do the gas leaving the gun
//------------------------------------------------------------------------------
void CGasObj::DoLeaveGun( void )
{
	m_Dir = m_pPlayer->m_Dir;	// Copy direction facing
	AnimateDir(anm_gas_left, anm_gas_right, 256,ANMFLG_NORESTART);

	m_XPos = m_pPlayer->m_GunXPos;	// Copy xpos
	m_YPos = m_pPlayer->m_GunYPos;	// Copy ypos

	m_YPos += GAS_FROM_GUN_Y;
	if (m_Dir==DIR_LEFT)
	{
		m_XPos -= GAS_FROM_GUN_LX;
	}else
	{
		m_XPos += GAS_FROM_GUN_RX;
	}

	if (HitWall())				// Gas hit a wall?
	{
		SetFade();
		return;
	}
	if (CheckCollect()) return;

	if (m_State&STATE_ANM_RESTART)	// Anim finished?
	{
		SetAnim(anm_gas_anim);
		m_GasCmd = GAS_MOVE;
		return;
	}

}

//------------------------------------------------------------------------------
//! \brief Set the gas to fadeout
//------------------------------------------------------------------------------
void CGasObj::SetFade( void )
{
	m_Flags = 0;
	SetAnim(anm_gas_fade);
	m_GasCmd = GAS_FADE;
}

//------------------------------------------------------------------------------
//! \brief Check to see if the gas will collect a baddie
//!
//! If a baddie is collected then set up the m_GasCmd
//!
//! 	\return 0 = No baddie collected
//------------------------------------------------------------------------------
int CGasObj::CheckCollect( void )
{
	CLinkObject *pobj;
	CSuckable *psuck;

	pobj = m_pGame->m_BaddieList.m_pFirst;			// Start address

	while (pobj)			// For all objects
	{
		if (pobj->m_Flags&FLAG_CATCHABLE)	// Correct object type?
		{
			if (pobj->CheckHit( this ))
			{
				psuck = (CSuckable *) pobj;		// Cast to correct object
				m_GasCmd = GAS_FLOAT;
				SetAnim(anm_gas_anim);
				psuck->SetupCaught();	// Setup the getting caught
				m_pBaddie = psuck;		// Save the baddie pointer
				psuck->UnLink();
				m_Flags = FLAG_SUCKABLE;	// Can suck this object in
				return 1;
			}
			
		}
		pobj = pobj->m_pNext;
	}

	return 0;
}

//------------------------------------------------------------------------------
//! \brief Do the gas fading away
//------------------------------------------------------------------------------
void CGasObj::DoGasFade( void )
{
	Animate(256);
	if (m_State&STATE_ANM_RESTART)	// Anim finished?
	{
		DeleteObject();
		return;
	}
}

//------------------------------------------------------------------------------
//! \brief Check to see if the gas has hit the wall
//!
//! 	\return 0 = Did not hit the wall
//------------------------------------------------------------------------------
int CGasObj::HitWall( void )
{
	if (m_pGame->m_Map.CheckHitWall( m_XPos+m_X1+3, m_YPos+m_Y1,
		m_X2-6, m_Y2 - 8)) return 1;
	else return 0;
}

//------------------------------------------------------------------------------
//! \brief Do the gas moving away from the gun
//------------------------------------------------------------------------------
void CGasObj::DoGasMove( void )
{
	int step;
	Animate(256);

	step = m_pPlayer->m_GasStep;

	if (m_Dir==DIR_LEFT)
	{
		m_XPos-=step;
	}else
	{
		m_XPos+=step;

	}
	::CheckPos(m_XPos, m_YPos);

	if (CheckCollect()) return;

	m_Timer++;
	if (((m_pPlayer->m_GasLength)<m_Timer) || (HitWall()) )	// Time up? - or hit a wall?
	{
		SetFade();
		return;
	}

}
//------------------------------------------------------------------------------
//! \brief Release a baddie from the bubble
//------------------------------------------------------------------------------
void CGasObj::ReleaseBaddie( void )
{
	m_pBaddie->m_OldXPos = m_pBaddie->m_XPos = m_XPos;
	m_pBaddie->m_OldYPos = m_pBaddie->m_YPos = m_YPos-HATCH_OFFSETY;
	m_pBaddie->m_YInert = m_pBaddie->m_XInert = 0;
	m_pBaddie->m_State |= STATE_ANGRY;
	m_pBaddie->Link();
	m_pBaddie = 0;		// Clear baddie pointer
	SetFade();
}
//------------------------------------------------------------------------------
//! \brief Grab a the baddie from a gas cloud, and destroy the gas cloud
//!
//! The baddie will have the correct xpos,ypos and inertias
//!
//!	\param pptr = The player that sucked the baddie
//!
//! 	\return The baddie address
//------------------------------------------------------------------------------
CSuckable *CGasObj::GrabBaddie( CPlayerObj *pptr )
{
	CSuckable *pobj;
	m_pBaddie->m_OldXPos = m_pBaddie->m_XPos = m_XPos;
	m_pBaddie->m_OldYPos = m_pBaddie->m_YPos = m_YPos-HATCH_OFFSETY;
	m_pBaddie->m_YInert = m_pBaddie->m_XInert = 0;
	pobj = m_pBaddie;
	m_pBaddie = 0;		// Clear baddie pointer
	if ( pobj) pobj->m_pPlayer = pptr;
	SetFade();
	return pobj;
}

//------------------------------------------------------------------------------
//! \brief Do the gas floating around the screen
//------------------------------------------------------------------------------
void CGasObj::DoGasFloat( void )
{
	int wind;
	int offsetx;
	int offsety;
	int randval;
	CLinkObject *pobj;

	m_HoldTimer++;
	if (m_HoldTimer>=GAS_RELEASEPOINT)
	{
		ReleaseBaddie();
		return;
	}

	Animate(256);

	offsetx = ( m_XPos + 16 );
	offsety = ( m_YPos + 16 );
	::CheckPos(offsetx, offsety);		// Make sure on screen
	offsetx = offsetx >> 4;				// /16
	offsety = (offsety >> 4)*MAP_WIDTH;	// Get correct offset

	wind = m_pGame->m_Map.m_WindData[offsetx+offsety];	// Get the wind

	pobj = m_pGame->m_GasList.m_pFirst;			// Start address
	while (pobj)			// For all objects check for collision
	{
		if (this!=pobj)		// Don't collide with itself!
		{
			if (pobj->CheckHit( this ))
			{
				randval = rand();
				if (randval&12) wind = 1 + (randval&3);	// Random Wind if collision
				break;
			}
		}
		pobj = pobj->m_pNext;
	}

	switch (wind)
	{
		case (WIND_LEFT):
			SetMoveLeft();
			break;
		case (WIND_UP):
			SetMoveUp();
			break;
		case (WIND_RIGHT):
			SetMoveRight();
			break;
		case (WIND_DOWN):
			SetMoveDown();
			break;
	}
	DoXInertia();
	DoYInertia();
	::CheckPos(m_XPos, m_YPos);
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CGasObj::Do( void )
{
	if (!m_pPlayer) return;	// Should never happen

	switch (m_GasCmd)
	{
		case (GAS_START):
			DoLeaveGun();
			break;
		case (GAS_MOVE):
			DoGasMove();
			break;
		case (GAS_FADE):
			DoGasFade();
			break;
		case (GAS_FLOAT):
			DoGasFloat();
			break;
	}

}

//------------------------------------------------------------------------------
// The Cloud Object
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Animation
//------------------------------------------------------------------------------
static int anm_cloud[] =
	{
	SPR_GAS_CLOUD4,
	SPR_GAS_FADE1,
	SPR_GAS_FADE2,
	SPR_GAS_FADE3,
	SPR_GAS_FADE4,
	SPR_GAS_FADE5,
	ANM_RESTART
	};


//------------------------------------------------------------------------------
//! \brief Initialise the cloud
//------------------------------------------------------------------------------
CCloudObj::CCloudObj()
{
	m_Frame = SPR_GAS_CLOUD4;
	m_pSequence = anm_cloud;
}

//------------------------------------------------------------------------------
//! \brief Draw the cloud object
//------------------------------------------------------------------------------
void CCloudObj::Draw( void )
{
	if (m_Frame) m_pGame->m_Sprites.Draw( m_Frame, m_XPos, m_YPos, GFX_COL0 );
}


//------------------------------------------------------------------------------
//! \brief Setup the cloud
//!
//! 	\param xpos = Where to display the cloud X
//! 	\param ypos = Where to display the cloud Y
//------------------------------------------------------------------------------
void CCloudObj::Setup(int xpos, int ypos)
{
	m_XPos = xpos;
	m_YPos = ypos;
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CCloudObj::Do( void )
{
	Animate(256);
	if (m_State&STATE_ANM_RESTART)	// Anim finished
	{
		DeleteObject();
	}
}

//------------------------------------------------------------------------------
// The FireLR Object
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Animation
//------------------------------------------------------------------------------
static int anm_firel[] =
	{
	SPR_FIREL_1,
	SPR_FIREL_2,
	SPR_FIREL_3,
	SPR_FIREL_4,
	ANM_RESTART
	};

static int anm_firer[] =
	{
	SPR_FIRER_1,
	SPR_FIRER_2,
	SPR_FIRER_3,
	SPR_FIRER_4,
	ANM_RESTART
	};

static OBJSIZE firelr_size = { 0,0,26,13 };

#define FIRELR_RATE 8

//------------------------------------------------------------------------------
//! \brief Initialise the firelr
//------------------------------------------------------------------------------
CFireLRObj::CFireLRObj()
{
	m_Frame = SPR_FIREL_4;
	m_pSequence = anm_firel;
	m_Flags = FLAG_WEAPON;
	InitSize(firelr_size);
}

//------------------------------------------------------------------------------
//! \brief Setup the firelr
//!
//! 	\param xpos = Where to display the firelr X
//! 	\param ypos = Where to display the firelr Y
//!	\param dir = DIR_xxx direction
//------------------------------------------------------------------------------
void CFireLRObj::Setup(int xpos, int ypos, int dir)
{
	m_XPos = xpos;
	m_YPos = ypos;
	m_Dir = dir;
	if (dir==DIR_LEFT)
	{
		m_Frame = SPR_FIREL_4;
		m_pSequence = anm_firel;
	}else
	{
		m_Frame = SPR_FIRER_4;
		m_pSequence = anm_firer;
	}
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CFireLRObj::Do( void )
{
	int rcode;
	Animate(256);
	m_OldXPos = m_XPos;

	if (m_Dir==DIR_LEFT)
	{
		m_XPos-=FIRELR_RATE;
		rcode = CheckLeft();
	}else
	{
		m_XPos+=FIRELR_RATE;
		rcode = CheckRight();
	}
	if ((rcode!=CBM_NOTINWALL) || (m_State & STATE_HITBADDIE))
	{
		m_pGame->CreateCloud( m_XPos, m_YPos );
		DeleteObject();
	}
}

