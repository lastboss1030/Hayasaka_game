//=============================================================================
//
// クロスヘアの処理 [closshair.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#ifndef _CLOSSHAIR_H_
#define _CLOSSHAIR_H_

#include "main.h"

//=============================================================================
// プロトタイプ宣言
//=============================================================================
HRESULT InitClosshair(void);
void UninitClosshair(void);
void UpdateClosshair(void);
void DrawClosshair(void);
void SetTextureClosshair(int nCntClosshair);

#endif


