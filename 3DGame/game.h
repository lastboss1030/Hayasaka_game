//=============================================================================
//
// ゲームの処理 [game.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

//=============================================================================
// プロトタイプ宣言
//=============================================================================
HRESULT InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetPause(bool bPause);
void SetWall(void);

#endif
