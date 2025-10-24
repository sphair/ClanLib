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
// Methane Brothers Weapons Object (Source File)
//------------------------------------------------------------------------------

#include "stdio.h"
#include "stdlib.h"
#include "weapon.h"
#include "global.h"
#include "baddie.h"
#include "bitgroup.h"
#include "player.h"
#include "game.h"
#include "goodie.h"
#include "objtypes.h"

//------------------------------------------------------------------------------
// Object Details
//------------------------------------------------------------------------------
static OBJSIZE mbugweapon_size = {7,0,18,24};
static OBJMOVE mbugweapon_move = {240,240/2,11*256,11*256,265,11*256,11*256};

//------------------------------------------------------------------------------
//! \brief Initialise the weapon object
//------------------------------------------------------------------------------
CMBugWeapon::CMBugWeapon()
{
	Reset();
}

//------------------------------------------------------------------------------
//! \brief Destroy the weapon object
//------------------------------------------------------------------------------
CMBugWeapon::~CMBugWeapon()
{
	if (m_pBaddie)	// Attached to a baddie?
	{
		m_pBaddie->m_pWeapon = 0;	// Detach weapon
		m_pBaddie = 0;
	}
}

//------------------------------------------------------------------------------
//! \brief Draw the weapon object
//------------------------------------------------------------------------------
void CMBugWeapon::Draw( void )
{
	if (m_Frame) m_pGame->m_Sprites.Draw( m_Frame, m_XPos, m_YPos );
}

//------------------------------------------------------------------------------
//! \brief Reset the object (object members)
//------------------------------------------------------------------------------
void CMBugWeapon::Reset( void )
{
	m_pBaddie = 0;
	m_Frame = SPR_MBUG_BOMB1;

	InitMovement(mbugweapon_move);
	InitSize(mbugweapon_size);
	m_Exploding = 0;
}

//------------------------------------------------------------------------------
//! \brief Control the weapon animation (AN AMIGA DIRECT PORT)
//------------------------------------------------------------------------------
void CMBugWeapon::ControlAnim( void )
{
	int flag;
	int p1xpos = 2000;
	int p2xpos = 2000;
	int xpos ;
	CPlayerObj *play1;
	CPlayerObj *play2;

	play1 = m_pGame->GetPlayer(OBJ_PLAYER_ONE);
	play2 = m_pGame->GetPlayer(OBJ_PLAYER_TWO);

	if (play1) p1xpos = play1->m_XPos + 16;
	if (play2) p2xpos = play2->m_XPos + 16;
	xpos = m_XPos + 16;

	p1xpos = p1xpos & (~0x1f);
	p2xpos = p2xpos & (~0x1f);

	xpos = xpos & (~0x1f);

	flag = 0;
	if (play1)
	{
		if (p1xpos == xpos) flag = 1;
	}

	if (play2)
	{
		if (p2xpos == xpos) flag = 1;
	}

	if ( flag )					// is player underneath?
	{				// --- Close chute
		if (m_Frame == SPR_MBUG_BOMB2)
		{
			m_Frame = SPR_MBUG_BOMB1;
		}else
		if (m_Frame == SPR_MBUG_BOMB3)
		{
			m_Frame = SPR_MBUG_BOMB2;
		}else
		if (m_Frame == SPR_MBUG_BOMB4)
		{
			m_Frame = SPR_MBUG_BOMB3;
		}

	}else
	{				// --- Open chute
		if (m_Frame == SPR_MBUG_BOMB3)
		{
			m_Frame = SPR_MBUG_BOMB4;
		}else
		if (m_Frame == SPR_MBUG_BOMB2)
		{
			m_Frame = SPR_MBUG_BOMB3;
		}else
		if (m_Frame == SPR_MBUG_BOMB1)
		{
			m_Frame = SPR_MBUG_BOMB2;
		}
		if (m_YInert > 256*2)
		{
			m_YInert = 256*2;		// Open maximum fall rate
		}
	}
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CMBugWeapon::Do( void )
{
	int flag;
	if (m_Exploding)
	{
		DeleteObject();
		return;
	}
	if (CheckKillPlayer())
	{
		flag = 1;
	}else flag = 0;

	MoveObject_Y();

	if ( (m_State&STATE_DOWN) || (flag) )		// Hit the floor? - Or hit player
	{
		m_Exploding = 1;
		m_YPos -=4;				// Up a bit
		m_Frame = SPR_MBUG_BANG;
	}else
	{
		ControlAnim();

	}
}

//------------------------------------------------------------------------------
// Animation
//------------------------------------------------------------------------------

static int anm_clownwpn_left[] =
	{
		SPR_CLOWN_BAT8,	SPR_CLOWN_BAT7,	SPR_CLOWN_BAT6,	SPR_CLOWN_BAT5,
		SPR_CLOWN_BAT4,	SPR_CLOWN_BAT3,	SPR_CLOWN_BAT2,	SPR_CLOWN_BAT1,
		ANM_RESTART
	};
static int anm_clownwpn_right[] =
	{
		SPR_CLOWN_BAT1,	SPR_CLOWN_BAT2,	SPR_CLOWN_BAT3,	SPR_CLOWN_BAT4,
		SPR_CLOWN_BAT5,	SPR_CLOWN_BAT6,	SPR_CLOWN_BAT7,	SPR_CLOWN_BAT8,
		ANM_RESTART
	};

//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------
#define CLOWN_WEAPON_SPEED	10
//------------------------------------------------------------------------------
// Object Details
//------------------------------------------------------------------------------
static OBJSIZE clownweapon_size[8] =
	{
		{8,6,16,8},
		{15,3,17,15},
		{19,10,10,14},
		{15,16,17,15},
		{9,20,16,8},
		{2,16,16,15},
		{5,11,10,14},
		{2,3,16,14}
	};
static OBJSIZE clownweapon_movesize =
		{8,8,16,16};

//------------------------------------------------------------------------------
//! \brief Initialise the weapon object
//------------------------------------------------------------------------------
CClownWeapon::CClownWeapon()
{
	Reset();
}

//------------------------------------------------------------------------------
//! \brief Destroy the weapon object
//------------------------------------------------------------------------------
CClownWeapon::~CClownWeapon()
{
	if (m_pBaddie)	// Attached to a baddie?
	{
		m_pBaddie->m_pWeapon = 0;	// Detach weapon
		m_pBaddie = 0;
	}
}

//------------------------------------------------------------------------------
//! \brief Draw the weapon object
//------------------------------------------------------------------------------
void CClownWeapon::Draw( void )
{
	if (m_Frame) m_pGame->m_Sprites.Draw( m_Frame, m_XPos, m_YPos );
}

//------------------------------------------------------------------------------
//! \brief Reset the object (object members)
//------------------------------------------------------------------------------
void CClownWeapon::Reset( void )
{
	m_pBaddie = 0;
	m_Frame = SPR_CLOWN_BAT8;
	InitSize(clownweapon_size[0]);
}

//------------------------------------------------------------------------------
//! \brief Match the weapon size to the weapons graphic frame
//------------------------------------------------------------------------------
void CClownWeapon::MatchSizeToFrame( void )
{
	int frmno;

	frmno = m_Frame - SPR_CLOWN_BAT1;
	if ( (frmno<0) || (frmno>7) ) frmno = 8;	// Validate the frame
	InitSize(clownweapon_size[frmno]);
}

//------------------------------------------------------------------------------
//! \brief Move the weapon
//------------------------------------------------------------------------------
void CClownWeapon::MoveWeapon( void )
{
	m_OldXPos = m_XPos;
	if (m_Dir==DIR_LEFT)
	{
		m_XPos -=CLOWN_WEAPON_SPEED;
		m_XInert = -1;			// For the below DoXCheck function
	}else		// Moving Right
	{
		m_XPos +=CLOWN_WEAPON_SPEED;
		m_XInert = 1;
	}
	DoXCheck();
	if ( (m_State&STATE_HORIZ) || (m_XPos<0) || (m_XPos >=(SCR_WIDTH-16)) )
	{			// Wall Hit? Or Screen Boundary hit?
		DeleteObject();
	}
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CClownWeapon::Do( void )
{
	if (CheckKillPlayer())
	{
		DeleteObject();
		return;
	}

	AnimateDir(anm_clownwpn_left, anm_clownwpn_right, 256);
	InitSize(clownweapon_movesize);
	MoveWeapon();
	MatchSizeToFrame();
}

//------------------------------------------------------------------------------
// Animation
//------------------------------------------------------------------------------

static int anm_dwarfwpn_left[] =
	{
		SPR_DWARF_HAMMER8,	SPR_DWARF_HAMMER7,	SPR_DWARF_HAMMER6,	SPR_DWARF_HAMMER5,
		SPR_DWARF_HAMMER4,	SPR_DWARF_HAMMER3,	SPR_DWARF_HAMMER2,	SPR_DWARF_HAMMER1,
		ANM_RESTART
	};
static int anm_dwarfwpn_right[] =
	{
		SPR_DWARF_HAMMER1,	SPR_DWARF_HAMMER2,	SPR_DWARF_HAMMER3,	SPR_DWARF_HAMMER4,
		SPR_DWARF_HAMMER5,	SPR_DWARF_HAMMER6,	SPR_DWARF_HAMMER7,	SPR_DWARF_HAMMER8,
		ANM_RESTART
	};

//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------
#define DWARF_WEAPON_SPEED	10

//------------------------------------------------------------------------------
// Object Details
//------------------------------------------------------------------------------
static OBJSIZE dwarfweapon_size[8] =
	{
		{8,6,16,8},
		{15,3,17,15},
		{19,10,10,14},
		{15,16,17,15},
		{9,20,16,8},
		{2,16,16,15},
		{5,11,10,14},
		{2,3,16,14}
	};
static OBJSIZE dwarfweapon_movesize =
		{8,8,16,16};

//------------------------------------------------------------------------------
//! \brief Initialise the weapon object
//------------------------------------------------------------------------------
CDwarfWeapon::CDwarfWeapon()
{
	Reset();
}

//------------------------------------------------------------------------------
//! \brief Destroy the weapon object
//------------------------------------------------------------------------------
CDwarfWeapon::~CDwarfWeapon()
{
	if (m_pBaddie)	// Attached to a baddie?
	{
		m_pBaddie->m_pWeapon = 0;	// Detach weapon
		m_pBaddie = 0;
	}
}

//------------------------------------------------------------------------------
//! \brief Draw the weapon object
//------------------------------------------------------------------------------
void CDwarfWeapon::Draw( void )
{
	if (m_Frame) m_pGame->m_Sprites.Draw( m_Frame, m_XPos, m_YPos );
}

//------------------------------------------------------------------------------
//! \brief Reset the object (object members)
//------------------------------------------------------------------------------
void CDwarfWeapon::Reset( void )
{
	m_pBaddie = 0;
	InitSize(dwarfweapon_size[0]);
	m_Command = DWEAPON_STOP;
}

//------------------------------------------------------------------------------
//! \brief Match the weapon size to the weapons graphic frame
//------------------------------------------------------------------------------
void CDwarfWeapon::MatchSizeToFrame( void )
{
	int frmno;

	frmno = m_Frame - SPR_DWARF_HAMMER1;
	if ( (frmno<0) || (frmno>7) ) frmno = 8;	// Validate the frame
	InitSize(dwarfweapon_size[frmno]);
}

//------------------------------------------------------------------------------
//! \brief Setup the weapon for throwing
//!
//! 	\param xpos = weapon position X
//! 	\param ypos = weapon position Y
//!	\param dir = weapon direction (IE DIR_LEFT)
//------------------------------------------------------------------------------
void CDwarfWeapon::InitThrow( int xpos, int ypos, int dir )
{
	m_Command = DWEAPON_GO;

	m_OldXPos = m_XPos = xpos;
	m_OldYPos = m_YPos = ypos;
	m_Dir = dir;
	m_OutFlag = 0;
	m_Seq_Offset = 0;
	m_Ticks = 0;
}

//------------------------------------------------------------------------------
//! \brief Drop the weapon when in the air (ie when the baddie is gassed)
//------------------------------------------------------------------------------
void CDwarfWeapon::DropWeapon( void )
{
	m_Command = DWEAPON_STOP;
	m_Frame = 0;
}

//------------------------------------------------------------------------------
//! \brief Move the weapon
//------------------------------------------------------------------------------
void CDwarfWeapon::MoveWeapon( void )
{
	m_OldXPos = m_XPos;
	if (m_Dir==DIR_LEFT)
	{
		m_XPos -=DWARF_WEAPON_SPEED;
		m_XInert = -1;			// For the below DoXCheck function
	}else		// Moving Right
	{
		m_XPos +=DWARF_WEAPON_SPEED;
		m_XInert = 1;
	}

	if (!m_OutFlag)		// Weapon Moving Out?
	{
		DoXCheck();
		m_Ticks++;
		if ( (m_State&STATE_HORIZ) || (m_XPos<0) || (m_XPos >=(SCR_WIDTH-16)) )
		{			// Wall Hit? Or Screen Boundary hit?
			m_OutFlag = 1;	// Move back flag
			if (m_Dir==DIR_LEFT)
			{
				m_Dir=DIR_RIGHT;
			}else
			{
				m_Dir=DIR_LEFT;
			}
		}
	}else				// Weapon Moving In?
	{
		m_Ticks--;
		if (m_Ticks<=0)
		{
			m_Command = DWEAPON_STOP;
			m_Frame = 0;
		}
	}
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CDwarfWeapon::Do( void )
{
	if (m_Command == DWEAPON_GO)
	{
		if (CheckKillPlayer())
		{
			if (!m_OutFlag)	// Still going out?
			{
				m_OutFlag = 1;
				if (m_Dir==DIR_LEFT)
				{
					m_Dir=DIR_RIGHT;
				}else
				{
					m_Dir=DIR_LEFT;
				}
			}
		}

		AnimateDir(anm_dwarfwpn_left, anm_dwarfwpn_right, 256);
		InitSize(dwarfweapon_movesize);
		MoveWeapon();
		MatchSizeToFrame();
	}

}

//------------------------------------------------------------------------------
// The Generator Object
//------------------------------------------------------------------------------

static int anm_generator[] =
	{
	SPR_GEN_1,
	SPR_GEN_2,
	SPR_GEN_3,
	SPR_GEN_4,
	ANM_RESTART
	};

#define GBADID_BUG		0
#define GBADID_ZOOM	1
#define GBADID_SPIKE	2
#define GBADID_SUCKER	3

#define GBADID_TOTAL	4

static int gen_id[GBADID_TOTAL] = {OBJ_BUG, OBJ_ZOOM, OBJ_SPIKE, OBJ_SUCKER };

static SUCKANIM *gen_suck_left[GBADID_TOTAL] = {
	suck_bug_left,
	suck_zoom_left,
	suck_spike_left,
	suck_sucker_left
	};

static SUCKANIM *gen_suck_right[GBADID_TOTAL] = {
	suck_bug_right,
	suck_zoom_right,
	suck_spike_right,
	suck_sucker_right
	};

static OBJSIZE generator_size = { 2, 10, 28, 39 };
static OBJMOVE generator_move = {160,160/2,4*256,11*256,285,11*256,11*256};

#define GEN_DELAY	150	// Delay between ejections

#define GEN_NUMX	8
#define GEN_NUMY	14
static int gen_xtab[GEN_NUMX] = {0,1,2,3,4,3,2,1};
static int gen_ytab[GEN_NUMY] = {4,3,2,1,0,1,2,3,2,1,0,1,2,3};

//------------------------------------------------------------------------------
//! \brief Initialise the generator
//------------------------------------------------------------------------------
CGeneratorObj::CGeneratorObj()
{
	m_Frame = SPR_GEN_1;
	m_pSequence = anm_generator;
	InitSize(generator_size);
	InitMovement(generator_move);
	m_Flags = FLAG_BADDIE;
	m_WhiteFlashCnt = 0;
	m_HitCnt = 3;	// Number of hits needed to kill this object
	m_ReleaseDelay = ( ::rand() & 0x3f ) + 60;
	m_EjectDir = 0;
	m_EjectXOff = 0;
	m_EjectYOff = 0;
	m_EjectCnt = 0;
	m_EjectFrame = 0;
	m_CheckFlag = 0;
	m_BaddieID = GBADID_BUG;
	m_NumFrame = 0;
	m_NumXPos = 0;
	m_NumYPos = 0;
	m_NumXOff = 0;
	m_NumYOff = 0;
}

//------------------------------------------------------------------------------
//! \brief Draw the generator object
//------------------------------------------------------------------------------
void CGeneratorObj::Draw( void )
{
	int sprcolour;
	if (m_WhiteFlashCnt) sprcolour = GFX_WHITE; else sprcolour = 0;
	if (m_EjectFrame)
	{
		m_pGame->m_Sprites.Draw( m_EjectFrame, m_XPos+m_EjectXOff,
			m_YPos+m_EjectYOff, sprcolour );

	}
	if (m_Frame) m_pGame->m_Sprites.Draw( m_Frame, m_XPos, m_YPos, sprcolour );
	if (m_NumFrame) m_pGame->m_Sprites.Draw( m_NumFrame, m_NumXPos, m_NumYPos, sprcolour );
}

//------------------------------------------------------------------------------
//! \brief Setup the generator
//------------------------------------------------------------------------------
void CGeneratorObj::Setup( void )
{
}

//------------------------------------------------------------------------------
//! \brief Load the object graphics
//------------------------------------------------------------------------------
void CGeneratorObj::LoadGfx( void )
{
	m_pGame->m_Sprites.LoadRange(SPR_GEN_1, SPR_GEN_4);
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CGeneratorObj::Do( void )
{
	FindTheBaddieID();
	if (m_WhiteFlashCnt) m_WhiteFlashCnt--;
	if (CheckHitByBaddie()) return;
	CheckKillPlayer();
	MoveObject();
	CheckReleaseBaddie();
	Animate(128);
}

//------------------------------------------------------------------------------
//! \brief Check to see if the object has a baddie thrown at it
//!
//! 	\return 0 = Ok. Else This object was deleted
//------------------------------------------------------------------------------
int CGeneratorObj::CheckHitByBaddie( void )
{
	CSuckable *pobj;
	int thetype;

	pobj = (CSuckable *) m_pGame->m_BaddieList.m_pFirst;
	for (;pobj;pobj = (CSuckable *) pobj->m_pNext)
	{
		thetype = pobj->m_Type;
		if ( (thetype==OBJ_BUG) || (thetype==OBJ_SPIKE) ||
			(thetype==OBJ_ZOOM) || (thetype==OBJ_SUCKER) )
		{
			if (pobj->m_SuckCmd==SUCK_ROLL)	// Must be rolling
			{
				if (CheckHit(pobj))			// Collision Occured
				{
					pobj->SetExplode();
					m_WhiteFlashCnt = 3;
					if (m_HitCnt)
					{
						m_HitCnt--;
					}else
					{
						ExplodeToys();
						DeleteObject();
						return 1;
					}
				}
			}
		}
	}
	return 0;
}

//------------------------------------------------------------------------------
//! \brief Check able to release a baddie
//------------------------------------------------------------------------------
void CGeneratorObj::CheckReleaseBaddie(void)
{
	int numbaddies;
	SUCKANIM *sanim;

	m_NumFrame = 0;
	numbaddies = m_pGame->CountBaddies();
	if (numbaddies >= 6)	// Limit the number of baddies on the screen
	{
		m_ReleaseDelay = GEN_DELAY;
	}else
	{
		m_ReleaseDelay--;
		if (m_ReleaseDelay<0)
		{
			m_ReleaseDelay = GEN_DELAY;
			m_EjectDir = m_EjectDir^1;	// Swap the direction
			m_EjectCnt = NUMSUCKANIM/2;
		}else
		{
			ControlTimer();
		}
	}

	if (m_EjectCnt)	// Ejecting baddie?
	{
		if (m_EjectCnt>=NUMSUCKANIM)	// All done?
		{
			CreateBaddie();
			m_EjectCnt = 0;	// Finished Ejecting
			m_EjectFrame = 0;
			return;
		}

		if (m_EjectDir) sanim = gen_suck_right[m_BaddieID];
			else sanim = gen_suck_left[m_BaddieID];
		sanim = &sanim[m_EjectCnt];
		m_EjectFrame = sanim->frame;
		m_EjectXOff = sanim->xoffset;
		if (m_EjectDir) m_EjectXOff+=24;
			else m_EjectXOff-=28;
		m_EjectYOff = sanim->yoffset + 14;
		m_EjectCnt++;

	}
}

//------------------------------------------------------------------------------
//! \brief Create the baddie to eject
//------------------------------------------------------------------------------
void CGeneratorObj::CreateBaddie(void)
{
	CSuckable *Suckable;

	switch (m_BaddieID)
	{
		case GBADID_BUG:
			SMB_NEW(Suckable,CBugObj);
			break;
		case GBADID_ZOOM:
			SMB_NEW(Suckable,CZoomObj);
			break;
		case GBADID_SPIKE:
			SMB_NEW(Suckable,CSpikeObj);
			break;
		default:
			SMB_NEW(Suckable,CSuckerObj);
	}
	if (Suckable)
	{
		Suckable->m_Dir = m_EjectDir;
		Suckable->m_OldXPos = Suckable->m_XPos = m_EjectXOff+m_XPos;
		Suckable->m_OldYPos = Suckable->m_YPos = m_EjectYOff+m_YPos;
		Suckable->m_YInert = -8*256;

		m_pGame->m_pSound->PlaySample(SND_SPIT, m_XPos);
		m_pGame->m_BaddieList.Attach(Suckable, gen_id[m_BaddieID], m_pGame);
	}
}

//------------------------------------------------------------------------------
//! \brief Find the baddie id - to be ejected from the generator
//------------------------------------------------------------------------------
void CGeneratorObj::FindTheBaddieID(void)
{
	int cnt;
	int id;
	if (m_CheckFlag) return;	// Already found
	m_CheckFlag = 1;

	for (cnt=0; cnt<GBADID_TOTAL; cnt++)
	{
		id = gen_id[cnt];
		if (m_pGame->m_BaddieList.FindFirst(id))
		{
			m_BaddieID = cnt;
		}
	}
}

//------------------------------------------------------------------------------
//! \brief Explode in loads of toys
//------------------------------------------------------------------------------
void CGeneratorObj::ExplodeToys( void )
{
	int cnt;

	for (cnt=0; cnt<12; cnt++)
		m_pGame->RandGoodie(m_XPos+16, m_YPos+16, GOODIE_TOYS, cnt&1);

}

//------------------------------------------------------------------------------
//! \brief Control the number above the object
//------------------------------------------------------------------------------
void CGeneratorObj::ControlTimer( void )
{
	int togo;

	togo = (10 * m_ReleaseDelay) / GEN_DELAY;	// 0 to 10
	if (togo<0) togo = 0;
	if (togo>9) togo = 9;

	m_NumFrame = SPR_FNT_Y0 + togo;
	m_NumXPos = m_XPos+gen_xtab[m_NumXOff]+8;
	m_NumYPos = m_YPos+gen_ytab[m_NumYOff]-12;

	m_NumXOff++;
	if (m_NumXOff>=GEN_NUMX) m_NumXOff=0;
	m_NumYOff++;
	if (m_NumYOff>=GEN_NUMY) m_NumYOff=0;

}

//------------------------------------------------------------------------------
// The BombObj Object
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Animation Frames
//------------------------------------------------------------------------------

static int anm_bomb[] =
	{
		SPR_LBOSS_B1,SPR_LBOSS_B2,SPR_LBOSS_B3,SPR_LBOSS_B4,SPR_LBOSS_B5,SPR_LBOSS_B6,SPR_LBOSS_B7,
		SPR_LBOSS_B8,SPR_LBOSS_B9,SPR_LBOSS_B10,
		SPR_LBOSS_BOOM,SPR_LBOSS_BOOM,
		ANM_RESTART
	};

static OBJSIZE bomb_size = {0,0,16,18};
static OBJMOVE bomb_move = {130,0x32,5*256,11*256,0x80,11*256,11*256};

//------------------------------------------------------------------------------
//! \brief Initialise the BombObj
//------------------------------------------------------------------------------
CBombObj::CBombObj()
{
	m_Frame = SPR_LBOSS_B1;
	m_pSequence = anm_bomb;
	InitMovement(bomb_move);
	InitSize(bomb_size);
	m_YInert = -3*256;
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CBombObj::Do( void )
{
	MoveObject();

	if (m_Frame==SPR_LBOSS_BOOM)
	{
		CheckKillPlayer();
	}

	Animate(128);
	if (m_State&STATE_ANM_RESTART)	// Anim finished
	{
		DeleteObject();
	}
}

