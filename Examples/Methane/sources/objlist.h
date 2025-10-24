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
// Methane Brothers Object lists (Header File)
//------------------------------------------------------------------------------

#ifndef _objlist_h
#define _objlist_h

//------------------------------------------------------------------------------
// Defines used for the objects
//------------------------------------------------------------------------------
#define DIR_LEFT	0
#define DIR_RIGHT	1

//------------------------------------------------------------------------------
// Defines used for the animation
//------------------------------------------------------------------------------
#define ANM_RESTART	0
#define ANM_PAUSE		1

#define ANMFLG_NORESTART 1

//------------------------------------------------------------------------------
// Defines used for the m_Flags (Bitmask)
//------------------------------------------------------------------------------
#define FLAG_CATCHABLE	1	// Object can be caught by a gas cloud (ie baddie)
#define FLAG_SUCKABLE	2	// Object can be sucked (ie gas)
#define FLAG_BADDIE	4	// Object must be destroyed before level completion
#define FLAG_HARDWALL	8	// Object cannot jump though thin walls
#define FLAG_STICKY	16	// Object is sticky (ie OIL)
#define FLAG_WEAPON	32	// Object can kill baddies (ie WATER)
#define FLAG_ACCURATE	64	// Accurate when falling and pushing left (ie PLAYER)

//------------------------------------------------------------------------------
// State flags (bit masks)
// 0 = False, else true
//------------------------------------------------------------------------------
#define STATE_LEFT		1	// Hit wall to the left
#define STATE_RIGHT		2	// Hit wall to the right
#define STATE_HORIZ		(1+2)	// Hit left or right wall
#define STATE_UP		4	// Hit wall to the above
#define STATE_DOWN		8	// Hit wall to the below
#define STATE_VERT		(4+8)	// Hit above or below wall
#define STATE_ANM_RESTART	16	// Animation restarted
#define STATE_ANGRY		32	// The object is angry (ie baddie)
#define STATE_HITBADDIE		64	// This object hit a baddie
//------------------------------------------------------------------------------
// Object size structure
//------------------------------------------------------------------------------
typedef struct _OBJSIZE
	{
		int x1;
		int y1;
		int x2;
		int y2;
	} OBJSIZE;

//------------------------------------------------------------------------------
// Object movement structure
//------------------------------------------------------------------------------
typedef struct _OBJMOVE
	{
		int xpower;
		int xmass;
		int max_xspeed;
		int ypower;
		int ymass;
		int max_yspeed;
		int ycentre;
	} OBJMOVE;

typedef struct _BOXPOS BOXPOS;

class CObjectList;
class CLinkObject;
class CPlayerObj;
class CGame;
class CLinkObject
{
public:
	CLinkObject();
	virtual ~CLinkObject();
	void UnLink( void );
	void Link( void );
	void LinkBefore( CLinkObject *linkto );
	void LinkAfter( CLinkObject *linkto );
	void Reset( void );
	void DeleteObject(void);
	virtual void Do(void);
	virtual void Draw(void);
	void Animate( int rate );
	void AnimateDir( int *seq_left, int *seq_right, int rate, int flags = 0 );
	void AnimateXInert( int *seq_left, int *seq_right, int scale = 2 );
	void SetAnim(int *seq_ptr);
	void SetMoveLeft( void );
	void SetMoveRight( void );
	void SetMoveUp( void );
	void SetMoveDown( void );
	void MoveObject( void );
	int CheckDown( void );
	int CheckUp( void );
	int CheckLeft( void );
	int CheckRight( void );
	BOXPOS *CheckHitWall( void );
	int CheckHit( CLinkObject *pobj );
	int CheckHit(int b_x1,int b_y1, int b_width, int b_height );
	void DoXCheck( void );
	void DoYCheck( void );
	void DoXInertia( void );
	void DoYInertia( void );
	void InitSize( OBJSIZE &zsize );
	void InitMovement( OBJMOVE &zmove );
	void DirWalk( void );
	void Jump2Player( int rndmask, int yoff );
	CPlayerObj* HitPlayer( int immune_flag = 0 );
	CPlayerObj* CheckKillPlayer( void );
	void MoveObject_Y( void );
	void DirFly( void );
	void GetFly2Player( CPlayerObj *play, int *pdiff_x, int *pdiff_y );

	int ControlSpeedUp( OBJMOVE &slow_move, OBJMOVE &fast_move );
	CLinkObject *CheckHitFlag( CObjectList &base, int flag );
	void Fly2ClosestPlayer( void );
public:
	CLinkObject *m_pNext;
	CLinkObject *m_pPrev;
	CObjectList *m_pListBase;
	int	m_Type;			// Object Id

	int	m_XPos;			// Object Position
	int	m_YPos;
	int	m_OldXPos;
	int	m_OldYPos;

	int	m_Dir;			// Facing Direction
	int	m_YDir;

	int	m_Frame;		// SPR_xxx sprite id

	int	m_XInert;		// Inertia settings
	int	m_XPower;
	int	m_XMass;
	int	m_Max_XSpeed;
	int	m_YInert;
	int	m_YPower;
	int	m_YMass;
	int	m_Max_YSpeed;
	int	m_YCentre;

	char	m_Y_Flag;

	int	m_Delay;		// Animation delay counter
	int	*m_pSequence;		// Animation pointer
	int	m_Seq_Offset;		// Offset into the animation

	int	m_X1;			// Collision sizes
	int	m_Y1;
	int	m_X2;
	int	m_Y2;

	int	m_State;		// Object state flags
	int	m_Flags;		// Object type flags

	CGame	*m_pGame;
};

//------------------------------------------------------------------------------
// Main Object List class
//------------------------------------------------------------------------------
class CObjectList
{
public:
	CObjectList();
	~CObjectList();
	void DeleteAll( void );
	void Attach(CLinkObject *objptr, int type, CGame *gptr );
	void DoAll( void );
	void DrawAll( void );
	void SetAnim(int *seq_ptr);
	CLinkObject *FindFirst(int type);
	CLinkObject *m_pFirst;
};


#endif // _objlist_h




