//=============================================================================
//
// タイトルの処理 [title.h]
// Author:Taiki Hayasaka
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"

//=============================================================================
// プロトタイプ宣言
//=============================================================================
HRESULT InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);
void SetTextureTitle(int nCntTitle);

#endif
