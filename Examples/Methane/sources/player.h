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

#ifndef _player_h
#define _player_h

#include "global.h"
#include "objlist.h"

//------------------------------------------------------------------------------
// m_PlayerFlags Flags (BITMASK)
//------------------------------------------------------------------------------
#define PF_IS_ALIVE	1
#define PF_IS_IMMUNE	2

//------------------------------------------------------------------------------
// Player Command Types
//------------------------------------------------------------------------------
#define PCMD_NORMAL	0	// Normal Player (During Game)
#define PCMD_DIE	1	// Player is dying
#define PCMD_RISEUP	2	// Player is escaping the level

#define PNUM_DELAY	4	// How for the suck number counter position is behind by

//------------------------------------------------------------------------------
// Graphic frames structure
//------------------------------------------------------------------------------
typedef struct _PLAYER_GFX_FRAMES {
	// Anim frames
	int *pfrm_left;
	int *pfrm_right;
	int *pfrm_stand_left;
	int *pfrm_stand_right;

	// Gun frames
	int frm_gun_left1;
	int frm_gun_right1;

	int frm_gun_left2;
	int frm_gun_right2;

	// Gun handle frames
	int frm_handle_left;
	int frm_handle_right;

	// Misc frames
	int frm_face_right;
	int frm_hit;
	int frm_ouch;

	// Grapple frames
	int frm_grapple_left;
	int frm_grapple_right;
	int frm_grapple_rope;
	int frm_grapple_hook;

	// Offsets
	int player_y_offset;
	int grapple_left_x_offset;
	int grapple_right_x_offset;

	} PLAYER_GFX_FRAMES;

class CSuckable;
class CPlayerObj : public CLinkObject
{
public:
	CPlayerObj();
	virtual ~CPlayerObj();
	virtual void Draw( void );
	void DrawNormal( void );
	void DoNormal( void );
	void DrawDie( void );
	void DoDie( void );
	void LoadGfx( void );
	void Reset( void );
	void ResetAll( void );
	void ResetStd( void );
	virtual void Do( void );
	void MovePlayer( void );
	void ControlAnim( void );
	void GunAnim( int old_inertx, int old_inerty );
	void ControlGun( void );
	void EjectGas(void);
	void CheckSuck( void );
	void SuckBaddie( void );
	void EnterBaddie( void );
	void SetupSuckBaddie( void );
	void CalcSuckGun( int &gun_xpos, int &gun_ypos );
	void NewLevel( void );
	void DeleteBaddie( void );
	void KillPlayer( void );
	void ReleaseBaddie( void );
	void DoRiseUp( void );
	void SetRiseUp( void );
	void DrawRiseUp( void );
	void AddScore( int value );
	void CheckExtraLife( void );
	void TogglePuffBlow( void );
	void SetPlayerName(const char *nptr);

private:
	void InitSuck(CSuckable *pnearest);
	CSuckable *CheckClose(CSuckable *pobj1,CSuckable *pobj2, int xpos);
	void ControlTimer( void );
	void InitTimer( void );
	JOYSTICK *GetJoy( void );
public:
	PLAYER_GFX_FRAMES *m_pGFX;
	char	m_StandPause;
	int	m_BlowFlag;
	int	m_GunFrame;
	int	m_HandleFrame;
	int	m_GunXPos;
	int	m_GunYPos;
	int	m_HandleXPos;
	int	m_HandleYPos;
	int	m_GunInertX;
	int	m_GunInertY;
	int	m_PlungeOffset;
	int	m_PlungeStepRate;
	int	m_PlungeSuckerRate;
	int	m_PlungeSpitFlag;
	char	m_GunFlag;
	int	m_GasLength;
	int	m_GasStep;
	CSuckable *m_pBaddie;
	unsigned int m_Score;
	unsigned int m_ExtraLifeCount;
	int	m_Lives;
	int	m_Cards;
	int	m_PlayerFlags;
	int	m_OrigXPos;
	int	m_OrigYPos;
	int	m_Immune;
	int	m_Command;
	int	m_AtTopFlag;
	int	m_GasFireFlag;
	int	m_GiddyFlag;
	int	m_Mask;
	int	m_MaskOffset;
	int	m_NoDieCnt;
	int	m_EggFlag;
	int	m_EggOffset;
	int	m_EggFrame;
	char	m_Name[8];
private:
	int	m_DeadPause;
	int	m_GrappleXPos;
	int	m_GrappleYPos;
	int	m_GoingUpFlag;

	int	m_NumFrame;
	int	m_NumXPos;
	int	m_NumYPos;
	int	m_NumXOff;
	int	m_NumYOff;
	int	m_HoldCnt;

	int	m_NumXList[PNUM_DELAY];
	int	m_NumYList[PNUM_DELAY];
};

#endif // _player_h




