//=============================================================================
//
// クールダウンUIの処理 [cooldown_UI.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#ifndef _COOLDOWN_UI_H_
#define _COOLDOWN_UI_H_

#include "main.h"

//=============================================================================
// プロトタイプ宣言
//=============================================================================
HRESULT InitCooldown(void);
void UninitCooldown(void);
void UpdateCooldown(void);
void DrawCooldown(void);
void SetTextureCooldown(int nCntCooldown);

#endif

