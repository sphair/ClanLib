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
// Methane Brothers Misc Objects (Header File)
//------------------------------------------------------------------------------

#ifndef _MISCOBJ_h
#define _MISCOBJ_h

#include "objlist.h"

//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------
#define PLAY_YOFF	4	// Status bar offsets
#define CREDIT_XOFF (160-4)	// Number of credits left xpos
#define CARD_YOFF	(0xeb)	// Card y offset

#define NUM_STD_LEVELS	109

extern const char *level_text_names[NUM_STD_LEVELS];

//------------------------------------------------------------------------------
// Status Bar Offsets Structure
//------------------------------------------------------------------------------
typedef struct _PLAYER_STATUS {
	int pxup;
	int pxoff;
	int scorefrm;
	int scorexoff;
	int livesxoff;
	int cardxoff;
	int cardxdelta;
	} PLAYER_STATUS;

//------------------------------------------------------------------------------
// The End Credits
//------------------------------------------------------------------------------

typedef struct _PARTYOFFS
{
	int	frame;
	int	xoffset;
	int	yoffset;
} PARTYOFFS;

typedef struct _ENDGROUP
{
	int	x_gfxoffset;
	PARTYOFFS	**party;
	int	y_textoffset;
	const char	*text;
} ENDGROUP;

extern ENDGROUP	*end_items[];

class CNumRiseObj : public CLinkObject
{
public:
	CNumRiseObj();
	virtual void Do( void );
	virtual void Draw( void );
};

extern PLAYER_STATUS pstat1;
extern PLAYER_STATUS pstat2;

class CRFontObj : public CLinkObject
{
public:
	CRFontObj();
	virtual void Do( void );
	virtual void Draw( void );
	void SetRiser( int frame, int xpos, int yoffset, int delay );
private:
	void DoCmdUp1(void);
	void DoCmdUp2(void);
	void DoCmdWobble(void);
public:
	int m_Command;
};

#define NUM_FONT_TABLE 64
extern int font_table[NUM_FONT_TABLE];

class CTubeObj : public CLinkObject
{
public:
	CTubeObj();
	void LoadGfx( void );
};

class CRealDayObj : public CLinkObject
{
public:
	CRealDayObj();
	void Setup( int xpos, int ypos );
	virtual void Draw( void );
	virtual void Do( void );
};


class CDayObj : public CLinkObject
{
public:
	CDayObj();
	virtual void Do( void );
	void Setup(int xpos, int ypos);
public:
};

class CCardObj : public CLinkObject
{
public:
	CCardObj();
	virtual void Do( void );
	int GetCardMask( void );
public:
	int m_CardType;
};


class COilObj : public CLinkObject
{
public:
	COilObj();
	virtual void Do( void );
	void Setup(int xpos, int ypos);
public:
	int m_Offset;
	int m_Fix;
};

class CWaterObj : public CLinkObject
{
public:
	CWaterObj();
	virtual void Do( void );
	void Setup(int xpos, int ypos);
public:
	int m_Fix;
};

class CFireObj : public CLinkObject
{
public:
	CFireObj();
	virtual void Do( void );
	void Setup(int xpos, int ypos);
public:
	int m_Offset;
	int m_Fix;
};


class CMarbleObj : public CLinkObject
{
public:
	CMarbleObj();
	virtual void Do( void );
	void Setup(int xpos, int ypos);
public:
};

class CNoteManObj : public CLinkObject
{
public:
	CNoteManObj();
	virtual void Do( void );
	void LoadGfx(void);
	void ReleaseNotes( void );
public:
	int m_ReleaseDelay;
	int m_TickTock;
	int m_LevelLength;
	int m_MusicOffset;
	int m_MusicDelay;
};

class CTreasSpotObj : public CLinkObject
{
public:
	CTreasSpotObj();
	virtual void Do( void );
public:
};

class CCardRoomObj : public CLinkObject
{
public:
	CCardRoomObj();
	virtual void Do( void );
	virtual void Draw( void );
	void LoadGfx(void);
public:
	int m_FrameOffset1;
	int m_FrameOffset2;
	int m_FrameOffset3;
	int m_FrameOffset4;
	int m_Counter;
};

class CSwitchObj : public CLinkObject
{
public:
	CSwitchObj();
	virtual void Do( void );
public:
	int m_HitFlag;
};


class CKeyRoomObj : public CLinkObject
{
public:
	CKeyRoomObj();
	virtual void Do( void );
	virtual void Draw( void );
	void LoadGfx(void);
	void Setup( int keytype );
public:
	int m_Counter;
	int m_KeyType;
};

class CDropOffObj : public CLinkObject
{
public:
	CDropOffObj();
	virtual void Do( void );
	virtual void Draw( void );
public:
};

class CBalloonObj : public CLinkObject
{
public:
	CBalloonObj();
	virtual void Do( void );
	virtual void Draw( void );
public:
	int	m_Rate;
};

#endif // _MISCOBJ_h

