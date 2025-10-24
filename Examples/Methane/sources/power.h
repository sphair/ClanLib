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
// Methane Brothers PowerUp Object (Header File)
//------------------------------------------------------------------------------

#ifndef _POWEROBJ_h
#define _POWEROBJ_h

#include "objlist.h"

#define PUP_YELLOWSTAR	0
#define PUP_SAUSAGE	1
#define PUP_TURBO	2
#define PUP_GREENSTAR	3
#define PUP_PURPLESTAR	4
#define PUP_OIL		5
#define PUP_TAP		6
#define PUP_REDSTAR	7
#define PUP_BOWLING	8
#define PUP_FEATHER	9
#define PUP_WINGS	10
#define PUP_COOKIE	11
#define PUP_CHICKEN	12
#define PUP_SPININGTOP	13
#define PUP_CANDLE	14
#define PUP_WHITEPOTION	15
#define PUP_MOON	16
#define PUP_MARBLE	17
#define PUP_MASK	18
#define PUP_XYLOPHONE	19
#define PUP_CARRYDOOR	20

class CPowerUpObj : public CLinkObject
{
public:
	CPowerUpObj();
	virtual void Do( void );
	virtual void Draw( void );
	void Setup(int type);
	void ConvertBaddies( int objtype );
	void DestroyAllBaddies(void);
public:
	int m_PowerUpType;
	int m_PowerCnt;
	int m_PowerDelay;
	int m_Sample;
};


#endif // _POWEROBJ_h

