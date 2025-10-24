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
// Methane Brothers Bitmap Item (Header File)
//------------------------------------------------------------------------------

#ifndef _bititem_h
#define _bititem_h

class CBitmapDraw;

//------------------------------------------------------------------------------
// Bitmaps flags (Bitmask)
//------------------------------------------------------------------------------
#define GFX_WHITE	2	// Draw the bitmap in white
#define GFX_NOWRAP	4	// Disable sprite screen wrapping
#define GFX_COL0	8	// Alternate Colour (Used with gas clouds)

class CGame;

class CBitmapItem
{
public:
	CBitmapItem();	// See warning in the code
	~CBitmapItem();
	CBitmapItem( CGame *gptr );
	void Draw(int xpos, int ypos, int flags = 0);
	void Load(int nIdResource);

public:
	int	m_Width;
	int	m_Height;
	int	m_XOff;
	int	m_YOff;

	CBitmapDraw	*m_pGfx;
	CGame		*m_pGame;

	int	m_PurgeFlag;
private:
	void DrawIt( int xpos, int ypos, int flags );
	void DrawWrap( int xpos, int ypos, int flags );
	void Init(void);

};

#endif // _bititem_h


