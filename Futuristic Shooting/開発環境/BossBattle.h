//=============================================================================
//
// ボスバトルの処理 [BossBattle.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#ifndef _BOSSBATTLE_H_
#define _BOSSBATTLE_H_

#include "main.h"

//=============================================================================
// プロトタイプ宣言
//=============================================================================
HRESULT InitBossBattle(void);
void UninitBossBattle(void);
void UpdateBossBattle(void);
void DrawBossBattle(void);

#endif
