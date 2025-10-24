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
// Methane Brothers Goodie Object (Source File)
//------------------------------------------------------------------------------

#include "stdio.h"
#include "stdlib.h"
#include "goodie.h"
#include "global.h"
#include "bitgroup.h"
#include "player.h"
#include "objtypes.h"
#include "misc.h"
#include "game.h"

//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------
#define PICKUPDELAY	20 	// Delay before you can pick up the object

#define GOODSTDLEN	180	// How long a goodie lasts for

#define SPECIAL_PICKUP	65	// Amount of items to pickup before the special bonuses appear

//------------------------------------------------------------------------------
// Goodie Commands
//------------------------------------------------------------------------------
#define GC_NORMAL	0
#define GC_EXPLODE	1

//------------------------------------------------------------------------------
// Goodie Flags (BIT MASKS)
//------------------------------------------------------------------------------
#define GC_DRIVE	1	// IE CAR or TRAIN
#define GC_FLOWER	2	// A flower
#define GC_SAUSAGE	4	// A Sausage
#define GC_ANIM		8	// Standard animation

//------------------------------------------------------------------------------
// Animation Frames
//------------------------------------------------------------------------------

static int anm_explode[] =
	{
		SPR_EXP_1,SPR_EXP_2,SPR_EXP_3,SPR_EXP_4,SPR_EXP_5,SPR_EXP_6,SPR_EXP_7,
		SPR_EXP_8,SPR_EXP_9,SPR_EXP_10,SPR_EXP_11,SPR_EXP_12,SPR_EXP_13,
		ANM_RESTART

	};

static int anm_car_left[] =
	{
		SPR_GOOD_CAR_L1,SPR_GOOD_CAR_L2,
		ANM_RESTART
	};

static int anm_car_right[] =
	{
		SPR_GOOD_CAR_R1,SPR_GOOD_CAR_R2,
		ANM_RESTART
	};

static int anm_train_left[] =
	{
		SPR_GOOD_TRAIN_L1,SPR_GOOD_TRAIN_L2,SPR_GOOD_TRAIN_L3,SPR_GOOD_TRAIN_L4,
		ANM_RESTART
	};

static int anm_train_right[] =
	{
		SPR_GOOD_TRAIN_R1,SPR_GOOD_TRAIN_R2,SPR_GOOD_TRAIN_R3,SPR_GOOD_TRAIN_R4,
		ANM_RESTART
	};

static int anm_flower1[] =
	{
		SPR_FLOWERS_1,SPR_FLOWERS_2,SPR_FLOWERS_3,SPR_FLOWERS_4,
		SPR_FLOWERS_5,SPR_FLOWERS_6,
		SPR_FLOWERS_A1,
		SPR_FLOWERS_A2,
		SPR_FLOWERS_A3,
		ANM_PAUSE
	};
static int anm_flower2[] =
	{
		SPR_FLOWERS_1,SPR_FLOWERS_2,SPR_FLOWERS_3,SPR_FLOWERS_4,
		SPR_FLOWERS_5,SPR_FLOWERS_6,
		SPR_FLOWERS_B1,
		SPR_FLOWERS_B2,
		SPR_FLOWERS_B3,
		ANM_PAUSE
	};
static int anm_flower3[] =
	{
		SPR_FLOWERS_1,SPR_FLOWERS_2,SPR_FLOWERS_3,SPR_FLOWERS_4,
		SPR_FLOWERS_5,SPR_FLOWERS_6,
		SPR_FLOWERS_C1,
		SPR_FLOWERS_C2,
		SPR_FLOWERS_C3,
		ANM_PAUSE
	};
static int anm_flower4[] =
	{
		SPR_FLOWERS_1,SPR_FLOWERS_2,SPR_FLOWERS_3,SPR_FLOWERS_4,
		SPR_FLOWERS_5,SPR_FLOWERS_6,
		SPR_FLOWERS_D1,
		SPR_FLOWERS_D2,
		SPR_FLOWERS_D3,
		ANM_PAUSE
	};

static int anm_sausage_left[] =
	{
		SPR_SAUSAGE_1,
		SPR_SAUSAGE_2,
		SPR_SAUSAGE_3,
		SPR_SAUSAGE_4,
		SPR_SAUSAGE_5,
		SPR_SAUSAGE_6,
		SPR_SAUSAGE_7,
		SPR_SAUSAGE_8,
		ANM_RESTART
	};

static int anm_sausage_right[] =
	{
		SPR_SAUSAGE_8,
		SPR_SAUSAGE_7,
		SPR_SAUSAGE_6,
		SPR_SAUSAGE_5,
		SPR_SAUSAGE_4,
		SPR_SAUSAGE_3,
		SPR_SAUSAGE_2,
		SPR_SAUSAGE_1,
		ANM_RESTART
	};

static int anm_notes_g[] =
	{
		SPR_NOTES_G1,SPR_NOTES_G2,SPR_NOTES_G3,SPR_NOTES_G4,SPR_NOTES_G5,SPR_NOTES_G6,
		SPR_NOTES_G6,SPR_NOTES_G5,SPR_NOTES_G4,SPR_NOTES_G3,SPR_NOTES_G2,SPR_NOTES_G1,
		ANM_RESTART
	};

static int anm_notes_p[] =
	{
		SPR_NOTES_P1,SPR_NOTES_P2,SPR_NOTES_P3,SPR_NOTES_P4,SPR_NOTES_P5,SPR_NOTES_P6,
		SPR_NOTES_P6,SPR_NOTES_P5,SPR_NOTES_P4,SPR_NOTES_P3,SPR_NOTES_P2,SPR_NOTES_P1,
		ANM_RESTART
	};

static int anm_notes_y[] =
	{
		SPR_NOTES_Y1,SPR_NOTES_Y2,SPR_NOTES_Y3,SPR_NOTES_Y4,SPR_NOTES_Y5,SPR_NOTES_Y6,
		SPR_NOTES_Y6,SPR_NOTES_Y5,SPR_NOTES_Y4,SPR_NOTES_Y3,SPR_NOTES_Y2,SPR_NOTES_Y1,
		ANM_RESTART
	};

static int anm_notes_b[] =
	{
		SPR_NOTES_B1,SPR_NOTES_B2,SPR_NOTES_B3,SPR_NOTES_B4,SPR_NOTES_B5,SPR_NOTES_B6,
		SPR_NOTES_B6,SPR_NOTES_B5,SPR_NOTES_B4,SPR_NOTES_B3,SPR_NOTES_B2,SPR_NOTES_B1,
		ANM_RESTART
	};

static int anm_notes_r[] =
	{
		SPR_NOTES_R1,SPR_NOTES_R2,SPR_NOTES_R3,SPR_NOTES_R4,SPR_NOTES_R5,SPR_NOTES_R6,
		SPR_NOTES_R6,SPR_NOTES_R5,SPR_NOTES_R4,SPR_NOTES_R3,SPR_NOTES_R2,SPR_NOTES_R1,
		ANM_RESTART
	};

static int anm_cheese[] =
	{
		SPR_TREAS_CHEESEB1,
		SPR_TREAS_CHEESEB1,
		SPR_TREAS_CHEESEB2,
		SPR_TREAS_CHEESEB2,
		SPR_TREAS_CHEESEB3,
		SPR_TREAS_CHEESEB3,
		ANM_RESTART
	};

static int anm_gems_y[] =
	{
		SPR_TREAS_GEMSY1,
		SPR_TREAS_GEMSY1,
		SPR_TREAS_GEMSY2,
		SPR_TREAS_GEMSY3,
		SPR_TREAS_GEMSY3,
		SPR_TREAS_GEMSY2,
		ANM_RESTART
	};
static int anm_gems_g[] =
	{
		SPR_TREAS_GEMSG1,
		SPR_TREAS_GEMSG1,
		SPR_TREAS_GEMSG2,
		SPR_TREAS_GEMSG3,
		SPR_TREAS_GEMSG3,
		SPR_TREAS_GEMSG2,
		ANM_RESTART
	};
static int anm_gems_b[] =
	{
		SPR_TREAS_GEMSB1,
		SPR_TREAS_GEMSB1,
		SPR_TREAS_GEMSB2,
		SPR_TREAS_GEMSB3,
		SPR_TREAS_GEMSB3,
		SPR_TREAS_GEMSB2,
		ANM_RESTART
	};
static int anm_gems_p[] =
	{
		SPR_TREAS_GEMSP1,
		SPR_TREAS_GEMSP1,
		SPR_TREAS_GEMSP2,
		SPR_TREAS_GEMSP3,
		SPR_TREAS_GEMSP3,
		SPR_TREAS_GEMSP2,
		ANM_RESTART
	};

static int anm_gems_r[] =
	{
		SPR_TREAS_GEMSR1,
		SPR_TREAS_GEMSR1,
		SPR_TREAS_GEMSR2,
		SPR_TREAS_GEMSR3,
		SPR_TREAS_GEMSR3,
		SPR_TREAS_GEMSR2,
		ANM_RESTART
	};
static int anm_coins[] =
	{
		SPR_TREAS_MONEY1,
		SPR_TREAS_MONEY1,
		SPR_TREAS_MONEY2,
		SPR_TREAS_MONEY2,
		SPR_TREAS_MONEY3,
		SPR_TREAS_MONEY3,
		SPR_TREAS_MONEY4,
		SPR_TREAS_MONEY4,
		SPR_TREAS_MONEY5,
		SPR_TREAS_MONEY5,
		SPR_TREAS_MONEY6,
		SPR_TREAS_MONEY6,
		ANM_RESTART
	};



int *group_car[2] = {anm_car_left, anm_car_right };
int *group_train[2] = {anm_train_left, anm_train_right };
int *group_flower1[2] = {anm_flower1, anm_flower1 };
int *group_flower2[2] = {anm_flower2, anm_flower2 };
int *group_flower3[2] = {anm_flower3, anm_flower3 };
int *group_flower4[2] = {anm_flower4, anm_flower4 };
int *group_sausage[2] = {anm_sausage_left, anm_sausage_right };
int *group_notes_g[2] = {anm_notes_g, anm_notes_g };
int *group_notes_p[2] = {anm_notes_p, anm_notes_p };
int *group_notes_y[2] = {anm_notes_y, anm_notes_y };
int *group_notes_b[2] = {anm_notes_b, anm_notes_b };
int *group_notes_r[2] = {anm_notes_r, anm_notes_r };
int *group_cheese[2] = {anm_cheese, anm_cheese };
int *group_gems_r[2] = {anm_gems_r, anm_gems_r };
int *group_gems_g[2] = {anm_gems_g, anm_gems_g };
int *group_gems_b[2] = {anm_gems_b, anm_gems_b };
int *group_gems_p[2] = {anm_gems_p, anm_gems_p };
int *group_gems_y[2] = {anm_gems_y, anm_gems_y };
int *group_coins[2] = {anm_coins, anm_coins };

//------------------------------------------------------------------------------
// Object Details
//------------------------------------------------------------------------------
static OBJSIZE goodie_size = {0,0,16,15};
static OBJSIZE goodiecheese_size = {0,0,16,15+8};
static OBJSIZE goodflower_size = {4,0,8,15};
static OBJSIZE goodie_size2 = {0,0,16,31};
static OBJMOVE goodie_move = {165,70,9*256,8*256,165,7*256,7*256};
static OBJMOVE goodsausage_move = {6*256,0,6*256,9*256,165,9*256,9*256};
static OBJMOVE goodcar_move = {200,100,5*256,8*256,165,7*256,7*256};
static OBJSIZE goodtrain_size = {0,0,16,22};
static OBJMOVE goodtrain_move = {80,80/2,5*256,8*256,165,7*256,7*256};
static OBJSIZE goodrock_size = {0,14,16,15};
static OBJSIZE goodkite_size = {0,16,16,15};
static OBJSIZE goodchip_size = {0,0,16,14};
static OBJSIZE goodcake_size = {0,0,16,13};
static OBJSIZE goodnotes_size = {0,0,16,18};

//------------------------------------------------------------------------------
// Goodie Lists
//------------------------------------------------------------------------------
GOODIE_TYPE toy_list[MAX_TOYS] =
	{
		{SPR_GOOD_BOAT,		100, SPR_NUM_100, 0, 0, SND_PICKUP1,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_GOOD_BALL,		200, SPR_NUM_200, 0, 0, SND_TRIBBLE,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_GOOD_TOP,		300, SPR_NUM_300, 0, 0, SND_PICKUP1,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_GOOD_BRICKS,	400, SPR_NUM_400, 0, 0, SND_TRIBBLE,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_GOOD_TEDDY,	300, SPR_NUM_300, 0, 0, SND_PICKUP1,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_GOOD_JOYSTICK,	200, SPR_NUM_200, 0, 0, SND_TRIBBLE,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_GOOD_DUCK,		100, SPR_NUM_100, 0, 0, SND_DUCK,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_GOOD_TANK,		200, SPR_NUM_200, 0, 0, SND_TRIBBLE,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_GOOD_CAR_R1,	700, SPR_NUM_700, GC_DRIVE, group_car, SND_CAR,
			&goodie_size, &goodcar_move,GOODSTDLEN},
		{SPR_GOOD_TRAIN_L1,	800, SPR_NUM_800, GC_DRIVE, group_train,SND_TRAIN,
			&goodtrain_size, &goodtrain_move,GOODSTDLEN},
		{SPR_GOOD_PHONE,	100, SPR_NUM_100, 0, 0, SND_PICKUP1,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_GOOD_PRESENT,	200, SPR_NUM_200, 0, 0, SND_TRIBBLE,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_GOOD_UFO,		300, SPR_NUM_300, 0, 0, SND_PICKUP1,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_GOOD_SKATE,	400, SPR_NUM_400, 0, 0, SND_TRIBBLE,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_GOOD_ROCKET,	500, SPR_NUM_500, 0, 0,	SND_PICKUP1,
			&goodrock_size, &goodie_move,GOODSTDLEN},
		{SPR_GOOD_KITE,		600, SPR_NUM_600, 0, 0, SND_TRIBBLE,
			&goodkite_size, &goodie_move,GOODSTDLEN}
	};

GOODIE_TYPE food_list[MAX_FOOD] =
	{
		{SPR_FOOD_1,	100, SPR_NUM_100, 0, 0,	SND_TRIBBLE,
			&goodchip_size, &goodie_move,GOODSTDLEN},
		{SPR_FOOD_2,	200, SPR_NUM_200, 0, 0, SND_PICKUP1,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_FOOD_3,	300, SPR_NUM_300, 0, 0,	SND_TRIBBLE,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_FOOD_4,	400, SPR_NUM_400, 0, 0, SND_PICKUP1,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_FOOD_5,	500, SPR_NUM_500, 0, 0,	SND_TRIBBLE,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_FOOD_6,	500, SPR_NUM_500, 0, 0, SND_PICKUP1,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_FOOD_7,	400, SPR_NUM_400, 0, 0,	SND_TRIBBLE,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_FOOD_8,	300, SPR_NUM_300, 0, 0, SND_PICKUP1,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_FOOD_9,	200, SPR_NUM_200, 0, 0,	SND_TRIBBLE,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_FOOD_10,	100, SPR_NUM_100, 0, 0, SND_PICKUP1,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_FOOD_11,	100, SPR_NUM_100, 0, 0,	SND_TRIBBLE,
			&goodcake_size, &goodie_move,GOODSTDLEN},
		{SPR_FOOD_12,	200, SPR_NUM_200, 0, 0, SND_PICKUP1,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_FOOD_13,	300, SPR_NUM_300, 0, 0,	SND_TRIBBLE,
			&goodie_size, &goodie_move,GOODSTDLEN}
	};

GOODIE_TYPE fruit_list[MAX_FRUIT] = 
	{
		{SPR_FRUIT_1,	100, SPR_NUM_100, 0, 0,	SND_TRIBBLE,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_FRUIT_2,	200, SPR_NUM_200, 0, 0, SND_PICKUP1,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_FRUIT_3,	300, SPR_NUM_300, 0, 0,	SND_TRIBBLE,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_FRUIT_4,	400, SPR_NUM_400, 0, 0, SND_PICKUP1,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_FRUIT_5,	300, SPR_NUM_300, 0, 0,	SND_TRIBBLE,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_FRUIT_6,	200, SPR_NUM_200, 0, 0, SND_PICKUP1,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_FRUIT_7,	100, SPR_NUM_100, 0, 0,	SND_TRIBBLE,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_FRUIT_8,	200, SPR_NUM_200, 0, 0, SND_PICKUP1,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_FRUIT_9,	300, SPR_NUM_300, 0, 0,	SND_TRIBBLE,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_FRUIT_10,	400, SPR_NUM_400, 0, 0, SND_PICKUP1,
			&goodie_size, &goodie_move,GOODSTDLEN}
	};

GOODIE_TYPE comp_list[MAX_COMP] = 
	{
		{SPR_COMP_01,	100, SPR_NUM_100, 0, 0,	SND_TRIBBLE,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_COMP_02,	200, SPR_NUM_200, 0, 0,	SND_PICKUP1,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_COMP_03,	300, SPR_NUM_300, 0, 0,	SND_TRIBBLE,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_COMP_04,	400, SPR_NUM_400, 0, 0,	SND_PICKUP1,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_COMP_05,	500, SPR_NUM_500, 0, 0,	SND_TRIBBLE,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_COMP_06,	600, SPR_NUM_600, 0, 0,	SND_PICKUP1,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_COMP_07,	700, SPR_NUM_700, 0, 0,	SND_TRIBBLE,
			&goodie_size2, &goodie_move,GOODSTDLEN},
		{SPR_COMP_08,	800, SPR_NUM_800, 0, 0,	SND_PICKUP1,
			&goodie_size2, &goodie_move,GOODSTDLEN},
		{SPR_COMP_09,	900, SPR_NUM_900, 0, 0,	SND_TRIBBLE,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_COMP_10,	100, SPR_NUM_100, 0, 0,	SND_PICKUP1,
			&goodie_size2, &goodie_move,GOODSTDLEN},
		{SPR_COMP_11,	200, SPR_NUM_200, 0, 0,	SND_TRIBBLE,
			&goodie_size2, &goodie_move,GOODSTDLEN},
		{SPR_COMP_12,	300, SPR_NUM_300, 0, 0,	SND_PICKUP1,
			&goodie_size2, &goodie_move,GOODSTDLEN},
		{SPR_COMP_13,	400, SPR_NUM_400, 0, 0,	SND_TRIBBLE,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_COMP_14,	500, SPR_NUM_500, 0, 0,	SND_PICKUP1,
			&goodie_size2, &goodie_move,GOODSTDLEN},
		{SPR_COMP_15,	600, SPR_NUM_600, 0, 0,	SND_TRIBBLE,
			&goodie_size2, &goodie_move,GOODSTDLEN},
		{SPR_COMP_16,	700, SPR_NUM_700, 0, 0,	SND_PICKUP1,
			&goodie_size2, &goodie_move,GOODSTDLEN},
		{SPR_COMP_17,	800, SPR_NUM_800, 0, 0,	SND_TRIBBLE,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_COMP_18,	900, SPR_NUM_900, 0, 0,	SND_PICKUP1,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_COMP_19,	100, SPR_NUM_100, 0, 0,	SND_TRIBBLE,
			&goodie_size2, &goodie_move,GOODSTDLEN},
		{SPR_COMP_20,	200, SPR_NUM_200, 0, 0,	SND_PICKUP1,
			&goodie_size2, &goodie_move,GOODSTDLEN},

	};

GOODIE_TYPE flowers_list[MAX_FLOWERS] = 
	{
		{SPR_FLOWERS_SEED,	100, SPR_NUM_100, GC_FLOWER, group_flower1,	SND_TRIBBLE,
			&goodflower_size, &goodie_move,GOODSTDLEN},
		{SPR_FLOWERS_SEED,	100, SPR_NUM_100, GC_FLOWER, group_flower2,	SND_PICKUP1,
			&goodflower_size, &goodie_move,GOODSTDLEN},
		{SPR_FLOWERS_SEED,	100, SPR_NUM_100, GC_FLOWER, group_flower3,	SND_TRIBBLE,
			&goodflower_size, &goodie_move,GOODSTDLEN},
		{SPR_FLOWERS_SEED,	100, SPR_NUM_100, GC_FLOWER, group_flower4,	SND_PICKUP1,
			&goodflower_size, &goodie_move,GOODSTDLEN}
	};

GOODIE_TYPE sausage_list[MAX_SAUSAGE] = 
	{
		{SPR_SAUSAGE_1,	100, SPR_NUM_100, GC_SAUSAGE, group_sausage,	SND_TRIBBLE,
			&goodie_size, &goodsausage_move,GOODSTDLEN*2},
	};

GOODIE_TYPE notes_list[MAX_NOTES] =
	{
		{SPR_NOTES_G1,		100, SPR_NUM_100, GC_ANIM, group_notes_g, SND_PICKUP1,
			&goodnotes_size, &goodie_move,GOODSTDLEN/3},
		{SPR_NOTES_P1,		200, SPR_NUM_200, GC_ANIM, group_notes_p, SND_PICKUP1,
			&goodnotes_size, &goodie_move,GOODSTDLEN/3},
		{SPR_NOTES_Y1,		300, SPR_NUM_300, GC_ANIM, group_notes_y, SND_PICKUP1,
			&goodnotes_size, &goodie_move,GOODSTDLEN/3},
		{SPR_NOTES_B1,		400, SPR_NUM_400, GC_ANIM, group_notes_b, SND_PICKUP1,
			&goodnotes_size, &goodie_move,GOODSTDLEN/3},
		{SPR_NOTES_R1,		500, SPR_NUM_500, GC_ANIM, group_notes_r, SND_PICKUP1,
			&goodnotes_size, &goodie_move,GOODSTDLEN/3},
		{SPR_NOTES_G,		100, SPR_NUM_100, 0, 0, SND_PICKUP1,
			&goodie_size, &goodie_move,GOODSTDLEN/3},
		{SPR_NOTES_P,		200, SPR_NUM_200, 0, 0, SND_PICKUP1,
			&goodie_size, &goodie_move,GOODSTDLEN/3},
		{SPR_NOTES_Y,		300, SPR_NUM_300, 0, 0, SND_PICKUP1,
			&goodie_size, &goodie_move,GOODSTDLEN/3},
		{SPR_NOTES_B,		400, SPR_NUM_400, 0, 0, SND_PICKUP1,
			&goodie_size, &goodie_move,GOODSTDLEN/3},
		{SPR_NOTES_R,		500, SPR_NUM_500, 0, 0, SND_PICKUP1,
			&goodie_size, &goodie_move,GOODSTDLEN/3}
	};

GOODIE_TYPE cheese_list[MAX_CHEESE] =
	{
		{SPR_TREAS_CHEESE,	100, SPR_NUM_100, 0, 0,	SND_TRIBBLE,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_TREAS_CHEESEB1, 200, SPR_NUM_200, GC_ANIM, group_cheese,	SND_PICKUP1,
			&goodiecheese_size, &goodie_move,GOODSTDLEN}
	};

GOODIE_TYPE jewel_list[MAX_JEWEL] =
	{
		{SPR_TREAS_GEMSY1,	500, SPR_NUM_500, GC_ANIM, group_gems_y,	SND_TRIBBLE,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_TREAS_GEMSG1,  600, SPR_NUM_600, GC_ANIM, group_gems_g,	SND_PICKUP1,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_TREAS_GEMSB1,  700, SPR_NUM_700, GC_ANIM, group_gems_b,	SND_TRIBBLE,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_TREAS_GEMSP1,  500, SPR_NUM_500, GC_ANIM, group_gems_p,	SND_PICKUP1,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_TREAS_GEMSR1,  600, SPR_NUM_600, GC_ANIM, group_gems_r,	SND_TRIBBLE,
			&goodie_size, &goodie_move,GOODSTDLEN}
	};

GOODIE_TYPE coin_list[MAX_COIN] =
	{
		{SPR_TREAS_MONEY1,	800, SPR_NUM_800, GC_ANIM, group_coins,	SND_TRIBBLE,
			&goodie_size, &goodie_move,GOODSTDLEN}
	};

GOODIE_TYPE tres_list[MAX_TRES] =
	{
		{SPR_TREAS_GOLD1,	500, SPR_NUM_500, 0, 0,	SND_TRIBBLE,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_TREAS_GOLD2,	600, SPR_NUM_600, 0, 0,	SND_PICKUP1,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_TREAS_GOLD3,	700, SPR_NUM_700, 0, 0,	SND_TRIBBLE,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_TREAS_GOLD4,	800, SPR_NUM_800, 0, 0,	SND_PICKUP1,
			&goodie_size, &goodie_move,GOODSTDLEN},
		{SPR_TREAS_GOLD5,	900, SPR_NUM_900, 0, 0,	SND_TRIBBLE,
			&goodie_size, &goodie_move,GOODSTDLEN}
	};

GOODIE_TYPE *goodie_group[GOODIE_MAX+1] =
	{
		toy_list,
		food_list,
		fruit_list,
		comp_list,
		flowers_list,
		sausage_list,
		notes_list,
		cheese_list,
		jewel_list,
		coin_list,
		tres_list
	};


int maxvals[GOODIE_MAX+1] =
	{
		MAX_TOYS,
		MAX_FOOD,
		MAX_FRUIT,
		MAX_COMP,
		MAX_FLOWERS,
		MAX_SAUSAGE,
		MAX_NOTES,
		MAX_CHEESE,
		MAX_JEWEL,
		MAX_COIN,
		MAX_TRES
	};

//------------------------------------------------------------------------------
//! \brief Initialise the goodie object
//------------------------------------------------------------------------------
CGoodieObj::CGoodieObj()
{
	Reset();
}

//------------------------------------------------------------------------------
//! \brief Draw the goodie object
//------------------------------------------------------------------------------
void CGoodieObj::Draw( void )
{
	if (m_Frame) m_pGame->m_Sprites.Draw( m_Frame, m_XPos, m_YPos );
}

//------------------------------------------------------------------------------
//! \brief Reset the object (object members)
//------------------------------------------------------------------------------
void CGoodieObj::Reset( void )
{
	m_Y_Flag = -1;
	m_GScore = m_GFlags = 0;
	m_PickUpDelay = PICKUPDELAY;
	m_GID = 0;
	m_Command = GC_NORMAL;
	m_ppAnimSeq = 0;
}
//------------------------------------------------------------------------------
//! \brief Set the goodie to use a GOODIE_TYPE
//!
//! 	\param good = pointer to the goodie list structure
//------------------------------------------------------------------------------
void CGoodieObj::SetStruct( GOODIE_TYPE *group )
{
	m_Frame = group->startframe;
	m_ppAnimSeq = group->animseq;
	m_GFlags = group->flags;
	m_GScore = group->score;
	m_RiseScore = group->risescore;
	m_Sound = group->sound;
	m_GoodLength = group->goodlength  - ( rand()&31 );

	InitMovement(*(group->movement));
	InitSize(*(group->size));

	m_pGame->m_GoodieCollectCnt++;
}

//------------------------------------------------------------------------------
//! \brief Check to see if the object is stuck inside the wall - If it is make the object jump
//------------------------------------------------------------------------------
void CGoodieObj::CheckStuckInWall( void )
{
	int xrand, yrand;
	if ( (m_XInert) || (m_Y_Flag) ) return;	// Only if object is stopped

	if ((m_pGame->m_MainCounter+m_GID)&0xe) return;	// After 8 frames (I think!)

	if (CheckHitWall())	// Wall Hit?
	{
		xrand = rand() & 0x1ff;
		yrand = rand() & 0x1ff;
		if (xrand&1)
		{
			m_XInert = (6*256) + xrand;
		}else
		{
			m_XInert = (-6*256) - xrand;
		}
		if (yrand&1)
		{
			m_YInert = (-6*256) - yrand;
			m_Y_Flag = -1;
		}
	}

}
//------------------------------------------------------------------------------
//! \brief Move the goodie - bouncing on the floor
//------------------------------------------------------------------------------
void CGoodieObj::MoveGoodie( void )
{
	int oldyinert;

	oldyinert = m_YInert;
	MoveObject();

	if (!m_Y_Flag)	// Landed?
	{
		m_YInert = -(oldyinert>>2);	// Bounce the object
	}

	// Make sure that the object does not fall forever
	if (!m_XInert)
	{
		if (m_YInert>(6*256))	// Falling straight?
		{
			m_XInert = 2*256;	// then tap object to the right
		}
	}
}
//------------------------------------------------------------------------------
//! \brief Set the goodie to explode
//------------------------------------------------------------------------------
void CGoodieObj::Explode( void )
{
	m_YPos = m_YPos + m_Y1 + m_Y2 - 16;	// Fix the y position
	m_Y1 = 0; m_Y2 = 16;
	m_Command = GC_EXPLODE;
	SetAnim(anm_explode);
}

//------------------------------------------------------------------------------
//! \brief Do the object driving (IE car/train)
//------------------------------------------------------------------------------
void CGoodieObj::DriveGoodie( void )
{
	DirWalk();
	AnimateXInert(m_ppAnimSeq[0], m_ppAnimSeq[1]);
}

//------------------------------------------------------------------------------
//! \brief Do the object flowering
//------------------------------------------------------------------------------
void CGoodieObj::FlowerGoodie( void )
{
	m_pSequence = m_ppAnimSeq[0];
	if (!m_Y_Flag)	// Landed?
	{
		if (m_YInert>=-256)	// Not bouncing?
		{
			Animate(256);
		}
	}
}

//------------------------------------------------------------------------------
//! \brief Do the object sausaging!
//------------------------------------------------------------------------------
void CGoodieObj::SausageGoodie( void )
{
	int rndval;
	if (!m_Y_Flag)	// Landed?
	{
		rndval = rand();
		if (!(rndval&7))
		{
			SetMoveUp();
			m_XInert = (rndval & ((256*16)-1)) - 256*8;
		}
	}

	if (m_State&STATE_LEFT) SetMoveRight();

	if (m_State&STATE_RIGHT) SetMoveLeft();

	if (m_XInert<0) m_Dir = DIR_LEFT; else m_Dir = DIR_RIGHT;

	AnimateXInert(m_ppAnimSeq[0], m_ppAnimSeq[1]);
}

//------------------------------------------------------------------------------
//! \brief Check to see if a player has picked up this object
//!
//! If the object has been picked up - it will be automatically be deleted
//!
//! 	\return 0 = Not Picked Up
//------------------------------------------------------------------------------
int CGoodieObj::CheckPickedUp( void )
{
	CPlayerObj *pobj;

	pobj = HitPlayer();
	if (pobj)			// Collision occured?
	{
		switch (m_Sound)	// Check for special bonuses
		{
			case SND_CAR:
				ChkSpecial( &(m_pGame->m_CarCnt), 8, SPECIAL_PICKUP, "LOTS OF CARS" );
				break;
			case SND_TRAIN:
				ChkSpecial( &(m_pGame->m_TrainCnt), 9, SPECIAL_PICKUP+10, "LOTS OF TRAINS" );
				break;
			case SND_DUCK:
				ChkSpecial( &(m_pGame->m_DuckCnt), 6, SPECIAL_PICKUP+20, "LOTS OF DUCKS" );
				break;
		}

		pobj->AddScore( m_GScore );
		m_pGame->MakeNumRise(m_XPos, m_YPos, m_RiseScore);
		m_pGame->m_pSound->PlaySample(m_Sound, m_XPos);
		m_pGame->m_GoodieCollectFlag = 1;
		DeleteObject();
		return 1;
	}
	return 0;
}


//------------------------------------------------------------------------------
//! \brief Do the normal object
//------------------------------------------------------------------------------
void CGoodieObj::DoNormal( void )
{
	if (m_PickUpDelay>=0) m_PickUpDelay--;	// Delay before you can pick the object up
	else if (CheckPickedUp()) return;

	CheckStuckInWall();

	if (m_GFlags&GC_DRIVE)	// Drive Type?
	{
		DriveGoodie();
	}

	MoveGoodie();

	if (m_GFlags&GC_FLOWER)	// Flower Type?
	{
		FlowerGoodie();
	}

	if (m_GFlags&GC_ANIM)	// Anim Type?
	{
		m_pSequence = m_ppAnimSeq[0];
		Animate(256);
	}

	if (m_GFlags&GC_SAUSAGE)	// Sausage Type?
	{
		SausageGoodie();
	}

	m_GoodLength--;
	if (m_GoodLength<0) Explode();

}

//------------------------------------------------------------------------------
//! \brief Do the object exploding
//------------------------------------------------------------------------------
void CGoodieObj::DoExplode( void )
{
	Animate(256);
	if (m_State&STATE_ANM_RESTART)	// Anim finished
	{
		DeleteObject();
	}
}
//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CGoodieObj::Do( void )
{
	switch (m_Command)
	{
		case GC_NORMAL:
		{
			DoNormal();
			break;
		}
		case GC_EXPLODE:
		{
			DoExplode();
			break;
		}
	}
}

//------------------------------------------------------------------------------
//! \brief Check to see if the special bonus will occur when a toy is pickup up
//!
//! 	\param cntptr = Pointer to the object counter
//!	\param toynumber = Toy offset number to create if the bonus occurs
//!	\param max = Number of objects to pickup before the toy explosion
//!	\param text = Message to display
//------------------------------------------------------------------------------
void CGoodieObj::ChkSpecial( int *cntptr, int toynumber, int max, const char *text )
{
	int cnt;
	(*cntptr)++;
	cnt = *cntptr;
	if (cnt>=max)		// Collected enough objects?
	{
		*cntptr = 0;	// Reset the counter
		for (int cnt=0; cnt<48; cnt++)
		{
				m_pGame->MakeGoodie( rand() % SCR_WIDTH, rand() % SCR_HEIGHT,
				GOODIE_TOYS, toynumber,
				(rand() & ((256*16)-1)) - 256*8,
				(rand() & ((256*16)-1)) - 256*8);
		}
		m_pGame->CreateMessage( 0, text, 0 );
	}
}

//------------------------------------------------------------------------------
// Object Details
//------------------------------------------------------------------------------
static OBJSIZE hotspot_size = {0,0,16,16};

//------------------------------------------------------------------------------
//! \brief Initialise the HotSpot object
//------------------------------------------------------------------------------
CHotSpotObj::CHotSpotObj()
{
	InitSize(hotspot_size);
}

//------------------------------------------------------------------------------
//! \brief Draw the HotSpot object
//------------------------------------------------------------------------------
void CHotSpotObj::Draw( void )
{
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CHotSpotObj::Do( void )
{
	CPlayerObj *pobj;

	pobj = HitPlayer();
	if (pobj)			// Collision occured?
	{
		m_pGame->RandGoodie(m_XPos, m_YPos, GOODIE_FRUIT, -6*256, -6*256);
		m_pGame->RandGoodie(m_XPos, m_YPos, GOODIE_FRUIT, 0, -7*256);
		m_pGame->RandGoodie(m_XPos, m_YPos, GOODIE_FRUIT, 6*256, -6*256);
		DeleteObject();
	}
}

//------------------------------------------------------------------------------
// Sucking in frames
//------------------------------------------------------------------------------

static SUCKANIM suck_tomato_right[NUMSUCKANIM] =
	{
		{SPR_TOMATO_SUCKR1, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_TOMATO_SUCKR2, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_TOMATO_SUCKR3, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_TOMATO_SUCKR4, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_TOMATO_SUCKR5, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_TOMATO_SUCKR6, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_TOMATO_SUCKR6, -6+ SUCKFIX_LX, -7+	SUCKFIX_Y},
		{SPR_TOMATO_SUCKR6, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_TOMATO_SUCKR4, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_TOMATO_SUCKR3, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_TOMATO_SUCKR2, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_TOMATO_SUCKR1, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y}
	};
static SUCKANIM suck_tomato_left[NUMSUCKANIM] =
	{
		{SPR_TOMATO_SUCKL1, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_TOMATO_SUCKL2, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_TOMATO_SUCKL3, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_TOMATO_SUCKL4, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_TOMATO_SUCKL5,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_TOMATO_SUCKL6,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_TOMATO_SUCKL6,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_TOMATO_SUCKL5,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_TOMATO_SUCKL4, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_TOMATO_SUCKL3, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_TOMATO_SUCKL2, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_TOMATO_SUCKL1, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y}
	};
//------------------------------------------------------------------------------
// Sucking Roll frames
//------------------------------------------------------------------------------
static int suck_tomato_roll[8] =
	{
		SPR_TOMATO_ROLL1,SPR_TOMATO_ROLL2,SPR_TOMATO_ROLL3,SPR_TOMATO_ROLL4,
		SPR_TOMATO_ROLL5,SPR_TOMATO_ROLL6,SPR_TOMATO_ROLL7,SPR_TOMATO_ROLL8
	};

//------------------------------------------------------------------------------
// Sucking general frames
//------------------------------------------------------------------------------
static SUCKFRAMES suck_tomato_frames=
	{
		SPR_TOMATO_SPLAT1,SPR_TOMATO_SPLAT1,
		suck_tomato_roll, 8,
		suck_tomato_left, suck_tomato_right
	};
//------------------------------------------------------------------------------
// Object Details
//------------------------------------------------------------------------------
static OBJSIZE tomato_size = {2,0,28,31};
static OBJMOVE tomato_move = {195,122,9*256,8*256,245,0x8*256,0x8*256};

//------------------------------------------------------------------------------
// Animation Frames
//------------------------------------------------------------------------------

static int tom_explode[] =
	{
		SPR_TOMATO_SPLAT1,SPR_TOMATO_SPLAT2,SPR_TOMATO_SPLAT3,
		SPR_TOMATO_SPLAT4,SPR_TOMATO_SPLAT5,SPR_TOMATO_SPLAT6,
		SPR_TOMATO_SPLAT7,SPR_TOMATO_SPLAT8,
		ANM_RESTART

	};

//------------------------------------------------------------------------------
//! \brief Initialise the tomato object
//------------------------------------------------------------------------------
CTomatoObj::CTomatoObj()
{
	m_OrigXPos = m_OrigYPos = 100;
	Reset();
}

//------------------------------------------------------------------------------
//! \brief Draw the tomato object
//------------------------------------------------------------------------------
void CTomatoObj::Draw( void )
{
	if (RunDrawSuck()) return;
	if (m_Frame) m_pGame->m_Sprites.Draw( m_Frame, m_XPos, m_YPos, GFX_NOWRAP );
}


//------------------------------------------------------------------------------
//! \brief Load the object graphics
//------------------------------------------------------------------------------
void CTomatoObj::LoadGfx( void )
{
	m_pGame->m_Sprites.LoadRange(SPR_TOMATO_SPLAT1, SPR_TOMATO_SUCKR6);
}

//------------------------------------------------------------------------------
//! \brief Reset the object (object members)
//------------------------------------------------------------------------------
void CTomatoObj::Reset( void )
{
	m_OldXPos = m_XPos = m_OrigXPos;
	m_OldYPos = m_YPos = m_OrigYPos;
	m_Frame = SPR_TOMATO_ROLL1;

	InitMovement(tomato_move);
	InitSize(tomato_size);
	m_Y_Flag = -1;

	m_Flags = FLAG_SUCKABLE;
	m_ExplodeFlag = 0;

	m_pSuckFrames = &suck_tomato_frames;
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CTomatoObj::Do( void )
{
	if (RunDoSuck()) return;

	if (m_ExplodeFlag)	// Tomato is exploding?
	{
		Animate(256);
		if (m_State&STATE_ANM_RESTART)	// Anim finished
		{
			Reset();
		}

	}else
	{
		MoveObject();
	}
}

//------------------------------------------------------------------------------
//! \brief Set the tomato to explode
//------------------------------------------------------------------------------
void CTomatoObj::SetExplode( void )
{
	m_SuckCmd = SUCK_NONE;
	m_Flags = 0;
	m_OldXPos = m_XPos;
	m_OldYPos = m_YPos;
	m_ExplodeFlag = 1;
	SetAnim(tom_explode);
}

//------------------------------------------------------------------------------
// Sucking in frames
//------------------------------------------------------------------------------

static SUCKANIM suck_bowling_right[NUMSUCKANIM] =
	{
		{SPR_BOWLING_SUCKR1, 26+ SUCKFIX_LX, -7+	SUCKFIX_Y},
		{SPR_BOWLING_SUCKR2, 26+ SUCKFIX_LX, -7+	SUCKFIX_Y},
		{SPR_BOWLING_SUCKR3, 26+ SUCKFIX_LX, -7+	SUCKFIX_Y},
		{SPR_BOWLING_SUCKR4, 26+ SUCKFIX_LX, -7+	SUCKFIX_Y},
		{SPR_BOWLING_SUCKR5,  7+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_BOWLING_SUCKR6, -5+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_BOWLING_SUCKR6, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_BOWLING_SUCKR6,  7+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_BOWLING_SUCKR4, 26+ SUCKFIX_LX, -7+	SUCKFIX_Y},
		{SPR_BOWLING_SUCKR3, 26+ SUCKFIX_LX, -7+	SUCKFIX_Y},
		{SPR_BOWLING_SUCKR2, 26+ SUCKFIX_LX, -7+	SUCKFIX_Y},
		{SPR_BOWLING_SUCKR1, 26+ SUCKFIX_LX, -7+	SUCKFIX_Y}
	};
static SUCKANIM suck_bowling_left[NUMSUCKANIM] =
	{
		{SPR_BOWLING_SUCKL1, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_BOWLING_SUCKL2, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_BOWLING_SUCKL3, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_BOWLING_SUCKL4, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_BOWLING_SUCKL5,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_BOWLING_SUCKL6,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_BOWLING_SUCKL6,   1+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_BOWLING_SUCKL5,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_BOWLING_SUCKL4, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_BOWLING_SUCKL3, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_BOWLING_SUCKL2, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_BOWLING_SUCKL1, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y}
	};
//------------------------------------------------------------------------------
// Sucking Roll frames
//------------------------------------------------------------------------------
static int suck_bowling_roll[8] =
	{
		SPR_BOWLING_ROLL8,SPR_BOWLING_ROLL7,SPR_BOWLING_ROLL6,SPR_BOWLING_ROLL5,
		SPR_BOWLING_ROLL4,SPR_BOWLING_ROLL3,SPR_BOWLING_ROLL2,SPR_BOWLING_ROLL1
	};

//------------------------------------------------------------------------------
// Sucking general frames
//------------------------------------------------------------------------------
static SUCKFRAMES suck_bowling_frames=
	{
		SPR_BOWLING_ROLL1,SPR_BOWLING_ROLL1,
		suck_bowling_roll, 8,
		suck_bowling_left, suck_bowling_right
	};
//------------------------------------------------------------------------------
// Object Details
//------------------------------------------------------------------------------
static OBJSIZE bowling_size = {2,4,28,27};
static OBJMOVE bowling_move = {195,122,9*256,8*256,545,0x8*256,0x8*256};

//------------------------------------------------------------------------------
//! \brief Initialise the bowling object
//------------------------------------------------------------------------------
CBowlingObj::CBowlingObj()
{
	Reset();
}

//------------------------------------------------------------------------------
//! \brief Draw the bowling object
//------------------------------------------------------------------------------
void CBowlingObj::Draw( void )
{
	if (RunDrawSuck()) return;
	if (m_Frame) m_pGame->m_Sprites.Draw( m_Frame, m_XPos, m_YPos );
}


//------------------------------------------------------------------------------
//! \brief Load the object graphics
//------------------------------------------------------------------------------
void CBowlingObj::LoadGfx( void )
{
	m_pGame->m_Sprites.LoadRange(SPR_BOWLING_ROLL1, SPR_BOWLING_SUCKR1);
}

//------------------------------------------------------------------------------
//! \brief Reset the object (object members)
//------------------------------------------------------------------------------
void CBowlingObj::Reset( void )
{
	m_Frame = SPR_BOWLING_ROLL1;

	InitMovement(bowling_move);
	InitSize(bowling_size);
	m_Y_Flag = -1;

	m_Flags = FLAG_SUCKABLE | FLAG_WEAPON;

	m_pSuckFrames = &suck_bowling_frames;
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CBowlingObj::Do( void )
{
	if (RunDoSuck()) return;

	MoveObject();
}

//------------------------------------------------------------------------------
//! \brief Set the bowling to explode
//------------------------------------------------------------------------------
void CBowlingObj::SetExplode( void )
{
	m_SuckCmd = SUCK_NONE;
	m_Flags = FLAG_SUCKABLE | FLAG_WEAPON;
}

//------------------------------------------------------------------------------
// Sucking in frames
//------------------------------------------------------------------------------

static SUCKANIM suck_block_right[NUMSUCKANIM] =
	{
		{SPR_BLOCK_SUCKR1, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_BLOCK_SUCKR2, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_BLOCK_SUCKR3, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_BLOCK_SUCKR4, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_BLOCK_SUCKR5, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_BLOCK_SUCKR6, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_BLOCK_SUCKR6, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_BLOCK_SUCKR6, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_BLOCK_SUCKR4, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_BLOCK_SUCKR3, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_BLOCK_SUCKR2, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_BLOCK_SUCKR1, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y}
	};
static SUCKANIM suck_block_left[NUMSUCKANIM] =
	{
		{SPR_BLOCK_SUCKL1, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_BLOCK_SUCKL2, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_BLOCK_SUCKL3, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_BLOCK_SUCKL4, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_BLOCK_SUCKL5,  -1+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_BLOCK_SUCKL6,  -1+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_BLOCK_SUCKL6,  -1+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_BLOCK_SUCKL5,  -1+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_BLOCK_SUCKL4, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_BLOCK_SUCKL3, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_BLOCK_SUCKL2, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_BLOCK_SUCKL1, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y}
	};
//------------------------------------------------------------------------------
// Sucking Roll frames
//------------------------------------------------------------------------------
static int suck_block_roll[4] =
	{
		SPR_BLOCK_1,SPR_BLOCK_2,SPR_BLOCK_3,SPR_BLOCK_2
	};

//------------------------------------------------------------------------------
// Sucking general frames
//------------------------------------------------------------------------------
static SUCKFRAMES suck_block_frames=
	{
		SPR_BLOCK_1,SPR_BLOCK_1,
		suck_block_roll, 4,
		suck_block_left, suck_block_right
	};
//------------------------------------------------------------------------------
// Object Details
//------------------------------------------------------------------------------
static OBJSIZE block_size = {3,8,26,23};
static OBJMOVE block_move = {195,122,9*256,8*256,545,0x8*256,0x8*256};

#define NUM_WINKS	16
static int wink_frames[NUM_WINKS] =
	{
		SPR_BLOCK_1,SPR_BLOCK_2,SPR_BLOCK_2,SPR_BLOCK_2,SPR_BLOCK_2,
		SPR_BLOCK_3,SPR_BLOCK_3,SPR_BLOCK_3,SPR_BLOCK_3,SPR_BLOCK_2,
		SPR_BLOCK_2,SPR_BLOCK_2,SPR_BLOCK_2,SPR_BLOCK_1,SPR_BLOCK_1,
		SPR_BLOCK_1
	};

//------------------------------------------------------------------------------
//! \brief Initialise the block object
//------------------------------------------------------------------------------
CBlockObj::CBlockObj()
{
	m_pBox = 0;
	Reset();
}

//------------------------------------------------------------------------------
//! \brief Draw the block object
//------------------------------------------------------------------------------
void CBlockObj::Draw( void )
{
	if (RunDrawSuck()) return;
	m_pGame->m_Sprites.Draw( wink_frames[m_WinkOffset], m_XPos, m_YPos );
}

//------------------------------------------------------------------------------
//! \brief Setup the object getting caught
//------------------------------------------------------------------------------
void CBlockObj::SetupCaught( void )
{
	CSuckable::SetupCaught();	// Must call virtual member
	ClearBoxPos();
}

//------------------------------------------------------------------------------
//! \brief Load the object graphics
//------------------------------------------------------------------------------
void CBlockObj::LoadGfx( void )
{
	m_pGame->m_Sprites.LoadRange(SPR_BLOCK_1, SPR_BLOCK_SUCKR6);
}

//------------------------------------------------------------------------------
//! \brief Reset the object (object members)
//------------------------------------------------------------------------------
void CBlockObj::Reset( void )
{
	m_Frame = SPR_BLOCK_1;

	InitMovement(block_move);
	InitSize(block_size);
	m_Y_Flag = -1;

	m_Flags = FLAG_SUCKABLE;

	m_pSuckFrames = &suck_block_frames;
	m_WinkOffset = 0;
	m_WinkInterval = rand() & 255;
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CBlockObj::Do( void )
{
	if (RunDoSuck())
	{
		ClearBoxPos();
		return;
	}

	DoTheAnim();
	MoveObject();
	SetBoxPos();
}

//------------------------------------------------------------------------------
//! \brief Set the collision box position
//------------------------------------------------------------------------------
void CBlockObj::SetBoxPos( void )
{
	// Because - Cannot allocate in the constructor
	if (!m_pBox) m_pBox = m_pGame->m_Map.GetBox();

	if (m_pBox)
	{
		m_pBox->x1 = m_XPos + m_X1;
		m_pBox->y1 = m_YPos + m_Y1;
		m_pBox->x2 = m_XPos + m_X1 + m_X2;
		m_pBox->y2 = m_YPos + m_Y1 + m_Y2;
		m_pBox->type = 0;	// Hard Wall
	}
}

//------------------------------------------------------------------------------
//! \brief Clear the collision box position
//------------------------------------------------------------------------------
void CBlockObj::ClearBoxPos( void )
{
	if (m_pBox)
	{
		m_pBox->x1 = 0;
		m_pBox->y1 = 0;
		m_pBox->x2 = 0;
		m_pBox->y2 = 0;
	}
}

//------------------------------------------------------------------------------
//! \brief Do the block's animation
//------------------------------------------------------------------------------
void CBlockObj::DoTheAnim( void )
{
	if (m_WinkInterval < 0)
	{
		m_WinkOffset++;
		if (m_WinkOffset>=NUM_WINKS)
		{
			m_WinkOffset = 0;
			m_WinkInterval = rand() & 255;
		}
	}else
	{
		m_WinkInterval--;
	}
}

//------------------------------------------------------------------------------
//! \brief Set the block to explode
//------------------------------------------------------------------------------
void CBlockObj::SetExplode( void )
{
	m_SuckCmd = SUCK_NONE;
	m_Flags = FLAG_SUCKABLE;
}

//------------------------------------------------------------------------------
// Sucking in frames
//------------------------------------------------------------------------------

static SUCKANIM suck_spring_right[NUMSUCKANIM] =
	{
		{SPR_SPRING_SUCKR1, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_SPRING_SUCKR2, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_SPRING_SUCKR3, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_SPRING_SUCKR4, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_SPRING_SUCKR5, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_SPRING_SUCKR6, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_SPRING_SUCKR6, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_SPRING_SUCKR6, -6+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_SPRING_SUCKR4, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_SPRING_SUCKR3, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_SPRING_SUCKR2, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y},
		{SPR_SPRING_SUCKR1, 26+ SUCKFIX_LX, -8+	SUCKFIX_Y}
	};
static SUCKANIM suck_spring_left[NUMSUCKANIM] =
	{
		{SPR_SPRING_SUCKL1, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_SPRING_SUCKL2, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_SPRING_SUCKL3, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_SPRING_SUCKL4, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_SPRING_SUCKL5,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_SPRING_SUCKL6,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_SPRING_SUCKL6,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_SPRING_SUCKL5,   0+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_SPRING_SUCKL4, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_SPRING_SUCKL3, -16+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_SPRING_SUCKL2, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y},
		{SPR_SPRING_SUCKL1, -32+ SUCKFIX_RX, -8+	SUCKFIX_Y}
	};
//------------------------------------------------------------------------------
// Sucking Roll frames
//------------------------------------------------------------------------------
static int suck_spring_roll[1] =
	{
		SPR_SPRING_1
	};

//------------------------------------------------------------------------------
// Sucking general frames
//------------------------------------------------------------------------------
static SUCKFRAMES suck_spring_frames=
	{
		SPR_SPRING_1,SPR_SPRING_1,
		suck_spring_roll, 1,
		suck_spring_left, suck_spring_right
	};
//------------------------------------------------------------------------------
// Object Details
//------------------------------------------------------------------------------
static OBJSIZE spring_size = {3,0,26,15};
static OBJSIZE spring_size2 = {3,-8,26,12};	// Player springboard
static OBJMOVE spring_move = {195,122,9*256,8*256,545,0x8*256,0x8*256};

static int anm_spring[] =
	{
	SPR_SPRING_1,
	ANM_RESTART,
	SPR_SPRING_2,
	SPR_SPRING_3,
	SPR_SPRING_3,
	SPR_SPRING_2,
	ANM_RESTART
	};

//------------------------------------------------------------------------------
//! \brief Initialise the spring object
//------------------------------------------------------------------------------
CSpringObj::CSpringObj()
{
	m_pBox = 0;
	m_pSequence = anm_spring;
	Reset();
}

//------------------------------------------------------------------------------
//! \brief Draw the spring object
//------------------------------------------------------------------------------
void CSpringObj::Draw( void )
{
	if (RunDrawSuck()) return;
	m_pGame->m_Sprites.Draw( m_Frame, m_XPos, m_YPos );
}


//------------------------------------------------------------------------------
//! \brief Load the object graphics
//------------------------------------------------------------------------------
void CSpringObj::LoadGfx( void )
{
	m_pGame->m_Sprites.LoadRange(SPR_SPRING_1, SPR_SPRING_SUCKR6);
}

//------------------------------------------------------------------------------
//! \brief Reset the object (object members)
//------------------------------------------------------------------------------
void CSpringObj::Reset( void )
{
	m_Frame = SPR_SPRING_1;

	InitMovement(spring_move);
	InitSize(spring_size);
	m_Y_Flag = -1;

	m_Flags = FLAG_SUCKABLE;

	m_pSuckFrames = &suck_spring_frames;
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CSpringObj::Do( void )
{
	if (RunDoSuck())
	{
		ClearBoxPos();
		return;
	}

	CheckBouncePlayer();
	Animate(256);
	InitSize(spring_size);
	MoveObject();
	SetBoxPos();
}

//------------------------------------------------------------------------------
//! \brief Set the collision box position
//------------------------------------------------------------------------------
void CSpringObj::SetBoxPos( void )
{
	// Because - Cannot allocate in the constructor
	if (!m_pBox) m_pBox = m_pGame->m_Map.GetBox();

	if (m_pBox)
	{
		m_pBox->x1 = m_XPos + m_X1;
		m_pBox->y1 = m_YPos + m_Y1;
		m_pBox->x2 = m_XPos + m_X1 + m_X2;
		m_pBox->y2 = m_YPos + m_Y1 + m_Y2;
		m_pBox->type = 1;	// Soft Wall
	}
}

//------------------------------------------------------------------------------
//! \brief Clear the collision box position
//------------------------------------------------------------------------------
void CSpringObj::ClearBoxPos( void )
{
	if (m_pBox)
	{
		m_pBox->x1 = 0;
		m_pBox->y1 = 0;
		m_pBox->x2 = 0;
		m_pBox->y2 = 0;
	}
}

//------------------------------------------------------------------------------
//! \brief Set the spring to explode
//------------------------------------------------------------------------------
void CSpringObj::SetExplode( void )
{
	m_SuckCmd = SUCK_NONE;
	m_Flags = FLAG_SUCKABLE;
	m_Seq_Offset = 0;
	m_Frame = SPR_SPRING_1;
}

//------------------------------------------------------------------------------
//! \brief Setup the object getting caught
//------------------------------------------------------------------------------
void CSpringObj::SetupCaught( void )
{
	CSuckable::SetupCaught();	// Must call virtual member
	ClearBoxPos();
}

//------------------------------------------------------------------------------
//! \brief Check to see if player will bounce on this object
//------------------------------------------------------------------------------
void CSpringObj::CheckBouncePlayer( void )
{
	CPlayerObj *pobj;

	if (m_Seq_Offset<2)			// Springing?
	{
	
		pobj = (CPlayerObj *) m_pGame->m_PlayerList.m_pFirst;
		while (pobj)
		{
			if (pobj->m_YInert >64)	// Must be falling
			{
				InitSize(spring_size2);
				if (CheckHit( pobj ))
				{
					pobj->m_YInert = -15 * 256;
					m_Seq_Offset = 2;
					m_Frame = SPR_SPRING_2;
					m_YInert = -14 * 256;	// Leap spring
					break;
				}
			}
			pobj = (CPlayerObj *) pobj->m_pNext;
		}
	}

}

static OBJSIZE skittle_size = {0,0,16,44};
static OBJMOVE skittle_move = {195,122,9*256,8*256,545,0x8*256,0x8*256};

//------------------------------------------------------------------------------
//! \brief Initialise the skittle object
//------------------------------------------------------------------------------
CSkittleObj::CSkittleObj()
{
	m_Frame = SPR_BOWLING_SKITTLE;
	InitMovement(skittle_move);
	InitSize(skittle_size);
	m_Flags = FLAG_BADDIE;
	m_YInert = -15*256;
	m_BounceCnt = rand()&63;
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CSkittleObj::Do( void )
{
	MoveObject();
	if (CheckHitFlag( m_pGame->m_GasList, FLAG_WEAPON ))
	{
		SetExplode();
		return;
	}
	if (CheckExtra()) return;
	if (!(m_Y_Flag))	// Only move if on ground
	{
		if ((m_BounceCnt--)<0)
		{
			m_YInert = -6*256;
			m_BounceCnt = 64;
		}
	}
}

static OBJSIZE chickenegg_size = {4,9,23,29};
static OBJMOVE chickenegg_move = {195,122,9*256,8*256,545,0x8*256,0x8*256};

//------------------------------------------------------------------------------
//! \brief Initialise the chickenegg object
//------------------------------------------------------------------------------
CChickenEggObj::CChickenEggObj()
{
	m_Frame = SPR_POWER_CHICKENEGG;
	InitMovement(chickenegg_move);
	InitSize(chickenegg_size);
	m_Flags = FLAG_BADDIE;
	m_YInert = -8*256;
	m_BounceCnt = rand()&63;
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CChickenEggObj::Do( void )
{
	MoveObject();
	if (CheckHitFlag( m_pGame->m_PlayerList, FLAG_ACCURATE ))	// Ie Player
	{
		SetExplode();
		return;
	}
	if (CheckExtra()) return;
	if (!(m_Y_Flag))	// Only move if on ground
	{
		if ((m_BounceCnt--)<0)
		{
			m_YInert = -6*256;
			m_BounceCnt = 64;
		}
	}

}

static OBJSIZE carrydoor_size = {0,0,32,48};

//------------------------------------------------------------------------------
//! \brief Initialise the carrydoor object
//------------------------------------------------------------------------------
CCarryDoorObj::CCarryDoorObj()
{
	m_Frame = 0;
	InitSize(carrydoor_size);
	m_Pause = 120;
	m_LifeTime = 100;
}

//------------------------------------------------------------------------------
//! \brief Load the carrydoor gfx
//------------------------------------------------------------------------------
void CCarryDoorObj::LoadGfx(void)
{
	m_pGame->m_Sprites.LoadRange(SPR_DOOR_FULL, SPR_DOOR_OUTLINE );
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CCarryDoorObj::Do( void )
{

	CPlayerObj *pobj;

	// Delay before the door appears
	if (m_Pause>0)
	{
		m_Pause--;
		return;
	}

	pobj = HitPlayer();
	if (pobj)			// Collision occured?
	{
		m_Frame = SPR_DOOR_FULL;
		if (pobj->m_pBaddie)	// Got baddie in gun
		{
			m_pGame->SetBonusLevel(BLEV_CHEESEROOM);
		}
	}else
	{
		m_Frame = SPR_DOOR_OUTLINE;
	}
	if ((m_LifeTime--)<0)
	{
		m_pGame->CreateCloud( m_XPos, m_YPos );
		DeleteObject();
	}
}

static OBJSIZE bigcheese_size = {0,0,48,22};

//------------------------------------------------------------------------------
//! \brief Initialise the bigcheese object
//------------------------------------------------------------------------------
CBigCheeseObj::CBigCheeseObj()
{
	InitSize(bigcheese_size);
	InitMovement(chickenegg_move);
	m_MainCheeseFlag = 0;
	m_CheeseCounter = 250;
	m_Frame = SPR_TREAS_BIGCHEESE1;
}

//------------------------------------------------------------------------------
//! \brief Load the bigcheese gfx
//------------------------------------------------------------------------------
void CBigCheeseObj::LoadGfx(void)
{
	m_pGame->m_Sprites.Load(SPR_TREAS_CHEESE);
	m_pGame->m_Sprites.LoadRange(SPR_TREAS_BIGCHEESE1, SPR_TREAS_CHEESEB3);
}

//------------------------------------------------------------------------------
//! \brief Draw the goodie object
//------------------------------------------------------------------------------
void CBigCheeseObj::Draw( void )
{
	if (m_Frame)
	{
		m_pGame->m_Sprites.Draw( m_Frame, m_XPos, m_YPos );
		m_pGame->m_Sprites.Draw( SPR_TREAS_BIGCHEESE2, m_XPos+32, m_YPos );
	}
}

//------------------------------------------------------------------------------
//! \brief Explode the big cheese into smaller cheeses
//------------------------------------------------------------------------------
void CBigCheeseObj::ExplodeCheese(void)
{
	int cnt;
	for (cnt=0;cnt<40;cnt++)
	{
		m_pGame->RandGoodie( m_XPos, m_YPos, GOODIE_CHEESE, cnt&1 );
	}
}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CBigCheeseObj::Do( void )
{

	CPlayerObj *pobj;

	if (m_Frame)	// Not Collected?
	{
		MoveObject();
		pobj = HitPlayer();
		if (pobj)			// Collision occured?
		{
			m_Frame = 0;
			m_pGame->CreateCloud( m_XPos, m_YPos );
			ExplodeCheese();
		}
	}

	if (m_MainCheeseFlag)
	{
		if (!(--m_CheeseCounter))	// Time up?
		{
			m_pGame->m_BonusCompleteFlag = 1;
		}
		m_pGame->m_CountDown = m_CheeseCounter>>2;
	}
}

static OBJSIZE chest_size = {20,0,8,31};

//------------------------------------------------------------------------------
//! \brief Initialise the chest object
//------------------------------------------------------------------------------
CChestObj::CChestObj()
{
	InitSize(chest_size);
	InitMovement(chickenegg_move);
	m_BonusRoomFlag = 0;
	m_ReleaseType = GOODIE_TOYS;
	m_LiftPos = 0;
	m_ToysCnt = 80;
	m_LevelLength = 350;
}

//------------------------------------------------------------------------------
//! \brief Load the chest gfx
//------------------------------------------------------------------------------
void CChestObj::LoadGfx(void)
{
	m_pGame->m_Sprites.LoadRange(SPR_TREAS_CHEST1L, SPR_TREAS_CHEST4R);
	switch (m_ReleaseType)
	{
		case GOODIE_COIN:
			m_pGame->m_Sprites.LoadRange(SPR_TREAS_MONEY1, SPR_TREAS_MONEY6);
			break;
		case GOODIE_JEWEL:
			m_pGame->m_Sprites.LoadRange(SPR_TREAS_GEMSY1, SPR_TREAS_GEMSR3);
			break;
		case GOODIE_TRES:
			m_pGame->m_Sprites.LoadRange(SPR_TREAS_GOLD1, SPR_TREAS_GOLD5);
			break;
	}
}

//------------------------------------------------------------------------------
//! \brief Draw the goodie object
//------------------------------------------------------------------------------
void CChestObj::Draw( void )
{
	int frm1,frm2;

	m_pGame->m_Sprites.Draw( SPR_TREAS_CHEST1L, m_XPos, m_YPos );
	m_pGame->m_Sprites.Draw( SPR_TREAS_CHEST1R, m_XPos+32, m_YPos );

	switch (m_LiftPos)
	{
		case 0:
			frm1 = SPR_TREAS_CHEST4L;
			frm2 = SPR_TREAS_CHEST4R;
			break;
		case 1:
		case 2:
		case 3:
			frm1 = SPR_TREAS_CHEST3L;
			frm2 = SPR_TREAS_CHEST3R;
			break;
		default:
			frm1 = SPR_TREAS_CHEST2L;
			frm2 = SPR_TREAS_CHEST2R;
	}
	m_pGame->m_Sprites.Draw( frm1, m_XPos, m_YPos );
	m_pGame->m_Sprites.Draw( frm2, m_XPos+32, m_YPos );

}

//------------------------------------------------------------------------------
//! \brief Do the object
//------------------------------------------------------------------------------
void CChestObj::Do( void )
{
	MoveObject();
	switch (m_LiftPos)
	{
		case 0:		// Closed
			CheckToOpen();
			break;
		case 1:		// Opening
		case 2:		// Opening
		case 3:		// Opening
			m_LiftPos++;
			break;
		default:	// Open
			ReleaseToys();
	}
	CheckEndBonusLevel();
}

//------------------------------------------------------------------------------
//! \brief Release the toys from the chest
//------------------------------------------------------------------------------
void CChestObj::ReleaseToys(void)
{
	if (m_pGame->m_MainCounter&1) return;
	if (m_ToysCnt>0)
	{
		m_ToysCnt--;
		m_pGame->RandGoodie(m_XPos+16, m_YPos-5, m_ReleaseType, m_ToysCnt&1);
	}

}

//------------------------------------------------------------------------------
//! \brief Check to see if a player has hit the chest
//------------------------------------------------------------------------------
void CChestObj::CheckToOpen(void)
{
	CPlayerObj *pobj;

	pobj = HitPlayer();
	if (pobj)			// Collision occured?
	{
		m_LiftPos++;
	}

}

//------------------------------------------------------------------------------
//! \brief Check to see if the bonus level has finished
//------------------------------------------------------------------------------
void CChestObj::CheckEndBonusLevel(void)
{
	if (!m_BonusRoomFlag) return;

	m_LevelLength--;
	if (m_LevelLength<0)
	{
		m_pGame->m_BonusCompleteFlag = 1;
	}else
	{
		m_pGame->m_CountDown = m_LevelLength>>4;
	}
}


