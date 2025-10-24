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
// Methane Brothers Boss Object (Header File)
//------------------------------------------------------------------------------

#ifndef _BOSSOBJ_h
#define _BOSSOBJ_h

#include "objlist.h"
#include "suck.h"

class CTomatoObj;
class CBowlingObj;

class CClownBoss : public CLinkObject
{
public:
	CClownBoss();
	virtual void Do( void );
	virtual void Draw( void );
	void LoadGfx( void );
	void CollisionCheck( void );
	void TomatoCollision( void );
	int HitBody( CTomatoObj *tobj, OBJSIZE *pcb );
	int HitHead( CTomatoObj *tobj, OBJSIZE *pcb );
public:
	int m_MouthCounter;
	int m_WhiteFlag;
	int m_Command;
	int m_OuchCount;
	int m_DamageCnt;
};

class CSpikeObj;

class CCrabBoss : public CLinkObject
{
public:
	CCrabBoss();
	virtual void Do( void );
	virtual void Draw( void );
	void LoadGfx( void );
	void CollisionCheck( void );
	void DoPattern( void );
	void PatDump( void );
	void PatRight( int destpos );
	void PatLeft( int destpos );
	void PatDown( int destpos );
	void PatUp( int destpos );
	int HitBody( CSpikeObj *tobj, OBJSIZE *pcb );
	int HitHead( CSpikeObj *tobj, OBJSIZE *pcb );
	void BossCollision( void );
public:
	int m_ClawCounter;
	int m_WhiteFlag;
	int m_Command;
	int m_OuchCount;
	int m_DamageCnt;
	int m_Pattern;
	int m_PipeCmd;
	int m_PipeOffset;
	int m_LumpOffset;
	int m_EjectDir;
	int m_EjectCnt;
	int m_EjectFrame;
	int m_EjectXOff;
	int m_EjectYOff;

};

typedef struct _CLKFRAMES
	{
		int frame;
		int xpos;
		int ypos;
	} CLKFRAMES;


class CClockBoss : public CLinkObject
{
public:
	CClockBoss();
	virtual void Do( void );
	virtual void Draw( void );
	void LoadGfx( void );
	void CollisionCheck( void );
	void BallCollision( void );
	int HitBody( CBowlingObj *tobj, OBJSIZE *pcb );
	int HitHead( CBowlingObj *tobj, OBJSIZE *pcb );
	void ControlHands( void );
public:
	int m_WhiteFlag;
	int m_Command;
	int m_OuchCount;
	int m_DamageCnt;
	int m_HandCntS;
	int m_HandCntB;
	CLKFRAMES *m_pFrmS;
	CLKFRAMES *m_pFrmB;
};

class CEndBoss;
class CEndBoss : public CSuckable
{
public:
	CEndBoss();
	virtual ~CEndBoss();
	virtual void Do( void );
	virtual void Draw( void );
	void LoadGfx( void );
	void Setup( int seg );
	virtual void SetupCaught( void );
	void ControlDay( void );
	virtual void SetExplode( void );
public:
	int	m_Segment;
	CEndBoss *m_pLastSeg;
	CEndBoss *m_pNextSeg;
	int m_HatFrame;
	int m_BombPause;
};

class CKillBossObj : public CLinkObject
{
public:
	CKillBossObj();
	virtual void Do( void );
	virtual void Draw( void );
public:
	int m_BossNumber;
	int m_Counter;
};


#endif // _BOSSOBJ_h


