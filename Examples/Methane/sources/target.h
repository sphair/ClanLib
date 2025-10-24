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
// GameTarget class Header File
//------------------------------------------------------------------------------

#ifndef _target_h
#define _target_h 1

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

#ifdef ENABLE_SOUND
#include <ClanLib/sound.h>
#include <ClanLib/mikmod.h>
#endif // ENABLE_SOUND

#include "game.h"

class CMethDoc;
class CL_DisplayWindow;

class CGameTarget
{
private:
	CMethDoc *m_pDoc;	// The portal to the outside world

public:
	CGameTarget();
	void Init(CMethDoc *pdoc, CL_DisplayWindow &window);
	void InitGame();
	void RedrawScreen(void);
	void StartGame(void);
	void MainLoop(void);
	void PrepareSoundDriver(void);
	void PlayModule(int id);
	void StopModule(void);
	void PlaySample(int id, int pos, int rate);
	void UpdateModule(int id);
	void Draw(int dest_xpos, int dest_ypos, int width, int height, int texture_number, int texture_xpos, int texture_ypos, bool draw_white);

	CGame m_Game;		// The Main Game
	JOYSTICK m_Joy1;	// To be written to by the OS
	JOYSTICK m_Joy2;	// To be written to by the OS
	int m_FadeChangeFlag;	// 0 = Palette has not changed

	float m_Lighting;	// -1 = Black. 0 = Normal. 1 = White

private:
	CL_DisplayWindow *m_pWindow;	//!< The window (always valid)

	static const int m_NumTextures = 5;
	CL_Texture m_Texture[m_NumTextures];

	CL_ProgramObject m_Shader_DrawWhite;
	CL_ProgramObject m_Shader_Standard;

	enum ShaderType
	{
		shader_none,
		shader_drawwhite,
		shader_standard
	};
	ShaderType m_LastShaderType;

#ifdef ENABLE_SOUND
	CL_SoundBuffer m_WAV_blow;
	CL_SoundBuffer m_WAV_bowling;
	CL_SoundBuffer m_WAV_candle;
	CL_SoundBuffer m_WAV_card;
	CL_SoundBuffer m_WAV_car;
	CL_SoundBuffer m_WAV_chicken;
	CL_SoundBuffer m_WAV_cookie;
	CL_SoundBuffer m_WAV_crying;
	CL_SoundBuffer m_WAV_day;
	CL_SoundBuffer m_WAV_die2;
	CL_SoundBuffer m_WAV_duck;
	CL_SoundBuffer m_WAV_feather;
	CL_SoundBuffer m_WAV_finlev1;
	CL_SoundBuffer m_WAV_hurry;
	CL_SoundBuffer m_WAV_marble;
	CL_SoundBuffer m_WAV_mask;
	CL_SoundBuffer m_WAV_moon;
	CL_SoundBuffer m_WAV_oil;
	CL_SoundBuffer m_WAV_pickup1;
	CL_SoundBuffer m_WAV_pstar;
	CL_SoundBuffer m_WAV_redstar;
	CL_SoundBuffer m_WAV_spiningtop;
	CL_SoundBuffer m_WAV_spit;
	CL_SoundBuffer m_WAV_splat;
	CL_SoundBuffer m_WAV_tap;
	CL_SoundBuffer m_WAV_train;
	CL_SoundBuffer m_WAV_tribble;
	CL_SoundBuffer m_WAV_turbo;
	CL_SoundBuffer m_WAV_twinkle;
	CL_SoundBuffer m_WAV_wings;
	CL_SoundBuffer m_WAV_wpotion;
	CL_SoundBuffer m_WAV_xylo;
	CL_SoundBuffer m_MOD_boss;
	CL_SoundBuffer m_MOD_complete;
	CL_SoundBuffer m_MOD_empty;
	CL_SoundBuffer m_MOD_title;
	CL_SoundBuffer m_MOD_tune1;
	CL_SoundBuffer m_MOD_tune2;

	CL_SoundBuffer_Session m_Session;
#endif
	bool m_bSessionActive;
};

extern CGameTarget *GLOBAL_GameTarget;

#endif


