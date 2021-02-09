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
// ポーズメニュー
//=============================================================================
typedef enum
{
	SELECT_MENU_GOLEM = 0,	//ゴーレム
	SELECT_MENU_LEO,			//レオ
	SELECT_MENU_STALKER,		//ストーカー
	SELECT_MENU_MAX
}SELECT_MENU;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
HRESULT InitSelect(void);
void UninitSelect(void);
void UpdateSelect(void);
void DrawSelect(void);
void SetTextureSelect(int nCntSelect);

#endif

