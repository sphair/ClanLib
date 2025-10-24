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
// Methane Brothers Global Header File)
//------------------------------------------------------------------------------

#ifndef _global_h
#define _global_h 1

#ifndef _METH_RC

#endif // _METH_RC

// Standard Screen Size
#define SCR_WIDTH	320
#define SCR_HEIGHT	256

// Standard Map Size
#define MAP_WIDTH	(SCR_WIDTH/16)
#define MAP_HEIGHT	(SCR_HEIGHT/16)
#define MAP_SIZE	(MAP_WIDTH * MAP_HEIGHT)

// Resource Defines
#define IDR_MAINFRAME		100
#define CM_MAINITEM1		18873
#define ID_APP_HELP		11843
#define CM_APPHELP		11842
#define CM_HELPITEM1		18873
#define IDD_ABOUTDLG		110
#define IDC_ICONRECT		111
#define IDC_STATICFRAME1	101
#define IDC_STATICFRAME2	102
#define IDC_STATICTEXT1		103
#define IDC_STATICTEXT2		104
#define IDC_STATICTEXT3		105
#define IDC_STATICTEXT4		106
#define IDC_STATICTEXT5		107
#define IDC_STATICTEXT6		108
#define IDC_STATICTEXT7		109
#define IDC_STATICTEXT8		110
#define IDC_STATICTEXT9		112
#define IDC_STATICTEXT10	113
#define IDC_STATICTEXT11	114
#define IDC_STATICTEXT12	115
#define IDC_STATICTEXT13	116
#define IDD_HELPDLG		120
#define IDD_GAMESPEED		121
#define ID_APP_PAUSE		301
#define ID_APP_MUSIC		302
#define ID_APP_SOUND		303
#define ID_APP_AUDIO_OFF	304
#define ID_APP_AUDIO_ON		305
#define ID_APP_ZOOM0		306
#define ID_APP_ZOOM1		307
#define ID_APP_ZOOM2		308
#define ID_APP_ZOOM3		309
#define ID_APP_ZOOM4		310
#define ID_APP_32COL		311
#define ID_APP_RVIDEO		312
#define ID_APP_MONO		313
#define ID_APP_RESTART		314
#define ID_APP_GAMESPEED	315
#define IDC_SPEEDVALUE		316
#define ID_APP_TOGGLE		317

#ifndef _METH_RC

// Yuck, fixme please!
#define SMB_NEW(obj_name, obj_type) obj_name = (obj_type *) new (obj_type)

#endif // _METH_RC

// Joystick Structure
typedef struct _JOYSTICK {
	char left;
	char right;
	char up;
	char down;
	char fire;

	char key;
	char next_level;
	} JOYSTICK;

typedef struct _MCOORDS {
	int xoff;
	int yoff;
	int width;
	int height;
	int texture_number;
	int texture_xpos;
	int texture_ypos;
	} MCOORDS;

typedef struct _MAP_RESOURCE_DATA {
	unsigned char *ptr;
	int length;
	} MAP_RESOURCE_DATA;

typedef struct _SAMPLE_RESOURCE_DATA {
	unsigned char *ptr;
	int length;
	int rate;	// SRATE_xxx sample rate
	void *handle;	// Used by the target sound driver
	} SAMPLE_RESOURCE_DATA;

typedef struct _MODULE_RESOURCE_DATA {
	unsigned char *ptr;
	int length;
	void *handle;	// Used by the target sound driver
	} MODULE_RESOURCE_DATA;

// Global Functions
void CheckPos(int &xpos, int &ypos);

#endif


