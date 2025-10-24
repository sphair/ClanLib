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
// Methane Brothers Main Game (Header File)
//------------------------------------------------------------------------------

#ifndef _game_h
#define _game_h 1

#include "maps.h"
#include "bititem.h"
#include "bitgroup.h"
#include "objlist.h"
#include "sound.h"

//------------------------------------------------------------------------------
// Game main loop types
//------------------------------------------------------------------------------
#define MC_GAME		0	// Game playing
#define MC_COMPLETED	1	// Game completed
#define MC_TITLE	2	// On the title screen
#define MC_HIGHSCREEN	3	// On the high score table screen
#define MC_GETPLAYER	4	// On the get player name screen

//------------------------------------------------------------------------------
// Fade types
//------------------------------------------------------------------------------
#define FADE_COMPLETE	0	// Fade has completed
#define FADE_NORMAL	1	// Fade to the normal palette
#define FADE_BLACK	2	// Fade to the black palette
#define FADE_WHITE	3	// Fade to the white palette
#define FADE_LEVELFADE	4	// Fade to black then to white
#define FADE_TITLESCREEN 6	// Fade to the title screeen

#define FADE_FLAG_WAIT	0	// Game loop waits until fade complete
#define FADE_FLAG_ONCE	1	// Game loop only runs once
#define FADE_FLAG_CONTINUE 2	// Game continues during fading

typedef struct _PUPTYPE
	{
		int type;
		int xpos;
		int ypos;
	} PUPTYPE;
#define MAX_PUP	8	// Maximum number of powerups in a FRK file

typedef struct _HISCORES
	{
		int		score;
		char	name[4];	// Note: NOT zero terminated
	} HISCORES;
#define MAX_HISCORES	8

typedef struct _PARTYOFFS PARTYOFFS;
typedef struct _ENDGROUP ENDGROUP;
typedef struct _PLAYER_STATUS PLAYER_STATUS;
class CGameTarget;
class CPlayerObj;
class CGame
{
public:
	CGame();
	void Init(CGameTarget *tptr, JOYSTICK *jptr1, JOYSTICK *jptr2);
	void StartGame(void);
	void MainLoop(void);
	void StartFRKObject(int type, int xpos, int ypos);
	void MakeNumRise(int xpos, int ypos, int frame);
	void LoadGoodieGfx( void );
	void RandGoodie(int xpos, int ypos, int gtype, int xinert, int yinert);
	void RandGoodie(int xpos, int ypos, int gtype, int dir);
	void MakeGoodie(int xpos, int ypos, int gtype, int gid, int xinert, int yinert);
	void SetJumpExplode(int xpos, int ypos, int dir);
	CPlayerObj *GetPlayer( int player_object_id );
	void InitSpriteList(void);
	int CreateMessage(int yoffset, const char *txt, int delay);
	void SetLevelName( int id );
	void SetBossDie( int xpos, int ypos );
	void CreateCloud( int xpos, int ypos );
	void SetAngryBaddies(int flag);
	void InitFrkObject(CLinkObject *ptr, int type, int xpos, int ypos, CObjectList *objlist);
	void SetBonusLevel( int lvl_id );
	int CountBaddies(void);
	void InitGameOver(void);
	void InitHighScreen(void);
	HISCORES *InsertHiScore(int score, char *name);
	void TogglePuffBlow(void);

private:
	int Fade( float desired_light, int speed );
	void CheckForEgg(void);
	void NextLevel(void);
	void EnterBonusLevel(void);
	void CreateSmallJump(int xpos, int ypos, int xinert, int yinert);
	void DrawPlayerInfo( CPlayerObj *pobj, PLAYER_STATUS *play );
	void DrawPlayersInfo(void);
	void CheckComplete(void);
	int IsComplete(void);
	void CheckDooDahDay(void);
	void MakePowerUp(int type, int xpos, int ypos);
	void UsePowerUp(void);
	void InitPowerUp(void);
	void CheckExtras(void);
	void CheckExtras2(void);
	void SetTreasure(int xpos, int ypos, int rtype);
	void PlayModule(int id);
	void GameLoop(void);
	int ControlFade(void);
	void CompletedLoop(void);
	void CreateBalloons(void);
	void DrawEndCredits(void);
	void DrawEndGfxItems(int xpos, int ypos, PARTYOFFS **party);
	void InitNewGame(int player_two_flag);
	void InitTitleScreen(void);
	void TitleScreenLoop(void);
	void DoGameOverLoop(void);
	void InitNewGame(void);
	void HighScreenLoop(void);
	void DrawFont(int ypos, const char *text);
	void DrawScrFont(int ypos, const char *text, int xpos = 0);
	void DrawHighTable(void);
	void RedrawScrIfNeeded(void);
	void InitGetPlayerNameScreen(int player_two_flag);
	void GetPlayerNameLoop(void);
	void EditName(JOYSTICK *pjoy, char *nptr);
	void PrepareEditName(void);
	void CheckForGameOver( void );

public:

	CGameTarget	*m_pGameTarget;
	JOYSTICK	*m_pJoy1;
	JOYSTICK	*m_pJoy2;
	CSoundDrv	*m_pSound;
	CSoundDrv	m_Sound;

	CMap		m_Map;
	CBitmapGroup	m_Sprites;

	CObjectList	m_PlayerList;
	CObjectList	m_DeadPlayerList;
	CObjectList	m_GasList;
	CObjectList	m_BaddieList;
	CObjectList	m_GoodieList;
	CObjectList	m_FontList;
	CObjectList	m_ExtraList;

	int	m_MainCounter;
	int	m_LevelNumber;

	int	m_CompleteFlag;
	int	m_CompleteCnt;
	int	m_PanelOrigin;
	int	m_DayDelay;
	int	m_DisableCard;
	int	m_LevelSkip;
	int	m_BonusLevelFlag;
	int	m_BonusCompleteFlag;
	int	m_CountDown;
	int	m_TreasSpotCnt;
	int	m_BossLevel;

	int	m_EggFlag;
	int	m_GoodieCollectCnt;
	int	m_GoodieCollectFlag;

	int	m_FadeFlag;
	int	m_FadeType;

	int	m_TrainCnt;
	int	m_DuckCnt;
	int	m_CarCnt;

	int	m_GameOverFlag;
	HISCORES m_HiScores[MAX_HISCORES];
private:
	int	m_PUP_Cnt;
	PUPTYPE	m_PUP_Data[MAX_PUP];
	int	m_FlowerFlag;
	int	m_BonusLevelID;
	int	m_EnterBonusLevelFlag;
	int	m_CurrentTune;
	int	m_BonusDelay;
	int	m_MainCommand;
	int	m_EndYOffset;

	int	m_HiOffset;
	int	m_ScrChgFlag;
	int	m_TwoPlayerModeFlag;
	char	m_PlayerNameBuff1[8];
	char	m_PlayerNameBuff2[8];
	int	m_EditPlayerOneNameFlag;
	int	m_NameEditFadeUpFlag;

};

#endif

