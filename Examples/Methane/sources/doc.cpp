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
// Methane Brothers Document (Source File)
//------------------------------------------------------------------------------
#include "doc.h"
#include "target.h"
#include "snddef.h"

#ifdef METHANE_MIKMOD
#include "../mikmod/audiodrv.h"
#endif

//------------------------------------------------------------------------------
//! \brief Initialise Document
//!
//!	\param window = Screen to draw to
//------------------------------------------------------------------------------
CMethDoc::CMethDoc(CL_DisplayWindow &window)
{

#ifdef METHANE_MIKMOD
	SMB_NEW(m_pMikModDrv,CMikModDrv);
#endif
	m_GameTarget.Init(this, window);
}

//------------------------------------------------------------------------------
//! \brief Destroy Document
//------------------------------------------------------------------------------
CMethDoc::~CMethDoc()
{
#ifdef METHANE_MIKMOD
	if (m_pMikModDrv)
	{
		delete(m_pMikModDrv);
		m_pMikModDrv = 0;
	}
#endif
}

//------------------------------------------------------------------------------
//! \brief Initialise the game
//------------------------------------------------------------------------------
void CMethDoc::InitGame(void)
{
	m_GameTarget.InitGame();
	m_GameTarget.PrepareSoundDriver();
}

//------------------------------------------------------------------------------
//! \brief Initialise the sound driver
//------------------------------------------------------------------------------
void CMethDoc::InitSoundDriver(void)
{
#ifdef METHANE_MIKMOD
	m_pMikModDrv->InitDriver();
#endif
}

//------------------------------------------------------------------------------
//! \brief Remove the sound driver
//------------------------------------------------------------------------------
void CMethDoc::RemoveSoundDriver(void)
{
#ifdef METHANE_MIKMOD
	m_pMikModDrv->RemoveDriver();
#endif
}

//------------------------------------------------------------------------------
//! \brief Start the game
//------------------------------------------------------------------------------
void CMethDoc::StartGame(void)
{
	m_GameTarget.StartGame();
}

//------------------------------------------------------------------------------
//! \brief The Game Main Loop 
//!
//! 	\param window = The screen
//------------------------------------------------------------------------------
void CMethDoc::MainLoop()
{
	m_GameTarget.MainLoop();

#ifdef METHANE_MIKMOD
	m_pMikModDrv->Update();
#endif
}

//------------------------------------------------------------------------------
//! \brief Play a sample (called from the game)
//!
//! 	\param id = SND_xxx id
//!	\param pos = Sample Position to use 0 to 255
//!	\param rate = The rate
//------------------------------------------------------------------------------
void CMethDoc::PlaySample(int id, int pos, int rate)
{
#ifdef METHANE_MIKMOD
	m_pMikModDrv->PlaySample(id, pos, rate);
#endif
}

//------------------------------------------------------------------------------
//! \brief Stop the module (called from the game)
//------------------------------------------------------------------------------
void CMethDoc::StopModule(void)
{
#ifdef METHANE_MIKMOD
	m_pMikModDrv->StopModule();
#endif
}

//------------------------------------------------------------------------------
//! \brief Play a module (called from the game)
//!
//! 	\param id = SMOD_xxx id
//------------------------------------------------------------------------------
void CMethDoc::PlayModule(int id)
{
#ifdef METHANE_MIKMOD
	m_pMikModDrv->PlayModule(id);
#endif
}

//------------------------------------------------------------------------------
//! \brief Update the current module (ie restart the module if it has stopped) (called from the game)
//!
//! 	\param id = SMOD_xxx id (The module that should be playing)
//------------------------------------------------------------------------------
void CMethDoc::UpdateModule(int id)
{
#ifdef METHANE_MIKMOD
	m_pMikModDrv->UpdateModule(id);
#endif
}

//------------------------------------------------------------------------------
//! \brief The Display Options screen
//!
//!	\param window = Current window
//!	\param page_no	= Instruction page number
//------------------------------------------------------------------------------
void CMethDoc::DisplayOptions( CL_DisplayWindow &window, CL_Font &font, int page_no )
{

	CL_GraphicContext gc = window.get_gc();

	if (page_no == 0)
	{
		font.draw_text(gc, 32, 2*32, "INSTRUCTIONS:");
		font.draw_text(gc, 32, 4*32, "FIRE TO START");
		font.draw_text(gc, 32, 5*32, "TYPE PLAYER NAMES");
		font.draw_text(gc, 32, 6*32, "TAP FIRE TO GAS");
		font.draw_text(gc, 32, 7*32, "HOLD FIRE TO SUCK");
		font.draw_text(gc, 32, 8*32, "RELEASE FIRE THROW");
		font.draw_text(gc, 32, 9*32, "THROW AT THE WALL");
	}
	if (page_no == 1)
	{
		font.draw_text(gc, 32, 2*32, "PLAYER ONE:");
		font.draw_text(gc, 32, 3*32, "USE CURSOR KEYS");
		font.draw_text(gc, 32, 4*32, "CTRL TO FIRE.");
		font.draw_text(gc, 32, 6*32, "PLAYER TWO:");
		font.draw_text(gc, 32, 7*32, "USE A W S D");
		font.draw_text(gc, 32, 8*32, "SHIFT TO FIRE");

	}
	if (page_no == 2)
	{
		font.draw_text(gc, 32, 2*32, "KEYS:");
		font.draw_text(gc, 32, 4*32, "F9 INCREASE SPEED");
		font.draw_text(gc, 32, 5*32, "F10 DECREASE SPEED");
		font.draw_text(gc, 32, 6*32, "F11 SKIP LEVEL");
		font.draw_text(gc, 32, 7*32, "TAB CHANGE GRAPHIC");
		font.draw_text(gc, 32, 8*32, "ESC EXIT PROGRAM");
	}

	font.draw_text(gc, 32, 12*32, "(PRESS SPACE)");

}

//------------------------------------------------------------------------------
//! \brief Draw the 32 colour font text (for the options screen)
//!
//! 	\param dptr = 32 bit per pixel screen
//!	\param ypos = Font y pos in pixels
//!	\param tptr = The text
//------------------------------------------------------------------------------
void CMethDoc::DrawFont32Text( int *dptr, int ypos, char *tptr)
{
	/*
	int txtlen;
	int xpos;
	int *wptr;
	char letter;
	char *fptr;
	int width;
	int height;
	int offset;
	int cnt;
	int *palette;

	txtlen = ::strlen(tptr);
	
	//xpos = (SCR_WIDTH/2) - (txtlen*(16/2));
	xpos = 0;
	ypos = ypos * SCR_WIDTH;

	dptr = dptr + xpos + ypos;

	palette = SMB_Font32_Palette;
	for (; txtlen>0; txtlen--)
	{
		letter = *(tptr++);
		// Validate the letter
		if ( (letter <32) || (letter > (FONT32_NUM_IN_LIST+32)) ) break;
		letter -=32;
		fptr = Spr_Font32[letter];
		if (fptr)	// Valid font
		{
			wptr = dptr;	// Save the pointer

			// Get sizes
			xpos = *(fptr++);
			ypos = *(fptr++);
			width = *(fptr++);
			height = *(fptr++);

			ypos = ypos * SCR_WIDTH;
			wptr = wptr + xpos + ypos;	// To the correct position

			offset = SCR_WIDTH - width;	// Offset to next line on the screen

			// For all rows in the font
			for (; height > 0; height--)
			{
				for (cnt=0; cnt<width; cnt++)
				{
					*(wptr++) = palette[( *(fptr++) )] | 0xff;
				}
				wptr+= offset;
			}

		}
		dptr += 16;	// To the next font position
	}
*/
}

//------------------------------------------------------------------------------
//! \brief Load the high scores
//------------------------------------------------------------------------------
void CMethDoc::LoadScores(void)
{

}

//------------------------------------------------------------------------------
//! \brief Save the high scores
//------------------------------------------------------------------------------
void CMethDoc::SaveScores(void)
{

}

