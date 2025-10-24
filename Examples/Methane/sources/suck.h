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
// Methane Brothers Suckable object (Header File)
//------------------------------------------------------------------------------

#ifndef _Suckable_h
#define _Suckable_h

#include "objlist.h"

//------------------------------------------------------------------------------
// NOTE: FOR THIS CLASS TO WORK CORRECTLY, YOU MUST SET THE
// m_suckframes TO A VALID STRUCTURE ON INITIALISATION
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// The below number MUST be the array size of SUCKANIM
//------------------------------------------------------------------------------
#define NUMSUCKANIM 12
typedef struct _SUCKANIM {
	int frame;
	int xoffset;
	int yoffset;
	} SUCKANIM;

//------------------------------------------------------------------------------
// The suck graphics structure
//------------------------------------------------------------------------------
typedef struct _SUCKFRAMES {
	int hatch1;
	int hatch2;
	int *rollanimseq;
	int numrollanim;
	SUCKANIM *leftsuckframes;
	SUCKANIM *rightsuckframes;
	} SUCKFRAMES;


//------------------------------------------------------------------------------
// The below offsets used used to fix the AMIGA suck offsets (which sucked (HO HO HO)!!!)
//------------------------------------------------------------------------------
#define SUCKFIX_LX	-18
#define SUCKFIX_RX	28
#define SUCKFIX_Y	6

//------------------------------------------------------------------------------
// Suck Commands
//------------------------------------------------------------------------------
#define SUCK_NONE	0
#define SUCK_ROLL	1
#define SUCK_GLIDE	2

class CGasObj;
class CSuckable : public CLinkObject
{
public:
	CSuckable();
	void Reset( void );
	virtual void SetupCaught( void );
	void DrawSuck( void );
	void SetupSuck( void );
	int SuckBaddie( int gun_xpos, int gun_ypos );
	void EnterBaddie( int gun_xpos, int gun_ypos, int dir );
	void SetRoll( int dir, int rollsize );
	int RunDrawSuck( void );
	int RunDoSuck( void );
	void DoSuckRoll( void );
	int TestRollWall( void );
	virtual void SetExplode( void );
	void ReleaseFromRoll( void );
	void CheckOil( void );
	int CheckExtra(void);
	void PrepareGlide(void);
	void DoGlide(void);
public:
	SUCKFRAMES	*m_pSuckFrames;
	CPlayerObj	*m_pPlayer;	//!< Player that sucked this object
	int	m_SFrame;
	int	m_BadXPosFract;
	int	m_BadYPosFract;
	int	m_SuckFrmCnt;
	int	m_SuckCmd;
	int	m_RollCounter;

	int	m_RollCol_XOff;
	int	m_RollCol_YOff;
	int	m_RollCol_Width;
	int	m_RollCol_Height;
	int	m_Release_YOff;

	int	m_DieScore;
	int	m_DieScoreFrame;
private:
	int	m_GlideDest;
};

#endif // _Suckable_h

