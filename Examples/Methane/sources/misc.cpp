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
// Methane Brothers Misc Objects (Source File)
//------------------------------------------------------------------------------

#include "stdio.h"
#include "stdlib.h"
#include "misc.h"
#include "global.h"
#include "bitgroup.h"
#include "objtypes.h"
#include "game.h"
#include "player.h"
#include "goodie.h"
#include "gasobj.h"

//------------------------------------------------------------------------------
// The End Credits
//------------------------------------------------------------------------------

static PARTYOFFS	party_00 = {SPR_ENDSPR_1+0x00,0x0000,0x0000};
static PARTYOFFS	party_01 = {SPR_ENDSPR_1+0x01,0x0010,0x0000};
static PARTYOFFS	party_02 = {SPR_ENDSPR_1+0x02,0x0030,0x0000};
static PARTYOFFS	party_03 = {SPR_ENDSPR_1+0x03,0x0050,0x0000};
static PARTYOFFS	party_04 = {SPR_ENDSPR_1+0x04,0x0070,0x0010};
static PARTYOFFS	party_05 = {SPR_ENDSPR_1+0x05,0x0000,0x0000};
static PARTYOFFS	party_06 = {SPR_ENDSPR_1+0x06,0x0020,0x0000};
static PARTYOFFS	party_07 = {SPR_ENDSPR_1+0x07,0x0000,0x0000};
static PARTYOFFS	party_08 = {SPR_ENDSPR_1+0x08,0x0028,0x0002};
static PARTYOFFS	party_09 = {SPR_ENDSPR_1+0x09,0x0048,0x0002};
static PARTYOFFS	party_0a = {SPR_ENDSPR_1+0x0a,0x0058,0x0002};
static PARTYOFFS	party_0b = {SPR_ENDSPR_1+0x0b,0x0000,0x0000};
static PARTYOFFS	party_0c = {SPR_ENDSPR_1+0x0c,0x0020,0xFFFFFFD0};
static PARTYOFFS	party_0d = {SPR_ENDSPR_1+0x0d,0x0040,0x0000};
static PARTYOFFS	party_0e = {SPR_ENDSPR_1+0x0e,0x0000,0x0000};
static PARTYOFFS	party_0f = {SPR_ENDSPR_1+0x0f,0x005E,0xFFFFFFF2};
static PARTYOFFS	party_10 = {SPR_ENDSPR_1+0x10,0x007E,0xFFFFFFFA};
static PARTYOFFS	party_11 = {SPR_ENDSPR_1+0x11,0x008E,0xFFFFFFF9};
static PARTYOFFS	party_12 = {SPR_ENDSPR_1+0x12,0x0092,0x000B};
static PARTYOFFS	party_13 = {SPR_ENDSPR_1+0x13,0x00B2,0xFFFFFFFB};
static PARTYOFFS	party_14 = {SPR_ENDSPR_1+0x14,0x00C2,0xFFFFFFFA};
static PARTYOFFS	party_15 = {SPR_ENDSPR_1+0x15,0x0000,0x0000};
static PARTYOFFS	party_16 = {SPR_ENDSPR_1+0x16,0x0020,0x0008};
static PARTYOFFS	party_17 = {SPR_ENDSPR_1+0x17,0x0020,0x0000};
static PARTYOFFS	party_18 = {SPR_ENDSPR_1+0x18,0x0180,0x0010};
static PARTYOFFS	party_19 = {SPR_ENDSPR_1+0x19,0x0000,0x0000};
static PARTYOFFS	party_1a = {SPR_ENDSPR_1+0x1a,0x0020,0x0000};
static PARTYOFFS	party_1b = {SPR_ENDSPR_1+0x1b,0x0000,0x0010};
static PARTYOFFS	party_1c = {SPR_ENDSPR_1+0x1c,0x0010,0x0};
static PARTYOFFS	party_1d = {SPR_ENDSPR_1+0x1d,0x0030,0xFFFFFFF0};

static PARTYOFFS	*party_group1[] = {
	&party_00,&party_01,&party_02,&party_00,&party_03,&party_04,0};

static PARTYOFFS	*party_group2[] = {
	&party_05,&party_06,0};

static PARTYOFFS	*party_group3[] = {
	&party_07,&party_08,&party_09,&party_0a,&party_0f,&party_10,
	&party_11,&party_12,&party_13,&party_14,0};

static PARTYOFFS	*party_group4[] = {
	&party_0b,&party_0c,&party_0d,0};

static PARTYOFFS	*party_group5[] = {
	&party_19,&party_1a,0};

static PARTYOFFS	*party_group6[] = {
	&party_15,&party_16,0};

static PARTYOFFS	*party_group7[] = {
	&party_0e,&party_17,&party_18,0};

static PARTYOFFS	*party_group8[] = {
	&party_1b,&party_1c,&party_1d,0};

static PARTYOFFS	*party_group9[] = {
	0};

static const char end_txt1[] =
	"\x20""PROJECT DIRECTOR\01"
	"\x30""DELVIN SORRELL\01";

static const char end_txt2[] =
	"\x18""BACKGROUND BLOCKS\01"
	"\x30""DEBBIE SORRELL\01";

static const char end_txt3[] =
	"\x28""THE DESIGN TEAM\01"
	"\x38""LLOYD MURPHY\01"
	"\x30""DEBBIE SORRELL\01"
	"\x30""DELVIN SORRELL\01"
	"\x60""MARK PAGE\01";

static const char end_txt4[] =
	"\x70""PICKUPS\01"
	"\x38""TONY GAITSKELL\01";

static const char end_txt5[] =
	"\x48""SPRITES AND\01"
	"\x58""ANIMATION\01"
	"\x40""LLOYD MURPHY\01";

static const char end_txt6[] =
	"\x70""SUPPORT\01"
	"\x60""TONY KING\01";

static const char end_txt7[] =
	"\x38""MUSIC AND SFX\01"
	"\x50""MATT OWENS\01";

static char end_txt8[] =
	"\x80""CODE\01"
	"\x58""MARK PAGE\01";

static char end_txt9[] =
	 	 //01234567890123456789
	"\x01""  YOU HAVE REACHED\01"
	"\x01""  THE END OF SUPER\01"
	"\x01""  METHANE BROTHERS\01"
	"\x01""  NOW TRY ENTERING\01"
	"\x01""  MANIC USING THE\01"
	"\x01""   JOYSTICK WHEN\01"
	"\x01"" PAUSED FOR AN EXTRA\01"
	"\x01""  HARD GAME OF SMB\01"
	"\x01""  THANKS GO OUT TO\01"
	"\x01"" EVERYBODY ENVOLVED\01"
	"\x01""FOR MAKING THIS GAME\01"
	"\x01""LOOK OUT FOR FUTURE\01"
	"\x01""  APACHE SOFTWARE\01"
	"\x01""GAMES FOR THE AMIGA\01"
	"\x01""      THE END\01";

static ENDGROUP	end_group1 = {
	88, party_group1, 64, end_txt1};

static ENDGROUP	end_group2 = {
	128, party_group2, 64, end_txt2};

static ENDGROUP	end_group3 = {
	16*3, party_group3, 64, end_txt3};

static ENDGROUP	end_group4 = {
	120, party_group4, 64, end_txt4};

static ENDGROUP	end_group5 = {
	128, party_group5, 64, end_txt5};

static ENDGROUP	end_group6 = {
	128, party_group6, 64, end_txt6};

static ENDGROUP	end_group7 = {
	120, party_group7, 64, end_txt7};

static ENDGROUP	end_group8 = {
	120, party_group8, 64, end_txt8};

static ENDGROUP	end_group9 = {
	88, party_group9, 64, end_txt9};

ENDGROUP	*end_items[] = {
	&end_group8,
	&end_group5,
	&end_group7,
	&end_group4,
	&end_group2,
	&end_group1,
	&end_group3,
	&end_group6,
	&end_group9,0
	};

//------------------------------------------------------------------------------
// Status Bar Offsets
//------------------------------------------------------------------------------
PLAYER_STATUS pstat1 = {
	SPR_P1UP,8,SPR_FNT_R0,8+32, 8+32+64+16, 1, 0xe
	};

PLAYER_STATUS pstat2 = {
	SPR_P2UP,184-8,SPR_FNT_G0,184+32-8,184+32+64+16-8, 0x130, -0xe
	};

//------------------------------------------------------------------------------
// Level Text Names
//------------------------------------------------------------------------------
const char *level_text_names[NUM_STD_LEVELS] =
{
"THIS IS EASY","FRUIT SURPRISE","FOUR EDGES","FLYING FRUIT","GAS EM!",
"SWITCH HUNT","FRYING TONIGHT","ROUND AND ROUND","WHIRLYGIGS GALORE",
"MEET GRUMPY","HARD BOILED","BIG TOP","MARBLE ALLEY","LOOKS TOUGH",
"GETTING HARD","GREEN FINGERS","SCARY WEARY","THE MAZE","EASIER THAN IT LOOKS",
"SPIKES HOUSE","BOUNCY SPRINGS","YIKES","LOCKED AWAY","TEN PIN","TAP DANCE",
"SECRET GARDEN","EMPTY CHEST","IRON GIRDERS","CLOWNING AROUND","DOOR SECRET",
"BONKS GROTTO","TAKE A BREAK","PATIENCE","BANGERS","JUMP FOR JOY",
"SMART COOKIE","STAY COOL","INVINCABLE","39 STEPS","RIBBIT RIBBET",
"ROUNDABOUT","SPEEDY","FAST AND FURIOUS","SNAP SNAP SNAP","ZIG ZAG DOWN",
"3RD DIMENSION","ROLL AROUND","SMALL ANGLES","FLY AWAY","TREE TOPS",
"RUN FOR IT","PANIC ATTACK","STONE STAIRS","DO NOT ENTER","CANDLE POWER",
"TIMES UP","UP THE WALL","MAGGOTS","HAMMER TIME","STACK EM HIGH",
"JUMPERS AGAIN","ZOOM SHAKE THE ROOM","ONE ONLY","DANG HARD","FRUIT SHOP",
"NICE AND FAST","JUMP FOR YOUR LIFE","FUNNEL","WALKIES","PLUS MINUS",
"BACK IN TIME","TRUNDLE","LEMMIE OUT","PIRATES CHEST","OLD SOXS","FIND THEM",
"BOMBS AWAY","IT IS FUNNY","BOUNCY BOUNCE","LOGICAL","BE CAREFUL","DEBBIES FLOOR",
"METHOD MADNESS","KNOCK KNOCK","DOWN THE SIDE","BLANK THOUGHTS","MARKS HAT",
"TWIN ENGINE","THE BIG NOSE","INTRODUCING THIS","DOUBLE WAMMY","BUG HUNT",
"BOXED UP","BONKS REVENGE","BOMBS AWAY","SQUARE DANCE","CHURCH TOWER",
"TOP DEAD CENTRE","MATTS MOUSTACHE","THE BELL TOWER", "THE MAD MUSICIAN",
"MOUSE HOUSE","CHEST OF TREASURES","HIDDEN CAVE OF GEMS","CHEST OF GOLD COINS",
"","","POWER STATION",""
};

//------------------------------------------------------------------------------
// Font Conversion Table (Ascii value - 32)
//------------------------------------------------------------------------------
int font_table[NUM_FONT_TABLE] ={
	SPR_FOOD_1, SPR_RFONT_PLING, SPR_FOOD_2, SPR_RFONT_CURSOR,
	SPR_CARD_SHEART, SPR_CARD_SCLUB, SPR_CARD_SDMOND, SPR_FOOD_7,
	SPR_FOOD_8, SPR_FOOD_9, SPR_FOOD_10, SPR_FOOD_11,
	SPR_FOOD_12, SPR_FOOD_13, SPR_FOOD_1, SPR_FOOD_2,
	SPR_RFONT_0, SPR_RFONT_1, SPR_RFONT_2, SPR_RFONT_3,
	SPR_RFONT_4, SPR_RFONT_5, SPR_RFONT_6, SPR_RFONT_7,
	SPR_RFONT_8, SPR_RFONT_9, SPR_FOOD_3, SPR_FOOD_4,
	SPR_FOOD_5, SPR_FOOD_6, SPR_FOOD_7, SPR_RFONT_QUES,
	SPR_FOOD_8, SPR_RFONT_A, SPR_RFONT_B, SPR_RFONT_C,
	SPR_RFONT_D, SPR_RFONT_E, SPR_RFONT_F, SPR_RFONT_G,
	SPR_RFONT_H, SPR_RFONT_I, SPR_RFONT_J, SPR_RFONT_K,
	SPR_RFONT_L, SPR_RFONT_M, SPR_RFONT_N, SPR_RFONT_O,
	SPR_RFONT_P, SPR_RFONT_Q, SPR_RFONT_R, SPR_RFONT_S,
	SPR_RFONT_T, SPR_RFONT_U, SPR_RFONT_V, SPR_RFONT_W,
	SPR_RFONT_X, SPR_RFONT_Y, SPR_RFONT_Z, SPR_FOOD_1,
	SPR_FOOD_2, SPR_FOOD_3, SPR_FOOD_4, SPR_FOOD_5
	};
//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------
#define NUMRISELENGTH	(50/3)

//------------------------------------------------------------------------------
//! \brief Initialise the number rising object
//------------------------------------------------------------------------------
CNumRiseObj::CNumRiseObj()
{
	m_Frame = SPR_BUG_LEFT1;
}

//------------------------------------------------------------------------------
//! \brief Draw the number rising object
//------------------------------------------------------------------------------
void CNumRiseObj::Draw( void )
{
	if (m_Frame) m_pGame->m_Sprites.Draw( m_Frame, m_XPos, m_YPos );
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CNumRiseObj::Do( void )
{
	m_Delay++;
	if (m_Delay>=NUMRISELENGTH)
	{
		DeleteObject();
		return;
	}
	m_YPos-=3;
	if (m_YPos<0) m_YPos+=SCR_HEIGHT;
}

//------------------------------------------------------------------------------
// The RainFont
//------------------------------------------------------------------------------

#define RFONT_CMD_NONE		0
#define RFONT_CMD_UP1		1
#define RFONT_CMD_WOBBLE	2
#define RFONT_CMD_UP2		3

#define RFONT_RISE_RATE 8	// Rise speed

#define RFONT_NUM_WOBBLES	14
static int wobble_offsets[RFONT_NUM_WOBBLES] =
	{
		1,2,3,3,2,1,0,-1,-2,-3,-3,-2,-1,0
	};


//------------------------------------------------------------------------------
//! \brief Initialise the rainbow font object
//------------------------------------------------------------------------------
CRFontObj::CRFontObj()
{
	m_Frame = SPR_RFONT_1;
	m_Command = RFONT_CMD_NONE;
}

//------------------------------------------------------------------------------
//! \brief Draw the rainbow font object
//------------------------------------------------------------------------------
void CRFontObj::Draw( void )
{
	if (m_Frame) m_pGame->m_Sprites.Draw( m_Frame, m_XPos, m_YPos, GFX_NOWRAP );
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CRFontObj::Do( void )
{
	switch (m_Command)
	{
		case (RFONT_CMD_UP1):
			DoCmdUp1();
			break;
		case (RFONT_CMD_WOBBLE):
			DoCmdWobble();
			break;
		case (RFONT_CMD_UP2):
			DoCmdUp2();
			break;
	}
}

//------------------------------------------------------------------------------
//! \brief Set a char rising, then wobble, then off screen
//!
//! 	\param frame = Frame to show
//!	\param xpos = X offset
//!	\param yoffset = Offset from the centre of the screen when wobbling
//!	\param delay = Number of frames to delay before rising takes place
//------------------------------------------------------------------------------
void CRFontObj::SetRiser( int frame, int xpos, int yoffset, int delay )
{
	m_Frame = frame;
	m_XPos = xpos;
	m_YPos = SCR_HEIGHT;

	m_YCentre = (SCR_HEIGHT/2) + yoffset + (7+6+5+4+3+2+1);
	m_Delay = delay;
	m_YInert = RFONT_RISE_RATE;
	m_Command = RFONT_CMD_UP1;
}

//------------------------------------------------------------------------------
//! \brief Do the object initial rising to the centre of the screen
//------------------------------------------------------------------------------
void CRFontObj::DoCmdUp1(void)
{
	if (m_Delay)	// On pause?
	{
		m_Delay--;
	}else
	{
		m_YPos -= m_YInert;
		if (m_YPos<=m_YCentre)		// Reached the centre?
		{
			// Slow down to zero
			m_YInert--;
			if (!m_YInert)
			{
				m_Command = RFONT_CMD_WOBBLE;
			}
		}
	}
}

//------------------------------------------------------------------------------
//! \brief Do the object wobbling
//------------------------------------------------------------------------------
void CRFontObj::DoCmdWobble(void)
{
	int offset;

	offset = wobble_offsets[m_Delay];

	m_YPos += offset;

	m_Delay++;
	if (m_Delay>=RFONT_NUM_WOBBLES)
	{
		m_Command = RFONT_CMD_UP2;
		m_YInert = 0;
	}
}

//------------------------------------------------------------------------------
//! \brief Do the object rising off the screen
//------------------------------------------------------------------------------
void CRFontObj::DoCmdUp2(void)
{
	m_YInert++;
	if (m_YInert>RFONT_RISE_RATE) m_YInert = RFONT_RISE_RATE;		// Speed up to 8

	m_YPos -= m_YInert;

	if (m_YPos<-32)					// Off screen
	{
		DeleteObject();
	}
}

//------------------------------------------------------------------------------
// The Tube
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//! \brief Initialise the tube object
//------------------------------------------------------------------------------
CTubeObj::CTubeObj()
{
	m_Frame = SPR_TUBE;
}

//------------------------------------------------------------------------------
//! \brief Load the object graphics
//------------------------------------------------------------------------------
void CTubeObj::LoadGfx( void )
{
	m_pGame->m_Sprites.Load(SPR_TUBE);
}

//------------------------------------------------------------------------------
// The Real Doo Dah Day
//------------------------------------------------------------------------------

static OBJMOVE realday_move = {0,0,0,50,0,8*256,0};

//------------------------------------------------------------------------------
//! \brief Initialise the day object
//------------------------------------------------------------------------------
CRealDayObj::CRealDayObj()
{
	m_Frame = SPR_CB_DAY3;
	InitMovement(realday_move);
}

//------------------------------------------------------------------------------
//! \brief Setup the day object
//!
//! 	\param xpos = X Position of "doo dah day"
//! 	\param ypos = Y Position of "doo dah day"
//------------------------------------------------------------------------------
void CRealDayObj::Setup( int xpos, int ypos )
{
	m_XPos = m_OldXPos = xpos;
	m_YPos = m_OldYPos = ypos;
}

//------------------------------------------------------------------------------
//! \brief Draw the sucker object
//------------------------------------------------------------------------------
void CRealDayObj::Draw( void )
{
	m_pGame->m_Sprites.Draw( m_Frame, m_XPos, m_YPos, GFX_NOWRAP );
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CRealDayObj::Do( void )
{
	SetMoveUp();
	DoYInertia();

	if (m_YPos<-100)
	{
		DeleteObject();
	}
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// The Day Object
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Animation
//------------------------------------------------------------------------------
static int anm_day[] =
	{
	SPR_DAY_1,
	SPR_DAY_2,
	SPR_DAY_3,
	SPR_DAY_4,
	ANM_RESTART
	};

static OBJSIZE day_size = { 2, 2, 28, 36 };
static OBJMOVE day_move = {130,130/2,2*256,130,130/2,2*256,0};


//------------------------------------------------------------------------------
//! \brief Initialise the day
//------------------------------------------------------------------------------
CDayObj::CDayObj()
{
	m_Frame = SPR_DAY_1;
	m_pSequence = anm_day;
	InitSize(day_size);
	InitMovement(day_move);
}

//------------------------------------------------------------------------------
//! \brief Setup the day
//!
//! 	\param xpos = Where to display the day X
//! 	\param ypos = Where to display the day Y
//------------------------------------------------------------------------------
void CDayObj::Setup(int xpos, int ypos)
{
	m_OldXPos = m_XPos = xpos;
	m_OldYPos = m_YPos = ypos;
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CDayObj::Do( void )
{
	int flag;
	CPlayerObj *pobj;
	CheckKillPlayer();

	flag = 0;

	if (m_pGame->m_CompleteFlag)	// Kill Day if completed
	{
		flag = 1;
	}
	pobj = (CPlayerObj *) m_pGame->m_PlayerList.m_pFirst;
	while (pobj)
	{
		if (pobj->m_Command==PCMD_DIE)
		{
			flag = 1;
		}else
		{
			if (!(m_pGame->m_MainCounter & 3)) pobj->AddScore( 1 );
		}
		pobj = (CPlayerObj *) pobj->m_pNext;
	}

	if (!flag)
	{
		if (CheckKillPlayer())
		{
			flag = 1;
		}

	}

	if (flag)		// Kill this object?
	{

		DeleteObject();
		return;
	}

	Fly2ClosestPlayer();

	DoXInertia();
	DoYInertia();
	::CheckPos(m_XPos, m_YPos);

	Animate(256);
}

//------------------------------------------------------------------------------
// The Card Object
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Animation
//------------------------------------------------------------------------------
#define NUM_CARD_FRMS	11
static int anm_card_spade[NUM_CARD_FRMS+1] =
	{
	SPR_CARD_SPADE4,
	SPR_CARD_SPADE5,
	SPR_CARD_BACK1,
	SPR_CARD_BACK2,
	SPR_CARD_BACK3,
	SPR_CARD_BACK4,
	SPR_CARD_BACK5,
	SPR_CARD_SIDE,
	SPR_CARD_SPADE1,
	SPR_CARD_SPADE2,
	SPR_CARD_SPADE3,
	ANM_RESTART
	};

static int anm_card_heart[NUM_CARD_FRMS+1] =
	{
	SPR_CARD_HEART4,
	SPR_CARD_HEART5,
	SPR_CARD_BACK1,
	SPR_CARD_BACK2,
	SPR_CARD_BACK3,
	SPR_CARD_BACK4,
	SPR_CARD_BACK5,
	SPR_CARD_SIDE,
	SPR_CARD_HEART1,
	SPR_CARD_HEART2,
	SPR_CARD_HEART3,
	ANM_RESTART
	};

static int anm_card_club[NUM_CARD_FRMS+1] =
	{
	SPR_CARD_CLUB4,
	SPR_CARD_CLUB5,
	SPR_CARD_BACK1,
	SPR_CARD_BACK2,
	SPR_CARD_BACK3,
	SPR_CARD_BACK4,
	SPR_CARD_BACK5,
	SPR_CARD_SIDE,
	SPR_CARD_CLUB1,
	SPR_CARD_CLUB2,
	SPR_CARD_CLUB3,
	ANM_RESTART
	};

static int anm_card_diamond[NUM_CARD_FRMS+1] =
	{
	SPR_CARD_DMOND4,
	SPR_CARD_DMOND5,
	SPR_CARD_BACK1,
	SPR_CARD_BACK2,
	SPR_CARD_BACK3,
	SPR_CARD_BACK4,
	SPR_CARD_BACK5,
	SPR_CARD_SIDE,
	SPR_CARD_DMOND1,
	SPR_CARD_DMOND2,
	SPR_CARD_DMOND3,
	ANM_RESTART
	};

static OBJSIZE card_size = { 4, 0, 24, 31 };
static OBJMOVE card_move = {0x2a0,0x100,6*256,0xf*256,0x1d0,0xf*256,0xf*256};

#define CARDTYPE_SPADE		0
#define CARDTYPE_HEART		1
#define CARDTYPE_CLUB		2
#define CARDTYPE_DIAMOND	3

static int *anm_card_group[4] =
	{
		anm_card_spade, 
		anm_card_heart, 
		anm_card_club, 
		anm_card_diamond 
	};
static int conv_card_id[4] = {1,2,4,8};

//------------------------------------------------------------------------------
//! \brief Initialise the card
//------------------------------------------------------------------------------
CCardObj::CCardObj()
{
	m_OldXPos = m_XPos = 140;
	m_OldYPos = m_YPos = 0;
	InitSize(card_size);
	InitMovement(card_move);
	m_CardType = rand() & 3;
	m_pSequence = anm_card_group[m_CardType];
	m_Frame = *m_pSequence;
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CCardObj::Do( void )
{

	CPlayerObj *pobj;

	pobj = HitPlayer();
	if (pobj)			// Collision occured?
	{
		m_pGame->m_pSound->PlaySample(SND_CARDSOUND, m_XPos);
		m_pGame->CreateCloud( m_XPos, m_YPos );
		pobj->m_Cards = pobj->m_Cards | conv_card_id[m_CardType];

		if (pobj->m_Cards==15)	// All cards collected?
		{
			pobj->m_Cards = 0;
			pobj->m_Lives++;
			m_pGame->SetBonusLevel(BLEV_CARDROOM);
		}

		DeleteObject();
		return ;
	}

	DirWalk();
	MoveObject();

	Animate(256);
}

//------------------------------------------------------------------------------
//! \brief Get the card mask, from its id
//!
//! 	\return the mask (1,2,4,8)
//------------------------------------------------------------------------------
int CCardObj::GetCardMask( void )
{
	return (conv_card_id[m_CardType]);
}

//------------------------------------------------------------------------------
// The Oil Object
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Animation
//------------------------------------------------------------------------------
static int anm_oil[] =
	{
	SPR_OIL_1,
	SPR_OIL_1,
	SPR_OIL_2,
	SPR_OIL_2,
	SPR_OIL_3,
	SPR_OIL_3,
	SPR_OIL_2,
	SPR_OIL_2,
	ANM_RESTART
	};

#define NUM_OIL_OFFSETS 24
static int oil_offsets[NUM_OIL_OFFSETS] =
	{
	3,2,2,1,1,1,0,-1,-1,-1,-2,-2,-3,
	-2,-2,-1,-1,-1,0,1,1,1,2,2
	};
	
static OBJSIZE oil_size = { 6, 10, 4, 5 };
static OBJMOVE oil_move = {195,40,9*256,8*256,145,8*256,8*256};

//------------------------------------------------------------------------------
//! \brief Initialise the oil
//------------------------------------------------------------------------------
COilObj::COilObj()
{
	m_Frame = SPR_OIL_LARGE;
	m_pSequence = anm_oil;
	m_Offset = 0;
	m_Fix = 0;
	InitSize(oil_size);
	InitMovement(oil_move);
	m_Flags = FLAG_STICKY;
}

//------------------------------------------------------------------------------
//! \brief Setup the oil
//!
//! (This function also sets the inertias)
//!
//! 	\param xpos = Where to display the oil X
//! 	\param ypos = Where to display the oil Y
//------------------------------------------------------------------------------
void COilObj::Setup(int xpos, int ypos)
{
	m_OldXPos = m_XPos = xpos;
	m_OldYPos = m_YPos = ypos;
	m_XInert = (rand() & ((256*16)-1)) - 256*8;
	m_YInert = (rand() & ((256*16)-1)) - 256*8;

}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void COilObj::Do( void )
{
	int offset;
	MoveObject();
	if (!m_Y_Flag)	// On Floor?
	{
		Animate(256);
		m_Offset++;
		if (m_Offset >= NUM_OIL_OFFSETS) m_Offset = 0;
		offset = oil_offsets[m_Offset];
		m_XInert = offset << 8;
		m_Fix = 0;
	}else
	{
		if (m_Fix)
		{
			m_Frame = SPR_OIL_LARGE;
			if (m_YInert>(6*256))	// Falling straight?
			{
				m_XInert = 2*256;	// then tap object to the right
			}
		}else
		{
			m_Fix = 1;
		}
	}
}

//------------------------------------------------------------------------------
// The Water Object
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Animation
//------------------------------------------------------------------------------
static int anm_water[] =
	{
	SPR_WATER_1,
	SPR_WATER_1,
	SPR_WATER_2,
	SPR_WATER_2,
	SPR_WATER_3,
	SPR_WATER_3,
	SPR_WATER_2,
	SPR_WATER_2,
	ANM_RESTART
	};

static OBJSIZE water_size = { 6, 10, 4, 5 };
static OBJMOVE water_move = {4*256,0,8*256,8*256,145,8*256,8*256};

//------------------------------------------------------------------------------
//! \brief Initialise the water
//------------------------------------------------------------------------------
CWaterObj::CWaterObj()
{
	m_Frame = SPR_WATER_LARGE;
	m_pSequence = anm_water;
	m_Fix = 0;
	InitSize(water_size);
	InitMovement(water_move);
	m_Flags = FLAG_WEAPON;
}

//------------------------------------------------------------------------------
//! \brief Setup the water
//!
//! (This function also sets the inertias)
//!
//! 	\param xpos = Where to display the water X
//! 	\param ypos = Where to display the water Y
//------------------------------------------------------------------------------
void CWaterObj::Setup(int xpos, int ypos)
{
	m_OldXPos = m_XPos = xpos;
	m_OldYPos = m_YPos = ypos;
	m_XInert = (rand() & 0xfff) - 0x7ff;
	m_YInert = (rand() & 0xfff) - 0x7ff;

	// Prevent a stationary water droplet
	if (m_XInert<0)
	{
		if (m_XInert > -256) m_XInert-=256;
	}else
	{
		if (m_XInert < 256) m_XInert+=256;
	}

}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CWaterObj::Do( void )
{
	MoveObject();
	if (m_State&STATE_LEFT) SetMoveRight();
	if (m_State&STATE_RIGHT) SetMoveLeft();

	if (!m_Y_Flag)	// On Floor?
	{
		Animate(256);
		m_Fix = 0;
	}else
	{
		if (m_Fix)
		{
			m_Frame = SPR_WATER_LARGE;
			if (m_YInert>(6*256))	// Falling straight?
			{
				m_XInert = 2*256;	// then tap object to the right
			}
		}else
		{
			m_Fix = 1;
		}
	}
}

//------------------------------------------------------------------------------
// The Fire Object
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Animation
//------------------------------------------------------------------------------
static int anm_fire_floor[] =
	{
	SPR_FIRE_F1,
	SPR_FIRE_F2,
	SPR_FIRE_F3,
	SPR_FIRE_F4,
	ANM_RESTART
	};
static int anm_fire_air[] =
	{
	SPR_FIRE_A1,
	SPR_FIRE_A2,
	SPR_FIRE_A3,
	SPR_FIRE_A4,
	ANM_RESTART
	};


static OBJSIZE fire_size = { 1, 11, 12, 20 };
static OBJMOVE fire_move = {195,40,9*256,6*256,130,6*256,6*256};

//------------------------------------------------------------------------------
//! \brief Initialise the fire
//------------------------------------------------------------------------------
CFireObj::CFireObj()
{
	m_Frame = SPR_FIRE_A1;
	m_pSequence = anm_fire_air;
	m_Fix = 0;
	m_Offset = 0;
	InitSize(fire_size);
	InitMovement(fire_move);
	m_Flags = FLAG_WEAPON;
}

//------------------------------------------------------------------------------
//! \brief Setup the fire
//!
//! (This function also sets the inertias)
//!
//! 	\param xpos = Where to display the fire X
//! 	\param ypos = Where to display the fire Y
//------------------------------------------------------------------------------
void CFireObj::Setup(int xpos, int ypos)
{
	m_OldXPos = m_XPos = xpos;
	m_OldYPos = m_YPos = ypos;
	m_XInert = (rand() & 0xfff) - 0x7ff;
	m_YInert = -(rand() & 0xfff);

}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CFireObj::Do( void )
{
	int offset;
	MoveObject();

	if (!m_Y_Flag)	// On Floor?
	{
		m_pSequence = anm_fire_floor;
		m_Offset++;
		if (m_Offset >= NUM_OIL_OFFSETS) m_Offset = 0;
		offset = oil_offsets[m_Offset];
		m_XInert = offset << 7;
		m_Fix = 0;
	}else
	{
		if (m_Fix)
		{
			m_pSequence = anm_fire_air;
			if (m_YInert>(6*256))	// Falling straight?
			{
				m_XInert = 2*256;	// then tap object to the right
			}
		}else
		{
			m_Fix = 1;
		}
	}
	Animate(256);
}

//------------------------------------------------------------------------------
// The Marble Object
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Animation
//------------------------------------------------------------------------------
static int anm_marble2[] =
	{
	SPR_MARBLE_1,
	SPR_MARBLE_2,
	SPR_MARBLE_3,
	SPR_MARBLE_4,
	ANM_RESTART
	};

static int anm_marble[] =
	{
	SPR_MARBLE_4,
	SPR_MARBLE_3,
	SPR_MARBLE_2,
	SPR_MARBLE_1,
	ANM_RESTART
	};

static OBJSIZE marble_size = { 6, 10, 4, 5 };
static OBJMOVE marble_move = {4*256,0,8*256,8*256,145,8*256,8*256};

//------------------------------------------------------------------------------
//! \brief Initialise the marble
//------------------------------------------------------------------------------
CMarbleObj::CMarbleObj()
{
	m_Frame = SPR_MARBLE_1;
	m_pSequence = anm_marble;
	InitSize(marble_size);
	InitMovement(marble_move);
	m_Flags = FLAG_WEAPON;
}

//------------------------------------------------------------------------------
//! \brief Setup the marble
//!
//! (This function also sets the inertias)
//!
//! 	\param xpos = Where to display the marble X
//! 	\param ypos = Where to display the marble Y
//------------------------------------------------------------------------------
void CMarbleObj::Setup(int xpos, int ypos)
{
	m_OldXPos = m_XPos = xpos;
	m_OldYPos = m_YPos = ypos;
	m_XInert = (rand() & 0xfff) - 0x7ff;
	m_YInert = (rand() & 0xfff) - 0x7ff;

	// Prevent a stationary marble droplet
	if (m_XInert<0)
	{
		if (m_XInert > -256) m_XInert-=256;
	}else
	{
		if (m_XInert < 256) m_XInert+=256;
	}

}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CMarbleObj::Do( void )
{
	MoveObject();
	if (m_State&STATE_LEFT) SetMoveRight();
	if (m_State&STATE_RIGHT) SetMoveLeft();

	AnimateDir(anm_marble, anm_marble2, 256);
}

//------------------------------------------------------------------------------
// The NoteMan Object
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Animation
//------------------------------------------------------------------------------
static int anm_noteman[] =
	{
	SPR_NOTES_MAN1,
	SPR_NOTES_MAN2,
	ANM_RESTART
	};

static OBJSIZE noteman_size = { 4, 4, 24, 23 };
static OBJMOVE noteman_move = {250,0,5*256,250,0,6*256,0};

#define NOTELEVEL_LENGTH	500

// The NoteMan Music
typedef struct _MANMUSIC
	{
		int delay;
		int rate;
	} MANMUSIC;
#define MUSIC_SIZE	18
MANMUSIC notemusic[MUSIC_SIZE] =
	{
		{1,11000},
		{2,8500},
		{5,10000},
		{6,9000},
		{4,7500},
		{3,8000},
		{1,10000},
		{2,11000},
		{5,7500},
		{6,8500},
		{4,7500},
		{3,9000},
		{1,8500},
		{2,10000},
		{5,8000},
		{6,11000},
		{4,9000},
		{3,8500}
	};

//------------------------------------------------------------------------------
//! \brief Initialise the noteman
//------------------------------------------------------------------------------
CNoteManObj::CNoteManObj()
{
	m_Frame = SPR_NOTES_MAN1;
	m_pSequence = anm_noteman;
	InitSize(noteman_size);
	InitMovement(noteman_move);
	m_Dir = DIR_RIGHT;
	m_ReleaseDelay = 0;
	m_TickTock=0;
	m_MusicOffset=0;
	m_MusicDelay=0;
	m_LevelLength = NOTELEVEL_LENGTH;
}

//------------------------------------------------------------------------------
//! \brief Load the object graphics
//------------------------------------------------------------------------------
void CNoteManObj::LoadGfx( void )
{
	m_pGame->m_Sprites.LoadRange(SPR_NOTES_G1, SPR_NOTES_MAN2);
}

//------------------------------------------------------------------------------
//! \brief Do the noteman releasing notes
//------------------------------------------------------------------------------
void CNoteManObj::ReleaseNotes( void )
{
	m_pGame->RandGoodie( m_XPos, m_YPos, GOODIE_NOTES, m_TickTock );
	m_TickTock^=1;

}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CNoteManObj::Do( void )
{

	CPlayerObj *pobj;

	pobj = HitPlayer();
	if (pobj)			// Collision occured?
	{
		m_pGame->m_BonusCompleteFlag = 1;
		return ;
	}
 
	DirFly();
	Animate(64);
	m_ReleaseDelay++;
	if (!(m_ReleaseDelay&1))
	{
		ReleaseNotes();
	}
	m_LevelLength--;
	if (m_LevelLength<0)
	{
		m_pGame->m_BonusCompleteFlag = 1;
	}else
	{
		m_pGame->m_CountDown = m_LevelLength>>4;
	}
	m_MusicDelay--;
	if (m_MusicDelay<0)
	{
		m_MusicOffset++;
		if (m_MusicOffset>=MUSIC_SIZE) m_MusicOffset = 0;

		m_MusicDelay = notemusic[m_MusicOffset].delay;
		int rate = notemusic[m_MusicOffset].rate;
	  	m_pGame->m_pSound->PlaySample(SND_XYLO, m_XPos, rate);
	}

}

//------------------------------------------------------------------------------
// The TreasSpot Object
//------------------------------------------------------------------------------

static OBJSIZE treasspot_size = { 0, 0, 16, 16 };

//------------------------------------------------------------------------------
//! \brief Initialise the treasspot
//------------------------------------------------------------------------------
CTreasSpotObj::CTreasSpotObj()
{
	InitSize(treasspot_size);
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CTreasSpotObj::Do( void )
{
	int type;
	CGasObj *pobj;
	pobj = (CGasObj *) m_pGame->m_GasList.m_pFirst;
	for (;pobj;pobj = (CGasObj *) pobj->m_pNext)
	{
		type = pobj->m_Type;
		if (type==OBJ_GAS)
		{
			if (pobj->m_GasCmd<=GAS_MOVE)
			{
				if (CheckHit( pobj ))
				{
					m_pGame->m_TreasSpotCnt--;
					if (!m_pGame->m_TreasSpotCnt)	// All hot spots hit?
					{
						// Goto the treasure room
						m_pGame->SetBonusLevel(BLEV_TREASROOM);
					}else
					{
						m_pGame->RandGoodie(m_XPos, m_YPos, GOODIE_FOOD, 0, -7*256);
					}
					DeleteObject();
				}
			}
		}
	}

}

//------------------------------------------------------------------------------
// The CardRoom Object
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//! \brief Initialise the cardroom
//------------------------------------------------------------------------------
CCardRoomObj::CCardRoomObj()
{
	m_FrameOffset1 = 0;
	m_FrameOffset2 = 3;
	m_FrameOffset3 = 6;
	m_FrameOffset4 = 9;
	m_Counter = 220;
}

//------------------------------------------------------------------------------
//! \brief Draw the bug object
//------------------------------------------------------------------------------
void CCardRoomObj::Draw( void )
{
	m_pGame->m_Sprites.Draw( anm_card_spade[m_FrameOffset1], 16*2, 112 );
	m_pGame->m_Sprites.Draw( anm_card_heart[m_FrameOffset2], 16*7, 112 );
	m_pGame->m_Sprites.Draw( anm_card_club[m_FrameOffset3], 16*11, 112 );
	m_pGame->m_Sprites.Draw( anm_card_diamond[m_FrameOffset4], 16*16, 112 );
}

//------------------------------------------------------------------------------
//! \brief Load the object graphics
//------------------------------------------------------------------------------
void CCardRoomObj::LoadGfx( void )
{
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CCardRoomObj::Do( void )
{
	int delay;
	m_Counter--;

	if (m_Counter==180)
	{
		delay = m_pGame->CreateMessage( 0, "WELL DONE!", 0 );
		m_pGame->CreateMessage( 64, "ALL CARDS COLLECTED", delay );
	}

	if (m_Counter==100)
	{
		// NOTE- ONLY WORKS IN ONE PLAYER MODE !!!
		delay = m_pGame->CreateMessage( 0, "PLAYER 1", 0 );
		m_pGame->CreateMessage( 64, "EXTRA LIFE", delay );
	}

	if (m_Counter<0)
	{
		m_pGame->m_BonusCompleteFlag = 1;
	}

	if (m_Counter&1)
	{
		// Cycle the borders
		m_pGame->m_Map.CycleBorder();

		// Animate the cards
		m_FrameOffset1++;
		if (m_FrameOffset1>=NUM_CARD_FRMS) m_FrameOffset1=0;
		m_FrameOffset2++;
		if (m_FrameOffset2>=NUM_CARD_FRMS) m_FrameOffset2=0;
		m_FrameOffset3++;
		if (m_FrameOffset3>=NUM_CARD_FRMS) m_FrameOffset3=0;
		m_FrameOffset4++;
		if (m_FrameOffset4>=NUM_CARD_FRMS) m_FrameOffset4=0;
	}
}

//------------------------------------------------------------------------------
// The SwitchObj Object
//------------------------------------------------------------------------------

static OBJSIZE SwitchObj_size = { 0, 0, 15, 15 };

//------------------------------------------------------------------------------
//! \brief Initialise the SwitchObj
//------------------------------------------------------------------------------
CSwitchObj::CSwitchObj()
{
	InitSize(SwitchObj_size);
	m_HitFlag = 0;
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CSwitchObj::Do( void )
{
	CPlayerObj *pobj;

	if (m_HitFlag)
	{
		if (m_pGame->m_FadeType == FADE_WHITE) return;

		m_pGame->m_FadeType = FADE_NORMAL;
		m_pGame->m_FadeFlag = FADE_FLAG_CONTINUE;
		m_pGame->m_Map.LoadSwapMap(m_pGame->m_LevelNumber);
		DeleteObject();
		return;
	}

	pobj = HitPlayer();
	if (pobj)			// Collision occured?
	{
		m_HitFlag = 1;
		m_pGame->m_FadeType = FADE_WHITE;
		m_pGame->m_FadeFlag = FADE_FLAG_CONTINUE;
	}
}

//------------------------------------------------------------------------------
// The KeyRoom Object
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// The Text
//------------------------------------------------------------------------------

static const char *key_txt_1a[]={"GREAT YOU GOT THE", "FIRST PART", 0};
static const char *key_txt_1b[]={"ONLY 3 PARTS OF THE", "KEY LEFT TO GET", 0};
static const char *key_txt_1c[]={"NOW YOU HAVE THE", "FIRST PART","WHERE ARE THE REST",0};

static const char *key_txt_2a[]={"THAT IS 2 PIECES", "HALF WAY THERE", 0};
static const char *key_txt_2b[]={"DO NOT WORRY THE", "FIRST 2 WERE HARD","THE REST IS EASY",0};
static const char *key_txt_2c[]={"ANOTHER PIECE ", "ONLY 2 TO GO", 0};

static const char *key_txt_3a[]={"3 PIECES YOU ARE", "DOING WELL", 0};
static const char *key_txt_3b[]={"LUCKY 3 JUST 1 MORE", "TO GET", 0};
static const char *key_txt_3c[]={"ONLY 1 MORE TO GO", "WILL YOU MAKE IT",0};

static const char *key_txt_4a[]={"GOT ALL THE PARTS", "WELL DONE YOU CAN","NOW ESCAPE",0};
static const char *key_txt_4b[]={"THAT IS IT THE GAME", "IS COMPLETED", 0};
static const char *key_txt_4c[]={"WELL DONE YOU HAVE", "COMPLETED","METHANE BROTHERS",0};

static const char **key_txt_1[] = {key_txt_1a,key_txt_1b,key_txt_1c};
static const char **key_txt_2[] = {key_txt_2a,key_txt_2b,key_txt_2c};
static const char **key_txt_3[] = {key_txt_3a,key_txt_3b,key_txt_3c};
static const char **key_txt_4[] = {key_txt_4a,key_txt_4b,key_txt_4c};

static const char ***key_txt[] = {key_txt_1,key_txt_2,key_txt_3,key_txt_4};

//------------------------------------------------------------------------------
//! \brief Initialise the keyroom
//------------------------------------------------------------------------------
CKeyRoomObj::CKeyRoomObj()
{
	m_Counter = 180;
	m_KeyType = 0;
}

//------------------------------------------------------------------------------
//! \brief Draw the bug object
//------------------------------------------------------------------------------
void CKeyRoomObj::Draw( void )
{
	m_pGame->m_Sprites.Draw( SPR_KEY_1, 0x80, 0x50 );
	if (m_KeyType>=2)
		m_pGame->m_Sprites.Draw( SPR_KEY_2, 0xa0, 0x50 );
	if (m_KeyType>=3)
		m_pGame->m_Sprites.Draw( SPR_KEY_3, 0x90, 0x80 );
	if (m_KeyType>=4)
		m_pGame->m_Sprites.Draw( SPR_KEY_4, 0xa0, 0x80 );
}


//------------------------------------------------------------------------------
//! \brief Setup the keyroom
//!
//! 	\param keytype = How many keys collected (1 to 4)
//------------------------------------------------------------------------------
void CKeyRoomObj::Setup( int keytype )
{
	int delay;

	int offset;
	const char ***bunch;
	const char **txtset;

	keytype--;	// 0 to 3
	keytype = keytype&3;	// Validate it is 0 to 3

	m_KeyType = keytype+1;

	bunch = key_txt[keytype];
	offset = rand() % 3;	// 0,1,2

	txtset = bunch[offset];

	delay = 32;
	for (; *txtset; txtset++)
	{
		delay = m_pGame->CreateMessage( 0, *txtset, delay );
	}
}

//------------------------------------------------------------------------------
//! \brief Load the object graphics
//------------------------------------------------------------------------------
void CKeyRoomObj::LoadGfx( void )
{
	m_pGame->m_Sprites.LoadRange(SPR_KEY_1, SPR_KEY_4);
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CKeyRoomObj::Do( void )
{
	m_Counter--;

	if (m_Counter<0)
	{
		m_pGame->m_BonusCompleteFlag = 1;
	}

	if (m_Counter&1)
	{
		// Cycle the borders
		m_pGame->m_Map.CycleBorder();

	}
}

//------------------------------------------------------------------------------
// The DropOffObj Object
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//! \brief Initialise the DropOffObj
//------------------------------------------------------------------------------
CDropOffObj::CDropOffObj()
{
}

//------------------------------------------------------------------------------
//! \brief Draw the object
//------------------------------------------------------------------------------
void CDropOffObj::Draw( void )
{
	if (m_Frame) m_pGame->m_Sprites.Draw( m_Frame, m_XPos, m_YPos, GFX_NOWRAP );
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CDropOffObj::Do( void )
{
	m_YPos+=8;
	if (m_YPos > (SCR_HEIGHT+16))
 	{
		DeleteObject();
	}
}

//------------------------------------------------------------------------------
// The BalloonObj Object
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//! \brief Initialise the BalloonObj
//------------------------------------------------------------------------------
CBalloonObj::CBalloonObj()
{
	m_Rate = 8;
}

//------------------------------------------------------------------------------
//! \brief Draw the object
//------------------------------------------------------------------------------
void CBalloonObj::Draw( void )
{
	if (m_Frame) m_pGame->m_Sprites.Draw( m_Frame, m_XPos, m_YPos, GFX_NOWRAP );
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CBalloonObj::Do( void )
{
	m_YPos-=m_Rate;
	if (m_YPos < (-64))
 	{
		DeleteObject();
	}
}


