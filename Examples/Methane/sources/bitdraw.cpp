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
// Methane Brothers Bitmap Draw (Source File)
//------------------------------------------------------------------------------

#include "global.h"
#include "bitdraw.h"
#include "gfxdef.h"
#include "target.h"

//------------------------------------------------------------------------------
//! \brief Constructor
//------------------------------------------------------------------------------
CBitmapDraw::CBitmapDraw()
{
}

//------------------------------------------------------------------------------
//! \brief Destructor
//------------------------------------------------------------------------------
CBitmapDraw::~CBitmapDraw()
{
	UnLoad();
}


//------------------------------------------------------------------------------
//! \brief Load a graphic
//!
//! 	\param rid = resource id
//!
//! 	\return 0 = okay, else error occured
//------------------------------------------------------------------------------
int CBitmapDraw::Load(int rid)
{
	if ((rid < SPR_START_NUMBER) || (rid > SPR_END_NUMBER) ) return 0;
	mcoord_ptr = &MainOffsets[rid - SPR_START_NUMBER];

	return 0;
}

//------------------------------------------------------------------------------
//! \brief UnLoad a graphic
//------------------------------------------------------------------------------
void CBitmapDraw::UnLoad(void)
{
}

//------------------------------------------------------------------------------
//! \brief Draw a bitmap onto the screen 
//!
//!	\param xpos = The x position to draw to
//!	\param ypos = The y position to draw to
//!	\param draw_white = true = Draw white (using the mask)
//------------------------------------------------------------------------------
void CBitmapDraw::Draw(int xpos, int ypos, bool draw_white )
{
	int dest_xpos = xpos;
	int dest_ypos = ypos;
	int width = mcoord_ptr->width;
	int height = mcoord_ptr->height;
	int tex_xpos = mcoord_ptr->texture_xpos;
	int tex_ypos = mcoord_ptr->texture_ypos;

	if (dest_xpos < 0)
	{
		width += dest_xpos;
		tex_xpos -= dest_xpos;
		dest_xpos = 0;
	}

	if (dest_ypos < 0)
	{
		height += dest_ypos;
		tex_ypos -= dest_ypos;
		dest_ypos = 0;
	}

	int edge = dest_xpos+width;
	if (edge > SCR_WIDTH)
	{
		width -= edge - SCR_WIDTH;
	}

	edge = dest_ypos+height;
	if (edge > SCR_HEIGHT)
	{
		height -= edge - SCR_HEIGHT;
	}

	if ((width > 0) && (height > 0))
	{
		GLOBAL_GameTarget->Draw(dest_xpos, dest_ypos, width, height, mcoord_ptr->texture_number, tex_xpos, tex_ypos, draw_white);
	}
}

//------------------------------------------------------------------------------
//! \brief Draw a bitmap onto the screen with an alternative colour (different colour bank)
//!
//!	\param xpos = The x position to draw to
//!	\param ypos = The y position to draw to
//------------------------------------------------------------------------------
void CBitmapDraw::DrawColour(int xpos, int ypos )
{
	// This is not required.
	Draw(xpos,ypos, false);
}


//------------------------------------------------------------------------------
//! \brief Draw a 16x16 section of a bitmap onto the screen
//!
//!	\param xpos = X Position
//!	\param ypos = Y Position
//!	\param block_offset = the block id to draw
//------------------------------------------------------------------------------
void CBitmapDraw::Draw16( int xpos, int ypos, int block_offset )
{
	int tex_xpos = mcoord_ptr->texture_xpos;	
	int tex_ypos = mcoord_ptr->texture_ypos;

	tex_xpos += block_offset*16;
	int yoffset = tex_xpos / 512;	// Amount left over
	tex_xpos -= (yoffset * 512);	// Fix x offset

	yoffset *= 16;	// Number of lines to move down
	tex_ypos += yoffset;

	int page_offset = tex_ypos / 512;	// Amount left over
	tex_ypos -= (page_offset * 512);	// Fix y offset

	GLOBAL_GameTarget->Draw(xpos, ypos, 16, 16,mcoord_ptr->texture_number + page_offset, tex_xpos, tex_ypos, false);
}

