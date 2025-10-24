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
// Methane Brothers Maps Control (Header File)
//------------------------------------------------------------------------------

#ifndef _maps_h
#define _maps_h

#include "global.h"
#include "bitdraw.h"

//------------------------------------------------------------------------------
// Bonus level map ID's
//------------------------------------------------------------------------------
#define BLEV_MUSICROOM		101
#define BLEV_CHEESEROOM		102
#define BLEV_TREASROOM		103
#define BLEV_JEWELROOM		104
#define BLEV_COINROOM		105
#define BLEV_CARDROOM		106
#define BLEV_KEYROOM		107
#define BLEV_POWERROOM		108
#define BLEV_COMPLETED		109
#define BLEV_TITLE		110
#define BLEV_HIGHMAP		111

//------------------------------------------------------------------------------
// Extra
//------------------------------------------------------------------------------
#define MAX_LEVELS	100

//------------------------------------------------------------------------------
// Maximum number of boxes (minimum = 42) - Else adjust CMap::LoadBox
//------------------------------------------------------------------------------
#define MAX_BOXES	80

//------------------------------------------------------------------------------
// Wind direction values
//------------------------------------------------------------------------------
#define WIND_NONE	0
#define WIND_UP		1
#define WIND_RIGHT	2
#define WIND_DOWN	3
#define WIND_LEFT	4

#define FRK_SIZE	400	// Maximum size of a FRK file in bytes

//------------------------------------------------------------------------------
// Used for the checkwall functions
//------------------------------------------------------------------------------
#define CBM_NOTINWALL	-1000

typedef struct _BOXPOS {
	short x1;
	short y1;
	short x2;
	short y2;
	short type;
	short buffer;	// Make the structure a multiple of 4
	} BOXPOS;

class CMap
{
public:
	CMap();
	void ZeroMap(void);
	void GetMap( int mapid );
	void Amiga2PC(short *data, int items);
	void Draw();
	void LoadBlockSet( int rid );
	void LoadMap( int id );
	void LoadBox( int id );
	void LoadWind( int id );
	void LoadFrk( int id );
	void ExtendBox( void );
	void SortBox( void );
	int CheckDown(int xpos, int ypos, int width, int ydelta);
	int CheckUp(int xpos, int ypos, int width, int ydelta, int hardwallflag=0);
	int CheckLeft(int xpos, int ypos, int height, int xdelta, int hardwallflag=0);
	int CheckRight(int xpos, int ypos, int height, int xdelta, int hardwallflag=0);
	BOXPOS *CheckHitWall(int xpos, int ypos, int width, int height, int hardwallflag=0);
	int GetLeftEdge(BOXPOS *bpos);
	int GetRightEdge(BOXPOS *bpos);
	BOXPOS *GetBox( void );
	void CycleBorder(void);
	void LoadSwapMap( int mapid );
private:
	void LoadData(int id, void *dest, int len);
			
public:
	short		m_MapData[MAP_SIZE];
	char		m_WindData[MAP_SIZE];
	short		m_FrkData[FRK_SIZE/2];
	CBitmapDraw	m_BlockSet;
private:
	BOXPOS		m_BoxData[MAX_BOXES];
	int		m_Num_Boxes;
};

#endif // _maps_h


