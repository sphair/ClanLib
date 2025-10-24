/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Mark Page
*/

#include "font.h"

CL_Font_Texture_Position TextureFont_Positions[] =
{
	{' ',0,0,0,0,0, 0 - TextureFont_Height, 16,0},
	{'A',1,1,29,32,0, 1 - 1 - TextureFont_Height, 29, 0},
	{'B',35,1,26,32,0,  1 - 1 - TextureFont_Height, 26, 0},
	{'C',66,1,28,32,0, 1 - 1 - TextureFont_Height, 28, 0},
	{'D',100,1,27,32,0, 1 - 1 - TextureFont_Height, 27, 0},
	{'E',133,1,24,32,0, 1 - 1 - TextureFont_Height, 24, 0},
	{'F',163,1,22,32,0, 1 - 1 - TextureFont_Height, 22, 0},
	{'G',190,1,29,32,0, 1 - 1 - TextureFont_Height, 29, 0},
	{'H',226,1,25,32,0, 1 - 1 - TextureFont_Height, 25, 0},
	{'I',257,1,9,32,0, 1 - 1 - TextureFont_Height, 9, 0},
	{'J',270,1,21,32,0, 1 - 1 - TextureFont_Height, 21, 0},
	{'K',298,1,28,32,0, 1 - 1 - TextureFont_Height, 28, 0},
	{'L',330,1,23,32,0, 1 - 1 - TextureFont_Height, 23, 0},
	{'M',358,1,30,32,0, 1 - 1 - TextureFont_Height, 30, 0},
	{'N',396,1,25,32,0, 1 - 1 - TextureFont_Height, 25, 0},
	{'O',427,1,29,32,0, 1 - 1 - TextureFont_Height, 29, 0},
	{'P',462,1,25,32,0, 1 - 1 - TextureFont_Height, 25, 0},
	{'Q',492,1,30,34,0, 1 - 1 - TextureFont_Height, 30, 0},
	{'R',528,1,27,32,0, 1 - 1 - TextureFont_Height, 27, 0},
	{'S',558,1,27,32,0, 1 - 1 - TextureFont_Height, 27, 0},
	{'T',589,1,24,32,0, 1 - 1 - TextureFont_Height, 24, 0},
	{'U',619,1,25,32,0, 1 - 1 - TextureFont_Height, 25, 0},
	{'V',648,1,30,32,0, 1 - 1 - TextureFont_Height, 30, 0},
	{'W',679,1,41,32,0, 1 - 1 - TextureFont_Height, 41, 0},
	{'X',722,1,29,32,0, 1 - 1 - TextureFont_Height, 29, 0},
	{'Y',752,1,29,32,0, 1 - 1 - TextureFont_Height, 29, 0},
	{'Z',782,1,25,32,0, 1 - 1 - TextureFont_Height, 25, 0},
	{'a',3,45,22,24,0, 45 - 37 - TextureFont_Height, 22, 0},
	{'b',31,37,22,32,0, 37 - 37 - TextureFont_Height, 22, 0},
	{'c',56,45,23,24,0, 45 - 37 - TextureFont_Height, 23, 0},
	{'d',82,37,23,32,0, 37 - 37 - TextureFont_Height, 23, 0},
	{'e',110,45,22,24,0, 45 - 37 - TextureFont_Height, 22, 0},
	{'f',134,37,17,32,0, 37 - 37 - TextureFont_Height, 17, 0},
	{'g',152,45,23,32,0, 45 - 37 - TextureFont_Height, 23, 0},
	{'h',181,37,21,32,0, 37 - 37 - TextureFont_Height, 21, 0},
	{'i',208,37,8,32,0, 37 - 37 - TextureFont_Height, 8, 0},
	{'j',218,37,13,40,0, 37 - 37 - TextureFont_Height, 13, 0},
	{'k',238,37,21,32,0, 37 - 37 - TextureFont_Height, 21, 0},
	{'l',263,37,9,32,0, 37 - 37 - TextureFont_Height, 9, 0},
	{'m',277,45,35,24,0, 45 - 37 - TextureFont_Height, 35, 0},
	{'n',317,45,21,24,0, 45 - 37 - TextureFont_Height, 21, 0},
	{'o',343,45,24,24,0, 45 - 37 - TextureFont_Height, 24, 0},
	{'p',372,45,23,32,0, 45 - 37 - TextureFont_Height, 23, 0},
	{'q',398,45,22,32,0, 45 - 37 - TextureFont_Height, 22, 0},
	{'r',428,45,16,24,0, 45 - 37 - TextureFont_Height, 16, 0},
	{'s',445,45,22,24,0, 45 - 37 - TextureFont_Height, 22, 0},
	{'t',470,38,15,31,0, 38 - 37 - TextureFont_Height, 15, 0},
	{'u',489,45,21,24,0, 45 - 37 - TextureFont_Height, 21, 0},
	{'v',512,40,24,30,0, 40 - 37 - TextureFont_Height, 24, 0},
	{'w',538,45,33,24,0, 45 - 37 - TextureFont_Height, 33, 0},
	{'x',572,45,25,24,0, 45 - 37 - TextureFont_Height, 25, 0},
	{'y',598,45,24,32,0, 45 - 37 - TextureFont_Height, 24, 0},
	{'z',624,45,20,24,0, 45 - 37 - TextureFont_Height, 20, 0},
	{'0',1,78,21,32,0, 78 - 78 - TextureFont_Height, 21, 0},
	{'1',29,78,15,32,0, 78 - 78 - TextureFont_Height, 15, 0},
	{'2',52,78,22,32,0, 78 - 78 - TextureFont_Height, 22, 0},
	{'3',78,78,22,32,0, 78 - 78 - TextureFont_Height, 22, 0},
	{'4',104,78,22,32,0, 78 - 78 - TextureFont_Height, 22, 0},
	{'5',130,78,22,32,0, 78 - 78 - TextureFont_Height, 22, 0},
	{'6',155,78,22,32,0, 78 - 78 - TextureFont_Height, 22, 0},
	{'7',181,78,21,32,0, 78 - 78 - TextureFont_Height, 21, 0},
	{'8',206,78,22,32,0, 78 - 78 - TextureFont_Height, 22, 0},
	{'9',232,78,22,32,0, 78 - 78 - TextureFont_Height, 22, 0},
	{'!',3,119,8,32,0, 119 - 119 - TextureFont_Height, 8, 0},
	{'"',18,119,17,13,0, 119 - 119 - TextureFont_Height, 17, 0},
	{'£',38,119,24,32,0, 119 - 119 - TextureFont_Height, 24, 0},
	{'$',65,117,22,38,0, 117 - 119 - TextureFont_Height, 22, 0},
	{'%',91,119,33,33,0, 119 - 119 - TextureFont_Height, 33, 0},
	{'^',129,119,21,18,0, 119 - 119 - TextureFont_Height, 21, 0},
	{'&',155,119,29,32,0, 119 - 119 - TextureFont_Height, 29, 0},
	{'*',187,119,15,16,0, 119 - 119 - TextureFont_Height, 15, 0},
	{'(',208,119,13,40,0, 119 - 119 - TextureFont_Height, 13, 0},
	{')',224,119,13,40,0, 119 - 119 - TextureFont_Height, 13, 0},
	{'-',242,135,13,8,0, 135 - 119 - TextureFont_Height, 13, 0},
	{'_',255,152,26,7,0, 152 - 119 - TextureFont_Height, 26, 0},
	{'+',283,125,22,22,0, 125 - 119 - TextureFont_Height, 22, 0},
	{'=',310,127,22,17,0, 127 - 119 - TextureFont_Height, 22, 0},
	{'[',339,119,12,40,0, 119 - 119 - TextureFont_Height, 12, 0},
	{']',352,119,13,40,0, 119 - 119 - TextureFont_Height, 13, 0},
	{'{',370,119,16,40,0, 119 - 119 - TextureFont_Height, 16, 0},
	{'}',390,119,16,40,0, 119 - 119 - TextureFont_Height, 16, 0},
	{':',412,127,9,24,0, 127 - 119 - TextureFont_Height, 9, 0},
	{';',430,127,8,31,0, 127 - 119 - TextureFont_Height, 8, 0},
	{'\'',446,119,8,13, 119 -0, 119 - TextureFont_Height, 8, 0},
	{'@',458,119,41,41,0, 119 - 119 - TextureFont_Height, 41, 0},
	{'#',501,119,22,32,0, 119 - 119 - TextureFont_Height, 22, 0},
	{'~',527,130,22,11,0, 130 - 119 - TextureFont_Height, 22, 0},
	{'\\',552,119,14,32, 119 -0, 119 - TextureFont_Height, 14, 0},
	{'|',570,119,6,41,0, 119 - 119 - TextureFont_Height, 6, 0},
	{',',585,142,8,16,0, 142 - 119 - TextureFont_Height, 8, 0},
	{'<',597,123,23,25,0, 123 - 119 - TextureFont_Height, 23, 0},
	{'.',626,142,9,9,0, 142 - 119 - TextureFont_Height, 9, 0},
	{'>',629,123,22,25,0, 123 - 119 - TextureFont_Height, 22, 0},
	{'/',664,119,14,32,0, 119 - 119 - TextureFont_Height, 14, 0},
	{'?',681,119,23,32,0, 119 - 119 - TextureFont_Height, 23, 0},
	{0,0,0,0,0,0,0,0,0}	
};
