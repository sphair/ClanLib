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
// Methane Brothers Suckable object (Source File)
//------------------------------------------------------------------------------

#include "suck.h"
#include "global.h"
#include "bitgroup.h"
#include "maps.h"
#include "objtypes.h"
#include "game.h"
#include "player.h"
#include "goodie.h"

//------------------------------------------------------------------------------
// NOTE: FOR THIS CLASS TO WORK CORRECTLY, YOU MUST SET THE
// m_pSuckFrames TO A VALID STRUCTURE ON INITIALISATION
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Offset defines
//------------------------------------------------------------------------------
#define BADDIESUCKRATE		(16)
#define ROLLSTEPRATE		(11)

//------------------------------------------------------------------------------
// Standard Collision Sizes
//------------------------------------------------------------------------------
#define STD_WIDTH		24
#define STD_HEIGHT		24

//------------------------------------------------------------------------------
//! \brief Initialise the suckable object
//------------------------------------------------------------------------------
CSuckable::CSuckable()
{
	Reset();
}

//------------------------------------------------------------------------------
//! \brief Reset the object (object members)
//------------------------------------------------------------------------------
void CSuckable::Reset( void )
{
	m_pSuckFrames = 0;
	m_SFrame = 0;
	m_SuckCmd = SUCK_NONE;
	m_Release_YOff = 0;
	m_DieScore = m_DieScoreFrame = 0;
	m_pPlayer = 0;
}

//------------------------------------------------------------------------------
//! \brief Setup the object getting caught
//------------------------------------------------------------------------------
void CSuckable::SetupCaught( void )
{
	m_SFrame = m_pSuckFrames->rollanimseq[0];
	m_Y_Flag = -1;
	m_YInert = 256;
	m_XInert = 0;
}

//------------------------------------------------------------------------------
//! \brief Setup the object getting sucked up to the gun
//------------------------------------------------------------------------------
void CSuckable::SetupSuck( void )
{
	// Get baddie x,y position and turn into a fraction
	m_BadXPosFract = m_XPos<<16;
	m_BadYPosFract = m_YPos<<16;
	m_SuckFrmCnt = 0;

}

//------------------------------------------------------------------------------
//! \brief Suck a baddie to the gun
//!
//! 	\param gun_xpos = Where to go to X
//! 	\param gun_ypos = Where to go to Y
//!
//! 	\return 0 = Still Sucking. Else = At the gun
//------------------------------------------------------------------------------
int CSuckable::SuckBaddie( int gun_xpos, int gun_ypos )
{
	int bad_xpos,bad_ypos;
	int delta_xpos,delta_ypos;
	int tmp_xpos, tmp_ypos;

	bad_xpos = m_BadXPosFract>>16;	// Get propper position
	bad_ypos = m_BadYPosFract>>16;
	::CheckPos(bad_xpos, bad_ypos);	// Check on screen
	::CheckPos(gun_xpos, gun_ypos);

	delta_xpos = gun_xpos-bad_xpos;	// Get difference
	delta_ypos = gun_ypos-bad_ypos;

// Check to see if the baddie is at the destination

	tmp_xpos = delta_xpos;
	tmp_ypos = delta_ypos;
	if (tmp_xpos <0) tmp_xpos = -tmp_xpos;
	if (tmp_ypos <0) tmp_ypos = -tmp_ypos;

	if ((tmp_xpos <=(BADDIESUCKRATE+1)) &&
		(tmp_ypos <=(BADDIESUCKRATE+1)) )
	{
		m_XPos = gun_xpos;
		m_YPos = gun_ypos;
		m_SuckFrmCnt = 0;
		return 1; 
	}

// Calculate fastest way of getting to the player

	if (delta_xpos<0)				// Negative X
	{
		if (delta_xpos<(-(SCR_WIDTH/2)))	// Too far left?
		{
			delta_xpos+=SCR_WIDTH;		// Make positive
		}
	}else
	{
		if (delta_xpos>((SCR_WIDTH/2)))		// Too far right?
		{
			delta_xpos-=SCR_WIDTH;			// Make negative
		}
	}

	if (delta_ypos<0)				// Negative Y
	{
		if (delta_ypos<(-(SCR_HEIGHT/2)))	// Too far up?
		{
			delta_ypos+=SCR_HEIGHT;		// Make positive
		}
	}else
	{
		if (delta_ypos>((SCR_HEIGHT/2)))		// Too far down?
		{
			delta_ypos-=SCR_HEIGHT;			// Make negative
		}
	}

// Make the deltas positive

	tmp_xpos = delta_xpos;
	tmp_ypos = delta_ypos;
	if (tmp_xpos <0) tmp_xpos = -tmp_xpos;
	if (tmp_ypos <0) tmp_ypos = -tmp_ypos;

// Find which movement formula to use and use it

	if (tmp_xpos >= tmp_ypos)				// Which delta to use?
	{
		if (delta_xpos<0)
			m_BadXPosFract-=BADDIESUCKRATE<<16;	// Move the baddie xpos
		else m_BadXPosFract+=(BADDIESUCKRATE<<16);

		if (!tmp_xpos) tmp_xpos=1;		// No divide by zero
		m_BadYPosFract+=(( (delta_ypos*BADDIESUCKRATE) << 16 )/tmp_xpos);

	}else
	{
		if (delta_ypos<0)
			m_BadYPosFract-=BADDIESUCKRATE<<16;	// Move the baddie ypos
		else m_BadYPosFract+=BADDIESUCKRATE<<16;

		if (!tmp_ypos) tmp_ypos=1;		// No divide by zero
		m_BadXPosFract+=( (delta_xpos*BADDIESUCKRATE) << 16 )/tmp_ypos;

	}
	bad_xpos = m_BadXPosFract>>16;	// Get propper position
	bad_ypos = m_BadYPosFract>>16;
	::CheckPos(bad_xpos, bad_ypos);	// Check on screen

	m_XPos = bad_xpos;
	m_YPos = bad_ypos;

	if (delta_xpos>=0)		// Roll the suck animation
	{
		m_SuckFrmCnt++;
		if (m_SuckFrmCnt>= m_pSuckFrames->numrollanim)
			m_SuckFrmCnt = 0;

	}else
	{
		m_SuckFrmCnt--;
		if (m_SuckFrmCnt<0)
		m_SuckFrmCnt = m_pSuckFrames->numrollanim - 1;
	}
	m_SFrame = m_pSuckFrames->rollanimseq[m_SuckFrmCnt];
	return 0;

}
//------------------------------------------------------------------------------
//! \brief Suck a baddie inside the gun.
//!
//! Note m_SuckFrmCnt should be '0', the  first time this function is called
//!
//! 	\param gun_xpos = Gun offset X
//! 	\param gun_ypos = Gun offset Y
//!	\param dir = player facing direction
//------------------------------------------------------------------------------
void CSuckable::EnterBaddie( int gun_xpos, int gun_ypos, int dir )
{
	SUCKANIM *sanim;
	SUCKANIM *sframe;

	if (dir==DIR_LEFT)
		sanim = m_pSuckFrames->leftsuckframes;
	else
		sanim = m_pSuckFrames->rightsuckframes;

	if (m_SuckFrmCnt>=NUMSUCKANIM)
	{
		m_SuckFrmCnt = 0;
	}

	sframe = &(sanim[m_SuckFrmCnt]);

	m_SFrame = sframe->frame;
	m_XPos = gun_xpos+sframe->xoffset;
	m_YPos = gun_ypos+sframe->yoffset;

}

//------------------------------------------------------------------------------
//! \brief Draw the baddie being sucked
//------------------------------------------------------------------------------
void CSuckable::DrawSuck( void )
{
	if (m_SFrame) m_pGame->m_Sprites.Draw( m_SFrame, m_XPos, m_YPos );
}

//------------------------------------------------------------------------------
//! \brief Set a baddie rolling out of a gun
//!
//! 	\param dir = direction of roll
//!	\param rollsize = roll distance
//------------------------------------------------------------------------------
void CSuckable::SetRoll( int dir, int rollsize )
{
	m_Dir = dir;			// Copy direction
	m_SuckCmd = SUCK_ROLL;
	m_SFrame = m_pSuckFrames->rollanimseq[0];
	m_Flags = m_Flags & (~FLAG_CATCHABLE);	// Prevent catching while rolling
	m_RollCounter = rollsize;
	m_SuckFrmCnt = 0;

	CBitmapItem *pitem;

	pitem = m_pGame->m_Sprites.GetItem(m_SFrame);
	if (pitem)
	{
		m_RollCol_Width = pitem->m_Width;
		m_RollCol_Height = pitem->m_Height;
		m_RollCol_XOff = pitem->m_XOff;
		m_RollCol_YOff = pitem->m_YOff;
	}else
	{
		m_RollCol_Width = STD_WIDTH;
		m_RollCol_Height = STD_HEIGHT;
		m_RollCol_XOff = m_RollCol_YOff = 0;
	}

	m_YPos = m_YPos - m_RollCol_Height - m_RollCol_YOff + 24;

	if (TestRollWall())
	{
		SetExplode();
	}
}

//------------------------------------------------------------------------------
//! \brief Set the baddie to explode into million of toys
//!
//! The object will be deleted
//------------------------------------------------------------------------------
void CSuckable::SetExplode( void )
{
	int cnt;

	m_YPos = m_YPos - 6;

	if (m_DieScore)
	{
		if (m_pPlayer) m_pPlayer->AddScore( m_DieScore );
		m_pGame->MakeNumRise( m_XPos, m_YPos, m_DieScoreFrame );
	}

	if (m_Type==OBJ_JUMP)	// Special Case?
	{
		m_pGame->SetJumpExplode(m_XPos, m_YPos, m_Dir);
	}else
	{
		for (cnt=0; cnt<4; cnt++)
			m_pGame->RandGoodie(m_XPos, m_YPos, GOODIE_TOYS, m_Dir);
	}

	DeleteObject();

}

//------------------------------------------------------------------------------
//! \brief Draw the suck frames
//!
//! 	\return 0 = No Suck frames to draw\n
//!		Else = The suck frames have been drawn
//------------------------------------------------------------------------------
int CSuckable::RunDrawSuck( void )
{
	if (!m_SuckCmd) return 0;	// Not Drawing
	if (m_SuckCmd==SUCK_GLIDE)
	{
		if (m_Frame) m_pGame->m_Sprites.Draw( m_Frame, m_XPos, m_YPos, GFX_NOWRAP );
	}else
	{
		DrawSuck();				// Draw the suck frames
	}
	return 1;
}

//------------------------------------------------------------------------------
//! \brief Test the rolling baddie against the wall
//!
//! xpos will be set to the wall edge if hit
//!
//! 	\return 0 = Wall not hit
//------------------------------------------------------------------------------
int CSuckable::TestRollWall( void )
{
	BOXPOS *xp;

	if (m_Dir==DIR_LEFT)
	{
		xp = m_pGame->m_Map.CheckHitWall(m_XPos+m_RollCol_XOff, m_YPos+m_RollCol_YOff,
			m_RollCol_Width+16, m_RollCol_Height);
	}else
	{
		xp = m_pGame->m_Map.CheckHitWall(m_XPos+m_RollCol_XOff, m_YPos+m_RollCol_YOff,
			m_RollCol_Width, m_RollCol_Height);
	}


	if (xp)			// Collision occured
	{
		if (m_Dir==DIR_LEFT)
		{
			m_XPos = m_pGame->m_Map.GetRightEdge(xp);
		}else
		{
			m_XPos = m_pGame->m_Map.GetLeftEdge(xp); //-m_x2;
			m_XPos = m_XPos - m_X1 - m_X2;
		}
		m_pGame->m_pSound->PlaySample(SND_SPLAT, m_XPos);
		return 1;
	}
	return 0;
}


//------------------------------------------------------------------------------
//! \brief Do the rolling code
//------------------------------------------------------------------------------
void CSuckable::DoSuckRoll( void )
{

	if (m_Dir==DIR_LEFT)
	{
		m_XPos -= ROLLSTEPRATE;
		m_SuckFrmCnt--;
		if (m_SuckFrmCnt<0)
			m_SuckFrmCnt = m_pSuckFrames->numrollanim - 1;
	}else
	{
		m_XPos += ROLLSTEPRATE;
		m_SuckFrmCnt++;
		if (m_SuckFrmCnt>= m_pSuckFrames->numrollanim)
			m_SuckFrmCnt = 0;
	}
	m_SFrame = m_pSuckFrames->rollanimseq[m_SuckFrmCnt];

	if (TestRollWall())
	{
		// the below does not work when set explode is overrided
		// so only allow with baddie objects (Bad assumption!!!)
		if (m_Flags&FLAG_BADDIE)
		{
			//m_pGame->CreateCloud( m_XPos, m_YPos);
			if (m_Dir==DIR_RIGHT) m_XPos += (m_RollCol_XOff + m_RollCol_Width);
			m_XPos-=8;
		}
		SetExplode();
		return;
	}
	::CheckPos(m_XPos,m_YPos);
	m_RollCounter--;
	if (m_RollCounter<0)	// Roll Complete
	{
		m_YPos -= m_Release_YOff;
		ReleaseFromRoll();
		return;
	}

}
//------------------------------------------------------------------------------
//! \brief Release the baddie from the rolling state - to it's normal state
//------------------------------------------------------------------------------
void CSuckable::ReleaseFromRoll( void )
{
	m_SuckCmd = SUCK_NONE;
	m_Flags = m_Flags | (FLAG_CATCHABLE);	// Allow catching
	m_OldXPos = m_XPos;
	m_OldYPos = m_YPos;
	m_State |= STATE_ANGRY;
}

//------------------------------------------------------------------------------
//! \brief Do the suck code
//!
//! 	\return 0 = No Suck code present\n
//!		Else = The suck code has been complete
//------------------------------------------------------------------------------
int CSuckable::RunDoSuck( void )
{
	if (!m_SuckCmd) return 0;	// No Command

	switch (m_SuckCmd)
	{
		case (SUCK_ROLL):
			DoSuckRoll();
			break;
		case (SUCK_GLIDE):
			DoGlide();
			break;
	}

	return 1;
}

//------------------------------------------------------------------------------
//! \brief Check the baddie hitting oil - If it has, slow it down
//!
//! (Note: The baddies movement MUST be reset before this function is called
//------------------------------------------------------------------------------
void CSuckable::CheckOil( void )
{
	if (CheckHitFlag( m_pGame->m_GoodieList, FLAG_STICKY ))
	{
		m_XPower = m_XPower >> 2;
		m_XMass = m_XMass >> 2;
	}

}

//------------------------------------------------------------------------------
//! \brief Check the baddie hitting a players weapon (IE water)
//!
//! 	\return 0 = Okay. Else Baddie was deleted
//------------------------------------------------------------------------------
int CSuckable::CheckExtra(void)
{
	CLinkObject *obj;
	obj = CheckHitFlag( m_pGame->m_GoodieList, FLAG_WEAPON );
	if (obj)
	{
		obj->m_State |= STATE_HITBADDIE;
		SetExplode();
		return 1;
	}
	CPlayerObj *pptr;
	pptr = (CPlayerObj *) m_pGame->m_PlayerList.m_pFirst;
	while(pptr)
	{
		if (pptr->m_Command == PCMD_NORMAL)
		{
			if (pptr->m_Mask)
			{
				if (pptr->m_Dir != m_Dir)	// Facing in different directions?
				{
					m_Dir = pptr->m_Dir;	// Turn the object around
				}
			}
		}
		pptr = (CPlayerObj *) pptr->m_pNext;
	}
	return 0;
}

//------------------------------------------------------------------------------
//! \brief Prepare the baddie gliding down the screen in level initialisation
//------------------------------------------------------------------------------
void CSuckable::PrepareGlide(void)
{
	m_SuckCmd = SUCK_GLIDE;
	m_GlideDest = m_YPos;
	m_YPos -= SCR_HEIGHT;
	m_Flags = m_Flags & (~FLAG_CATCHABLE);
}

//------------------------------------------------------------------------------
//! \brief Do the baddie gliding down the screen
//------------------------------------------------------------------------------
void CSuckable::DoGlide(void)
{
	int offset;

	offset = m_GlideDest - m_YPos;
	offset = offset >> 2;
  	if (offset<4) offset = 4;
	if (offset>8) offset = 8;

	m_YPos += offset;

	if (m_YPos>= m_GlideDest)
	{
		m_YPos= m_GlideDest;
		m_Flags = m_Flags | FLAG_CATCHABLE;
		m_SuckCmd = SUCK_NONE;
		
	}

}


