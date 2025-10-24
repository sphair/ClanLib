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
// Methane Brothers Boss Baddies (Source File)
//------------------------------------------------------------------------------

#include "boss.h"
#include "global.h"
#include "bitgroup.h"
#include "objtypes.h"
#include "goodie.h"
#include "game.h"
#include "misc.h"
#include "maps.h"
#include "weapon.h"
#include "baddie.h"

//------------------------------------------------------------------------------
// ClownBoss
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Object Details
//------------------------------------------------------------------------------
static OBJSIZE clownboss_size = {0,0,96,0x66};
static OBJMOVE clownboss_move = {280,280/2,5*256,230,0,4*256,0};

static OBJSIZE	cb_col_head = {0x23,0x01,(0x3D)-(0x23),(0x1D)-(0x01)};
static OBJSIZE	cb_col_top = {0x01,0x28,(0x5D)-(0x01),(0x40)-(0x28)};
static OBJSIZE	cb_col_middle = {0x22,0x19,(0x3C)-(0x22),(0x65)-(0x19)};

static OBJSIZE	cb_col_bottom = {0x05,0x5A,(0x5C)-(0x05),(0x65)-(0x5A)};

//------------------------------------------------------------------------------
// ClownBoss Commands
//------------------------------------------------------------------------------
#define CLOWNBOSS_NORMAL	0
#define CLOWNBOSS_OUCH		1

//------------------------------------------------------------------------------
//! \brief Initialise the number rising object
//------------------------------------------------------------------------------
CClownBoss::CClownBoss()
{
	m_Frame = SPR_BUG_LEFT1;
	m_Flags = FLAG_BADDIE | FLAG_HARDWALL;
	m_MouthCounter = 0;
	m_WhiteFlag = 0;

	InitMovement(clownboss_move);
	InitSize(clownboss_size);
	m_Y_Flag = -1;
	m_Command = CLOWNBOSS_NORMAL;
	m_DamageCnt = 0;
}

//------------------------------------------------------------------------------
//! \brief Draw the number rising object
//------------------------------------------------------------------------------
void CClownBoss::Draw( void )
{
	int xframe;
	int sprcolour;
	if (m_WhiteFlag)
	{
		sprcolour = GFX_WHITE;
		xframe = SPR_CB_DAY2;
	}else
	{
		sprcolour = 0;
		xframe = SPR_CB_DAY1;
	}
	m_pGame->m_Sprites.Draw( xframe, m_XPos+33, m_YPos );

	m_pGame->m_Sprites.Draw( SPR_CB_TOPPART, m_XPos+32, m_YPos, sprcolour );
	m_pGame->m_Sprites.Draw( SPR_CB_LEFTPART, m_XPos, m_YPos+16, sprcolour );
	m_pGame->m_Sprites.Draw( SPR_CB_RIGHTPART, m_XPos+64, m_YPos+16, sprcolour );

	if (m_MouthCounter&16)
	{
		m_pGame->m_Sprites.Draw( SPR_CB_MOUTH2, m_XPos+32, m_YPos+64, sprcolour );
	}else
	{
		m_pGame->m_Sprites.Draw( SPR_CB_MOUTH1, m_XPos+32, m_YPos+64, sprcolour );
	}
}

//------------------------------------------------------------------------------
//! \brief Do the collision detection between player and object
//------------------------------------------------------------------------------
void CClownBoss::CollisionCheck( void )
{
	InitSize(cb_col_head);
	CheckKillPlayer();
	InitSize(cb_col_top);
	CheckKillPlayer();
	InitSize(cb_col_middle);
	CheckKillPlayer();
	InitSize(cb_col_bottom);
	CheckKillPlayer();

}

//------------------------------------------------------------------------------
//! \brief Do the collision detection between tomatos and this object
//------------------------------------------------------------------------------
void CClownBoss::TomatoCollision( void )
{
	CTomatoObj *pobj;

	pobj = (CTomatoObj *) m_pGame->m_GasList.m_pFirst;
	for (;pobj;pobj = (CTomatoObj *) pobj->m_pNext)
	{
		if (pobj->m_Type==OBJ_TOMATO)	// Must be a tomato (else illegal cast)
		{
			if (pobj->m_SuckCmd==SUCK_ROLL)
			{
				if (HitHead(pobj,&cb_col_head)) continue;
				if (HitBody(pobj,&cb_col_top)) continue;
				if (HitBody(pobj,&cb_col_middle)) continue;
				if (HitBody(pobj,&cb_col_bottom)) continue;
			}
		}
	}
}

//------------------------------------------------------------------------------
//! \brief Check to see if a tomato hit this objects head
//!
//! 	\param tobj = Tomato Object
//!	\param pcb = The collision box
//!
//! 	\return 0 = No collision occured
//------------------------------------------------------------------------------
int CClownBoss::HitHead( CTomatoObj *tobj, OBJSIZE *pcb )
{
	int rcode;
	int cnt;

	rcode = HitBody( tobj, pcb );

	if (rcode)
	{
		m_Command = CLOWNBOSS_OUCH;
		m_OuchCount = 8;
		m_DamageCnt++;

		for (cnt=0; cnt<8; cnt++)
		{
			m_pGame->RandGoodie( m_XPos+16, m_YPos + 8, GOODIE_COMP, cnt&1 );
		}

		if (m_DamageCnt>=5)
		{
			if (m_Type!=OBJ_DEAD)	// This is possible! (Very unlikely though)
			{
				m_pGame->SetBossDie( m_XPos + 16, m_YPos );
				DeleteObject();
			}
		}
	}

	return rcode;
}

//------------------------------------------------------------------------------
//! \brief Check to see if a tomato hit this objects body
//!
//! 	\param tobj = Tomato Object
//!	\param pcb = The collision box
//!
//! 	\return 0 = No collision occured
//------------------------------------------------------------------------------
int CClownBoss::HitBody( CTomatoObj *tobj, OBJSIZE *pcb )
{
	InitSize(*pcb);
	if (CheckHit( tobj ))
	{
		tobj->SetExplode();
		return 1;
	}
	return 0;
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CClownBoss::Do( void )
{
	switch (m_Command)
	{
		case CLOWNBOSS_NORMAL:
			m_MouthCounter++;

			CollisionCheck();
			TomatoCollision();

			InitSize(clownboss_size);
			DirFly();
			break;
		case CLOWNBOSS_OUCH:
			m_WhiteFlag = 1;
			m_OuchCount--;
			if (m_OuchCount<0)
			{
				m_Command = CLOWNBOSS_NORMAL;
				m_WhiteFlag = 0;
			}
			break;
	}
}

//------------------------------------------------------------------------------
//! \brief Load the object graphics
//------------------------------------------------------------------------------
void CClownBoss::LoadGfx( void )
{
	m_pGame->m_Sprites.LoadRange(SPR_CB_LEFTPART, SPR_CB_DAY3);
	m_pGame->m_Sprites.LoadRange(SPR_COMP_01, SPR_COMP_20);
}

//------------------------------------------------------------------------------
// CrabBoss
//------------------------------------------------------------------------------

#define CRAB_SPEED	3

//------------------------------------------------------------------------------
// Object Details
//------------------------------------------------------------------------------
static OBJSIZE crabboss_size = {0,0,0x58,0x56};
static OBJMOVE crabboss_move = {130,130/2,5*256,0x400,0x100,4*256,0};

static OBJSIZE	crab_col_head = {0x23,0x00,(0x3C)-(0x23),(0x1A)-(0x00)};
static OBJSIZE	crab_col_middle = {0x24,0x1C,(0x3B)-(0x24),(0x65)-(0x1C)};
static OBJSIZE	crab_col_bottom = {0x01,0x32+6,(0x5E)-(0x01),(0x52)-(0x32)-20};

//------------------------------------------------------------------------------
// CrabBoss Movement Pattern
//------------------------------------------------------------------------------

#define PAT_DOWN	1
#define PAT_UP		2
#define PAT_LEFT	3
#define PAT_RIGHT	4
#define PAT_DUMP	5
#define PAT_LOOP	6

typedef struct _CRABPAT
	{
		int cmd;
		int value;
	} CRABPAT;

CRABPAT crab_pattern[]=
	{
		{PAT_UP,0x12},
		{PAT_DOWN,0x48},
		{PAT_RIGHT,0x6E},
		{PAT_DOWN,0x8A},
		{PAT_DUMP,0},
		{PAT_UP,0x48},
		{PAT_RIGHT,0xC6},
		{PAT_UP,0x12},
		{PAT_DOWN,0x48},
		{PAT_LEFT,0x6E},
		{PAT_DOWN,0x8A},
		{PAT_DUMP,0},
		{PAT_UP,0x48},
		{PAT_LEFT,0x16},
		{PAT_LOOP,0}
	};

//------------------------------------------------------------------------------
// CrabBoss Commands
//------------------------------------------------------------------------------
#define CRABBOSS_NORMAL	0
#define CRABBOSS_OUCH		1

//------------------------------------------------------------------------------
// Crabboss pipe commands
//------------------------------------------------------------------------------
#define PIPE_START		0	// Initial pipe
#define PIPE_BOSSUP		1	// Move boss up
#define PIPE_BADDOWNPIPE	2	// Move baddie down the pipe
#define PIPE_LETOUT		3	// Let the baddie out of the pipe
#define PIPE_UP			4	// Move pipe back up

//------------------------------------------------------------------------------
//! \brief Initialise the number rising object
//------------------------------------------------------------------------------
CCrabBoss::CCrabBoss()
{
	m_Frame = SPR_BUG_LEFT1;
	m_Flags = FLAG_BADDIE | FLAG_HARDWALL;
	m_ClawCounter = 0;
	m_WhiteFlag = 0;
	m_Pattern = 0;

	InitMovement(crabboss_move);
	InitSize(crabboss_size);
	m_Y_Flag = -1;
	m_Command = CRABBOSS_NORMAL;
	m_DamageCnt = 0;
	m_PipeOffset = 0;
	m_LumpOffset = 0;
	m_PipeCmd = PIPE_START;
	m_EjectFrame = 0;
	m_EjectDir = 1;
}

//------------------------------------------------------------------------------
//! \brief Draw the number rising object
//------------------------------------------------------------------------------
void CCrabBoss::Draw( void )
{
	int xframe;
	int sprcolour;
	if (m_WhiteFlag)
	{
		sprcolour = GFX_WHITE;
		xframe = SPR_CB_DAY2;
	}else
	{
		sprcolour = 0;
		xframe = SPR_CB_DAY1;
	}

	if (m_EjectFrame)
	{
		m_pGame->m_Sprites.Draw( m_EjectFrame, m_XPos+m_EjectXOff,
			m_YPos+m_EjectYOff, sprcolour );

	}

	m_pGame->m_Sprites.Draw( xframe, m_XPos+33, m_YPos );

	m_pGame->m_Sprites.Draw( SPR_CRAB_PIPE, m_XPos+0x20, m_YPos+0x38+m_PipeOffset, sprcolour );

	if (m_LumpOffset)
	{
		m_pGame->m_Sprites.Draw( SPR_CRAB_SEG, m_XPos+0x20, m_YPos+(0x48)+m_LumpOffset, sprcolour );
	}

	m_pGame->m_Sprites.Draw( SPR_CRAB_MPART, m_XPos+32, m_YPos, sprcolour );

	if (m_ClawCounter&16)
	{
		m_pGame->m_Sprites.Draw( SPR_CRAB_LCLAW1, m_XPos, m_YPos+32, sprcolour );
		m_pGame->m_Sprites.Draw( SPR_CRAB_RCLAW2, m_XPos+64, m_YPos+32, sprcolour );
	}else
	{
		m_pGame->m_Sprites.Draw( SPR_CRAB_LCLAW2, m_XPos, m_YPos+32, sprcolour );
		m_pGame->m_Sprites.Draw( SPR_CRAB_RCLAW1, m_XPos+64, m_YPos+32, sprcolour );
	}
}

//------------------------------------------------------------------------------
//! \brief Do the collision detection between player and object
//------------------------------------------------------------------------------
void CCrabBoss::CollisionCheck( void )
{
	InitSize(crab_col_head);
	CheckKillPlayer();
	InitSize(crab_col_middle);
	CheckKillPlayer();
	InitSize(crab_col_bottom);
	CheckKillPlayer();

}

//------------------------------------------------------------------------------
//! \brief Do the crabboss pattern
//------------------------------------------------------------------------------
void CCrabBoss::DoPattern( void )
{
	int cmd;
	int value;

	cmd = crab_pattern[m_Pattern].cmd;
	if (cmd==PAT_LOOP)		// Pattern loop?
	{
		m_Pattern = 0;
		cmd = crab_pattern[m_Pattern].cmd;
	}
	value = crab_pattern[m_Pattern].value;
	switch (cmd)
	{
		case PAT_UP:
			PatUp( value );
			break;

		case PAT_DOWN:
			PatDown( value );
			break;

		case PAT_LEFT:
			PatLeft( value );
			break;

		case PAT_RIGHT:
			PatRight( value );
			break;

		case PAT_DUMP:
			PatDump();
			break;

	}
}

//------------------------------------------------------------------------------
//! \brief Move the crab boss up
//!
//! 	\param destpos = dest y position
//------------------------------------------------------------------------------
void CCrabBoss::PatUp( int destpos )
{
	m_YPos-=CRAB_SPEED;
	if (m_YPos <= destpos) m_Pattern++;
}

//------------------------------------------------------------------------------
//! \brief Move the crab boss down
//!
//! 	\param destpos = dest y position
//------------------------------------------------------------------------------
void CCrabBoss::PatDown( int destpos )
{
	m_PipeCmd = PIPE_START;
	m_YPos+=CRAB_SPEED;
	if (m_YPos >= destpos) m_Pattern++;

}

//------------------------------------------------------------------------------
//! \brief Move the crab boss left
//!
//! 	\param destpos = dest x position
//------------------------------------------------------------------------------
void CCrabBoss::PatLeft( int destpos )
{
	m_XPos-=CRAB_SPEED;
	if (m_XPos <= destpos) m_Pattern++;

}

//------------------------------------------------------------------------------
//! \brief Move the crab boss right
//!
//! 	\param destpos = dest x position
//------------------------------------------------------------------------------
void CCrabBoss::PatRight( int destpos )
{
	m_XPos+=CRAB_SPEED;
	if (m_XPos >= destpos) m_Pattern++;

}

//------------------------------------------------------------------------------
//! \brief Make the crab boss baddie
//------------------------------------------------------------------------------
void CCrabBoss::PatDump( void )
{
	switch (m_PipeCmd)
	{
		case PIPE_START:
			// Check to see if need to reswawn (not 100% correct - added features!)
			if (m_pGame->m_BaddieList.FindFirst(OBJ_SPIKE))
			{
				m_Pattern++;
				break;
			}

			m_PipeOffset = 0;
			m_PipeCmd = PIPE_BOSSUP;
			// Fall through
		case PIPE_BOSSUP:
			m_PipeOffset+=2;
			m_YPos-=2;
			if (m_PipeOffset>=30)
			{
				m_PipeCmd = PIPE_BADDOWNPIPE;
				m_LumpOffset = 0;
			}
			break;
		case PIPE_BADDOWNPIPE:
			m_LumpOffset+=3;
			if (m_LumpOffset>=36)
			{
				m_LumpOffset = 0;
				m_EjectDir = m_EjectDir^1;	// Swap the direction
				m_EjectCnt = NUMSUCKANIM/2;
				m_PipeCmd = PIPE_LETOUT;
			}
			break;
		case PIPE_LETOUT:
			SUCKANIM *sanim;
			CSpikeObj *sobj;

			if (m_EjectCnt>=NUMSUCKANIM)	// All done?
			{
				m_EjectFrame = 0;
				m_PipeCmd = PIPE_UP;
				SMB_NEW(sobj,CSpikeObj);
				if (sobj)
				{
					sobj->Setup(m_EjectDir,m_EjectXOff+m_XPos,
						m_EjectYOff + m_YPos);
					m_pGame->m_BaddieList.Attach(sobj, OBJ_SPIKE, m_pGame);
					m_pGame->m_pSound->PlaySample(SND_SPIT, m_XPos);
				}

				break;
			}

			if (m_EjectDir) sanim = suck_spike_right;
				else sanim = suck_spike_left;
			sanim = &sanim[m_EjectCnt];
			m_EjectFrame = sanim->frame;
			m_EjectXOff = sanim->xoffset;
			if (m_EjectDir) m_EjectXOff+=0x38;
				else m_EjectXOff+=0x4;
			m_EjectYOff = sanim->yoffset + 0x70-1;
			m_EjectCnt++;
			break;
		case PIPE_UP:
			m_PipeOffset-=2;
			if (m_PipeOffset<=0)
			{
				m_PipeCmd = PIPE_START;
				m_Pattern++;
			}
	}
}

//------------------------------------------------------------------------------
//! \brief Do the collision detection between the baddie and this object
//------------------------------------------------------------------------------
void CCrabBoss::BossCollision( void )
{
	CSpikeObj *pobj;

	pobj = (CSpikeObj *) m_pGame->m_BaddieList.m_pFirst;
	for (;pobj;pobj = (CSpikeObj *) pobj->m_pNext)
	{
		if (pobj->m_Type==OBJ_SPIKE)	// Must be a spike (else illegal cast)
		{
			if (pobj->m_SuckCmd==SUCK_ROLL)
			{
				// (break, not continue because there can only be 1 baddie)
				if (HitHead(pobj,&crab_col_head)) break;
				if (HitBody(pobj,&crab_col_middle)) break;
				if (HitBody(pobj,&crab_col_bottom)) break;
			}
		}
	}
}


//------------------------------------------------------------------------------
//! \brief Check to see if a spike hit this objects head
//!
//! 	\param tobj = spike Object
//!	\param pcb = The collision box
//!
//! 	\return 0 = No collision occured
//------------------------------------------------------------------------------
int CCrabBoss::HitHead( CSpikeObj *tobj, OBJSIZE *pcb )
{
	int rcode;
	int cnt;

	rcode = HitBody( tobj, pcb );

	if (rcode)
	{
		m_Command = CRABBOSS_OUCH;
		m_OuchCount = 8;
		m_DamageCnt++;

		for (cnt=0; cnt<8; cnt++)
		{
			m_pGame->RandGoodie( m_XPos+16, m_YPos + 8, GOODIE_COMP, cnt&1 );
		}

		if (m_DamageCnt>=6)
		{
			if (m_Type!=OBJ_DEAD)	// This should always occur
			{
				m_pGame->SetBossDie( m_XPos + 16, m_YPos );
				DeleteObject();
			}
		}
	}

	return rcode;
}

//------------------------------------------------------------------------------
//! \brief Check to see if a spike hit this objects body
//!
//! 	\param tobj = Spike Object
//!	\param pcb = The collision box
//!
//! 	\return 0 = No collision occured
//------------------------------------------------------------------------------
int CCrabBoss::HitBody( CSpikeObj *tobj, OBJSIZE *pcb )
{
	InitSize(*pcb);
	if (CheckHit( tobj ))
	{
		tobj->SetExplode();
		return 1;
	}
	return 0;
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CCrabBoss::Do( void )
{
	switch (m_Command)
	{
		case CRABBOSS_NORMAL:
			m_ClawCounter++;

			CollisionCheck();
			BossCollision();

			DoPattern();
			break;
		case CRABBOSS_OUCH:
			m_WhiteFlag = 1;
			m_OuchCount--;
			if (m_OuchCount<0)
			{
				m_Command = CRABBOSS_NORMAL;
				m_WhiteFlag = 0;
			}
			break;
	}
}

//------------------------------------------------------------------------------
//! \brief Load the object graphics
//------------------------------------------------------------------------------
void CCrabBoss::LoadGfx( void )
{
	m_pGame->m_Sprites.LoadRange(SPR_CRAB_LCLAW1, SPR_CRAB_SEG);
	m_pGame->m_Sprites.LoadRange(SPR_CB_DAY1, SPR_CB_DAY3);
	m_pGame->m_Sprites.LoadRange(SPR_COMP_01, SPR_COMP_20);
}


//------------------------------------------------------------------------------
// ClockBoss
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Object Details
//------------------------------------------------------------------------------
static OBJSIZE clockboss_size = {0,0,96,0x7e};
static OBJMOVE clockboss_move = {280,280/2,5*256,230,0,4*256,0};

static OBJSIZE	clock_col_head = {0x20,0x00,(0x40)-(0x20),(0x20)-(0x00)};
static OBJSIZE	clock_col_body = {0x00,0x20,(0x60)-(0x00),(0x80)-(0x20)};

//------------------------------------------------------------------------------
// Clock hand offsets
//------------------------------------------------------------------------------

#define CLOCK_B_FRMS	6
#define CLOCK_S_FRMS	16

static CLKFRAMES clock_small[CLOCK_S_FRMS] =
	{	
		{SPR_CLOCK_S16,0x21,0x33},
		{SPR_CLOCK_S15,0x12,0x31},
		{SPR_CLOCK_S14,0x12,0x40},
		{SPR_CLOCK_S13,0x0e,0x48},
		{SPR_CLOCK_S12,0x12,0x50},
		{SPR_CLOCK_S11,0x10,0x4f},
		{SPR_CLOCK_S10,0x1f,0x4f},
		{SPR_CLOCK_S09,0x28,0x45},
		{SPR_CLOCK_S08,0x2f,0x4f},
		{SPR_CLOCK_S07,0x2f,0x50},
		{SPR_CLOCK_S06,0x2f,0x50},
		{SPR_CLOCK_S05,0x32,0x48},
		{SPR_CLOCK_S04,0x2e,0x42},
		{SPR_CLOCK_S03,0x2e,0x31},
		{SPR_CLOCK_S02,0x2f,0x33},
		{SPR_CLOCK_S01,0x28,0x2f}
	};

static CLKFRAMES clock_large[CLOCK_B_FRMS] =
	{	
		{SPR_CLOCK_B2,0x28,0x2a},
		{SPR_CLOCK_B1,0x10,0x30},
		{SPR_CLOCK_B4,0x10,0x52},
		{SPR_CLOCK_B5,0x28,0x58},
		{SPR_CLOCK_B6,0x2f,0x51},
		{SPR_CLOCK_B3,0x30,0x30}
	};

//------------------------------------------------------------------------------
// ClockBoss Commands
//------------------------------------------------------------------------------
#define CLOCKBOSS_NORMAL	0
#define CLOCKBOSS_OUCH		1

//------------------------------------------------------------------------------
//! \brief Initialise the number rising object
//------------------------------------------------------------------------------
CClockBoss::CClockBoss()
{
	m_Frame = SPR_BUG_LEFT1;
	m_Flags = FLAG_BADDIE | FLAG_HARDWALL;
	m_WhiteFlag = 0;

	InitMovement(clockboss_move);
	InitSize(clockboss_size);
	m_Y_Flag = -1;
	m_Command = CLOCKBOSS_NORMAL;
	m_DamageCnt = 0;
	m_HandCntS = m_HandCntB = 0;
	m_pFrmB = m_pFrmS = 0;
}

//------------------------------------------------------------------------------
//! \brief Draw the number rising object
//------------------------------------------------------------------------------
void CClockBoss::Draw( void )
{
	int xframe;
	int sprcolour;
	if (m_WhiteFlag)
	{
		sprcolour = GFX_WHITE;
		xframe = SPR_CB_DAY2;
	}else
	{
		sprcolour = 0;
		xframe = SPR_CB_DAY1;
	}
	m_pGame->m_Sprites.Draw( xframe, m_XPos+33, m_YPos );

	m_pGame->m_Sprites.Draw( SPR_CLOCK_MIDDLE, m_XPos+32, m_YPos, sprcolour );
	m_pGame->m_Sprites.Draw( SPR_CLOCK_LEFT, m_XPos, m_YPos+16, sprcolour );
	m_pGame->m_Sprites.Draw( SPR_CLOCK_RIGHT, m_XPos+64, m_YPos+16, sprcolour );

	if (m_pFrmB)
	{
		m_pGame->m_Sprites.Draw( m_pFrmB->frame,
			m_XPos + m_pFrmB->xpos, m_YPos + m_pFrmB->ypos, sprcolour );
	}

	if (m_pFrmS)
	{
		m_pGame->m_Sprites.Draw( m_pFrmS->frame,
			m_XPos + m_pFrmS->xpos, m_YPos + m_pFrmS->ypos, sprcolour );
	}

}

//------------------------------------------------------------------------------
//! \brief Do the collision detection between player and object
//------------------------------------------------------------------------------
void CClockBoss::CollisionCheck( void )
{
	InitSize(clock_col_head);
	CheckKillPlayer();
	InitSize(clock_col_body);
	CheckKillPlayer();

}

//------------------------------------------------------------------------------
//! \brief Do the collision detection between bowling ball and this object
//------------------------------------------------------------------------------
void CClockBoss::BallCollision( void )
{
	CBowlingObj *pobj;

	pobj = (CBowlingObj *) m_pGame->m_GasList.m_pFirst;
	for (;pobj;pobj = (CBowlingObj *) pobj->m_pNext)
	{
		if (pobj->m_Type==OBJ_BOWLING)	// Must be a ball (else illegal cast)
		{
			if (pobj->m_SuckCmd==SUCK_ROLL)
			{
				if (HitHead(pobj,&clock_col_head)) break;
				if (HitBody(pobj,&clock_col_body)) break;
			}
		}
	}
}

//------------------------------------------------------------------------------
//! \brief Check to see if a bowling ball hit this objects head
//!
//! 	\param tobj = bowling ball Object
//!	\param pcb = The collision box
//!
//! 	\return 0 = No collision occured
//------------------------------------------------------------------------------
int CClockBoss::HitHead( CBowlingObj *tobj, OBJSIZE *pcb )
{
	int rcode;
	int cnt;

	rcode = HitBody( tobj, pcb );

	if (rcode)
	{
		m_Command = CLOCKBOSS_OUCH;
		m_OuchCount = 8;
		m_DamageCnt++;

		for (cnt=0; cnt<8; cnt++)
		{
			m_pGame->RandGoodie( m_XPos+16, m_YPos + 8, GOODIE_COMP, cnt&1 );
		}

		if (m_DamageCnt>=6)
		{
			if (m_Type!=OBJ_DEAD)
			{
				m_pGame->SetBossDie( m_XPos + 16, m_YPos );
				DeleteObject();
			}
		}
	}

	return rcode;
}

//------------------------------------------------------------------------------
//! \brief Check to see if a bowling ball hit this objects body
//!
//! 	\param tobj = bowling ball Object
//!	\param pcb = The collision box
//!
//! 	\return 0 = No collision occured
//------------------------------------------------------------------------------
int CClockBoss::HitBody( CBowlingObj *tobj, OBJSIZE *pcb )
{
	InitSize(*pcb);
	if (CheckHit( tobj ))
	{
		tobj->SetExplode();
		return 1;
	}
	return 0;
}

//------------------------------------------------------------------------------
//! \brief Control the clock hands
//------------------------------------------------------------------------------
void CClockBoss::ControlHands( void )
{
	m_HandCntS++;
	if (m_HandCntS>=CLOCK_S_FRMS) m_HandCntS = 0;

	m_HandCntB++;
	if (m_HandCntB>=CLOCK_B_FRMS) m_HandCntB = 0;

	m_pFrmS = &clock_small[m_HandCntS];
	m_pFrmB = &clock_large[m_HandCntB];

}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CClockBoss::Do( void )
{
	switch (m_Command)
	{
		case CLOCKBOSS_NORMAL:

			ControlHands();
			CollisionCheck();
			BallCollision();
			InitSize(clockboss_size);
			DirFly();
			break;
		case CLOCKBOSS_OUCH:
			m_WhiteFlag = 1;
			m_OuchCount--;
			if (m_OuchCount<0)
			{
				m_Command = CLOCKBOSS_NORMAL;
				m_WhiteFlag = 0;
			}
			break;
	}
}

//------------------------------------------------------------------------------
//! \brief Load the object graphics
//------------------------------------------------------------------------------
void CClockBoss::LoadGfx( void )
{
	m_pGame->m_Sprites.LoadRange(SPR_CB_DAY1, SPR_CB_DAY3);
	m_pGame->m_Sprites.LoadRange(SPR_CLOCK_LEFT, SPR_CLOCK_S16);
	m_pGame->m_Sprites.LoadRange(SPR_COMP_01, SPR_COMP_20);
}

//------------------------------------------------------------------------------
// EndBoss
//------------------------------------------------------------------------------

#define BOSS_FIRE_RATE 20

//------------------------------------------------------------------------------
// Object Details
//------------------------------------------------------------------------------
static OBJSIZE endboss_size = {6+2,8,22-4,24};
static OBJMOVE endboss_move_slow = {160,160/2,4*256,11*256,285,11*256,11*256};

//------------------------------------------------------------------------------
// Animation
//------------------------------------------------------------------------------

static int anm_lboss_left[] =
	{
	SPR_LBOSS_L1,SPR_LBOSS_L2,SPR_LBOSS_L3,SPR_LBOSS_L4,
	SPR_LBOSS_L5,SPR_LBOSS_L6,SPR_LBOSS_L7,SPR_LBOSS_L8,
	ANM_RESTART
	};
static int anm_lboss_right[] =
	{
	SPR_LBOSS_R1,SPR_LBOSS_R2,SPR_LBOSS_R3,SPR_LBOSS_R4,
	SPR_LBOSS_R5,SPR_LBOSS_R6,SPR_LBOSS_R7,SPR_LBOSS_R8,
	ANM_RESTART
	};

//------------------------------------------------------------------------------
// Sucking in frames
//------------------------------------------------------------------------------

SUCKANIM suck_lboss_right[NUMSUCKANIM] =
	{
		{SPR_LBOSS_SUCKR1, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_LBOSS_SUCKR2, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_LBOSS_SUCKR3, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_LBOSS_SUCKR4, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_LBOSS_SUCKR5, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_LBOSS_SUCKR6, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_LBOSS_SUCKR6, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_LBOSS_SUCKR6, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_LBOSS_SUCKR4, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_LBOSS_SUCKR3, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_LBOSS_SUCKR2, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_LBOSS_SUCKR1, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y}
	};
SUCKANIM suck_lboss_left[NUMSUCKANIM] =
	{
		{SPR_LBOSS_SUCKL1, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_LBOSS_SUCKL2, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_LBOSS_SUCKL3, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_LBOSS_SUCKL4, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_LBOSS_SUCKL5,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_LBOSS_SUCKL6,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_LBOSS_SUCKL6,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_LBOSS_SUCKL5,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_LBOSS_SUCKL4, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_LBOSS_SUCKL3, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_LBOSS_SUCKL2, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_LBOSS_SUCKL1, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y}
	};

//------------------------------------------------------------------------------
// Sucking Roll frames
//------------------------------------------------------------------------------
static int suck_lboss_roll[8] =
	{
		SPR_LBOSS_ROLL1,SPR_LBOSS_ROLL2,SPR_LBOSS_ROLL3,SPR_LBOSS_ROLL4,
		SPR_LBOSS_ROLL5,SPR_LBOSS_ROLL6,SPR_LBOSS_ROLL7,SPR_LBOSS_ROLL8
	};

//------------------------------------------------------------------------------
// Sucking general frames
//------------------------------------------------------------------------------
static SUCKFRAMES suck_lboss_frames=
	{
		SPR_LBOSS_HATCH1,SPR_LBOSS_HATCH2,
		suck_lboss_roll, 8,
		suck_lboss_left, suck_lboss_right
	};

//------------------------------------------------------------------------------
//! \brief Initialise the object
//------------------------------------------------------------------------------
CEndBoss::CEndBoss()
{
	m_Frame = SPR_LBOSS_L1;
	m_Flags = FLAG_BADDIE;

	m_DieScore = 1000;
	m_DieScoreFrame = SPR_NUM_1000;
	m_pSequence = anm_lboss_left;
	m_Release_YOff = 8;
	InitMovement(endboss_move_slow);
	InitSize(endboss_size);
	m_Segment = 0;
	m_pSuckFrames = &suck_lboss_frames;
	m_pLastSeg = m_pNextSeg = 0;
	m_HatFrame = SPR_LBOSS_DSMALL;
	m_BombPause = 0;
}

//------------------------------------------------------------------------------
//! \brief Destroy the object
//------------------------------------------------------------------------------
CEndBoss::~CEndBoss()
{
	// Unlink boss
	if (m_pLastSeg) m_pLastSeg->m_pNextSeg = m_pNextSeg;
	if (m_pNextSeg) m_pNextSeg->m_pLastSeg = m_pLastSeg;
}

//------------------------------------------------------------------------------
//! \brief Draw the object
//------------------------------------------------------------------------------
void CEndBoss::Draw( void )
{
	int yoffset;
	if (RunDrawSuck()) return;

	CBitmapItem *item;
	item = m_pGame->m_Sprites.GetItem( m_Frame );
	if (item)
	{
		yoffset = item->m_YOff;
	}else
	{
		yoffset = 0;
	}

	if (!m_pNextSeg)	// Has man?
	{
		int xpos;
		int ypos;
		int frm;

		xpos = m_XPos;
		ypos = m_YPos;
		if (m_Dir==DIR_LEFT)
		{
			frm = SPR_LBOSS_FLEFT;
			xpos+= 8;
		}else
		{
			frm = SPR_LBOSS_FRIGHT;
			xpos+= 7;
		}
		if ((!m_Segment) && (m_HatFrame))
		{
			ypos -=24;
		}else
		{
			ypos -=20;
		}

		m_pGame->m_Sprites.Draw( frm, xpos, ypos+yoffset );
	}

	m_pGame->m_Sprites.Draw( m_Frame, m_XPos, m_YPos );
	if (m_HatFrame)
	{
		m_pGame->m_Sprites.Draw( m_HatFrame, m_XPos, m_YPos-(0x1e)+yoffset );
	}
}

//------------------------------------------------------------------------------
//! \brief Setup the object getting caught
//------------------------------------------------------------------------------
void CEndBoss::SetupCaught( void )
{
	CSuckable::SetupCaught();	// Must call virtual member

	if (m_HatFrame)	// Destroy Hat?
	{
		CDropOffObj *cobj;
		SMB_NEW(cobj,CDropOffObj);
		if (cobj)
		{
			m_pGame->InitFrkObject(cobj, OBJ_DROPOFF, m_XPos , m_YPos-20,
				&m_pGame->m_GoodieList);
			cobj->m_Frame = m_HatFrame;
		}
		m_HatFrame = 0;
	}
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CEndBoss::Do( void )
{
	if (RunDoSuck()) return;

	CheckKillPlayer();
	if (!(m_Y_Flag))	// Only move if on ground
	{
		DirWalk();
	}
	MoveObject();
	AnimateXInert(anm_lboss_left, anm_lboss_right);

	// Check if suckable
	if (!m_pNextSeg)
	{
		ControlDay();
		m_Flags = FLAG_BADDIE | FLAG_CATCHABLE;
	}

	// Control Main Segments Hat
	if (!m_Segment)
	{
		if (m_HatFrame)
		{
			if (m_Dir==DIR_LEFT) m_HatFrame = SPR_LBOSS_DLEFT;
			else m_HatFrame = SPR_LBOSS_DRIGHT;
		}
	}
}

//------------------------------------------------------------------------------
//! \brief Load the object graphics
//------------------------------------------------------------------------------
void CEndBoss::LoadGfx( void )
{
	m_pGame->m_Sprites.LoadRange(SPR_CB_DAY1, SPR_CB_DAY3);
	m_pGame->m_Sprites.LoadRange(SPR_LBOSS_L1, SPR_LBOSS_BOOM);
	m_pGame->m_Sprites.LoadRange(SPR_EXPAND_A1, SPR_EXPAND_E3);
}

//------------------------------------------------------------------------------
//! \brief Setup the object
//!
//! 	\param seg = segment number (0-7)
//------------------------------------------------------------------------------
void CEndBoss::Setup( int seg )
{
	m_Segment = seg;
}

//------------------------------------------------------------------------------
//! \brief Control do dah day in the back of the train
//------------------------------------------------------------------------------
void CEndBoss::ControlDay( void )
{
	m_BombPause++;
	if (m_BombPause>=BOSS_FIRE_RATE)
	{
		m_BombPause = 0;

		CBombObj *cobj;
		SMB_NEW(cobj,CBombObj);
		if (cobj)
		{
			m_pGame->InitFrkObject(cobj, OBJ_BOMB, m_XPos , m_YPos,
				&m_pGame->m_BaddieList);
			cobj->m_XInert = -m_XInert;
		}
	}
}

//------------------------------------------------------------------------------
//! \brief Set the object to explode
//------------------------------------------------------------------------------
void CEndBoss::SetExplode( void )
{
	if (!m_Segment)
	{
		CKillBossObj *cobj;
		SMB_NEW(cobj,CKillBossObj);
		if (cobj)
		{
			m_pGame->InitFrkObject(cobj, OBJ_KILLBOSS,
				(SCR_WIDTH/2) - 16, (SCR_HEIGHT/2) - 16,
				&m_pGame->m_BaddieList);
		}else
		{
			// NOTE IF ALLOCATION FAILED - STILL COMPLETE THE GAME!
		}

	}

	CSuckable::SetExplode();
}

//------------------------------------------------------------------------------
// The KillBossObj Object
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//! \brief Initialise the KillBossObj
//------------------------------------------------------------------------------
CKillBossObj::CKillBossObj()
{
	m_Flags = FLAG_BADDIE;
	m_Frame = SPR_EXPAND_A1;
	m_BossNumber = 0;
	m_Counter = 0;
}

//------------------------------------------------------------------------------
//! \brief Draw the object
//------------------------------------------------------------------------------
void CKillBossObj::Draw( void )
{
	switch (m_BossNumber)
	{
		case (0):
			m_pGame->m_Sprites.Draw( SPR_EXPAND_A1, m_XPos, m_YPos, GFX_NOWRAP );
			break;
		case (1):
			m_pGame->m_Sprites.Draw( SPR_EXPAND_B1, m_XPos-0x10, m_YPos, GFX_NOWRAP );
			m_pGame->m_Sprites.Draw( SPR_EXPAND_B2, m_XPos+0x10, m_YPos, GFX_NOWRAP );
			break;
		case (2):
			m_pGame->m_Sprites.Draw( SPR_EXPAND_C1, m_XPos-0x10, m_YPos-0x10, GFX_NOWRAP );
			m_pGame->m_Sprites.Draw( SPR_EXPAND_C2, m_XPos+0x10, m_YPos-0x10, GFX_NOWRAP );
			break;
		case (3):
			m_pGame->m_Sprites.Draw( SPR_EXPAND_D1, m_XPos-(0x10)-4, m_YPos-0x10, GFX_NOWRAP );
			m_pGame->m_Sprites.Draw( SPR_EXPAND_D2, m_XPos+(0x10)-4, m_YPos-0x10, GFX_NOWRAP );
			m_pGame->m_Sprites.Draw( SPR_EXPAND_D3, m_XPos+(0x30)-4, m_YPos-0x10, GFX_NOWRAP );
			break;
		case (4):
			m_pGame->m_Sprites.Draw( SPR_EXPAND_E1, m_XPos-(0x10)-8, m_YPos, GFX_NOWRAP );
			m_pGame->m_Sprites.Draw( SPR_EXPAND_E2, m_XPos+(0x10)-8, m_YPos-0x10, GFX_NOWRAP );
			m_pGame->m_Sprites.Draw( SPR_EXPAND_E3, m_XPos+(0x30)-8, m_YPos, GFX_NOWRAP );
			break;
	}
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CKillBossObj::Do( void )
{
	m_Counter++;
	if (!(m_Counter&3))
	{
		m_BossNumber++;
		if (m_BossNumber>4) m_BossNumber = 4;
	}
	if (m_BossNumber>=4)
	{
		m_YPos+=8;
		if (m_YPos > (SCR_HEIGHT+32))
 		{
			DeleteObject();
		}
	}
}

