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
// Methane Brothers Bug Object (Header File)
//------------------------------------------------------------------------------

#ifndef _Baddie_h
#define _Baddie_h

#include "objlist.h"
#include "suck.h"

class CPlayerObj;

extern SUCKANIM suck_bug_left[NUMSUCKANIM];
extern SUCKANIM suck_bug_right[NUMSUCKANIM];

class CBugObj : public CSuckable
{
public:
	CBugObj();
	virtual void Do( void );
	virtual void Draw( void );
	void LoadGfx( void );
	void Reset( void );
};


class CWhirlyObj : public CSuckable
{
public:
	CWhirlyObj();
	virtual void Do( void );
	virtual void Draw( void );
	void LoadGfx( void );
	void Reset( void );
public:
};


class CDoofusObj : public CSuckable
{
public:
	CDoofusObj();
	virtual void Do( void );
	virtual void Draw( void );
	void LoadGfx( void );
	void Reset( void );
public:
};


class CJumpObj : public CSuckable
{
public:
	CJumpObj();
	virtual void Do( void );
	virtual void Draw( void );
	void LoadGfx( void );
	void Reset( void );
	void DoJump( int frmoffset );
public:
	int m_FixFlag;
	int m_JumpSize;
	int m_ImmumeCnt;
};

extern OBJSIZE jump_size ;
extern OBJMOVE jump_move ;
extern OBJSIZE jump_smallsize ;

extern SUCKFRAMES suck_jump_frames;
extern SUCKFRAMES suck_jump_smallframes;


class CMBugWeapon;

class CMBugObj : public CSuckable
{
public:
	CMBugObj();
	virtual ~CMBugObj();
	virtual void Do( void );
	virtual void Draw( void );
	void LoadGfx( void );
	void Reset( void );
	void CheckWeapon( void );
public:
	CMBugWeapon *m_pWeapon;
};

class CClownWeapon;
class CClownObj : public CSuckable
{
public:
	CClownObj();
	virtual ~CClownObj();
	virtual void Do( void );
	virtual void Draw( void );
	void LoadGfx( void );
	void Reset( void );
	void WalkClown( void );
	int CheckToShoot( CPlayerObj *play );
public:
	CClownWeapon *m_pWeapon;
	int m_ClownCmd;
};

class CDwarfWeapon;
class CDwarfObj : public CSuckable
{
public:
	CDwarfObj();
	virtual ~CDwarfObj();
	virtual void Do( void );
	virtual void Draw( void );
	void LoadGfx( void );
	void Reset( void );
	void WalkDwarf( void );
	int CheckToShoot( CPlayerObj *play );
	virtual void SetupCaught( void );
public:
	CDwarfWeapon *m_pWeapon;
};

extern SUCKANIM suck_zoom_right[NUMSUCKANIM];
extern SUCKANIM suck_zoom_left[NUMSUCKANIM];
class CZoomObj : public CSuckable
{
public:
	CZoomObj();
	virtual void Do( void );
	virtual void Draw( void );
	void LoadGfx( void );
	void Reset( void );
};


class CSpikeObj : public CSuckable
{
public:
	CSpikeObj();
	virtual void Do( void );
	virtual void Draw( void );
	void LoadGfx( void );
	void Reset( void );
	void SpikeInAir( void );
	void SpikeOnGround( void );
	void Setup( int dir, int xpos, int ypos );
public:
	int m_Length;
};

extern SUCKANIM suck_spike_right[NUMSUCKANIM];
extern SUCKANIM suck_spike_left[NUMSUCKANIM];
extern SUCKANIM suck_sucker_left[NUMSUCKANIM];
extern SUCKANIM suck_sucker_right[NUMSUCKANIM];

class CSuckerObj : public CSuckable
{
public:
	CSuckerObj();
	virtual void Do( void );
	virtual void Draw( void );
	void LoadGfx( void );
	void Reset( void );
	void MoveSucker( void );
	void SuckMoveRight( void );
	void SuckMoveLeft( void );
	void SuckMoveDown( void );
	void SuckMoveUp( void );
public:
	int m_Direction;
};


#endif // _Baddie_h

