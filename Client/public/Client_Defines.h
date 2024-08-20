#pragma once

static const unsigned int g_iWinCX = 1280;
static const unsigned int g_iWinCY = 720;

enum LEVEL { LEVEL_STATIC, LEVEL_MAIN, LEVEL_LOGO, LEVEL_LOADING, LEVEL_FIELD, LEVEL_RESPAWN1, LEVEL_RESPAWN2, LEVEL_SLIME, LEVEL_ICEBRAIN, LEVEL_COLOSUSS, LEVEL_EYECUBE, LEVEL_YETI, LEVEL_ENDING, LEVEL_CREDIT, LEVEL_END };
//enum CHANNELID { BGM, PLAYER, EFFECT, UI, SYSTEM_EFFECT, SYSTEM_EFFECT2, SYSTEM_EFFECT3, MAXCHANNEL };

enum BOARD { MAIN_LOBY, MAIN_START, MAIN_OPTION, MAIN_QUIT, MAIN_END };
enum MENUSTATE { MENU_DEFAULT, MENU_START, MENU_OPTION, MENU_EXIT, MENU_YES, MENU_NO, MENU_END };
extern bool		g_bStartgame;

extern HINSTANCE	g_hInst;
extern HWND			g_hWnd;
extern LEVEL		g_eCurrentLevel;
extern LEVEL		g_eNextLevel;
extern bool		g_bGoLevel;
extern bool		g_bSlimeClear;
extern bool		g_bEyeCubeClear;
extern bool		g_bYetiClear;
extern bool		g_bIceBrainClear;
extern bool		g_bColossusClear;
extern bool		g_bCameraDirecting;
extern bool		g_bTimeSlow;
extern int		g_iKill;
extern bool		g_bRespawn1Clear;
extern bool		g_bRespawn2Clear;
namespace Client {}

using namespace Client;

#define PLAYER_MOVESPEED		5.f
#define PLAYER_ROTATIONSPEED	90.f