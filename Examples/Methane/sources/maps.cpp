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
// Methane Brothers Maps Control (Source File)
//------------------------------------------------------------------------------

#include "global.h"
#include "maps.h"
#include "mapdef.h"
#include "target.h"

//------------------------------------------------------------------------------
// Private Data
//------------------------------------------------------------------------------
typedef struct _LEVEL_DATA
{
	int map_id;
	int pos_id;
	int wnd_id;
	int frk_id;
	int alt_map_id;
	int alt_pos_id;

} LEVEL_DATA;
#define NUM_LEVEL_DATA 112
static LEVEL_DATA the_level_data[NUM_LEVEL_DATA] =
{
	{0,0,0,0,0,0},
	{MAPD_MAP_1,MAPD_POS_1,MAPD_WND_1,MAPD_FRK_1,0,0},
	{MAPD_MAP_2,MAPD_POS_2,MAPD_WND_2,MAPD_FRK_2,0,0},
	{MAPD_MAP_3,MAPD_POS_3,MAPD_WND_3,MAPD_FRK_3,0,0},
	{MAPD_MAP_4,MAPD_POS_4,MAPD_WND_4,MAPD_FRK_4,0,0},
	{MAPD_MAP_5,MAPD_POS_5,MAPD_WND_5,MAPD_FRK_5,0,0},
	{MAPD_MAP_6,MAPD_POS_6,MAPD_WND_6,MAPD_FRK_6,MAPD_MAP_6S,MAPD_POS_6S},
	{MAPD_MAP_7,MAPD_POS_7,MAPD_WND_7,MAPD_FRK_7,0,0},
	{MAPD_MAP_8,MAPD_POS_8,MAPD_WND_8,MAPD_FRK_8,0,0},
	{MAPD_MAP_9,MAPD_POS_9,MAPD_WND_9,MAPD_FRK_9,0,0},
	{MAPD_MAP_A,MAPD_POS_A,MAPD_WND_A,MAPD_FRK_A,0,0},
	{MAPD_MAP_B,MAPD_POS_B,MAPD_WND_B,MAPD_FRK_B,0,0},
	{MAPD_MAP_C,MAPD_POS_C,MAPD_WND_C,MAPD_FRK_C,0,0},
	{MAPD_MAP_D,MAPD_POS_D,MAPD_WND_D,MAPD_FRK_D,0,0},
	{MAPD_MAP_E,MAPD_POS_E,MAPD_WND_E,MAPD_FRK_E,0,0},
	{MAPD_MAP_F,MAPD_POS_F,MAPD_WND_F,MAPD_FRK_F,0,0},
	{MAPD_MAP_10,MAPD_POS_10,MAPD_WND_10,MAPD_FRK_10,0,0},
	{MAPD_MAP_11,MAPD_POS_11,MAPD_WND_11,MAPD_FRK_11,0,0},
	{MAPD_MAP_12,MAPD_POS_12,MAPD_WND_12,MAPD_FRK_12,0,0},
	{MAPD_MAP_13,MAPD_POS_13,MAPD_WND_13,MAPD_FRK_13,0,0},
	{MAPD_MAP_14,MAPD_POS_14,MAPD_WND_14,MAPD_FRK_14,0,0},
	{MAPD_MAP_15,MAPD_POS_15,MAPD_WND_15,MAPD_FRK_15,0,0},
	{MAPD_MAP_16,MAPD_POS_16,MAPD_WND_16,MAPD_FRK_16,0,0},
	{MAPD_MAP_17,MAPD_POS_17,MAPD_WND_17,MAPD_FRK_17,MAPD_MAP_17S,MAPD_POS_17S},
	{MAPD_MAP_18,MAPD_POS_18,MAPD_WND_18,MAPD_FRK_18,0,0},
	{MAPD_MAP_19,MAPD_POS_19,MAPD_WND_19,MAPD_FRK_19,0,0},
	{MAPD_MAP_1A,MAPD_POS_1A,MAPD_WND_1A,MAPD_FRK_1A,0,0},
	{MAPD_MAP_1B,MAPD_POS_1B,MAPD_WND_1B,MAPD_FRK_1B,MAPD_MAP_1BS,MAPD_POS_1BS},
	{MAPD_MAP_1C,MAPD_POS_1C,MAPD_WND_1C,MAPD_FRK_1C,0,0},
	{MAPD_MAP_1D,MAPD_POS_1D,MAPD_WND_1D,MAPD_FRK_1D,0,0},
	{MAPD_MAP_1E,MAPD_POS_1E,MAPD_WND_1E,MAPD_FRK_1E,0,0},
	{MAPD_MAP_1F,MAPD_POS_1F,MAPD_WND_1F,MAPD_FRK_1F,0,0},
	{MAPD_MAP_20,MAPD_POS_20,MAPD_WND_20,MAPD_FRK_20,0,0},
	{MAPD_MAP_21,MAPD_POS_21,MAPD_WND_21,MAPD_FRK_21,0,0},
	{MAPD_MAP_22,MAPD_POS_22,MAPD_WND_22,MAPD_FRK_22,0,0},
	{MAPD_MAP_23,MAPD_POS_23,MAPD_WND_23,MAPD_FRK_23,0,0},
	{MAPD_MAP_24,MAPD_POS_24,MAPD_WND_24,MAPD_FRK_24,0,0},
	{MAPD_MAP_25,MAPD_POS_25,MAPD_WND_25,MAPD_FRK_25,0,0},
	{MAPD_MAP_26,MAPD_POS_26,MAPD_WND_26,MAPD_FRK_26,0,0},
	{MAPD_MAP_27,MAPD_POS_27,MAPD_WND_27,MAPD_FRK_27,0,0},
	{MAPD_MAP_28,MAPD_POS_28,MAPD_WND_28,MAPD_FRK_28,0,0},
	{MAPD_MAP_29,MAPD_POS_29,MAPD_WND_29,MAPD_FRK_29,0,0},
	{MAPD_MAP_2A,MAPD_POS_2A,MAPD_WND_2A,MAPD_FRK_2A,MAPD_MAP_2AS,MAPD_POS_2AS},
	{MAPD_MAP_2B,MAPD_POS_2B,MAPD_WND_2B,MAPD_FRK_2B,0,0},
	{MAPD_MAP_2C,MAPD_POS_2C,MAPD_WND_2C,MAPD_FRK_2C,0,0},
	{MAPD_MAP_2D,MAPD_POS_2D,MAPD_WND_2D,MAPD_FRK_2D,0,0},
	{MAPD_MAP_2E,MAPD_POS_2E,MAPD_WND_2E,MAPD_FRK_2E,0,0},
	{MAPD_MAP_2F,MAPD_POS_2F,MAPD_WND_2F,MAPD_FRK_2F,0,0},
	{MAPD_MAP_30,MAPD_POS_30,MAPD_WND_30,MAPD_FRK_30,0,0},
	{MAPD_MAP_31,MAPD_POS_31,MAPD_WND_31,MAPD_FRK_31,0,0},
	{MAPD_MAP_32,MAPD_POS_32,MAPD_WND_32,MAPD_FRK_32,0,0},
	{MAPD_MAP_33,MAPD_POS_33,MAPD_WND_33,MAPD_FRK_33,0,0},
	{MAPD_MAP_34,MAPD_POS_34,MAPD_WND_34,MAPD_FRK_34,0,0},
	{MAPD_MAP_35,MAPD_POS_35,MAPD_WND_35,MAPD_FRK_35,MAPD_MAP_35S,MAPD_POS_35S},
	{MAPD_MAP_36,MAPD_POS_36,MAPD_WND_36,MAPD_FRK_36,0,0},
	{MAPD_MAP_37,MAPD_POS_37,MAPD_WND_37,MAPD_FRK_37,0,0},
	{MAPD_MAP_38,MAPD_POS_38,MAPD_WND_38,MAPD_FRK_38,0,0},
	{MAPD_MAP_39,MAPD_POS_39,MAPD_WND_39,MAPD_FRK_39,0,0},
	{MAPD_MAP_3A,MAPD_POS_3A,MAPD_WND_3A,MAPD_FRK_3A,0,0},
	{MAPD_MAP_3B,MAPD_POS_3B,MAPD_WND_3B,MAPD_FRK_3B,0,0},
	{MAPD_MAP_3C,MAPD_POS_3C,MAPD_WND_3C,MAPD_FRK_3C,0,0},
	{MAPD_MAP_3D,MAPD_POS_3D,MAPD_WND_3D,MAPD_FRK_3D,0,0},
	{MAPD_MAP_3E,MAPD_POS_3E,MAPD_WND_3E,MAPD_FRK_3E,0,0},
	{MAPD_MAP_3F,MAPD_POS_3F,MAPD_WND_3F,MAPD_FRK_3F,MAPD_MAP_3FS,MAPD_POS_3FS},
	{MAPD_MAP_40,MAPD_POS_40,MAPD_WND_40,MAPD_FRK_40,0,0},
	{MAPD_MAP_41,MAPD_POS_41,MAPD_WND_41,MAPD_FRK_41,0,0},
	{MAPD_MAP_42,MAPD_POS_42,MAPD_WND_42,MAPD_FRK_42,0,0},
	{MAPD_MAP_43,MAPD_POS_43,MAPD_WND_43,MAPD_FRK_43,0,0},
	{MAPD_MAP_44,MAPD_POS_44,MAPD_WND_44,MAPD_FRK_44,0,0},
	{MAPD_MAP_45,MAPD_POS_45,MAPD_WND_45,MAPD_FRK_45,0,0},
	{MAPD_MAP_46,MAPD_POS_46,MAPD_WND_46,MAPD_FRK_46,0,0},
	{MAPD_MAP_47,MAPD_POS_47,MAPD_WND_47,MAPD_FRK_47,0,0},
	{MAPD_MAP_48,MAPD_POS_48,MAPD_WND_48,MAPD_FRK_48,0,0},
	{MAPD_MAP_49,MAPD_POS_49,MAPD_WND_49,MAPD_FRK_49,MAPD_MAP_49S,MAPD_POS_49S},
	{MAPD_MAP_4A,MAPD_POS_4A,MAPD_WND_4A,MAPD_FRK_4A,MAPD_MAP_4AS,MAPD_POS_4AS},
	{MAPD_MAP_4B,MAPD_POS_4B,MAPD_WND_4B,MAPD_FRK_4B,0,0},
	{MAPD_MAP_4C,MAPD_POS_4C,MAPD_WND_4C,MAPD_FRK_4C,0,0},
	{MAPD_MAP_4D,MAPD_POS_4D,MAPD_WND_4D,MAPD_FRK_4D,0,0},
	{MAPD_MAP_4E,MAPD_POS_4E,MAPD_WND_4E,MAPD_FRK_4E,0,0},
	{MAPD_MAP_4F,MAPD_POS_4F,MAPD_WND_4F,MAPD_FRK_4F,0,0},
	{MAPD_MAP_50,MAPD_POS_50,MAPD_WND_50,MAPD_FRK_50,0,0},
	{MAPD_MAP_51,MAPD_POS_51,MAPD_WND_51,MAPD_FRK_51,0,0},
	{MAPD_MAP_52,MAPD_POS_52,MAPD_WND_52,MAPD_FRK_52,0,0},
	{MAPD_MAP_53,MAPD_POS_53,MAPD_WND_53,MAPD_FRK_53,0,0},
	{MAPD_MAP_54,MAPD_POS_54,MAPD_WND_54,MAPD_FRK_54,0,0},
	{MAPD_MAP_55,MAPD_POS_55,MAPD_WND_55,MAPD_FRK_55,MAPD_MAP_55S,MAPD_POS_55S},
	{MAPD_MAP_56,MAPD_POS_56,MAPD_WND_56,MAPD_FRK_56,0,0},
	{MAPD_MAP_57,MAPD_POS_57,MAPD_WND_57,MAPD_FRK_57,0,0},
	{MAPD_MAP_58,MAPD_POS_58,MAPD_WND_58,MAPD_FRK_58,0,0},
	{MAPD_MAP_59,MAPD_POS_59,MAPD_WND_59,MAPD_FRK_59,0,0},
	{MAPD_MAP_5A,MAPD_POS_5A,MAPD_WND_5A,MAPD_FRK_5A,0,0},
	{MAPD_MAP_5B,MAPD_POS_5B,MAPD_WND_5B,MAPD_FRK_5B,0,0},
	{MAPD_MAP_5C,MAPD_POS_5C,MAPD_WND_5C,MAPD_FRK_5C,0,0},
	{MAPD_MAP_5D,MAPD_POS_5D,MAPD_WND_5D,MAPD_FRK_5D,MAPD_MAP_5DS,MAPD_POS_5DS},
	{MAPD_MAP_5E,MAPD_POS_5E,MAPD_WND_5E,MAPD_FRK_5E,0,0},
	{MAPD_MAP_5F,MAPD_POS_5F,MAPD_WND_5F,MAPD_FRK_5F,0,0},
	{MAPD_MAP_60,MAPD_POS_60,MAPD_WND_60,MAPD_FRK_60,0,0},
	{MAPD_MAP_61,MAPD_POS_61,MAPD_WND_61,MAPD_FRK_61,0,0},
	{MAPD_MAP_62,MAPD_POS_62,MAPD_WND_62,MAPD_FRK_62,0,0},
	{MAPD_MAP_63,MAPD_POS_63,MAPD_WND_63,MAPD_FRK_63,0,0},
	{MAPD_MAP_64,MAPD_POS_64,MAPD_WND_64,MAPD_FRK_64,0,0},
	{MAPD_MAP_65,MAPD_POS_65,MAPD_WND_65,MAPD_FRK_65,0,0},
	{MAPD_MAP_66,MAPD_POS_66,MAPD_WND_66,MAPD_FRK_66,0,0},
	{MAPD_MAP_67,MAPD_POS_67,MAPD_WND_67,MAPD_FRK_67,0,0},
	{MAPD_MAP_68,MAPD_POS_68,MAPD_WND_68,MAPD_FRK_68,0,0},
	{MAPD_MAP_69,MAPD_POS_69,MAPD_WND_69,MAPD_FRK_69,0,0},
	{MAPD_MAP_6A,MAPD_POS_6A,MAPD_WND_6A,MAPD_FRK_6A,0,0},
	{MAPD_MAP_6B,MAPD_POS_6B,MAPD_WND_6B,MAPD_FRK_6B,0,0},
	{MAPD_MAP_6C,MAPD_POS_6C,MAPD_WND_6C,MAPD_FRK_6C,0,0},
	{MAPD_MAP_6D,0,0,0,0,0},
	{MAPD_MAP_6E,0,0,0,0,0},
	{MAPD_MAP_6F,0,0,0,0,0}

};

//------------------------------------------------------------------------------
// Map Border Offsets (for the CycleBorder function)
// Taken straight from the amiga - Not ideal, but it works
//------------------------------------------------------------------------------
#define NUM_CYCLES 68
static int cycle_table[NUM_CYCLES] =
	{
		(14*MAP_WIDTH)+00,
		(13*MAP_WIDTH)+00,
		(12*MAP_WIDTH)+00,
		(11*MAP_WIDTH)+00,
		(10*MAP_WIDTH)+00,
		(9*MAP_WIDTH)+00,
		(8*MAP_WIDTH)+00,
		(7*MAP_WIDTH)+00,
		(6*MAP_WIDTH)+00,
		(5*MAP_WIDTH)+00,
		(4*MAP_WIDTH)+00,
		(3*MAP_WIDTH)+00,
		(2*MAP_WIDTH)+00,
		(1*MAP_WIDTH)+00,
		0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,
		(1*MAP_WIDTH)+19,
		(2*MAP_WIDTH)+19,
		(3*MAP_WIDTH)+19,
		(4*MAP_WIDTH)+19,
		(5*MAP_WIDTH)+19,
		(6*MAP_WIDTH)+19,
		(7*MAP_WIDTH)+19,
		(8*MAP_WIDTH)+19,
		(9*MAP_WIDTH)+19,
		(10*MAP_WIDTH)+19,
		(11*MAP_WIDTH)+19,
		(12*MAP_WIDTH)+19,
		(13*MAP_WIDTH)+19,
		(14*MAP_WIDTH)+19,
		(15*MAP_WIDTH)+19,(15*MAP_WIDTH)+18,(15*MAP_WIDTH)+17,
		(15*MAP_WIDTH)+16,(15*MAP_WIDTH)+15,(15*MAP_WIDTH)+14,
		(15*MAP_WIDTH)+13,(15*MAP_WIDTH)+12,(15*MAP_WIDTH)+11,
		(15*MAP_WIDTH)+10,(15*MAP_WIDTH)+9,(15*MAP_WIDTH)+8,
		(15*MAP_WIDTH)+7,(15*MAP_WIDTH)+6,(15*MAP_WIDTH)+5,
		(15*MAP_WIDTH)+4,(15*MAP_WIDTH)+3,(15*MAP_WIDTH)+2,
		(15*MAP_WIDTH)+1,(15*MAP_WIDTH)+0
	};
	

//------------------------------------------------------------------------------
//! \brief Create a new map 
//------------------------------------------------------------------------------
CMap::CMap()
{
	ZeroMap();
}

//------------------------------------------------------------------------------
//! \brief Zero all the map data
//------------------------------------------------------------------------------
void CMap::ZeroMap(void)
{
	memset(m_MapData, 0, sizeof(m_MapData));
	memset(m_BoxData, 0, sizeof(m_BoxData));
	memset(m_WindData, 0, sizeof(m_WindData));
	memset(m_FrkData, 0, sizeof(m_FrkData));
	m_Num_Boxes = 0;
}

//------------------------------------------------------------------------------
//! \brief Load a standard map into memory
//!
//! 	\param mapid = The map id
//------------------------------------------------------------------------------
void CMap::GetMap( int mapid )
{
	LEVEL_DATA *ld;
	if (mapid>=NUM_LEVEL_DATA) mapid = 0;

	ld = &the_level_data[mapid];

	LoadMap( ld->map_id );
	LoadBox( ld->pos_id );
	LoadWind( ld->wnd_id );
	LoadFrk( ld->frk_id );
}

//------------------------------------------------------------------------------
//! \brief Load the map resource entry into memory
//!
//! 	\param id = resource id
//!	\param dest = Where to write to
//!	\param len = Length of the destination (in bytes)
//------------------------------------------------------------------------------
void CMap::LoadData(int id, void *dest, int len)
{
	MAP_RESOURCE_DATA *mptr;

	memset(dest, 0, len);

	if (id < MAPD_START_NUMBER) return;
	if (id > MAPD_END_NUMBER) return;

	mptr = &MethaneMapData[id - MAPD_START_NUMBER];
	memcpy(dest, mptr->ptr, mptr->length);
}

//------------------------------------------------------------------------------
//! \brief Load a map into memory. The Map is converted from amiga to pc
//!
//! 	\param id = resource id
//------------------------------------------------------------------------------
void CMap::LoadMap( int id )
{
	LoadData(id, m_MapData, sizeof(m_MapData));
	if (!id) return;
	Amiga2PC( m_MapData, MAP_SIZE );
}

//------------------------------------------------------------------------------
//! \brief Load a FRK (sprite position) file into memory.
//!
//! The file is converted from amiga to pc
//!
//! 	\param id = resource id
//------------------------------------------------------------------------------
void CMap::LoadFrk( int id )
{
	LoadData( id, m_FrkData, sizeof(m_FrkData));
	if (!id) return;
	Amiga2PC( m_FrkData, FRK_SIZE/2 );

}

//------------------------------------------------------------------------------
//! \brief Load a map wind file into memory.
//!
//! 	\param id = resource id
//------------------------------------------------------------------------------
void CMap::LoadWind( int id )
{
	LoadData( id, m_WindData, sizeof(m_WindData));

}

//------------------------------------------------------------------------------
//! \brief Load a box_data into memory. The box_data is converted from amiga to pc
//!
//! (This contains the collision boxes)
//!
//! 	\param id = resource id
//------------------------------------------------------------------------------
void CMap::LoadBox( int id )
{
	short *ptr;
	int cnt;

	m_Num_Boxes = 0;

	LoadData( id, m_BoxData, sizeof(m_BoxData));
	if (!id) return;
	
	Amiga2PC( (short *) m_BoxData, 250 );
	ptr = (short *) m_BoxData;

	m_Num_Boxes = *(ptr);
	ptr +=3;				// Point to the first box

	for (cnt = 0; cnt < m_Num_Boxes; cnt++)	// For all the boxes
	{
		m_BoxData[cnt].x1 = *(ptr++);	// Store X1
		m_BoxData[cnt].y1 = *(ptr++);	// Store Y1
		m_BoxData[cnt].x2 = *(ptr++);	// Store X2
		m_BoxData[cnt].y2 = *(ptr++);	// Store Y2
		m_BoxData[cnt].type = *(ptr);	// Store type
		ptr += 3;			// Point to the next box
	}
	ExtendBox();			// extend box edges
	SortBox();				// sort the boxes
	
}

//------------------------------------------------------------------------------
//! \brief Extend the box screen edges 
//!
//! (to fix comparing problems when an object is moving off screen)
//------------------------------------------------------------------------------
void CMap::ExtendBox( void )
{
	int cnt;
	int box_x1,box_y1,box_x2,box_y2;
	for (cnt = 0; cnt < m_Num_Boxes; cnt++)	// For all boxes
	{
		box_x1 = m_BoxData[cnt].x1;		// Extract box details
		box_y1 = m_BoxData[cnt].y1;
		box_x2 = m_BoxData[cnt].x2;
		box_y2 = m_BoxData[cnt].y2;

		if (!box_x1) box_x1-=200;
		if (!box_y1) box_y1-=200;
		if (box_x2==SCR_WIDTH) box_x2+=200;
		if (box_y2==SCR_HEIGHT) box_y2+=200;

		m_BoxData[cnt].x1 = box_x1;		// Write Back box details
		m_BoxData[cnt].y1 = box_y1;
		m_BoxData[cnt].x2 = box_x2;
		m_BoxData[cnt].y2 = box_y2;

	}

}

//------------------------------------------------------------------------------
//! \brief Sort the boxes, so type '0' are first
//!
//! (to fix problems - which I cannot currently remember) (well I coded it years ago!)\n
//! This uses a slow sort algorithm - A fast one is not needed
//------------------------------------------------------------------------------
void CMap::SortBox( void )
{
	int cnt;
	int box_x1,box_y1,box_x2,box_y2,box_type;
	for (cnt = 0; cnt < (m_Num_Boxes-1); cnt++)	// For all boxes (except last box)
	{
		box_type = m_BoxData[cnt].type;
		if (!box_type) continue;				// Type 0 is first - No swap needed
		if (m_BoxData[cnt+1].type) continue;	// If next type is 1 - No swap needed

		box_x1 = m_BoxData[cnt].x1;		// Extract box details
		box_y1 = m_BoxData[cnt].y1;
		box_x2 = m_BoxData[cnt].x2;
		box_y2 = m_BoxData[cnt].y2;

		m_BoxData[cnt].x1 = m_BoxData[cnt+1].x1;
		m_BoxData[cnt].y1 = m_BoxData[cnt+1].y1;
		m_BoxData[cnt].x2 = m_BoxData[cnt+1].x2;
		m_BoxData[cnt].y2 = m_BoxData[cnt+1].y2;
		m_BoxData[cnt].type = m_BoxData[cnt+1].type;

		m_BoxData[cnt+1].x1 = box_x1;		// Write Back box details
		m_BoxData[cnt+1].y1 = box_y1;
		m_BoxData[cnt+1].x2 = box_x2;
		m_BoxData[cnt+1].y2 = box_y2;
		m_BoxData[cnt+1].type = box_type;
		cnt = 0;	// Reset sort counter
	}

}

//------------------------------------------------------------------------------
//! \brief Convert Amiga Short to PC Short by swapping the bytes
//!
//! 	\param data = pointer to the data to convert
//!	\param items = number of items to convert
//------------------------------------------------------------------------------
void CMap::Amiga2PC(short *data, int items)
{
	unsigned short	v1,v2;
	unsigned char	*ptr;
	unsigned short	value;

	ptr = (unsigned char *) data;
	for (; items>0; items--)	// For all items
	{
		v1 = *(ptr++);
		v2 = *(ptr++);
		value = (v1 << 8 ) | v2;
		*(data++) = value;		// Write data
	}
}

//------------------------------------------------------------------------------
//! \brief Draw the map onto the screen (Coded for speed - not safety)
//!
//! (using the damage list)
//------------------------------------------------------------------------------
void CMap::Draw( )
{
	int x,y;
	short *mapptr;

	mapptr = m_MapData;
	for (y = 0; y < MAP_HEIGHT; y++)
	{
		for (x = 0; x < MAP_WIDTH; x++)
		{
			m_BlockSet.Draw16( x*16, y*16, *mapptr );
			mapptr++;
		}
	}
}

//------------------------------------------------------------------------------
//! \brief Load the block set the map will use
//!
//! 	\param rid = The block set graphic id
//------------------------------------------------------------------------------
void CMap::LoadBlockSet( int rid )
{
	m_BlockSet.Load(rid);
}

//------------------------------------------------------------------------------
//! \brief Check to see if an object will move down into a wall
//!
//! 	\param xpos = object xpos (bottom) (on pixel)
//! 	\param ypos = object ypos (at foot) (on pixel)
//!	\param width = object width
//!	\param ydelta = object (ypos - last_ypos) (Should be +'ve)
//!
//! 	\return The topmost pixel ypos of the wall. (Subtract 1 for above the wall)\n
//!		CBM_NOTINWALL = Did not hit a wall
//------------------------------------------------------------------------------
int CMap::CheckDown(int xpos, int ypos, int width, int ydelta)
{
	int cnt;
	int box_x1,box_y1,box_x2,box_y2;

	::CheckPos( xpos, ypos);

	for (cnt = 0; cnt < m_Num_Boxes; cnt++)	// For all boxes
	{
		box_x1 = m_BoxData[cnt].x1;		// Extract box details
		box_y1 = m_BoxData[cnt].y1;
		box_x2 = m_BoxData[cnt].x2;
		box_y2 = m_BoxData[cnt].y2;

		if (xpos >= box_x2 ) continue;	// Box is to the left
		// The below line should be '<=' - But you can jump through corners
		if ((xpos+width) <= box_x1 ) continue;	// Box is to the right

		box_y2 += ydelta;				// Extend box to prevent jumping through walls

		if (ypos >= box_y2 ) continue;	// Box is above foot
		if (ypos < box_y1 ) continue;	// Box is below foot

		if ( (ypos-ydelta) < box_y1) return box_y1;	// Only if last was not in box
	}
	return CBM_NOTINWALL;
}

//------------------------------------------------------------------------------
//! \brief Check to see if an object will move up into a wall
//!
//! 	\param xpos = object xpos (on pixel)
//! 	\param ypos = object ypos (at foot) (on pixel)
//!	\param width = object width
//!	\param ydelta = object (ypos - last_ypos) (Should be -'ve)
//!	\param hardwallflag = (Default = 0) 1 = Disable jump though wall
//!
//! 	\return The bottom pixel ypos of the wall. (Add 1 for below the wall)\n
//!		CBM_NOTINWALL = Did not hit a wall
//------------------------------------------------------------------------------
int CMap::CheckUp(int xpos, int ypos, int width, int ydelta, int hardwallflag)
{
	int cnt;
	int box_x1,box_y1,box_x2,box_y2,box_type;

	::CheckPos( xpos, ypos);

	for (cnt = 0; cnt < m_Num_Boxes; cnt++)	// For all boxes
	{
		box_x1 = m_BoxData[cnt].x1;		// Extract box details
		box_y1 = m_BoxData[cnt].y1;
		box_x2 = m_BoxData[cnt].x2;
		box_y2 = m_BoxData[cnt].y2;
		box_type = m_BoxData[cnt].type;

		if (!hardwallflag) if (box_type) continue;	// Jump throughable wall

		if (xpos >= box_x2 ) continue;	// Box is to the left
		// The below line should be '<=' - But you can jump through corners
		if ((xpos+width) <= box_x1 ) continue;	// Box is to the right

		box_y1 += ydelta;				// Extend box to prevent jumping through walls

		if (ypos >= box_y2 ) continue;	// Box is above head
		if (ypos < box_y1 ) continue;	// Box is below head

		if ( (ypos-ydelta) >= box_y2) return box_y2-1;
	}
	return CBM_NOTINWALL;
}

//------------------------------------------------------------------------------
//! \brief Check to see if an object will move left into a wall
//!
//! 	\param xpos = object xpos (on pixel)
//! 	\param ypos = object ypos (at head) (on pixel)
//!	\param height = object height
//!	\param xdelta = object (xpos - last_xpos) (Should be -'ve)
//!	\param hardwallflag = (Default = 0) 1 = Disable jump though wall
//!
//! 	\return The rightmost pixel xpos of the wall. (Add 1 for the right the wall)\n
//!		CBM_NOTINWALL = Did not hit a wall
//------------------------------------------------------------------------------
int CMap::CheckLeft(int xpos, int ypos, int height, int xdelta, int hardwallflag)
{
	int cnt;
	int box_x1,box_y1,box_x2,box_y2,box_type;

	::CheckPos( xpos, ypos);

	for (cnt = 0; cnt < m_Num_Boxes; cnt++)	// For all boxes
	{
		box_x1 = m_BoxData[cnt].x1;		// Extract box details
		box_y1 = m_BoxData[cnt].y1;
		box_x2 = m_BoxData[cnt].x2;
		box_y2 = m_BoxData[cnt].y2;
		box_type = m_BoxData[cnt].type;

		if (!hardwallflag) if (box_type) continue;	// Jump throughable wall

		if (ypos >= box_y2 ) continue;	// Box is above head
		if ( (ypos+height) <= box_y1 ) continue;	// Box is below head

		box_x1 += xdelta;				// Extend box to prevent jumping through walls

		if (xpos >= box_x2 ) continue;	// Box is to the left
		if (xpos < box_x1 ) continue;	// Box is to the right

		if ( (xpos-xdelta) >= box_x2) return box_x2-1;
	}
	return CBM_NOTINWALL;
}

//------------------------------------------------------------------------------
//! \brief Check to see if an object will move right into a wall
//!
//! 	\param xpos = object xpos (right of object) (on pixel)
//! 	\param ypos = object ypos (at head) (on pixel)
//!	\param height = object height
//!	\param xdelta = object (xpos - last_xpos) (Should be +'ve)
//!	\param hardwallflag = (Default = 0) 1 = Disable jump though wall
//!
//! 	\return The leftmost pixel xpos of the wall. (Subtract 1 for the right the wall)\n
//!		CBM_NOTINWALL = Did not hit a wall
//------------------------------------------------------------------------------
int CMap::CheckRight(int xpos, int ypos, int height, int xdelta, int hardwallflag)
{
	int cnt;
	int box_x1,box_y1,box_x2,box_y2,box_type;

	::CheckPos( xpos, ypos);

	for (cnt = 0; cnt < m_Num_Boxes; cnt++)	// For all boxes
	{
		box_x1 = m_BoxData[cnt].x1;		// Extract box details
		box_y1 = m_BoxData[cnt].y1;
		box_x2 = m_BoxData[cnt].x2;
		box_y2 = m_BoxData[cnt].y2;
		box_type = m_BoxData[cnt].type;

		if (!hardwallflag) if (box_type) continue;	// Jump throughable wall

		if (ypos >= box_y2 ) continue;	// Box is above head
		if ( (ypos + height) <= box_y1 ) continue;	// Box is below head

		box_x2 += xdelta;				// Extend box to prevent jumping through walls

		if (xpos >= box_x2 ) continue;	// Box is to the left
		if (xpos < box_x1 ) continue;	// Box is to the right

		if ( (xpos-xdelta) < box_x1) return box_x1;
	}
	return CBM_NOTINWALL;
}

//------------------------------------------------------------------------------
//! \brief Check to see if an object is touching a wall
//!
//! 	\param xpos = object xpos (right of object) (on pixel)
//! 	\param ypos = object ypos (at head) (on pixel)
//!	\param width = object width
//!	\param height = object height
//!	\param hardwallflag = (Default = 0) 1 = Disable jump though wall
//!
//! 	\return 0 = Did not hit the wall. Else = Box Hit
//------------------------------------------------------------------------------
BOXPOS *CMap::CheckHitWall(int xpos, int ypos, int width, int height, int hardwallflag)
{
	int cnt;
	int box_x1,box_y1,box_x2,box_y2,box_type;

	::CheckPos( xpos, ypos);

	for (cnt = 0; cnt < m_Num_Boxes; cnt++)	// For all boxes
	{
		box_x1 = m_BoxData[cnt].x1;		// Extract box details
		box_y1 = m_BoxData[cnt].y1;
		box_x2 = m_BoxData[cnt].x2;
		box_y2 = m_BoxData[cnt].y2;
		box_type = m_BoxData[cnt].type;

		if (!hardwallflag) if (box_type) continue;	// Jump throughable wall
		if (xpos >= box_x2) continue;
		if (ypos >= box_y2) continue;
		if (xpos+width < box_x1) continue;
		if (ypos+height >= box_y1) return &(m_BoxData[cnt]);

	}
	return 0;
}

//------------------------------------------------------------------------------
//! \brief Get the x position of the left edge of wall
//!
//! This takes account of the screen edges fix (PORTED FROM THE AMIGA VERSION!)\n
//! Note: This function assumes that a wall at the edge of the screen\n
//!  has a wall at the other side of the screen. (A fix!)\n
//!
//! 	\param bpos = Box Pointer
//!
//! 	\return the x pos value - 1
//------------------------------------------------------------------------------
int CMap::GetLeftEdge(BOXPOS *bpos)
{
	int xpos;
	xpos = bpos->x1;

	if (xpos<0) xpos = 0;

	if (xpos==0) xpos = SCR_WIDTH-16;	// See function comment
	return xpos-1;
}

//------------------------------------------------------------------------------
//! \brief Get the x position of the right edge of wall
//!
//! This takes account of the screen edges fix (PORTED FROM THE AMIGA VERSION!)\n
//! Note: This function assumes that a wall at the edge of the screen\n
//!  has a wall at the other side of the screen. (A fix!)
//!
//! 	\param bpos = Box Pointer
//!
//! 	\return the x pos value + 1
//------------------------------------------------------------------------------
int CMap::GetRightEdge(BOXPOS *bpos)
{
	int xpos;
	xpos = bpos->x2;
	if (xpos>SCR_WIDTH) xpos = SCR_WIDTH;

	if (xpos==SCR_WIDTH) xpos = 16;		// See function comment
	return xpos+1;
}

//------------------------------------------------------------------------------
//! \brief Get a new collision box
//!
//! The box is cleared to zero
//!
//! 	\return The box. 0 = Error occured (too many boxes on screen)
//------------------------------------------------------------------------------
BOXPOS *CMap::GetBox( void )
{
	BOXPOS *bpos;

	if (m_Num_Boxes >= MAX_BOXES)
	{
		return 0;	// Too many boxes
	}
	bpos = &m_BoxData[m_Num_Boxes];
	m_Num_Boxes++;
	bpos->x1 = bpos->y1 = bpos->x2 = bpos->y2 = bpos->type = 0;
	return bpos;
}

//------------------------------------------------------------------------------
//! \brief Cycle the map borders
//------------------------------------------------------------------------------
void CMap::CycleBorder(void)
{
	int cnt;
	short first_item;
	int last_pos;
	int this_pos;

	last_pos = cycle_table[0];
	first_item = m_MapData[last_pos];
	for (cnt=1; cnt<(NUM_CYCLES); cnt++)
	{
		this_pos = cycle_table[cnt];
		m_MapData[last_pos] = m_MapData[this_pos];
		last_pos = this_pos;
	}
	m_MapData[last_pos] = first_item;


}

//------------------------------------------------------------------------------
//! \brief Load the swap map (the alternate map used by the switch)
//!
//! 	\param mapid = The map id
//------------------------------------------------------------------------------
void CMap::LoadSwapMap( int mapid )
{
	LEVEL_DATA *ld;
	if (mapid>=NUM_LEVEL_DATA) mapid = 0;

	ld = &the_level_data[mapid];

	LoadMap( ld->alt_map_id );
	LoadBox( ld->alt_pos_id );
}

