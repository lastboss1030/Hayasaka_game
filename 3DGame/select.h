//=============================================================================
//
// 選択画面の処理 [select.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#ifndef _SELECTL_H_
#define _SELECTL_H_

#include "main.h"

//=============================================================================
// プロトタイプ宣言
//=============================================================================
HRESULT InitSelect(void);
void UninitSelect(void);
void UpdateSelect(void);
void DrawSelect(void);
void SetTextureSelect(int nCntSelect);

#endif

