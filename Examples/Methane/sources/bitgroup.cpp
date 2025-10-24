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
// Methane Brothers Bitmap Grouping (Source File)
//------------------------------------------------------------------------------

#include "bitgroup.h"
#include "global.h"
#include "bititem.h"
#include "game.h"

//------------------------------------------------------------------------------
// Sprite offset fixes
//------------------------------------------------------------------------------
#define FCMD_GROUP	0	// Command - New Group
#define FCMD_RANGE	1	// Command - Use from a to b
#define FCMD_ITEMS	2	// Command - Use from a list
#define FCMD_END	0	// End of List

#define FCMD_EOL	3	// (Internal Use Only)

static int fixoffs[] =
	{
		FCMD_GROUP,
			FCMD_RANGE,SPR_PUFF_LEFT1, SPR_PUFF_STAND_R2,
			FCMD_ITEMS,SPR_PUFF_STAND_L1, SPR_PUFF_STAND_L2,
		FCMD_GROUP,
			FCMD_RANGE,SPR_BLOW_LEFT1, SPR_BLOW_STAND_R2,
			FCMD_ITEMS,SPR_BLOW_STAND_L1, SPR_BLOW_STAND_L2,
		FCMD_GROUP,
			FCMD_RANGE,SPR_BUG_LEFT1, SPR_BUG_RIGHT6,
		FCMD_GROUP,
			FCMD_RANGE,SPR_BUG_ROLL1, SPR_BUG_ROLL8,
		FCMD_GROUP,
			FCMD_RANGE,SPR_BUG_HATCH1, SPR_BUG_HATCH2,
		FCMD_GROUP,
			FCMD_RANGE,SPR_WHIRLY_LEFT1, SPR_WHIRLY_RIGHT6,
		FCMD_GROUP,
			FCMD_RANGE,SPR_WHIRLY_ROLL1, SPR_WHIRLY_ROLL8,
		FCMD_GROUP,
			FCMD_RANGE,SPR_WHIRLY_HATCH1, SPR_WHIRLY_HATCH2,
		FCMD_GROUP,
			FCMD_RANGE,SPR_DOOFUS_LEFT1, SPR_DOOFUS_RIGHT6,
		FCMD_GROUP,
			FCMD_RANGE,SPR_DOOFUS_ROLL1, SPR_DOOFUS_ROLL8,
		FCMD_GROUP,
			FCMD_RANGE,SPR_DOOFUS_HATCH1, SPR_DOOFUS_HATCH2,
		FCMD_GROUP,
			FCMD_RANGE,SPR_JUMP_LJUMP1, SPR_JUMP_LJUMP6,
		FCMD_GROUP,
			FCMD_RANGE,SPR_JUMP_LROLL1, SPR_JUMP_LROLL8,
		FCMD_GROUP,
			FCMD_RANGE,SPR_JUMP_SJUMP1, SPR_JUMP_SJUMP6,
		FCMD_GROUP,
			FCMD_RANGE,SPR_JUMP_SROLL1, SPR_JUMP_SROLL8,
		FCMD_GROUP,
			FCMD_RANGE,SPR_MBUG_LEFT1, SPR_MBUG_RIGHT3,
		FCMD_GROUP,
			FCMD_RANGE,SPR_MBUG_ROLL1, SPR_MBUG_ROLL8,
		FCMD_GROUP,
			FCMD_RANGE,SPR_MBUG_HATCH1, SPR_MBUG_HATCH2,
		FCMD_GROUP,
			FCMD_RANGE,SPR_MBUG_BOMB1, SPR_MBUG_BOMB4,
		FCMD_GROUP,
			FCMD_RANGE,SPR_CLOWN_RIGHT1, SPR_CLOWN_LEFT9,
		FCMD_GROUP,
			FCMD_RANGE,SPR_CLOWN_ROLL1, SPR_CLOWN_ROLL8,
		FCMD_GROUP,
			FCMD_RANGE,SPR_DWARF_RIGHT1, SPR_DWARF_STOP_L,
		FCMD_GROUP,
			FCMD_RANGE,SPR_DWARF_ROLL1, SPR_DWARF_ROLL8,
		FCMD_GROUP,
			FCMD_RANGE,SPR_ZOOM_LEFT1, SPR_ZOOM_RIGHT6,
		FCMD_GROUP,
			FCMD_RANGE,SPR_ZOOM_ROLL1, SPR_ZOOM_ROLL8,
		FCMD_GROUP,
			FCMD_RANGE,SPR_ZOOM_HATCH1, SPR_ZOOM_HATCH2,
		FCMD_GROUP,
			FCMD_RANGE,SPR_SPIKE_LEFT1, SPR_SPIKE_RIGHT5,
		FCMD_GROUP,
			FCMD_RANGE,SPR_SPIKE_ROLL1, SPR_SPIKE_ROLL8,
		FCMD_GROUP,
			FCMD_RANGE,SPR_NOTES_G1, SPR_NOTES_R6,
		FCMD_GROUP,
			FCMD_RANGE,SPR_TREAS_BIGCHEESE1, SPR_TREAS_CHEESEB3,
//		FCMD_GROUP,
//			FCMD_RANGE,SPR_LBOSS_L1, SPR_LBOSS_L8,
//		FCMD_GROUP,
//			FCMD_RANGE,SPR_LBOSS_R1, SPR_LBOSS_R8,
		FCMD_GROUP,
			FCMD_RANGE,SPR_LBOSS_ROLL1, SPR_LBOSS_ROLL8,
		FCMD_END
	};

//------------------------------------------------------------------------------
//! \brief Constructror
//!
//! WARNING - YOU MUST SET UP THE m_pGame POINTER BEFORE USING ANY FUNCTIONS
//------------------------------------------------------------------------------
CBitmapGroup::CBitmapGroup()
{
	m_pGame = 0;
	Init();
}

//------------------------------------------------------------------------------
//! \brief Initialise the bitmap group
//------------------------------------------------------------------------------
void CBitmapGroup::Init(void)
{
	int cnt;

	for (cnt = 0; cnt < SPR_SIZE; cnt++)	// For all bitmaps
	{
		m_ItemList[cnt] = 0;				// Clear the list
	}

	FixOffsets();		// Fix the sprite offsets
}

//------------------------------------------------------------------------------
//! \brief Initialise the bitmap group (constructor)
//!
//! 	\param gptr = The game pointer
//------------------------------------------------------------------------------
CBitmapGroup::CBitmapGroup( CGame *gptr )
{
	m_pGame = gptr;
	Init();
}

//------------------------------------------------------------------------------
//! \brief Destroy the bitmap group
//------------------------------------------------------------------------------
CBitmapGroup::~CBitmapGroup()
{
	DeleteAll();	// Delete all the items
}

//------------------------------------------------------------------------------
//! \brief Delete all the bitmap items in the list
//------------------------------------------------------------------------------
void CBitmapGroup::DeleteAll(void)
{
	int cnt;
	CBitmapItem *itemptr;

	for (cnt = 0; cnt < SPR_SIZE; cnt++)	// For all bitmaps
	{
		itemptr = m_ItemList[cnt];			// Get the bitmap
		if (itemptr)						// If exists
		{
			m_ItemList[cnt] = 0;			// Clear pointer
			delete itemptr;					// Delete the item
		}
	}
}

//------------------------------------------------------------------------------
//! \brief Load a bitmap - ready to draw (Load Bitmap)
//!
//! 	\param sprid = Sprite ID
//------------------------------------------------------------------------------
void CBitmapGroup::Load(int sprid)
{
	int offset;

	offset = sprid - SPR_START_NUMBER;	// Get offset
	if ( (offset < 0) || (offset >= SPR_SIZE) ) return;	// Illegal Sprite ID

	if (m_ItemList[offset])		// Already exists?
	{
		// Check for mask exist?
		m_ItemList[offset]->m_PurgeFlag = 1;	// Set the purge flag
		return;
	}
	CBitmapItem *pitem;
	SMB_NEW(pitem,CBitmapItem);	// Allocate structure
	if (pitem)	// Allocated okay?
	{
		pitem->m_pGame = m_pGame;
		m_ItemList[offset] = pitem;	// Insert bitmap item
		pitem->m_PurgeFlag = 1;		// Set the purge flag
		pitem->Load(sprid);		// Load the bitmap

		pitem->m_XOff = MainOffsets[offset].xoff;
		pitem->m_YOff = MainOffsets[offset].yoff;

	}
}

//------------------------------------------------------------------------------
//! \brief Get the bitmap item pointer
//!
//! 	\param sprid = Sprite ID
//!
//! 	\return The Item (0 = Error)
//------------------------------------------------------------------------------
CBitmapItem * CBitmapGroup::GetItem(int sprid)
{
	int offset;

	offset = sprid - SPR_START_NUMBER;	// Get offset
	if ( (offset < 0) || (offset >= SPR_SIZE) ) return 0;	// Illegal Sprite ID

	return (m_ItemList[offset]);

}

//------------------------------------------------------------------------------
//! \brief Draw a bitmap 
//!
//! 	\param sprid = Sprite ID
//!	\param xpos = X Draw position (offsets will be added to this)
//!	\param ypos = Y Draw position (offsets will be added to this)
//!	\param flags = (GFX_xxx flags) (Default = 0)
//------------------------------------------------------------------------------
void CBitmapGroup::Draw(int sprid, int xpos, int ypos, int flags)
{
	CBitmapItem *pitem;
	pitem = GetItem(sprid);
	if (pitem)
	{
		pitem->Draw(xpos, ypos, flags);
	}
	
}

//------------------------------------------------------------------------------
//! \brief Load a range of sprites 
//!
//! 	\param sprid1 = start sprite id
//! 	\param sprid2 = end sprite id (including this)
//------------------------------------------------------------------------------
void CBitmapGroup::LoadRange(int sprid1, int sprid2)
{
	if (sprid2<sprid1) return;	// Error!

	if (sprid2>SPR_END_NUMBER) sprid2 = SPR_END_NUMBER;	// Validate end value
	if (sprid1<SPR_START_NUMBER) sprid1 = SPR_START_NUMBER;	// Validate start value

	while (sprid1<=sprid2)	// For all sprites
	{
		Load(sprid1);			// Setup sprite
		sprid1++;				// Setup the next sprite
	}

}

//------------------------------------------------------------------------------
//! \brief Fix the sprite offset list
//------------------------------------------------------------------------------
void CBitmapGroup::FixOffsets(void)
{
	int cmd;
	int *fixptr;

	fixptr = fixoffs;		// Start of the fix list
	while(1)				// Forever
	{
		cmd = *(fixptr++);	// Get command
		switch (cmd)
		{
			case (FCMD_GROUP):
			{
				fixptr = FixGroup(fixptr);
				break;
			}

			default:		// End of the list
			{
				return;
			}
		}
	}
}

//------------------------------------------------------------------------------
//! \brief Get the minimum y pos for a range of sprites
//!
//! (Called by FixGroup())
//!
//! 	\param fixptr = array containing the group of sprites
//!
//! 	\return the minimum y pos
//------------------------------------------------------------------------------
int CBitmapGroup::MinGroup(int *fixptr)
{
	int	startspr;
	int endspr;
	int thisypos;
	int minypos;
	int cnt;
	int cmd;

	minypos = 16;

	do			// Forever
	{
		cmd = *(fixptr++);	// Get command
		switch (cmd)
		{
			case (FCMD_RANGE):
			{
				startspr = *(fixptr++);
				endspr = *(fixptr++);
				startspr -= SPR_START_NUMBER;
				endspr -= SPR_START_NUMBER;

				for (cnt = startspr; cnt <= endspr; cnt++)	// For all the offsets
				{
					thisypos = MainOffsets[cnt].yoff;
					if (thisypos < minypos) minypos = thisypos;	// Find the minimum y offset
				}
				break;
			}

			case (FCMD_ITEMS):
			{
				while ((*fixptr)>=SPR_START_NUMBER)	// For all items
				{
					startspr = *(fixptr++);
					startspr -= SPR_START_NUMBER;
					thisypos = MainOffsets[startspr].yoff;
					if (thisypos < minypos) minypos = thisypos;	// Find the minimum y offset
				}
				break;
			}

			default:		// End of the list
			{
				cmd = FCMD_EOL;
				break;
			}
		}
	
	}while(cmd!=FCMD_EOL);
	return minypos;

}

//------------------------------------------------------------------------------
//! \brief Fix the offsets for a range of sprites
//!
//! WARNING: NO VALIDATION CHECKS ARE CARRIED OUT
//!
//! 	\param fixptr = array containing the group of sprites
//!
//! 	\return end of the array
//------------------------------------------------------------------------------
int *CBitmapGroup::FixGroup(int *fixptr)
{
	int startspr;
	int endspr;
	int thisypos;
	int minypos;
	int cnt;
	int cmd;

	minypos = MinGroup(fixptr);

	do			// Forever
	{
		cmd = *(fixptr++);	// Get command
		switch (cmd)
		{
			case (FCMD_RANGE):
			{
				startspr = *(fixptr++);
				endspr = *(fixptr++);
				startspr -= SPR_START_NUMBER;
				endspr -= SPR_START_NUMBER;

				for (cnt = startspr; cnt <= endspr; cnt++)	// For all the offsets
				{
					thisypos = MainOffsets[cnt].yoff;
					MainOffsets[cnt].yoff = thisypos - minypos;	// Fix the offset
				}

				break;
			}

			case (FCMD_ITEMS):
			{
				while ((*fixptr)>=SPR_START_NUMBER)	// For all items
				{
					startspr = *(fixptr++);
					startspr -= SPR_START_NUMBER;
					thisypos = MainOffsets[startspr].yoff;
					MainOffsets[startspr].yoff = thisypos - minypos;	// Fix the offset
				}
				break;
			}

			default:		// End of the list
			{
				cmd = FCMD_EOL;
				break;
			}
		}
	
	}while(cmd!=FCMD_EOL);
	return fixptr-1;
}

//------------------------------------------------------------------------------
//! \brief Initialise the graphics purge
//!
//! (Sets a flag on each sprite)
//------------------------------------------------------------------------------
void CBitmapGroup::InitPurge(void)
{
	int cnt;
	CBitmapItem *itemptr;

	for (cnt = 0; cnt < SPR_SIZE; cnt++)	// For all bitmaps
	{
		itemptr = m_ItemList[cnt];			// Get the bitmap
		if (itemptr)						// If exists
		{
			itemptr->m_PurgeFlag = 0;		// Clear Purge Flag
		}
	}
}

//------------------------------------------------------------------------------
//! \brief Do the graphics purge
//!
//! (Removes unused graphics)
//------------------------------------------------------------------------------
void CBitmapGroup::DoPurge(void)
{
	int cnt;
	CBitmapItem *itemptr;

	for (cnt = 0; cnt < SPR_SIZE; cnt++)	// For all bitmaps
	{
		itemptr = m_ItemList[cnt];			// Get the bitmap
		if (itemptr)						// If exists
		{
			if (!(itemptr->m_PurgeFlag))	// Test Purge Flag
			{
				m_ItemList[cnt] = 0;		// Clear pointer
				delete itemptr;				// Delete the item

			}
		}
	}
}

