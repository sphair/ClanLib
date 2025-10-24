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
// Methane Brothers Baddie Objects (Source File)
//------------------------------------------------------------------------------
#include "stdio.h"
#include "stdlib.h"
#include "baddie.h"
#include "global.h"
#include "bitgroup.h"
#include "player.h"
#include "objtypes.h"
#include "game.h"
#include "weapon.h"
#include "goodie.h"

//------------------------------------------------------------------------------
// Animation
//------------------------------------------------------------------------------

static int anm_bug_left[] =
	{
	SPR_BUG_LEFT1,SPR_BUG_LEFT2,SPR_BUG_LEFT3,SPR_BUG_LEFT4,
	SPR_BUG_LEFT5,SPR_BUG_LEFT6,ANM_RESTART
	};
static int anm_bug_right[] =
	{
	SPR_BUG_RIGHT1,SPR_BUG_RIGHT2,
	SPR_BUG_RIGHT3,SPR_BUG_RIGHT4,SPR_BUG_RIGHT5,SPR_BUG_RIGHT6,
	ANM_RESTART
	};

//------------------------------------------------------------------------------
// Sucking in frames
//------------------------------------------------------------------------------

SUCKANIM suck_bug_right[NUMSUCKANIM] =
	{
		{SPR_BUG_SUCKR1, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_BUG_SUCKR2, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_BUG_SUCKR3, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_BUG_SUCKR4, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_BUG_SUCKR5,  2+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_BUG_SUCKR6, -1+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_BUG_INGUN,   3+ SUCKFIX_LX,  0+	SUCKFIX_Y},
		{SPR_BUG_SUCKR6,  2+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_BUG_SUCKR4, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_BUG_SUCKR3, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_BUG_SUCKR2, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_BUG_SUCKR1, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y}
	};
SUCKANIM suck_bug_left[NUMSUCKANIM] =
	{
		{SPR_BUG_SUCKL1, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_BUG_SUCKL2, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_BUG_SUCKL3, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_BUG_SUCKL4, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_BUG_SUCKL5,  -8+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_BUG_SUCKL6,  -5+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_BUG_INGUN,    7+ SUCKFIX_RX,  0+	SUCKFIX_Y},
		{SPR_BUG_SUCKL5,  -8+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_BUG_SUCKL4, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_BUG_SUCKL3, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_BUG_SUCKL2, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_BUG_SUCKL1, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y}
	};
//------------------------------------------------------------------------------
// Sucking Roll frames
//------------------------------------------------------------------------------
static int suck_bug_roll[8] =
	{
		SPR_BUG_ROLL1,SPR_BUG_ROLL2,SPR_BUG_ROLL3,SPR_BUG_ROLL4,
		SPR_BUG_ROLL5,SPR_BUG_ROLL6,SPR_BUG_ROLL7,SPR_BUG_ROLL8
	};

//------------------------------------------------------------------------------
// Sucking general frames
//------------------------------------------------------------------------------
static SUCKFRAMES suck_bug_frames=
	{
		SPR_BUG_HATCH1,SPR_BUG_HATCH2,
		suck_bug_roll, 8,
		suck_bug_left, suck_bug_right
	};
//------------------------------------------------------------------------------
// Object Details
//------------------------------------------------------------------------------
static OBJSIZE bug_size = {6,8,22,24};
static OBJMOVE bug_move_slow = {160,160/2,4*256,11*256,285,11*256,11*256};
static OBJMOVE bug_move_fast = {0x2a0,0x100,6*256,0xf*256,0x1d0,0xf*256,0xf*256};

//------------------------------------------------------------------------------
//! \brief Initialise the bug object
//------------------------------------------------------------------------------
CBugObj::CBugObj()
{
	Reset();
}

//------------------------------------------------------------------------------
//! \brief Draw the bug object
//------------------------------------------------------------------------------
void CBugObj::Draw( void )
{
	if (RunDrawSuck()) return;
	if (m_Frame) m_pGame->m_Sprites.Draw( m_Frame, m_XPos, m_YPos );
}


//------------------------------------------------------------------------------
//! \brief Load the object graphics
//------------------------------------------------------------------------------
void CBugObj::LoadGfx( void )
{
	m_pGame->m_Sprites.LoadRange(SPR_BUG_LEFT1, SPR_BUG_INGUN);
}

//------------------------------------------------------------------------------
//! \brief Reset the object (object members)
//------------------------------------------------------------------------------
void CBugObj::Reset( void )
{
	m_DieScore = 1000;
	m_DieScoreFrame = SPR_NUM_1000;
	m_OldXPos = m_XPos = 100;
	m_OldYPos =	m_YPos = 100;
	m_Frame = SPR_BUG_LEFT1;
	m_pSequence = anm_bug_left;
	m_Release_YOff = 8;
	InitMovement(bug_move_slow);
	InitSize(bug_size);
	m_Y_Flag = -1;

	m_Flags = FLAG_CATCHABLE | FLAG_BADDIE;

	m_pSuckFrames = &suck_bug_frames;
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CBugObj::Do( void )
{
	if (RunDoSuck()) return;
	ControlSpeedUp( bug_move_slow, bug_move_fast );
	CheckOil();
	if (CheckExtra()) return;

	CheckKillPlayer();

	if (!(m_Y_Flag))	// Only move if on ground
	{
		DirWalk();
		Jump2Player( 31, 8 );
	}
	MoveObject();
	AnimateXInert(anm_bug_left, anm_bug_right);
}

//------------------------------------------------------------------------------
// Animation
//------------------------------------------------------------------------------

static int anm_whirly_left[] =
	{
	SPR_WHIRLY_LEFT1,SPR_WHIRLY_LEFT2,SPR_WHIRLY_LEFT3,SPR_WHIRLY_LEFT4,
	SPR_WHIRLY_LEFT5,SPR_WHIRLY_LEFT6,ANM_RESTART
	};
static int anm_whirly_right[] =
	{
	SPR_WHIRLY_RIGHT1,SPR_WHIRLY_RIGHT2,
	SPR_WHIRLY_RIGHT3,SPR_WHIRLY_RIGHT4,SPR_WHIRLY_RIGHT5,SPR_WHIRLY_RIGHT6,
	ANM_RESTART
	};

//------------------------------------------------------------------------------
// Sucking in frames
//------------------------------------------------------------------------------

static SUCKANIM suck_whirly_right[NUMSUCKANIM] =
	{
		{SPR_WHIRLY_SUCKR1, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_WHIRLY_SUCKR2, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_WHIRLY_SUCKR3, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_WHIRLY_SUCKR4, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_WHIRLY_SUCKR5, -4+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_WHIRLY_SUCKR6, -4+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_WHIRLY_SUCKR6, -5+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_WHIRLY_SUCKR6, -4+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_WHIRLY_SUCKR4, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_WHIRLY_SUCKR3, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_WHIRLY_SUCKR2, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_WHIRLY_SUCKR1, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y}
	};
static SUCKANIM suck_whirly_left[NUMSUCKANIM] =
	{
		{SPR_WHIRLY_SUCKL1, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_WHIRLY_SUCKL2, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_WHIRLY_SUCKL3, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_WHIRLY_SUCKL4, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_WHIRLY_SUCKL5,  -5+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_WHIRLY_SUCKL6,  -2+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_WHIRLY_SUCKL6,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_WHIRLY_SUCKL5,  -5+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_WHIRLY_SUCKL4, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_WHIRLY_SUCKL3, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_WHIRLY_SUCKL2, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_WHIRLY_SUCKL1, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y}
	};
//------------------------------------------------------------------------------
// Sucking Roll frames
//------------------------------------------------------------------------------
static int suck_whirly_roll[8] =
	{
		SPR_WHIRLY_ROLL1,SPR_WHIRLY_ROLL2,SPR_WHIRLY_ROLL3,SPR_WHIRLY_ROLL4,
		SPR_WHIRLY_ROLL5,SPR_WHIRLY_ROLL6,SPR_WHIRLY_ROLL7,SPR_WHIRLY_ROLL8
	};

//------------------------------------------------------------------------------
// Sucking general frames
//------------------------------------------------------------------------------
static SUCKFRAMES suck_whirly_frames=
	{
		SPR_WHIRLY_HATCH1,SPR_WHIRLY_HATCH2,
		suck_whirly_roll, 8,
		suck_whirly_left, suck_whirly_right
	};
//------------------------------------------------------------------------------
// Object Details
//------------------------------------------------------------------------------
static OBJSIZE whirly_size = {6,2,22,41};
static OBJMOVE whirly_move_slow = {300,300/2,3*256,300,300/2,3*256,0};
static OBJMOVE whirly_move_fast = {320,320/2,4*256,320,320/2,4*256,0};

//------------------------------------------------------------------------------
//! \brief Initialise the whirly object
//------------------------------------------------------------------------------
CWhirlyObj::CWhirlyObj()
{
	Reset();
}

//------------------------------------------------------------------------------
//! \brief Draw the whirly object
//------------------------------------------------------------------------------
void CWhirlyObj::Draw( void )
{
	if (RunDrawSuck()) return;
	if (m_Frame) m_pGame->m_Sprites.Draw( m_Frame, m_XPos, m_YPos );
}


//------------------------------------------------------------------------------
//! \brief Load the object graphics
//------------------------------------------------------------------------------
void CWhirlyObj::LoadGfx( void )
{
	m_pGame->m_Sprites.LoadRange(SPR_WHIRLY_LEFT1, SPR_WHIRLY_SUCKR6);
}

//------------------------------------------------------------------------------
//! \brief Reset the object (object members)
//------------------------------------------------------------------------------
void CWhirlyObj::Reset( void )
{
	m_DieScore = 1000;
	m_DieScoreFrame = SPR_NUM_1000;
	m_OldXPos = m_XPos = 100;
	m_OldYPos =	m_YPos = 100;
	m_Frame = SPR_WHIRLY_LEFT1;
	m_pSequence = anm_whirly_left;
	m_Release_YOff = 16;

	InitMovement(whirly_move_slow);
	InitSize(whirly_size);
	m_Y_Flag = -1;

	m_Flags = FLAG_CATCHABLE | FLAG_BADDIE | FLAG_HARDWALL;

	m_pSuckFrames = &suck_whirly_frames;

	m_YDir = rand()&1;
	m_Dir = rand()&1;
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CWhirlyObj::Do( void )
{
	if (RunDoSuck()) return;
	ControlSpeedUp( whirly_move_slow, whirly_move_fast );
	CheckOil();
	if (CheckExtra()) return;

	CheckKillPlayer();

	DirFly();

	AnimateDir(anm_whirly_left, anm_whirly_right, 256);
}

//------------------------------------------------------------------------------
// Animation
//------------------------------------------------------------------------------

static int anm_doofus_left[] =
	{
	SPR_DOOFUS_LEFT1,SPR_DOOFUS_LEFT2,SPR_DOOFUS_LEFT3,SPR_DOOFUS_LEFT4,
	SPR_DOOFUS_LEFT5,SPR_DOOFUS_LEFT6,ANM_RESTART
	};
static int anm_doofus_right[] =
	{
	SPR_DOOFUS_RIGHT1,SPR_DOOFUS_RIGHT2,
	SPR_DOOFUS_RIGHT3,SPR_DOOFUS_RIGHT4,SPR_DOOFUS_RIGHT5,SPR_DOOFUS_RIGHT6,
	ANM_RESTART
	};

//------------------------------------------------------------------------------
// Sucking in frames
//------------------------------------------------------------------------------

static SUCKANIM suck_doofus_right[NUMSUCKANIM] =
	{
		{SPR_DOOFUS_SUCKR1, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_DOOFUS_SUCKR2, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_DOOFUS_SUCKR3, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_DOOFUS_SUCKR4, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_DOOFUS_SUCKR5, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_DOOFUS_SUCKR6, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_DOOFUS_SUCKR6, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_DOOFUS_SUCKR6, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_DOOFUS_SUCKR4, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_DOOFUS_SUCKR3, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_DOOFUS_SUCKR2, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_DOOFUS_SUCKR1, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y}
	};
static SUCKANIM suck_doofus_left[NUMSUCKANIM] =
	{
		{SPR_DOOFUS_SUCKL1, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_DOOFUS_SUCKL2, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_DOOFUS_SUCKL3, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_DOOFUS_SUCKL4, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_DOOFUS_SUCKL5,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_DOOFUS_SUCKL6,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_DOOFUS_SUCKL6,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_DOOFUS_SUCKL5,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_DOOFUS_SUCKL4, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_DOOFUS_SUCKL3, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_DOOFUS_SUCKL2, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_DOOFUS_SUCKL1, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y}
	};
//------------------------------------------------------------------------------
// Sucking Roll frames
//------------------------------------------------------------------------------
static int suck_doofus_roll[8] =
	{
		SPR_DOOFUS_ROLL1,SPR_DOOFUS_ROLL2,SPR_DOOFUS_ROLL3,SPR_DOOFUS_ROLL4,
		SPR_DOOFUS_ROLL5,SPR_DOOFUS_ROLL6,SPR_DOOFUS_ROLL7,SPR_DOOFUS_ROLL8
	};

//------------------------------------------------------------------------------
// Sucking general frames
//------------------------------------------------------------------------------
static SUCKFRAMES suck_doofus_frames=
	{
		SPR_DOOFUS_HATCH1,SPR_DOOFUS_HATCH2,
		suck_doofus_roll, 8,
		suck_doofus_left, suck_doofus_right
	};
//------------------------------------------------------------------------------
// Object Details
//------------------------------------------------------------------------------
static OBJSIZE doofus_size = {2,1,26,23};
static OBJMOVE doofus_move = {130,130/2,2*256,130,130/2,2*256,0};

//------------------------------------------------------------------------------
//! \brief Initialise the doofus object
//------------------------------------------------------------------------------
CDoofusObj::CDoofusObj()
{
	Reset();
}

//------------------------------------------------------------------------------
//! \brief Draw the doofus object
//------------------------------------------------------------------------------
void CDoofusObj::Draw( void )
{
	if (RunDrawSuck()) return;
	if (m_Frame) m_pGame->m_Sprites.Draw( m_Frame, m_XPos, m_YPos );
}

//------------------------------------------------------------------------------
//! \brief Load the object graphics
//------------------------------------------------------------------------------
void CDoofusObj::LoadGfx( void )
{
	m_pGame->m_Sprites.LoadRange(SPR_DOOFUS_LEFT1, SPR_DOOFUS_SUCKR6);
}

//------------------------------------------------------------------------------
//! \brief Reset the object (object members)
//------------------------------------------------------------------------------
void CDoofusObj::Reset( void )
{
	m_DieScore = 5000;
	m_DieScoreFrame = SPR_NUM_5000;
	m_OldXPos = m_XPos = 100;
	m_OldYPos =	m_YPos = 100;
	m_Frame = SPR_DOOFUS_LEFT1;
	m_pSequence = anm_doofus_left;

	InitMovement(doofus_move);
	InitSize(doofus_size);
	m_Y_Flag = -1;

	m_Flags = FLAG_CATCHABLE | FLAG_BADDIE;

	m_pSuckFrames = &suck_doofus_frames;

}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CDoofusObj::Do( void )
{
	if (RunDoSuck()) return;
	if (CheckExtra()) return;

	CheckKillPlayer();

	Fly2ClosestPlayer();

	MoveObject();

	AnimateDir(anm_doofus_left, anm_doofus_right, 256);
}

//------------------------------------------------------------------------------
// Sucking in frames
//------------------------------------------------------------------------------

static SUCKANIM suck_jump_right[NUMSUCKANIM] =
	{
		{SPR_JUMP_LSUCKR1, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_JUMP_LSUCKR2, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_JUMP_LSUCKR3, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_JUMP_LSUCKR4, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_JUMP_LSUCKR5, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_JUMP_LSUCKR6, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_JUMP_LSUCKR6, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_JUMP_LSUCKR6, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_JUMP_LSUCKR4, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_JUMP_LSUCKR3, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_JUMP_LSUCKR2, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_JUMP_LSUCKR1, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y}
	};
static SUCKANIM suck_jump_left[NUMSUCKANIM] =
	{
		{SPR_JUMP_LSUCKL1, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_JUMP_LSUCKL2, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_JUMP_LSUCKL3, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_JUMP_LSUCKL4, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_JUMP_LSUCKL5,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_JUMP_LSUCKL6,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_JUMP_LSUCKL6,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_JUMP_LSUCKL5,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_JUMP_LSUCKL4, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_JUMP_LSUCKL3, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_JUMP_LSUCKL2, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_JUMP_LSUCKL1, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y}
	};

static SUCKANIM suck_jump_sright[NUMSUCKANIM] =
	{
		{SPR_JUMP_SSUCKR1, 26+ SUCKFIX_LX, -3+	SUCKFIX_Y},
		{SPR_JUMP_SSUCKR2, 26+ SUCKFIX_LX, -2+	SUCKFIX_Y},
		{SPR_JUMP_SSUCKR3, 26+ SUCKFIX_LX, -1+	SUCKFIX_Y},
		{SPR_JUMP_SSUCKR4, 26+ SUCKFIX_LX, -1+	SUCKFIX_Y},
		{SPR_JUMP_LSUCKR5, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_JUMP_LSUCKR6, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_JUMP_LSUCKR6, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_JUMP_LSUCKR6, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_JUMP_SSUCKR4, 26+ SUCKFIX_LX, -1+	SUCKFIX_Y},
		{SPR_JUMP_SSUCKR3, 26+ SUCKFIX_LX, -1+	SUCKFIX_Y},
		{SPR_JUMP_SSUCKR2, 26+ SUCKFIX_LX, -2+	SUCKFIX_Y},
		{SPR_JUMP_SSUCKR1, 26+ SUCKFIX_LX, -3+	SUCKFIX_Y}
	};
static SUCKANIM suck_jump_sleft[NUMSUCKANIM] =
	{
		{SPR_JUMP_SSUCKL1, -16+ SUCKFIX_RX, -3+	SUCKFIX_Y},
		{SPR_JUMP_SSUCKL2, -16+ SUCKFIX_RX, -2+	SUCKFIX_Y},
		{SPR_JUMP_SSUCKL3, -16+ SUCKFIX_RX, -1+	SUCKFIX_Y},
		{SPR_JUMP_SSUCKL4, -16+ SUCKFIX_RX, -1+	SUCKFIX_Y},
		{SPR_JUMP_LSUCKL5,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_JUMP_LSUCKL6,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_JUMP_LSUCKL6,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_JUMP_LSUCKL5,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_JUMP_SSUCKL4, -16+ SUCKFIX_RX, -1+	SUCKFIX_Y},
		{SPR_JUMP_SSUCKL3, -16+ SUCKFIX_RX, -1+	SUCKFIX_Y},
		{SPR_JUMP_SSUCKL2, -16+ SUCKFIX_RX, -2+	SUCKFIX_Y},
		{SPR_JUMP_SSUCKL1, -16+ SUCKFIX_RX, -3+	SUCKFIX_Y}
	};

//------------------------------------------------------------------------------
// Sucking Roll frames
//------------------------------------------------------------------------------
static int suck_jump_roll[8] =
	{
		SPR_JUMP_LROLL1,SPR_JUMP_LROLL2,SPR_JUMP_LROLL3,SPR_JUMP_LROLL4,
		SPR_JUMP_LROLL5,SPR_JUMP_LROLL6,SPR_JUMP_LROLL7,SPR_JUMP_LROLL8
	};

static int suck_jump_sroll[8] =
	{
		SPR_JUMP_SROLL1,SPR_JUMP_SROLL2,SPR_JUMP_SROLL3,SPR_JUMP_SROLL4,
		SPR_JUMP_SROLL5,SPR_JUMP_SROLL6,SPR_JUMP_SROLL7,SPR_JUMP_SROLL8
	};

//------------------------------------------------------------------------------
// Sucking general frames
//------------------------------------------------------------------------------
SUCKFRAMES suck_jump_frames=
	{
		SPR_JUMP_LHATCH1,SPR_JUMP_LHATCH2,
		suck_jump_roll, 8,
		suck_jump_left, suck_jump_right
	};

SUCKFRAMES suck_jump_smallframes=
	{
		SPR_JUMP_SHATCH1,SPR_JUMP_SHATCH2,
		suck_jump_sroll, 8,
		suck_jump_sleft, suck_jump_sright
	};

//------------------------------------------------------------------------------
// Object Details
//------------------------------------------------------------------------------
OBJSIZE jump_size = {6,0,21,31};
OBJMOVE jump_move = {130,130/2,5*256,14*256,175,14*256,14*256};
OBJSIZE jump_smallsize = {8,0,8,15};

//------------------------------------------------------------------------------
//! \brief Initialise the jump object
//------------------------------------------------------------------------------
CJumpObj::CJumpObj()
{
	Reset();
}

//------------------------------------------------------------------------------
//! \brief Draw the jump object
//------------------------------------------------------------------------------
void CJumpObj::Draw( void )
{
	if (RunDrawSuck()) return;
	if (m_Frame) m_pGame->m_Sprites.Draw( m_Frame, m_XPos, m_YPos );
}

//------------------------------------------------------------------------------
//! \brief Load the object graphics
//------------------------------------------------------------------------------
void CJumpObj::LoadGfx( void )
{
	m_pGame->m_Sprites.LoadRange(SPR_JUMP_LJUMP1, SPR_JUMP_SSUCKR4);
}

//------------------------------------------------------------------------------
//! \brief Reset the object (object members)
//------------------------------------------------------------------------------
void CJumpObj::Reset( void )
{
	m_DieScore = 4000;
	m_DieScoreFrame = SPR_NUM_4000;
	m_OldXPos = m_XPos = 100;
	m_OldYPos =	m_YPos = 100;
	m_Frame = SPR_JUMP_LJUMP1;

	InitMovement(jump_move);
	InitSize(jump_size);
	m_Y_Flag = -1;

	m_Flags = FLAG_CATCHABLE | FLAG_BADDIE;

	m_pSuckFrames = &suck_jump_frames;
	m_FixFlag = 1;
	m_JumpSize = 0;
	m_ImmumeCnt = 30;	// Immume for 30 frames
}

//------------------------------------------------------------------------------
//! \brief Control the jump - Another Exact AMIGA conversion !!!
//!
//! 	\param frmoffset = frame offset
//------------------------------------------------------------------------------
void CJumpObj::DoJump( int frmoffset )
{
	if ((!m_Y_Flag) || (m_FixFlag))	// On Ground?
	{
		if (m_Frame >= (frmoffset+SPR_JUMP_LJUMP2))
		{
			m_Frame = (frmoffset+SPR_JUMP_LJUMP1);
			m_FixFlag = 1;
		}
		if (!(rand()&15))	// Random Jump Interval
		{
			m_Frame = (frmoffset+SPR_JUMP_LJUMP2);
			SetMoveUp();	// Make the object jump
			m_FixFlag = 0;			
		}
	}else				// In the air
	{
		if (m_YInert<0)	// Going Up?
		{
			if (m_Frame == (frmoffset+SPR_JUMP_LJUMP1))
			{
				m_Frame = (frmoffset+SPR_JUMP_LJUMP2);
			}else
			{
				m_Frame = (frmoffset+SPR_JUMP_LJUMP3);
			}
		}else
		{
			if (!m_YInert)	// At top?
			{
				m_Frame = (frmoffset+SPR_JUMP_LJUMP2);
			}else
			{
				if (m_Frame<=(frmoffset+SPR_JUMP_LJUMP3))
				{
					m_Frame = (frmoffset+SPR_JUMP_LJUMP4);
				}else
				{
					if (m_Frame==(frmoffset+SPR_JUMP_LJUMP4))
					{
						m_Frame = (frmoffset+SPR_JUMP_LJUMP5);
					}else
					{
						m_Frame = (frmoffset+SPR_JUMP_LJUMP6);
					}
				}
			}
		}
	}

}


//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CJumpObj::Do( void )
{
	if (RunDoSuck())
	{
		m_FixFlag = 0;
		return;
	}
	if (CheckExtra()) return;
	if (m_ImmumeCnt>0)
	{
		m_ImmumeCnt--;
	}else
	{
		CheckKillPlayer();
	}

	if (m_JumpSize)	// Small Jumper?
	{
		DoJump(SPR_JUMP_SJUMP1 - SPR_JUMP_LJUMP1);
	}else
	{
		DoJump(0);
	}

	MoveObject();

}

//------------------------------------------------------------------------------
// Animation
//------------------------------------------------------------------------------

static int anm_mbug_left[] =
	{
	SPR_MBUG_LEFT1,SPR_MBUG_LEFT2,SPR_MBUG_LEFT3,ANM_RESTART
	};
static int anm_mbug_right[] =
	{
	SPR_MBUG_RIGHT1,SPR_MBUG_RIGHT2,SPR_MBUG_RIGHT3,ANM_RESTART
	};

//------------------------------------------------------------------------------
// Sucking in frames
//------------------------------------------------------------------------------

static SUCKANIM suck_mbug_right[NUMSUCKANIM] =
	{
		{SPR_MBUG_SUCKR1, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_MBUG_SUCKR2, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_MBUG_SUCKR3, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_MBUG_SUCKR4, 25+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_MBUG_SUCKR5, -7+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_MBUG_SUCKR6, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_MBUG_SUCKR6, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_MBUG_SUCKR6, -7+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_MBUG_SUCKR4, 25+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_MBUG_SUCKR3, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_MBUG_SUCKR2, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_MBUG_SUCKR1, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y}
	};
static SUCKANIM suck_mbug_left[NUMSUCKANIM] =
	{
		{SPR_MBUG_SUCKL1, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_MBUG_SUCKL2, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_MBUG_SUCKL3, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_MBUG_SUCKL4, -15+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_MBUG_SUCKL5,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_MBUG_SUCKL6,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_MBUG_SUCKL6,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_MBUG_SUCKL5,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_MBUG_SUCKL4, -15+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_MBUG_SUCKL3, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_MBUG_SUCKL2, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_MBUG_SUCKL1, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y}
	};
//------------------------------------------------------------------------------
// Sucking Roll frames
//------------------------------------------------------------------------------
static int suck_mbug_roll[8] =
	{
		SPR_MBUG_ROLL1,SPR_MBUG_ROLL2,SPR_MBUG_ROLL3,SPR_MBUG_ROLL4,
		SPR_MBUG_ROLL5,SPR_MBUG_ROLL6,SPR_MBUG_ROLL7,SPR_MBUG_ROLL8
	};

//------------------------------------------------------------------------------
// Sucking general frames
//------------------------------------------------------------------------------
static SUCKFRAMES suck_mbug_frames=
	{
		SPR_MBUG_HATCH1,SPR_MBUG_HATCH2,
		suck_mbug_roll, 8,
		suck_mbug_left, suck_mbug_right
	};
//------------------------------------------------------------------------------
// Object Details
//------------------------------------------------------------------------------
static OBJSIZE mbug_size = {6,2,22,24};
static OBJMOVE mbug_move_slow = {130,130/2,3*256,11*256,265,11*256,11*256};
static OBJMOVE mbug_move_fast = {0x282,130/2,8*256,11*256,265,11*256,11*256};

//------------------------------------------------------------------------------
//! \brief Initialise the mbug object
//------------------------------------------------------------------------------
CMBugObj::CMBugObj()
{
	Reset();
}

//------------------------------------------------------------------------------
//! \brief Destroy the mbug object
//------------------------------------------------------------------------------
CMBugObj::~CMBugObj()
{
	if (m_pWeapon)	// Attached to a weapon?
	{
		m_pWeapon->m_pBaddie= 0;	// Detach weapon
		m_pWeapon = 0;

	}
}

//------------------------------------------------------------------------------
//! \brief Draw the mbug object
//------------------------------------------------------------------------------
void CMBugObj::Draw( void )
{
	if (RunDrawSuck()) return;
	if (m_Frame) m_pGame->m_Sprites.Draw( m_Frame, m_XPos, m_YPos );
}


//------------------------------------------------------------------------------
//! \brief Load the object graphics
//------------------------------------------------------------------------------
void CMBugObj::LoadGfx( void )
{
	m_pGame->m_Sprites.LoadRange(SPR_MBUG_LEFT1, SPR_MBUG_BANG);
}

//------------------------------------------------------------------------------
//! \brief Reset the object (object members)
//------------------------------------------------------------------------------
void CMBugObj::Reset( void )
{
	m_DieScore = 1000;
	m_DieScoreFrame = SPR_NUM_1000;
	m_OldXPos = m_XPos = 100;
	m_OldYPos =	m_YPos = 100;
	m_Frame = SPR_MBUG_LEFT1;
	m_pSequence = anm_mbug_left;

	InitMovement(mbug_move_slow);
	InitSize(mbug_size);
	m_Y_Flag = -1;

	m_Flags = FLAG_CATCHABLE | FLAG_BADDIE | FLAG_HARDWALL;

	m_pSuckFrames = &suck_mbug_frames;
	m_pWeapon = 0;
}

//------------------------------------------------------------------------------
//! \brief Check to see if the weapon needs to be created
//------------------------------------------------------------------------------
void CMBugObj::CheckWeapon( void )
{
	if (!m_pWeapon)
	{
		SMB_NEW(m_pWeapon,CMBugWeapon);
		if (m_pWeapon)
		{
			m_pWeapon->m_pBaddie = this;
			m_pWeapon->m_OldXPos = m_pWeapon->m_XPos = m_XPos;
			m_pWeapon->m_OldYPos = m_pWeapon->m_YPos = m_YPos+20;
			m_pGame->m_BaddieList.Attach(m_pWeapon,OBJ_MBUGWEAPON, m_pGame);
		}
	}
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CMBugObj::Do( void )
{
	if (RunDoSuck()) return;
	ControlSpeedUp( mbug_move_slow, mbug_move_fast );
	CheckOil();
	if (CheckExtra()) return;

	CheckKillPlayer();
	if (!(m_Y_Flag))	// Only move if on ground
	{
		DirWalk();
	}
	MoveObject();
	AnimateXInert(anm_mbug_left, anm_mbug_right);

	CheckWeapon();

}

//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------
#define CLOWN_NORMAL 0
#define CLOWN_SHOOT 1

//------------------------------------------------------------------------------
// Animation
//------------------------------------------------------------------------------

static int anm_clown_left[] =
	{
	SPR_CLOWN_LEFT1,SPR_CLOWN_LEFT2,SPR_CLOWN_LEFT3,SPR_CLOWN_LEFT4,
	SPR_CLOWN_LEFT5,SPR_CLOWN_LEFT6,SPR_CLOWN_LEFT7,SPR_CLOWN_LEFT8,SPR_CLOWN_LEFT9,
	ANM_RESTART
	};
static int anm_clown_right[] =
	{
	SPR_CLOWN_RIGHT1,SPR_CLOWN_RIGHT2,
	SPR_CLOWN_RIGHT3,SPR_CLOWN_RIGHT4,SPR_CLOWN_RIGHT5,SPR_CLOWN_RIGHT6,
	SPR_CLOWN_RIGHT7,SPR_CLOWN_RIGHT8,SPR_CLOWN_RIGHT9,
	ANM_RESTART
	};

//------------------------------------------------------------------------------
// Sucking in frames
//------------------------------------------------------------------------------

static SUCKANIM suck_clown_right[NUMSUCKANIM] =
	{
		{SPR_CLOWN_SUCKR1, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_CLOWN_SUCKR2, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_CLOWN_SUCKR3, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_CLOWN_SUCKR4, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_CLOWN_SUCKR5, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_CLOWN_SUCKR6, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_CLOWN_SUCKR6, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_CLOWN_SUCKR6, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_CLOWN_SUCKR4, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_CLOWN_SUCKR3, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_CLOWN_SUCKR2, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_CLOWN_SUCKR1, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y}
	};
static SUCKANIM suck_clown_left[NUMSUCKANIM] =
	{
		{SPR_CLOWN_SUCKL1, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_CLOWN_SUCKL2, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_CLOWN_SUCKL3, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_CLOWN_SUCKL4, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_CLOWN_SUCKL5,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_CLOWN_SUCKL6,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_CLOWN_SUCKL6,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_CLOWN_SUCKL5,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_CLOWN_SUCKL4, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_CLOWN_SUCKL3, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_CLOWN_SUCKL2, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_CLOWN_SUCKL1, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y}
	};
//------------------------------------------------------------------------------
// Sucking Roll frames
//------------------------------------------------------------------------------
static int suck_clown_roll[8] =
	{
		SPR_CLOWN_ROLL1,SPR_CLOWN_ROLL2,SPR_CLOWN_ROLL3,SPR_CLOWN_ROLL4,
		SPR_CLOWN_ROLL5,SPR_CLOWN_ROLL6,SPR_CLOWN_ROLL7,SPR_CLOWN_ROLL8
	};

//------------------------------------------------------------------------------
// Sucking general frames
//------------------------------------------------------------------------------
static SUCKFRAMES suck_clown_frames=
	{
		SPR_CLOWN_HATCH1,SPR_CLOWN_HATCH2,
		suck_clown_roll, 8,
		suck_clown_left, suck_clown_right
	};
//------------------------------------------------------------------------------
// Object Details
//------------------------------------------------------------------------------
static OBJSIZE clown_size = {6,8,20,27};
static OBJMOVE clown_move_slow = {160,160/2,4*256,12*256,256,12*256,12*256};
static OBJMOVE clown_move_fast = {0x2a0,0x100,6*256,0xf*256,256,0xf*256,0xf*256};

//------------------------------------------------------------------------------
//! \brief Initialise the clown object
//------------------------------------------------------------------------------
CClownObj::CClownObj()
{
	Reset();
}

//------------------------------------------------------------------------------
//! \brief Destroy the clown object
//------------------------------------------------------------------------------
CClownObj::~CClownObj()
{
	if (m_pWeapon)	// Attached to a weapon?
	{
		m_pWeapon->m_pBaddie= 0;	// Detach weapon
		m_pWeapon = 0;
	}
}

//------------------------------------------------------------------------------
//! \brief Draw the clown object
//------------------------------------------------------------------------------
void CClownObj::Draw( void )
{
	if (RunDrawSuck()) return;
	m_pGame->m_Sprites.Draw( m_Frame, m_XPos, m_YPos );
}


//------------------------------------------------------------------------------
//! \brief Load the object graphics
//------------------------------------------------------------------------------
void CClownObj::LoadGfx( void )
{
	m_pGame->m_Sprites.LoadRange(SPR_CLOWN_RIGHT1, SPR_CLOWN_BAT8);
}

//------------------------------------------------------------------------------
//! \brief Reset the object (object members)
//------------------------------------------------------------------------------
void CClownObj::Reset( void )
{
	m_DieScore = 3000;
	m_DieScoreFrame = SPR_NUM_3000;
	m_OldXPos = m_XPos = 100;
	m_OldYPos =	m_YPos = 100;
	m_Frame = SPR_CLOWN_LEFT1;
	m_pSequence = anm_clown_left;
	m_Release_YOff = 8;

	InitMovement(clown_move_slow);
	InitSize(clown_size);
	m_Y_Flag = -1;

	m_Flags = FLAG_CATCHABLE | FLAG_BADDIE;

	m_pSuckFrames = &suck_clown_frames;

	m_ClownCmd = CLOWN_NORMAL;
	m_pWeapon = 0;
}
//------------------------------------------------------------------------------
//! \brief Check to see if the clown should shoot at the player
//!
//! \note (NOTE: Like the amiga version, this function does not take account of screen wrapping)
//!
//! 	\param play = player to test with
//!
//! 	\return 0 = Not going to shoot
//------------------------------------------------------------------------------
int CClownObj::CheckToShoot( CPlayerObj *play )
{
	int play_ypos;
	int ypos;

	if (!play) return 0;		// No player exists

	play_ypos = play->m_YPos - 8;

	play_ypos = play_ypos & (~0x3f);

	ypos = m_YPos & (~0x3f);

	if ( (play_ypos == ypos) )		// is player in the y range
	{
		if (m_Dir==DIR_LEFT)	// Facing the player?
		{
			if (m_XPos < play->m_XPos) return 0;
		}else
		{
			if (m_XPos > play->m_XPos) return 0;
		}

		SMB_NEW(m_pWeapon,CClownWeapon);
		if (m_pWeapon)
		{
			m_ClownCmd = CLOWN_SHOOT;
			m_pWeapon->m_pBaddie = this;
			m_pWeapon->m_OldXPos = m_pWeapon->m_XPos = m_XPos;
			m_pWeapon->m_OldYPos = m_pWeapon->m_YPos = m_YPos;
			m_pWeapon->m_Dir = m_Dir;
			m_pGame->m_BaddieList.Attach(m_pWeapon,OBJ_CLOWNWEAPON, m_pGame);
		}

		return 1;
	}
	return 0;
}

//------------------------------------------------------------------------------
//! \brief Walk the clown object
//------------------------------------------------------------------------------
void CClownObj::WalkClown( void )
{
	CPlayerObj *pobj;

	if (!(m_Y_Flag))	// Only move if on ground
	{
		DirWalk();
		Jump2Player( 31, 8 );
	}
	MoveObject();
	AnimateXInert(anm_clown_left, anm_clown_right);

	if (!m_Y_Flag)		// Only fire if on ground
	{
		if (!(rand()&0x1f))	// 1/32 Chance to throw
		{
			pobj = (CPlayerObj *) m_pGame->m_PlayerList.m_pFirst;
			while (pobj)
			{
				if (CheckToShoot(pobj)) break;
				pobj = (CPlayerObj *) pobj->m_pNext;
			}
		}
	}
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CClownObj::Do( void )
{
	if (RunDoSuck()) return;
	ControlSpeedUp( clown_move_slow, clown_move_fast );
	CheckOil();
	if (CheckExtra()) return;

	CheckKillPlayer();
	switch (m_ClownCmd)
	{
		case (CLOWN_NORMAL):
			WalkClown();
			break;
		case (CLOWN_SHOOT):
			if (!m_pWeapon) m_ClownCmd = CLOWN_NORMAL;
			break;
	}
}

//------------------------------------------------------------------------------
// Animation
//------------------------------------------------------------------------------

static int anm_dwarf_left[] =
	{
	SPR_DWARF_LEFT1,SPR_DWARF_LEFT2,SPR_DWARF_LEFT3,SPR_DWARF_LEFT4,
	SPR_DWARF_LEFT5,SPR_DWARF_LEFT6,
	ANM_RESTART
	};
static int anm_dwarf_right[] =
	{
	SPR_DWARF_RIGHT1,SPR_DWARF_RIGHT2,
	SPR_DWARF_RIGHT3,SPR_DWARF_RIGHT4,SPR_DWARF_RIGHT5,SPR_DWARF_RIGHT6,
	ANM_RESTART
	};

//------------------------------------------------------------------------------
// Sucking in frames
//------------------------------------------------------------------------------

static SUCKANIM suck_dwarf_right[NUMSUCKANIM] =
	{
		{SPR_DWARF_SUCKR1, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_DWARF_SUCKR2, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_DWARF_SUCKR3, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_DWARF_SUCKR4, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_DWARF_SUCKR5,  4+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_DWARF_SUCKR6, -2+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_DWARF_INGUN,   3+ SUCKFIX_LX,  0+	SUCKFIX_Y},
		{SPR_DWARF_SUCKR6,  4+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_DWARF_SUCKR4, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_DWARF_SUCKR3, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_DWARF_SUCKR2, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_DWARF_SUCKR1, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y}
	};
static SUCKANIM suck_dwarf_left[NUMSUCKANIM] =
	{
		{SPR_DWARF_SUCKL1, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_DWARF_SUCKL2, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_DWARF_SUCKL3, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_DWARF_SUCKL4, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_DWARF_SUCKR5, -10+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_DWARF_SUCKR6,  -5+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_DWARF_INGUN,    7+ SUCKFIX_RX,  0+	SUCKFIX_Y},
		{SPR_DWARF_SUCKR5, -10+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_DWARF_SUCKL4, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_DWARF_SUCKL3, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_DWARF_SUCKL2, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_DWARF_SUCKL1, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y}
	};
//------------------------------------------------------------------------------
// Sucking Roll frames
//------------------------------------------------------------------------------
static int suck_dwarf_roll[8] =
	{
		SPR_DWARF_ROLL1,SPR_DWARF_ROLL2,SPR_DWARF_ROLL3,SPR_DWARF_ROLL4,
		SPR_DWARF_ROLL5,SPR_DWARF_ROLL6,SPR_DWARF_ROLL7,SPR_DWARF_ROLL8
	};

//------------------------------------------------------------------------------
// Sucking general frames
//------------------------------------------------------------------------------
static SUCKFRAMES suck_dwarf_frames=
	{
		SPR_DWARF_HATCH1,SPR_DWARF_HATCH2,
		suck_dwarf_roll, 8,
		suck_dwarf_left, suck_dwarf_right
	};
//------------------------------------------------------------------------------
// Object Details
//------------------------------------------------------------------------------
static OBJSIZE dwarf_size = {6,4,22,30};
static OBJMOVE dwarf_move_slow = {365,260,4*256,11*256,265,11*256,11*256};
static OBJMOVE dwarf_move_fast = {0x200,0x104,8*256,11*256,265,11*256,11*256};

//------------------------------------------------------------------------------
//! \brief Initialise the dwarf object (Including allocating the hammer object)
//------------------------------------------------------------------------------
CDwarfObj::CDwarfObj()
{
	m_pWeapon = 0;
	Reset();
}

//------------------------------------------------------------------------------
//! \brief Destroy the dwarf object
//------------------------------------------------------------------------------
CDwarfObj::~CDwarfObj()
{
	if (m_pWeapon)	// Attached to a weapon?
	{
		m_pWeapon->m_pBaddie= 0;	// Detach weapon
		m_pWeapon->DeleteObject();		// Delete the weapon
		m_pWeapon = 0;
	}
}

//------------------------------------------------------------------------------
//! \brief Draw the dwarf object
//------------------------------------------------------------------------------
void CDwarfObj::Draw( void )
{
	if (RunDrawSuck()) return;

	if (m_pWeapon)			// Draw baddie if throwing
	{
		if (m_pWeapon->m_Frame)
		{
			m_pGame->m_Sprites.Draw( m_Frame, m_XPos, m_YPos );
			return;
		}
	}
	if (m_Dir==DIR_LEFT)
	{
		m_pGame->m_Sprites.Draw( m_Frame, m_XPos, m_YPos );
		m_pGame->m_Sprites.Draw( SPR_DWARF_HAM_HAND, m_XPos+5, m_YPos );
	}else
	{
		m_pGame->m_Sprites.Draw( SPR_DWARF_HAMMER8, m_XPos-5, m_YPos );
		m_pGame->m_Sprites.Draw( m_Frame, m_XPos, m_YPos );
	}
}

//------------------------------------------------------------------------------
//! \brief Load the object graphics
//------------------------------------------------------------------------------
void CDwarfObj::LoadGfx( void )
{
	m_pGame->m_Sprites.LoadRange(SPR_DWARF_RIGHT1, SPR_DWARF_INGUN);
}

//------------------------------------------------------------------------------
//! \brief Reset the object (object members)
//------------------------------------------------------------------------------
void CDwarfObj::Reset( void )
{
	m_DieScore = 2000;
	m_DieScoreFrame = SPR_NUM_2000;
	m_OldXPos = m_XPos = 100;
	m_OldYPos =	m_YPos = 100;
	m_Frame = SPR_DWARF_LEFT1;
	m_pSequence = anm_dwarf_left;
	m_Release_YOff = 8;

	InitMovement(dwarf_move_slow);
	InitSize(dwarf_size);
	m_Y_Flag = -1;

	m_Flags = FLAG_CATCHABLE | FLAG_BADDIE;

	m_pSuckFrames = &suck_dwarf_frames;
}

//------------------------------------------------------------------------------
//! \brief Check to see if the dwarf should shoot at the player
//!
//! \note (NOTE: Like the amiga version, this function does not take account of screen wrapping)
//!
//! 	\param play = player to test with
//!
//! 	\return 0 = Not going to shoot
//------------------------------------------------------------------------------
int CDwarfObj::CheckToShoot( CPlayerObj *play )
{
	int play_ypos;
	int ypos;

	if (!play) return 0;		// No player exists

	play_ypos = play->m_YPos - 8;

	play_ypos = play_ypos & (~0x3f);

	ypos = m_YPos & (~0x3f);

	if ( (play_ypos == ypos) )		// is player in the y range
	{
		if (m_Dir==DIR_LEFT)	// Facing the player?
		{
			if (m_XPos < play->m_XPos) return 0;
		}else
		{
			if (m_XPos > play->m_XPos) return 0;
		}

		if (m_pWeapon)
		{
			m_pWeapon->InitThrow(m_XPos, m_YPos, m_Dir);
		}

		return 1;
	}
	return 0;
}

//------------------------------------------------------------------------------
//! \brief Walk the dwarf
//------------------------------------------------------------------------------
void CDwarfObj::WalkDwarf( void )
{
	CPlayerObj *pobj;

	if (!(m_Y_Flag))	// Only move if on ground
	{
		DirWalk();
		Jump2Player( 16, 8 );
	}
	MoveObject();
	AnimateXInert(anm_dwarf_left, anm_dwarf_right);
	if (!m_Y_Flag)		// Only fire if on ground
	{
		if (!(rand()&0x1f))	// 1/32 Chance to throw
		{
			pobj = (CPlayerObj *) m_pGame->m_PlayerList.m_pFirst;
			while (pobj)
			{
				if (CheckToShoot(pobj)) break;
				pobj = (CPlayerObj *) pobj->m_pNext;
			}
		}
	}
}

//------------------------------------------------------------------------------
//! \brief Setup the object getting caught
//------------------------------------------------------------------------------
void CDwarfObj::SetupCaught( void )
{
	CSuckable::SetupCaught();	// Must call virtual member
 	if (m_pWeapon)
	{
		if (m_pWeapon->m_Command == DWEAPON_GO)
		{
			m_pWeapon->DropWeapon();
		}
	}
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CDwarfObj::Do( void )
{

	// The below cannot be called in the constructor because m_pGame does not exist
	if (!m_pWeapon)
	{
		SMB_NEW(m_pWeapon,CDwarfWeapon);
		if (m_pWeapon)
		{
			m_pWeapon->m_pBaddie = this;
			m_pGame->m_BaddieList.Attach(m_pWeapon,OBJ_DWARFWEAPON, m_pGame);
		}
	}

	if (RunDoSuck())
	{
		return;
	}
	ControlSpeedUp( dwarf_move_slow, dwarf_move_fast );
	CheckOil();
	if (CheckExtra()) return;
	CheckKillPlayer();

	if (m_pWeapon)				// Wait for the weapon to finish throwing
	{
		if (m_pWeapon->m_Command == DWEAPON_GO) return;
	}
	WalkDwarf();

}

//------------------------------------------------------------------------------
// Animation
//------------------------------------------------------------------------------

static int anm_zoom_left[] =
	{
	SPR_ZOOM_LEFT1,SPR_ZOOM_LEFT2,SPR_ZOOM_LEFT3,
	SPR_ZOOM_LEFT4,SPR_ZOOM_LEFT5,SPR_ZOOM_LEFT6,
	ANM_RESTART
	};
static int anm_zoom_right[] =
	{
	SPR_ZOOM_RIGHT1,SPR_ZOOM_RIGHT2,SPR_ZOOM_RIGHT3,
	SPR_ZOOM_RIGHT4,SPR_ZOOM_RIGHT5,SPR_ZOOM_RIGHT6,
	ANM_RESTART
	};

//------------------------------------------------------------------------------
// Sucking in frames
//------------------------------------------------------------------------------

SUCKANIM suck_zoom_right[NUMSUCKANIM] =
	{
		{SPR_ZOOM_SUCKR1, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_ZOOM_SUCKR2, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_ZOOM_SUCKR3, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_ZOOM_SUCKR4, 25+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_ZOOM_SUCKR5, -7+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_ZOOM_SUCKR6, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_ZOOM_SUCKR6, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_ZOOM_SUCKR6, -7+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_ZOOM_SUCKR4, 25+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_ZOOM_SUCKR3, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_ZOOM_SUCKR2, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_ZOOM_SUCKR1, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y}
	};
SUCKANIM suck_zoom_left[NUMSUCKANIM] =
	{
		{SPR_ZOOM_SUCKL1, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_ZOOM_SUCKL2, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_ZOOM_SUCKL3, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_ZOOM_SUCKL4, -15+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_ZOOM_SUCKL5,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_ZOOM_SUCKL6,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_ZOOM_SUCKL6,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_ZOOM_SUCKL5,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_ZOOM_SUCKL4, -15+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_ZOOM_SUCKL3, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_ZOOM_SUCKL2, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_ZOOM_SUCKL1, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y}
	};
//------------------------------------------------------------------------------
// Sucking Roll frames
//------------------------------------------------------------------------------
static int suck_zoom_roll[8] =
	{
		SPR_ZOOM_ROLL1,SPR_ZOOM_ROLL2,SPR_ZOOM_ROLL3,SPR_ZOOM_ROLL4,
		SPR_ZOOM_ROLL5,SPR_ZOOM_ROLL6,SPR_ZOOM_ROLL7,SPR_ZOOM_ROLL8
	};

//------------------------------------------------------------------------------
// Sucking general frames
//------------------------------------------------------------------------------
static SUCKFRAMES suck_zoom_Frames=
	{
		SPR_ZOOM_HATCH1,SPR_ZOOM_HATCH2,
		suck_zoom_roll, 8,
		suck_zoom_left, suck_zoom_right
	};
//------------------------------------------------------------------------------
// Object Details
//------------------------------------------------------------------------------
static OBJSIZE zoom_size = {6,2,22,24};
static OBJMOVE zoom_move_slow = {240,240/2,9*256,11*256,565,11*256,11*256};
static OBJMOVE zoom_move_fast = {0x3f0,240/2,9*256,11*256,565,11*256,11*256};

//------------------------------------------------------------------------------
//! \brief Initialise the zoom object
//------------------------------------------------------------------------------
CZoomObj::CZoomObj()
{
	Reset();
}

//------------------------------------------------------------------------------
//! \brief Draw the zoom object
//------------------------------------------------------------------------------
void CZoomObj::Draw( void )
{
	if (RunDrawSuck()) return;
	if (m_Frame) m_pGame->m_Sprites.Draw( m_Frame, m_XPos, m_YPos );
}

//------------------------------------------------------------------------------
//! \brief Load the object graphics
//------------------------------------------------------------------------------
void CZoomObj::LoadGfx( void )
{
	m_pGame->m_Sprites.LoadRange(SPR_ZOOM_LEFT1, SPR_ZOOM_SUCKR6);
}

//------------------------------------------------------------------------------
//! \brief Reset the object (object members)
//------------------------------------------------------------------------------
void CZoomObj::Reset( void )
{
	m_DieScore = 1000;
	m_DieScoreFrame = SPR_NUM_1000;
	m_OldXPos = m_XPos = 100;
	m_OldYPos =	m_YPos = 100;
	m_Frame = SPR_ZOOM_LEFT1;
	m_pSequence = anm_zoom_left;

	InitMovement(zoom_move_slow);
	InitSize(zoom_size);
	m_Y_Flag = -1;

	m_Flags = FLAG_CATCHABLE | FLAG_BADDIE;

	m_pSuckFrames = &suck_zoom_Frames;
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CZoomObj::Do( void )
{
	if (RunDoSuck()) return;
	ControlSpeedUp( zoom_move_slow, zoom_move_fast );
	CheckOil();
	if (CheckExtra()) return;
	CheckKillPlayer();

	if (!(m_Y_Flag))	// Only move if on ground
	{
		DirWalk();
	}
	MoveObject();
	AnimateXInert(anm_zoom_left, anm_zoom_right);
}

//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------

#define SPIKE_ON_GROUND 80		// Max delay of Spike on the ground

//------------------------------------------------------------------------------
// Animation
//------------------------------------------------------------------------------
static int spike_left_frms[6] = {
	SPR_SPIKE_LEFT1,SPR_SPIKE_LEFT2,SPR_SPIKE_LEFT3,SPR_SPIKE_LEFT4,
	SPR_SPIKE_LEFT5,SPR_SPIKE_LEFT4
	};

static int spike_right_frms[6] = {
	SPR_SPIKE_RIGHT1,SPR_SPIKE_RIGHT2,SPR_SPIKE_RIGHT3,SPR_SPIKE_RIGHT4,
	SPR_SPIKE_RIGHT5,SPR_SPIKE_RIGHT4
	};

//------------------------------------------------------------------------------
// Sucking in frames
//------------------------------------------------------------------------------

SUCKANIM suck_spike_right[NUMSUCKANIM] =
	{
		{SPR_SPIKE_SUCKR1, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_SPIKE_SUCKR2, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_SPIKE_SUCKR3, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_SPIKE_SUCKR4, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_SPIKE_SUCKR5, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_SPIKE_SUCKR6, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_SPIKE_SUCKR6, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_SPIKE_SUCKR6, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_SPIKE_SUCKR4, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_SPIKE_SUCKR3, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_SPIKE_SUCKR2, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_SPIKE_SUCKR1, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y}
	};
SUCKANIM suck_spike_left[NUMSUCKANIM] =
	{
		{SPR_SPIKE_SUCKL1, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_SPIKE_SUCKL2, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_SPIKE_SUCKL3, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_SPIKE_SUCKL4, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_SPIKE_SUCKL5,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_SPIKE_SUCKL6,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_SPIKE_SUCKL6,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_SPIKE_SUCKL5,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_SPIKE_SUCKL4, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_SPIKE_SUCKL3, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_SPIKE_SUCKL2, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_SPIKE_SUCKL1, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y}
	};
//------------------------------------------------------------------------------
// Sucking Roll frames
//------------------------------------------------------------------------------
static int suck_spike_roll[8] =
	{
		SPR_SPIKE_ROLL1,SPR_SPIKE_ROLL2,SPR_SPIKE_ROLL3,SPR_SPIKE_ROLL4,
		SPR_SPIKE_ROLL5,SPR_SPIKE_ROLL6,SPR_SPIKE_ROLL7,SPR_SPIKE_ROLL8
	};

//------------------------------------------------------------------------------
// Sucking general frames
//------------------------------------------------------------------------------
static SUCKFRAMES suck_spike_frames=
	{
		SPR_SPIKE_HATCH1,SPR_SPIKE_HATCH2,
		suck_spike_roll, 8,
		suck_spike_left, suck_spike_right
	};
//------------------------------------------------------------------------------
// Object Details
//------------------------------------------------------------------------------
static OBJSIZE spike_size = {6,8,21,38};
static OBJMOVE spike_move = {130,130/2,7*256,9*256,296,9*256,9*256};
static OBJMOVE spike_movebig = {130,130/2,7*256,12*256,195,12*256,12*256};

//------------------------------------------------------------------------------
//! \brief Initialise the spike object
//------------------------------------------------------------------------------
CSpikeObj::CSpikeObj()
{
	Reset();
}

//------------------------------------------------------------------------------
//! \brief Draw the spike object
//------------------------------------------------------------------------------
void CSpikeObj::Draw( void )
{
	if (RunDrawSuck()) return;
	if (m_Frame) m_pGame->m_Sprites.Draw( m_Frame, m_XPos, m_YPos );
}

//------------------------------------------------------------------------------
//! \brief Setup a spike object - when ejected from the crabboss
//!
//! 	\param dir = the direction (DIR_LEFT,DIR_RIGHT)
//!	\param xpos = the x position
//!	\param ypos = the y position
//------------------------------------------------------------------------------
void CSpikeObj::Setup( int dir, int xpos, int ypos )
{
	m_Dir = dir;
	m_XPos = m_OldXPos = xpos;
	m_YPos = m_OldYPos = ypos-16;
	m_YInert = -8*256;
}

//------------------------------------------------------------------------------
//! \brief Load the object graphics
//------------------------------------------------------------------------------
void CSpikeObj::LoadGfx( void )
{
	m_pGame->m_Sprites.LoadRange(SPR_SPIKE_LEFT1, SPR_SPIKE_SUCKR6);
}

//------------------------------------------------------------------------------
//! \brief Reset the object (object members)
//------------------------------------------------------------------------------
void CSpikeObj::Reset( void )
{
	m_DieScore = 4000;
	m_DieScoreFrame = SPR_NUM_4000;
	m_OldXPos = m_XPos = 100;
	m_OldYPos =	m_YPos = 100;
	m_Frame = SPR_SPIKE_LEFT1;
	m_Release_YOff = 20;

	InitMovement(spike_move);
	InitSize(spike_size);
	m_Y_Flag = -1;

	m_Flags = FLAG_CATCHABLE | FLAG_BADDIE;

	m_pSuckFrames = &suck_spike_frames;
	m_Length = 0;
}

//------------------------------------------------------------------------------
//! \brief Do the spike on the ground (AN EXACT AMIGA CONVERSION)
//------------------------------------------------------------------------------
void CSpikeObj::SpikeOnGround( void )
{

	InitMovement(spike_move);

	if (m_Seq_Offset<2)	// is landing?
	{
		if (m_Seq_Offset==1) m_Seq_Offset = 0;
		else m_Seq_Offset = 3;
	}else
	{
		if (m_pGame->m_MainCounter&1)	// A animation delay
		{
			if (m_Seq_Offset==2) m_Seq_Offset = 3;
			else if (m_Seq_Offset==3) m_Seq_Offset = 4;
			else if (m_Seq_Offset==4) m_Seq_Offset = 5;
			else m_Seq_Offset = 2;
		}
		m_Length += rand() & 7;	// Add on a random length on the ground
		if (m_Length>=SPIKE_ON_GROUND)
		{
			m_Length = 0;
			if (!(rand()&3))	// Random big jump
			{
				InitMovement(spike_movebig);
			}
			SetMoveUp();
		}
	}
}

//------------------------------------------------------------------------------
//! \brief Do the spike in the air (AN EXACT AMIGA CONVERSION)
//------------------------------------------------------------------------------
void CSpikeObj::SpikeInAir( void )
{
	if (m_Seq_Offset >= 2) m_Seq_Offset = 0;
	else m_Seq_Offset = 1;
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CSpikeObj::Do( void )
{
	if (RunDoSuck()) return;
	if (CheckExtra()) return;

	CheckKillPlayer();
	if (!m_Y_Flag)		// Not in the air?
	{
		SpikeOnGround();
	}else				// In the Air
	{
		SpikeInAir();
		MoveObject();
	}

	if (m_Y_Flag)		// In the Air
	{
		DirWalk();		// We are walking in the air...
	}

	if (m_Dir==DIR_LEFT)
	{
		m_Frame = spike_left_frms[m_Seq_Offset];
	}else
	{
		m_Frame = spike_right_frms[m_Seq_Offset];
	}

}

//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------
#define SUCK_DIR_LEFT	0
#define SUCK_DIR_RIGHT	1
#define SUCK_DIR_UP	2
#define SUCK_DIR_DOWN	3

#define SUCK_SPEED 2

#define SUCK_FIX	3	// A fix, so that the sucker sticks to the wall

//------------------------------------------------------------------------------
// Animation
//------------------------------------------------------------------------------

static int anm_sucker[] =
	{
	SPR_SUCKER_MOVE1,SPR_SUCKER_MOVE2,SPR_SUCKER_MOVE3,
	ANM_RESTART
	};

//------------------------------------------------------------------------------
// Sucking in frames
//------------------------------------------------------------------------------

SUCKANIM suck_sucker_right[NUMSUCKANIM] =
	{
		{SPR_SUCKER_SUCKR1, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_SUCKER_SUCKR2, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_SUCKER_SUCKR3, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_SUCKER_SUCKR4, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_SUCKER_SUCKR5, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_SUCKER_SUCKR6, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_SUCKER_SUCKR6, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_SUCKER_SUCKR6, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_SUCKER_SUCKR4, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_SUCKER_SUCKR3, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_SUCKER_SUCKR2, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_SUCKER_SUCKR1, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y}
	};
SUCKANIM suck_sucker_left[NUMSUCKANIM] =
	{
		{SPR_SUCKER_SUCKL1, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_SUCKER_SUCKL2, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_SUCKER_SUCKL3, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_SUCKER_SUCKL4, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_SUCKER_SUCKL5,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_SUCKER_SUCKL6,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_SUCKER_SUCKL6,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_SUCKER_SUCKL5,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_SUCKER_SUCKL4, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_SUCKER_SUCKL3, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_SUCKER_SUCKL2, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_SUCKER_SUCKL1, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y}
	};
//------------------------------------------------------------------------------
// Sucking Roll frames
//------------------------------------------------------------------------------
static int suck_sucker_roll[4] =
	{
		SPR_SUCKER_MOVE2,SPR_SUCKER_ROLL1,SPR_SUCKER_ROLL2,SPR_SUCKER_ROLL3
	};

//------------------------------------------------------------------------------
// Sucking general frames
//------------------------------------------------------------------------------
static SUCKFRAMES suck_sucker_frames=
	{
		SPR_SUCKER_HATCH1,SPR_SUCKER_HATCH2,
		suck_sucker_roll, 4,
		suck_sucker_left, suck_sucker_right
	};
//------------------------------------------------------------------------------
// Object Details
//------------------------------------------------------------------------------
static OBJSIZE sucker_size = {4,2,0x19,0x19};
static OBJMOVE sucker_move = {130,130/2,1,9*256,296,9*256,9*256};

//------------------------------------------------------------------------------
//! \brief Initialise the sucker object
//------------------------------------------------------------------------------
CSuckerObj::CSuckerObj()
{
	Reset();
}

//------------------------------------------------------------------------------
//! \brief Draw the sucker object
//------------------------------------------------------------------------------
void CSuckerObj::Draw( void )
{
	if (RunDrawSuck()) return;
	if (m_Frame) m_pGame->m_Sprites.Draw( m_Frame, m_XPos, m_YPos );
}

//------------------------------------------------------------------------------
//! \brief Load the object graphics
//------------------------------------------------------------------------------
void CSuckerObj::LoadGfx( void )
{
	m_pGame->m_Sprites.LoadRange(SPR_SUCKER_MOVE1, SPR_SUCKER_SUCKR6);
}

//------------------------------------------------------------------------------
//! \brief Reset the object (object members)
//------------------------------------------------------------------------------
void CSuckerObj::Reset( void )
{
	m_DieScore = 2000;
	m_DieScoreFrame = SPR_NUM_2000;
	m_OldXPos = m_XPos = 100;
	m_OldYPos = m_YPos = 100;
	m_Frame = SPR_SUCKER_MOVE1;
	m_pSequence = anm_sucker;

	InitMovement(sucker_move);
	InitSize(sucker_size);
	m_Y_Flag = -1;

	m_Flags = FLAG_CATCHABLE | FLAG_BADDIE | FLAG_HARDWALL;

	m_pSuckFrames = &suck_sucker_frames;
	m_Direction = SUCK_DIR_DOWN;
}

//------------------------------------------------------------------------------
//! \brief Move the sucker right
//------------------------------------------------------------------------------
void CSuckerObj::SuckMoveRight( void )
{
	int rcode;
	m_XPos += m_Max_XSpeed;
	rcode = CheckRight();
	if (rcode!=CBM_NOTINWALL)
	{
		m_XPos = rcode;
		m_Direction = SUCK_DIR_UP;
	}else
	{
		// Test to see if there is a wall below
		m_YPos += SUCK_FIX;
		rcode = CheckDown();
		m_YPos -= SUCK_FIX;
		if (rcode==CBM_NOTINWALL)
		{
			m_Direction = SUCK_DIR_DOWN;
		}
	}
}

//------------------------------------------------------------------------------
//! \brief Move the sucker left
//------------------------------------------------------------------------------
void CSuckerObj::SuckMoveLeft( void )
{
	int rcode;
	m_XPos -= m_Max_XSpeed;
	rcode = CheckLeft();
	if (rcode!=CBM_NOTINWALL)
	{
		m_XPos = rcode;
		m_Direction = SUCK_DIR_DOWN;
	}else
	{
		// Test to see if there is a wall above
		m_YPos -= SUCK_FIX;
		rcode = CheckUp();
		m_YPos += SUCK_FIX;
		if (rcode==CBM_NOTINWALL)
		{
			m_XPos&=~1;	// A fix - I don't know why it works!!!
			m_Direction = SUCK_DIR_UP;
		}
	}
}

//------------------------------------------------------------------------------
//! \brief Move the sucker down
//------------------------------------------------------------------------------
void CSuckerObj::SuckMoveDown( void )
{
	int rcode;
	m_YPos += m_Max_XSpeed;
	rcode = CheckDown();
	if (rcode!=CBM_NOTINWALL)
	{
		m_YPos = rcode;
		m_Direction = SUCK_DIR_RIGHT;
		m_Max_XSpeed = SUCK_SPEED;
	}else
	{
		// Test to see if there is a wall left
		m_XPos -= SUCK_FIX;
		rcode = CheckLeft();
		m_XPos += SUCK_FIX;
		if (rcode==CBM_NOTINWALL)	// Not Hit?
		{
			// Test to see if there is wall left/up
			m_Max_XSpeed = SUCK_SPEED*2;
			m_XPos -= SUCK_FIX;
			m_YPos -= SUCK_FIX;
			rcode = CheckLeft();
			m_YPos += SUCK_FIX;
			m_XPos += SUCK_FIX;
			if (rcode!=CBM_NOTINWALL)	// Hit?
			{
				m_Direction = SUCK_DIR_LEFT;
				m_Max_XSpeed = SUCK_SPEED;
			}
		}else
		{
			m_Max_XSpeed = SUCK_SPEED;
		}
	}
}

//------------------------------------------------------------------------------
//! \brief Move the sucker up
//------------------------------------------------------------------------------
void CSuckerObj::SuckMoveUp( void )
{
	int rcode;
	m_YPos -= m_Max_XSpeed;
	rcode = CheckUp();
	if (rcode!=CBM_NOTINWALL)
	{
		m_YPos = rcode;
		m_Direction = SUCK_DIR_LEFT;
	}else
	{
		// Test to see if there is a wall right
		m_XPos += SUCK_FIX;
		rcode = CheckRight();
		m_XPos -= SUCK_FIX;
		if (rcode==CBM_NOTINWALL)
		{
			m_Direction = SUCK_DIR_RIGHT;
		}
	}
}

//------------------------------------------------------------------------------
//! \brief Do the sucker movement
//------------------------------------------------------------------------------
void CSuckerObj::MoveSucker( void )
{
	m_OldXPos = m_XPos;
	m_OldYPos = m_YPos;

	switch (m_Direction)
	{
		case (SUCK_DIR_RIGHT):
		{
			SuckMoveRight();
			break;
		}
		case (SUCK_DIR_LEFT):
		{
			SuckMoveLeft();
			break;
		}
		case (SUCK_DIR_DOWN):
		{
			SuckMoveDown();
			break;
		}
		case (SUCK_DIR_UP):
		{
			SuckMoveUp();
			break;
		}

	}
	::CheckPos(m_XPos, m_YPos);
	
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CSuckerObj::Do( void )
{
	if (RunDoSuck()) return;
	if (CheckExtra()) return;
	CheckKillPlayer();

	MoveSucker();

	Animate(256);
}



