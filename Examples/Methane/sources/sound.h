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
// Methane Brothers Sound Control (Header File)
//------------------------------------------------------------------------------

#ifndef _sound_h
#define _sound_h 1

#include "snddef.h"

class CSoundDrv
{
public:
	CSoundDrv();
	~CSoundDrv();
	void Init(void);

	void PlayModule(int id);
	void StopModule(void);

	void Update(void);
	void PlaySample(int id, int pos = (SCR_WIDTH/2), int rate = 0 );
	void PrepareAudio(void);
	void UpdateModule(void);

	int m_CurrentModule;	// SMOD_xxx id (0 = Unknown)

};

#endif

