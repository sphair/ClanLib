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
// Methane Brothers Bitmap Grouping (Header File)
//------------------------------------------------------------------------------

#ifndef _bitgroup_h
#define _bitgroup_h

#include "gfxdef.h"

class CGame;
class CBitmapItem;
class CBitmapGroup
{
public:
	CBitmapGroup();
	~CBitmapGroup();
	CBitmapGroup( CGame *gptr );
	void DeleteAll(void);
	void Load(int sprid);
	void LoadRange(int sprid1, int sprid2);
	void Draw(int sprid, int xpos, int ypos, int flags = 0);
	CBitmapItem * GetItem(int sprid);
	void InitPurge(void);
	void DoPurge(void);
	CGame	*m_pGame;
private:
	void FixOffsets(void);
	int *FixGroup(int *fixptr);
	int MinGroup(int *fixptr);
	void Init(void);

	CBitmapItem *m_ItemList[SPR_SIZE];
};

#endif // _bitgroup_h



